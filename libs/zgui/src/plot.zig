//--------------------------------------------------------------------------------------------------
const std = @import("std");
const assert = std.debug.assert;
const gui = @import("gui.zig");
//--------------------------------------------------------------------------------------------------
pub fn init() void {
    if (zguiPlot_GetCurrentContext() == null) {
        _ = zguiPlot_CreateContext();
    }
}
pub fn deinit() void {
    if (zguiPlot_GetCurrentContext() != null) {
        zguiPlot_DestroyContext(null);
    }
}
const Context = *opaque {};

extern fn zguiPlot_GetCurrentContext() ?Context;
extern fn zguiPlot_CreateContext() Context;
extern fn zguiPlot_DestroyContext(ctx: ?Context) void;
//--------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------
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
pub const Axis = enum(u32) { x1, x2, x3, y1, y2, y3 };
pub const SetupAxis = struct {
    label: ?[:0]const u8 = null,
    flags: AxisFlags = .{},
};
pub fn setupAxis(axis: Axis, args: SetupAxis) void {
    zguiPlot_SetupAxis(axis, if (args.label) |l| l else null, args.flags);
}
extern fn zguiPlot_SetupAxis(axis: Axis, label: ?[*:0]const u8, flags: AxisFlags) void;
//----------------------------------------------------------------------------------------------
pub const Condition = enum(u32) {
    none = @enumToInt(gui.Condition.none),
    always = @enumToInt(gui.Condition.always),
    once = @enumToInt(gui.Condition.once),
};
const SetupAxisLimits = struct {
    min: f64,
    max: f64,
    cond: Condition = .once,
};
pub fn setupAxisLimits(axis: Axis, args: SetupAxisLimits) void {
    zguiPlot_SetupAxisLimits(axis, args.min, args.max, args.cond);
}
extern fn zguiPlot_SetupAxisLimits(axis: Axis, min: f64, max: f64, cond: Condition) void;
//----------------------------------------------------------------------------------------------
/// `pub fn setupFinish() void`
pub const setupFinish = zguiPlot_SetupFinish;
extern fn zguiPlot_SetupFinish() void;
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
fn PlotLineValuesGen(comptime T: type) type {
    return struct {
        v: []const T,
        xscale: f64 = 1.0,
        xstart: f64 = 0.0,
        flags: LineFlags = .{},
        offset: i32 = 0,
        stride: i32 = @sizeOf(T),
    };
}
pub fn plotLineValues(label_id: [:0]const u8, comptime T: type, args: PlotLineValuesGen(T)) void {
    zguiPlot_PlotLineValues(
        label_id,
        gui.typeToDataTypeEnum(T),
        args.v.ptr,
        @intCast(i32, args.v.len),
        args.xscale,
        args.xstart,
        args.flags,
        args.offset,
        args.stride,
    );
}
extern fn zguiPlot_PlotLineValues(
    label_id: [*:0]const u8,
    data_type: gui.DataType,
    values: *const anyopaque,
    count: i32,
    xscale: f64,
    xstart: f64,
    flags: LineFlags,
    offset: i32,
    stride: i32,
) void;
//----------------------------------------------------------------------------------------------
fn PlotLineGen(comptime T: type) type {
    return struct {
        xv: []const T,
        yv: []const T,
        flags: LineFlags = .{},
        offset: i32 = 0,
        stride: i32 = @sizeOf(T),
    };
}
pub fn plotLine(label_id: [:0]const u8, comptime T: type, args: PlotLineGen(T)) void {
    assert(args.xv.len == args.yv.len);
    zguiPlot_PlotLine(
        label_id,
        gui.typeToDataTypeEnum(T),
        args.xv.ptr,
        args.yv.ptr,
        @intCast(i32, args.xv.len),
        args.flags,
        args.offset,
        args.stride,
    );
}
extern fn zguiPlot_PlotLine(
    label_id: [*:0]const u8,
    data_type: gui.DataType,
    xv: *const anyopaque,
    yv: *const anyopaque,
    count: i32,
    flags: LineFlags,
    offset: i32,
    stride: i32,
) void;
//----------------------------------------------------------------------------------------------
pub const ScatterFlags = packed struct(u32) {
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
    no_clip: bool = false,
    _padding: u21 = 0,
};
fn PlotScatterValuesGen(comptime T: type) type {
    return struct {
        v: []const T,
        xscale: f64 = 1.0,
        xstart: f64 = 0.0,
        flags: ScatterFlags = .{},
        offset: i32 = 0,
        stride: i32 = @sizeOf(T),
    };
}
pub fn plotScatterValues(label_id: [:0]const u8, comptime T: type, args: PlotScatterValuesGen(T)) void {
    zguiPlot_PlotScatterValues(
        label_id,
        gui.typeToDataTypeEnum(T),
        args.v.ptr,
        @intCast(i32, args.v.len),
        args.xscale,
        args.xstart,
        args.flags,
        args.offset,
        args.stride,
    );
}
extern fn zguiPlot_PlotScatterValues(
    label_id: [*:0]const u8,
    data_type: gui.DataType,
    values: *const anyopaque,
    count: i32,
    xscale: f64,
    xstart: f64,
    flags: ScatterFlags,
    offset: i32,
    stride: i32,
) void;
//----------------------------------------------------------------------------------------------
fn PlotScatterGen(comptime T: type) type {
    return struct {
        xv: []const T,
        yv: []const T,
        flags: ScatterFlags = .{},
        offset: i32 = 0,
        stride: i32 = @sizeOf(T),
    };
}
pub fn plotScatter(label_id: [:0]const u8, comptime T: type, args: PlotScatterGen(T)) void {
    assert(args.xv.len == args.yv.len);
    zguiPlot_PlotScatter(
        label_id,
        gui.typeToDataTypeEnum(T),
        args.xv.ptr,
        args.yv.ptr,
        @intCast(i32, args.xv.len),
        args.flags,
        args.offset,
        args.stride,
    );
}
extern fn zguiPlot_PlotScatter(
    label_id: [*:0]const u8,
    data_type: gui.DataType,
    xv: *const anyopaque,
    yv: *const anyopaque,
    count: i32,
    flags: ScatterFlags,
    offset: i32,
    stride: i32,
) void;
//----------------------------------------------------------------------------------------------
pub const endPlot = zguiPlot_EndPlot;
extern fn zguiPlot_EndPlot() void;
//----------------------------------------------------------------------------------------------
