const std = @import("std");
const assert = std.debug.assert;

pub fn init() Context {
    assert(getCurrentContext() == null);
    temp_buffer.resize(3 * 1024 + 1) catch unreachable;
    return createContext(null);
}

pub fn deinit() void {
    assert(getCurrentContext() != null);
    destroyContext(null);
    temp_buffer.deinit();
}

pub const createContext = zguiCreateContext;
extern fn zguiCreateContext(shared_font_atlas: ?*const anyopaque) Context;

pub const destroyContext = zguiDestroyContext;
extern fn zguiDestroyContext(ctx: ?Context) void;

pub const getCurrentContext = zguiGetCurrentContext;
extern fn zguiGetCurrentContext() ?Context;

pub const setCurrentContext = zguiSetCurrentContext;
extern fn zguiSetCurrentContext(ctx: ?Context) void;

pub const io = struct {
    pub const getWantCaptureMouse = zguiIoGetWantCaptureMouse;
    extern fn zguiIoGetWantCaptureMouse() bool;

    pub const getWantCaptureKeyboard = zguiIoGetWantCaptureKeyboard;
    extern fn zguiIoGetWantCaptureKeyboard() bool;

    pub const addFontFromFile = zguiIoAddFontFromFile;
    extern fn zguiIoAddFontFromFile(filename: [*:0]const u8, size_pixels: f32) void;

    pub const setIniFilename = zguiIoSetIniFilename;
    extern fn zguiIoSetIniFilename(filename: [*:0]const u8) void;

    pub const setDisplaySize = zguiIoSetDisplaySize;
    extern fn zguiIoSetDisplaySize(width: f32, height: f32) void;

    pub const setDisplayFramebufferScale = zguiIoSetDisplayFramebufferScale;
    extern fn zguiIoSetDisplayFramebufferScale(sx: f32, sy: f32) void;
};

pub const Context = *opaque {};
pub const DrawData = *opaque {};

pub const WindowFlags = packed struct {
    no_title_bar: bool = false,
    no_resize: bool = false,
    no_move: bool = false,
    no_scrollbar: bool = false,
    no_scroll_with_mouse: bool = false,
    no_collapse: bool = false,
    always_auto_resize: bool = false,
    no_background: bool = false,
    no_saved_settings: bool = false,
    no_mouse_inputs: bool = false,
    menu_bar: bool = false,
    horizontal_scrollbar: bool = false,
    no_focus_on_appearing: bool = false,
    no_bring_to_front_on_focus: bool = false,
    always_vertical_scrollbar: bool = false,
    always_horizontal_scrollbar: bool = false,
    always_use_window_padding: bool = false,
    no_nav_inputs: bool = false,
    no_nav_focus: bool = false,
    unsaved_document: bool = false,

    _padding: u12 = 0,

    pub const no_nav = WindowFlags{ .no_nav_inputs = true, .no_nav_focus = true };
    pub const no_decoration = WindowFlags{
        .no_title_bar = true,
        .no_resize = true,
        .no_scrollbar = true,
        .no_collapse = true,
    };
    pub const no_inputs = WindowFlags{ .no_mouse_inputs = true, .no_nav_inputs = true, .no_nav_focus = true };

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

pub const SliderFlags = packed struct {
    _reserved0: bool = false,
    _reserved1: bool = false,
    _reserved2: bool = false,
    _reserved3: bool = false,
    always_clamp: bool = false,
    logarithmic: bool = false,
    no_round_to_format: bool = false,
    no_input: bool = false,

    _padding: u24 = 0,

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

pub const ButtonFlags = packed struct {
    mouse_button_left: bool = false,
    mouse_button_right: bool = false,
    mouse_button_middle: bool = false,

    _padding: u29 = 0,

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

pub const Direction = enum(i32) {
    none = -1,
    left = 0,
    right = 1,
    up = 2,
    down = 3,
};

pub fn begin(name: [:0]const u8, p_open: ?*bool, flags: WindowFlags) bool {
    return zguiBegin(name, p_open, @bitCast(u32, flags));
}
extern fn zguiBegin(name: [*:0]const u8, p_open: ?*bool, flags: u32) bool;

pub fn sameLine(args: struct { offset_from_start_x: f32 = 0.0, spacing: f32 = -1.0 }) void {
    zguiSameLine(args.offset_from_start_x, args.spacing);
}
extern fn zguiSameLine(offset_from_start_x: f32, spacing: f32) void;

pub fn comboStr(
    label: [*:0]const u8,
    current_item: *i32,
    items_separated_by_zeros: [:0]const u8,
    popup_max_height_in_items: i32,
) bool {
    return zguiComboStr(label, current_item, items_separated_by_zeros, popup_max_height_in_items);
}
extern fn zguiComboStr(
    label: [*:0]const u8,
    current_item: *i32,
    items_separated_by_zeros: [*:0]const u8,
    popup_max_height_in_items: i32,
) bool;

pub fn sliderFloat(
    label: [*:0]const u8,
    v: *f32,
    v_min: f32,
    v_max: f32,
    args: struct {
        format: [:0]const u8 = "%.3f",
        flags: SliderFlags = .{},
    },
) bool {
    return zguiSliderFloat(label, v, v_min, v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiSliderFloat(
    label: [*:0]const u8,
    v: *f32,
    v_min: f32,
    v_max: f32,
    format: [*:0]const u8,
    flags: u32,
) bool;

pub fn sliderInt(
    label: [*:0]const u8,
    v: *i32,
    v_min: i32,
    v_max: i32,
    args: struct {
        format: [:0]const u8 = "%d",
        flags: SliderFlags = .{},
    },
) bool {
    return zguiSliderInt(label, v, v_min, v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiSliderInt(
    label: [*:0]const u8,
    v: *i32,
    v_min: i32,
    v_max: i32,
    format: [*:0]const u8,
    flags: u32,
) bool;

var temp_buffer = std.ArrayList(u8).init(std.heap.c_allocator);

fn format(comptime fmt: []const u8, args: anytype) []const u8 {
    const len = std.fmt.count(fmt, args);
    if (len > temp_buffer.items.len) temp_buffer.resize(len + 64) catch unreachable;
    return std.fmt.bufPrint(temp_buffer.items, fmt, args) catch unreachable;
}

fn formatZ(comptime fmt: []const u8, args: anytype) [:0]const u8 {
    const len = std.fmt.count(fmt ++ "\x00", args);
    if (len > temp_buffer.items.len) temp_buffer.resize(len + 64) catch unreachable;
    return std.fmt.bufPrintZ(temp_buffer.items, fmt, args) catch unreachable;
}

// Widgets: Text

pub fn textUnformatted(txt: []const u8) void {
    zguiTextUnformatted(txt.ptr, txt.ptr + txt.len);
}
pub fn text(comptime fmt: []const u8, args: anytype) void {
    const result = format(fmt, args);
    zguiTextUnformatted(result.ptr, result.ptr + result.len);
}
extern fn zguiTextUnformatted(txt: [*]const u8, txt_end: [*]const u8) void;

pub fn textColored(color: [4]f32, comptime fmt: []const u8, args: anytype) void {
    zguiTextColored(&color, "%s", formatZ(fmt, args).ptr);
}
extern fn zguiTextColored(color: *const [4]f32, fmt: [*:0]const u8, ...) void;

pub fn textDisabled(comptime fmt: []const u8, args: anytype) void {
    zguiTextDisabled("%s", formatZ(fmt, args).ptr);
}
extern fn zguiTextDisabled(fmt: [*:0]const u8, ...) void;

pub fn textWrapped(comptime fmt: []const u8, args: anytype) void {
    zguiTextWrapped("%s", formatZ(fmt, args).ptr);
}
extern fn zguiTextWrapped(fmt: [*:0]const u8, ...) void;

pub fn bulletText(comptime fmt: []const u8, args: anytype) void {
    zguiBulletText("%s", formatZ(fmt, args).ptr);
}
extern fn zguiBulletText(fmt: [*:0]const u8, ...) void;

pub fn labelText(label: [*:0]const u8, comptime fmt: []const u8, args: anytype) void {
    zguiLabelText(label, "%s", formatZ(fmt, args).ptr);
}
extern fn zguiLabelText(label: [*:0]const u8, fmt: [*:0]const u8, ...) void;

// Widgets: Main

pub fn button(label: [*:0]const u8, size: struct { w: f32 = 0.0, h: f32 = 0.0 }) bool {
    return zguiButton(label, size.w, size.h);
}
extern fn zguiButton(label: [*:0]const u8, w: f32, h: f32) bool;

pub const smallButton = zguiButton;
extern fn zguiSmallButton(label: [*:0]const u8) bool;

pub fn invisibleButton(str_id: [*:0]const u8, args: struct { w: f32, h: f32, flags: ButtonFlags = .{} }) bool {
    return zguiInvisibleButton(str_id, args.w, args.h, @bitCast(u32, args.flags));
}
extern fn zguiInvisibleButton(str_id: [*:0]const u8, w: f32, h: f32, flags: u32) bool;

pub const arrowButton = zguiArrowButton;
extern fn zguiArrowButton(label: [*:0]const u8, dir: Direction) bool;

pub const radioButtonIntPtr = zguiRadioButtonIntPtr;
extern fn zguiRadioButtonIntPtr(label: [*:0]const u8, v: *i32, v_button: i32) bool;

pub const checkbox = zguiCheckbox;
extern fn zguiCheckbox(label: [*:0]const u8, v: *bool) bool;

pub fn beginDisabled(args: struct { disabled: bool = true }) void {
    zguiBeginDisabled(args.disabled);
}
extern fn zguiBeginDisabled(disabled: bool) void;

pub const endDisabled = zguiEndDisabled;
extern fn zguiEndDisabled() void;

pub const end = zguiEnd;
extern fn zguiEnd() void;

pub const spacing = zguiSpacing;
extern fn zguiSpacing() void;

pub const newLine = zguiNewLine;
extern fn zguiNewLine() void;

pub const separator = zguiSeparator;
extern fn zguiSeparator() void;

pub const dummy = zguiDummy;
extern fn zguiDummy(w: f32, h: f32) void;

pub const newFrame = zguiNewFrame;
extern fn zguiNewFrame() void;

pub const render = zguiRender;
extern fn zguiRender() void;

pub const getDrawData = zguiGetDrawData;
extern fn zguiGetDrawData() DrawData;

pub const showDemoWindow = zguiShowDemoWindow;
extern fn zguiShowDemoWindow(p_open: ?*bool) void;
