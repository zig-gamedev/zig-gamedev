const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: instanced pills (wgpu)";

const DemoState = struct {
    gctx: *zgpu.GraphicsContext,
};

fn create(allocator: std.mem.Allocator, window: zglfw.Window) !DemoState {
    const gctx = try zgpu.GraphicsContext.create(allocator, window);

    zgui.init(allocator);
    const scale_factor = scale_factor: {
        const scale = window.getContentScale();
        break :scale_factor math.max(scale[0], scale[1]);
    };
    const font_size = 16.0 * scale_factor;
    const font_normal = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", font_size);
    assert(zgui.io.getFont(0) == font_normal);

    // This needs to be called *after* adding your custom fonts.
    zgui.backend.init(window, gctx.device, @enumToInt(zgpu.GraphicsContext.swapchain_format));

    // This call is optional. Initially, zgui.io.getFont(0) is a default font.
    zgui.io.setDefaultFont(font_normal);

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

    // To reset zgui.Style with default values:
    //zgui.getStyle().* = zgui.Style.init();

    return .{
        .gctx = gctx,
    };
}

fn destroy(allocator: std.mem.Allocator, demo: *DemoState) void {
    zgui.backend.deinit();
    zgui.deinit();
    demo.gctx.destroy(allocator);
}

fn update(demo: *DemoState) !void {
    zgui.backend.newFrame(
        demo.gctx.swapchain_descriptor.width,
        demo.gctx.swapchain_descriptor.height,
    );
    if (zgui.begin("Pill control", .{})) {
        zgui.text(
            "{d:.3} ms/frame ({d:.1} fps)",
            .{ demo.gctx.stats.average_cpu_time, demo.gctx.stats.fps },
        );

        const static = struct {
            var segments: i32 = 6;
            var length: f32 = 0.5;
            var width: f32 = 0.1;
            var x: f32 = 0.5;
            var y: f32 = 0.5;
            var angle: f32 = math.pi / 3.0;
        };
        _ = zgui.sliderInt("Segments", .{ .v = &static.segments, .min = 1, .max = 20 });
        _ = zgui.sliderFloat("Length", .{ .v = &static.length, .min = 0.0, .max = 1.0 });
        _ = zgui.sliderFloat("Width", .{ .v = &static.width, .min = 0.0, .max = 1.0 });
        _ = zgui.sliderFloat("X", .{ .v = &static.x, .min = 0.0, .max = 1.0 });
        _ = zgui.sliderFloat("Y", .{ .v = &static.y, .min = 0.0, .max = 1.0 });
        _ = zgui.sliderAngle("Angle", .{ .vrad = &static.angle, .deg_min = 0.0, .deg_max = 360.0 });
    }
    zgui.end();
}

fn draw(demo: *DemoState) void {
    const gctx = demo.gctx;
    //const fb_width = gctx.swapchain_descriptor.width;
    //const fb_height = gctx.swapchain_descriptor.height;

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

    var demo = create(allocator, window) catch {
        std.log.err("Failed to initialize the demo.", .{});
        return;
    };
    defer destroy(allocator, &demo);

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();
        try update(&demo);
        draw(&demo);
    }
}
