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

fn create(allocator: std.mem.Allocator, window: *zglfw.Window) !*DemoState {
    const gctx = try zgpu.GraphicsContext.create(
        allocator,
        .{
            .window = window,
            .fn_getTime = @ptrCast(&zglfw.getTime),
            .fn_getFramebufferSize = @ptrCast(&zglfw.Window.getFramebufferSize),
            .fn_getWin32Window = @ptrCast(&zglfw.getWin32Window),
            .fn_getX11Display = @ptrCast(&zglfw.getX11Display),
            .fn_getX11Window = @ptrCast(&zglfw.getX11Window),
            .fn_getCocoaWindow = @ptrCast(&zglfw.getCocoaWindow),
        },
        .{},
    );
    errdefer gctx.destroy(allocator);

    const success = zglfw.Gamepad.updateMappings(@embedFile("gamecontrollerdb.txt"));
    if (!success) {
        @panic("failed to update gamepad mappings");
    }

    zgui.init(allocator);
    const scale_factor = scale_factor: {
        const scale = window.getContentScale();
        break :scale_factor @max(scale[0], scale[1]);
    };
    _ = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", math.floor(20.0 * scale_factor));

    // This needs to be called *after* adding your custom fonts.
    zgui.backend.init(
        window,
        gctx.device,
        @intFromEnum(zgpu.GraphicsContext.swapchain_format),
        @intFromEnum(wgpu.TextureFormat.undef),
    );

    // You can directly manipulate zgui.Style *before* `newFrame()` call.
    // Once frame is started (after `newFrame()` call) you have to use
    // zgui.pushStyleColor*()/zgui.pushStyleVar*() functions.
    const style = zgui.getStyle();

    style.window_min_size = .{ 320.0, 240.0 };
    style.window_border_size = 0.0;
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

const action_labels = action_labels: {
    var labels = std.enums.EnumArray(zglfw.Joystick.ButtonAction, [:0]const u8).initUndefined();
    labels.set(.release, "release");
    labels.set(.press, "press");
    break :action_labels labels;
};
const axis_labels = axis_labels: {
    var labels = std.enums.EnumArray(zglfw.Gamepad.Axis, [:0]const u8).initUndefined();
    labels.set(.left_x, "left x");
    labels.set(.left_y, "left y");
    labels.set(.right_x, "right x");
    labels.set(.right_y, "right y");
    labels.set(.left_trigger, "left trigger");
    labels.set(.right_trigger, "right trigger");
    break :axis_labels labels;
};
const button_labels = button_labels: {
    var labels = std.enums.EnumArray(zglfw.Gamepad.Button, [:0]const u8).initUndefined();
    labels.set(.a, "a");
    labels.set(.b, "b");
    labels.set(.x, "x");
    labels.set(.y, "y");
    labels.set(.left_bumper, "left bumper");
    labels.set(.right_bumper, "right bumper");
    labels.set(.back, "back");
    labels.set(.start, "start");
    labels.set(.guide, "guide");
    labels.set(.left_thumb, "left thumb");
    labels.set(.right_thumb, "right thumb");
    labels.set(.dpad_up, "dpad up");
    labels.set(.dpad_right, "dpad right");
    labels.set(.dpad_down, "dpad down");
    labels.set(.dpad_left, "dpad left");
    break :button_labels labels;
};

fn update(allocator: std.mem.Allocator, demo: *DemoState) !void {
    var arena = std.heap.ArenaAllocator.init(allocator);
    defer arena.deinit();

    zgui.backend.newFrame(
        demo.gctx.swapchain_descriptor.width,
        demo.gctx.swapchain_descriptor.height,
    );

    if (zgui.begin("Gamepad", .{
        .flags = .{
            .no_title_bar = true,
            .no_move = true,
            .no_collapse = true,
            .always_auto_resize = true,
        },
    })) {
        _ = zgui.beginTabBar("Joystick picker", .{});
        defer zgui.endTabBar();

        var jid: u32 = 0;
        while (jid < zglfw.Joystick.maximum_supported) : (jid += 1) {
            if (zgui.beginTabItem(
                try std.fmt.allocPrintZ(arena.allocator(), "Joystick {}", .{jid + 1}),
                .{},
            )) {
                if (zglfw.Joystick.get(@as(zglfw.Joystick.Id, @intCast(jid)))) |joystick| {
                    zgui.text("Present: yes", .{});
                    zgui.newLine();
                    zgui.beginGroup();
                    zgui.text("Raw joystick: {s}", .{joystick.getGuid()});
                    zgui.indent(.{ .indent_w = 50.0 });
                    zgui.beginGroup();
                    for (joystick.getAxes(), 0..) |axis, i| {
                        zgui.progressBar(.{
                            .fraction = (axis + 1.0) / 2.0,
                            .w = 400.0,
                            .h = 50.0,
                            .overlay = try std.fmt.allocPrintZ(arena.allocator(), "{d:.2}", .{axis}),
                        });
                        zgui.sameLine(.{});
                        zgui.text("Axis {}", .{i});
                    }
                    zgui.endGroup();
                    zgui.sameLine(.{});
                    zgui.beginGroup();
                    for (joystick.getButtons(), 0..) |action, i| {
                        zgui.progressBar(.{
                            .fraction = if (action == .press) 1.0 else 0.0,
                            .w = 400.0,
                            .h = 50.0,
                            .overlay = action_labels.get(action),
                        });
                        zgui.sameLine(.{});
                        zgui.text("Button {}", .{i});
                    }
                    zgui.endGroup();
                    zgui.unindent(.{ .indent_w = 50.0 });
                    zgui.endGroup();
                    zgui.sameLine(.{});
                    zgui.beginGroup();
                    if (joystick.asGamepad()) |gamepad| {
                        zgui.text("Mapped gamepad: {s}", .{gamepad.getName()});
                        zgui.indent(.{ .indent_w = 50.0 });
                        zgui.beginGroup();
                        const gamepad_state = gamepad.getState();
                        for (std.enums.values(zglfw.Gamepad.Axis)) |axis| {
                            const value = gamepad_state.axes[@intFromEnum(axis)];
                            zgui.progressBar(.{
                                .fraction = (value + 1.0) / 2.0,
                                .w = 400.0,
                                .h = 50.0,
                                .overlay = try std.fmt.allocPrintZ(arena.allocator(), "{d:.2}", .{value}),
                            });
                            zgui.sameLine(.{});
                            zgui.text("{s}", .{axis_labels.get(axis)});
                        }
                        zgui.endGroup();
                        zgui.sameLine(.{});
                        zgui.beginGroup();
                        for (std.enums.values(zglfw.Gamepad.Button)) |button| {
                            const action = gamepad_state.buttons[@intFromEnum(button)];
                            zgui.progressBar(.{
                                .fraction = if (action == .press) 1.0 else 0.0,
                                .w = 400.0,
                                .h = 50.0,
                                .overlay = action_labels.get(action),
                            });
                            zgui.sameLine(.{});
                            zgui.text("{s}", .{button_labels.get(button)});
                        }
                        zgui.endGroup();
                        zgui.unindent(.{ .indent_w = 50.0 });
                    } else {
                        zgui.text("Mapped gamepad: Missing mapping. Is GUID found in gamecontrollerdb.txt?", .{});
                    }
                    zgui.endGroup();
                } else {
                    zgui.text("Present: no", .{});
                }
                zgui.endTabItem();
            }
        }
    }
    zgui.end();
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
    try zglfw.init();
    defer zglfw.terminate();

    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.posix.chdir(path) catch {};
    }

    zglfw.windowHintTyped(.client_api, .no_api);

    const window = try zglfw.Window.create(1600, 775, window_title, null);
    defer window.destroy();
    window.setSizeLimits(400, 400, -1, -1);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    const demo = try create(allocator, window);
    defer destroy(allocator, demo);

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();
        try update(allocator, demo);
        draw(demo);
    }
}
