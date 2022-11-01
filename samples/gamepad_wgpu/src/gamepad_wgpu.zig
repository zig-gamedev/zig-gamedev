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

    const success = zglfw.updateGamepadMappings(@embedFile("gamecontrollerdb.txt"));
    if (!success) {
        @panic("failed to update gamepad mappings");
    }

    zgui.init(allocator);
    const scale_factor = scale_factor: {
        const scale = window.getContentScale();
        break :scale_factor math.max(scale[0], scale[1]);
    };
    const font_size = 20.0 * scale_factor;
    _ = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", font_size);

    // This needs to be called *after* adding your custom fonts.
    zgui.backend.init(window, gctx.device, @enumToInt(zgpu.GraphicsContext.swapchain_format));

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
    var labels = std.enums.EnumArray(zglfw.ButtonAction, [:0]const u8).initUndefined();
    labels.set(.release, "release");
    labels.set(.press, "press");
    break :action_labels labels;
};
const axis_labels = axis_labels: {
    var labels = std.enums.EnumArray(zglfw.GamepadAxis, [:0]const u8).initUndefined();
    labels.set(.left_x, "left x");
    labels.set(.left_y, "left y");
    labels.set(.right_x, "right x");
    labels.set(.right_y, "right y");
    labels.set(.left_trigger, "left trigger");
    labels.set(.right_trigger, "right trigger");
    break :axis_labels labels;
};
const button_labels = button_labels: {
    var labels = std.enums.EnumArray(zglfw.GamepadButton, [:0]const u8).initUndefined();
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

        inline for (@typeInfo(zglfw.Joystick).Enum.fields) |joystick_enum_field| {
            const jid = @intToEnum(zglfw.Joystick, joystick_enum_field.value);
            if (zgui.beginTabItem(try std.fmt.allocPrintZ(arena.allocator(), "Joystick {}", .{joystick_enum_field.value + 1}), .{})) {
                const present = zglfw.joystickPresent(jid);
                zgui.text("Present: {s}", .{if (present) "yes" else "no"});
                zgui.newLine();
                if (present) {
                    zgui.beginGroup();
                    zgui.text("Raw joystick: {s}", .{zglfw.getJoystickGUID(jid)});
                    zgui.indent(.{ .indent_w = 50.0 });
                    zgui.beginGroup();
                    for (zglfw.getJoystickAxes(jid)) |axis, i| {
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
                    for (zglfw.getJoystickButtons(jid)) |button, i| {
                        zgui.progressBar(.{
                            .fraction = if (button == .press) 1.0 else 0.0,
                            .w = 400.0,
                            .h = 50.0,
                            .overlay = action_labels.get(button),
                        });
                        zgui.sameLine(.{});
                        zgui.text("Button {}", .{i});
                    }
                    zgui.endGroup();
                    zgui.unindent(.{ .indent_w = 50.0 });
                    zgui.endGroup();
                    zgui.sameLine(.{});
                    zgui.beginGroup();
                    const mapped = zglfw.joystickIsGamepad(jid);
                    zgui.text("Mapped gamepad: {s}", .{if (mapped) zglfw.getGamepadName(jid) else "Missing mapping. Is GUID found in gamecontrollerdb.txt?"});
                    zgui.indent(.{ .indent_w = 50.0 });
                    if (mapped) {
                        zgui.beginGroup();
                        const gamepad_state = zglfw.getGamepadState(jid);
                        inline for (@typeInfo(zglfw.GamepadAxis).Enum.fields) |axis_enum_field| {
                            const axis = @intCast(usize, axis_enum_field.value);
                            const value = gamepad_state.axes[axis];
                            zgui.progressBar(.{
                                .fraction = (value + 1.0) / 2.0,
                                .w = 400.0,
                                .h = 50.0,
                                .overlay = try std.fmt.allocPrintZ(arena.allocator(), "{d:.2}", .{value}),
                            });
                            zgui.sameLine(.{});
                            zgui.text("{s}", .{axis_labels.get(@intToEnum(zglfw.GamepadAxis, axis))});
                        }
                        zgui.endGroup();
                        zgui.sameLine(.{});
                        zgui.beginGroup();
                        inline for (@typeInfo(zglfw.GamepadButton).Enum.fields) |button_enum_field| {
                            const button = @intCast(usize, button_enum_field.value);
                            const action = gamepad_state.buttons[button];
                            zgui.progressBar(.{
                                .fraction = if (action == .press) 1.0 else 0.0,
                                .w = 400.0,
                                .h = 50.0,
                                .overlay = action_labels.get(action),
                            });
                            zgui.sameLine(.{});
                            zgui.text("{s}", .{button_labels.get(@intToEnum(zglfw.GamepadButton, button))});
                        }
                        zgui.endGroup();
                    }
                    zgui.unindent(.{ .indent_w = 50.0 });
                    zgui.endGroup();
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
    const window = zglfw.createWindow(1600, 775, window_title, null, null) catch {
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
        try update(allocator, demo);
        draw(demo);
    }
}
