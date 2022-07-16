const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const glfw = @import("glfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = zgpu.zgui;
const zmath = @import("zmath");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: gui test (wgpu)";

const DemoState = struct {
    gctx: *zgpu.GraphicsContext,
};

fn init(allocator: std.mem.Allocator, window: glfw.Window) !*DemoState {
    const gctx = try zgpu.GraphicsContext.init(allocator, window);

    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    //const arena = arena_state.allocator();

    const demo = try allocator.create(DemoState);
    demo.* = .{
        .gctx = gctx,
    };

    return demo;
}

fn deinit(allocator: std.mem.Allocator, demo: *DemoState) void {
    demo.gctx.deinit(allocator);
    allocator.destroy(demo);
}

fn update(demo: *DemoState) !void {
    zgpu.gui.newFrame(demo.gctx.swapchain_descriptor.width, demo.gctx.swapchain_descriptor.height);

    zgui.setNextWindowPos(.{ .x = 20.0, .y = 20.0, .cond = .first_use_ever });
    zgui.setNextWindowSize(.{ .w = 600.0, .h = 600.0, .cond = .first_use_ever });

    if (!zgui.begin("Demo Settings", .{})) {
        zgui.end();
        return;
    }

    zgui.bullet();
    zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Average :");
    zgui.sameLine(.{});
    zgui.text(
        "  {d:.3} ms/frame ({d:.1} fps)",
        .{ demo.gctx.stats.average_cpu_time, demo.gctx.stats.fps },
    );

    zgui.separator();
    zgui.textWrapped("zgui API supports default parameters and function name overloading", .{});
    zgui.separator();

    if (zgui.treeNode("Main Widgets")) {
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Button");
        _ = zgui.button("Button 1", .{ .w = 200.0 });
        zgui.sameLine(.{ .spacing = 20.0 });
        _ = zgui.button("Button 2", .{ .h = 60.0 });
        zgui.sameLine(.{});
        _ = zgui.button("Button 3", .{ .w = 100.0, .h = 0.0 });
        zgui.sameLine(.{});
        _ = zgui.button("Button 4", .{});
        _ = zgui.button("Button 5", .{ .w = -1.0, .h = 100.0 });

        zgui.pushStyleColor(.text, .{ .color = 0xff_00_00_ff });
        _ = zgui.button("  Red Text Button  ", .{});
        zgui.popStyleColor(.{});

        zgui.sameLine(.{});
        zgui.pushStyleColor(.text, .{ .color = .{ 1.0, 1.0, 0.0, 1.0 } });
        _ = zgui.button("  Yellow Text Button  ", .{});
        zgui.popStyleColor(.{});

        _ = zgui.smallButton("  Small Button  ");
        zgui.sameLine(.{});
        _ = zgui.arrowButton("left_button_id", .{ .dir = .left });
        zgui.sameLine(.{});
        _ = zgui.arrowButton("right_button_id", .{ .dir = .right });
        zgui.spacing();

        const static = struct {
            var check0: bool = true;
            var flags_u32: u32 = 0xf;
            var radio_value: u32 = 0;
            var month: i32 = 1;
            var progress: f32 = 0.0;
        };
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Checkbox");
        _ = zgui.checkbox("Magic Is Everywhere", .{ .v = &static.check0 });
        zgui.spacing();

        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Checkbox flags");
        zgui.text("Flags value: {b} ({d})", .{ static.flags_u32, static.flags_u32 });
        _ = zgui.checkboxFlags("Bit 0", .{ .flags = &static.flags_u32, .flags_value = 0x1 });
        _ = zgui.checkboxFlags("Bit 1", .{ .flags = &static.flags_u32, .flags_value = 0x2 });
        _ = zgui.checkboxFlags("Bit 2", .{ .flags = &static.flags_u32, .flags_value = 0x4 });
        _ = zgui.checkboxFlags("Bit 3", .{ .flags = &static.flags_u32, .flags_value = 0x8 });
        zgui.spacing();

        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Radio buttons");
        if (zgui.radioButton("One", .{ .active = static.radio_value == 1 })) static.radio_value = 1;
        if (zgui.radioButton("Two", .{ .active = static.radio_value == 2 })) static.radio_value = 2;
        if (zgui.radioButton("Three", .{ .active = static.radio_value == 3 })) static.radio_value = 3;
        if (zgui.radioButton("Four", .{ .active = static.radio_value == 4 })) static.radio_value = 4;
        if (zgui.radioButton("Five", .{ .active = static.radio_value == 5 })) static.radio_value = 5;
        zgui.spacing();

        _ = zgui.radioButton("January", .{ .v = &static.month, .v_button = 1 });
        zgui.sameLine(.{});
        _ = zgui.radioButton("February", .{ .v = &static.month, .v_button = 2 });
        zgui.sameLine(.{});
        _ = zgui.radioButton("March", .{ .v = &static.month, .v_button = 3 });
        zgui.spacing();

        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Progress bar");
        zgui.progressBar(.{ .fraction = static.progress });
        static.progress += 0.005;
        if (static.progress > 1.0) static.progress = 0.0;
        zgui.spacing();

        zgui.bulletText("keep going...", .{});
        zgui.treePop();
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
            const pass = zgpu.util.beginRenderPassSimple(encoder, .load, swapchain_texv, null, null, null);
            defer zgpu.util.endRelease(pass);
            zgpu.gui.draw(pass);
        }

        break :commands encoder.finish(null);
    };
    defer commands.release();

    gctx.submit(&.{commands});
    _ = gctx.present();
}

pub fn main() !void {
    try glfw.init(.{});
    defer glfw.terminate();

    zgpu.checkSystem(content_dir) catch {
        // In case of error zgpu.checkSystem() will print error message.
        return;
    };

    const window = try glfw.Window.create(1600, 1000, window_title, null, null, .{
        .client_api = .no_api,
        .cocoa_retina_framebuffer = true,
    });
    defer window.destroy();
    try window.setSizeLimits(.{ .width = 400, .height = 400 }, .{ .width = null, .height = null });

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    const demo = try init(allocator, window);
    defer deinit(allocator, demo);

    zgpu.gui.init(window, demo.gctx.device, content_dir, "Roboto-Medium.ttf", 25.0);
    defer zgpu.gui.deinit();

    while (!window.shouldClose()) {
        try glfw.pollEvents();
        try update(demo);
        draw(demo);
    }
}
