// zmesh v0.2

pub const Shape = @import("Shape.zig");
pub usingnamespace @import("meshoptimizer.zig");

pub fn init(alloc: std.mem.Allocator) void {
    std.debug.assert(allocator == null and allocations == null);
    allocator = alloc;
    allocations = std.AutoHashMap(usize, usize).init(allocator.?);
    allocations.?.ensureTotalCapacity(32) catch unreachable;
    zmesh_setAllocator(mallocFunc, callocFunc, reallocFunc, freeFunc);
    meshopt_setAllocator(mallocFunc, freeFunc);
}

pub fn deinit() void {
    allocations.?.deinit();
    allocations = null;
    allocator = null;
}

const std = @import("std");
const expect = std.testing.expect;
const Mutex = std.Thread.Mutex;

extern fn zmesh_setAllocator(
    malloc: fn (size: usize) callconv(.C) ?*anyopaque,
    calloc: fn (num: usize, size: usize) callconv(.C) ?*anyopaque,
    realloc: fn (ptr: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque,
    free: fn (ptr: ?*anyopaque) callconv(.C) void,
) void;

extern fn meshopt_setAllocator(
    allocate: fn (size: usize) callconv(.C) ?*anyopaque,
    deallocate: fn (ptr: ?*anyopaque) callconv(.C) void,
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

const save = false;

test "zmesh.basic" {
    init(std.testing.allocator);
    defer deinit();

    const cylinder = Shape.initCylinder(10, 10);
    defer cylinder.deinit();
    if (save) cylinder.saveToObj("zmesh.cylinder.obj");

    const cone = Shape.initCone(10, 10);
    defer cone.deinit();
    if (save) cone.saveToObj("zmesh.cone.obj");

    const pdisk = Shape.initParametricDisk(10, 10);
    defer pdisk.deinit();
    if (save) pdisk.saveToObj("zmesh.pdisk.obj");

    const torus = Shape.initTorus(10, 10, 0.2);
    defer torus.deinit();
    if (save) torus.saveToObj("zmesh.torus.obj");

    const psphere = Shape.initParametricSphere(10, 10);
    defer psphere.deinit();
    if (save) psphere.saveToObj("zmesh.psphere.obj");

    const subdsphere = Shape.initSubdividedSphere(3);
    defer subdsphere.deinit();
    if (save) subdsphere.saveToObj("zmesh.subdsphere.obj");

    const trefoil_knot = Shape.initTrefoilKnot(10, 100, 0.6);
    defer trefoil_knot.deinit();
    if (save) trefoil_knot.saveToObj("zmesh.trefoil_knot.obj");

    const hemisphere = Shape.initHemisphere(10, 10);
    defer hemisphere.deinit();
    if (save) hemisphere.saveToObj("zmesh.hemisphere.obj");

    const plane = Shape.initPlane(10, 10);
    defer plane.deinit();
    if (save) plane.saveToObj("zmesh.plane.obj");

    const icosahedron = Shape.initIcosahedron();
    defer icosahedron.deinit();
    if (save) icosahedron.saveToObj("zmesh.icosahedron.obj");

    const dodecahedron = Shape.initDodecahedron();
    defer dodecahedron.deinit();
    if (save) dodecahedron.saveToObj("zmesh.dodecahedron.obj");

    const octahedron = Shape.initOctahedron();
    defer octahedron.deinit();
    if (save) octahedron.saveToObj("zmesh.octahedron.obj");

    const tetrahedron = Shape.initTetrahedron();
    defer tetrahedron.deinit();
    if (save) tetrahedron.saveToObj("zmesh.tetrahedron.obj");

    var cube = Shape.initCube();
    defer cube.deinit();
    cube.unweld();
    cube.computeNormals();
    if (save) cube.saveToObj("zmesh.cube.obj");

    const rock = Shape.initRock(1337, 3);
    defer rock.deinit();
    if (save) rock.saveToObj("zmesh.rock.obj");

    const disk = Shape.initDisk(3.0, 10, &.{ 1, 2, 3 }, &.{ 0, 1, 0 });
    defer disk.deinit();
    if (save) disk.saveToObj("zmesh.disk.obj");
}

test "zmesh.clone" {
    init(std.testing.allocator);
    defer deinit();

    const cube = Shape.initCube();
    defer cube.deinit();

    var clone0 = cube.clone();
    defer clone0.deinit();

    try expect(@ptrToInt(clone0.handle) != @ptrToInt(cube.handle));
}

test "zmesh.merge" {
    init(std.testing.allocator);
    defer deinit();

    var cube = Shape.initCube();
    defer cube.deinit();

    var sphere = Shape.initSubdividedSphere(3);
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

    var hemisphere = Shape.initParametricSphere(10, 10);
    defer hemisphere.deinit();
    hemisphere.invert(0, 0);

    hemisphere.removeDegenerate(0.001);
    hemisphere.unweld();
    hemisphere.weld(0.001, null);

    if (save) hemisphere.saveToObj("zmesh.invert.obj");
}
