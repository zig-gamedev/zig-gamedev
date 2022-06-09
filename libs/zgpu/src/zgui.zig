const std = @import("std");
const assert = std.debug.assert;

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

    _pad0: u12 = 0,

    pub const no_nav = WindowFlags{ .no_nav_inputs = true, .no_nav_focus = true };
    pub const no_decoration = WindowFlags{
        .no_title_bar = true,
        .no_resize = true,
        .no_scrollbar = true,
        .no_collapse = true,
    };
    pub const no_inputs = WindowFlags{ .no_mouse_inputs = true, .no_nav_inputs = true, .no_nav_focus = true };

    comptime {
        std.debug.assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
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

    _pad0: u8 = 0,
    _pad1: u16 = 0,

    comptime {
        std.debug.assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

pub fn button(label: [:0]const u8, size: struct { w: f32 = 0.0, h: f32 = 0.0 }) bool {
    return zguiButton(label.ptr, size.w, size.h);
}

pub fn begin(name: [:0]const u8, p_open: ?*bool, flags: WindowFlags) bool {
    return zguiBegin(name.ptr, p_open, @bitCast(u32, flags));
}

pub fn sameLine(args: struct { offset_from_start_x: f32 = 0.0, spacing: f32 = -1.0 }) void {
    zguiSameLine(args.offset_from_start_x, args.spacing);
}

pub fn comboStr(
    label: [:0]const u8,
    current_item: *i32,
    items_separated_by_zeros: [:0]const u8,
    popup_max_height_in_items: i32,
) bool {
    return zguiComboStr(label.ptr, current_item, items_separated_by_zeros.ptr, popup_max_height_in_items);
}

pub fn sliderFloat(
    label: [:0]const u8,
    v: *f32,
    v_min: f32,
    v_max: f32,
    args: struct {
        format: ?[:0]const u8 = null,
        flags: SliderFlags = .{},
    },
) bool {
    return zguiSliderFloat(
        label.ptr,
        v,
        v_min,
        v_max,
        if (args.format) |fmt| fmt.ptr else null,
        @bitCast(u32, args.flags),
    );
}

pub fn bulletText(comptime fmt: []const u8, args: anytype) void {
    // TODO: Max. text length is hardcoded, make it more robust.
    var buf: [512]u8 = undefined;
    const result = std.fmt.bufPrintZ(buf[0..], fmt, args) catch blk: {
        buf[buf.len - 1] = '\x00';
        break :blk buf[0 .. buf.len - 1 :0];
    };
    zguiBulletText("%s", result.ptr);
}

pub const end = zguiEnd;
pub const spacing = zguiSpacing;
pub const newLine = zguiNewLine;
pub const separator = zguiSeparator;
pub const dummy = zguiDummy;

//
// Raw C functions.
//
extern fn zguiButton(label: [*:0]const u8, w: f32, h: f32) bool;
extern fn zguiBegin(name: [*:0]const u8, p_open: ?*bool, flags: u32) bool;
extern fn zguiEnd() void;
extern fn zguiSpacing() void;
extern fn zguiNewLine() void;
extern fn zguiSeparator() void;
extern fn zguiSameLine(offset_from_start_x: f32, spacing: f32) void;
extern fn zguiDummy(w: f32, h: f32) void;
extern fn zguiComboStr(
    label: [*:0]const u8,
    current_item: *i32,
    items_separated_by_zeros: [*:0]const u8,
    popup_max_height_in_items: i32,
) bool;
extern fn zguiSliderFloat(
    label: [*:0]const u8,
    v: *f32,
    v_min: f32,
    v_max: f32,
    format: ?[*:0]const u8,
    flags: u32,
) bool;
extern fn zguiBulletText(fmt: [*:0]const u8, ...) void;
