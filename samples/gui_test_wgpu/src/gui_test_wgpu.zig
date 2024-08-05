const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");
const zstbi = @import("zstbi");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: gui test (wgpu)";

const embedded_font_data = @embedFile("./FiraCode-Medium.ttf");

const DemoState = struct {
    gctx: *zgpu.GraphicsContext,
    texture_view: zgpu.TextureViewHandle,
    font_normal: zgui.Font,
    font_large: zgui.Font,
    draw_list: zgui.DrawList,
    alloced_input_text_buf: [:0]u8,
    alloced_input_text_multiline_buf: [:0]u8,
    alloced_input_text_with_hint_buf: [:0]u8,
    node_editor: *zgui.node_editor.EditorContext,
};
var _te: *zgui.te.TestEngine = undefined;

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
            .fn_getWaylandDisplay = @ptrCast(&zglfw.getWaylandDisplay),
            .fn_getWaylandSurface = @ptrCast(&zglfw.getWaylandWindow),
            .fn_getCocoaWindow = @ptrCast(&zglfw.getCocoaWindow),
        },
        .{},
    );
    errdefer gctx.destroy(allocator);

    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    const arena = arena_state.allocator();

    zstbi.init(arena);
    defer zstbi.deinit();

    var image = try zstbi.Image.loadFromFile(content_dir ++ "genart_0025_5.png", 4);
    defer image.deinit();

    // Create a texture.
    const texture = gctx.createTexture(.{
        .usage = .{ .texture_binding = true, .copy_dst = true },
        .size = .{
            .width = image.width,
            .height = image.height,
            .depth_or_array_layers = 1,
        },
        .format = zgpu.imageInfoToTextureFormat(
            image.num_components,
            image.bytes_per_component,
            image.is_hdr,
        ),
        .mip_level_count = 1,
    });
    const texture_view = gctx.createTextureView(texture, .{});

    gctx.queue.writeTexture(
        .{ .texture = gctx.lookupResource(texture).? },
        .{
            .bytes_per_row = image.bytes_per_row,
            .rows_per_image = image.height,
        },
        .{ .width = image.width, .height = image.height },
        u8,
        image.data,
    );

    zgui.init(allocator);
    zgui.plot.init();
    _te = zgui.te.getTestEngine().?;
    const scale_factor = scale_factor: {
        const scale = window.getContentScale();
        break :scale_factor @max(scale[0], scale[1]);
    };
    const font_size = 16.0 * scale_factor;
    const font_large = zgui.io.addFontFromMemory(embedded_font_data, math.floor(font_size * 1.1));
    const font_normal = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", math.floor(font_size));
    assert(zgui.io.getFont(0) == font_large);
    assert(zgui.io.getFont(1) == font_normal);

    // This needs to be called *after* adding your custom fonts.
    zgui.backend.init(
        window,
        gctx.device,
        @intFromEnum(zgpu.GraphicsContext.swapchain_format),
        @intFromEnum(wgpu.TextureFormat.undef),
    );

    // This call is optional. Initially, zgui.io.getFont(0) is a default font.
    zgui.io.setDefaultFont(font_normal);

    // You can directly manipulate zgui.Style *before* `newFrame()` call.
    // Once frame is started (after `newFrame()` call) you have to use
    // zgui.pushStyleColor*()/zgui.pushStyleVar*() functions.
    const style = zgui.getStyle();

    style.window_min_size = .{ 320.0, 240.0 };
    style.scrollbar_size = 6.0;
    {
        var color = style.getColor(.scrollbar_grab);
        color[1] = 0.8;
        style.setColor(.scrollbar_grab, color);
    }
    style.scaleAllSizes(scale_factor);

    // To reset zgui.Style with default values:
    //zgui.getStyle().* = zgui.Style.init();

    {
        zgui.plot.getStyle().line_weight = 3.0;
        const plot_style = zgui.plot.getStyle();
        plot_style.marker = .circle;
        plot_style.marker_size = 5.0;
    }

    const draw_list = zgui.createDrawList();

    const demo = try allocator.create(DemoState);
    demo.* = .{
        .gctx = gctx,
        .texture_view = texture_view,
        .font_normal = font_normal,
        .font_large = font_large,
        .draw_list = draw_list,
        .alloced_input_text_buf = try allocator.allocSentinel(u8, 4, 0),
        .alloced_input_text_multiline_buf = try allocator.allocSentinel(u8, 4, 0),
        .alloced_input_text_with_hint_buf = try allocator.allocSentinel(u8, 4, 0),
        .node_editor = zgui.node_editor.EditorContext.create(.{ .enable_smooth_zoom = true }),
    };
    demo.alloced_input_text_buf[0] = 0;
    demo.alloced_input_text_multiline_buf[0] = 0;
    demo.alloced_input_text_with_hint_buf[0] = 0;

    return demo;
}

fn destroy(allocator: std.mem.Allocator, demo: *DemoState) void {
    zgui.backend.deinit();
    zgui.plot.deinit();
    zgui.destroyDrawList(demo.draw_list);
    zgui.deinit();
    demo.gctx.destroy(allocator);
    allocator.free(demo.alloced_input_text_buf);
    allocator.free(demo.alloced_input_text_multiline_buf);
    allocator.free(demo.alloced_input_text_with_hint_buf);
    allocator.destroy(demo);
}

var check_b = false;
fn registerTests() void {
    _ = _te.registerTest(
        "Awesome",
        "should_do_some_magic",
        @src(),
        struct {
            pub fn gui(ctx: *zgui.te.TestContext) !void {
                _ = ctx;
            }

            pub fn run(ctx: *zgui.te.TestContext) !void {
                ctx.setRef("/Demo Settings");
                ctx.windowFocus("");
                ctx.itemAction(.open, "Widgets: Main", .{}, null);
                ctx.itemAction(.click, "**/Button 1", .{}, null);
                ctx.itemAction(.click, "**/Magic Is Everywhere", .{}, null);

                std.testing.expect(true) catch |err| {
                    zgui.te.checkTestError(@src(), err);
                    return;
                };
            }
        },
    );

    _ = _te.registerTest(
        "Awesome",
        "should_do_some_another_magic",
        @src(),
        struct {
            pub fn gui(ctx: *zgui.te.TestContext) !void {
                _ = ctx; // autofix
                _ = zgui.begin("Test Window", .{ .flags = .{ .no_saved_settings = true } });
                defer zgui.end();

                zgui.text("Hello, automation world", .{});
                _ = zgui.button("Click Me", .{});
                if (zgui.treeNode("Node")) {
                    defer zgui.treePop();

                    _ = zgui.checkbox("Checkbox", .{ .v = &check_b });
                }
            }

            pub fn run(ctx: *zgui.te.TestContext) !void {
                ctx.setRef("/Test Window");
                ctx.windowFocus("");

                ctx.itemAction(.click, "Click Me", .{}, null);
                ctx.itemAction(.open, "Node", .{}, null);
                ctx.itemAction(.check, "Node/Checkbox", .{}, null);
                ctx.itemAction(.uncheck, "Node/Checkbox", .{}, null);

                std.testing.expect(true) catch |err| {
                    zgui.te.checkTestError(@src(), err);
                    return;
                };
            }
        },
    );
}

const SimpleEnum = enum {
    first,
    second,
    third,
};
const SparseEnum = enum(i32) {
    first = 10,
    second = 100,
    third = 1000,
};
const NonExhaustiveEnum = enum(i32) {
    first = 10,
    second = 100,
    third = 1000,
    _,
};

fn update(demo: *DemoState) !void {
    zgui.backend.newFrame(
        demo.gctx.swapchain_descriptor.width,
        demo.gctx.swapchain_descriptor.height,
    );

    _te.showTestEngineWindows(null);

    zgui.setNextWindowPos(.{ .x = 20.0, .y = 20.0, .cond = .first_use_ever });
    zgui.setNextWindowSize(.{ .w = -1.0, .h = -1.0, .cond = .first_use_ever });

    zgui.pushStyleVar1f(.{ .idx = .window_rounding, .v = 5.0 });
    zgui.pushStyleVar2f(.{ .idx = .window_padding, .v = .{ 5.0, 5.0 } });
    defer zgui.popStyleVar(.{ .count = 2 });

    if (zgui.begin("Demo Settings", .{})) {
        zgui.bullet();
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Average :");
        zgui.sameLine(.{});
        zgui.text(
            "{d:.3} ms/frame ({d:.1} fps)",
            .{ demo.gctx.stats.average_cpu_time, demo.gctx.stats.fps },
        );

        zgui.pushFont(demo.font_large);
        zgui.separator();
        zgui.dummy(.{ .w = -1.0, .h = 20.0 });
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "zgui -");
        zgui.sameLine(.{});
        zgui.textWrapped("Zig bindings for 'dear imgui' library. " ++
            "Easy to use, hand-crafted API with default arguments, " ++
            "named parameters and Zig style text formatting.", .{});
        zgui.dummy(.{ .w = -1.0, .h = 20.0 });
        zgui.separator();
        zgui.popFont();

        if (zgui.collapsingHeader("Widgets: Main", .{})) {
            zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Button");
            if (zgui.button("Button 1", .{ .w = 200.0 })) {
                // 'Button 1' pressed.
            }
            zgui.sameLine(.{ .spacing = 20.0 });
            if (zgui.button("Button 2", .{ .h = 60.0 })) {
                // 'Button 2' pressed.
            }
            zgui.sameLine(.{});
            {
                const label = "Button 3 is special ;)";
                const s = zgui.calcTextSize(label, .{});
                _ = zgui.button(label, .{ .w = s[0] + 30.0 });
            }
            zgui.sameLine(.{});
            _ = zgui.button("Button 4", .{});
            _ = zgui.button("Button 5", .{ .w = -1.0, .h = 100.0 });

            zgui.pushStyleColor4f(.{ .idx = .text, .c = .{ 1.0, 0.0, 0.0, 1.0 } });
            _ = zgui.button("  Red Text Button  ", .{});
            zgui.popStyleColor(.{});

            zgui.sameLine(.{});
            zgui.pushStyleColor4f(.{ .idx = .text, .c = .{ 1.0, 1.0, 0.0, 1.0 } });
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
        }

        if (zgui.collapsingHeader("Widgets: Combo Box", .{})) {
            const static = struct {
                var selection_index: u32 = 0;
                var current_item: i32 = 0;
                var simple_enum_value: SimpleEnum = .first;
                var sparse_enum_value: SparseEnum = .first;
                var non_exhaustive_enum_value: NonExhaustiveEnum = .first;
            };

            const items = [_][:0]const u8{ "aaa", "bbb", "ccc", "ddd", "eee", "FFF", "ggg", "hhh" };
            if (zgui.beginCombo("Combo 0", .{ .preview_value = items[static.selection_index] })) {
                for (items, 0..) |item, index| {
                    const i = @as(u32, @intCast(index));
                    if (zgui.selectable(item, .{ .selected = static.selection_index == i }))
                        static.selection_index = i;
                }
                zgui.endCombo();
            }

            _ = zgui.combo("Combo 1", .{
                .current_item = &static.current_item,
                .items_separated_by_zeros = "Item 0\x00Item 1\x00Item 2\x00Item 3\x00\x00",
            });

            _ = zgui.comboFromEnum("simple enum", &static.simple_enum_value);
            _ = zgui.comboFromEnum("sparse enum", &static.sparse_enum_value);
            _ = zgui.comboFromEnum("non-exhaustive enum", &static.non_exhaustive_enum_value);
        }

        if (zgui.collapsingHeader("Widgets: Drag Sliders", .{})) {
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
            _ = zgui.dragFloatRange2(
                "Drag float range 2",
                .{ .current_min = &static.range[0], .current_max = &static.range[1] },
            );
            _ = zgui.dragInt("Drag int 1", .{ .v = &static.v1i });
            _ = zgui.dragInt2("Drag int 2", .{ .v = &static.v2i });
            _ = zgui.dragInt3("Drag int 3", .{ .v = &static.v3i });
            _ = zgui.dragInt4("Drag int 4", .{ .v = &static.v4i });
            _ = zgui.dragIntRange2(
                "Drag int range 2",
                .{ .current_min = &static.rangei[0], .current_max = &static.rangei[1] },
            );
            _ = zgui.dragScalar("Drag scalar (i8)", i8, .{ .v = &static.si8, .min = -20 });
            _ = zgui.dragScalarN(
                "Drag scalar N ([3]u16)",
                @TypeOf(static.vu16),
                .{ .v = &static.vu16, .max = 100 },
            );
            _ = zgui.dragScalar(
                "Drag scalar (f64)",
                f64,
                .{ .v = &static.sd, .min = -1.0, .max = 1.0, .speed = 0.005 },
            );
        }

        if (zgui.collapsingHeader("Widgets: Regular Sliders", .{})) {
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
            _ = zgui.sliderFloat("Slider float 1", .{ .v = &static.v1, .min = 0.0, .max = 1.0 });
            _ = zgui.sliderFloat2("Slider float 2", .{ .v = &static.v2, .min = -1.0, .max = 1.0 });
            _ = zgui.sliderFloat3("Slider float 3", .{ .v = &static.v3, .min = 0.0, .max = 1.0 });
            _ = zgui.sliderFloat4("Slider float 4", .{ .v = &static.v4, .min = 0.0, .max = 1.0 });
            _ = zgui.sliderInt("Slider int 1", .{ .v = &static.v1i, .min = 0, .max = 100 });
            _ = zgui.sliderInt2("Slider int 2", .{ .v = &static.v2i, .min = -20, .max = 20 });
            _ = zgui.sliderInt3("Slider int 3", .{ .v = &static.v3i, .min = 10, .max = 50 });
            _ = zgui.sliderInt4("Slider int 4", .{ .v = &static.v4i, .min = 0, .max = 10 });
            _ = zgui.sliderScalar(
                "Slider scalar (u8)",
                u8,
                .{ .v = &static.su8, .min = 0, .max = 100, .cfmt = "%Xh" },
            );
            _ = zgui.sliderScalarN(
                "Slider scalar N ([3]u16)",
                [3]u16,
                .{ .v = &static.vu16, .min = 1, .max = 100 },
            );
            _ = zgui.sliderAngle("Slider angle", .{ .vrad = &static.angle });
            _ = zgui.vsliderFloat(
                "VSlider float",
                .{ .w = 80.0, .h = 200.0, .v = &static.vsf, .min = 0.0, .max = 1.0 },
            );
            zgui.sameLine(.{});
            _ = zgui.vsliderInt(
                "VSlider int",
                .{ .w = 80.0, .h = 200.0, .v = &static.vsi, .min = 0, .max = 100 },
            );
            zgui.sameLine(.{});
            _ = zgui.vsliderScalar(
                "VSlider scalar (u8)",
                u8,
                .{ .w = 80.0, .h = 200.0, .v = &static.vsu8, .min = 0, .max = 200 },
            );
        }

        if (zgui.collapsingHeader("Widgets: Input with Keyboard", .{})) {
            const static = struct {
                var input_text_buf = [_:0]u8{0} ** 4;
                var input_text_multiline_buf = [_:0]u8{0} ** 4;
                var input_text_with_hint_buf = [_:0]u8{0} ** 4;
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
            zgui.separatorText("static input text");
            _ = zgui.inputText("Input text", .{ .buf = static.input_text_buf[0..] });
            _ = zgui.text("length of Input text {}", .{std.mem.len(@as([*:0]u8, static.input_text_buf[0..]))});

            _ = zgui.inputTextMultiline("Input text multiline", .{ .buf = static.input_text_multiline_buf[0..] });
            _ = zgui.text("length of Input text multiline {}", .{std.mem.len(@as([*:0]u8, static.input_text_multiline_buf[0..]))});
            _ = zgui.inputTextWithHint("Input text with hint", .{
                .hint = "Enter your name",
                .buf = static.input_text_with_hint_buf[0..],
            });
            _ = zgui.text("length of Input text with hint {}", .{std.mem.len(@as([*:0]u8, static.input_text_with_hint_buf[0..]))});

            zgui.separatorText("alloced input text");
            _ = zgui.inputText("Input text alloced", .{ .buf = demo.alloced_input_text_buf });
            _ = zgui.text("length of Input text alloced {}", .{std.mem.len(demo.alloced_input_text_buf.ptr)});
            _ = zgui.inputTextMultiline("Input text multiline alloced", .{ .buf = demo.alloced_input_text_multiline_buf });
            _ = zgui.text("length of Input text multiline {}", .{std.mem.len(demo.alloced_input_text_multiline_buf.ptr)});
            _ = zgui.inputTextWithHint("Input text with hint alloced", .{
                .hint = "Enter your name",
                .buf = demo.alloced_input_text_with_hint_buf,
            });
            _ = zgui.text("length of Input text with hint alloced {}", .{std.mem.len(demo.alloced_input_text_with_hint_buf.ptr)});

            zgui.separatorText("input numeric");
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
        }

        if (zgui.collapsingHeader("Widgets: Color Editor/Picker", .{})) {
            const static = struct {
                var col3: [3]f32 = .{ 0, 0, 0 };
                var col4: [4]f32 = .{ 0, 1, 0, 0 };
                var col3p: [3]f32 = .{ 0, 0, 0 };
                var col4p: [4]f32 = .{ 0, 0, 0, 0 };
            };
            _ = zgui.colorEdit3("Color edit 3", .{ .col = &static.col3 });
            _ = zgui.colorEdit4("Color edit 4", .{ .col = &static.col4 });
            _ = zgui.colorEdit4("Color edit 4 float", .{ .col = &static.col4, .flags = .{ .float = true } });
            _ = zgui.colorPicker3("Color picker 3", .{ .col = &static.col3p });
            _ = zgui.colorPicker4("Color picker 4", .{ .col = &static.col4p });
            _ = zgui.colorButton("color_button_id", .{ .col = .{ 0, 1, 0, 1 } });
        }

        if (zgui.collapsingHeader("Widgets: Trees", .{})) {
            if (zgui.treeNodeStrId("tree_id", "My Tree {d}", .{1})) {
                zgui.textUnformatted("Some content...");
                zgui.treePop();
            }
            if (zgui.collapsingHeader("Collapsing header 1", .{})) {
                zgui.textUnformatted("Some content...");
            }
        }

        if (zgui.collapsingHeader("Widgets: List Boxes", .{})) {
            const static = struct {
                var selection_index: u32 = 0;
            };
            const items = [_][:0]const u8{ "aaa", "bbb", "ccc", "ddd", "eee", "FFF", "ggg", "hhh" };
            if (zgui.beginListBox("List Box 0", .{})) {
                for (items, 0..) |item, index| {
                    const i = @as(u32, @intCast(index));
                    if (zgui.selectable(item, .{ .selected = static.selection_index == i }))
                        static.selection_index = i;
                }
                zgui.endListBox();
            }
        }

        if (zgui.collapsingHeader("Widgets: Image", .{})) {
            const tex_id = demo.gctx.lookupResource(demo.texture_view).?;
            zgui.image(tex_id, .{ .w = 512.0, .h = 512.0 });
            _ = zgui.imageButton("image_button_id", tex_id, .{ .w = 512.0, .h = 512.0 });
        }

        const draw_list = zgui.getBackgroundDrawList();
        draw_list.pushClipRect(.{ .pmin = .{ 0, 0 }, .pmax = .{ 400, 400 } });
        draw_list.addLine(.{
            .p1 = .{ 0, 0 },
            .p2 = .{ 400, 400 },
            .col = zgui.colorConvertFloat3ToU32([_]f32{ 1, 0, 1 }),
            .thickness = 5.0,
        });
        draw_list.popClipRect();

        draw_list.pushClipRectFullScreen();
        draw_list.addRectFilled(.{
            .pmin = .{ 100, 100 },
            .pmax = .{ 300, 200 },
            .col = zgui.colorConvertFloat3ToU32([_]f32{ 1, 1, 1 }),
            .rounding = 25.0,
        });
        draw_list.addRectFilledMultiColor(.{
            .pmin = .{ 100, 300 },
            .pmax = .{ 200, 400 },
            .col_upr_left = zgui.colorConvertFloat3ToU32([_]f32{ 1, 0, 0 }),
            .col_upr_right = zgui.colorConvertFloat3ToU32([_]f32{ 0, 1, 0 }),
            .col_bot_right = zgui.colorConvertFloat3ToU32([_]f32{ 0, 0, 1 }),
            .col_bot_left = zgui.colorConvertFloat3ToU32([_]f32{ 1, 1, 0 }),
        });
        draw_list.addQuadFilled(.{
            .p1 = .{ 150, 400 },
            .p2 = .{ 250, 400 },
            .p3 = .{ 200, 500 },
            .p4 = .{ 100, 500 },
            .col = 0xff_ff_ff_ff,
        });
        draw_list.addQuad(.{
            .p1 = .{ 170, 420 },
            .p2 = .{ 270, 420 },
            .p3 = .{ 220, 520 },
            .p4 = .{ 120, 520 },
            .col = zgui.colorConvertFloat3ToU32([_]f32{ 1, 0, 0 }),
            .thickness = 3.0,
        });
        draw_list.addText(.{ 130, 130 }, 0xff_00_00_ff, "The number is: {}", .{7});
        draw_list.addCircleFilled(.{
            .p = .{ 200, 600 },
            .r = 50,
            .col = zgui.colorConvertFloat3ToU32([_]f32{ 1, 1, 1 }),
        });
        draw_list.addCircle(.{
            .p = .{ 200, 600 },
            .r = 30,
            .col = zgui.colorConvertFloat3ToU32([_]f32{ 1, 0, 0 }),
            .thickness = 11,
        });
        draw_list.addPolyline(
            &.{ .{ 100, 700 }, .{ 200, 600 }, .{ 300, 700 }, .{ 400, 600 } },
            .{ .col = zgui.colorConvertFloat3ToU32([_]f32{ 0x11.0 / 0xff.0, 0xaa.0 / 0xff.0, 0 }), .thickness = 7 },
        );
        _ = draw_list.getClipRectMin();
        _ = draw_list.getClipRectMax();
        draw_list.popClipRect();

        if (zgui.collapsingHeader("Plot: Scatter", .{})) {
            zgui.plot.pushStyleVar1f(.{ .idx = .marker_size, .v = 3.0 });
            zgui.plot.pushStyleVar1f(.{ .idx = .marker_weight, .v = 1.0 });
            if (zgui.plot.beginPlot("Scatter Plot", .{ .flags = .{ .no_title = true } })) {
                zgui.plot.setupAxis(.x1, .{ .label = "xaxis" });
                zgui.plot.setupAxisLimits(.x1, .{ .min = 0, .max = 5 });
                zgui.plot.setupLegend(.{ .north = true, .east = true }, .{});
                zgui.plot.setupFinish();
                zgui.plot.plotScatterValues("y data", i32, .{ .v = &.{ 0, 1, 0, 1, 0, 1 } });
                zgui.plot.plotScatter("xy data", f32, .{
                    .xv = &.{ 0.1, 0.2, 0.5, 2.5 },
                    .yv = &.{ 0.1, 0.3, 0.5, 0.9 },
                });
                zgui.plot.endPlot();
            }
            zgui.plot.popStyleVar(.{ .count = 2 });
        }
    }
    zgui.end();

    if (zgui.begin("Plot", .{})) {
        if (zgui.plot.beginPlot("Line Plot", .{ .h = -1.0 })) {
            zgui.plot.setupAxis(.x1, .{ .label = "xaxis" });
            zgui.plot.setupAxisLimits(.x1, .{ .min = 0, .max = 5 });
            zgui.plot.setupLegend(.{ .south = true, .west = true }, .{});
            zgui.plot.setupFinish();
            zgui.plot.plotLineValues("y data", i32, .{ .v = &.{ 0, 1, 0, 1, 0, 1 } });
            zgui.plot.plotLine("xy data", f32, .{
                .xv = &.{ 0.1, 0.2, 0.5, 2.5 },
                .yv = &.{ 0.1, 0.3, 0.5, 0.9 },
            });
            zgui.plot.endPlot();
        }
    }
    zgui.end();

    // TODO: will not draw on screen for now
    demo.draw_list.reset();
    demo.draw_list.addCircle(.{
        .p = .{ 200, 700 },
        .r = 30,
        .col = zgui.colorConvertFloat3ToU32([_]f32{ 1, 1, 0 }),
        .thickness = 15 + 15 * @as(f32, @floatCast(@sin(demo.gctx.stats.time))),
    });

    node_editor_window(demo);
}

fn node_editor_window(demo: *DemoState) void {
    defer zgui.end();

    if (zgui.begin("Node editor", .{ .flags = .{ .no_saved_settings = true } })) {
        zgui.node_editor.setCurrentEditor(demo.node_editor);
        defer zgui.node_editor.setCurrentEditor(null);

        {
            zgui.node_editor.begin("NodeEditor", .{ 0, 0 });
            defer zgui.node_editor.end();

            zgui.node_editor.beginNode(1);
            {
                defer zgui.node_editor.endNode();

                zgui.textUnformatted("Node A");

                zgui.node_editor.beginPin(1, .input);
                {
                    defer zgui.node_editor.endPin();
                    zgui.textUnformatted("-> In");
                }

                zgui.sameLine(.{});

                zgui.node_editor.beginPin(2, .output);
                {
                    defer zgui.node_editor.endPin();
                    zgui.textUnformatted("Out ->");
                }
            }
        }
    }
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
    try zglfw.init();
    defer zglfw.terminate();

    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.posix.chdir(path) catch {};
    }

    zglfw.windowHintTyped(.client_api, .no_api);

    const window = try zglfw.Window.create(1600, 1000, window_title, null);
    defer window.destroy();
    window.setSizeLimits(400, 400, -1, -1);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    const demo = try create(allocator, window);
    defer destroy(allocator, demo);

    registerTests();

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();
        try update(demo);
        draw(demo);
    }
}
