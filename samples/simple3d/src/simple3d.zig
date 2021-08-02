const builtin = @import("builtin");
const std = @import("std");
const w = @import("win32");
const gr = @import("graphics");
const lib = @import("library");
usingnamespace @import("vectormath");
const vhr = gr.vhr;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

pub fn main() !void {
    const window_name = "zig-gamedev: simple3d";
    const window_width = 800;
    const window_height = 800;

    _ = w.SetProcessDPIAware();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa.deinit();
        std.debug.assert(leaked == false);
    }
    const allocator = &gpa.allocator;

    const window = try lib.initWindow(window_name, window_width, window_height);
    var grfx = try gr.GraphicsContext.init(window);
    defer grfx.deinit(&gpa.allocator);

    const pipeline = blk: {
        const input_layout_desc = [_]w.D3D12_INPUT_ELEMENT_DESC{
            w.D3D12_INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
        };
        var pso_desc = w.D3D12_GRAPHICS_PIPELINE_STATE_DESC{
            .pRootSignature = null,
            .VS = w.D3D12_SHADER_BYTECODE.initZero(),
            .PS = w.D3D12_SHADER_BYTECODE.initZero(),
            .DS = w.D3D12_SHADER_BYTECODE.initZero(),
            .HS = w.D3D12_SHADER_BYTECODE.initZero(),
            .GS = w.D3D12_SHADER_BYTECODE.initZero(),
            .StreamOutput = w.D3D12_STREAM_OUTPUT_DESC.initZero(),
            .BlendState = w.D3D12_BLEND_DESC.initDefault(),
            .SampleMask = 0xffff_ffff,
            .RasterizerState = w.D3D12_RASTERIZER_DESC.initDefault(),
            .DepthStencilState = blk1: {
                var desc = w.D3D12_DEPTH_STENCIL_DESC.initDefault();
                desc.DepthEnable = w.FALSE;
                break :blk1 desc;
            },
            .InputLayout = .{
                .pInputElementDescs = &input_layout_desc,
                .NumElements = input_layout_desc.len,
            },
            .IBStripCutValue = .DISABLED,
            .PrimitiveTopologyType = .TRIANGLE,
            .NumRenderTargets = 1,
            .RTVFormats = [_]w.DXGI_FORMAT{.R8G8B8A8_UNORM} ++ [_]w.DXGI_FORMAT{.UNKNOWN} ** 7,
            .DSVFormat = .UNKNOWN,
            .SampleDesc = .{ .Count = 1, .Quality = 0 },
            .NodeMask = 0,
            .CachedPSO = w.D3D12_CACHED_PIPELINE_STATE.initZero(),
            .Flags = .{},
        };
        break :blk try grfx.createGraphicsShaderPipeline(
            allocator,
            &pso_desc,
            "content/shaders/simple3d.vs.cso",
            "content/shaders/simple3d.ps.cso",
        );
    };
    defer {
        _ = grfx.releasePipeline(pipeline);
    }

    const vertex_buffer = try grfx.createCommittedResource(
        .DEFAULT,
        .{},
        &w.D3D12_RESOURCE_DESC.initBuffer(3 * @sizeOf(Vec3)),
        .{ .COPY_DEST = true },
        null,
    );
    //const vertex_buffer_srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    //grfx.device.CreateShaderResourceView(
    //   grfx.getResource(vertex_buffer),
    //  &w.D3D12_SHADER_RESOURCE_VIEW_DESC.initTypedBuffer(.R32G32B32_FLOAT, 0, 3),
    // vertex_buffer_srv,
    //);
    defer _ = grfx.releaseResource(vertex_buffer);

    const index_buffer = try grfx.createCommittedResource(
        .DEFAULT,
        .{},
        &w.D3D12_RESOURCE_DESC.initBuffer(3 * @sizeOf(u32)),
        .{ .COPY_DEST = true },
        null,
    );
    //const index_buffer_srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    //grfx.device.CreateShaderResourceView(
    //   grfx.getResource(index_buffer),
    //  &w.D3D12_SHADER_RESOURCE_VIEW_DESC.initTypedBuffer(.R32G32B32_FLOAT, 0, 3),
    // vertex_buffer_srv,
    //);
    defer _ = grfx.releaseResource(index_buffer);

    try grfx.beginFrame();

    const upload_verts = grfx.allocateUploadBufferRegion(Vec3, 3);
    upload_verts.cpu_slice[0] = vec3Init(-0.7, -0.7, 0.0);
    upload_verts.cpu_slice[1] = vec3Init(0.0, 0.7, 0.0);
    upload_verts.cpu_slice[2] = vec3Init(0.7, -0.7, 0.0);

    grfx.cmdlist.CopyBufferRegion(
        grfx.getResource(vertex_buffer),
        0,
        upload_verts.buffer,
        upload_verts.buffer_offset,
        upload_verts.cpu_slice.len * @sizeOf(Vec3),
    );

    const upload_indices = grfx.allocateUploadBufferRegion(u32, 3);
    upload_indices.cpu_slice[0] = 0;
    upload_indices.cpu_slice[1] = 1;
    upload_indices.cpu_slice[2] = 2;

    grfx.cmdlist.CopyBufferRegion(
        grfx.getResource(index_buffer),
        0,
        upload_indices.buffer,
        upload_indices.buffer_offset,
        upload_indices.cpu_slice.len * @sizeOf(u32),
    );

    grfx.addTransitionBarrier(vertex_buffer, .{ .VERTEX_AND_CONSTANT_BUFFER = true });
    grfx.addTransitionBarrier(index_buffer, .{ .INDEX_BUFFER = true });
    grfx.flushResourceBarriers();

    try grfx.flushGpuCommands();
    try grfx.finishGpuCommands();

    var stats = lib.FrameStats.init();

    while (true) {
        var message = std.mem.zeroes(w.user32.MSG);
        if (w.user32.PeekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) > 0) {
            _ = w.user32.DispatchMessageA(&message);
            if (message.message == w.user32.WM_QUIT)
                break;
        } else {
            stats.update();
            {
                var buffer = [_]u8{0} ** 64;
                const text = std.fmt.bufPrint(
                    buffer[0..],
                    "FPS: {d:.1}  CPU time: {d:.3} ms | {s}",
                    .{ stats.fps, stats.average_cpu_time, window_name },
                ) catch unreachable;
                _ = w.SetWindowTextA(window, @ptrCast([*:0]const u8, text.ptr));
            }

            try grfx.beginFrame();

            const back_buffer = grfx.getBackBuffer();

            grfx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
            grfx.flushResourceBarriers();

            grfx.cmdlist.OMSetRenderTargets(
                1,
                &[_]w.D3D12_CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
                w.TRUE,
                null,
            );
            grfx.cmdlist.ClearRenderTargetView(
                back_buffer.descriptor_handle,
                &[4]f32{ 0.2, 0.4, 0.8, 1.0 },
                0,
                null,
            );
            grfx.setCurrentPipeline(pipeline);
            grfx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
            grfx.cmdlist.IASetVertexBuffers(0, 1, &[_]w.D3D12_VERTEX_BUFFER_VIEW{.{
                .BufferLocation = grfx.getResource(vertex_buffer).GetGPUVirtualAddress(),
                .SizeInBytes = 3 * @sizeOf(Vec3),
                .StrideInBytes = @sizeOf(Vec3),
            }});
            grfx.cmdlist.IASetIndexBuffer(&.{
                .BufferLocation = grfx.getResource(index_buffer).GetGPUVirtualAddress(),
                .SizeInBytes = 3 * @sizeOf(u32),
                .Format = .R32_UINT,
            });
            grfx.cmdlist.DrawIndexedInstanced(3, 1, 0, 0, 0);

            grfx.addTransitionBarrier(back_buffer.resource_handle, w.D3D12_RESOURCE_STATE_PRESENT);
            grfx.flushResourceBarriers();

            try grfx.endFrame();
        }
    }

    try grfx.finishGpuCommands();
}
