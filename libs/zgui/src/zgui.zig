//--------------------------------------------------------------------------------------------------
// Zig bindings for 'dear imgui' library. Easy to use, hand-crafted API with default arguments,
// named parameters and Zig style text formatting.
//--------------------------------------------------------------------------------------------------
const std = @import("std");
const assert = std.debug.assert;
//--------------------------------------------------------------------------------------------------
pub const f32_min: f32 = 1.17549435082228750796873653722225e-38;
pub const f32_max: f32 = 3.40282346638528859811704183484517e+38;
//--------------------------------------------------------------------------------------------------
pub fn init() void {
    if (zguiGetCurrentContext() == null) {
        _ = zguiCreateContext(null);
        temp_buffer.resize(3 * 1024 + 1) catch unreachable;
    }
}
pub fn deinit() void {
    if (zguiGetCurrentContext() != null) {
        temp_buffer.deinit();
        zguiDestroyContext(null);
    }
}
extern fn zguiCreateContext(shared_font_atlas: ?*const anyopaque) Context;
extern fn zguiDestroyContext(ctx: ?Context) void;
extern fn zguiGetCurrentContext() ?Context;
//--------------------------------------------------------------------------------------------------
pub const io = struct {
    pub fn addFontFromFile(filename: [:0]const u8, size_pixels: f32) Font {
        return zguiIoAddFontFromFile(filename, size_pixels);
    }
    extern fn zguiIoAddFontFromFile(filename: [*:0]const u8, size_pixels: f32) Font;

    /// `pub fn getFont(index: u32) Font`
    pub const getFont = zguiIoGetFont;
    extern fn zguiIoGetFont(index: u32) Font;

    /// `pub fn setDefaultFont(font: Font) void`
    pub const setDefaultFont = zguiIoSetDefaultFont;
    extern fn zguiIoSetDefaultFont(font: Font) void;

    /// `pub fn zguiIoGetWantCaptureMouse() bool`
    pub const getWantCaptureMouse = zguiIoGetWantCaptureMouse;
    extern fn zguiIoGetWantCaptureMouse() bool;

    /// `pub fn zguiIoGetWantCaptureKeyboard() bool`
    pub const getWantCaptureKeyboard = zguiIoGetWantCaptureKeyboard;
    extern fn zguiIoGetWantCaptureKeyboard() bool;

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
pub const Font = *opaque {};
pub const Ident = u32;
pub const TextureIdent = *anyopaque;
pub const Wchar = u16;
pub const Key = i32;
//--------------------------------------------------------------------------------------------------
pub const WindowFlags = packed struct(u32) {
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
    pub const no_inputs = WindowFlags{
        .no_mouse_inputs = true,
        .no_nav_inputs = true,
        .no_nav_focus = true,
    };
};
//--------------------------------------------------------------------------------------------------
pub const SliderFlags = packed struct(u32) {
    _reserved0: bool = false,
    _reserved1: bool = false,
    _reserved2: bool = false,
    _reserved3: bool = false,
    always_clamp: bool = false,
    logarithmic: bool = false,
    no_round_to_format: bool = false,
    no_input: bool = false,
    _padding: u24 = 0,
};
//--------------------------------------------------------------------------------------------------
pub const ButtonFlags = packed struct(u32) {
    mouse_button_left: bool = false,
    mouse_button_right: bool = false,
    mouse_button_middle: bool = false,
    _padding: u29 = 0,
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
//
// Main
//
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
//
// Demo, Debug, Information
//
//--------------------------------------------------------------------------------------------------
/// `pub fn showDemoWindow(popen: ?*bool) void`
pub const showDemoWindow = zguiShowDemoWindow;
extern fn zguiShowDemoWindow(popen: ?*bool) void;
//--------------------------------------------------------------------------------------------------
//
// Windows
//
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
const SetNextWindowCollapsed = struct {
    collapsed: bool,
    cond: Condition = .none,
};
pub fn setNextWindowCollapsed(args: SetNextWindowCollapsed) void {
    zguiSetNextWindowCollapsed(args.collapsed, args.cond);
}
extern fn zguiSetNextWindowCollapsed(collapsed: bool, cond: Condition) void;
//--------------------------------------------------------------------------------------------------
/// `pub fn setNextWindowFocus() void`
pub const setNextWindowFocus = zguiSetNextWindowFocus;
extern fn zguiSetNextWindowFocus() void;
//--------------------------------------------------------------------------------------------------
const SetNextWindowBgAlpha = struct {
    alpha: f32,
};
pub fn setNextWindowBgAlpha(args: SetNextWindowBgAlpha) void {
    zguiSetNextWindowBgAlpha(args.alpha);
}
extern fn zguiSetNextWindowBgAlpha(alpha: f32) void;
//--------------------------------------------------------------------------------------------------
const Begin = struct {
    popen: ?*bool = null,
    flags: WindowFlags = .{},
};
pub fn begin(name: [:0]const u8, args: Begin) bool {
    return zguiBegin(name, args.popen, args.flags);
}
/// `pub fn end() void`
pub const end = zguiEnd;
extern fn zguiBegin(name: [*:0]const u8, popen: ?*bool, flags: WindowFlags) bool;
extern fn zguiEnd() void;
//--------------------------------------------------------------------------------------------------
const BeginChild = struct {
    w: f32 = 0.0,
    h: f32 = 0.0,
    border: bool = false,
    flags: WindowFlags = .{},
};
pub fn beginChild(str_id: [:0]const u8, args: BeginChild) bool {
    return zguiBeginChild(str_id, args.w, args.h, args.border, args.flags);
}
pub fn beginChildId(id: Ident, args: BeginChild) bool {
    return zguiBeginChildId(id, args.w, args.h, args.border, args.flags);
}
/// `pub fn endChild() void`
pub const endChild = zguiEndChild;
extern fn zguiBeginChild(str_id: [*:0]const u8, w: f32, h: f32, border: bool, flags: WindowFlags) bool;
extern fn zguiBeginChildId(id: Ident, w: f32, h: f32, border: bool, flags: WindowFlags) bool;
extern fn zguiEndChild() void;
//--------------------------------------------------------------------------------------------------
/// `pub fn zguiGetScrollX() f32`
pub const getScrollX = zguiGetScrollX;
/// `pub fn zguiGetScrollY() f32`
pub const getScrollY = zguiGetScrollY;
/// `pub fn zguiSetScrollX(scroll_x: f32) void`
pub const setScrollX = zguiSetScrollX;
/// `pub fn zguiSetScrollY(scroll_y: f32) void`
pub const setScrollY = zguiSetScrollY;
/// `pub fn zguiGetScrollMaxX() f32`
pub const getScrollMaxX = zguiGetScrollMaxX;
/// `pub fn zguiGetScrollMaxY() f32`
pub const getScrollMaxY = zguiGetScrollMaxY;
extern fn zguiGetScrollX() f32;
extern fn zguiGetScrollY() f32;
extern fn zguiSetScrollX(scroll_x: f32) void;
extern fn zguiSetScrollY(scroll_y: f32) void;
extern fn zguiGetScrollMaxX() f32;
extern fn zguiGetScrollMaxY() f32;
const SetScrollHereX = struct {
    center_x_ratio: f32 = 0.5,
};
const SetScrollHereY = struct {
    center_y_ratio: f32 = 0.5,
};
pub fn setScrollHereX(args: SetScrollHereX) void {
    zguiSetScrollHereX(args.center_x_ratio);
}
pub fn setScrollHereY(args: SetScrollHereY) void {
    zguiSetScrollHereY(args.center_y_ratio);
}
const SetScrollFromPosX = struct {
    local_x: f32,
    center_x_ratio: f32 = 0.5,
};
const SetScrollFromPosY = struct {
    local_y: f32,
    center_y_ratio: f32 = 0.5,
};
pub fn setScrollFromPosX(args: SetScrollFromPosX) void {
    zguiSetScrollFromPosX(args.local_x, args.center_x_ratio);
}
pub fn setScrollFromPosY(args: SetScrollFromPosY) void {
    zguiSetScrollFromPosY(args.local_y, args.center_y_ratio);
}
extern fn zguiSetScrollHereX(center_x_ratio: f32) void;
extern fn zguiSetScrollHereY(center_y_ratio: f32) void;
extern fn zguiSetScrollFromPosX(local_x: f32, center_x_ratio: f32) void;
extern fn zguiSetScrollFromPosY(local_y: f32, center_y_ratio: f32) void;
//--------------------------------------------------------------------------------------------------
pub const FocusedFlags = packed struct(u32) {
    child_windows: bool = false,
    root_window: bool = false,
    any_window: bool = false,
    no_popup_hierarchy: bool = false,
    _padding: u28 = 0,

    pub const root_and_child_windows = FocusedFlags{ .root_window = true, .child_windows = true };
};
//--------------------------------------------------------------------------------------------------
pub const HoveredFlags = packed struct(u32) {
    child_windows: bool = false,
    root_window: bool = false,
    any_window: bool = false,
    no_popup_hierarchy: bool = false,
    _reserved0: bool = false,
    allow_when_blocked_by_popup: bool = false,
    _reserved1: bool = false,
    allow_when_blocked_by_active_item: bool = false,
    allow_when_overlapped: bool = false,
    allow_when_disabled: bool = false,
    no_nav_override: bool = false,
    _padding: u21 = 0,

    pub const rect_only = HoveredFlags{
        .allow_when_blocked_by_popup = true,
        .allow_when_blocked_by_active_item = true,
        .allow_when_overlapped = true,
    };
    pub const root_and_child_windows = HoveredFlags{ .root_window = true, .child_windows = true };
};
//--------------------------------------------------------------------------------------------------
/// `pub fn isWindowAppearing() bool`
pub const isWindowAppearing = zguiIsWindowAppearing;
/// `pub fn isWindowCollapsed() bool`
pub const isWindowCollapsed = zguiIsWindowCollapsed;
pub fn isWindowFocused(flags: FocusedFlags) bool {
    return zguiIsWindowFocused(flags);
}
pub fn isWindowHovered(flags: HoveredFlags) bool {
    return zguiIsWindowHovered(flags);
}
extern fn zguiIsWindowAppearing() bool;
extern fn zguiIsWindowCollapsed() bool;
extern fn zguiIsWindowFocused(flags: WindowFlags) bool;
extern fn zguiIsWindowHovered(flags: WindowFlags) bool;
//--------------------------------------------------------------------------------------------------
pub fn getWindowPos() [2]f32 {
    var pos: [2]f32 = undefined;
    zguiGetWindowPos(&pos);
    return pos;
}
pub fn getWindowSize() [2]f32 {
    var size: [2]f32 = undefined;
    zguiGetWindowSize(&size);
    return size;
}

pub fn getContentRegionAvail() [2]f32 {
    var size: [2]f32 = undefined;
    zguiGetContentRegionAvail(&size);
    return size;
}

/// `pub fn getWindowWidth() f32`
pub const getWindowWidth = zguiGetWindowWidth;
/// `pub fn getWindowHeight() f32`
pub const getWindowHeight = zguiGetWindowHeight;
extern fn zguiGetWindowPos(pos: *[2]f32) void;
extern fn zguiGetWindowSize(size: *[2]f32) void;
extern fn zguiGetWindowWidth() f32;
extern fn zguiGetWindowHeight() f32;
extern fn zguiGetContentRegionAvail(size: *[2]f32) void;
//--------------------------------------------------------------------------------------------------
//
// Style
//
//--------------------------------------------------------------------------------------------------
pub const Style = extern struct {
    alpha: f32,
    disabled_alpha: f32,
    window_padding: [2]f32,
    window_rounding: f32,
    window_border_size: f32,
    window_min_size: [2]f32,
    window_title_align: [2]f32,
    window_menu_button_position: Direction,
    child_rounding: f32,
    child_border_size: f32,
    popup_rounding: f32,
    popup_border_size: f32,
    frame_padding: [2]f32,
    frame_rounding: f32,
    frame_border_size: f32,
    item_spacing: [2]f32,
    item_inner_spacing: [2]f32,
    cell_padding: [2]f32,
    touch_extra_padding: [2]f32,
    indent_spacing: f32,
    columns_min_spacing: f32,
    scrollbar_size: f32,
    scrollbar_rounding: f32,
    grab_min_size: f32,
    grab_rounding: f32,
    log_slider_deadzone: f32,
    tab_rounding: f32,
    tab_border_size: f32,
    tab_min_width_for_close_button: f32,
    color_button_position: Direction,
    button_text_align: [2]f32,
    selectable_text_align: [2]f32,
    display_window_padding: [2]f32,
    display_safe_area_padding: [2]f32,
    mouse_cursor_scale: f32,
    anti_aliased_lines: bool,
    anti_aliased_lines_use_tex: bool,
    anti_aliased_fill: bool,
    curve_tessellation_tol: f32,
    circle_tessellation_max_error: f32,
    colors: [@typeInfo(StyleCol).Enum.fields.len][4]f32,

    /// `pub fn init() Style`
    pub const init = zguiStyle_init;
    extern fn zguiStyle_init() Style;

    /// `pub fn scaleAllSizes(style: *Style, scale_factor: f32) void`
    pub const scaleAllSizes = zguiStyle_scaleAllSizes;
    extern fn zguiStyle_scaleAllSizes(style: *Style, scale_factor: f32) void;

    pub fn getColor(style: Style, idx: StyleCol) [4]f32 {
        return style.colors[@enumToInt(idx)];
    }
    pub fn setColor(style: *Style, idx: StyleCol, color: [4]f32) void {
        style.colors[@enumToInt(idx)] = color;
    }
};
/// `pub fn getStyle() *Style`
pub const getStyle = zguiGetStyle;
extern fn zguiGetStyle() *Style;
//--------------------------------------------------------------------------------------------------
pub const StyleCol = enum(u32) {
    text,
    text_disabled,
    window_bg,
    child_bg,
    popup_bg,
    border,
    border_shadow,
    frame_bg,
    frame_bg_hovered,
    frame_bg_active,
    title_bg,
    title_bg_active,
    title_bg_collapsed,
    menu_bar_bg,
    scrollbar_bg,
    scrollbar_grab,
    scrollbar_grab_hovered,
    scrollbar_grab_active,
    check_mark,
    slider_grab,
    slider_grab_active,
    button,
    button_hovered,
    button_active,
    header,
    header_hovered,
    header_active,
    separator,
    separator_hovered,
    separator_active,
    resize_grip,
    resize_grip_hovered,
    resize_grip_active,
    tab,
    tab_hovered,
    tab_active,
    tab_unfocused,
    tab_unfocused_active,
    plot_lines,
    plot_lines_hovered,
    plot_histogram,
    plot_histogram_hovered,
    table_header_bg,
    table_border_strong,
    table_border_light,
    table_row_bg,
    table_row_bg_alt,
    text_selected_bg,
    drag_drop_target,
    nav_highlight,
    nav_windowing_highlight,
    nav_windowing_dim_bg,
    modal_window_dim_bg,
};
const PushStyleColor4f = struct {
    idx: StyleCol,
    c: [4]f32,
};
pub fn pushStyleColor4f(args: PushStyleColor4f) void {
    zguiPushStyleColor4f(args.idx, &args.c);
}
const PushStyleColor1u = struct {
    idx: StyleCol,
    c: u32,
};
pub fn pushStyleColor1u(args: PushStyleColor1u) void {
    zguiPushStyleColor1u(args.idx, args.c);
}
const PopStyleColor = struct {
    count: i32 = 1,
};
pub fn popStyleColor(args: PopStyleColor) void {
    zguiPopStyleColor(args.count);
}
extern fn zguiPushStyleColor4f(idx: StyleCol, col: *const [4]f32) void;
extern fn zguiPushStyleColor1u(idx: StyleCol, col: u32) void;
extern fn zguiPopStyleColor(count: i32) void;
//--------------------------------------------------------------------------------------------------
pub const StyleVar = enum(u32) {
    alpha, // 1f
    disabled_alpha, // 1f
    window_padding, // 2f
    window_rounding, // 1f
    window_border_size, // 1f
    window_min_size, // 2f
    window_title_align, // 2f
    child_rounding, // 1f
    child_border_size, // 1f
    popup_rounding, // 1f
    popup_border_size, // 1f
    frame_padding, // 2f
    frame_rounding, // 1f
    frame_border_size, // 1f
    item_spacing, // 2f
    item_inner_spacing, // 2f
    indent_spacing, // 1f
    cell_padding, // 2f
    scrollbar_size, // 1f
    scrollbar_rounding, // 1f
    grab_min_size, // 1f
    grab_rounding, // 1f
    tab_rounding, // 1f
    button_text_align, // 2f
    selectable_text_align, // 2f
};
const PushStyleVar1f = struct {
    idx: StyleVar,
    v: f32,
};
pub fn pushStyleVar1f(args: PushStyleVar1f) void {
    zguiPushStyleVar1f(args.idx, args.v);
}
const PushStyleVar2f = struct {
    idx: StyleVar,
    v: [2]f32,
};
pub fn pushStyleVar2f(args: PushStyleVar2f) void {
    zguiPushStyleVar2f(args.idx, &args.v);
}
const PopStyleVar = struct {
    count: i32 = 1,
};
pub fn popStyleVar(args: PopStyleVar) void {
    zguiPopStyleVar(args.count);
}
extern fn zguiPushStyleVar1f(idx: StyleVar, v: f32) void;
extern fn zguiPushStyleVar2f(idx: StyleVar, v: *const [2]f32) void;
extern fn zguiPopStyleVar(count: i32) void;
//--------------------------------------------------------------------------------------------------
/// `void pushItemWidth(item_width: f32) void`
pub const pushItemWidth = zguiPushItemWidth;
/// `void popItemWidth() void`
pub const popItemWidth = zguiPopItemWidth;
/// `void setNextItemWidth(item_width: f32) void`
pub const setNextItemWidth = zguiSetNextItemWidth;
extern fn zguiPushItemWidth(item_width: f32) void;
extern fn zguiPopItemWidth() void;
extern fn zguiSetNextItemWidth(item_width: f32) void;
//--------------------------------------------------------------------------------------------------
/// `pub fn getFont() Font'
pub const getFont = zguiGetFont;
extern fn zguiGetFont() Font;
/// `pub fn getFontSize() f32'
pub const getFontSize = zguiGetFontSize;
extern fn zguiGetFontSize() f32;
/// `void pushFont(font: Font) void`
pub const pushFont = zguiPushFont;
extern fn zguiPushFont(font: Font) void;
/// `void popFont() void`
pub const popFont = zguiPopFont;
extern fn zguiPopFont() void;
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
const BeginDisabled = struct {
    disabled: bool = true,
};
pub fn beginDisabled(args: BeginDisabled) void {
    zguiBeginDisabled(args.disabled);
}
/// `pub fn endDisabled() void`
pub const endDisabled = zguiEndDisabled;
extern fn zguiBeginDisabled(disabled: bool) void;
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
const Indent = struct {
    indent_w: f32 = 0.0,
};
pub fn indent(args: Indent) void {
    zguiIndent(args.indent_w);
}
const Unindent = struct {
    indent_w: f32 = 0.0,
};
pub fn unindent(args: Unindent) void {
    zguiUnindent(args.indent_w);
}
extern fn zguiIndent(indent_w: f32) void;
extern fn zguiUnindent(indent_w: f32) void;
//--------------------------------------------------------------------------------------------------
/// `pub fn beginGroup() void`
const beginGroup = zguiBeginGroup;
extern fn zguiBeginGroup() void;
/// `pub fn endGroup() void`
const endGroup = zguiEndGroup;
extern fn zguiEndGroup() void;
//--------------------------------------------------------------------------------------------------
pub fn getCursorPos() [2]f32 {
    var pos: [2]f32 = undefined;
    zguiGetCursorPos(&pos);
    return pos;
}
/// `pub fn getCursorPosX() f32`
pub const getCursorPosX = zguiGetCursorPosX;
/// `pub fn getCursorPosY() f32`
pub const getCursorPosY = zguiGetCursorPosY;
extern fn zguiGetCursorPos(pos: *[2]f32) void;
extern fn zguiGetCursorPosX() f32;
extern fn zguiGetCursorPosY() f32;
//--------------------------------------------------------------------------------------------------
pub fn setCursorPos(local_pos: [2]f32) void {
    zguiSetCursorPos(local_pos[0], local_pos[1]);
}
/// `pub fn setCursorPosX(local_x: f32) void`
pub const setCursorPosX = zguiSetCursorPosX;
/// `pub fn setCursorPosY(local_y: f32) void`
pub const setCursorPosY = zguiSetCursorPosY;
extern fn zguiSetCursorPos(local_x: f32, local_y: f32) void;
extern fn zguiSetCursorPosX(local_x: f32) void;
extern fn zguiSetCursorPosY(local_y: f32) void;
//--------------------------------------------------------------------------------------------------
pub fn getCursorStartPos() [2]f32 {
    var pos: [2]f32 = undefined;
    zguiGetCursorStartPos(&pos);
    return pos;
}
pub fn getCursorScreenPos() [2]f32 {
    var pos: [2]f32 = undefined;
    zguiGetCursorScreenPos(&pos);
    return pos;
}
pub fn setCursorScreenPos(screen_pos: [2]f32) void {
    zguiSetCursorPos(screen_pos[0], screen_pos[1]);
}
extern fn zguiGetCursorStartPos(pos: *[2]f32) void;
extern fn zguiGetCursorScreenPos(pos: *[2]f32) void;
extern fn zguiSetCursorScreenPos(screen_x: f32, screen_y: f32) void;
//--------------------------------------------------------------------------------------------------
/// `pub fn alignTextToFramePadding() void`
pub const alignTextToFramePadding = zguiAlignTextToFramePadding;
/// `pub fn getTextLineHeight() f32`
pub const getTextLineHeight = zguiGetTextLineHeight;
/// `pub fn getTextLineHeightWithSpacing() f32`
pub const getTextLineHeightWithSpacing = zguiGetTextLineHeightWithSpacing;
/// `pub fn getFrameHeight() f32`
pub const getFrameHeight = zguiGetFrameHeight;
/// `pub fn getFrameHeightWithSpacing() f32`
pub const getFrameHeightWithSpacing = zguiGetFrameHeightWithSpacing;
extern fn zguiAlignTextToFramePadding() void;
extern fn zguiGetTextLineHeight() f32;
extern fn zguiGetTextLineHeightWithSpacing() f32;
extern fn zguiGetFrameHeight() f32;
extern fn zguiGetFrameHeightWithSpacing() f32;
//--------------------------------------------------------------------------------------------------
//
// ID stack/scopes
//
//--------------------------------------------------------------------------------------------------
pub fn pushStrId(str_id: []const u8) void {
    zguiPushStrId(str_id.ptr, str_id.ptr + str_id.len);
}
pub fn pushStrIdZ(str_id: [:0]const u8) void {
    zguiPushStrIdZ(str_id);
}
pub fn pushPtrId(ptr_id: *const anyopaque) void {
    zguiPushPtrId(ptr_id);
}
pub fn pushIntId(int_id: i32) void {
    zguiPushIntId(int_id);
}
extern fn zguiPushStrId(str_id_begin: [*]const u8, str_id_end: [*]const u8) void;
extern fn zguiPushStrIdZ(str_id: [*:0]const u8) void;
extern fn zguiPushPtrId(ptr_id: *const anyopaque) void;
extern fn zguiPushIntId(int_id: i32) void;
//--------------------------------------------------------------------------------------------------
/// `pub fn popId() void`
pub const popId = zguiPopId;
extern fn zguiPopId() void;
//--------------------------------------------------------------------------------------------------
pub fn getStrId(str_id: []const u8) Ident {
    return zguiGetStrId(str_id.ptr, str_id.ptr + str_id.len);
}
pub fn getStrIdZ(str_id: [:0]const u8) Ident {
    return zguiGetStrIdZ(str_id);
}
pub fn getPtrId(ptr_id: *const anyopaque) Ident {
    return zguiGetPtrId(ptr_id);
}
extern fn zguiGetStrId(str_id_begin: [*]const u8, str_id_end: [*]const u8) Ident;
extern fn zguiGetStrIdZ(str_id: [*:0]const u8) Ident;
extern fn zguiGetPtrId(ptr_id: *const anyopaque) Ident;
//--------------------------------------------------------------------------------------------------
//
// Widgets: Text
//
//--------------------------------------------------------------------------------------------------
pub fn textUnformatted(txt: []const u8) void {
    zguiTextUnformatted(txt.ptr, txt.ptr + txt.len);
}
pub fn textUnformattedColored(color: [4]f32, txt: []const u8) void {
    pushStyleColor4f(.{ .idx = .text, .c = color });
    textUnformatted(txt);
    popStyleColor(.{});
}
//--------------------------------------------------------------------------------------------------
pub fn text(comptime fmt: []const u8, args: anytype) void {
    const result = format(fmt, args);
    zguiTextUnformatted(result.ptr, result.ptr + result.len);
}
pub fn textColored(color: [4]f32, comptime fmt: []const u8, args: anytype) void {
    pushStyleColor4f(.{ .idx = .text, .c = color });
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
const CalcTextSize = struct {
    hide_text_after_double_hash: bool = false,
    wrap_width: f32 = -1.0,
};
pub fn calcTextSize(txt: []const u8, args: CalcTextSize) [2]f32 {
    var w: f32 = undefined;
    var h: f32 = undefined;
    zguiCalcTextSize(
        txt.ptr,
        txt.ptr + txt.len,
        args.hide_text_after_double_hash,
        args.wrap_width,
        &w,
        &h,
    );
    return .{ w, h };
}
extern fn zguiCalcTextSize(
    txt: [*]const u8,
    txt_end: [*]const u8,
    hide_text_after_double_hash: bool,
    wrap_width: f32,
    out_w: *f32,
    out_h: *f32,
) void;
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
    return zguiInvisibleButton(str_id, args.w, args.h, args.flags);
}
extern fn zguiInvisibleButton(str_id: [*:0]const u8, w: f32, h: f32, flags: ButtonFlags) bool;
//--------------------------------------------------------------------------------------------------
const ArrowButton = struct {
    dir: Direction,
};
pub fn arrowButton(label: [:0]const u8, args: ArrowButton) bool {
    return zguiArrowButton(label, args.dir);
}
extern fn zguiArrowButton(label: [*:0]const u8, dir: Direction) bool;
//--------------------------------------------------------------------------------------------------
const Image = struct {
    w: f32,
    h: f32,
    uv0: [2]f32 = .{ 0.0, 0.0 },
    uv1: [2]f32 = .{ 1.0, 1.0 },
    tint_col: [4]f32 = .{ 1.0, 1.0, 1.0, 1.0 },
    border_col: [4]f32 = .{ 0.0, 0.0, 0.0, 0.0 },
};
pub fn image(user_texture_id: TextureIdent, args: Image) void {
    zguiImage(user_texture_id, args.w, args.h, &args.uv0, &args.uv1, &args.tint_col, &args.border_col);
}
extern fn zguiImage(
    user_texture_id: TextureIdent,
    w: f32,
    h: f32,
    uv0: *const [2]f32,
    uv1: *const [2]f32,
    tint_col: *const [4]f32,
    border_col: *const [4]f32,
) void;
//--------------------------------------------------------------------------------------------------
const ImageButton = struct {
    w: f32,
    h: f32,
    uv0: [2]f32 = .{ 0.0, 0.0 },
    uv1: [2]f32 = .{ 1.0, 1.0 },
    frame_padding: i32 = -1,
    bg_col: [4]f32 = .{ 0.0, 0.0, 0.0, 0.0 },
    tint_col: [4]f32 = .{ 1.0, 1.0, 1.0, 1.0 },
};
pub fn imageButton(user_texture_id: TextureIdent, args: ImageButton) bool {
    return zguiImageButton(
        user_texture_id,
        args.w,
        args.h,
        &args.uv0,
        &args.uv1,
        args.frame_padding,
        &args.bg_col,
        &args.tint_col,
    );
}
extern fn zguiImageButton(
    user_texture_id: TextureIdent,
    w: f32,
    h: f32,
    uv0: *const [2]f32,
    uv1: *const [2]f32,
    frame_padding: i32,
    bg_col: *const [4]f32,
    tint_col: *const [4]f32,
) bool;
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
    w: f32 = -f32_min,
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
    return zguiCombo(
        label,
        args.current_item,
        args.items_separated_by_zeros,
        args.popup_max_height_in_items,
    );
}
extern fn zguiCombo(
    label: [*:0]const u8,
    current_item: *i32,
    items_separated_by_zeros: [*:0]const u8,
    popup_max_height_in_items: i32,
) bool;
//--------------------------------------------------------------------------------------------------
pub const ComboFlags = packed struct(u32) {
    popup_align_left: bool = false,
    height_small: bool = false,
    height_regular: bool = false,
    height_large: bool = false,
    height_largest: bool = false,
    no_arrow_button: bool = false,
    no_preview: bool = false,
    _padding: u25 = 0,
};
//--------------------------------------------------------------------------------------------------
const BeginCombo = struct {
    preview_value: [*:0]const u8,
    flags: ComboFlags = .{},
};
pub fn beginCombo(label: [:0]const u8, args: BeginCombo) bool {
    return zguiBeginCombo(label, args.preview_value, args.flags);
}
extern fn zguiBeginCombo(label: [*:0]const u8, preview_value: ?[*:0]const u8, flags: ComboFlags) bool;
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
        speed: f32 = 1.0,
        min: f32 = 0.0,
        max: f32 = 0.0,
        cfmt: [:0]const u8 = "%.3f",
        flags: SliderFlags = .{},
    };
}
//--------------------------------------------------------------------------------------------------
const DragFloat = DragFloatGen(f32);
pub fn dragFloat(label: [:0]const u8, args: DragFloat) bool {
    return zguiDragFloat(
        label,
        args.v,
        args.speed,
        args.min,
        args.max,
        args.cfmt,
        args.flags,
    );
}
extern fn zguiDragFloat(
    label: [*:0]const u8,
    v: *f32,
    speed: f32,
    min: f32,
    max: f32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const DragFloat2 = DragFloatGen([2]f32);
pub fn dragFloat2(label: [:0]const u8, args: DragFloat2) bool {
    return zguiDragFloat2(label, args.v, args.speed, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiDragFloat2(
    label: [*:0]const u8,
    v: *[2]f32,
    speed: f32,
    min: f32,
    max: f32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const DragFloat3 = DragFloatGen([3]f32);
pub fn dragFloat3(label: [:0]const u8, args: DragFloat3) bool {
    return zguiDragFloat3(label, args.v, args.speed, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiDragFloat3(
    label: [*:0]const u8,
    v: *[3]f32,
    speed: f32,
    min: f32,
    max: f32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const DragFloat4 = DragFloatGen([4]f32);
pub fn dragFloat4(label: [:0]const u8, args: DragFloat4) bool {
    return zguiDragFloat4(label, args.v, args.speed, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiDragFloat4(
    label: [*:0]const u8,
    v: *[4]f32,
    speed: f32,
    min: f32,
    max: f32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const DragFloatRange2 = struct {
    current_min: *f32,
    current_max: *f32,
    speed: f32 = 1.0,
    min: f32 = 0.0,
    max: f32 = 0.0,
    cfmt: [:0]const u8 = "%.3f",
    cfmt_max: ?[:0]const u8 = null,
    flags: SliderFlags = .{},
};
pub fn dragFloatRange2(label: [:0]const u8, args: DragFloatRange2) bool {
    return zguiDragFloatRange2(
        label,
        args.current_min,
        args.current_max,
        args.speed,
        args.min,
        args.max,
        args.cfmt,
        if (args.cfmt_max) |fm| fm else null,
        args.flags,
    );
}
extern fn zguiDragFloatRange2(
    label: [*:0]const u8,
    current_min: *f32,
    current_max: *f32,
    speed: f32,
    min: f32,
    max: f32,
    cfmt: [*:0]const u8,
    cfmt_max: ?[*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
fn DragIntGen(comptime T: type) type {
    return struct {
        v: *T,
        speed: f32 = 1.0,
        min: i32 = 0.0,
        max: i32 = 0.0,
        cfmt: [:0]const u8 = "%d",
        flags: SliderFlags = .{},
    };
}
//--------------------------------------------------------------------------------------------------
const DragInt = DragIntGen(i32);
pub fn dragInt(label: [:0]const u8, args: DragInt) bool {
    return zguiDragInt(label, args.v, args.speed, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiDragInt(
    label: [*:0]const u8,
    v: *i32,
    speed: f32,
    min: i32,
    max: i32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const DragInt2 = DragIntGen([2]i32);
pub fn dragInt2(label: [:0]const u8, args: DragInt2) bool {
    return zguiDragInt2(label, args.v, args.speed, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiDragInt2(
    label: [*:0]const u8,
    v: *[2]i32,
    speed: f32,
    min: i32,
    max: i32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const DragInt3 = DragIntGen([3]i32);
pub fn dragInt3(label: [:0]const u8, args: DragInt3) bool {
    return zguiDragInt3(label, args.v, args.speed, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiDragInt3(
    label: [*:0]const u8,
    v: *[3]i32,
    speed: f32,
    min: i32,
    max: i32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const DragInt4 = DragIntGen([4]i32);
pub fn dragInt4(label: [:0]const u8, args: DragInt4) bool {
    return zguiDragInt4(label, args.v, args.speed, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiDragInt4(
    label: [*:0]const u8,
    v: *[4]i32,
    speed: f32,
    min: i32,
    max: i32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const DragIntRange2 = struct {
    current_min: *i32,
    current_max: *i32,
    speed: f32 = 1.0,
    min: i32 = 0.0,
    max: i32 = 0.0,
    cfmt: [:0]const u8 = "%d",
    cfmt_max: ?[:0]const u8 = null,
    flags: SliderFlags = .{},
};
pub fn dragIntRange2(label: [:0]const u8, args: DragIntRange2) bool {
    return zguiDragIntRange2(
        label,
        args.current_min,
        args.current_max,
        args.speed,
        args.min,
        args.max,
        args.cfmt,
        if (args.cfmt_max) |fm| fm else null,
        args.flags,
    );
}
extern fn zguiDragIntRange2(
    label: [*:0]const u8,
    current_min: *i32,
    current_max: *i32,
    speed: f32,
    min: i32,
    max: i32,
    cfmt: [*:0]const u8,
    cfmt_max: ?[*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
fn DragScalarGen(comptime T: type) type {
    return struct {
        v: *T,
        speed: f32 = 1.0,
        min: ?T = null,
        max: ?T = null,
        cfmt: ?[:0]const u8 = null,
        flags: SliderFlags = .{},
    };
}
pub fn dragScalar(label: [:0]const u8, comptime T: type, args: DragScalarGen(T)) bool {
    return zguiDragScalar(
        label,
        typeToDataTypeEnum(T),
        args.v,
        args.speed,
        if (args.min) |vm| &vm else null,
        if (args.max) |vm| &vm else null,
        if (args.cfmt) |fmt| fmt else null,
        args.flags,
    );
}
extern fn zguiDragScalar(
    label: [*:0]const u8,
    data_type: DataType,
    pdata: *anyopaque,
    speed: f32,
    pmin: ?*const anyopaque,
    pmax: ?*const anyopaque,
    cfmt: ?[*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
fn DragScalarNGen(comptime T: type) type {
    const ScalarType = @typeInfo(T).Array.child;
    return struct {
        v: *T,
        speed: f32 = 1.0,
        min: ?ScalarType = null,
        max: ?ScalarType = null,
        cfmt: ?[:0]const u8 = null,
        flags: SliderFlags = .{},
    };
}
pub fn dragScalarN(label: [:0]const u8, comptime T: type, args: DragScalarNGen(T)) bool {
    const ScalarType = @typeInfo(T).Array.child;
    const components = @typeInfo(T).Array.len;
    return zguiDragScalarN(
        label,
        typeToDataTypeEnum(ScalarType),
        args.v,
        components,
        args.speed,
        if (args.min) |vm| &vm else null,
        if (args.max) |vm| &vm else null,
        if (args.cfmt) |fmt| fmt else null,
        args.flags,
    );
}
extern fn zguiDragScalarN(
    label: [*:0]const u8,
    data_type: DataType,
    pdata: *anyopaque,
    components: i32,
    speed: f32,
    pmin: ?*const anyopaque,
    pmax: ?*const anyopaque,
    cfmt: ?[*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
//
// Widgets: Regular Sliders
//
//--------------------------------------------------------------------------------------------------
fn SliderFloatGen(comptime T: type) type {
    return struct {
        v: *T,
        min: f32,
        max: f32,
        cfmt: [:0]const u8 = "%.3f",
        flags: SliderFlags = .{},
    };
}
//--------------------------------------------------------------------------------------------------
const SliderFloat = SliderFloatGen(f32);
pub fn sliderFloat(label: [:0]const u8, args: SliderFloat) bool {
    return zguiSliderFloat(label, args.v, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiSliderFloat(
    label: [*:0]const u8,
    v: *f32,
    min: f32,
    max: f32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderFloat2 = SliderFloatGen([2]f32);
pub fn sliderFloat2(label: [:0]const u8, args: SliderFloat2) bool {
    return zguiSliderFloat2(label, args.v, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiSliderFloat2(
    label: [*:0]const u8,
    v: *[2]f32,
    min: f32,
    max: f32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderFloat3 = SliderFloatGen([3]f32);
pub fn sliderFloat3(label: [:0]const u8, args: SliderFloat3) bool {
    return zguiSliderFloat3(label, args.v, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiSliderFloat3(
    label: [*:0]const u8,
    v: *[3]f32,
    min: f32,
    max: f32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderFloat4 = SliderFloatGen([4]f32);
pub fn sliderFloat4(label: [:0]const u8, args: SliderFloat4) bool {
    return zguiSliderFloat4(label, args.v, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiSliderFloat4(
    label: [*:0]const u8,
    v: *[4]f32,
    min: f32,
    max: f32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
fn SliderIntGen(comptime T: type) type {
    return struct {
        v: *T,
        min: i32,
        max: i32,
        cfmt: [:0]const u8 = "%d",
        flags: SliderFlags = .{},
    };
}
//--------------------------------------------------------------------------------------------------
const SliderInt = SliderIntGen(i32);
pub fn sliderInt(label: [:0]const u8, args: SliderInt) bool {
    return zguiSliderInt(label, args.v, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiSliderInt(
    label: [*:0]const u8,
    v: *i32,
    min: i32,
    max: i32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderInt2 = SliderIntGen([2]i32);
pub fn sliderInt2(label: [:0]const u8, args: SliderInt2) bool {
    return zguiSliderInt2(label, args.v, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiSliderInt2(
    label: [*:0]const u8,
    v: *[2]i32,
    min: i32,
    max: i32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderInt3 = SliderIntGen([3]i32);
pub fn sliderInt3(label: [:0]const u8, args: SliderInt3) bool {
    return zguiSliderInt3(label, args.v, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiSliderInt3(
    label: [*:0]const u8,
    v: *[3]i32,
    min: i32,
    max: i32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderInt4 = SliderIntGen([4]i32);
pub fn sliderInt4(label: [:0]const u8, args: SliderInt4) bool {
    return zguiSliderInt4(label, args.v, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiSliderInt4(
    label: [*:0]const u8,
    v: *[4]i32,
    min: i32,
    max: i32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
fn SliderScalarGen(comptime T: type) type {
    return struct {
        v: *T,
        min: T,
        max: T,
        cfmt: ?[:0]const u8 = null,
        flags: SliderFlags = .{},
    };
}
pub fn sliderScalar(label: [:0]const u8, comptime T: type, args: SliderScalarGen(T)) bool {
    return zguiSliderScalar(
        label,
        typeToDataTypeEnum(T),
        args.v,
        &args.min,
        &args.max,
        if (args.cfmt) |fmt| fmt else null,
        args.flags,
    );
}
extern fn zguiSliderScalar(
    label: [*:0]const u8,
    data_type: DataType,
    pdata: *anyopaque,
    pmin: *const anyopaque,
    pmax: *const anyopaque,
    cfmt: ?[*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
fn SliderScalarNGen(comptime T: type) type {
    const ScalarType = @typeInfo(T).Array.child;
    return struct {
        v: *T,
        min: ScalarType,
        max: ScalarType,
        cfmt: ?[:0]const u8 = null,
        flags: SliderFlags = .{},
    };
}
pub fn sliderScalarN(label: [:0]const u8, comptime T: type, args: SliderScalarNGen(T)) bool {
    const ScalarType = @typeInfo(T).Array.child;
    const components = @typeInfo(T).Array.len;
    return zguiSliderScalarN(
        label,
        typeToDataTypeEnum(ScalarType),
        args.v,
        components,
        &args.min,
        &args.max,
        if (args.cfmt) |fmt| fmt else null,
        args.flags,
    );
}
extern fn zguiSliderScalarN(
    label: [*:0]const u8,
    data_type: DataType,
    pdata: *anyopaque,
    components: i32,
    pmin: *const anyopaque,
    pmax: *const anyopaque,
    cfmt: ?[*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const VSliderFloat = struct {
    w: f32,
    h: f32,
    v: *f32,
    min: f32,
    max: f32,
    cfmt: [:0]const u8 = "%.3f",
    flags: SliderFlags = .{},
};
pub fn vsliderFloat(label: [:0]const u8, args: VSliderFloat) bool {
    return zguiVSliderFloat(label, args.w, args.h, args.v, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiVSliderFloat(
    label: [*:0]const u8,
    w: f32,
    h: f32,
    v: *f32,
    min: f32,
    max: f32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const VSliderInt = struct {
    w: f32,
    h: f32,
    v: *i32,
    min: i32,
    max: i32,
    cfmt: [:0]const u8 = "%d",
    flags: SliderFlags = .{},
};
pub fn vsliderInt(label: [:0]const u8, args: VSliderInt) bool {
    return zguiVSliderInt(label, args.w, args.h, args.v, args.min, args.max, args.cfmt, args.flags);
}
extern fn zguiVSliderInt(
    label: [*:0]const u8,
    w: f32,
    h: f32,
    v: *i32,
    min: i32,
    max: i32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
fn VSliderScalarGen(comptime T: type) type {
    return struct {
        w: f32,
        h: f32,
        v: *T,
        min: T,
        max: T,
        cfmt: ?[:0]const u8 = null,
        flags: SliderFlags = .{},
    };
}
pub fn vsliderScalar(label: [:0]const u8, comptime T: type, args: VSliderScalarGen(T)) bool {
    return zguiVSliderScalar(
        label,
        args.w,
        args.h,
        typeToDataTypeEnum(T),
        args.v,
        &args.min,
        &args.max,
        if (args.cfmt) |fmt| fmt else null,
        args.flags,
    );
}
extern fn zguiVSliderScalar(
    label: [*:0]const u8,
    w: f32,
    h: f32,
    data_type: DataType,
    pdata: *anyopaque,
    pmin: *const anyopaque,
    pmax: *const anyopaque,
    cfmt: ?[*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const SliderAngle = struct {
    vrad: *f32,
    deg_min: f32 = -360.0,
    deg_max: f32 = 360.0,
    cfmt: [:0]const u8 = "%.0f deg",
    flags: SliderFlags = .{},
};
pub fn sliderAngle(label: [:0]const u8, args: SliderAngle) bool {
    return zguiSliderAngle(
        label,
        args.vrad,
        args.deg_min,
        args.deg_max,
        args.cfmt,
        args.flags,
    );
}
extern fn zguiSliderAngle(
    label: [*:0]const u8,
    vrad: *f32,
    deg_min: f32,
    deg_max: f32,
    cfmt: [*:0]const u8,
    flags: SliderFlags,
) bool;
//--------------------------------------------------------------------------------------------------
//
// Widgets: Input with Keyboard
//
//--------------------------------------------------------------------------------------------------
pub const InputTextFlags = packed struct(u32) {
    chars_decimal: bool = false,
    chars_hexadecimal: bool = false,
    chars_uppercase: bool = false,
    chars_no_blank: bool = false,
    auto_select_all: bool = false,
    enter_returns_true: bool = false,
    callback_completion: bool = false,
    callback_history: bool = false,
    callback_always: bool = false,
    callback_char_filter: bool = false,
    allow_tab_input: bool = false,
    ctrl_enter_for_new_line: bool = false,
    no_horizontal_scroll: bool = false,
    always_overwrite: bool = false,
    read_only: bool = false,
    password: bool = false,
    no_undo_redo: bool = false,
    chars_scientific: bool = false,
    callback_resize: bool = false,
    callback_edit: bool = false,
    _padding: u12 = 0,
};
//--------------------------------------------------------------------------------------------------
pub const InputTextCallbackData = extern struct {
    event_flag: InputTextFlags,
    flags: InputTextFlags,
    user_data: ?*anyopaque,
    event_char: Wchar,
    event_key: Key,
    buf: [*]u8,
    buf_text_len: i32,
    buf_size: i32,
    buf_dirty: bool,
    cursor_pos: i32,
    selection_start: i32,
    selection_end: i32,

    /// `pub fn init() InputTextCallbackData`
    pub const init = zguiInputTextCallbackData_init;

    /// `pub fn deleteChars(data: *InputTextCallbackData, pos: i32, bytes_count: i32) void`
    pub const deleteChars = zguiInputTextCallbackData_deleteChars;

    pub fn insertChars(data: *InputTextCallbackData, pos: i32, txt: []const u8) void {
        zguiInputTextCallbackData_insertChars(data, pos, txt.ptr, txt.ptr + txt.len);
    }

    pub fn selectAll(data: *InputTextCallbackData) void {
        data.selection_start = 0;
        data.selection_end = data.buf_text_len;
    }

    pub fn clearSelection(data: *InputTextCallbackData) void {
        data.selection_start = data.buf_text_len;
        data.selection_end = data.buf_text_len;
    }

    pub fn hasSelection(data: InputTextCallbackData) bool {
        return data.selection_start != data.selection_end;
    }

    extern fn zguiInputTextCallbackData_init() InputTextCallbackData;
    extern fn zguiInputTextCallbackData_deleteChars(
        data: *InputTextCallbackData,
        pos: i32,
        bytes_count: i32,
    ) void;
    extern fn zguiInputTextCallbackData_insertChars(
        data: *InputTextCallbackData,
        pos: i32,
        text: [*]const u8,
        text_end: [*]const u8,
    ) void;
};

pub const InputTextCallback = if (@import("builtin").zig_backend == .stage1)
    fn (data: *InputTextCallbackData) i32
else
    *const fn (data: *InputTextCallbackData) i32;
//--------------------------------------------------------------------------------------------------
const InputText = struct {
    buf: []u8,
    flags: InputTextFlags = .{},
    callback: ?InputTextCallback = null,
    user_data: ?*anyopaque = null,
};
pub fn inputText(label: [:0]const u8, args: InputText) bool {
    return zguiInputText(
        label,
        args.buf.ptr,
        args.buf.len,
        args.flags,
        if (args.callback) |cb| cb else null,
        args.user_data,
    );
}
extern fn zguiInputText(
    label: [*:0]const u8,
    buf: [*]u8,
    buf_size: usize,
    flags: InputTextFlags,
    callback: ?*const anyopaque,
    user_data: ?*anyopaque,
) bool;
//--------------------------------------------------------------------------------------------------
const InputTextMultiline = struct {
    buf: []u8,
    w: f32 = 0.0,
    h: f32 = 0.0,
    flags: InputTextFlags = .{},
    callback: ?InputTextCallback = null,
    user_data: ?*anyopaque = null,
};
pub fn inputTextMultiline(label: [:0]const u8, args: InputTextMultiline) bool {
    return zguiInputTextMultiline(
        label,
        args.buf.ptr,
        args.buf.len,
        args.w,
        args.h,
        args.flags,
        if (args.callback) |cb| cb else null,
        args.user_data,
    );
}
extern fn zguiInputTextMultiline(
    label: [*:0]const u8,
    buf: [*]u8,
    buf_size: usize,
    w: f32,
    h: f32,
    flags: InputTextFlags,
    callback: ?*const anyopaque,
    user_data: ?*anyopaque,
) bool;
//--------------------------------------------------------------------------------------------------
const InputTextWithHint = struct {
    hint: [:0]const u8,
    buf: []u8,
    flags: InputTextFlags = .{},
    callback: ?InputTextCallback = null,
    user_data: ?*anyopaque = null,
};
pub fn inputTextWithHint(label: [:0]const u8, args: InputTextWithHint) bool {
    return zguiInputTextWithHint(
        label,
        args.hint,
        args.buf.ptr,
        args.buf.len,
        args.flags,
        if (args.callback) |cb| cb else null,
        args.user_data,
    );
}
extern fn zguiInputTextWithHint(
    label: [*:0]const u8,
    hint: [*:0]const u8,
    buf: [*]u8,
    buf_size: usize,
    flags: InputTextFlags,
    callback: ?*const anyopaque,
    user_data: ?*anyopaque,
) bool;
//--------------------------------------------------------------------------------------------------
const InputFloat = struct {
    v: *f32,
    step: f32 = 0.0,
    step_fast: f32 = 0.0,
    cfmt: [:0]const u8 = "%.3f",
    flags: InputTextFlags = .{},
};
pub fn inputFloat(label: [:0]const u8, args: InputFloat) bool {
    return zguiInputFloat(
        label,
        args.v,
        args.step,
        args.step_fast,
        args.cfmt,
        args.flags,
    );
}
extern fn zguiInputFloat(
    label: [*:0]const u8,
    v: *f32,
    step: f32,
    step_fast: f32,
    cfmt: [*:0]const u8,
    flags: InputTextFlags,
) bool;
//--------------------------------------------------------------------------------------------------
fn InputFloatGen(comptime T: type) type {
    return struct {
        v: *T,
        cfmt: [:0]const u8 = "%.3f",
        flags: InputTextFlags = .{},
    };
}
//--------------------------------------------------------------------------------------------------
const InputFloat2 = InputFloatGen([2]f32);
pub fn inputFloat2(label: [:0]const u8, args: InputFloat2) bool {
    return zguiInputFloat2(label, args.v, args.cfmt, args.flags);
}
extern fn zguiInputFloat2(
    label: [*:0]const u8,
    v: *[2]f32,
    cfmt: [*:0]const u8,
    flags: InputTextFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const InputFloat3 = InputFloatGen([3]f32);
pub fn inputFloat3(label: [:0]const u8, args: InputFloat3) bool {
    return zguiInputFloat3(label, args.v, args.cfmt, args.flags);
}
extern fn zguiInputFloat3(
    label: [*:0]const u8,
    v: *[3]f32,
    cfmt: [*:0]const u8,
    flags: InputTextFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const InputFloat4 = InputFloatGen([4]f32);
pub fn inputFloat4(label: [:0]const u8, args: InputFloat4) bool {
    return zguiInputFloat4(label, args.v, args.cfmt, args.flags);
}
extern fn zguiInputFloat4(
    label: [*:0]const u8,
    v: *[4]f32,
    cfmt: [*:0]const u8,
    flags: InputTextFlags,
) bool;
//--------------------------------------------------------------------------------------------------
const InputInt = struct {
    v: *i32,
    step: i32 = 1,
    step_fast: i32 = 100,
    flags: InputTextFlags = .{},
};
pub fn inputInt(label: [:0]const u8, args: InputInt) bool {
    return zguiInputInt(label, args.v, args.step, args.step_fast, args.flags);
}
extern fn zguiInputInt(
    label: [*:0]const u8,
    v: *i32,
    step: i32,
    step_fast: i32,
    flags: InputTextFlags,
) bool;
//--------------------------------------------------------------------------------------------------
fn InputIntGen(comptime T: type) type {
    return struct {
        v: *T,
        flags: InputTextFlags = .{},
    };
}
//--------------------------------------------------------------------------------------------------
const InputInt2 = InputIntGen([2]i32);
pub fn inputInt2(label: [:0]const u8, args: InputInt2) bool {
    return zguiInputInt2(label, args.v, args.flags);
}
extern fn zguiInputInt2(label: [*:0]const u8, v: *[2]i32, flags: InputTextFlags) bool;
//--------------------------------------------------------------------------------------------------
const InputInt3 = InputIntGen([3]i32);
pub fn inputInt3(label: [:0]const u8, args: InputInt3) bool {
    return zguiInputInt3(label, args.v, args.flags);
}
extern fn zguiInputInt3(label: [*:0]const u8, v: *[3]i32, flags: InputTextFlags) bool;
//--------------------------------------------------------------------------------------------------
const InputInt4 = InputIntGen([4]i32);
pub fn inputInt4(label: [:0]const u8, args: InputInt4) bool {
    return zguiInputInt4(label, args.v, args.flags);
}
extern fn zguiInputInt4(label: [*:0]const u8, v: *[4]i32, flags: InputTextFlags) bool;
//--------------------------------------------------------------------------------------------------
const InputDouble = struct {
    v: *f64,
    step: f64 = 0.0,
    step_fast: f64 = 0.0,
    cfmt: [:0]const u8 = "%.6f",
    flags: InputTextFlags = .{},
};
pub fn inputDouble(label: [:0]const u8, args: InputDouble) bool {
    return zguiInputDouble(label, args.v, args.step, args.step_fast, args.cfmt, args.flags);
}
extern fn zguiInputDouble(
    label: [*:0]const u8,
    v: *f64,
    step: f64,
    step_fast: f64,
    cfmt: [*:0]const u8,
    flags: InputTextFlags,
) bool;
//--------------------------------------------------------------------------------------------------
fn InputScalarGen(comptime T: type) type {
    return struct {
        v: *T,
        step: ?T = null,
        step_fast: ?T = null,
        cfmt: ?[:0]const u8 = null,
        flags: InputTextFlags = .{},
    };
}
pub fn inputScalar(label: [:0]const u8, comptime T: type, args: InputScalarGen(T)) bool {
    return zguiInputScalar(
        label,
        typeToDataTypeEnum(T),
        args.v,
        if (args.step) |s| &s else null,
        if (args.step_fast) |sf| &sf else null,
        if (args.cfmt) |fmt| fmt else null,
        args.flags,
    );
}
extern fn zguiInputScalar(
    label: [*:0]const u8,
    data_type: DataType,
    pdata: *anyopaque,
    pstep: ?*const anyopaque,
    pstep_fast: ?*const anyopaque,
    cfmt: ?[*:0]const u8,
    flags: InputTextFlags,
) bool;
//--------------------------------------------------------------------------------------------------
fn InputScalarNGen(comptime T: type) type {
    const ScalarType = @typeInfo(T).Array.child;
    return struct {
        v: *T,
        step: ?ScalarType = null,
        step_fast: ?ScalarType = null,
        cfmt: ?[:0]const u8 = null,
        flags: InputTextFlags = .{},
    };
}
pub fn inputScalarN(label: [:0]const u8, comptime T: type, args: InputScalarNGen(T)) bool {
    const ScalarType = @typeInfo(T).Array.child;
    const components = @typeInfo(T).Array.len;
    return zguiInputScalarN(
        label,
        typeToDataTypeEnum(ScalarType),
        args.v,
        components,
        if (args.step) |s| &s else null,
        if (args.step_fast) |sf| &sf else null,
        if (args.cfmt) |fmt| fmt else null,
        args.flags,
    );
}
extern fn zguiInputScalarN(
    label: [*:0]const u8,
    data_type: DataType,
    pdata: *anyopaque,
    components: i32,
    pstep: ?*const anyopaque,
    pstep_fast: ?*const anyopaque,
    cfmt: ?[*:0]const u8,
    flags: InputTextFlags,
) bool;
//--------------------------------------------------------------------------------------------------
//
// Widgets: Color Editor/Picker
//
//--------------------------------------------------------------------------------------------------
pub const ColorEditFlags = packed struct(u32) {
    no_alpha: bool = false,
    no_picker: bool = false,
    no_options: bool = false,
    no_small_preview: bool = false,
    no_inputs: bool = false,
    no_tooltip: bool = false,
    no_label: bool = false,
    no_side_preview: bool = false,
    no_drag_drop: bool = false,
    no_border: bool = false,

    _reserved0: bool = false,
    _reserved1: bool = false,
    _reserved2: bool = false,
    _reserved3: bool = false,
    _reserved4: bool = false,

    alpha_bar: bool = false,
    alpha_preview: bool = false,
    alpha_preview_half: bool = false,
    hdr: bool = false,
    display_rgb: bool = false,
    display_hsv: bool = false,
    display_hex: bool = false,
    uint8: bool = false,
    float: bool = false,
    picker_hue_bar: bool = false,
    picker_hue_wheel: bool = false,
    input_rgb: bool = false,
    input_hsv: bool = false,

    _padding: u4 = 0,

    pub const default_options = ColorEditFlags{
        .uint8 = true,
        .display_rgb = true,
        .input_rgb = true,
        .picker_hue_bar = true,
    };
};
//--------------------------------------------------------------------------------------------------
const ColorEdit3 = struct {
    col: *[3]f32,
    flags: ColorEditFlags = .{},
};
pub fn colorEdit3(label: [:0]const u8, args: ColorEdit3) bool {
    return zguiColorEdit3(label, args.col, args.flags);
}
extern fn zguiColorEdit3(label: [*:0]const u8, col: *[3]f32, flags: ColorEditFlags) bool;
//--------------------------------------------------------------------------------------------------
const ColorEdit4 = struct {
    col: *[4]f32,
    flags: ColorEditFlags = .{},
};
pub fn colorEdit4(label: [:0]const u8, args: ColorEdit4) bool {
    return zguiColorEdit4(label, args.col, args.flags);
}
extern fn zguiColorEdit4(label: [*:0]const u8, col: *[4]f32, flags: ColorEditFlags) bool;
//--------------------------------------------------------------------------------------------------
const ColorPicker3 = struct {
    col: *[3]f32,
    flags: ColorEditFlags = .{},
};
pub fn colorPicker3(label: [:0]const u8, args: ColorPicker3) bool {
    return zguiColorPicker3(label, args.col, args.flags);
}
extern fn zguiColorPicker3(label: [*:0]const u8, col: *[3]f32, flags: ColorEditFlags) bool;
//--------------------------------------------------------------------------------------------------
const ColorPicker4 = struct {
    col: *[4]f32,
    flags: ColorEditFlags = .{},
    ref_col: ?[*]const f32 = null,
};
pub fn colorPicker4(label: [:0]const u8, args: ColorPicker4) bool {
    return zguiColorPicker4(
        label,
        args.col,
        args.flags,
        if (args.ref_col) |rc| rc else null,
    );
}
extern fn zguiColorPicker4(
    label: [*:0]const u8,
    col: *[4]f32,
    flags: ColorEditFlags,
    ref_col: ?[*]const f32,
) bool;
//--------------------------------------------------------------------------------------------------
const ColorButton = struct {
    col: [4]f32,
    flags: ColorEditFlags = .{},
    w: f32 = 0.0,
    h: f32 = 0.0,
};
pub fn colorButton(desc_id: [:0]const u8, args: ColorButton) bool {
    return zguiColorButton(desc_id, &args.col, args.flags, args.w, args.h);
}
extern fn zguiColorButton(
    desc_id: [*:0]const u8,
    col: *const [4]f32,
    flags: ColorEditFlags,
    w: f32,
    h: f32,
) bool;
//--------------------------------------------------------------------------------------------------
//
// Widgets: Trees
//
//--------------------------------------------------------------------------------------------------
pub const TreeNodeFlags = packed struct(u32) {
    selected: bool = false,
    framed: bool = false,
    allow_item_overlap: bool = false,
    no_tree_push_on_open: bool = false,
    no_auto_open_on_log: bool = false,
    default_open: bool = false,
    open_on_double_click: bool = false,
    open_on_arrow: bool = false,
    leaf: bool = false,
    bullet: bool = false,
    frame_padding: bool = false,
    span_avail_width: bool = false,
    span_full_width: bool = false,
    nav_left_jumps_back_here: bool = false,
    _padding: u18 = 0,

    pub const collapsing_header = TreeNodeFlags{
        .framed = true,
        .no_tree_push_on_open = true,
        .no_auto_open_on_log = true,
    };
};
//--------------------------------------------------------------------------------------------------
pub fn treeNode(label: [:0]const u8) bool {
    return zguiTreeNode(label);
}
pub fn treeNodeFlags(label: [:0]const u8, flags: TreeNodeFlags) bool {
    return zguiTreeNodeFlags(label, flags);
}
extern fn zguiTreeNode(label: [*:0]const u8) bool;
extern fn zguiTreeNodeFlags(label: [*:0]const u8, flags: TreeNodeFlags) bool;
//--------------------------------------------------------------------------------------------------
pub fn treeNodeStrId(str_id: [:0]const u8, comptime fmt: []const u8, args: anytype) bool {
    return zguiTreeNodeStrId(str_id, "%s", formatZ(fmt, args).ptr);
}
pub fn treeNodeStrIdFlags(
    str_id: [:0]const u8,
    flags: TreeNodeFlags,
    comptime fmt: []const u8,
    args: anytype,
) bool {
    return zguiTreeNodeStrIdFlags(str_id, flags, "%s", formatZ(fmt, args).ptr);
}
extern fn zguiTreeNodeStrId(str_id: [*:0]const u8, fmt: [*:0]const u8, ...) bool;
extern fn zguiTreeNodeStrIdFlags(
    str_id: [*:0]const u8,
    flags: TreeNodeFlags,
    fmt: [*:0]const u8,
    ...,
) bool;
//--------------------------------------------------------------------------------------------------
pub fn treeNodePtrId(ptr_id: *const anyopaque, comptime fmt: []const u8, args: anytype) bool {
    return zguiTreeNodePtrId(ptr_id, "%s", formatZ(fmt, args).ptr);
}
pub fn treeNodePtrIdFlags(
    ptr_id: *const anyopaque,
    flags: TreeNodeFlags,
    comptime fmt: []const u8,
    args: anytype,
) bool {
    return zguiTreeNodePtrIdFlags(ptr_id, flags, "%s", formatZ(fmt, args).ptr);
}
extern fn zguiTreeNodePtrId(ptr_id: *const anyopaque, fmt: [*:0]const u8, ...) bool;
extern fn zguiTreeNodePtrIdFlags(
    ptr_id: *const anyopaque,
    flags: TreeNodeFlags,
    fmt: [*:0]const u8,
    ...,
) bool;
//--------------------------------------------------------------------------------------------------
pub fn treePushStrId(str_id: [:0]const u8) void {
    zguiTreePushStrId(str_id);
}
pub fn treePushPtrId(ptr_id: *const anyopaque) void {
    zguiTreePushPtrId(ptr_id);
}
extern fn zguiTreePushStrId(str_id: [*:0]const u8) void;
extern fn zguiTreePushPtrId(ptr_id: *const anyopaque) void;
//--------------------------------------------------------------------------------------------------
/// `pub fn treePop() void`
pub const treePop = zguiTreePop;
extern fn zguiTreePop() void;
//--------------------------------------------------------------------------------------------------
const CollapsingHeaderStatePtr = struct {
    pvisible: *bool,
    flags: TreeNodeFlags = .{},
};
pub fn collapsingHeader(label: [:0]const u8, flags: TreeNodeFlags) bool {
    return zguiCollapsingHeader(label, flags);
}
pub fn collapsingHeaderStatePtr(label: [:0]const u8, args: CollapsingHeaderStatePtr) bool {
    return zguiCollapsingHeaderStatePtr(label, args.pvisible, args.flags);
}
extern fn zguiCollapsingHeader(label: [*:0]const u8, flags: TreeNodeFlags) bool;
extern fn zguiCollapsingHeaderStatePtr(label: [*:0]const u8, pvisible: bool, flags: TreeNodeFlags) bool;
//--------------------------------------------------------------------------------------------------
const SetNextItemOpen = struct {
    is_open: bool,
    cond: Condition = .none,
};
pub fn setNextItemOpen(args: SetNextItemOpen) void {
    zguiSetNextItemOpen(args.is_open, args.cond);
}
extern fn zguiSetNextItemOpen(is_open: bool, cond: Condition) void;
//--------------------------------------------------------------------------------------------------
//
// Selectables
//
//--------------------------------------------------------------------------------------------------
pub const SelectableFlags = packed struct(u32) {
    dont_close_popups: bool = false,
    span_all_colums: bool = false,
    allow_double_click: bool = false,
    disabled: bool = false,
    allow_item_overlap: bool = false,
    _padding: u27 = 0,
};
//--------------------------------------------------------------------------------------------------
const Selectable = struct {
    selected: bool = false,
    flags: SelectableFlags = .{},
    w: f32 = 0,
    h: f32 = 0,
};
pub fn selectable(label: [:0]const u8, args: Selectable) bool {
    return zguiSelectable(label, args.selected, args.flags, args.w, args.h);
}
extern fn zguiSelectable(
    label: [*:0]const u8,
    selected: bool,
    flags: SelectableFlags,
    w: f32,
    h: f32,
) bool;
//--------------------------------------------------------------------------------------------------
const SelectableStatePtr = struct {
    pselected: *bool,
    flags: SelectableFlags = .{},
    w: f32 = 0,
    h: f32 = 0,
};
pub fn selectableStatePtr(label: [:0]const u8, args: SelectableStatePtr) bool {
    return zguiSelectableStatePtr(label, args.pselected, args.flags, args.w, args.h);
}
extern fn zguiSelectableStatePtr(
    label: [*:0]const u8,
    pselected: *bool,
    flags: SelectableFlags,
    w: f32,
    h: f32,
) bool;
//--------------------------------------------------------------------------------------------------
//
// Widgets: List Boxes
//
//--------------------------------------------------------------------------------------------------
const BeginListBox = struct {
    w: f32 = 0.0,
    h: f32 = 0.0,
};
pub fn beginListBox(label: [:0]const u8, args: BeginListBox) bool {
    return zguiBeginListBox(label, args.w, args.h);
}
/// `pub fn endListBox() void`
pub const endListBox = zguiEndListBox;
extern fn zguiBeginListBox(label: [*:0]const u8, w: f32, h: f32) bool;
extern fn zguiEndListBox() void;
//--------------------------------------------------------------------------------------------------
//
// Item/Widgets Utilities and Query Functions
//
//--------------------------------------------------------------------------------------------------
pub fn isItemHovered(flags: HoveredFlags) bool {
    return zguiIsItemHovered(flags);
}
/// `pub fn isItemActive() bool`
pub const isItemActive = zguiIsItemActive;
/// `pub fn isItemFocused() bool`
pub const isItemFocused = zguiIsItemFocused;
pub const MouseButton = enum(u32) {
    left = 0,
    right = 1,
    middle = 2,
};
/// `pub fn isItemClicked(mouse_button: MouseButton) bool`
pub const isItemClicked = zguiIsItemClicked;
/// `pub fn isItemVisible() bool`
pub const isItemVisible = zguiIsItemVisible;
/// `pub fn isItemEdited() bool`
pub const isItemEdited = zguiIsItemEdited;
/// `pub fn isItemActivated() bool`
pub const isItemActivated = zguiIsItemActivated;
/// `pub fn isItemDeactivated bool`
pub const isItemDeactivated = zguiIsItemDeactivated;
/// `pub fn isItemDeactivatedAfterEdit() bool`
pub const isItemDeactivatedAfterEdit = zguiIsItemDeactivatedAfterEdit;
/// `pub fn isItemToggledOpen() bool`
pub const isItemToggledOpen = zguiIsItemToggledOpen;
/// `pub fn isAnyItemHovered() bool`
pub const isAnyItemHovered = zguiIsAnyItemHovered;
/// `pub fn isAnyItemActive() bool`
pub const isAnyItemActive = zguiIsAnyItemActive;
/// `pub fn isAnyItemFocused() bool`
pub const isAnyItemFocused = zguiIsAnyItemFocused;
extern fn zguiIsItemHovered(flags: HoveredFlags) bool;
extern fn zguiIsItemActive() bool;
extern fn zguiIsItemFocused() bool;
extern fn zguiIsItemClicked(mouse_button: MouseButton) bool;
extern fn zguiIsItemVisible() bool;
extern fn zguiIsItemEdited() bool;
extern fn zguiIsItemActivated() bool;
extern fn zguiIsItemDeactivated() bool;
extern fn zguiIsItemDeactivatedAfterEdit() bool;
extern fn zguiIsItemToggledOpen() bool;
extern fn zguiIsAnyItemHovered() bool;
extern fn zguiIsAnyItemActive() bool;
extern fn zguiIsAnyItemFocused() bool;
//--------------------------------------------------------------------------------------------------
//
// Internal Helpers
//
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
//
// Tabs
//
//--------------------------------------------------------------------------------------------------
pub fn beginTabBar(label: [:0]const u8) bool {
    return zguiBeginTabBar(label);
}
pub fn beginTabItem(label: [:0]const u8) bool {
    return zguiBeginTabItem(label);
}
pub const endTabItem = zguiEndTabItem;
pub const endTabBar = zguiEndTabBar;

extern fn zguiBeginTabBar(label: [*:0]const u8) bool;
extern fn zguiBeginTabItem(label: [*:0]const u8) bool;
extern fn zguiEndTabItem() void;
extern fn zguiEndTabBar() void;

//--------------------------------------------------------------------------------------------------
//
// Viewport
//
//--------------------------------------------------------------------------------------------------
pub const Viewport = *opaque {
    pub fn getWorkPos(viewport: Viewport) [2]f32 {
       var pos: [2]f32 = undefined;
       zguiViewport_GetWorkPos(viewport, &pos);
       return pos;
    }
    extern fn zguiViewport_GetWorkPos(viewport: Viewport, pos: *[2]f32) void;
    pub fn getWorkSize(viewport: Viewport) [2]f32 {
       var pos: [2]f32 = undefined;
        zguiViewport_GetWorkSize(viewport, &pos);
        return pos;
    }
    extern fn zguiViewport_GetWorkSize(viewport: Viewport, size: *[2]f32) void;
};
pub const getMainViewport = zguiGetMainViewport;
extern fn zguiGetMainViewport() Viewport;


//--------------------------------------------------------------------------------------------------
//
// DrawList
//
//--------------------------------------------------------------------------------------------------
pub const DrawFlags = packed struct(u32) {
    closed: bool = false,
    _padding0: u3 = 0,
    round_corners_top_left: bool = false,
    round_corners_top_right: bool = false,
    round_corners_bottom_left: bool = false,
    round_corners_bottom_right: bool = false,
    round_corners_none: bool = false,
    _padding1: u23 = 0,

    pub const round_corners_top = DrawFlags{
        .round_corners_top_left = true,
        .round_corners_top_right = true,
    };

    pub const round_corners_bottom = DrawFlags{
        .round_corners_bottom_left = true,
        .round_corners_bottom_right = true,
    };

    pub const round_corners_left = DrawFlags{
        .round_corners_top_left = true,
        .round_corners_bottom_left = true,
    };

    pub const round_corners_right = DrawFlags{
        .round_corners_top_right = true,
        .round_corners_bottom_right = true,
    };

    pub const round_corners_all = DrawFlags{
        .round_corners_top_left = true,
        .round_corners_top_right = true,
        .round_corners_bottom_left = true,
        .round_corners_bottom_right = true,
    };
};

pub const getWindowDrawList = zguiGetWindowDrawList;
pub const getBackgroundDrawList = zguiGetBackgroundDrawList;
pub const getForegroundDrawList = zguiGetForegroundDrawList;

extern fn zguiGetWindowDrawList() DrawList;
extern fn zguiGetBackgroundDrawList() DrawList;
extern fn zguiGetForegroundDrawList() DrawList;

pub const DrawList = *opaque {
    //----------------------------------------------------------------------------------------------
    const ClipRect = struct {
        pmin: [2]f32,
        pmax: [2]f32,
        intersect_with_current: bool = false,
    };
    pub fn pushClipRect(draw_list: DrawList, args: ClipRect) void {
        zguiDrawList_PushClipRect(
            draw_list,
            &args.pmin,
            &args.pmax,
            args.intersect_with_current,
        );
    }
    extern fn zguiDrawList_PushClipRect(
        draw_list: DrawList,
        clip_rect_min: *const [2]f32,
        clip_rect_max: *const [2]f32,
        intersect_with_current_clip_rect: bool,
    ) void;
    //----------------------------------------------------------------------------------------------
    pub const pushClipRectFullScreen = zguiDrawList_PushClipRectFullScreen;
    extern fn zguiDrawList_PushClipRectFullScreen(draw_list: DrawList) void;

    pub const popClipRect = zguiDrawList_PopClipRect;
    extern fn zguiDrawList_PopClipRect(draw_list: DrawList) void;
    //----------------------------------------------------------------------------------------------
    pub const pushTextureId = zguiDrawList_PushTextureId;
    extern fn zguiDrawList_PushTextureId(draw_list: DrawList, texture_id: TextureIdent) void;

    pub const popTextureId = zguiDrawList_PopTextureId;
    extern fn zguiDrawList_PopTextureId(draw_list: DrawList) void;
    //----------------------------------------------------------------------------------------------
    pub fn getClipRectMin(draw_list: DrawList) [2]f32 {
        var v: [2]f32 = undefined;
        zguiDrawList_GetClipRectMin(draw_list, &v);
        return v;
    }
    extern fn zguiDrawList_GetClipRectMin(draw_list: DrawList, clip_min: *[2]f32) void;

    pub fn getClipRectMax(draw_list: DrawList) [2]f32 {
        var v: [2]f32 = undefined;
        zguiDrawList_GetClipRectMax(draw_list, &v);
        return v;
    }
    extern fn zguiDrawList_GetClipRectMax(draw_list: DrawList, clip_min: *[2]f32) void;
    //----------------------------------------------------------------------------------------------
    const AddLine = struct {
        p1: [2]f32,
        p2: [2]f32,
        col: u32,
        thickness: f32,
    };
    pub fn addLine(draw_list: DrawList, args: AddLine) void {
        zguiDrawList_AddLine(draw_list, &args.p1, &args.p2, args.col, args.thickness);
    }
    extern fn zguiDrawList_AddLine(
        draw_list: DrawList,
        p1: *const [2]f32,
        p2: *const [2]f32,
        col: u32,
        thickness: f32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddRect = struct {
        pmin: [2]f32,
        pmax: [2]f32,
        col: u32,
        rounding: f32 = 0.0,
        flags: DrawFlags = .{},
        thickness: f32 = 1.0,
    };
    pub fn addRect(draw_list: DrawList, args: AddRect) void {
        zguiDrawList_AddRect(
            draw_list,
            &args.pmin,
            &args.pmax,
            args.col,
            args.rounding,
            args.flags,
            args.thickness,
        );
    }
    extern fn zguiDrawList_AddRect(
        draw_list: DrawList,
        pmin: *const [2]f32,
        pmax: *const [2]f32,
        col: u32,
        rounding: f32,
        flags: DrawFlags,
        thickness: f32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddRectFilled = struct {
        pmin: [2]f32,
        pmax: [2]f32,
        col: u32,
        rounding: f32 = 0.0,
        flags: DrawFlags = .{},
    };
    pub fn addRectFilled(draw_list: DrawList, args: AddRectFilled) void {
        zguiDrawList_AddRectFilled(draw_list, &args.pmin, &args.pmax, args.col, args.rounding, args.flags);
    }
    extern fn zguiDrawList_AddRectFilled(
        draw_list: DrawList,
        pmin: *const [2]f32,
        pmax: *const [2]f32,
        col: u32,
        rounding: f32,
        flags: DrawFlags,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddRectFilledMultiColor = struct {
        pmin: [2]f32,
        pmax: [2]f32,
        col_upr_left: u32,
        col_upr_right: u32,
        col_bot_right: u32,
        col_bot_left: u32,
    };
    pub fn addRectFilledMultiColor(draw_list: DrawList, args: AddRectFilledMultiColor) void {
        zguiDrawList_AddRectFilledMultiColor(
            draw_list,
            &args.pmin,
            &args.pmax,
            args.col_upr_left,
            args.col_upr_right,
            args.col_bot_right,
            args.col_bot_left,
        );
    }
    extern fn zguiDrawList_AddRectFilledMultiColor(
        draw_list: DrawList,
        pmin: *const [2]f32,
        pmax: *const [2]f32,
        col_upr_left: u32,
        col_upr_right: u32,
        col_bot_right: u32,
        col_bot_left: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddQuad = struct {
        p1: [2]f32,
        p2: [2]f32,
        p3: [2]f32,
        p4: [2]f32,
        col: u32,
        thickness: f32 = 1.0,
    };
    pub fn addQuad(draw_list: DrawList, args: AddQuad) void {
        zguiDrawList_AddQuad(draw_list, &args.p1, &args.p2, &args.p3, &args.p4, args.col, args.thickness);
    }
    extern fn zguiDrawList_AddQuad(
        draw_list: DrawList,
        p1: *const [2]f32,
        p2: *const [2]f32,
        p3: *const [2]f32,
        p4: *const [2]f32,
        col: u32,
        thickness: f32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddQuadFilled = struct {
        p1: [2]f32,
        p2: [2]f32,
        p3: [2]f32,
        p4: [2]f32,
        col: u32,
    };
    pub fn addQuadFilled(draw_list: DrawList, args: AddQuadFilled) void {
        zguiDrawList_AddQuadFilled(draw_list, &args.p1, &args.p2, &args.p3, &args.p4, args.col);
    }
    extern fn zguiDrawList_AddQuadFilled(
        draw_list: DrawList,
        p1: *const [2]f32,
        p2: *const [2]f32,
        p3: *const [2]f32,
        p4: *const [2]f32,
        col: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddTriangle = struct {
        p1: [2]f32,
        p2: [2]f32,
        p3: [2]f32,
        col: u32,
        thickness: f32 = 1.0,
    };
    pub fn addTriangle(draw_list: DrawList, args: AddTriangle) void {
        zguiDrawList_AddTriangle(draw_list, &args.p1, &args.p2, &args.p3, args.col, args.thickness);
    }
    extern fn zguiDrawList_AddTriangle(
        draw_list: DrawList,
        p1: *const [2]f32,
        p2: *const [2]f32,
        p3: *const [2]f32,
        col: u32,
        thickness: f32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddTriangleFilled = struct {
        p1: [2]f32,
        p2: [2]f32,
        p3: [2]f32,
        col: u32,
    };
    pub fn addTriangleFilled(draw_list: DrawList, args: AddTriangleFilled) void {
        zguiDrawList_AddTriangleFilled(draw_list, &args.p1, &args.p2, &args.p3, args.col);
    }
    extern fn zguiDrawList_AddTriangleFilled(
        draw_list: DrawList,
        p1: *const [2]f32,
        p2: *const [2]f32,
        p3: *const [2]f32,
        col: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddCircle = struct {
        p: [2]f32,
        r: f32,
        col: u32,
        num_segments: u32 = 0,
        thickness: f32 = 1.0,
    };
    pub fn addCircle(draw_list: DrawList, args: AddCircle) void {
        zguiDrawList_AddCircle(draw_list, &args.p, args.r, args.col, args.num_segments, args.thickness);
    }
    extern fn zguiDrawList_AddCircle(
        draw_list: DrawList,
        center: *const [2]f32,
        radius: f32,
        col: u32,
        num_segments: u32,
        thickness: f32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddCircleFilled = struct {
        p: [2]f32,
        r: f32,
        col: u32,
        num_segments: u32 = 0,
    };
    pub fn addCircleFilled(draw_list: DrawList, args: AddCircleFilled) void {
        zguiDrawList_AddCircleFilled(draw_list, &args.p, args.r, args.col, args.num_segments);
    }
    extern fn zguiDrawList_AddCircleFilled(
        draw_list: DrawList,
        center: *const [2]f32,
        radius: f32,
        col: u32,
        num_segments: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddNgon = struct {
        p: [2]f32,
        r: f32,
        col: u32,
        num_segments: u32,
        thickness: f32 = 1.0,
    };
    pub fn addNgon(draw_list: DrawList, args: AddNgon) void {
        zguiDrawList_AddNgon(draw_list, &args.p, args.r, args.col, args.num_segments, args.thickness);
    }
    extern fn zguiDrawList_AddNgon(
        draw_list: DrawList,
        center: *const [2]f32,
        radius: f32,
        col: u32,
        num_segments: u32,
        thickness: f32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddNgonFilled = struct {
        p: [2]f32,
        r: f32,
        col: u32,
        num_segments: u32,
    };
    pub fn addNgonFilled(draw_list: DrawList, args: AddNgonFilled) void {
        zguiDrawList_AddNgonFilled(draw_list, &args.p, args.r, args.col, args.num_segments);
    }
    extern fn zguiDrawList_AddNgonFilled(
        draw_list: DrawList,
        center: *const [2]f32,
        radius: f32,
        col: u32,
        num_segments: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    pub fn addText(draw_list: DrawList, pos: [2]f32, col: u32, comptime fmt: []const u8, args: anytype) void {
        const txt = format(fmt, args);
        draw_list.addTextUnformatted(pos, col, txt);
    }
    pub fn addTextUnformatted(draw_list: DrawList, pos: [2]f32, col: u32, txt: []const u8) void {
        zguiDrawList_AddText(draw_list, &pos, col, txt.ptr, txt.ptr + txt.len);
    }
    extern fn zguiDrawList_AddText(
        draw_list: DrawList,
        pos: *const [2]f32,
        col: u32,
        text: [*]const u8,
        text_end: [*]const u8,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddPolyline = struct {
        col: u32,
        flags: DrawFlags = .{},
        thickness: f32 = 1.0,
    };
    pub fn addPolyline(draw_list: DrawList, points: []const [2]f32, args: AddPolyline) void {
        zguiDrawList_AddPolyline(
            draw_list,
            points.ptr,
            @intCast(u32, points.len),
            args.col,
            args.flags,
            args.thickness,
        );
    }
    extern fn zguiDrawList_AddPolyline(
        draw_list: DrawList,
        points: [*]const [2]f32,
        num_points: u32,
        col: u32,
        flags: DrawFlags,
        thickness: f32,
    ) void;
    //----------------------------------------------------------------------------------------------
    pub fn addConvexPolyFilled(
        draw_list: DrawList,
        points: []const [2]f32,
        col: u32,
    ) void {
        zguiDrawList_AddConvexPolyFilled(
            draw_list,
            points.ptr,
            @intCast(u32, points.len),
            col,
        );
    }
    extern fn zguiDrawList_AddConvexPolyFilled(
        draw_list: DrawList,
        points: [*]const [2]f32,
        num_points: u32,
        col: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddBezierCubic = struct {
        p1: [2]f32,
        p2: [2]f32,
        p3: [2]f32,
        p4: [2]f32,
        col: u32,
        thickness: f32 = 1.0,
        num_segments: u32 = 0,
    };
    pub fn addBezierCubic(draw_list: DrawList, args: AddBezierCubic) void {
        zguiDrawList_AddBezierCubic(
            draw_list,
            &args.p1,
            &args.p2,
            &args.p3,
            &args.p4,
            args.col,
            args.thickness,
            args.num_segments,
        );
    }
    extern fn zguiDrawList_AddBezierCubic(
        draw_list: DrawList,
        p1: *const [2]f32,
        p2: *const [2]f32,
        p3: *const [2]f32,
        p4: *const [2]f32,
        col: u32,
        thickness: f32,
        num_segments: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddBezierQuadratic = struct {
        p1: [2]f32,
        p2: [2]f32,
        p3: [2]f32,
        col: u32,
        thickness: f32 = 1.0,
        num_segments: u32 = 0,
    };
    pub fn addBezierQuadratic(draw_list: DrawList, args: AddBezierQuadratic) void {
        zguiDrawList_AddBezierQuadratic(
            draw_list,
            &args.p1,
            &args.p2,
            &args.p3,
            args.col,
            args.thickness,
            args.num_segments,
        );
    }
    extern fn zguiDrawList_AddBezierQuadratic(
        draw_list: DrawList,
        p1: *const [2]f32,
        p2: *const [2]f32,
        p3: *const [2]f32,
        col: u32,
        thickness: f32,
        num_segments: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddImage = struct {
        pmin: [2]f32,
        pmax: [2]f32,
        uvmin: [2]f32 = .{ 0, 0 },
        uvmax: [2]f32 = .{ 1, 1 },
        col: u32 = 0xff_ff_ff_ff,
    };
    pub fn addImage(draw_list: DrawList, user_texture_id: TextureIdent, args: AddImage) void {
        zguiDrawList_AddImage(
            draw_list,
            user_texture_id,
            &args.pmin,
            &args.pmax,
            &args.uvmin,
            &args.uvmax,
            args.col,
        );
    }
    extern fn zguiDrawList_AddImage(
        draw_list: DrawList,
        user_texture_id: TextureIdent,
        pmin: *const [2]f32,
        pmax: *const [2]f32,
        uvmin: *const [2]f32,
        uvmax: *const [2]f32,
        col: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddImageQuad = struct {
        p1: [2]f32,
        p2: [2]f32,
        p3: [2]f32,
        p4: [2]f32,
        uv1: [2]f32 = .{ 0, 0 },
        uv2: [2]f32 = .{ 1, 0 },
        uv3: [2]f32 = .{ 1, 1 },
        uv4: [2]f32 = .{ 0, 1 },
        col: u32 = 0xff_ff_ff_ff,
    };
    pub fn addImageQuad(draw_list: DrawList, user_texture_id: TextureIdent, args: AddImageQuad) void {
        zguiDrawList_AddImageQuad(
            draw_list,
            user_texture_id,
            &args.p1,
            &args.p2,
            &args.p3,
            &args.p4,
            &args.uv1,
            &args.uv2,
            &args.uv3,
            &args.uv4,
            args.col,
        );
    }
    extern fn zguiDrawList_AddImageQuad(
        draw_list: DrawList,
        user_texture_id: TextureIdent,
        p1: *const [2]f32,
        p2: *const [2]f32,
        p3: *const [2]f32,
        p4: *const [2]f32,
        uv1: *const [2]f32,
        uv2: *const [2]f32,
        uv3: *const [2]f32,
        uv4: *const [2]f32,
        col: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const AddImageRounded = struct {
        pmin: [2]f32,
        pmax: [2]f32,
        uvmin: [2]f32 = .{ 0, 0 },
        uvmax: [2]f32 = .{ 1, 1 },
        col: u32 = 0xff_ff_ff_ff,
        rounding: f32 = 4.0,
        flags: DrawFlags = .{},
    };
    pub fn addImageRounded(draw_list: DrawList, user_texture_id: TextureIdent, args: AddImageRounded) void {
        zguiDrawList_AddImageRounded(
            draw_list,
            user_texture_id,
            &args.pmin,
            &args.pmax,
            &args.uvmin,
            &args.uvmax,
            args.col,
            args.rounding,
            args.flags,
        );
    }
    extern fn zguiDrawList_AddImageRounded(
        draw_list: DrawList,
        user_texture_id: TextureIdent,
        pmin: *const [2]f32,
        pmax: *const [2]f32,
        uvmin: *const [2]f32,
        uvmax: *const [2]f32,
        col: u32,
        rounding: f32,
        flags: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    pub const pathClear = zguiDrawList_PathClear;
    extern fn zguiDrawList_PathClear(draw_list: DrawList) void;
    //----------------------------------------------------------------------------------------------
    pub fn pathLineTo(draw_list: DrawList, pos: [2]f32) void {
        zguiDrawList_PathLineTo(draw_list, &pos);
    }
    extern fn zguiDrawList_PathLineTo(draw_list: DrawList, pos: *const [2]f32) void;
    //----------------------------------------------------------------------------------------------
    pub fn pathLineToMergeDuplicate(draw_list: DrawList, pos: [2]f32) void {
        zguiDrawList_PathLineToMergeDuplicate(draw_list, &pos);
    }
    extern fn zguiDrawList_PathLineToMergeDuplicate(draw_list: DrawList, pos: *const [2]f32) void;
    //----------------------------------------------------------------------------------------------
    pub const pathFillConvex = zguiDrawList_PathFillConvex;
    extern fn zguiDrawList_PathFillConvex(draw_list: DrawList, col: u32) void;
    //----------------------------------------------------------------------------------------------
    const PathStroke = struct {
        col: u32,
        flags: DrawFlags = .{},
        thickness: f32 = 1.0,
    };
    pub fn pathStroke(draw_list: DrawList, args: PathStroke) void {
        zguiDrawList_PathStroke(draw_list, args.col, args.flags, args.thickness);
    }
    extern fn zguiDrawList_PathStroke(draw_list: DrawList, col: u32, flags: DrawFlags, thickness: f32) void;
    //----------------------------------------------------------------------------------------------
    const PathArcTo = struct {
        p: [2]f32,
        r: f32,
        amin: f32,
        amax: f32,
        num_segments: u32 = 0,
    };
    pub fn pathArcTo(draw_list: DrawList, args: PathArcTo) void {
        zguiDrawList_PathArcTo(
            draw_list,
            &args.p,
            args.r,
            args.amin,
            args.amax,
            args.num_segments,
        );
    }
    extern fn zguiDrawList_PathArcTo(
        draw_list: DrawList,
        center: *const [2]f32,
        radius: f32,
        amin: f32,
        amax: f32,
        num_segments: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const PathArcToFast = struct {
        p: [2]f32,
        r: f32,
        amin_of_12: f32,
        amax_of_12: f32,
    };
    pub fn pathArcToFast(draw_list: DrawList, args: PathArcToFast) void {
        zguiDrawList_PathArcToFast(draw_list, &args.p, args.r, args.amin_of_12, args.amax_of_12);
    }
    extern fn zguiDrawList_PathArcToFast(
        draw_list: DrawList,
        center: *const [2]f32,
        radius: f32,
        a_min_of_12: f32,
        a_max_of_12: f32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const PathBezierCubicCurveTo = struct {
        p2: [2]f32,
        p3: [2]f32,
        p4: [2]f32,
        num_segments: u32 = 0,
    };
    pub fn pathBezierCubicCurveTo(draw_list: DrawList, args: PathBezierCubicCurveTo) void {
        zguiDrawList_PathBezierCubicCurveTo(draw_list, &args.p2, &args.p3, &args.p4, args.num_segments);
    }
    extern fn zguiDrawList_PathBezierCubicCurveTo(
        draw_list: DrawList,
        p2: *const [2]f32,
        p3: *const [2]f32,
        p4: *const [2]f32,
        num_segments: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const PathBezierQuadraticCurveTo = struct {
        p2: [2]f32,
        p3: [2]f32,
        num_segments: u32 = 0,
    };
    pub fn pathPathBezierQuadraticCurveTo(draw_list: DrawList, args: PathBezierQuadraticCurveTo) void {
        zguiDrawList_PathBezierQuadraticCurveTo(draw_list, &args.p2, &args.p3, args.num_segments);
    }
    extern fn zguiDrawList_PathBezierQuadraticCurveTo(
        draw_list: DrawList,
        p2: *const [2]f32,
        p3: *const [2]f32,
        num_segments: u32,
    ) void;
    //----------------------------------------------------------------------------------------------
    const PathRect = struct {
        bmin: [2]f32,
        bmax: [2]f32,
        rounding: f32 = 0.0,
        flags: DrawFlags = .{},
    };
    pub fn pathRect(draw_list: DrawList, args: PathRect) void {
        zguiDrawList_PathRect(draw_list, &args.bmin, &args.bmax, args.rounding, args.flags);
    }
    extern fn zguiDrawList_PathRect(
        draw_list: DrawList,
        rect_min: *const [2]f32,
        rect_max: *const [2]f32,
        rounding: f32,
        flags: DrawFlags,
    ) void;
    //----------------------------------------------------------------------------------------------
};
//--------------------------------------------------------------------------------------------------
//
// ImPlot
//
//--------------------------------------------------------------------------------------------------
pub const plot = struct {
    //----------------------------------------------------------------------------------------------
    pub fn init() void {
        if (zguiPlot_GetCurrentContext() == null) {
            _ = zguiPlot_CreateContext(null);
        }
    }
    pub fn deinit() void {
        if (zguiPlot_GetCurrentContext() != null) {
            zguiPlot_DestroyContext(null);
        }
    }
    extern fn zguiPlot_GetCurrentContext() ?Context;
    extern fn zguiPlot_CreateContext(shared_font_atlas: ?*const anyopaque) Context;
    extern fn zguiPlot_DestroyContext(ctx: ?Context) void;
    //----------------------------------------------------------------------------------------------
    pub const PlotLocation = packed struct(u32) {
        north: bool = false,
        south: bool = false,
        west: bool = false,
        east: bool = false,
        _padding: u28 = 0,

        pub const north_west = PlotLocation{ .north = true, .west = true };
        pub const north_east = PlotLocation{ .north = true, .east = true };
        pub const south_west = PlotLocation{ .south = true, .west = true };
        pub const south_east = PlotLocation{ .south = true, .east = true };
    };
    pub const LegendFlags = packed struct(u32) {
        no_buttons: bool = false,
        no_highlight_item: bool = false,
        no_highlight_axis: bool = false,
        no_menus: bool = false,
        outside: bool = false,
        horizontal: bool = false,
        _padding: u26 = 0,
    };
    pub fn setupLegend(location: PlotLocation, flags: LegendFlags) void {
        zguiPlot_SetupLegend(location, flags);
    }
    extern fn zguiPlot_SetupLegend(location: PlotLocation, flags: LegendFlags) void;
    //----------------------------------------------------------------------------------------------
    pub const AxisFlags = packed struct(u32) {
        no_label: bool = false,
        no_grid_lines: bool = false,
        no_tick_marks: bool = false,
        no_tick_labels: bool = false,
        no_initial_fit: bool = false,
        no_menus: bool = false,
        no_side_switch: bool = false,
        no_highlight: bool = false,
        opposite: bool = false,
        foreground: bool = false,
        invert: bool = false,
        auto_fit: bool = false,
        range_fit: bool = false,
        pan_stretch: bool = false,
        lock_min: bool = false,
        lock_max: bool = false,
        _padding: u16 = 0,

        pub const lock = AxisFlags{
            .lock_min = true,
            .lock_max = true,
        };
        pub const no_decorations = AxisFlags{
            .no_label = true,
            .no_grid_lines = true,
            .no_tick_marks = true,
            .no_tick_labels = true,
        };
        pub const aux_default = AxisFlags{
            .no_grid_lines = true,
            .opposite = true,
        };
    };
    pub fn setupXAxis(label: [:0]const u8, flags: AxisFlags) void {
        zguiPlot_SetupXAxis(label, flags);
    }
    pub fn setupYAxis(label: [:0]const u8, flags: AxisFlags) void {
        zguiPlot_SetupYAxis(label, flags);
    }
    extern fn zguiPlot_SetupXAxis(label: [*:0]const u8, flags: AxisFlags) void;
    extern fn zguiPlot_SetupYAxis(label: [*:0]const u8, flags: AxisFlags) void;
    //----------------------------------------------------------------------------------------------
    pub const Flags = packed struct(u32) {
        no_title: bool = false,
        no_legend: bool = false,
        no_mouse_text: bool = false,
        no_inputs: bool = false,
        no_menus: bool = false,
        no_box_select: bool = false,
        no_child: bool = false,
        no_frame: bool = false,
        equal: bool = false,
        crosshairs: bool = false,
        _padding: u22 = 0,

        pub const canvas_only = Flags{
            .no_title = true,
            .no_legend = true,
            .no_menus = true,
            .no_box_select = true,
            .no_mouse_text = true,
        };
    };
    pub const BeginPlot = struct {
        w: f32 = -1.0,
        h: f32 = 0.0,
        flags: Flags = .{},
    };
    pub fn beginPlot(title_id: [:0]const u8, args: BeginPlot) bool {
        return zguiPlot_BeginPlot(title_id, args.w, args.h, args.flags);
    }
    extern fn zguiPlot_BeginPlot(title_id: [*:0]const u8, width: f32, height: f32, flags: Flags) bool;
    //----------------------------------------------------------------------------------------------
    pub const LineFlags = packed struct(u32) {
        _reserved0: bool = false,
        _reserved1: bool = false,
        _reserved2: bool = false,
        _reserved3: bool = false,
        _reserved4: bool = false,
        _reserved5: bool = false,
        _reserved6: bool = false,
        _reserved7: bool = false,
        _reserved8: bool = false,
        _reserved9: bool = false,
        segments: bool = false,
        loop: bool = false,
        skip_nan: bool = false,
        no_clip: bool = false,
        shaded: bool = false,
        _padding: u17 = 0,
    };
    pub fn plotLineValuesInt(label: [:0]const u8, values: []const i32, flags: LineFlags) void {
        zguiPlot_PlotLineValues(label, values.ptr, @intCast(i32, values.len), flags);
    }
    extern fn zguiPlot_PlotLineValues(
        label_id: [*:0]const u8,
        values: [*]const i32,
        count: i32,
        flags: LineFlags,
    ) void;
    //----------------------------------------------------------------------------------------------
    pub const endPlot = zguiPlot_EndPlot;
    extern fn zguiPlot_EndPlot() void;
    //----------------------------------------------------------------------------------------------
};
