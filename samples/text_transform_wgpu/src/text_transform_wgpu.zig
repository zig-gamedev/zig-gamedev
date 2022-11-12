const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");
const zm = @import("zmath");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: text transform(wgpu)";

const DemoState = struct {
    gctx: *zgpu.GraphicsContext,

    text_texture: zgpu.TextureHandle,
    text_texture_cache_key: []u8 = "",

    sample_text: [32]u8 = [_]u8{0} ** 32,
    render_scale: f32 = 1,
    msaa: bool = false,
    offset: [2]f32 = .{ 0, 0 },
    scale: [2]f32 = .{ 1, 1 },
    angle: f32 = 0,

    fn init(allocator: std.mem.Allocator, window: zglfw.Window) !DemoState {
        const gctx = try zgpu.GraphicsContext.create(allocator, window);

        zgui.init(allocator);
        const scale_factor = scale_factor: {
            const scale = window.getContentScale();
            break :scale_factor math.max(scale[0], scale[1]);
        };
        _ = zgui.io.addFontFromFile(
            content_dir ++ "Roboto-Medium.ttf",
            math.floor(20.0 * scale_factor),
        );
        {
            var config = zgui.FontConfig.init();
            config.merge_mode = true;
            const ranges: []const u16 = &.{ 0x02DA, 0x02DB, 0 };
            _ = zgui.io.addFontFromFileWithConfig(
                content_dir ++ "Roboto-Medium.ttf",
                math.floor(20.0 * scale_factor),
                config,
                ranges.ptr,
            );
        }

        // This needs to be called *after* adding your custom fonts.
        zgui.backend.init(window, gctx.device, @enumToInt(zgpu.GraphicsContext.swapchain_format));

        var demo = DemoState{
            .gctx = gctx,
            .text_texture = .{},
        };

        const default_text = "Greetings!";
        std.mem.copy(u8, demo.sample_text[0..], default_text);

        return demo;
    }

    fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
        const gctx = demo.gctx;
        zgui.backend.deinit();
        zgui.deinit();
        gctx.destroy(allocator);
    }

    fn update(demo: *DemoState, _: std.mem.Allocator) !void {
        const gctx = demo.gctx;

        zgui.backend.newFrame(
            gctx.swapchain_descriptor.width,
            gctx.swapchain_descriptor.height,
        );

        if (!std.mem.eql(u8, demo.text_texture_cache_key, demo.sample_text[0..])) {
            demo.recreateTextTexture();
            demo.text_texture_cache_key = demo.sample_text[0..];
        }
        _ = zgui.begin("Controls", .{
            .flags = .{
                .no_title_bar = true,
                .no_move = true,
                .no_collapse = true,
                .always_auto_resize = true,
            },
        });
        defer zgui.end();

        _ = zgui.inputText("Sample text", .{ .buf = demo.sample_text[0..] });
        _ = zgui.sliderFloat("Render scale", .{
            .v = &demo.render_scale,
            .min = 0.01,
            .max = 8,
            .cfmt = "%.2fx",
        });
        _ = zgui.checkbox("4x multisample anti-aliasing", .{ .v = &demo.msaa });
        _ = zgui.sliderFloat2("Translate", .{
            .v = &demo.offset,
            .min = -1000,
            .max = 1000,
            .cfmt = "%.0f",
        });
        _ = zgui.sliderFloat2("Scale", .{
            .v = &demo.scale,
            .min = -2,
            .max = 2,
            .cfmt = "%.2f x",
        });
        _ = zgui.sliderFloat("Rotate", .{
            .v = &demo.angle,
            .min = 0,
            .max = 360,
            .cfmt = "%.0f˚",
        });
    }

    fn draw(demo: *DemoState) void {
        const gctx = demo.gctx;

        const back_buffer_view = gctx.swapchain.getCurrentTextureView();
        defer back_buffer_view.release();

        const commands = commands: {
            const encoder = gctx.device.createCommandEncoder(null);
            defer encoder.release();

            {
                const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
                    .view = back_buffer_view,
                    .load_op = .load,
                    .store_op = .store,
                }};
                const render_pass_info = wgpu.RenderPassDescriptor{
                    .color_attachment_count = color_attachments.len,
                    .color_attachments = &color_attachments,
                };
                const pass = encoder.beginRenderPass(render_pass_info);
                defer {
                    pass.end();
                    pass.release();
                }

                zgui.backend.draw(pass);
            }

            break :commands encoder.finish(null);
        };
        defer commands.release();

        gctx.submit(&.{commands});
        _ = gctx.present();
    }

    fn recreateTextTexture(demo: *DemoState) void {
        const gctx = demo.gctx;
        gctx.releaseResource(demo.text_texture);

        const text_size = zgui.calcTextSize(demo.sample_text[0..], .{});
        demo.text_texture = gctx.createTexture(.{
            .usage = .{ .render_attachment = true },
            .dimension = .tdim_2d,
            .size = .{
                .width = @floatToInt(u32, @ceil(text_size[0])),
                .height = @floatToInt(u32, @ceil(text_size[1])),
            },
            .format = gctx.swapchain_descriptor.format,
            .sample_count = 1,
        });
    }
};

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

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = DemoState.init(allocator, window) catch {
        std.log.err("Failed to initialize the demo.", .{});
        return;
    };
    defer demo.deinit(allocator);

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();
        try demo.update(allocator);
        demo.draw();
    }
}
