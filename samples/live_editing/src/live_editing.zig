const std = @import("std");

const zglfw = @import("zglfw");
const zwin32 = @import("zwin32");
const zd3d12 = @import("zd3d12");
const w32 = zwin32.w32;
const d3d12 = zwin32.d3d12;

const externs = @import("externs.zig");
const Entry = externs.ExternEntry;
const Reloader = externs.Reloader;

pub export const D3D12SDKVersion: u32 = 610;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: live editing";

pub fn main() !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    try zglfw.init();
    defer zglfw.terminate();

    zglfw.windowHintTyped(.client_api, .no_api);
    const zglfw_window = try zglfw.Window.create(1600, 1200, window_name, null);
    defer zglfw_window.destroy();

    const window = zglfw.getWin32Window(zglfw_window) orelse return error.FailedToGetWin32Window;
    var gctx = zd3d12.GraphicsContext.init(allocator, window);
    defer gctx.deinit(allocator);

    var entry = try Entry.init(allocator, &gctx);
    defer entry.deinit(allocator);

    // watch for src changes
    var reloader = try Reloader.initAlloc(allocator);
    defer reloader.deinit(allocator);

    var framebuffer_size = zglfw_window.getFramebufferSize();

    while (!zglfw_window.shouldClose() and zglfw_window.getKey(.escape) != .press) {
        if (zglfw_window.getAttribute(.iconified)) {
            // Window is minimized
            const ns_in_ms: u64 = 1_000_000;
            std.time.sleep(10 * ns_in_ms);
            continue;
        }

        entry = try reloader.update(allocator, &entry);

        zglfw.pollEvents();
        {
            const cursor_pos = zglfw_window.getCursorPos();
            const input = Entry.Input{
                .mouse_position = [_]f32{
                    @floatCast(cursor_pos[0]),
                    @floatCast(cursor_pos[1]),
                },
            };
            entry.inputUpdated(input);
        }

        {
            const next_framebuffer_size = zglfw_window.getFramebufferSize();
            if (!std.meta.eql(framebuffer_size, next_framebuffer_size)) {
                gctx.resize(@intCast(next_framebuffer_size[0]), @intCast(next_framebuffer_size[1]));
            }
            framebuffer_size = next_framebuffer_size;
        }

        {
            gctx.beginFrame();
            defer gctx.endFrame();

            try entry.renderFrameD3d12();
        }

        entry.postRenderFrame();
    }

    gctx.finishGpuCommands();
}
