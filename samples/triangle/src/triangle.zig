const std = @import("std");
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const d3d12 = zwin32.d3d12;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const zd3d12 = @import("zd3d12");
const common = @import("common");
const c = common.c;
const vm = common.vectormath;
const GuiRenderer = common.GuiRenderer;

pub export const D3D12SDKVersion: u32 = 608;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: triangle";
const window_width = 900;
const window_height = 900;

pub fn main() !void {
    common.init();
    defer common.deinit();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    const window = try common.initWindow(allocator, window_name, window_width, window_height);
    defer common.deinitWindow(allocator);

    var gctx = zd3d12.GraphicsContext.init(allocator, window);
    defer gctx.deinit(allocator);

    const pipeline = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
        };
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.DepthStencilState.DepthEnable = w32.FALSE;
        pso_desc.InputLayout = .{
            .pInputElementDescs = &input_layout_desc,
            .NumElements = input_layout_desc.len,
        };
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/triangle.vs.cso",
            content_dir ++ "shaders/triangle.ps.cso",
        );
    };

    const vertex_buffer = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &d3d12.RESOURCE_DESC.initBuffer(3 * @sizeOf(vm.Vec3)),
        .{ .COPY_DEST = true },
        null,
    ) catch |err| hrPanic(err);

    const index_buffer = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &d3d12.RESOURCE_DESC.initBuffer(3 * @sizeOf(u32)),
        .{ .COPY_DEST = true },
        null,
    ) catch |err| hrPanic(err);

    gctx.beginFrame();

    var guir = GuiRenderer.init(arena_allocator, &gctx, 1, content_dir);
    defer guir.deinit(&gctx);

    const upload_verts = gctx.allocateUploadBufferRegion(vm.Vec3, 3);
    upload_verts.cpu_slice[0] = vm.Vec3.init(-0.7, -0.7, 0.0);
    upload_verts.cpu_slice[1] = vm.Vec3.init(0.0, 0.7, 0.0);
    upload_verts.cpu_slice[2] = vm.Vec3.init(0.7, -0.7, 0.0);

    gctx.cmdlist.CopyBufferRegion(
        gctx.lookupResource(vertex_buffer).?,
        0,
        upload_verts.buffer,
        upload_verts.buffer_offset,
        upload_verts.cpu_slice.len * @sizeOf(vm.Vec3),
    );

    const upload_indices = gctx.allocateUploadBufferRegion(u32, 3);
    upload_indices.cpu_slice[0] = 0;
    upload_indices.cpu_slice[1] = 1;
    upload_indices.cpu_slice[2] = 2;

    gctx.cmdlist.CopyBufferRegion(
        gctx.lookupResource(index_buffer).?,
        0,
        upload_indices.buffer,
        upload_indices.buffer_offset,
        upload_indices.cpu_slice.len * @sizeOf(u32),
    );

    gctx.addTransitionBarrier(vertex_buffer, .{ .VERTEX_AND_CONSTANT_BUFFER = true });
    gctx.addTransitionBarrier(index_buffer, .{ .INDEX_BUFFER = true });
    gctx.flushResourceBarriers();

    gctx.endFrame();
    gctx.finishGpuCommands();

    var triangle_color = vm.Vec3.init(0.0, 1.0, 0.0);

    var stats = common.FrameStats.init();

    while (common.handleWindowEvents()) {
        stats.update(window, window_name);
        common.newImGuiFrame(stats.delta_time);

        c.igSetNextWindowPos(.{ .x = 10.0, .y = 10.0 }, c.ImGuiCond_FirstUseEver, .{ .x = 0.0, .y = 0.0 });
        c.igSetNextWindowSize(c.ImVec2{ .x = 600.0, .y = 0.0 }, c.ImGuiCond_FirstUseEver);
        _ = c.igBegin(
            "Demo Settings",
            null,
            c.ImGuiWindowFlags_NoMove | c.ImGuiWindowFlags_NoResize | c.ImGuiWindowFlags_NoSavedSettings,
        );
        _ = c.igColorEdit3("Triangle color", &triangle_color.c, c.ImGuiColorEditFlags_None);
        c.igEnd();

        gctx.beginFrame();

        const back_buffer = gctx.getBackBuffer();

        gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
        gctx.flushResourceBarriers();

        gctx.cmdlist.OMSetRenderTargets(
            1,
            &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
            w32.TRUE,
            null,
        );
        gctx.cmdlist.ClearRenderTargetView(
            back_buffer.descriptor_handle,
            &[4]f32{ 0.2, 0.4, 0.8, 1.0 },
            0,
            null,
        );
        gctx.setCurrentPipeline(pipeline);
        gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
        gctx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
            .BufferLocation = gctx.lookupResource(vertex_buffer).?.GetGPUVirtualAddress(),
            .SizeInBytes = 3 * @sizeOf(vm.Vec3),
            .StrideInBytes = @sizeOf(vm.Vec3),
        }});
        gctx.cmdlist.IASetIndexBuffer(&.{
            .BufferLocation = gctx.lookupResource(index_buffer).?.GetGPUVirtualAddress(),
            .SizeInBytes = 3 * @sizeOf(u32),
            .Format = .R32_UINT,
        });
        gctx.cmdlist.SetGraphicsRoot32BitConstant(
            0,
            c.igColorConvertFloat4ToU32(.{
                .x = triangle_color.c[0],
                .y = triangle_color.c[1],
                .z = triangle_color.c[2],
                .w = 1.0,
            }),
            0,
        );
        gctx.cmdlist.DrawIndexedInstanced(3, 1, 0, 0, 0);

        guir.draw(&gctx);

        gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
        gctx.flushResourceBarriers();

        gctx.endFrame();
    }

    gctx.finishGpuCommands();
}
