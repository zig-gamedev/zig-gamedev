const std = @import("std");
const assert = std.debug.assert;

pub fn init() void {
    assert(getCurrentContext() == null);
    temp_buffer.resize(3 * 1024 + 1) catch unreachable;
    _ = createContext(null);
}

pub fn deinit() void {
    assert(getCurrentContext() != null);
    destroyContext(null);
    temp_buffer.deinit();
}

const createContext = zguiCreateContext;
extern fn zguiCreateContext(shared_font_atlas: ?*const anyopaque) Context;

const destroyContext = zguiDestroyContext;
extern fn zguiDestroyContext(ctx: ?Context) void;

const getCurrentContext = zguiGetCurrentContext;
extern fn zguiGetCurrentContext() ?Context;

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

const Context = *opaque {};
pub const DrawData = *opaque {};

pub const StyleColorIndex = enum(u32) {
    text,
    text_disabled,
    window_bg,
    // TODO: Add all the values.
};

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

/// `args: .{ p_open: ?*bool = null, flags: WindowFlags = .{} }`
pub fn begin(name: [:0]const u8, args: anytype) bool {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 0 and len <= 2)) @compileError("Invalid parameter count");
    if (@hasField(T, "p_open")) len -= 1;
    if (@hasField(T, "flags")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    return zguiBegin(
        name,
        if (@hasField(T, "p_open")) args.p_open else null,
        if (@hasField(T, "flags")) @bitCast(u32, @as(WindowFlags, args.flags)) else 0,
    );
}
extern fn zguiBegin(name: [*:0]const u8, p_open: ?*bool, flags: u32) bool;

/// `args: .{ offset_from_start_x: f32 = 0.0, spacing: f32 = -1.0 }`
pub fn sameLine(args: anytype) void {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 0 and len <= 2)) @compileError("Invalid parameter count");
    if (@hasField(T, "offset_from_start_x")) len -= 1;
    if (@hasField(T, "spacing")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    zguiSameLine(
        if (@hasField(T, "offset_from_start_x")) args.offset_from_start_x else 0.0,
        if (@hasField(T, "spacing")) args.spacing else -1.0,
    );
}
extern fn zguiSameLine(offset_from_start_x: f32, spacing: f32) void;

/// `args: .{ current_item: *i32, items_separated_by_zeros: [*:0]const u8, popup_max_height_in_items: i32 = -1 }`
pub fn combo(label: [*:0]const u8, args: anytype) bool {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 2 and len <= 3)) @compileError("Invalid parameter count");
    if (@hasField(T, "current_item")) len -= 1;
    if (@hasField(T, "items_separated_by_zeros")) len -= 1;
    if (@hasField(T, "popup_max_height_in_items")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    return zguiCombo0(
        label,
        args.current_item,
        args.items_separated_by_zeros,
        if (@hasField(T, "popup_max_height_in_items")) args.popup_max_height_in_items else -1,
    );
}
extern fn zguiCombo0(
    label: [*:0]const u8,
    current_item: *i32,
    items_separated_by_zeros: [*:0]const u8,
    popup_max_height_in_items: i32,
) bool;

pub const ComboFlags = packed struct {
    popup_align_left: bool = false,
    height_small: bool = false,
    height_regular: bool = false,
    height_large: bool = false,
    height_largest: bool = false,
    no_arrow_button: bool = false,
    no_preview: bool = false,

    _padding: u25 = 0,

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

/// `args: .{ preview_value: [*:0]const u8, flags: ComboFlags = .{} }`
pub fn beginCombo(label: [*:0]const u8, args: anytype) bool {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 1 and len <= 2)) @compileError("Invalid parameter count");
    if (@hasField(T, "preview_value")) len -= 1;
    if (@hasField(T, "flags")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    return zguiBeginCombo(
        label,
        args.preview_value,
        if (@hasField(T, "flags")) @bitCast(u32, @as(ComboFlags, args.flags)) else 0,
    );
}
extern fn zguiBeginCombo(label: [*:0]const u8, preview_value: ?[*:0]const u8, flags: u32) bool;

pub const endCombo = zguiEndCombo;
extern fn zguiEndCombo() void;

pub const SelectableFlags = packed struct {
    dont_close_popups: bool = false,
    span_all_colums: bool = false,
    allow_double_click: bool = false,
    disabled: bool = false,
    allow_item_overlap: bool = false,

    _padding: u27 = 0,

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

/// `args: .{ selected: bool = false, flags: SelectableFlags = .{}, w: f32 = 0, h: f32 = 0 }`
/// `args: .{ p_selected: *bool, flags: SelectableFlags = .{}, w: f32 = 0, h: f32 = 0 }`
pub fn selectable(label: [*:0]const u8, args: anytype) bool {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 0 and len <= 4)) @compileError("Invalid parameter count");
    if (@hasField(T, "w")) len -= 1;
    if (@hasField(T, "h")) len -= 1;
    if (@hasField(T, "flags")) len -= 1;

    if (@hasField(T, "p_selected")) {
        len -= 1;
        return zguiSelectable1(
            label,
            args.p_selected,
            if (@hasField(T, "flags")) @bitCast(u32, @as(SelectableFlags, args.flags)) else 0,
            if (@hasField(T, "w")) args.w else 0.0,
            if (@hasField(T, "h")) args.h else 0.0,
        );
    } else {
        if (@hasField(T, "selected")) len -= 1;
        return zguiSelectable0(
            label,
            if (@hasField(T, "selected")) args.selected else false,
            if (@hasField(T, "flags")) @bitCast(u32, @as(SelectableFlags, args.flags)) else 0,
            if (@hasField(T, "w")) args.w else 0.0,
            if (@hasField(T, "h")) args.h else 0.0,
        );
    }
    if (len != 0) @compileError("Invalid parameter name(s)");
}
extern fn zguiSelectable0(label: [*:0]const u8, selected: bool, flags: u32, w: f32, h: f32) bool;
extern fn zguiSelectable1(label: [*:0]const u8, p_selected: *bool, flags: u32, w: f32, h: f32) bool;

pub const setItemDefaultFocus = zguiSetItemDefaultFocus;
extern fn zguiSetItemDefaultFocus() void;

/// `args: .{ v: *f32, v_min: f32, v_max: f32, format: [*:0]const u8 = "%.3f", flags: SliderFlags = .{} }`
pub fn sliderFloat(label: [*:0]const u8, args: anytype) bool {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 3 and len <= 5)) @compileError("Invalid parameter count");
    if (@hasField(T, "v")) len -= 1;
    if (@hasField(T, "v_min")) len -= 1;
    if (@hasField(T, "v_max")) len -= 1;
    if (@hasField(T, "format")) len -= 1;
    if (@hasField(T, "flags")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    return zguiSliderFloat(
        label,
        args.v,
        args.v_min,
        args.v_max,
        if (@hasField(T, "format")) args.format else "%.3f",
        if (@hasField(T, "flags")) @bitCast(u32, @as(SliderFlags, args.flags)) else 0,
    );
}
extern fn zguiSliderFloat(
    label: [*:0]const u8,
    v: *f32,
    v_min: f32,
    v_max: f32,
    format: [*:0]const u8,
    flags: u32,
) bool;

/// `args: .{ v: *i32, v_min: i32, v_max: i32, format: [*:0]const u8 = "%d", flags: SliderFlags = .{} }`
pub fn sliderInt(label: [*:0]const u8, args: anytype) bool {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 3 and len <= 5)) @compileError("Invalid parameter count");
    if (@hasField(T, "v")) len -= 1;
    if (@hasField(T, "v_min")) len -= 1;
    if (@hasField(T, "v_max")) len -= 1;
    if (@hasField(T, "format")) len -= 1;
    if (@hasField(T, "flags")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    return zguiSliderInt(
        label,
        args.v,
        args.v_min,
        args.v_max,
        if (@hasField(T, "format")) args.format else "%d",
        if (@hasField(T, "flags")) @bitCast(u32, @as(SliderFlags, args.flags)) else 0,
    );
}
extern fn zguiSliderInt(
    label: [*:0]const u8,
    v: *i32,
    v_min: i32,
    v_max: i32,
    format: [*:0]const u8,
    flags: u32,
) bool;

pub fn textUnformatted(txt: []const u8) void {
    zguiTextUnformatted(txt.ptr, txt.ptr + txt.len);
}
pub fn textUnformattedColored(color: [4]f32, txt: []const u8) void {
    pushStyleColor(.text, .{ .color = color });
    textUnformatted(txt);
    popStyleColor(.{});
}

pub fn text(comptime fmt: []const u8, args: anytype) void {
    const result = format(fmt, args);
    zguiTextUnformatted(result.ptr, result.ptr + result.len);
}
pub fn textColored(color: [4]f32, comptime fmt: []const u8, args: anytype) void {
    pushStyleColor(.text, .{ .color = color });
    text(fmt, args);
    popStyleColor(.{});
}
extern fn zguiTextUnformatted(txt: [*]const u8, txt_end: [*]const u8) void;

pub fn textDisabled(comptime fmt: []const u8, args: anytype) void {
    zguiTextDisabled("%s", formatZ(fmt, args).ptr);
}
extern fn zguiTextDisabled(fmt: [*:0]const u8, ...) void;

pub fn textWrapped(comptime fmt: []const u8, args: anytype) void {
    zguiTextWrapped("%s", formatZ(fmt, args).ptr);
}
extern fn zguiTextWrapped(fmt: [*:0]const u8, ...) void;

pub fn bulletText(comptime fmt: []const u8, args: anytype) void {
    bullet();
    text(fmt, args);
}

pub fn labelText(label: [*:0]const u8, comptime fmt: []const u8, args: anytype) void {
    zguiLabelText(label, "%s", formatZ(fmt, args).ptr);
}
extern fn zguiLabelText(label: [*:0]const u8, fmt: [*:0]const u8, ...) void;

/// `args: .{ w: f32 = 0.0, h: f32 = 0.0 }`
pub fn button(label: [*:0]const u8, args: anytype) bool {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 0 and len <= 2)) @compileError("Invalid parameter count");
    if (@hasField(T, "w")) len -= 1;
    if (@hasField(T, "h")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    return zguiButton(
        label,
        if (@hasField(T, "w")) args.w else 0.0,
        if (@hasField(T, "h")) args.h else 0.0,
    );
}
extern fn zguiButton(label: [*:0]const u8, w: f32, h: f32) bool;

pub const smallButton = zguiButton;
extern fn zguiSmallButton(label: [*:0]const u8) bool;

/// `args: .{ w: f32, h: f32, flags: ButtonFlags = .{} }`
pub fn invisibleButton(str_id: [*:0]const u8, args: anytype) bool {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 2 and len <= 3)) @compileError("Invalid parameter count");
    if (@hasField(T, "w")) len -= 1;
    if (@hasField(T, "h")) len -= 1;
    if (@hasField(T, "flags")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    return zguiInvisibleButton(
        str_id,
        args.w,
        args.h,
        if (@hasField(T, "flags")) @bitCast(u32, @as(ButtonFlags, args.flags)) else 0,
    );
}
extern fn zguiInvisibleButton(str_id: [*:0]const u8, w: f32, h: f32, flags: u32) bool;

/// `args: .{ dir: Direction }`
pub fn arrowButton(label: [*:0]const u8, args: anytype) bool {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (len != 1) @compileError("Invalid parameter count");
    if (@hasField(T, "dir")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    return zguiArrowButton(label, args.dir);
}
extern fn zguiArrowButton(label: [*:0]const u8, dir: Direction) bool;

pub const bullet = zguiBullet;
extern fn zguiBullet() void;

/// `args: .{ active: bool }`
/// `args: .{ v: *i32, v_button: i32 }`
pub fn radioButton(label: [*:0]const u8, args: anytype) bool {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 1 and len <= 2)) @compileError("Invalid parameter count");

    if (len == 1 and @hasField(T, "active")) {
        if (@hasField(T, "active")) len -= 1;
        if (len != 0) @compileError("Invalid parameter name(s)");

        return zguiRadioButton0(label, args.active);
    } else {
        if (@hasField(T, "v")) len -= 1;
        if (@hasField(T, "v_button")) len -= 1;
        if (len != 0) @compileError("Invalid parameter name(s)");

        return zguiRadioButton1(label, args.v, args.v_button);
    }
}
extern fn zguiRadioButton0(label: [*:0]const u8, active: bool) bool;
extern fn zguiRadioButton1(label: [*:0]const u8, v: *i32, v_button: i32) bool;

/// `args: .{ v: *bool }`
pub fn checkbox(label: [*:0]const u8, args: anytype) bool {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (len != 1) @compileError("Invalid parameter count");
    if (@hasField(T, "v")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    return zguiCheckbox(label, args.v);
}
extern fn zguiCheckbox(label: [*:0]const u8, v: *bool) bool;

/// `args: .{ flags: *i32, flags_value: i32 }`
/// `args: .{ flags: *u32, flags_value: u32 }`
pub fn checkboxFlags(label: [*:0]const u8, args: anytype) bool {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (len != 2) @compileError("Invalid parameter count");
    if (@hasField(T, "flags")) len -= 1;
    if (@hasField(T, "flags_value")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    if (@typeInfo(@TypeOf(args.flags)).Pointer.child == i32) {
        return zguiCheckboxFlags0(label, args.flags, args.flags_value);
    } else {
        return zguiCheckboxFlags1(label, args.flags, args.flags_value);
    }
}
extern fn zguiCheckboxFlags0(label: [*:0]const u8, flags: *i32, flags_value: i32) bool;
extern fn zguiCheckboxFlags1(label: [*:0]const u8, flags: *u32, flags_value: u32) bool;

/// `args: .{ fraction: f32, w: f32 = -math.f32_min, h: f32 = 0.0, overlay: ?[*:0]const u8 = null }`
pub fn progressBar(args: anytype) void {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 1 and len <= 4)) @compileError("Invalid parameter count");
    if (@hasField(T, "fraction")) len -= 1;
    if (@hasField(T, "w")) len -= 1;
    if (@hasField(T, "h")) len -= 1;
    if (@hasField(T, "overlay")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    zguiProgressBar(
        args.fraction,
        if (@hasField(T, "w")) args.w else -std.math.f32_min,
        if (@hasField(T, "h")) args.h else 0.0,
        if (@hasField(T, "overlay")) args.overlay else null,
    );
}
extern fn zguiProgressBar(fraction: f32, w: f32, h: f32, overlay: ?[*:0]const u8) void;

/// `args: .{ color: u32 }`
/// `args: .{ color: [4]f32 }`
pub fn pushStyleColor(idx: StyleColorIndex, args: anytype) void {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (len != 1) @compileError("Invalid parameter count");
    if (@hasField(T, "color")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    if (@TypeOf(args.color) == u32) {
        zguiPushStyleColor0(idx, args.color);
    } else {
        zguiPushStyleColor1(idx, &args.color);
    }
}
extern fn zguiPushStyleColor0(idx: StyleColorIndex, color: u32) void;
extern fn zguiPushStyleColor1(idx: StyleColorIndex, color: *const [4]f32) void;

/// `args: .{ count: i32 = 1 }`
pub fn popStyleColor(args: anytype) void {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 0 and len <= 1)) @compileError("Invalid parameter count");
    if (@hasField(T, "count")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    zguiPopStyleColor(
        if (@hasField(T, "count")) args.count else 1,
    );
}
extern fn zguiPopStyleColor(count: i32) void;

/// `args: .{ disabled: bool = true }`
pub fn beginDisabled(args: anytype) void {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (!(len >= 0 and len <= 1)) @compileError("Invalid parameter count");
    if (@hasField(T, "disabled")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    zguiBeginDisabled(
        if (@hasField(T, "disabled")) args.disabled else true,
    );
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

/// `args: .{ w: f32, h: f32 }`
pub fn dummy(args: anytype) void {
    const T = @TypeOf(args);
    comptime var len = getArgsLen(T);

    if (len != 2) @compileError("Invalid parameter count");
    if (@hasField(T, "w")) len -= 1;
    if (@hasField(T, "h")) len -= 1;
    if (len != 0) @compileError("Invalid parameter name(s)");

    zguiDummy(args.w, args.h);
}
extern fn zguiDummy(w: f32, h: f32) void;

pub const newFrame = zguiNewFrame;
extern fn zguiNewFrame() void;

pub const render = zguiRender;
extern fn zguiRender() void;

pub const getDrawData = zguiGetDrawData;
extern fn zguiGetDrawData() DrawData;

pub const showDemoWindow = zguiShowDemoWindow;
extern fn zguiShowDemoWindow(p_open: ?*bool) void;

//

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

fn getArgsLen(comptime T: type) comptime_int {
    const type_info = @typeInfo(T);
    const len = type_info.Struct.fields.len;
    if (len > 0 and type_info.Struct.is_tuple) {
        @compileError("Expected struct with named fields but got tuple");
    }
    return len;
}
