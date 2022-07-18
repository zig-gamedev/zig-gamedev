//--------------------------------------------------------------------------------------------------
const std = @import("std");
const assert = std.debug.assert;
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
/// `fn createContext(shared_font_atlas: ?*const anyopaque) Context`
const createContext = zguiCreateContext;
extern fn zguiCreateContext(shared_font_atlas: ?*const anyopaque) Context;

/// `fn destroyContext(ctx: ?Context) void`
const destroyContext = zguiDestroyContext;
extern fn zguiDestroyContext(ctx: ?Context) void;

/// `fn getCurrentContext() ?Context`
const getCurrentContext = zguiGetCurrentContext;
extern fn zguiGetCurrentContext() ?Context;
//--------------------------------------------------------------------------------------------------
pub const io = struct {
    /// `pub fn zguiIoGetWantCaptureMouse() bool`
    pub const getWantCaptureMouse = zguiIoGetWantCaptureMouse;
    extern fn zguiIoGetWantCaptureMouse() bool;

    /// `pub fn zguiIoGetWantCaptureKeyboard() bool`
    pub const getWantCaptureKeyboard = zguiIoGetWantCaptureKeyboard;
    extern fn zguiIoGetWantCaptureKeyboard() bool;

    pub fn addFontFromFile(filename: [:0]const u8, size_pixels: f32) void {
        zguiIoAddFontFromFile(filename, size_pixels);
    }
    extern fn zguiIoAddFontFromFile(filename: [*:0]const u8, size_pixels: f32) void;

    pub fn setIniFilename(filename: [:0]const u8) void {
        zguiIoSetIniFilename(filename);
    }
    extern fn zguiIoSetIniFilename(filename: [*:0]const u8) void;

    /// `pub fn setDisplaySize(width: f32, height: f32) void`
    pub const setDisplaySize = zguiIoSetDisplaySize;
    extern fn zguiIoSetDisplaySize(width: f32, height: f32) void;

    /// `pub fn setDisplayFramebufferScale(sx: f32, sy: f32) void`
    pub const setDisplayFramebufferScale = zguiIoSetDisplayFramebufferScale;
    extern fn zguiIoSetDisplayFramebufferScale(sx: f32, sy: f32) void;
};
//--------------------------------------------------------------------------------------------------
const Context = *opaque {};
pub const DrawData = *opaque {};
//--------------------------------------------------------------------------------------------------
pub const StyleColorIndex = enum(u32) {
    text,
    text_disabled,
    window_bg,
    // TODO: Add all the values.
};
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
pub const ButtonFlags = packed struct {
    mouse_button_left: bool = false,
    mouse_button_right: bool = false,
    mouse_button_middle: bool = false,

    _padding: u29 = 0,

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};
//--------------------------------------------------------------------------------------------------
pub const Direction = enum(i32) {
    none = -1,
    left = 0,
    right = 1,
    up = 2,
    down = 3,
};
//--------------------------------------------------------------------------------------------------
pub const DataType = enum(u32) { I8, U8, I16, U16, I32, U32, I64, U64, F32, F64 };
//--------------------------------------------------------------------------------------------------
pub const Condition = enum(u32) {
    none = 0,
    always = 1,
    once = 2,
    first_use_ever = 4,
    appearing = 8,
};
//--------------------------------------------------------------------------------------------------
const SetNextWindowPos = struct {
    x: f32,
    y: f32,
    cond: Condition = .none,
    pivot_x: f32 = 0.0,
    pivot_y: f32 = 0.0,
};
pub fn setNextWindowPos(args: SetNextWindowPos) void {
    zguiSetNextWindowPos(args.x, args.y, args.cond, args.pivot_x, args.pivot_y);
}
extern fn zguiSetNextWindowPos(x: f32, y: f32, cond: Condition, pivot_x: f32, pivot_y: f32) void;
//--------------------------------------------------------------------------------------------------
const SetNextWindowSize = struct {
    w: f32,
    h: f32,
    cond: Condition = .none,
};
pub fn setNextWindowSize(args: SetNextWindowSize) void {
    zguiSetNextWindowSize(args.w, args.h, args.cond);
}
extern fn zguiSetNextWindowSize(w: f32, h: f32, cond: Condition) void;
//--------------------------------------------------------------------------------------------------
const Begin = struct {
    p_open: ?*bool = null,
    flags: WindowFlags = .{},
};
pub fn begin(name: [:0]const u8, args: Begin) bool {
    return zguiBegin(name, args.p_open, @bitCast(u32, args.flags));
}
extern fn zguiBegin(name: [*:0]const u8, p_open: ?*bool, flags: u32) bool;
//--------------------------------------------------------------------------------------------------
/// `pub fn end() void`
pub const end = zguiEnd;
extern fn zguiEnd() void;
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
const Selectable = struct {
    selected: bool = false,
    flags: SelectableFlags = .{},
    w: f32 = 0,
    h: f32 = 0,
};
pub fn selectable(label: [:0]const u8, args: Selectable) bool {
    return zguiSelectable(label, args.selected, @bitCast(u32, args.flags), args.w, args.h);
}
extern fn zguiSelectable(label: [*:0]const u8, selected: bool, flags: u32, w: f32, h: f32) bool;
//--------------------------------------------------------------------------------------------------
const SelectableStatePtr = struct {
    p_selected: *bool,
    flags: SelectableFlags = .{},
    w: f32 = 0,
    h: f32 = 0,
};
pub fn selectableStatePtr(label: [:0]const u8, args: SelectableStatePtr) bool {
    return zguiSelectableStatePtr(label, args.p_selected, @bitCast(u32, args.flags), args.w, args.h);
}
extern fn zguiSelectableStatePtr(label: [*:0]const u8, p_selected: *bool, flags: u32, w: f32, h: f32) bool;

//--------------------------------------------------------------------------------------------------
const PushStyleColor = struct {
    color: [4]f32,
};
pub fn pushStyleColor(idx: StyleColorIndex, args: PushStyleColor) void {
    zguiPushStyleColor(idx, &args.color);
}
extern fn zguiPushStyleColor(idx: StyleColorIndex, color: *const [4]f32) void;
//--------------------------------------------------------------------------------------------------
const PopStyleColor = struct {
    count: i32 = 1,
};
pub fn popStyleColor(args: PopStyleColor) void {
    zguiPopStyleColor(args.count);
}
extern fn zguiPopStyleColor(count: i32) void;
//--------------------------------------------------------------------------------------------------
const BeginDisabled = struct {
    disabled: bool = true,
};
pub fn beginDisabled(args: BeginDisabled) void {
    zguiBeginDisabled(args.disabled);
}
extern fn zguiBeginDisabled(disabled: bool) void;
//--------------------------------------------------------------------------------------------------
/// `pub fn endDisabled() void`
pub const endDisabled = zguiEndDisabled;
extern fn zguiEndDisabled() void;
//--------------------------------------------------------------------------------------------------
//
// Cursor / Layout
//
//--------------------------------------------------------------------------------------------------
/// `pub fn separator() void`
pub const separator = zguiSeparator;
extern fn zguiSeparator() void;
//--------------------------------------------------------------------------------------------------
const SameLine = struct {
    offset_from_start_x: f32 = 0.0,
    spacing: f32 = -1.0,
};
pub fn sameLine(args: SameLine) void {
    zguiSameLine(args.offset_from_start_x, args.spacing);
}
extern fn zguiSameLine(offset_from_start_x: f32, spacing: f32) void;
//--------------------------------------------------------------------------------------------------
/// `pub fn newLine() void`
pub const newLine = zguiNewLine;
extern fn zguiNewLine() void;
//--------------------------------------------------------------------------------------------------
/// `pub fn spacing() void`
pub const spacing = zguiSpacing;
extern fn zguiSpacing() void;
//--------------------------------------------------------------------------------------------------
const Dummy = struct {
    w: f32,
    h: f32,
};
pub fn dummy(args: Dummy) void {
    zguiDummy(args.w, args.h);
}
extern fn zguiDummy(w: f32, h: f32) void;
//--------------------------------------------------------------------------------------------------
/// `pub fn indent() void`
pub const indent = zguiIndent;
extern fn zguiIndent() void;
//--------------------------------------------------------------------------------------------------
/// `pub fn unindent() void`
pub const unindent = zguiUnindent;
extern fn zguiUnindent() void;
//--------------------------------------------------------------------------------------------------
//
// ID stack/scopes
//
//--------------------------------------------------------------------------------------------------
// TODO: Add functions.
//--------------------------------------------------------------------------------------------------
//
// Widgets: Text
//
//--------------------------------------------------------------------------------------------------
pub fn textUnformatted(txt: []const u8) void {
    zguiTextUnformatted(txt.ptr, txt.ptr + txt.len);
}
pub fn textUnformattedColored(color: [4]f32, txt: []const u8) void {
    pushStyleColor(.text, .{ .color = color });
    textUnformatted(txt);
    popStyleColor(.{});
}
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
pub fn textDisabled(comptime fmt: []const u8, args: anytype) void {
    zguiTextDisabled("%s", formatZ(fmt, args).ptr);
}
extern fn zguiTextDisabled(fmt: [*:0]const u8, ...) void;
//--------------------------------------------------------------------------------------------------
pub fn textWrapped(comptime fmt: []const u8, args: anytype) void {
    zguiTextWrapped("%s", formatZ(fmt, args).ptr);
}
extern fn zguiTextWrapped(fmt: [*:0]const u8, ...) void;
//--------------------------------------------------------------------------------------------------
pub fn bulletText(comptime fmt: []const u8, args: anytype) void {
    bullet();
    text(fmt, args);
}
//--------------------------------------------------------------------------------------------------
pub fn labelText(label: [:0]const u8, comptime fmt: []const u8, args: anytype) void {
    zguiLabelText(label, "%s", formatZ(fmt, args).ptr);
}
extern fn zguiLabelText(label: [*:0]const u8, fmt: [*:0]const u8, ...) void;
//--------------------------------------------------------------------------------------------------
//
// Widgets: Main
//
//--------------------------------------------------------------------------------------------------
const Button = struct {
    w: f32 = 0.0,
    h: f32 = 0.0,
};
pub fn button(label: [:0]const u8, args: Button) bool {
    return zguiButton(label, args.w, args.h);
}
extern fn zguiButton(label: [*:0]const u8, w: f32, h: f32) bool;
//--------------------------------------------------------------------------------------------------
pub fn smallButton(label: [:0]const u8) bool {
    return zguiSmallButton(label);
}
extern fn zguiSmallButton(label: [*:0]const u8) bool;
//--------------------------------------------------------------------------------------------------
const InvisibleButton = struct {
    w: f32,
    h: f32,
    flags: ButtonFlags = .{},
};
pub fn invisibleButton(str_id: [:0]const u8, args: InvisibleButton) bool {
    return zguiInvisibleButton(str_id, args.w, args.h, @bitCast(u32, args.flags));
}
extern fn zguiInvisibleButton(str_id: [*:0]const u8, w: f32, h: f32, flags: u32) bool;
//--------------------------------------------------------------------------------------------------
const ArrowButton = struct {
    dir: Direction,
};
pub fn arrowButton(label: [:0]const u8, args: ArrowButton) bool {
    return zguiArrowButton(label, args.dir);
}
extern fn zguiArrowButton(label: [*:0]const u8, dir: Direction) bool;
//--------------------------------------------------------------------------------------------------
/// `pub fn bullet() void`
pub const bullet = zguiBullet;
extern fn zguiBullet() void;
//--------------------------------------------------------------------------------------------------
const RadioButton = struct {
    active: bool,
};
pub fn radioButton(label: [:0]const u8, args: RadioButton) bool {
    return zguiRadioButton(label, args.active);
}
extern fn zguiRadioButton(label: [*:0]const u8, active: bool) bool;
//--------------------------------------------------------------------------------------------------
const RadioButtonStatePtr = struct {
    v: *i32,
    v_button: i32,
};
pub fn radioButtonStatePtr(label: [:0]const u8, args: RadioButtonStatePtr) bool {
    return zguiRadioButtonStatePtr(label, args.v, args.v_button);
}
extern fn zguiRadioButtonStatePtr(label: [*:0]const u8, v: *i32, v_button: i32) bool;
//--------------------------------------------------------------------------------------------------
const Checkbox = struct {
    v: *bool,
};
pub fn checkbox(label: [:0]const u8, args: Checkbox) bool {
    return zguiCheckbox(label, args.v);
}
extern fn zguiCheckbox(label: [*:0]const u8, v: *bool) bool;
//--------------------------------------------------------------------------------------------------
const CheckboxBits = struct {
    bits: *u32,
    bits_value: u32,
};
pub fn checkboxBits(label: [:0]const u8, args: CheckboxBits) bool {
    return zguiCheckboxBits(label, args.bits, args.bits_value);
}
extern fn zguiCheckboxBits(label: [*:0]const u8, bits: *u32, bits_value: u32) bool;
//--------------------------------------------------------------------------------------------------
const ProgressBar = struct {
    fraction: f32,
    w: f32 = -1.0,
    h: f32 = 0.0,
    overlay: ?[:0]const u8 = null,
};
pub fn progressBar(args: ProgressBar) void {
    zguiProgressBar(args.fraction, args.w, args.h, if (args.overlay) |o| o else null);
}
extern fn zguiProgressBar(fraction: f32, w: f32, h: f32, overlay: ?[*:0]const u8) void;
//--------------------------------------------------------------------------------------------------
//
// Widgets: Combo Box
//
//--------------------------------------------------------------------------------------------------
const Combo = struct {
    current_item: *i32,
    items_separated_by_zeros: [:0]const u8,
    popup_max_height_in_items: i32 = -1,
};
pub fn combo(label: [:0]const u8, args: Combo) bool {
    return zguiCombo(label, args.current_item, args.items_separated_by_zeros, args.popup_max_height_in_items);
}
extern fn zguiCombo(
    label: [*:0]const u8,
    current_item: *i32,
    items_separated_by_zeros: [*:0]const u8,
    popup_max_height_in_items: i32,
) bool;
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
const BeginCombo = struct {
    preview_value: [*:0]const u8,
    flags: ComboFlags = .{},
};
pub fn beginCombo(label: [:0]const u8, args: BeginCombo) bool {
    return zguiBeginCombo(label, args.preview_value, @bitCast(u32, args.flags));
}
extern fn zguiBeginCombo(label: [*:0]const u8, preview_value: ?[*:0]const u8, flags: u32) bool;
//--------------------------------------------------------------------------------------------------
/// `pub fn endCombo() void`
pub const endCombo = zguiEndCombo;
extern fn zguiEndCombo() void;
//--------------------------------------------------------------------------------------------------
//
// Widgets: Drag Sliders
//
//--------------------------------------------------------------------------------------------------
fn DragFloatGen(comptime T: type) type {
    return struct {
        v: *T,
        v_speed: f32 = 1.0,
        v_min: f32 = 0.0,
        v_max: f32 = 0.0,
        format: [:0]const u8 = "%.3f",
        flags: SliderFlags = .{},
    };
}
//--------------------------------------------------------------------------------------------------
const DragFloat = DragFloatGen(f32);
pub fn dragFloat(label: [:0]const u8, args: DragFloat) bool {
    return zguiDragFloat(label, args.v, args.v_speed, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiDragFloat(
    label: [*:0]const u8,
    v: *f32,
    v_speed: f32,
    v_min: f32,
    v_max: f32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const DragFloat2 = DragFloatGen([2]f32);
pub fn dragFloat2(label: [:0]const u8, args: DragFloat2) bool {
    return zguiDragFloat2(label, args.v, args.v_speed, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiDragFloat2(
    label: [*:0]const u8,
    v: *[2]f32,
    v_speed: f32,
    v_min: f32,
    v_max: f32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const DragFloat3 = DragFloatGen([3]f32);
pub fn dragFloat3(label: [:0]const u8, args: DragFloat3) bool {
    return zguiDragFloat3(label, args.v, args.v_speed, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiDragFloat3(
    label: [*:0]const u8,
    v: *[3]f32,
    v_speed: f32,
    v_min: f32,
    v_max: f32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const DragFloat4 = DragFloatGen([4]f32);
pub fn dragFloat4(label: [:0]const u8, args: DragFloat4) bool {
    return zguiDragFloat4(label, args.v, args.v_speed, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiDragFloat4(
    label: [*:0]const u8,
    v: *[4]f32,
    v_speed: f32,
    v_min: f32,
    v_max: f32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const DragFloatRange2 = struct {
    v_current_min: *f32,
    v_current_max: *f32,
    v_speed: f32 = 1.0,
    v_min: f32 = 0.0,
    v_max: f32 = 0.0,
    format: [:0]const u8 = "%.3f",
    format_max: ?[:0]const u8 = null,
    flags: SliderFlags = .{},
};
pub fn dragFloatRange2(label: [:0]const u8, args: DragFloatRange2) bool {
    return zguiDragFloatRange2(
        label,
        args.v_current_min,
        args.v_current_max,
        args.v_speed,
        args.v_min,
        args.v_max,
        args.format,
        if (args.format_max) |fm| fm else null,
        @bitCast(u32, args.flags),
    );
}
extern fn zguiDragFloatRange2(
    label: [*:0]const u8,
    v_current_min: *f32,
    v_current_max: *f32,
    v_speed: f32,
    v_min: f32,
    v_max: f32,
    format: [*:0]const u8,
    format_max: ?[*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
fn DragIntGen(comptime T: type) type {
    return struct {
        v: *T,
        v_speed: f32 = 1.0,
        v_min: i32 = 0.0,
        v_max: i32 = 0.0,
        format: [:0]const u8 = "%d",
        flags: SliderFlags = .{},
    };
}
//--------------------------------------------------------------------------------------------------
const DragInt = DragIntGen(i32);
pub fn dragInt(label: [:0]const u8, args: DragInt) bool {
    return zguiDragInt(label, args.v, args.v_speed, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiDragInt(
    label: [*:0]const u8,
    v: *i32,
    v_speed: f32,
    v_min: i32,
    v_max: i32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const DragInt2 = DragIntGen([2]i32);
pub fn dragInt2(label: [:0]const u8, args: DragInt2) bool {
    return zguiDragInt2(label, args.v, args.v_speed, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiDragInt2(
    label: [*:0]const u8,
    v: *[2]i32,
    v_speed: f32,
    v_min: i32,
    v_max: i32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const DragInt3 = DragIntGen([3]i32);
pub fn dragInt3(label: [:0]const u8, args: DragInt3) bool {
    return zguiDragInt3(label, args.v, args.v_speed, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiDragInt3(
    label: [*:0]const u8,
    v: *[3]i32,
    v_speed: f32,
    v_min: i32,
    v_max: i32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const DragInt4 = DragIntGen([4]i32);
pub fn dragInt4(label: [:0]const u8, args: DragInt4) bool {
    return zguiDragInt4(label, args.v, args.v_speed, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiDragInt4(
    label: [*:0]const u8,
    v: *[4]i32,
    v_speed: f32,
    v_min: i32,
    v_max: i32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const DragIntRange2 = struct {
    v_current_min: *i32,
    v_current_max: *i32,
    v_speed: f32 = 1.0,
    v_min: i32 = 0.0,
    v_max: i32 = 0.0,
    format: [:0]const u8 = "%d",
    format_max: ?[:0]const u8 = null,
    flags: SliderFlags = .{},
};
pub fn dragIntRange2(label: [:0]const u8, args: DragIntRange2) bool {
    return zguiDragIntRange2(
        label,
        args.v_current_min,
        args.v_current_max,
        args.v_speed,
        args.v_min,
        args.v_max,
        args.format,
        if (args.format_max) |fm| fm else null,
        @bitCast(u32, args.flags),
    );
}
extern fn zguiDragIntRange2(
    label: [*:0]const u8,
    v_current_min: *i32,
    v_current_max: *i32,
    v_speed: f32,
    v_min: i32,
    v_max: i32,
    format: [*:0]const u8,
    format_max: ?[*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
fn DragScalarGen(comptime T: type) type {
    return struct {
        p_data: *T,
        v_speed: f32 = 1.0,
        p_min: ?*const T = null,
        p_max: ?*const T = null,
        format: ?[:0]const u8 = null,
        flags: SliderFlags = .{},
    };
}
pub fn dragScalar(label: [:0]const u8, comptime T: type, args: DragScalarGen(T)) bool {
    return zguiDragScalar(
        label,
        typeToDataTypeEnum(T),
        args.p_data,
        args.v_speed,
        args.p_min,
        args.p_max,
        if (args.format) |fmt| fmt else null,
        @bitCast(u32, args.flags),
    );
}
extern fn zguiDragScalar(
    label: [*:0]const u8,
    data_type: DataType,
    p_data: *anyopaque,
    v_speed: f32,
    p_min: ?*const anyopaque,
    p_max: ?*const anyopaque,
    format: ?[*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
fn DragScalarNGen(comptime T: type) type {
    const ScalarType = @typeInfo(T).Array.child;
    return struct {
        p_data: *T,
        v_speed: f32 = 1.0,
        p_min: ?*const ScalarType = null,
        p_max: ?*const ScalarType = null,
        format: ?[:0]const u8 = null,
        flags: SliderFlags = .{},
    };
}
pub fn dragScalarN(label: [:0]const u8, comptime T: type, args: DragScalarNGen(T)) bool {
    const ScalarType = @typeInfo(T).Array.child;
    const components = @typeInfo(T).Array.len;
    return zguiDragScalarN(
        label,
        typeToDataTypeEnum(ScalarType),
        args.p_data,
        components,
        args.v_speed,
        args.p_min,
        args.p_max,
        if (args.format) |fmt| fmt else null,
        @bitCast(u32, args.flags),
    );
}
extern fn zguiDragScalarN(
    label: [*:0]const u8,
    data_type: DataType,
    p_data: *anyopaque,
    components: i32,
    v_speed: f32,
    p_min: ?*const anyopaque,
    p_max: ?*const anyopaque,
    format: ?[*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
//
// Widgets: Regular Sliders
//
//--------------------------------------------------------------------------------------------------
fn SliderFloatGen(comptime T: type) type {
    return struct {
        v: *T,
        v_min: f32,
        v_max: f32,
        format: [:0]const u8 = "%.3f",
        flags: SliderFlags = .{},
    };
}
//--------------------------------------------------------------------------------------------------
const SliderFloat = SliderFloatGen(f32);
pub fn sliderFloat(label: [:0]const u8, args: SliderFloat) bool {
    return zguiSliderFloat(label, args.v, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiSliderFloat(
    label: [*:0]const u8,
    v: *f32,
    v_min: f32,
    v_max: f32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderFloat2 = SliderFloatGen([2]f32);
pub fn sliderFloat2(label: [:0]const u8, args: SliderFloat2) bool {
    return zguiSliderFloat2(label, args.v, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiSliderFloat2(
    label: [*:0]const u8,
    v: *[2]f32,
    v_min: f32,
    v_max: f32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderFloat3 = SliderFloatGen([3]f32);
pub fn sliderFloat3(label: [:0]const u8, args: SliderFloat3) bool {
    return zguiSliderFloat3(label, args.v, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiSliderFloat3(
    label: [*:0]const u8,
    v: *[3]f32,
    v_min: f32,
    v_max: f32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderFloat4 = SliderFloatGen([4]f32);
pub fn sliderFloat4(label: [:0]const u8, args: SliderFloat4) bool {
    return zguiSliderFloat4(label, args.v, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiSliderFloat4(
    label: [*:0]const u8,
    v: *[4]f32,
    v_min: f32,
    v_max: f32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
fn SliderIntGen(comptime T: type) type {
    return struct {
        v: *T,
        v_min: i32,
        v_max: i32,
        format: [:0]const u8 = "%d",
        flags: SliderFlags = .{},
    };
}
//--------------------------------------------------------------------------------------------------
const SliderInt = SliderIntGen(i32);
pub fn sliderInt(label: [:0]const u8, args: SliderInt) bool {
    return zguiSliderInt(label, args.v, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiSliderInt(
    label: [*:0]const u8,
    v: *i32,
    v_min: i32,
    v_max: i32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderInt2 = SliderIntGen([2]i32);
pub fn sliderInt2(label: [:0]const u8, args: SliderInt2) bool {
    return zguiSliderInt2(label, args.v, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiSliderInt2(
    label: [*:0]const u8,
    v: *[2]i32,
    v_min: i32,
    v_max: i32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderInt3 = SliderIntGen([3]i32);
pub fn sliderInt3(label: [:0]const u8, args: SliderInt3) bool {
    return zguiSliderInt3(label, args.v, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiSliderInt3(
    label: [*:0]const u8,
    v: *[3]i32,
    v_min: i32,
    v_max: i32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderInt4 = SliderIntGen([4]i32);
pub fn sliderInt4(label: [:0]const u8, args: SliderInt4) bool {
    return zguiSliderInt4(label, args.v, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiSliderInt4(
    label: [*:0]const u8,
    v: *[4]i32,
    v_min: i32,
    v_max: i32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
fn SliderScalarGen(comptime T: type) type {
    return struct {
        p_data: *T,
        p_min: *const T,
        p_max: *const T,
        format: ?[:0]const u8 = null,
        flags: SliderFlags = .{},
    };
}
pub fn sliderScalar(label: [:0]const u8, comptime T: type, args: SliderScalarGen(T)) bool {
    return zguiSliderScalar(
        label,
        typeToDataTypeEnum(T),
        args.p_data,
        args.p_min,
        args.p_max,
        if (args.format) |fmt| fmt else null,
        @bitCast(u32, args.flags),
    );
}
extern fn zguiSliderScalar(
    label: [*:0]const u8,
    data_type: DataType,
    p_data: *anyopaque,
    p_min: *const anyopaque,
    p_max: *const anyopaque,
    format: ?[*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
fn SliderScalarNGen(comptime T: type) type {
    const ScalarType = @typeInfo(T).Array.child;
    return struct {
        p_data: *T,
        p_min: *const ScalarType,
        p_max: *const ScalarType,
        format: ?[:0]const u8 = null,
        flags: SliderFlags = .{},
    };
}
pub fn sliderScalarN(label: [:0]const u8, comptime T: type, args: SliderScalarNGen(T)) bool {
    const ScalarType = @typeInfo(T).Array.child;
    const components = @typeInfo(T).Array.len;
    return zguiSliderScalarN(
        label,
        typeToDataTypeEnum(ScalarType),
        args.p_data,
        components,
        args.p_min,
        args.p_max,
        if (args.format) |fmt| fmt else null,
        @bitCast(u32, args.flags),
    );
}
extern fn zguiSliderScalarN(
    label: [*:0]const u8,
    data_type: DataType,
    p_data: *anyopaque,
    components: i32,
    p_min: *const anyopaque,
    p_max: *const anyopaque,
    format: ?[*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const VSliderFloat = struct {
    w: f32,
    h: f32,
    v: *f32,
    v_min: f32,
    v_max: f32,
    format: [:0]const u8 = "%.3f",
    flags: SliderFlags = .{},
};
pub fn vsliderFloat(label: [:0]const u8, args: VSliderFloat) bool {
    return zguiVSliderFloat(label, args.w, args.h, args.v, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiVSliderFloat(
    label: [*:0]const u8,
    w: f32,
    h: f32,
    v: *f32,
    v_min: f32,
    v_max: f32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
const VSliderInt = struct {
    w: f32,
    h: f32,
    v: *i32,
    v_min: i32,
    v_max: i32,
    format: [:0]const u8 = "%d",
    flags: SliderFlags = .{},
};
pub fn vsliderInt(label: [:0]const u8, args: VSliderInt) bool {
    return zguiVSliderInt(label, args.w, args.h, args.v, args.v_min, args.v_max, args.format, @bitCast(u32, args.flags));
}
extern fn zguiVSliderInt(
    label: [*:0]const u8,
    w: f32,
    h: f32,
    v: *i32,
    v_min: i32,
    v_max: i32,
    format: [*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
fn VSliderScalarGen(comptime T: type) type {
    return struct {
        w: f32,
        h: f32,
        p_data: *T,
        p_min: *const T,
        p_max: *const T,
        format: ?[:0]const u8 = null,
        flags: SliderFlags = .{},
    };
}
pub fn vsliderScalar(label: [:0]const u8, comptime T: type, args: VSliderScalarGen(T)) bool {
    return zguiVSliderScalar(
        label,
        args.w,
        args.h,
        typeToDataTypeEnum(T),
        args.p_data,
        args.p_min,
        args.p_max,
        if (args.format) |fmt| fmt else null,
        @bitCast(u32, args.flags),
    );
}
extern fn zguiVSliderScalar(
    label: [*:0]const u8,
    w: f32,
    h: f32,
    data_type: DataType,
    p_data: *anyopaque,
    p_min: *const anyopaque,
    p_max: *const anyopaque,
    format: ?[*:0]const u8,
    flags: u32,
) bool;
//--------------------------------------------------------------------------------------------------
/// `pub fn newFrame() void`
pub const newFrame = zguiNewFrame;
extern fn zguiNewFrame() void;
//--------------------------------------------------------------------------------------------------
/// `pub fn render() void`
pub const render = zguiRender;
extern fn zguiRender() void;
//--------------------------------------------------------------------------------------------------
/// `pub fn getDrawData() DrawData`
pub const getDrawData = zguiGetDrawData;
extern fn zguiGetDrawData() DrawData;
//--------------------------------------------------------------------------------------------------
/// `pub fn showDemoWindow(p_open: ?*bool) void`
pub const showDemoWindow = zguiShowDemoWindow;
extern fn zguiShowDemoWindow(p_open: ?*bool) void;
//--------------------------------------------------------------------------------------------------
pub fn treeNode(label: [:0]const u8) bool {
    return zguiTreeNode(label);
}
extern fn zguiTreeNode(label: [*:0]const u8) bool;
//--------------------------------------------------------------------------------------------------
/// `pub fn treePop() void`
pub const treePop = zguiTreePop;
extern fn zguiTreePop() void;
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
fn typeToDataTypeEnum(comptime T: type) DataType {
    return switch (T) {
        i8 => .I8,
        u8 => .U8,
        i16 => .I16,
        u16 => .U16,
        i32 => .I32,
        u32 => .U32,
        i64 => .I64,
        u64 => .U64,
        f32 => .F32,
        f64 => .F64,
        else => @compileError("Only fundamental scalar types allowed"),
    };
}
//--------------------------------------------------------------------------------------------------
