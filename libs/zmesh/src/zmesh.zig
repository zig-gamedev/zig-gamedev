// zmesh - Zig bindings for par_shapes

const std = @import("std");
const expect = std.testing.expect;
const Mutex = std.Thread.Mutex;

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
    ) catch @panic("zmesh: out of memory");

    allocations.?.put(@ptrToInt(slice.ptr), size) catch
        @panic("zmesh: out of memory");

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
        allocations.?.get(@ptrToInt(ptr.?)).?
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
    ) catch @panic("zmesh: out of memory");

    if (ptr != null) {
        const removed = allocations.?.remove(@ptrToInt(ptr.?));
        std.debug.assert(removed);
    }

    allocations.?.put(@ptrToInt(slice.ptr), size) catch
        @panic("zmesh: out of memory");

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
    allocations.?.ensureTotalCapacity(32) catch unreachable;
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
    indices: []IndexType,
    positions: [][3]f32,
    normals: ?[][3]f32,
    texcoords: ?[][2]f32,
    handle: MeshHandle,

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

    pub fn clone(mesh: Mesh) Mesh {
        return initMesh(par_shapes_clone(mesh.handle, null));
    }
    extern fn par_shapes_clone(mesh: MeshHandle, target: ?MeshHandle) MeshHandle;

    pub fn merge(mesh: *Mesh, src_mesh: Mesh) void {
        par_shapes_merge(mesh.handle, src_mesh.handle);
        mesh.* = initMesh(mesh.handle);
    }
    extern fn par_shapes_merge(mesh: MeshHandle, src_mesh: MeshHandle) void;

    pub fn translate(mesh: *Mesh, x: f32, y: f32, z: f32) void {
        par_shapes_translate(mesh.handle, x, y, z);
        mesh.* = initMesh(mesh.handle);
    }
    extern fn par_shapes_translate(mesh: MeshHandle, x: f32, y: f32, z: f32) void;

    pub fn rotate(mesh: *Mesh, radians: f32, x: f32, y: f32, z: f32) void {
        par_shapes_rotate(mesh.handle, radians, &.{ x, y, z });
        mesh.* = initMesh(mesh.handle);
    }
    extern fn par_shapes_rotate(
        mesh: MeshHandle,
        radians: f32,
        axis: *const [3]f32,
    ) void;

    pub fn scale(mesh: *Mesh, x: f32, y: f32, z: f32) void {
        par_shapes_scale(mesh.handle, x, y, z);
        mesh.* = initMesh(mesh.handle);
    }
    extern fn par_shapes_scale(mesh: MeshHandle, x: f32, y: f32, z: f32) void;

    pub fn invert(mesh: *Mesh, start_face: i32, num_faces: i32) void {
        par_shapes_invert(mesh.handle, start_face, num_faces);
        mesh.* = initMesh(mesh.handle);
    }
    extern fn par_shapes_invert(
        mesh: MeshHandle,
        start_face: i32,
        num_faces: i32,
    ) void;

    pub fn removeDegenerate(mesh: *Mesh, min_area: f32) void {
        par_shapes_remove_degenerate(mesh.handle, min_area);
        mesh.* = initMesh(mesh.handle);
    }
    extern fn par_shapes_remove_degenerate(mesh: MeshHandle, min_area: f32) void;

    pub fn unweld(mesh: *Mesh) void {
        par_shapes_unweld(mesh.handle, true);
        mesh.* = initMesh(mesh.handle);
    }
    extern fn par_shapes_unweld(mesh: MeshHandle, create_indices: bool) void;

    pub fn weld(mesh: *Mesh, epsilon: f32, mapping: ?[*]IndexType) void {
        const new_mesh = par_shapes_weld(mesh.handle, epsilon, mapping);
        par_shapes_free_mesh(mesh.handle);
        mesh.* = initMesh(new_mesh);
    }
    extern fn par_shapes_weld(
        mesh: MeshHandle,
        epsilon: f32,
        mapping: ?[*]IndexType,
    ) MeshHandle;

    pub fn computeNormals(mesh: *Mesh) void {
        par_shapes_compute_normals(mesh.handle);
        mesh.* = initMesh(mesh.handle);
    }
    extern fn par_shapes_compute_normals(mesh: MeshHandle) void;
};

fn initMesh(handle: MeshHandle) Mesh {
    const parmesh = @ptrCast(
        *ParMesh,
        @alignCast(@alignOf(ParMesh), handle),
    );
    return .{
        .handle = handle,
        .positions = @ptrCast(
            [*][3]f32,
            parmesh.points,
        )[0..@intCast(usize, parmesh.npoints)],
        .indices = parmesh.triangles[0..@intCast(usize, parmesh.ntriangles * 3)],
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

pub fn initCylinder(slices: i32, stacks: i32) Mesh {
    return initMesh(par_shapes_create_cylinder(slices, stacks));
}
extern fn par_shapes_create_cylinder(slices: i32, stacks: i32) MeshHandle;

pub fn initCone(slices: i32, stacks: i32) Mesh {
    return initMesh(par_shapes_create_cone(slices, stacks));
}
extern fn par_shapes_create_cone(slices: i32, stacks: i32) MeshHandle;

pub fn initParametricDisk(slices: i32, stacks: i32) Mesh {
    return initMesh(par_shapes_create_parametric_disk(slices, stacks));
}
extern fn par_shapes_create_parametric_disk(slices: i32, stacks: i32) MeshHandle;

pub fn initTorus(slices: i32, stacks: i32, radius: f32) Mesh {
    return initMesh(par_shapes_create_torus(slices, stacks, radius));
}
extern fn par_shapes_create_torus(slices: i32, stacks: i32, radius: f32) MeshHandle;

pub fn initParametricSphere(slices: i32, stacks: i32) Mesh {
    return initMesh(par_shapes_create_parametric_sphere(slices, stacks));
}
extern fn par_shapes_create_parametric_sphere(slices: i32, stacks: i32) MeshHandle;

pub fn initSubdividedSphere(num_subdivisions: i32) Mesh {
    return initMesh(par_shapes_create_subdivided_sphere(num_subdivisions));
}
extern fn par_shapes_create_subdivided_sphere(num_subdivisions: i32) MeshHandle;

pub fn initTrefoilKnot(slices: i32, stacks: i32, radius: f32) Mesh {
    return initMesh(par_shapes_create_trefoil_knot(slices, stacks, radius));
}
extern fn par_shapes_create_trefoil_knot(
    slices: i32,
    stacks: i32,
    radius: f32,
) MeshHandle;

pub fn initHemisphere(slices: i32, stacks: i32) Mesh {
    return initMesh(par_shapes_create_hemisphere(slices, stacks));
}
extern fn par_shapes_create_hemisphere(slices: i32, stacks: i32) MeshHandle;

pub fn initPlane(slices: i32, stacks: i32) Mesh {
    return initMesh(par_shapes_create_plane(slices, stacks));
}
extern fn par_shapes_create_plane(slices: i32, stacks: i32) MeshHandle;

pub fn initIcosahedron() Mesh {
    return initMesh(par_shapes_create_icosahedron());
}
extern fn par_shapes_create_icosahedron() MeshHandle;

pub fn initDodecahedron() Mesh {
    return initMesh(par_shapes_create_dodecahedron());
}
extern fn par_shapes_create_dodecahedron() MeshHandle;

pub fn initOctahedron() Mesh {
    return initMesh(par_shapes_create_octahedron());
}
extern fn par_shapes_create_octahedron() MeshHandle;

pub fn initTetrahedron() Mesh {
    return initMesh(par_shapes_create_tetrahedron());
}
extern fn par_shapes_create_tetrahedron() MeshHandle;

pub fn initCube() Mesh {
    return initMesh(par_shapes_create_cube());
}
extern fn par_shapes_create_cube() MeshHandle;

pub fn initDisk(
    radius: f32,
    slices: i32,
    center: *const [3]f32,
    normal: *const [3]f32,
) Mesh {
    return initMesh(par_shapes_create_disk(radius, slices, center, normal));
}
extern fn par_shapes_create_disk(
    radius: f32,
    slices: i32,
    center: *const [3]f32,
    normal: *const [3]f32,
) MeshHandle;

pub fn initRock(seed: i32, num_subdivisions: i32) Mesh {
    return initMesh(par_shapes_create_rock(seed, num_subdivisions));
}
extern fn par_shapes_create_rock(seed: i32, num_subdivisions: i32) MeshHandle;

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
) Mesh {
    return initMesh(par_shapes_create_parametric(fun, slices, stacks, userdata));
}
extern fn par_shapes_create_parametric(
    fun: UvToPositionFn,
    slices: i32,
    stacks: i32,
    userdata: ?*anyopaque,
) MeshHandle;

const save = false;

test "zmesh.basic" {
    init(std.testing.allocator);
    defer deinit();

    const cylinder = initCylinder(10, 10);
    defer cylinder.deinit();
    if (save) cylinder.saveToObj("zmesh.cylinder.obj");

    const cone = initCone(10, 10);
    defer cone.deinit();
    if (save) cone.saveToObj("zmesh.cone.obj");

    const pdisk = initParametricDisk(10, 10);
    defer pdisk.deinit();
    if (save) pdisk.saveToObj("zmesh.pdisk.obj");

    const torus = initTorus(10, 10, 0.2);
    defer torus.deinit();
    if (save) torus.saveToObj("zmesh.torus.obj");

    const psphere = initParametricSphere(10, 10);
    defer psphere.deinit();
    if (save) psphere.saveToObj("zmesh.psphere.obj");

    const subdsphere = initSubdividedSphere(3);
    defer subdsphere.deinit();
    if (save) subdsphere.saveToObj("zmesh.subdsphere.obj");

    const trefoil_knot = initTrefoilKnot(10, 100, 0.6);
    defer trefoil_knot.deinit();
    if (save) trefoil_knot.saveToObj("zmesh.trefoil_knot.obj");

    const hemisphere = initHemisphere(10, 10);
    defer hemisphere.deinit();
    if (save) hemisphere.saveToObj("zmesh.hemisphere.obj");

    const plane = initPlane(10, 10);
    defer plane.deinit();
    if (save) plane.saveToObj("zmesh.plane.obj");

    const icosahedron = initIcosahedron();
    defer icosahedron.deinit();
    if (save) icosahedron.saveToObj("zmesh.icosahedron.obj");

    const dodecahedron = initDodecahedron();
    defer dodecahedron.deinit();
    if (save) dodecahedron.saveToObj("zmesh.dodecahedron.obj");

    const octahedron = initOctahedron();
    defer octahedron.deinit();
    if (save) octahedron.saveToObj("zmesh.octahedron.obj");

    const tetrahedron = initTetrahedron();
    defer tetrahedron.deinit();
    if (save) tetrahedron.saveToObj("zmesh.tetrahedron.obj");

    var cube = initCube();
    defer cube.deinit();
    cube.unweld();
    cube.computeNormals();
    if (save) cube.saveToObj("zmesh.cube.obj");

    const rock = initRock(1337, 3);
    defer rock.deinit();
    if (save) rock.saveToObj("zmesh.rock.obj");

    const disk = initDisk(3.0, 10, &.{ 1, 2, 3 }, &.{ 0, 1, 0 });
    defer disk.deinit();
    if (save) disk.saveToObj("zmesh.disk.obj");
}

test "zmesh.clone" {
    init(std.testing.allocator);
    defer deinit();

    const cube = initCube();
    defer cube.deinit();

    var clone0 = cube.clone();
    defer clone0.deinit();

    try expect(@ptrToInt(clone0.handle) != @ptrToInt(cube.handle));
}

test "zmesh.merge" {
    init(std.testing.allocator);
    defer deinit();

    var cube = initCube();
    defer cube.deinit();

    var sphere = initSubdividedSphere(3);
    defer sphere.deinit();

    cube.translate(0, 2, 0);
    sphere.merge(cube);
    cube.translate(0, 2, 0);
    sphere.merge(cube);

    if (save) sphere.saveToObj("zmesh.merge.obj");
}

test "zmesh.invert" {
    init(std.testing.allocator);
    defer deinit();

    var hemisphere = initParametricSphere(10, 10);
    defer hemisphere.deinit();
    hemisphere.invert(0, 0);

    hemisphere.removeDegenerate(0.001);
    hemisphere.unweld();
    hemisphere.weld(0.001, null);

    if (save) hemisphere.saveToObj("zmesh.invert.obj");
}
