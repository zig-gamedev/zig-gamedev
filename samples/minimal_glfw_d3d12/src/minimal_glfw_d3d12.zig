const std = @import("std");
const glfw = @import("zglfw");
const zwin32 = @import("zwin32");
const zd3d12 = @import("zd3d12");
const w32 = zwin32.w32;
const d3d12 = zwin32.d3d12;

pub export const D3D12SDKVersion: u32 = 610;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: minimal glfw d3d12";

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    try glfw.init();
    defer glfw.terminate();

    glfw.windowHintTyped(.client_api, .no_api);
    const glfw_window = try glfw.Window.create(1600, 1200, window_name, null);
    defer glfw_window.destroy();

    const window = glfw.getWin32Window(glfw_window) orelse return error.FailedToGetWin32Window;
    var gctx = zd3d12.GraphicsContext.init(allocator, window);
    defer gctx.deinit(allocator);

    const pipeline = pipeline: {
        var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
        defer arena_allocator_state.deinit();
        const arena_allocator = arena_allocator_state.allocator();

        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.DepthStencilState.DepthEnable = w32.FALSE;
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;

        break :pipeline gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "minimal_glfw_d3d12.vs.cso",
            content_dir ++ "minimal_glfw_d3d12.ps.cso",
        );
    };

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
                &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
                w32.TRUE,
                null,
            );
            gctx.cmdlist.ClearRenderTargetView(
                back_buffer.descriptor_handle,
                &.{ 0.2, frac, 0.8, 1.0 },
                0,
                null,
            );

            gctx.setCurrentPipeline(pipeline);
            gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
            gctx.cmdlist.DrawInstanced(3, 1, 0, 0);

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
