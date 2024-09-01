const builtin = @import("builtin");
const std = @import("std");
const assert = std.debug.assert;
const zmeshMalloc = @import("memory.zig").zmeshMalloc;

pub const IndexType: type = blk: {
    if (!builtin.is_test) {
        const options = @import("zmesh_options");
        if (@hasDecl(options, "shape_use_32bit_indices")) {
            if (options.shape_use_32bit_indices)
                break :blk u32;
            break :blk u16;
        }
        break :blk u16;
    } else break :blk u16;
};

pub const ShapeHandle = *opaque {};
pub const Shape = @This();

indices: []IndexType,
positions: [][3]f32,
normals: ?[][3]f32,
texcoords: ?[][2]f32,
handle: ShapeHandle,

pub fn init(
    indices: std.ArrayList(IndexType),
    positions: std.ArrayList([3]f32),
    maybe_normals: ?std.ArrayList([3]f32),
    maybe_texcoords: ?std.ArrayList([2]f32),
) Shape {
    const handle = par_shapes_create_empty();
    const parmesh = @as(
        *ParShape,
        @ptrCast(@alignCast(handle)),
    );

    parmesh.triangles = @as(
        [*]IndexType,
        @ptrCast(@alignCast(zmeshMalloc(indices.items.len * @sizeOf(IndexType)))),
    );
    parmesh.ntriangles = @as(c_int, @intCast(@divExact(indices.items.len, 3)));
    @memcpy(parmesh.triangles[0..indices.items.len], indices.items);

    parmesh.points = @as(
        [*]f32,
        @ptrCast(@alignCast(zmeshMalloc(positions.items.len * @sizeOf(f32) * 3))),
    );
    parmesh.npoints = @as(c_int, @intCast(positions.items.len));
    @memcpy(
        parmesh.points[0 .. positions.items.len * 3],
        @as([*]f32, @ptrCast(positions.items.ptr))[0 .. positions.items.len * 3],
    );

    if (maybe_normals) |normals| {
        assert(normals.items.len == positions.items.len);

        parmesh.normals = @as(
            [*]f32,
            @ptrCast(@alignCast(zmeshMalloc(normals.items.len * @sizeOf(f32) * 3))),
        );
        @memcpy(
            parmesh.normals.?[0 .. normals.items.len * 3],
            @as([*]f32, @ptrCast(normals.items.ptr))[0 .. normals.items.len * 3],
        );
    }

    if (maybe_texcoords) |texcoords| {
        assert(texcoords.items.len == positions.items.len);

        parmesh.tcoords = @as(
            [*]f32,
            @ptrCast(@alignCast(zmeshMalloc(texcoords.items.len * @sizeOf(f32) * 2))),
        );
        @memcpy(
            parmesh.tcoords.?[0 .. texcoords.items.len * 2],
            @as([*]f32, @ptrCast(texcoords.items.ptr))[0 .. texcoords.items.len * 2],
        );
    }

    return initShape(handle);
}

pub fn deinit(mesh: Shape) void {
    par_shapes_free_mesh(mesh.handle);
}

pub fn saveToObj(mesh: Shape, filename: [:0]const u8) void {
    par_shapes_export(mesh.handle, filename);
}

pub fn computeAabb(mesh: Shape) [6]f32 {
    var aabb: [6]f32 = undefined;
    par_shapes_compute_aabb(mesh.handle, &aabb);
    return aabb;
}

pub fn clone(mesh: Shape) Shape {
    return initShape(par_shapes_clone(mesh.handle, null));
}

pub fn merge(mesh: *Shape, src_mesh: Shape) void {
    par_shapes_merge(mesh.handle, src_mesh.handle);
    mesh.* = initShape(mesh.handle);
}

pub fn translate(mesh: *Shape, x: f32, y: f32, z: f32) void {
    par_shapes_translate(mesh.handle, x, y, z);
    mesh.* = initShape(mesh.handle);
}

pub fn rotate(mesh: *Shape, radians: f32, x: f32, y: f32, z: f32) void {
    par_shapes_rotate(mesh.handle, radians, &.{ x, y, z });
    mesh.* = initShape(mesh.handle);
}

pub fn scale(mesh: *Shape, x: f32, y: f32, z: f32) void {
    par_shapes_scale(mesh.handle, x, y, z);
    mesh.* = initShape(mesh.handle);
}

pub fn invert(mesh: *Shape, start_face: i32, num_faces: i32) void {
    par_shapes_invert(mesh.handle, start_face, num_faces);
    mesh.* = initShape(mesh.handle);
}

pub fn removeDegenerate(mesh: *Shape, min_area: f32) void {
    par_shapes_remove_degenerate(mesh.handle, min_area);
    mesh.* = initShape(mesh.handle);
}

pub fn unweld(mesh: *Shape) void {
    par_shapes_unweld(mesh.handle, true);
    mesh.* = initShape(mesh.handle);
}

pub fn weld(mesh: *Shape, epsilon: f32, mapping: ?[*]IndexType) void {
    const new_mesh = par_shapes_weld(mesh.handle, epsilon, mapping);
    par_shapes_free_mesh(mesh.handle);
    mesh.* = initShape(new_mesh);
}

pub fn computeNormals(mesh: *Shape) void {
    par_shapes_compute_normals(mesh.handle);
    mesh.* = initShape(mesh.handle);
}

fn initShape(handle: ShapeHandle) Shape {
    const parmesh = @as(
        *ParShape,
        @ptrCast(@alignCast(handle)),
    );
    return .{
        .handle = handle,
        .positions = @as(
            [*][3]f32,
            @ptrCast(parmesh.points),
        )[0..@as(usize, @intCast(parmesh.npoints))],
        .indices = parmesh.triangles[0..@as(usize, @intCast(parmesh.ntriangles * 3))],
        .normals = if (parmesh.normals == null)
            null
        else
            @as(
                [*][3]f32,
                @ptrCast(parmesh.normals.?),
            )[0..@as(usize, @intCast(parmesh.npoints))],
        .texcoords = if (parmesh.tcoords == null)
            null
        else
            @as(
                [*][2]f32,
                @ptrCast(parmesh.tcoords.?),
            )[0..@as(usize, @intCast(parmesh.npoints))],
    };
}

pub fn initCylinder(slices: i32, stacks: i32) Shape {
    return initShape(par_shapes_create_cylinder(slices, stacks));
}

pub fn initCone(slices: i32, stacks: i32) Shape {
    return initShape(par_shapes_create_cone(slices, stacks));
}

pub fn initParametricDisk(slices: i32, stacks: i32) Shape {
    return initShape(par_shapes_create_parametric_disk(slices, stacks));
}

pub fn initTorus(slices: i32, stacks: i32, radius: f32) Shape {
    return initShape(par_shapes_create_torus(slices, stacks, radius));
}

pub fn initParametricSphere(slices: i32, stacks: i32) Shape {
    return initShape(par_shapes_create_parametric_sphere(slices, stacks));
}

pub fn initSubdividedSphere(num_subdivisions: i32) Shape {
    return initShape(par_shapes_create_subdivided_sphere(num_subdivisions));
}

pub fn initTrefoilKnot(slices: i32, stacks: i32, radius: f32) Shape {
    return initShape(par_shapes_create_trefoil_knot(slices, stacks, radius));
}

pub fn initHemisphere(slices: i32, stacks: i32) Shape {
    return initShape(par_shapes_create_hemisphere(slices, stacks));
}

pub fn initPlane(slices: i32, stacks: i32) Shape {
    return initShape(par_shapes_create_plane(slices, stacks));
}

pub fn initIcosahedron() Shape {
    return initShape(par_shapes_create_icosahedron());
}

pub fn initDodecahedron() Shape {
    return initShape(par_shapes_create_dodecahedron());
}

pub fn initOctahedron() Shape {
    return initShape(par_shapes_create_octahedron());
}

pub fn initTetrahedron() Shape {
    return initShape(par_shapes_create_tetrahedron());
}

pub fn initCube() Shape {
    return initShape(par_shapes_create_cube());
}

pub fn initDisk(
    radius: f32,
    slices: i32,
    center: *const [3]f32,
    normal: *const [3]f32,
) Shape {
    return initShape(par_shapes_create_disk(radius, slices, center, normal));
}

pub fn initRock(seed: i32, num_subdivisions: i32) Shape {
    return initShape(par_shapes_create_rock(seed, num_subdivisions));
}

pub const UvToPositionFn = *const fn (
    uv: *const [2]f32,
    position: *[3]f32,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub fn initParametric(
    fun: UvToPositionFn,
    slices: i32,
    stacks: i32,
    userdata: ?*anyopaque,
) Shape {
    return initShape(par_shapes_create_parametric(fun, slices, stacks, userdata));
}

const ParShape = extern struct {
    points: [*]f32,
    npoints: c_int,
    triangles: [*]IndexType,
    ntriangles: c_int,
    normals: ?[*]f32,
    tcoords: ?[*]f32,
};

extern fn par_shapes_free_mesh(mesh: ShapeHandle) void;
extern fn par_shapes_export(mesh: ShapeHandle, filename: [*:0]const u8) void;
extern fn par_shapes_compute_aabb(mesh: ShapeHandle, aabb: *[6]f32) void;
extern fn par_shapes_clone(mesh: ShapeHandle, target: ?ShapeHandle) ShapeHandle;
extern fn par_shapes_merge(mesh: ShapeHandle, src_mesh: ShapeHandle) void;
extern fn par_shapes_translate(mesh: ShapeHandle, x: f32, y: f32, z: f32) void;
extern fn par_shapes_rotate(
    mesh: ShapeHandle,
    radians: f32,
    axis: *const [3]f32,
) void;
extern fn par_shapes_scale(mesh: ShapeHandle, x: f32, y: f32, z: f32) void;
extern fn par_shapes_invert(
    mesh: ShapeHandle,
    start_face: i32,
    num_faces: i32,
) void;
extern fn par_shapes_remove_degenerate(mesh: ShapeHandle, min_area: f32) void;
extern fn par_shapes_unweld(mesh: ShapeHandle, create_indices: bool) void;
extern fn par_shapes_weld(
    mesh: ShapeHandle,
    epsilon: f32,
    mapping: ?[*]IndexType,
) ShapeHandle;
extern fn par_shapes_compute_normals(mesh: ShapeHandle) void;
extern fn par_shapes_create_cylinder(slices: i32, stacks: i32) ShapeHandle;
extern fn par_shapes_create_cone(slices: i32, stacks: i32) ShapeHandle;
extern fn par_shapes_create_parametric_disk(slices: i32, stacks: i32) ShapeHandle;
extern fn par_shapes_create_torus(slices: i32, stacks: i32, radius: f32) ShapeHandle;
extern fn par_shapes_create_parametric_sphere(slices: i32, stacks: i32) ShapeHandle;
extern fn par_shapes_create_subdivided_sphere(num_subdivisions: i32) ShapeHandle;
extern fn par_shapes_create_trefoil_knot(
    slices: i32,
    stacks: i32,
    radius: f32,
) ShapeHandle;
extern fn par_shapes_create_hemisphere(slices: i32, stacks: i32) ShapeHandle;
extern fn par_shapes_create_plane(slices: i32, stacks: i32) ShapeHandle;
extern fn par_shapes_create_icosahedron() ShapeHandle;
extern fn par_shapes_create_dodecahedron() ShapeHandle;
extern fn par_shapes_create_octahedron() ShapeHandle;
extern fn par_shapes_create_tetrahedron() ShapeHandle;
extern fn par_shapes_create_cube() ShapeHandle;
extern fn par_shapes_create_disk(
    radius: f32,
    slices: i32,
    center: *const [3]f32,
    normal: *const [3]f32,
) ShapeHandle;
extern fn par_shapes_create_rock(seed: i32, num_subdivisions: i32) ShapeHandle;
extern fn par_shapes_create_parametric(
    fun: UvToPositionFn,
    slices: i32,
    stacks: i32,
    userdata: ?*anyopaque,
) ShapeHandle;
extern fn par_shapes_create_empty() ShapeHandle;

const test_enable_write_to_disk = false;
const expect = std.testing.expect;

test "zmesh.basic" {
    const zmesh = @import("root.zig");

    zmesh.init(std.testing.allocator);
    defer zmesh.deinit();

    const cylinder = Shape.initCylinder(10, 10);
    defer cylinder.deinit();
    if (test_enable_write_to_disk) cylinder.saveToObj("zmesh.cylinder.obj");

    const cone = Shape.initCone(10, 10);
    defer cone.deinit();
    if (test_enable_write_to_disk) cone.saveToObj("zmesh.cone.obj");

    const pdisk = Shape.initParametricDisk(10, 10);
    defer pdisk.deinit();
    if (test_enable_write_to_disk) pdisk.saveToObj("zmesh.pdisk.obj");

    const torus = Shape.initTorus(10, 10, 0.2);
    defer torus.deinit();
    if (test_enable_write_to_disk) torus.saveToObj("zmesh.torus.obj");

    const psphere = Shape.initParametricSphere(10, 10);
    defer psphere.deinit();
    if (test_enable_write_to_disk) psphere.saveToObj("zmesh.psphere.obj");

    const subdsphere = Shape.initSubdividedSphere(3);
    defer subdsphere.deinit();
    if (test_enable_write_to_disk) subdsphere.saveToObj("zmesh.subdsphere.obj");

    const trefoil_knot = Shape.initTrefoilKnot(10, 100, 0.6);
    defer trefoil_knot.deinit();
    if (test_enable_write_to_disk) trefoil_knot.saveToObj("zmesh.trefoil_knot.obj");

    const hemisphere = Shape.initHemisphere(10, 10);
    defer hemisphere.deinit();
    if (test_enable_write_to_disk) hemisphere.saveToObj("zmesh.hemisphere.obj");
    _ = hemisphere.computeAabb();

    const plane = Shape.initPlane(10, 10);
    defer plane.deinit();
    if (test_enable_write_to_disk) plane.saveToObj("zmesh.plane.obj");

    const icosahedron = Shape.initIcosahedron();
    defer icosahedron.deinit();
    if (test_enable_write_to_disk) icosahedron.saveToObj("zmesh.icosahedron.obj");

    const dodecahedron = Shape.initDodecahedron();
    defer dodecahedron.deinit();
    if (test_enable_write_to_disk) dodecahedron.saveToObj("zmesh.dodecahedron.obj");

    const octahedron = Shape.initOctahedron();
    defer octahedron.deinit();
    if (test_enable_write_to_disk) octahedron.saveToObj("zmesh.octahedron.obj");

    const tetrahedron = Shape.initTetrahedron();
    defer tetrahedron.deinit();
    if (test_enable_write_to_disk) tetrahedron.saveToObj("zmesh.tetrahedron.obj");

    var cube = Shape.initCube();
    defer cube.deinit();
    cube.unweld();
    cube.computeNormals();
    if (test_enable_write_to_disk) cube.saveToObj("zmesh.cube.obj");

    const rock = Shape.initRock(1337, 3);
    defer rock.deinit();
    if (test_enable_write_to_disk) rock.saveToObj("zmesh.rock.obj");

    const disk = Shape.initDisk(3.0, 10, &.{ 1, 2, 3 }, &.{ 0, 1, 0 });
    defer disk.deinit();
    if (test_enable_write_to_disk) disk.saveToObj("zmesh.disk.obj");
}

test "zmesh.clone" {
    const zmesh = @import("root.zig");

    zmesh.init(std.testing.allocator);
    defer zmesh.deinit();

    const cube = Shape.initCube();
    defer cube.deinit();

    var clone0 = cube.clone();
    defer clone0.deinit();

    try expect(@intFromPtr(clone0.handle) != @intFromPtr(cube.handle));
}

test "zmesh.merge" {
    const zmesh = @import("root.zig");

    zmesh.init(std.testing.allocator);
    defer zmesh.deinit();

    var cube = Shape.initCube();
    defer cube.deinit();

    var sphere = Shape.initSubdividedSphere(3);
    defer sphere.deinit();

    cube.translate(0, 2, 0);
    sphere.merge(cube);
    cube.translate(0, 2, 0);
    sphere.merge(cube);

    if (test_enable_write_to_disk) sphere.saveToObj("zmesh.merge.obj");
}

test "zmesh.invert" {
    const zmesh = @import("root.zig");

    zmesh.init(std.testing.allocator);
    defer zmesh.deinit();

    var hemisphere = Shape.initParametricSphere(10, 10);
    defer hemisphere.deinit();
    hemisphere.invert(0, 0);

    hemisphere.removeDegenerate(0.001);
    hemisphere.unweld();
    hemisphere.weld(0.001, null);

    if (test_enable_write_to_disk) hemisphere.saveToObj("zmesh.invert.obj");
}

test "zmesh.custom" {
    const zmesh = @import("root.zig");

    zmesh.init(std.testing.allocator);
    defer zmesh.deinit();

    var positions = std.ArrayList([3]f32).init(std.testing.allocator);
    defer positions.deinit();
    try positions.append(.{ 0.0, 0.0, 0.0 });
    try positions.append(.{ 1.0, 0.0, 0.0 });
    try positions.append(.{ 1.0, 0.0, 1.0 });

    var indices = std.ArrayList(IndexType).init(std.testing.allocator);
    defer indices.deinit();
    try indices.append(0);
    try indices.append(1);
    try indices.append(2);

    var shape = Shape.init(indices, positions, null, null);
    defer shape.deinit();

    if (test_enable_write_to_disk) shape.saveToObj("zmesh.custom.obj");
}
