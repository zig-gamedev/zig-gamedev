//--------------------------------------------------------------------------------------------------
const assert = @import("std").debug.assert;
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
pub const Marker = enum(i32) {
    none = -1,
    circle = 0,
    square,
    diamond,
    up,
    down,
    left,
    right,
    cross,
    plus,
    asterisk,
};

pub const Colormap = enum(u32) {
    deep,
    dark,
    pastel,
    paired,
    viridis,
    plasma,
    hot,
    cool,
    pink,
    jet,
    twilight,
    rd_bu,
    br_b_g,
    pi_y_g,
    spectral,
    greys,
};

pub const Style = extern struct {
    line_weight: f32,
    marker: Marker,
    marker_size: f32,
    marker_weight: f32,
    fill_alpha: f32,
    error_bar_size: f32,
    error_bar_weight: f32,
    digital_bit_height: f32,
    digital_bit_gap: f32,
    plot_border_size: f32,
    minor_alpha: f32,
    major_tick_len: [2]f32,
    minor_tick_len: [2]f32,
    major_tick_size: [2]f32,
    minor_tick_size: [2]f32,
    major_grid_size: [2]f32,
    minor_grid_size: [2]f32,
    plot_padding: [2]f32,
    label_padding: [2]f32,
    legend_padding: [2]f32,
    legend_inner_padding: [2]f32,
    legend_spacing: [2]f32,
    mouse_pos_padding: [2]f32,
    annotation_padding: [2]f32,
    fit_padding: [2]f32,
    plot_default_size: [2]f32,
    plot_min_size: [2]f32,

    colors: [@typeInfo(StyleCol).Enum.fields.len][4]f32,
    colormap: Colormap,

    use_local_time: bool,
    use_iso_8601: bool,
    use_24h_clock: bool,

    /// `pub fn init() Style`
    pub const init = zguiPlotStyle_Init;
    extern fn zguiPlotStyle_Init() Style;

    pub fn getColor(style: Style, idx: StyleCol) [4]f32 {
        return style.colors[@intFromEnum(idx)];
    }
    pub fn setColor(style: *Style, idx: StyleCol, color: [4]f32) void {
        style.colors[@intFromEnum(idx)] = color;
    }
};
/// `pub fn getStyle() *Style`
pub const getStyle = zguiPlot_GetStyle;
extern fn zguiPlot_GetStyle() *Style;
//--------------------------------------------------------------------------------------------------
pub const StyleCol = enum(u32) {
    line,
    fill,
    marker_outline,
    marker_fill,
    error_bar,
    frame_bg,
    plot_bg,
    plot_border,
    legend_bg,
    legend_border,
    legend_text,
    title_text,
    inlay_text,
    axis_text,
    axis_grid,
    axis_tick,
    axis_bg,
    axis_bg_hovered,
    axis_bg_active,
    selection,
    crosshairs,
};
const PushStyleColor4f = struct {
    idx: StyleCol,
    c: [4]f32,
};
pub fn pushStyleColor4f(args: PushStyleColor4f) void {
    zguiPlot_PushStyleColor4f(args.idx, &args.c);
}
const PushStyleColor1u = struct {
    idx: StyleCol,
    c: u32,
};
pub fn pushStyleColor1u(args: PushStyleColor1u) void {
    zguiPlot_PushStyleColor1u(args.idx, args.c);
}
const PopStyleColor = struct {
    count: i32 = 1,
};
pub fn popStyleColor(args: PopStyleColor) void {
    zguiPlot_PopStyleColor(args.count);
}
extern fn zguiPlot_PushStyleColor4f(idx: StyleCol, col: *const [4]f32) void;
extern fn zguiPlot_PushStyleColor1u(idx: StyleCol, col: u32) void;
extern fn zguiPlot_PopStyleColor(count: i32) void;
//--------------------------------------------------------------------------------------------------
pub const StyleVar = enum(u32) {
    line_weight, // 1f
    marker, // 1i
    marker_size, // 1f
    marker_weight, // 1f
    fill_alpha, // 1f
    error_bar_size, // 1f
    error_bar_weight, // 1f
    digital_bit_height, // 1f
    digital_bit_gap, // 1f
    plot_border_size, // 1f
    minor_alpha, // 1f
    major_tick_len, // 2f
    minor_tick_len, // 2f
    major_tick_size, // 2f
    minor_tick_size, // 2f
    major_grid_size, // 2f
    minor_grid_size, // 2f
    plot_padding, // 2f
    label_padding, // 2f
    legend_padding, // 2f
    legend_inner_padding, // 2f
    legend_spacing, // 2f
    mouse_pos_padding, // 2f
    annotation_padding, // 2f
    fit_padding, // 2f
    plot_default_size, // 2f
    plot_min_size, // 2f
};
const PushStyleVar1i = struct {
    idx: StyleVar,
    v: i32,
};
pub fn pushStyleVar1i(args: PushStyleVar1i) void {
    zguiPlot_PushStyleVar1i(args.idx, args.v);
}
const PushStyleVar1f = struct {
    idx: StyleVar,
    v: f32,
};
pub fn pushStyleVar1f(args: PushStyleVar1f) void {
    zguiPlot_PushStyleVar1f(args.idx, args.v);
}
const PushStyleVar2f = struct {
    idx: StyleVar,
    v: [2]f32,
};
pub fn pushStyleVar2f(args: PushStyleVar2f) void {
    zguiPlot_PushStyleVar2f(args.idx, &args.v);
}
const PopStyleVar = struct {
    count: i32 = 1,
};
pub fn popStyleVar(args: PopStyleVar) void {
    zguiPlot_PopStyleVar(args.count);
}
extern fn zguiPlot_PushStyleVar1i(idx: StyleVar, v: i32) void;
extern fn zguiPlot_PushStyleVar1f(idx: StyleVar, v: f32) void;
extern fn zguiPlot_PushStyleVar2f(idx: StyleVar, v: *const [2]f32) void;
extern fn zguiPlot_PopStyleVar(count: i32) void;
//--------------------------------------------------------------------------------------------------
pub fn getLastItemColor() [4]f32 {
    var color: [4]f32 = undefined;
    zguiPlot_GetLastItemColor(&color);
    return color;
}
extern fn zguiPlot_GetLastItemColor(color: *[4]f32) void;
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
    none = @intFromEnum(gui.Condition.none),
    always = @intFromEnum(gui.Condition.always),
    once = @intFromEnum(gui.Condition.once),
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
    no_frame: bool = false,
    equal: bool = false,
    crosshairs: bool = false,
    _padding: u23 = 0,

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
        @as(i32, @intCast(args.v.len)),
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
        @as(i32, @intCast(args.xv.len)),
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
        @as(i32, @intCast(args.v.len)),
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
        @as(i32, @intCast(args.xv.len)),
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

pub const ShadedFlags = packed struct(u32) {
    _padding: u32 = 0,
};
fn PlotShadedGen(comptime T: type) type {
    return struct {
        xv: []const T,
        yv: []const T,
        yref: f64 = 0.0,
        flags: ShadedFlags = .{},
        offset: i32 = 0,
        stride: i32 = @sizeOf(T),
    };
}
pub fn plotShaded(label_id: [:0]const u8, comptime T: type, args: PlotShadedGen(T)) void {
    assert(args.xv.len == args.yv.len);
    zguiPlot_PlotShaded(
        label_id,
        gui.typeToDataTypeEnum(T),
        args.xv.ptr,
        args.yv.ptr,
        @as(i32, @intCast(args.xv.len)),
        args.yref,
        args.flags,
        args.offset,
        args.stride,
    );
}
extern fn zguiPlot_PlotShaded(
    label_id: [*:0]const u8,
    data_type: gui.DataType,
    xv: *const anyopaque,
    yv: *const anyopaque,
    count: i32,
    yref: f64,
    flags: ShadedFlags,
    offset: i32,
    stride: i32,
) void;
//----------------------------------------------------------------------------------------------
pub const BarsFlags = packed struct(u32) {
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
    horizontal: bool = false,
    _padding: u21 = 0,
};
fn PlotBarsGen(comptime T: type) type {
    return struct {
        xv: []const T,
        yv: []const T,
        bar_size: f64 = 0.67,
        flags: BarsFlags = .{},
        offset: i32 = 0,
        stride: i32 = @sizeOf(T),
    };
}
pub fn plotBars(label_id: [:0]const u8, comptime T: type, args: PlotBarsGen(T)) void {
    assert(args.xv.len == args.yv.len);
    zguiPlot_PlotBars(
        label_id,
        gui.typeToDataTypeEnum(T),
        args.xv.ptr,
        args.yv.ptr,
        @as(i32, @intCast(args.xv.len)),
        args.bar_size,
        args.flags,
        args.offset,
        args.stride,
    );
}
extern fn zguiPlot_PlotBars(
    label_id: [*:0]const u8,
    data_type: gui.DataType,
    xv: *const anyopaque,
    yv: *const anyopaque,
    count: i32,
    bar_size: f64,
    flags: BarsFlags,
    offset: i32,
    stride: i32,
) void;

fn PlotBarsValuesGen(comptime T: type) type {
    return struct {
        v: []const T,
        bar_size: f64 = 0.0,
        shift: f64 = 0.0,
        flags: BarsFlags = .{},
        offset: i32 = 0,
        stride: i32 = @sizeOf(T),
    };
}
pub fn plotBarsValues(label_id: [:0]const u8, comptime T: type, args: PlotBarsValuesGen(T)) void {
    assert(args.xv.len == args.yv.len);
    zguiPlot_PlotBars(
        label_id,
        gui.typeToDataTypeEnum(T),
        args.v.ptr,
        @as(i32, @intCast(args.xv.len)),
        args.bar_size,
        args.shift,
        args.flags,
        args.offset,
        args.stride,
    );
}
extern fn zguiPlot_PlotBarsValues(
    label_id: [*:0]const u8,
    data_type: gui.DataType,
    values: *const anyopaque,
    count: i32,
    bar_size: f64,
    shift: f64,
    flags: BarsFlags,
    offset: i32,
    stride: i32,
) void;
//----------------------------------------------------------------------------------------------
pub const DragToolFlags = packed struct(u32) {
    no_cursors: bool = false,
    no_fit: bool = false,
    no_no_inputs: bool = false,
    delayed: bool = false,
    _padding: u28 = 0,
};
const DragPoint = struct {
    x: *f64,
    y: *f64,
    col: [4]f32,
    size: f32 = 4,
    flags: DragToolFlags = .{},
};
pub fn dragPoint(id: i32, args: DragPoint) bool {
    return zguiPlot_DragPoint(
        id,
        args.x,
        args.y,
        &args.col,
        args.size,
        args.flags,
    );
}
extern fn zguiPlot_DragPoint(id: i32, x: *f64, y: *f64, *const [4]f32, size: f32, flags: DragToolFlags) bool;
//----------------------------------------------------------------------------------------------
// PlotText
const PlotTextFlags = packed struct(u32) {
    vertical: bool = false,
    _padding: u31 = 0,
};
const PlotText = struct {
    x: f64,
    y: f64,
    pix_offset: [2]f32 = .{ 0, 0 },
    flags: PlotTextFlags = .{},
};
pub fn plotText(text: [*:0]const u8, args: PlotText) void {
    zguiPlot_PlotText(text, args.x, args.y, &args.pix_offset, args.flags);
}
extern fn zguiPlot_PlotText(
    text: [*:0]const u8,
    x: f64,
    y: f64,
    pix_offset: *const [2]f32,
    flags: PlotTextFlags,
) void;

//----------------------------------------------------------------------------------------------
pub fn isPlotHovered() bool {
    return zguiPlot_IsPlotHovered();
}
extern fn zguiPlot_IsPlotHovered() bool;
//----------------------------------------------------------------------------------------------
/// `pub fn showDemoWindow(popen: ?*bool) void`
pub const showDemoWindow = zguiPlot_ShowDemoWindow;
extern fn zguiPlot_ShowDemoWindow(popen: ?*bool) void;
//----------------------------------------------------------------------------------------------
/// `pub fn endPlot() void`
pub const endPlot = zguiPlot_EndPlot;
extern fn zguiPlot_EndPlot() void;
//----------------------------------------------------------------------------------------------
