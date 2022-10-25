const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: gamepad (wgpu)";

const DemoState = struct {
    gctx: *zgpu.GraphicsContext,
};

fn create(allocator: std.mem.Allocator, window: zglfw.Window) !*DemoState {
    const gctx = try zgpu.GraphicsContext.create(allocator, window);

    zgui.init(allocator);
    const scale_factor = scale_factor: {
        const scale = window.getContentScale();
        break :scale_factor math.max(scale[0], scale[1]);
    };
    const font_size = 24.0 * scale_factor;
    _ = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", font_size);

    // This needs to be called *after* adding your custom fonts.
    zgui.backend.init(window, gctx.device, @enumToInt(zgpu.GraphicsContext.swapchain_format));

    // You can directly manipulate zgui.Style *before* `newFrame()` call.
    // Once frame is started (after `newFrame()` call) you have to use
    // zgui.pushStyleColor*()/zgui.pushStyleVar*() functions.
    const style = zgui.getStyle();

    style.window_min_size = .{ 320.0, 240.0 };
    style.window_border_size = 8.0;
    style.scrollbar_size = 6.0;
    {
        var color = style.getColor(.scrollbar_grab);
        color[1] = 0.8;
        style.setColor(.scrollbar_grab, color);
    }
    style.scaleAllSizes(scale_factor);

    const demo = try allocator.create(DemoState);
    demo.* = .{
        .gctx = gctx,
    };

    return demo;
}

fn destroy(allocator: std.mem.Allocator, demo: *DemoState) void {
    zgui.backend.deinit();
    zgui.deinit();
    demo.gctx.destroy(allocator);
    allocator.destroy(demo);
}

fn update(demo: *DemoState) !void {
    zgui.backend.newFrame(
        demo.gctx.swapchain_descriptor.width,
        demo.gctx.swapchain_descriptor.height,
    );

    const draw_list = zgui.getBackgroundDrawList();

    draw_list.addText(.{ 0, 0 }, 0xff_ff_ff_ff, "Hello", .{});
}

fn draw(demo: *DemoState) void {
    const gctx = demo.gctx;

    const swapchain_texv = gctx.swapchain.getCurrentTextureView();
    defer swapchain_texv.release();

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        // Gui pass.
        {
            const pass = zgpu.beginRenderPassSimple(encoder, .load, swapchain_texv, null, null, null);
            defer zgpu.endReleasePass(pass);
            zgui.backend.draw(pass);
        }

        break :commands encoder.finish(null);
    };
    defer commands.release();

    gctx.submit(&.{commands});
    _ = gctx.present();
}

pub fn main() !void {
    zglfw.init() catch {
        std.log.err("Failed to initialize GLFW library.", .{});
        return;
    };
    defer zglfw.terminate();

    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.os.chdir(path) catch {};
    }

    zglfw.defaultWindowHints();
    zglfw.windowHint(.cocoa_retina_framebuffer, 1);
    zglfw.windowHint(.client_api, 0);
    const window = zglfw.createWindow(1600, 1000, window_title, null, null) catch {
        std.log.err("Failed to create demo window.", .{});
        return;
    };
    defer window.destroy();
    window.setSizeLimits(400, 400, -1, -1);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    const demo = create(allocator, window) catch {
        std.log.err("Failed to initialize the demo.", .{});
        return;
    };
    defer destroy(allocator, demo);

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();
        try update(demo);
        draw(demo);
    }
}
