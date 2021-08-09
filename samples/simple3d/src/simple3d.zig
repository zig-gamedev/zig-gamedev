const builtin = @import("builtin");
const std = @import("std");
const w = @import("win32");
const gr = @import("graphics");
const lib = @import("library");
const c = @import("c");
usingnamespace @import("vectormath");
const vhr = gr.vhr;
const math = std.math;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const DemoState = struct {
    const window_name = "zig-gamedev: simple3d";
    const window_width = 1920;
    const window_height = 1080;

    window: w.HWND,
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,
    pipeline: gr.PipelineHandle,
    vertex_buffer: gr.ResourceHandle,
    index_buffer: gr.ResourceHandle,
    entity_buffer: gr.ResourceHandle,
    entity_buffer_srv: w.D3D12_CPU_DESCRIPTOR_HANDLE,

    fn init(allocator: *std.mem.Allocator) !DemoState {
        _ = c.igCreateContext(null);
        errdefer c.igDestroyContext(null);

        const window = try lib.initWindow(window_name, window_width, window_height);

        var grfx = try gr.GraphicsContext.init(window);
        errdefer grfx.deinit(allocator);

        const pipeline = blk: {
            const input_layout_desc = [_]w.D3D12_INPUT_ELEMENT_DESC{
                w.D3D12_INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32B32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
            };
            var pso_desc = w.D3D12_GRAPHICS_PIPELINE_STATE_DESC.initDefault();
            pso_desc.RasterizerState.CullMode = .NONE;
            pso_desc.DepthStencilState.DepthEnable = w.FALSE;
            pso_desc.InputLayout = .{
                .pInputElementDescs = &input_layout_desc,
                .NumElements = input_layout_desc.len,
            };
            break :blk try grfx.createGraphicsShaderPipeline(
                allocator,
                &pso_desc,
                "content/shaders/simple3d.vs.cso",
                "content/shaders/simple3d.ps.cso",
            );
        };
        errdefer _ = grfx.releasePipeline(pipeline);

        const vertex_buffer = try grfx.createCommittedResource(
            .DEFAULT,
            .{},
            &w.D3D12_RESOURCE_DESC.initBuffer(3 * @sizeOf(Vec3)),
            .{ .COPY_DEST = true },
            null,
        );
        errdefer _ = grfx.releaseResource(vertex_buffer);

        const index_buffer = try grfx.createCommittedResource(
            .DEFAULT,
            .{},
            &w.D3D12_RESOURCE_DESC.initBuffer(3 * @sizeOf(u32)),
            .{ .COPY_DEST = true },
            null,
        );
        errdefer _ = grfx.releaseResource(index_buffer);

        const entity_buffer = try grfx.createCommittedResource(
            .DEFAULT,
            .{},
            &w.D3D12_RESOURCE_DESC.initBuffer(1 * @sizeOf(Mat4)),
            .{ .NON_PIXEL_SHADER_RESOURCE = true },
            null,
        );
        errdefer _ = grfx.releaseResource(entity_buffer);

        const entity_buffer_srv = grfx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
        grfx.device.CreateShaderResourceView(
            grfx.getResource(entity_buffer),
            &w.D3D12_SHADER_RESOURCE_VIEW_DESC.initStructuredBuffer(0, 1, @sizeOf(Mat4)),
            entity_buffer_srv,
        );

        try grfx.beginFrame();

        var gui = try gr.GuiContext.init(allocator, &grfx);
        errdefer gui.deinit(&grfx);

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

        return DemoState{
            .grfx = grfx,
            .gui = gui,
            .window = window,
            .frame_stats = lib.FrameStats.init(),
            .pipeline = pipeline,
            .vertex_buffer = vertex_buffer,
            .index_buffer = index_buffer,
            .entity_buffer = entity_buffer,
            .entity_buffer_srv = entity_buffer_srv,
        };
    }

    fn deinit(demo: *DemoState, allocator: *std.mem.Allocator) void {
        demo.grfx.finishGpuCommands() catch unreachable;
        _ = demo.grfx.releaseResource(demo.vertex_buffer);
        _ = demo.grfx.releaseResource(demo.index_buffer);
        _ = demo.grfx.releaseResource(demo.entity_buffer);
        _ = demo.grfx.releasePipeline(demo.pipeline);
        demo.gui.deinit(&demo.grfx);
        demo.grfx.deinit(allocator);
        c.igDestroyContext(null);
        demo.* = undefined;
    }

    fn update(demo: *DemoState) void {
        {
            var stats = &demo.frame_stats;
            stats.update();
            var buffer = [_]u8{0} ** 64;
            const text = std.fmt.bufPrint(
                buffer[0..],
                "FPS: {d:.1}  CPU time: {d:.3} ms | {s}",
                .{ stats.fps, stats.average_cpu_time, window_name },
            ) catch unreachable;
            _ = w.SetWindowTextA(demo.window, @ptrCast([*:0]const u8, text.ptr));
        }
        gr.GuiContext.update(demo.frame_stats.delta_time);

        c.igShowDemoWindow(null);
    }

    fn draw(demo: *DemoState) !void {
        var grfx = &demo.grfx;
        try grfx.beginFrame();

        grfx.addTransitionBarrier(demo.entity_buffer, .{ .COPY_DEST = true });
        grfx.flushResourceBarriers();

        {
            const object_to_camera = mat4Mul(
                mat4InitRotationY(@floatCast(f32, demo.frame_stats.time)),
                mat4InitLookAt(
                    vec3Init(3.0, 3.0, -3.0),
                    vec3Init(0.0, 0.0, 0.0),
                    vec3Init(0.0, 1.0, 0.0),
                ),
            );
            const upload_entity = grfx.allocateUploadBufferRegion(Mat4, 1);
            upload_entity.cpu_slice[0] = mat4Transpose(
                mat4Mul(
                    object_to_camera,
                    mat4InitPerspectiveFovLh(
                        math.pi / 3.0,
                        @intToFloat(f32, grfx.viewport_width) / @intToFloat(f32, grfx.viewport_height),
                        0.1,
                        100.0,
                    ),
                ),
            );
            grfx.cmdlist.CopyBufferRegion(
                grfx.getResource(demo.entity_buffer),
                0,
                upload_entity.buffer,
                upload_entity.buffer_offset,
                upload_entity.cpu_slice.len * @sizeOf(Mat4),
            );
        }

        const back_buffer = grfx.getBackBuffer();

        grfx.addTransitionBarrier(demo.entity_buffer, .{ .NON_PIXEL_SHADER_RESOURCE = true });
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
        grfx.setCurrentPipeline(demo.pipeline);
        grfx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
        grfx.cmdlist.IASetVertexBuffers(0, 1, &[_]w.D3D12_VERTEX_BUFFER_VIEW{.{
            .BufferLocation = grfx.getResource(demo.vertex_buffer).GetGPUVirtualAddress(),
            .SizeInBytes = 3 * @sizeOf(Vec3),
            .StrideInBytes = @sizeOf(Vec3),
        }});
        grfx.cmdlist.IASetIndexBuffer(&.{
            .BufferLocation = grfx.getResource(demo.index_buffer).GetGPUVirtualAddress(),
            .SizeInBytes = 3 * @sizeOf(u32),
            .Format = .R32_UINT,
        });
        grfx.cmdlist.SetGraphicsRootDescriptorTable(1, grfx.copyDescriptorsToGpuHeap(1, demo.entity_buffer_srv));
        grfx.cmdlist.SetGraphicsRoot32BitConstant(0, 0, 0);
        grfx.cmdlist.DrawIndexedInstanced(3, 1, 0, 0, 0);

        try demo.gui.draw(grfx);

        try grfx.beginDraw2d();
        try grfx.endDraw2d();

        try grfx.endFrame();
    }
};

pub fn main() !void {
    _ = w.SetProcessDPIAware();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa.deinit();
        std.debug.assert(leaked == false);
    }
    const allocator = &gpa.allocator;

    var demo = try DemoState.init(allocator);
    defer demo.deinit(allocator);

    while (true) {
        var message = std.mem.zeroes(w.user32.MSG);
        if (w.user32.PeekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) > 0) {
            _ = w.user32.DispatchMessageA(&message);
            if (message.message == w.user32.WM_QUIT)
                break;
        } else {
            demo.update();
            try demo.draw();
        }
    }
}
