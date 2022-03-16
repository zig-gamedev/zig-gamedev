// zmesh - Zig bindings for par_shapes

const std = @import("std");
const Mutex = std.Thread.Mutex;

pub const Error = error{OutOfMemory};
pub const IndexType = u16;
pub const MeshHandle = *opaque {};

extern fn zmesh_set_allocator(
    malloc: fn (size: usize) callconv(.C) ?*anyopaque,
    calloc: fn (num: usize, size: usize) callconv(.C) ?*anyopaque,
    realloc: fn (ptr: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque,
    free: fn (ptr: ?*anyopaque) callconv(.C) void,
) void;

var allocator: ?std.mem.Allocator = null;
var allocations: ?std.AutoHashMap(usize, usize) = null;
var mutex: Mutex = .{};

export fn mallocFunc(size: usize) callconv(.C) ?*anyopaque {
    mutex.lock();
    defer mutex.unlock();

    var slice = allocator.?.allocBytes(
        @sizeOf(usize),
        size,
        0,
        @returnAddress(),
    ) catch return null;
    allocations.?.put(@ptrToInt(slice.ptr), size) catch unreachable;
    return slice.ptr;
}

export fn callocFunc(num: usize, size: usize) callconv(.C) ?*anyopaque {
    const ptr = mallocFunc(num * size);
    if (ptr != null) {
        @memset(@ptrCast([*]u8, ptr), 0, num * size);
        return ptr;
    }
    return null;
}

export fn reallocFunc(ptr: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque {
    mutex.lock();
    defer mutex.unlock();

    const old_len = if (ptr != null)
        allocations.?.fetchRemove(@ptrToInt(ptr.?)).?.value
    else
        0;

    var old_mem = if (old_len > 0)
        @ptrCast([*]u8, ptr)[0..old_len]
    else
        @as([*]u8, undefined)[0..0];

    var slice = allocator.?.reallocBytes(
        old_mem,
        @sizeOf(usize),
        size,
        @sizeOf(usize),
        0,
        @returnAddress(),
    ) catch return null;
    allocations.?.put(@ptrToInt(slice.ptr), size) catch unreachable;
    return slice.ptr;
}

export fn freeFunc(ptr: ?*anyopaque) callconv(.C) void {
    if (ptr != null) {
        mutex.lock();
        defer mutex.unlock();

        const size = allocations.?.fetchRemove(@ptrToInt(ptr.?)).?.value;
        const slice = @ptrCast([*]u8, ptr.?)[0..size];
        allocator.?.free(slice);
    }
}

pub fn init(alloc: std.mem.Allocator) void {
    std.debug.assert(allocator == null and allocations == null);
    allocator = alloc;
    allocations = std.AutoHashMap(usize, usize).init(allocator.?);
    allocations.?.ensureTotalCapacity(256) catch unreachable;
    zmesh_set_allocator(mallocFunc, callocFunc, reallocFunc, freeFunc);
}

pub fn deinit() void {
    allocations.?.deinit();
    allocations = null;
    allocator = null;
}

const ParMesh = extern struct {
    points: [*]f32,
    npoints: c_int,
    triangles: [*]IndexType,
    ntriangles: c_int,
    normals: ?[*]f32,
    tcoords: ?[*]f32,
};

pub const Mesh = struct {
    handle: MeshHandle,
    positions: [][3]f32,
    triangles: [][3]IndexType,
    normals: ?[][3]f32,
    texcoords: ?[][2]f32,

    pub fn deinit(mesh: Mesh) void {
        par_shapes_free_mesh(mesh.handle);
    }
    extern fn par_shapes_free_mesh(mesh: MeshHandle) void;

    pub fn saveToObj(mesh: Mesh, filename: [*:0]const u8) void {
        par_shapes_export(mesh.handle, filename);
    }
    extern fn par_shapes_export(mesh: MeshHandle, filename: [*:0]const u8) void;

    pub fn computeAabb(mesh: Mesh, aabb: *[6]f32) void {
        par_shapes_compute_aabb(mesh.handle, aabb);
    }
    extern fn par_shapes_compute_aabb(mesh: MeshHandle, aabb: *[6]f32) void;

    pub fn clone(mesh: Mesh, target: ?*Mesh) Error!Mesh {
        const parmesh = par_shapes_clone(
            mesh.handle,
            if (target != null) target.handle else null,
        );
        if (parmesh == null)
            return error.OutOfMemory;
        return parMeshToMesh(parmesh.?);
    }
    extern fn par_shapes_clone(mesh: MeshHandle, target: MeshHandle) ?*ParMesh;
};

fn parMeshToMesh(parmesh: *ParMesh) Mesh {
    return .{
        .handle = @ptrCast(MeshHandle, parmesh),
        .positions = @ptrCast(
            [*][3]f32,
            parmesh.points,
        )[0..@intCast(usize, parmesh.npoints)],
        .triangles = @ptrCast(
            [*][3]IndexType,
            parmesh.triangles,
        )[0..@intCast(usize, parmesh.ntriangles)],
        .normals = if (parmesh.normals == null)
            null
        else
            @ptrCast(
                [*][3]f32,
                parmesh.normals.?,
            )[0..@intCast(usize, parmesh.npoints)],
        .texcoords = if (parmesh.tcoords == null)
            null
        else
            @ptrCast(
                [*][2]f32,
                parmesh.tcoords.?,
            )[0..@intCast(usize, parmesh.npoints)],
    };
}

pub fn initCylinder(slices: i32, stacks: i32) Error!Mesh {
    const parmesh = par_shapes_create_cylinder(slices, stacks);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_cylinder(slices: i32, stacks: i32) ?*ParMesh;

pub fn initCone(slices: i32, stacks: i32) Error!Mesh {
    const parmesh = par_shapes_create_cone(slices, stacks);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_cone(slices: i32, stacks: i32) ?*ParMesh;

pub fn initParametricDisk(slices: i32, stacks: i32) Error!Mesh {
    const parmesh = par_shapes_create_parametric_disk(slices, stacks);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_parametric_disk(slices: i32, stacks: i32) ?*ParMesh;

pub fn initTorus(slices: i32, stacks: i32, radius: f32) Error!Mesh {
    const parmesh = par_shapes_create_torus(slices, stacks, radius);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_torus(slices: i32, stacks: i32, radius: f32) ?*ParMesh;

pub fn initParametricSphere(slices: i32, stacks: i32) Error!Mesh {
    const parmesh = par_shapes_create_parametric_sphere(slices, stacks);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_parametric_sphere(slices: i32, stacks: i32) ?*ParMesh;

pub fn initSubdividedSphere(num_subdivisions: i32) Error!Mesh {
    const parmesh = par_shapes_create_subdivided_sphere(num_subdivisions);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_subdivided_sphere(num_subdivisions: i32) ?*ParMesh;

pub fn initKleinBottle(slices: i32, stacks: i31) Error!Mesh {
    const parmesh = par_shapes_create_klein_bottle(slices, stacks);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_klein_bottle(slices: i32, stacks: i32) ?*ParMesh;

pub fn initTrefoilKnot(slices: i32, stacks: i32, radius: f32) Error!Mesh {
    const parmesh = par_shapes_create_trefoil_knot(slices, stacks, radius);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_trefoil_knot(
    slices: i32,
    stacks: i32,
    radius: f32,
) ?*ParMesh;

pub fn initHemisphere(slices: i32, stacks: i32) Error!Mesh {
    const parmesh = par_shapes_create_hemisphere(slices, stacks);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_hemisphere(slices: i32, stacks: i32) ?*ParMesh;

pub fn initPlane(slices: i32, stacks: i32) Error!Mesh {
    const parmesh = par_shapes_create_plane(slices, stacks);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_plane(slices: i32, stacks: i32) ?*ParMesh;

pub fn initIcosahedron() Error!Mesh {
    const parmesh = par_shapes_create_icosahedron();
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_icosahedron() ?*ParMesh;

pub fn initDodecahedron() Error!Mesh {
    const parmesh = par_shapes_create_dodecahedron();
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_dodecahedron() ?*ParMesh;

pub fn initOctahedron() Error!Mesh {
    const parmesh = par_shapes_create_octahedron();
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_octahedron() ?*ParMesh;

pub fn initTetrahedron() Error!Mesh {
    const parmesh = par_shapes_create_tetrahedron();
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_tetrahedron() ?*ParMesh;

pub fn initCube() Error!Mesh {
    const parmesh = par_shapes_create_cube();
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_cube() ?*ParMesh;

pub fn initEmpty() Error!Mesh {
    const parmesh = par_shapes_create_empty();
    if (parmesh == null)
        return error.OutOfMemory;
    return Mesh{
        .handle = @ptrCast(MeshHandle, parmesh),
        .positions = undefined,
        .triangles = undefined,
        .normals = null,
        .texcoords = null,
    };
}
extern fn par_shapes_create_empty() ?*ParMesh;

pub fn initDisk(
    radius: f32,
    slices: i32,
    center: *const [3]f32,
    normal: *const [3]f32,
) Error!Mesh {
    const parmesh = par_shapes_create_disk(radius, slices, center, normal);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_disk(
    radius: f32,
    slices: i32,
    center: *const [3]f32,
    normal: *const [3]f32,
) ?*ParMesh;

pub fn initRock(seed: i32, num_subdivisions: i32) Error!Mesh {
    const parmesh = par_shapes_create_rock(seed, num_subdivisions);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_rock(seed: i32, num_subdivisions: i32) ?*ParMesh;

pub fn initLSystem(program: [*:0]const u8, slices: i32, maxdepth: i32) Error!Mesh {
    const parmesh = par_shapes_create_lsystem(program, slices, maxdepth);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_lsystem(
    program: [*:0]const u8,
    slices: i32,
    maxdepth: i32,
) ?*ParMesh;

pub const UvToPositionFn = fn (
    uv: *const [2]f32,
    position: *[3]f32,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub fn initParametric(
    fun: UvToPositionFn,
    slices: i32,
    stacks: i32,
    userdata: ?*anyopaque,
) Error!Mesh {
    const parmesh = par_shapes_create_parametric(fun, slices, stacks, userdata);
    if (parmesh == null)
        return error.OutOfMemory;
    return parMeshToMesh(parmesh.?);
}
extern fn par_shapes_create_parametric(
    fun: UvToPositionFn,
    slices: i32,
    stacks: i32,
    userdata: ?*anyopaque,
) ?*ParMesh;

test "zmesh.basic" {
    init(std.testing.allocator);
    defer deinit();
    const save = true;

    const cylinder = try initCylinder(10, 10);
    defer cylinder.deinit();
    if (save) cylinder.saveToObj("zmesh.cylinder.obj");

    const cone = try initCone(10, 10);
    defer cone.deinit();
    if (save) cone.saveToObj("zmesh.cone.obj");

    const pdisk = try initParametricDisk(10, 10);
    defer pdisk.deinit();
    if (save) pdisk.saveToObj("zmesh.pdisk.obj");

    const torus = try initTorus(10, 10, 0.2);
    defer torus.deinit();
    if (save) torus.saveToObj("zmesh.torus.obj");

    const psphere = try initParametricSphere(10, 10);
    defer psphere.deinit();
    if (save) psphere.saveToObj("zmesh.psphere.obj");

    const subdsphere = try initSubdividedSphere(3);
    defer subdsphere.deinit();
    if (save) subdsphere.saveToObj("zmesh.subdsphere.obj");

    const klein_bottle = try initKleinBottle(10, 60);
    defer klein_bottle.deinit();
    if (save) klein_bottle.saveToObj("zmesh.klein_bottle.obj");

    const trefoil_knot = try initTrefoilKnot(10, 100, 0.6);
    defer trefoil_knot.deinit();
    if (save) trefoil_knot.saveToObj("zmesh.trefoil_knot.obj");

    const hemisphere = try initHemisphere(10, 10);
    defer hemisphere.deinit();
    if (save) hemisphere.saveToObj("zmesh.hemisphere.obj");

    const plane = try initPlane(10, 10);
    defer plane.deinit();
    if (save) plane.saveToObj("zmesh.plane.obj");

    const icosahedron = try initIcosahedron();
    defer icosahedron.deinit();
    if (save) icosahedron.saveToObj("zmesh.icosahedron.obj");

    const dodecahedron = try initDodecahedron();
    defer dodecahedron.deinit();
    if (save) dodecahedron.saveToObj("zmesh.dodecahedron.obj");

    const octahedron = try initOctahedron();
    defer octahedron.deinit();
    if (save) octahedron.saveToObj("zmesh.octahedron.obj");

    const tetrahedron = try initTetrahedron();
    defer tetrahedron.deinit();
    if (save) tetrahedron.saveToObj("zmesh.tetrahedron.obj");

    const cube = try initCube();
    defer cube.deinit();
    if (save) cube.saveToObj("zmesh.cube.obj");

    const empty = try initEmpty();
    defer empty.deinit();

    const rock = try initRock(1337, 3);
    defer rock.deinit();
    if (save) rock.saveToObj("zmesh.rock.obj");

    const disk = try initDisk(3.0, 10, &.{ 1, 2, 3 }, &.{ 0, 1, 0 });
    defer disk.deinit();
    if (save) disk.saveToObj("zmesh.disk.obj");
}

test "zmesh.clone" {
    init(std.testing.allocator);
    defer deinit();
    const cube = try initCube();
    defer cube.deinit();

    //const clone0 = cube.clone(null);
    //defer clone0.deinit();
}
