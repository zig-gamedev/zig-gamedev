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
    zgui.dummy(.{ .w = -1.0, .h = 20.0 });
    zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "zgui -");
    zgui.sameLine(.{});
    zgui.textWrapped("Zig bindings for 'dear imgui' library. Easy to use API with defualt arguments, named parameters and Zig style text formatting.", .{});
    zgui.dummy(.{ .w = -1.0, .h = 20.0 });
    zgui.separator();

    if (zgui.treeNode("Widgets: Main")) {
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Button");
        _ = zgui.button("Button 1", .{ .w = 200.0 });
        zgui.sameLine(.{ .spacing = 20.0 });
        _ = zgui.button("Button 2", .{ .h = 60.0 });
        zgui.sameLine(.{});
        _ = zgui.button("Button 3", .{ .w = 100.0, .h = 0.0 });
        zgui.sameLine(.{});
        _ = zgui.button("Button 4", .{});
        _ = zgui.button("Button 5", .{ .w = -1.0, .h = 100.0 });

        zgui.pushStyleColor(.text, .{ .col = .{ 1.0, 0.0, 0.0, 1.0 } });
        _ = zgui.button("  Red Text Button  ", .{});
        zgui.popStyleColor(.{});

        zgui.sameLine(.{});
        zgui.pushStyleColor(.text, .{ .col = .{ 1.0, 1.0, 0.0, 1.0 } });
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
            var bits: u32 = 0xf;
            var radio_value: u32 = 1;
            var month: i32 = 1;
            var progress: f32 = 0.0;
        };
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Checkbox");
        _ = zgui.checkbox("Magic Is Everywhere", .{ .v = &static.check0 });
        zgui.spacing();

        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Checkbox bits");
        zgui.text("Bits value: {b} ({d})", .{ static.bits, static.bits });
        _ = zgui.checkboxBits("Bit 0", .{ .bits = &static.bits, .bits_value = 0x1 });
        _ = zgui.checkboxBits("Bit 1", .{ .bits = &static.bits, .bits_value = 0x2 });
        _ = zgui.checkboxBits("Bit 2", .{ .bits = &static.bits, .bits_value = 0x4 });
        _ = zgui.checkboxBits("Bit 3", .{ .bits = &static.bits, .bits_value = 0x8 });
        zgui.spacing();

        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Radio buttons");
        if (zgui.radioButton("One", .{ .active = static.radio_value == 1 })) static.radio_value = 1;
        if (zgui.radioButton("Two", .{ .active = static.radio_value == 2 })) static.radio_value = 2;
        if (zgui.radioButton("Three", .{ .active = static.radio_value == 3 })) static.radio_value = 3;
        if (zgui.radioButton("Four", .{ .active = static.radio_value == 4 })) static.radio_value = 4;
        if (zgui.radioButton("Five", .{ .active = static.radio_value == 5 })) static.radio_value = 5;
        zgui.spacing();

        _ = zgui.radioButtonStatePtr("January", .{ .v = &static.month, .v_button = 1 });
        zgui.sameLine(.{});
        _ = zgui.radioButtonStatePtr("February", .{ .v = &static.month, .v_button = 2 });
        zgui.sameLine(.{});
        _ = zgui.radioButtonStatePtr("March", .{ .v = &static.month, .v_button = 3 });
        zgui.spacing();

        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Progress bar");
        zgui.progressBar(.{ .fraction = static.progress });
        static.progress += 0.005;
        if (static.progress > 1.0) static.progress = 0.0;
        zgui.spacing();

        zgui.bulletText("keep going...", .{});
        zgui.treePop();
    }

    if (zgui.treeNode("Widgets: Combo Box")) {
        const static = struct {
            var selection_index: u32 = 0;
            var current_item: i32 = 0;
        };

        const items = [_][:0]const u8{ "aaa", "bbb", "ccc", "ddd", "eee", "FFF", "ggg", "hhh" };
        if (zgui.beginCombo("Combo 0", .{ .preview_value = items[static.selection_index] })) {
            for (items) |item, index| {
                const i = @intCast(u32, index);
                if (zgui.selectable(item, .{ .selected = static.selection_index == i })) static.selection_index = i;
            }
            zgui.endCombo();
        }

        _ = zgui.combo("Combo 1", .{
            .current_item = &static.current_item,
            .items_separated_by_zeros = "Item 0\x00Item 1\x00Item 2\x00Item 3\x00\x00",
        });

        zgui.treePop();
    }

    if (zgui.treeNode("Widgets: Drag Sliders")) {
        const static = struct {
            var v1: f32 = 0.0;
            var v2: [2]f32 = .{ 0.0, 0.0 };
            var v3: [3]f32 = .{ 0.0, 0.0, 0.0 };
            var v4: [4]f32 = .{ 0.0, 0.0, 0.0, 0.0 };
            var range: [2]f32 = .{ 0.0, 0.0 };
            var v1i: i32 = 0.0;
            var v2i: [2]i32 = .{ 0, 0 };
            var v3i: [3]i32 = .{ 0, 0, 0 };
            var v4i: [4]i32 = .{ 0, 0, 0, 0 };
            var rangei: [2]i32 = .{ 0, 0 };
            var si8: i8 = 123;
            var vu16: [3]u16 = .{ 10, 11, 12 };
            var sd: f64 = 0.0;
        };
        _ = zgui.dragFloat("Drag float 1", .{ .v = &static.v1 });
        _ = zgui.dragFloat2("Drag float 2", .{ .v = &static.v2 });
        _ = zgui.dragFloat3("Drag float 3", .{ .v = &static.v3 });
        _ = zgui.dragFloat4("Drag float 4", .{ .v = &static.v4 });
        _ = zgui.dragFloatRange2("Drag float range 2", .{ .v_current_min = &static.range[0], .v_current_max = &static.range[1] });
        _ = zgui.dragInt("Drag int 1", .{ .v = &static.v1i });
        _ = zgui.dragInt2("Drag int 2", .{ .v = &static.v2i });
        _ = zgui.dragInt3("Drag int 3", .{ .v = &static.v3i });
        _ = zgui.dragInt4("Drag int 4", .{ .v = &static.v4i });
        _ = zgui.dragIntRange2("Drag int range 2", .{ .v_current_min = &static.rangei[0], .v_current_max = &static.rangei[1] });
        _ = zgui.dragScalar("Drag scalar (i8)", i8, .{ .v = &static.si8, .v_min = -20 });
        _ = zgui.dragScalarN("Drag scalar N ([3]u16)", @TypeOf(static.vu16), .{ .v = &static.vu16, .v_max = 100 });
        _ = zgui.dragScalar("Drag scalar (f64)", f64, .{ .v = &static.sd, .v_min = -1.0, .v_max = 1.0, .v_speed = 0.005 });
        zgui.treePop();
    }

    if (zgui.treeNode("Widgets: Regular Sliders")) {
        const static = struct {
            var v1: f32 = 0;
            var v2: [2]f32 = .{ 0, 0 };
            var v3: [3]f32 = .{ 0, 0, 0 };
            var v4: [4]f32 = .{ 0, 0, 0, 0 };
            var v1i: i32 = 0;
            var v2i: [2]i32 = .{ 0, 0 };
            var v3i: [3]i32 = .{ 10, 10, 10 };
            var v4i: [4]i32 = .{ 0, 0, 0, 0 };
            var su8: u8 = 1;
            var vu16: [3]u16 = .{ 10, 11, 12 };
            var vsf: f32 = 0;
            var vsi: i32 = 0;
            var vsu8: u8 = 1;
            var angle: f32 = 0;
        };
        _ = zgui.sliderFloat("Slider float 1", .{ .v = &static.v1, .v_min = 0.0, .v_max = 1.0 });
        _ = zgui.sliderFloat2("Slider float 2", .{ .v = &static.v2, .v_min = -1.0, .v_max = 1.0 });
        _ = zgui.sliderFloat3("Slider float 3", .{ .v = &static.v3, .v_min = 0.0, .v_max = 1.0 });
        _ = zgui.sliderFloat4("Slider float 4", .{ .v = &static.v4, .v_min = 0.0, .v_max = 1.0 });
        _ = zgui.sliderInt("Slider int 1", .{ .v = &static.v1i, .v_min = 0, .v_max = 100 });
        _ = zgui.sliderInt2("Slider int 2", .{ .v = &static.v2i, .v_min = -20, .v_max = 20 });
        _ = zgui.sliderInt3("Slider int 3", .{ .v = &static.v3i, .v_min = 10, .v_max = 50 });
        _ = zgui.sliderInt4("Slider int 4", .{ .v = &static.v4i, .v_min = 0, .v_max = 10 });
        _ = zgui.sliderScalar("Slider scalar (u8)", u8, .{ .v = &static.su8, .v_min = 0, .v_max = 100, .format = "%Xh" });
        _ = zgui.sliderScalarN("Slider scalar N ([3]u16)", [3]u16, .{ .v = &static.vu16, .v_min = 1, .v_max = 100 });
        _ = zgui.sliderAngle("Slider angle", .{ .v_rad = &static.angle });
        _ = zgui.vsliderFloat("VSlider float", .{ .w = 80.0, .h = 200.0, .v = &static.vsf, .v_min = 0.0, .v_max = 1.0 });
        zgui.sameLine(.{});
        _ = zgui.vsliderInt("VSlider int", .{ .w = 80.0, .h = 200.0, .v = &static.vsi, .v_min = 0, .v_max = 100 });
        zgui.sameLine(.{});
        _ = zgui.vsliderScalar(
            "VSlider scalar (u8)",
            u8,
            .{ .w = 80.0, .h = 200.0, .v = &static.vsu8, .v_min = 0, .v_max = 200 },
        );
        zgui.treePop();
    }

    if (zgui.treeNode("Widgets: Input with Keyboard")) {
        const static = struct {
            var v1: f32 = 0;
            var v2: [2]f32 = .{ 0, 0 };
            var v3: [3]f32 = .{ 0, 0, 0 };
            var v4: [4]f32 = .{ 0, 0, 0, 0 };
            var v1i: i32 = 0;
            var v2i: [2]i32 = .{ 0, 0 };
            var v3i: [3]i32 = .{ 0, 0, 0 };
            var v4i: [4]i32 = .{ 0, 0, 0, 0 };
            var sf64: f64 = 0.0;
            var si8: i8 = 0;
            var v3u8: [3]u8 = .{ 0, 0, 0 };
        };
        _ = zgui.inputFloat("Input float 1", .{ .v = &static.v1 });
        _ = zgui.inputFloat2("Input float 2", .{ .v = &static.v2 });
        _ = zgui.inputFloat3("Input float 3", .{ .v = &static.v3 });
        _ = zgui.inputFloat4("Input float 4", .{ .v = &static.v4 });
        _ = zgui.inputInt("Input int 1", .{ .v = &static.v1i });
        _ = zgui.inputInt2("Input int 2", .{ .v = &static.v2i });
        _ = zgui.inputInt3("Input int 3", .{ .v = &static.v3i });
        _ = zgui.inputInt4("Input int 4", .{ .v = &static.v4i });
        _ = zgui.inputDouble("Input double", .{ .v = &static.sf64 });
        _ = zgui.inputScalar("Input scalar (i8)", i8, .{ .v = &static.si8 });
        _ = zgui.inputScalarN("Input scalar N ([3]u8)", [3]u8, .{ .v = &static.v3u8 });
        zgui.treePop();
    }

    if (zgui.treeNode("Widgets: Color Editor/Picker")) {
        const static = struct {
            var col3: [3]f32 = .{ 0, 0, 0 };
            var col4: [4]f32 = .{ 0, 0, 0, 0 };
            var col3p: [3]f32 = .{ 0, 0, 0 };
            var col4p: [4]f32 = .{ 0, 0, 0, 0 };
        };
        _ = zgui.colorEdit3("Color edit 3", .{ .col = &static.col3 });
        _ = zgui.colorEdit4("Color edit 4", .{ .col = &static.col4 });
        _ = zgui.colorPicker3("Color picker 3", .{ .col = &static.col3p });
        _ = zgui.colorPicker4("Color picker 4", .{ .col = &static.col4p });
        _ = zgui.colorButton("color_button_id", .{ .col = .{ 0, 1, 0, 1 } });
        zgui.treePop();
    }

    if (zgui.treeNode("Widgets: Trees")) {
        if (zgui.treeNodeStrId("tree_id", "My Tree {d}", .{1})) {
            zgui.textUnformatted("Some content...");
            zgui.treePop();
        }
        if (zgui.collapsingHeader("Collapsing header 1", .{})) {
            zgui.textUnformatted("Some content...");
        }
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
