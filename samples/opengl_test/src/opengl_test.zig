const std = @import("std");
const glfw = @import("glfw");
const glfw_native = glfw.Native(.{ .win32 = true });
const zwin32 = @import("zwin32");
const w32 = zwin32.base;
const d3d12 = zwin32.d3d12;
const zd3d12 = @import("zd3d12");
const common = @import("common");

pub export const D3D12SDKVersion: u32 = 4;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: opengl test";
const window_width = 1920;
const window_height = 1080;

pub fn main() !void {
    common.init();
    defer common.deinit();

    try glfw.init(.{});
    defer glfw.terminate();

    const window = try glfw.Window.create(window_width, window_height, window_name, null, null, .{
        .client_api = .no_api,
    });
    defer window.destroy();

    const hwnd = glfw_native.getWin32Window(window);

    var gctx = zd3d12.GraphicsContext.init(hwnd);
    defer gctx.deinit();

    //gctx.present_flags = 0;
    //gctx.present_interval = 1;

    var frame_stats = common.FrameStats.init();

    while (!window.shouldClose()) {
        try glfw.pollEvents();

        frame_stats.update(gctx.window, window_name);

        gctx.beginFrame();

        const back_buffer = gctx.getBackBuffer();
        gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
        gctx.flushResourceBarriers();

        gctx.cmdlist.OMSetRenderTargets(
            1,
            &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
            w32.TRUE,
            null,
        );
        gctx.cmdlist.ClearRenderTargetView(
            back_buffer.descriptor_handle,
            &.{ 0.2, 0.4, 0.8, 1.0 },
            0,
            null,
        );

        gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_PRESENT);
        gctx.flushResourceBarriers();

        gctx.endFrame();
    }
}
