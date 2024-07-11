const gui = @import("gui.zig");
const DrawList = gui.DrawList;

pub const Matrix = [16]f32;
pub const Vector = [3]f32;

/// [-x, -y, -z, x, y, z]
pub const Bounds = [6]f32;

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
    _padding: u18 = 0,

    pub fn translate() Operation {
        return .{ .translate_x = true, .translate_y = true, .translate_z = true };
    }
    pub fn rotate() Operation {
        return .{ .rotate_x = true, .rotate_y = true, .rotate_z = true };
    }
    pub fn scale() Operation {
        return .{ .scale_x = true, .scale_y = true, .scale_z = true };
    }
    pub fn scaleU() Operation {
        return .{ .scale_xu = true, .scale_yu = true, .scale_zu = true };
    }
    pub fn universal() Operation {
        return .{
            .translate_x = true,
            .translate_y = true,
            .translate_z = true,
            .rotate_x = true,
            .rotate_y = true,
            .rotate_z = true,
            .scale_xu = true,
            .scale_yu = true,
            .scale_zu = true,
        };
    }
};

pub const Mode = enum(u32) {
    local,
    world,
};

pub const Color = enum(u32) {
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

pub const Style = extern struct {
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

//---------------------------------------------------------------------------------------------------------------------|

pub fn setDrawList(draw_list: ?DrawList) void {
    zguiGizmo_SetDrawlist(draw_list);
}

pub fn beginFrame() void {
    zguiGizmo_BeginFrame();
}

pub fn setImGuiContext(ctx: *anyopaque) void {
    zguiGizmo_SetImGuiContext(ctx);
}

pub fn isOver() bool {
    return zguiGizmo_IsOver();
}

pub fn isUsing() bool {
    return zguiGizmo_IsUsing();
}

pub fn isUsingAny() bool {
    return zguiGizmo_IsUsingAny();
}

pub fn setEnabled(enable: bool) void {
    zguiGizmo_Enable(enable);
}

pub fn decomposeMatrixToComponents(
    matrix: *const Matrix,
    translation: *Vector,
    rotation: *Vector,
    scale: *Vector,
) void {
    zguiGizmo_DecomposeMatrixToComponents(&matrix[0], &translation[0], &rotation[0], &scale[0]);
}

pub fn recomposeMatrixFromComponents(
    translation: *const Vector,
    rotation: *const Vector,
    scale: *const Vector,
    matrix: *Matrix,
) void {
    zguiGizmo_RecomposeMatrixFromComponents(&translation[0], &rotation[0], &scale[0], &matrix[0]);
}

pub fn setRect(x: f32, y: f32, width: f32, height: f32) void {
    zguiGizmo_SetRect(x, y, width, height);
}

pub fn setOrthographic(is_orthographic: bool) void {
    zguiGizmo_SetOrthographic(is_orthographic);
}

pub fn drawCubes(view: *const Matrix, projection: *const Matrix, matrices: []const Matrix) void {
    zguiGizmo_DrawCubes(&view[0], &projection[0], &matrices[0][0], @as(i32, @intCast(matrices.len)));
}

pub fn drawGrid(view: *const Matrix, projection: *const Matrix, matrix: *const Matrix, grid_size: f32) void {
    zguiGizmo_DrawGrid(&view[0], &projection[0], &matrix[0], grid_size);
}

pub fn manipulate(
    view: *const Matrix,
    projection: *const Matrix,
    operation: Operation,
    mode: Mode,
    matrix: *Matrix,
    opt: struct {
        delta_matrix: ?*Matrix = null,
        snap: ?*const Vector = null,
        local_bounds: ?*const Bounds = null,
        bounds_snap: ?*const Vector = null,
    },
) bool {
    return zguiGizmo_Manipulate(
        &view[0],
        &projection[0],
        operation,
        mode,
        &matrix[0],
        if (opt.delta_matrix) |arr| &arr[0] else null,
        if (opt.snap) |arr| &arr[0] else null,
        if (opt.local_bounds) |arr| &arr[0] else null,
        if (opt.bounds_snap) |arr| &arr[0] else null,
    );
}

/// Please note that this cubeview is patented by Autodesk : https://patents.google.com/patent/US7782319B2/en
/// It seems to be a defensive patent in the US. I don't think it will bring troubles using it as
/// other software are using the same mechanics. But just in case, you are now warned!
pub fn viewManipulate(
    view: *Matrix,
    length: f32,
    position: *const [2]f32,
    size: *const [2]f32,
    background_color: u32,
) void {
    zguiGizmo_ViewManipulate(&view[0], length, position, size, background_color);
}

/// Use this version if you did not call `manipulate` before and you are just using `viewManipulate`
pub fn viewManipulateIndependent(
    view: *Matrix,
    projection: *const Matrix,
    operation: Operation,
    mode: Mode,
    matrix: *Matrix,
    length: f32,
    position: *const [2]f32,
    size: *const [2]f32,
    background_color: u32,
) void {
    zguiGizmo_ViewManipulateIndependent(
        &view[0],
        &projection[0],
        operation,
        mode,
        &matrix[0],
        length,
        position,
        size,
        background_color,
    );
}

pub fn setID(id: i32) void {
    zguiGizmo_SetID(id);
}

pub fn isOverOperation(op: Operation) bool {
    return zguiGizmo_IsOverOperation(op);
}

pub fn allowAxisFlip(allowed: bool) void {
    zguiGizmo_AllowAxisFlip(allowed);
}

pub fn setAxisLimit(limit: f32) void {
    zguiGizmo_SetAxisLimit(limit);
}

pub fn setPlaneLimit(limit: f32) void {
    zguiGizmo_SetPlaneLimit(limit);
}

pub fn getStyle() *Style {
    return zguiGizmo_GetStyle();
}

//---------------------------------------------------------------------------------------------------------------------|

extern fn zguiGizmo_SetDrawlist(draw_list: ?DrawList) void;
extern fn zguiGizmo_BeginFrame() void;
extern fn zguiGizmo_SetImGuiContext(ctx: *anyopaque) void;
extern fn zguiGizmo_IsOver() bool;
extern fn zguiGizmo_IsUsing() bool;
extern fn zguiGizmo_IsUsingAny() bool;
extern fn zguiGizmo_Enable(enable: bool) void;
extern fn zguiGizmo_DecomposeMatrixToComponents(
    matrix: *const f32,
    translation: *f32,
    rotation: *f32,
    scale: *f32,
) void;
extern fn zguiGizmo_RecomposeMatrixFromComponents(
    translation: *const f32,
    rotation: *const f32,
    scale: *const f32,
    matrix: *f32,
) void;
extern fn zguiGizmo_SetRect(x: f32, y: f32, width: f32, height: f32) void;
extern fn zguiGizmo_SetOrthographic(is_orthographic: bool) void;
extern fn zguiGizmo_DrawCubes(view: *const f32, projection: *const f32, matrices: *const f32, matrix_count: i32) void;
extern fn zguiGizmo_DrawGrid(view: *const f32, projection: *const f32, matrix: *const f32, grid_size: f32) void;
extern fn zguiGizmo_Manipulate(
    view: *const f32,
    projection: *const f32,
    operation: Operation,
    mode: Mode,
    matrix: *f32,
    delta_matrix: ?*f32,
    snap: ?*const f32,
    local_bounds: ?*const f32,
    bounds_snap: ?*const f32,
) bool;
extern fn zguiGizmo_ViewManipulate(
    view: *f32,
    length: f32,
    position: *const [2]f32,
    size: *const [2]f32,
    background_color: u32,
) void;
extern fn zguiGizmo_ViewManipulateIndependent(
    view: *f32,
    projection: *const f32,
    operation: Operation,
    mode: Mode,
    matrix: *f32,
    length: f32,
    position: *const [2]f32,
    size: *const [2]f32,
    background_color: u32,
) void;
extern fn zguiGizmo_SetID(id: i32) void;
extern fn zguiGizmo_IsOverOperation(op: Operation) bool;
extern fn zguiGizmo_AllowAxisFlip(value: bool) void;
extern fn zguiGizmo_SetAxisLimit(value: f32) void;
extern fn zguiGizmo_SetPlaneLimit(value: f32) void;
extern fn zguiGizmo_GetStyle() *Style;
