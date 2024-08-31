const std = @import("std");
const zgui = @import("zgui");
const glfw = @import("zglfw");

const zwindows = @import("zwindows");
const windows = zwindows.windows;
const d3d12 = zwindows.d3d12;
const dxgi = zwindows.dxgi;

const zd3d12 = @import("zd3d12");

pub export const D3D12SDKVersion: u32 = 610;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: minimal imgui glfw d3d12";

pub fn main() !void {
    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.posix.chdir(path) catch {};
    }

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    try glfw.init();
    defer glfw.terminate();

    glfw.windowHintTyped(.client_api, .no_api);
    const glfw_window = try glfw.Window.create(800, 600, window_name, null);
    defer glfw_window.destroy();
    glfw_window.setSizeLimits(400, 400, -1, -1);

    zgui.init(allocator);
    defer zgui.deinit();

    const window = glfw.getWin32Window(glfw_window) orelse return error.FailedToGetWin32Window;
    var gctx = zd3d12.GraphicsContext.init(.{
        .allocator = allocator,
        .window = window,
    });
    defer gctx.deinit(allocator);

    const scale_factor = scale_factor: {
        const scale = glfw_window.getContentScale();
        break :scale_factor @max(scale[0], scale[1]);
    };
    _ = zgui.io.addFontFromFile(
        content_dir ++ "Roboto-Medium.ttf",
        std.math.floor(16.0 * scale_factor),
    );

    zgui.getStyle().scaleAllSizes(scale_factor);

    {
        const cbv_srv = gctx.cbv_srv_uav_gpu_heaps[0];
        zgui.backend.init(
            glfw_window,
            gctx.device,
            zd3d12.GraphicsContext.max_num_buffered_frames,
            @intFromEnum(dxgi.FORMAT.R8G8B8A8_UNORM),
            cbv_srv.heap.?,
            @bitCast(cbv_srv.base.cpu_handle),
            @bitCast(cbv_srv.base.gpu_handle),
        );
    }
    defer zgui.backend.deinit();

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
                &.{ 0.0, 0.0, 0.0, 1.0 },
                0,
                null,
            );

            zgui.backend.newFrame(@intCast(framebuffer_size[0]), @intCast(framebuffer_size[1]));

            // Set the starting window position and size to custom values
            zgui.setNextWindowPos(.{ .x = 20.0, .y = 20.0, .cond = .first_use_ever });
            zgui.setNextWindowSize(.{ .w = -1.0, .h = -1.0, .cond = .first_use_ever });

            if (zgui.begin("My window", .{})) {
                if (zgui.button("Press me!", .{ .w = 200.0 })) {
                    std.debug.print("Button pressed\n", .{});
                }
            }
            zgui.end();

            zgui.backend.draw(gctx.cmdlist);

            gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
            gctx.flushResourceBarriers();
        }
    }

    gctx.finishGpuCommands();
}
