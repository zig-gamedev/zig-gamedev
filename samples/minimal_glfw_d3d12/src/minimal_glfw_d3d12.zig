const std = @import("std");
const glfw = @import("zglfw");

const zwindows = @import("zwindows");
const windows = zwindows.windows;
const d3d12 = zwindows.d3d12;

const zd3d12 = @import("zd3d12");

pub export const D3D12SDKVersion: u32 = 610;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: minimal glfw d3d12";

pub fn main() !void {
    // Change current working directory to where the executable is located for std.fs.cwd()
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        try std.posix.chdir(path);
    }

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    try glfw.init();
    defer glfw.terminate();

    glfw.windowHintTyped(.client_api, .no_api);
    const glfw_window = try glfw.Window.create(1600, 1200, window_name, null);
    defer glfw_window.destroy();

    const window = glfw.getWin32Window(glfw_window) orelse return error.FailedToGetWin32Window;
    var gctx = zd3d12.GraphicsContext.init(.{
        .allocator = allocator,
        .window = window,
    });
    defer gctx.deinit(allocator);

    const pipeline = pipeline: {
        var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
        defer arena_allocator_state.deinit();
        const arena_allocator = arena_allocator_state.allocator();

        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.DepthStencilState.DepthEnable = windows.FALSE;
        pso_desc.InputLayout = d3d12.INPUT_LAYOUT_DESC.init(&.{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
        });
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.VS = d3d12.SHADER_BYTECODE.init(try std.fs.cwd().readFileAlloc(arena_allocator, content_dir ++ "minimal_glfw_d3d12.vs.cso", 256 * 1024));
        pso_desc.PS = d3d12.SHADER_BYTECODE.init(try std.fs.cwd().readFileAlloc(arena_allocator, content_dir ++ "minimal_glfw_d3d12.ps.cso", 256 * 1024));

        break :pipeline gctx.createGraphicsShaderPipeline(&pso_desc);
    };

    const Vertex = extern struct {
        position: [2]f32,
    };
    const gpu_vertices = try gctx.uploadVertices(Vertex, &.{
        .{ .position = [_]f32{ -0.9, -0.9 } },
        .{ .position = [_]f32{ 0.0, 0.9 } },
        .{ .position = [_]f32{ 0.9, -0.9 } },
    });
    const gpu_vertex_indices = try gctx.uploadVertexIndices(u16, &.{ 0, 1, 2 });

    const Input = extern struct {
        mouse_position: [2]f32,
    };
    var input = try gctx.createConstantBuffer(Input);

    var frac: f32 = 0.0;
    var frac_delta: f32 = 0.0001;

    var framebuffer_size = glfw_window.getFramebufferSize();

    while (!glfw_window.shouldClose() and glfw_window.getKey(.escape) != .press) {
        glfw.pollEvents();

        if (glfw_window.getAttribute(.iconified)) {
            // Window is minimized
            const ns_in_ms: u64 = 1_000_000;
            std.time.sleep(10 * ns_in_ms);
            continue;
        }

        {
            const next_framebuffer_size = glfw_window.getFramebufferSize();
            if (!std.meta.eql(framebuffer_size, next_framebuffer_size)) {
                gctx.resize(@intCast(next_framebuffer_size[0]), @intCast(next_framebuffer_size[1]));
            }
            framebuffer_size = next_framebuffer_size;
        }

        {
            gctx.beginFrame();
            defer gctx.endFrame();

            const back_buffer = gctx.getBackBuffer();
            gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
            gctx.flushResourceBarriers();

            gctx.cmdlist.OMSetRenderTargets(
                1,
                &.{back_buffer.descriptor_handle},
                windows.TRUE,
                null,
            );
            gctx.cmdlist.ClearRenderTargetView(
                back_buffer.descriptor_handle,
                &.{ 0.2, frac, 0.8, 1.0 },
                0,
                null,
            );

            gctx.setCurrentPipeline(pipeline);

            {
                const cursor_pos = glfw_window.getCursorPos();
                input.ptr.mouse_position = [_]f32{
                    @floatCast(cursor_pos[0]),
                    @floatCast(cursor_pos[1]),
                };

                const resource = gctx.lookupResource(input.resource).?;
                gctx.cmdlist.SetGraphicsRootConstantBufferView(0, resource.GetGPUVirtualAddress());
            }

            gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
            gctx.cmdlist.IASetVertexBuffers(0, 1, &.{gpu_vertices.view});
            gctx.cmdlist.IASetIndexBuffer(&gpu_vertex_indices.view);
            gctx.cmdlist.DrawIndexedInstanced(3, 1, 0, 0, 0);

            gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
            gctx.flushResourceBarriers();
        }

        frac += frac_delta;
        if (frac > 1.0 or frac < 0.0) {
            frac_delta = -frac_delta;
        }
    }

    gctx.finishGpuCommands();
}
