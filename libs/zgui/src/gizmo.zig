const gui = @import("gui.zig");
const DrawList = gui.DrawList;
const Context = gui.Context;

pub const Operation = packed struct(u32) {
    translate_x: bool = false,
    translate_y: bool = false,
    translate_z: bool = false,
    rotate_x: bool = false,
    rotate_y: bool = false,
    rotate_z: bool = false,
    rotate_screen: bool = false,
    scale_x: bool = false,
    scale_y: bool = false,
    scale_z: bool = false,
    bounds: bool = false,
    scale_xu: bool = false,
    scale_yu: bool = false,
    scale_zu: bool = false,

    pub fn translate() Operation { return .{ .translate_x = true, .translate_y = true, .translate_z = true }; }
    pub fn rotate() Operation { return .{ .rotate_x = true, .rotate_y = true, .rotate_z = true }; }
    pub fn scale() Operation { return .{ .scale_x = true, .scale_y = true, .scale_z = true }; }
    pub fn scaleU() Operation { return .{ .scale_xu = true, .scale_yu = true, .scale_zu = true }; }
    pub fn universal() Operation { return translate() | rotate() | scaleU(); }
};

pub const Mode = enum {
    local,
    world,
};

pub const Color = enum {
    direction_x,
    direction_y,
    direction_z,
    plane_x,
    plane_y,
    plane_z,
    selection,
    inactive,
    translation_line,
    scale_line,
    rotation_using_border,
    rotation_using_fill,
    hatched_axis_lines,
    text,
    text_shadow,
};

pub const Style = struct {
    translation_line_thickness: f32,
    translation_line_arrow_size: f32,
    rotation_line_thickness: f32,
    rotation_outer_line_thickness: f32,
    scale_line_thickness: f32,
    scale_line_circle_size: f32,
    hatched_axis_line_thickness: f32,
    center_circle_size: f32,
    colors: [@typeInfo(Color).Enum.fields.len][4]f32,
};

extern fn setDrawlist(draw_list: *DrawList) void;
extern fn beginFrame() void;
extern fn setImGuiContext(ctx: *Context) void;
extern fn isOver() bool;
extern fn isUsing() bool;
extern fn isUsingAny() bool;
extern fn enable(enable: bool) void;
extern fn decomposeMatrixToComponents(matrix: *const f32, translation: *f32, rotation: *f32, scale: *f32) void;
extern fn recomposeMatrixFromComponents(
    translation: *const f32,
    rotation: *const f32,
    scale: *const f32,
    matrix: *f32,
) void;
extern fn setRect(x: f32, y: f32, width: f32, height: f32) void;
extern fn setOrthographic(isOrthographic: bool) void;
extern fn drawCubes(view: *const f32, projection: *const f32, matrices: *const f32, matrixCount: i32) void;
extern fn drawGrid(view: *const f32, projection: *const f32, matrix: *const f32, gridSize: f32) void;
extern fn manipulate(
    view: *const f32,
    projection: *const f32,
    operation: Operation,
    mode: Mode,
    matrix: *f32,
    delta_matrix: ?*f32,
    snap: ?*const f32,
    local_bounds: ?*const f32,
    boundsSnap: ?*const f32,
) bool;
extern fn viewManipulate(
    view: *f32,
    length: f32,
    position: *const [2]f32,
    size: *const [2]f32,
    background_color: u32
) void;
extern fn viewManipulateIndependent(
    view: *f32,
    projection: *const f32,
    operation: Operation,
    mode: Mode,
    matrix: *f32,
    length: f32,
    position: *const [2]f32,
    size: *const [2]f32,
    background_color: u32
) void;
extern fn setID(id: i32) void;
extern fn isOverOperation(op: Operation) bool;
extern fn allowAxisFlip(value: bool) void;
extern fn setAxisLimit(value: f32) void;
extern fn setPlaneLimit(value: f32) void;
extern fn getStyle() *Style;