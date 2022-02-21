pub const GuiRenderer = @This();

const std = @import("std");
const assert = std.debug.assert;
const zwin32 = @import("zwin32");
const w = zwin32.base;
const d3d12 = zwin32.d3d12;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const zd3d12 = @import("zd3d12");
const c = @import("common.zig").c;

font: zd3d12.ResourceHandle,
font_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
pipeline: zd3d12.PipelineHandle,
vb: [zd3d12.GraphicsContext.max_num_buffered_frames]zd3d12.ResourceHandle,
ib: [zd3d12.GraphicsContext.max_num_buffered_frames]zd3d12.ResourceHandle,
vb_cpu_addr: [zd3d12.GraphicsContext.max_num_buffered_frames][]align(8) u8,
ib_cpu_addr: [zd3d12.GraphicsContext.max_num_buffered_frames][]align(8) u8,

pub fn init(
    arena: std.mem.Allocator,
    gr: *zd3d12.GraphicsContext,
    num_msaa_samples: u32,
    comptime content_dir: []const u8,
) GuiRenderer {
    assert(gr.is_cmdlist_opened);
    assert(c.igGetCurrentContext() != null);

    const io = c.igGetIO().?;

    _ = c.ImFontAtlas_AddFontFromFileTTF(io.*.Fonts, content_dir ++ "Roboto-Medium.ttf", 25.0, null, null);
    const font_info = blk: {
        var pp: [*c]u8 = undefined;
        var ww: i32 = undefined;
        var hh: i32 = undefined;
        c.ImFontAtlas_GetTexDataAsRGBA32(io.*.Fonts, &pp, &ww, &hh, null);
        break :blk .{
            .pixels = pp[0..@intCast(usize, ww * hh * 4)],
            .width = @intCast(u32, ww),
            .height = @intCast(u32, hh),
        };
    };

    const font = gr.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initTex2d(.R8G8B8A8_UNORM, font_info.width, font_info.height, 1),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    gr.updateTex2dSubresource(font, 0, font_info.pixels, font_info.width * 4);
    gr.addTransitionBarrier(font, d3d12.RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    const font_srv = gr.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    gr.device.CreateShaderResourceView(gr.getResource(font), null, font_srv);

    const pipeline = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Uv", 0, .R32G32_FLOAT, 0, 8, .PER_VERTEX_DATA, 0),
            d3d12.INPUT_ELEMENT_DESC.init("_Color", 0, .R8G8B8A8_UNORM, 0, 16, .PER_VERTEX_DATA, 0),
        };
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RasterizerState.CullMode = .NONE;
        pso_desc.DepthStencilState.DepthEnable = w.FALSE;
        pso_desc.BlendState.RenderTarget[0].BlendEnable = w.TRUE;
        pso_desc.BlendState.RenderTarget[0].SrcBlend = .SRC_ALPHA;
        pso_desc.BlendState.RenderTarget[0].DestBlend = .INV_SRC_ALPHA;
        pso_desc.BlendState.RenderTarget[0].BlendOp = .ADD;
        pso_desc.BlendState.RenderTarget[0].SrcBlendAlpha = .INV_SRC_ALPHA;
        pso_desc.BlendState.RenderTarget[0].DestBlendAlpha = .ZERO;
        pso_desc.BlendState.RenderTarget[0].BlendOpAlpha = .ADD;
        pso_desc.InputLayout = .{
            .pInputElementDescs = &input_layout_desc,
            .NumElements = input_layout_desc.len,
        };
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.SampleDesc = .{ .Count = num_msaa_samples, .Quality = 0 };
        break :blk gr.createGraphicsShaderPipeline(
            arena,
            &pso_desc,
            content_dir ++ "shaders/imgui.vs.cso",
            content_dir ++ "shaders/imgui.ps.cso",
        );
    };
    return GuiRenderer{
        .font = font,
        .font_srv = font_srv,
        .pipeline = pipeline,
        .vb = .{.{ .index = 0, .generation = 0 }} ** zd3d12.GraphicsContext.max_num_buffered_frames,
        .ib = .{.{ .index = 0, .generation = 0 }} ** zd3d12.GraphicsContext.max_num_buffered_frames,
        .vb_cpu_addr = [_][]align(8) u8{&.{}} ** zd3d12.GraphicsContext.max_num_buffered_frames,
        .ib_cpu_addr = [_][]align(8) u8{&.{}} ** zd3d12.GraphicsContext.max_num_buffered_frames,
    };
}

pub fn deinit(gui: *GuiRenderer, gr: *zd3d12.GraphicsContext) void {
    gr.finishGpuCommands();
    _ = gr.releasePipeline(gui.pipeline);
    _ = gr.releaseResource(gui.font);
    for (gui.vb) |vb| _ = gr.releaseResource(vb);
    for (gui.ib) |ib| _ = gr.releaseResource(ib);
    gui.* = undefined;
}

pub fn draw(gui: *GuiRenderer, gr: *zd3d12.GraphicsContext) void {
    assert(gr.is_cmdlist_opened);
    assert(c.igGetCurrentContext() != null);

    c.igRender();
    const draw_data = c.igGetDrawData();
    if (draw_data == null or draw_data.?.*.TotalVtxCount == 0) {
        return;
    }
    const num_vertices = @intCast(u32, draw_data.?.*.TotalVtxCount);
    const num_indices = @intCast(u32, draw_data.?.*.TotalIdxCount);

    var vb = gui.vb[gr.frame_index];
    var ib = gui.ib[gr.frame_index];

    if (gr.getResourceSize(vb) < num_vertices * @sizeOf(c.ImDrawVert)) {
        _ = gr.releaseResource(vb);
        const new_size = (num_vertices + 5_000) * @sizeOf(c.ImDrawVert);
        vb = gr.createCommittedResource(
            .UPLOAD,
            d3d12.HEAP_FLAG_NONE,
            &d3d12.RESOURCE_DESC.initBuffer(new_size),
            d3d12.RESOURCE_STATE_GENERIC_READ,
            null,
        ) catch |err| hrPanic(err);
        gui.vb[gr.frame_index] = vb;
        gui.vb_cpu_addr[gr.frame_index] = blk: {
            var ptr: ?[*]align(8) u8 = null;
            hrPanicOnFail(gr.getResource(vb).Map(
                0,
                &.{ .Begin = 0, .End = 0 },
                @ptrCast(*?*anyopaque, &ptr),
            ));
            break :blk ptr.?[0..new_size];
        };
    }
    if (gr.getResourceSize(ib) < num_indices * @sizeOf(c.ImDrawIdx)) {
        _ = gr.releaseResource(ib);
        const new_size = (num_indices + 10_000) * @sizeOf(c.ImDrawIdx);
        ib = gr.createCommittedResource(
            .UPLOAD,
            d3d12.HEAP_FLAG_NONE,
            &d3d12.RESOURCE_DESC.initBuffer(new_size),
            d3d12.RESOURCE_STATE_GENERIC_READ,
            null,
        ) catch |err| hrPanic(err);
        gui.ib[gr.frame_index] = ib;
        gui.ib_cpu_addr[gr.frame_index] = blk: {
            var ptr: ?[*]align(8) u8 = null;
            hrPanicOnFail(gr.getResource(ib).Map(
                0,
                &.{ .Begin = 0, .End = 0 },
                @ptrCast(*?*anyopaque, &ptr),
            ));
            break :blk ptr.?[0..new_size];
        };
    }
    // Update vertex and index buffers.
    {
        var vb_slice = std.mem.bytesAsSlice(c.ImDrawVert, gui.vb_cpu_addr[gr.frame_index]);
        var ib_slice = std.mem.bytesAsSlice(c.ImDrawIdx, gui.ib_cpu_addr[gr.frame_index]);
        var vb_idx: u32 = 0;
        var ib_idx: u32 = 0;
        var cmdlist_idx: u32 = 0;
        const num_cmdlists = @intCast(u32, draw_data.?.*.CmdListsCount);
        while (cmdlist_idx < num_cmdlists) : (cmdlist_idx += 1) {
            const list = draw_data.?.*.CmdLists[cmdlist_idx];
            const list_vb_size = @intCast(u32, list.*.VtxBuffer.Size);
            const list_ib_size = @intCast(u32, list.*.IdxBuffer.Size);
            std.mem.copy(
                c.ImDrawVert,
                vb_slice[vb_idx .. vb_idx + list_vb_size],
                list.*.VtxBuffer.Data[0..list_vb_size],
            );
            std.mem.copy(
                c.ImDrawIdx,
                ib_slice[ib_idx .. ib_idx + list_ib_size],
                list.*.IdxBuffer.Data[0..list_ib_size],
            );
            vb_idx += list_vb_size;
            ib_idx += list_ib_size;
        }
    }

    const display_x = draw_data.?.*.DisplayPos.x;
    const display_y = draw_data.?.*.DisplayPos.y;
    const display_w = draw_data.?.*.DisplaySize.x;
    const display_h = draw_data.?.*.DisplaySize.y;
    gr.cmdlist.RSSetViewports(1, &[_]d3d12.VIEWPORT{.{
        .TopLeftX = 0.0,
        .TopLeftY = 0.0,
        .Width = display_w,
        .Height = display_h,
        .MinDepth = 0.0,
        .MaxDepth = 1.0,
    }});
    gr.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
    gr.setCurrentPipeline(gui.pipeline);
    {
        const l = draw_data.?.*.DisplayPos.x;
        const r = draw_data.?.*.DisplayPos.x + draw_data.?.*.DisplaySize.x;
        const t = draw_data.?.*.DisplayPos.y;
        const b = draw_data.?.*.DisplayPos.y + draw_data.?.*.DisplaySize.y;

        const mem = gr.allocateUploadMemory([4][4]f32, 1);
        mem.cpu_slice[0] = [4][4]f32{
            [4]f32{ 2.0 / (r - l), 0.0, 0.0, 0.0 },
            [4]f32{ 0.0, 2.0 / (t - b), 0.0, 0.0 },
            [4]f32{ 0.0, 0.0, 0.5, 0.0 },
            [4]f32{ (r + l) / (l - r), (t + b) / (b - t), 0.5, 1.0 },
        };
        gr.cmdlist.SetGraphicsRootConstantBufferView(0, mem.gpu_base);
    }
    gr.cmdlist.SetGraphicsRootDescriptorTable(1, gr.copyDescriptorsToGpuHeap(1, gui.font_srv));
    gr.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
        .BufferLocation = gr.getResource(vb).GetGPUVirtualAddress(),
        .SizeInBytes = num_vertices * @sizeOf(c.ImDrawVert),
        .StrideInBytes = @sizeOf(c.ImDrawVert),
    }});
    gr.cmdlist.IASetIndexBuffer(&.{
        .BufferLocation = gr.getResource(ib).GetGPUVirtualAddress(),
        .SizeInBytes = num_indices * @sizeOf(c.ImDrawIdx),
        .Format = if (@sizeOf(c.ImDrawIdx) == 2) .R16_UINT else .R32_UINT,
    });

    var global_vtx_offset: i32 = 0;
    var global_idx_offset: u32 = 0;

    var cmdlist_idx: u32 = 0;
    const num_cmdlists = @intCast(u32, draw_data.?.*.CmdListsCount);
    while (cmdlist_idx < num_cmdlists) : (cmdlist_idx += 1) {
        const cmdlist = draw_data.?.*.CmdLists[cmdlist_idx];

        var cmd_idx: u32 = 0;
        const num_cmds = cmdlist.*.CmdBuffer.Size;
        while (cmd_idx < num_cmds) : (cmd_idx += 1) {
            const cmd = &cmdlist.*.CmdBuffer.Data[cmd_idx];

            if (cmd.*.UserCallback != null) {
                // TODO(mziulek): Call the callback.
            } else {
                const rect = [1]d3d12.RECT{.{
                    .left = @floatToInt(i32, cmd.*.ClipRect.x - display_x),
                    .top = @floatToInt(i32, cmd.*.ClipRect.y - display_y),
                    .right = @floatToInt(i32, cmd.*.ClipRect.z - display_x),
                    .bottom = @floatToInt(i32, cmd.*.ClipRect.w - display_y),
                }};
                if (rect[0].right > rect[0].left and rect[0].bottom > rect[0].top) {
                    gr.cmdlist.RSSetScissorRects(1, &rect);
                    gr.cmdlist.DrawIndexedInstanced(
                        cmd.*.ElemCount,
                        1,
                        cmd.*.IdxOffset + global_idx_offset,
                        @intCast(i32, cmd.*.VtxOffset) + global_vtx_offset,
                        0,
                    );
                }
            }
        }
        global_idx_offset += @intCast(u32, cmdlist.*.IdxBuffer.Size);
        global_vtx_offset += cmdlist.*.VtxBuffer.Size;
    }
}
