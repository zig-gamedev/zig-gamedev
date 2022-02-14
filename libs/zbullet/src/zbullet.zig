const std = @import("std");
const expect = std.testing.expect;
const assert = std.debug.assert;

pub const World = opaque {
    pub const init = cbtWorldCreate;
    extern fn cbtWorldCreate() *const World;

    pub const deinit = cbtWorldDestroy;
    extern fn cbtWorldDestroy(world: *const World) void;

    pub const setGravity = cbtWorldSetGravity;
    extern fn cbtWorldSetGravity(world: *const World, gravity: *const [3]f32) void;

    pub const getGravity = cbtWorldGetGravity;
    extern fn cbtWorldGetGravity(world: *const World, gravity: *[3]f32) void;

    pub const stepSimulation = cbtWorldStepSimulation;
    extern fn cbtWorldStepSimulation(
        world: *const World,
        time_step: f32,
        max_sub_steps: c_int,
        fixed_time_step: f32,
    ) c_int;
};

pub const ShapeType = enum(c_int) {
    box = 0,
    sphere = 8,
};

pub const Shape = opaque {
    pub const allocate = cbtShapeAllocate;
    extern fn cbtShapeAllocate(stype: ShapeType) *const Shape;

    pub const deallocate = cbtShapeDeallocate;
    extern fn cbtShapeDeallocate(shape: *const Shape) void;

    pub const destroy = cbtShapeDestroy;
    extern fn cbtShapeDestroy(shape: *const Shape) void;

    pub const isCreated = cbtShapeIsCreated;
    extern fn cbtShapeIsCreated(shape: *const Shape) bool;

    pub const getType = cbtShapeGetType;
    extern fn cbtShapeGetType(shape: *const Shape) ShapeType;

    pub const setMargin = cbtShapeSetMargin;
    extern fn cbtShapeSetMargin(shape: *const Shape, margin: f32) void;

    pub const getMargin = cbtShapeGetMargin;
    extern fn cbtShapeGetMargin(shape: *const Shape) f32;
};

pub const BoxShape = opaque {
    pub fn init(half_extents: *const [3]f32) *const BoxShape {
        const box = allocate();
        box.create(half_extents);
        return box;
    }

    pub fn deinit(box: *const BoxShape) void {
        box.destroy();
        box.deallocate();
    }

    pub const create = cbtShapeBoxCreate;
    extern fn cbtShapeBoxCreate(box: *const BoxShape, half_extents: *const [3]f32) void;

    pub const getHalfExtentsWithoutMargin = cbtShapeBoxGetHalfExtentsWithoutMargin;
    extern fn cbtShapeBoxGetHalfExtentsWithoutMargin(box: *const BoxShape, half_extents: *[3]f32) void;

    pub const getHalfExtentsWithMargin = cbtShapeBoxGetHalfExtentsWithMargin;
    extern fn cbtShapeBoxGetHalfExtentsWithMargin(box: *const BoxShape, half_extents: *[3]f32) void;

    // zig fmt: off
    pub fn allocate() *const BoxShape { return @ptrCast(*const BoxShape, Shape.allocate(.box)); }
    pub fn deallocate(shape: *const BoxShape) void { @ptrCast(*const Shape, shape).deallocate(); }
    pub fn destroy(shape: *const BoxShape) void { @ptrCast(*const Shape, shape).destroy(); }
    pub fn isCreated(shape: *const BoxShape) bool { return @ptrCast(*const Shape, shape).isCreated(); }
    pub fn getType(shape: *const BoxShape) ShapeType { return @ptrCast(*const Shape, shape).getType(); }
    pub fn setMargin(shape: *const BoxShape, margin: f32) void { @ptrCast(*const Shape, shape).setMargin(margin); }
    pub fn getMargin(shape: *const BoxShape) f32 { return @ptrCast(*const Shape, shape).getMargin(); }
    // zig fmt: on
};

pub const SphereShape = opaque {
    pub fn init(radius: f32) *const SphereShape {
        const sphere = allocate();
        sphere.create(radius);
        return sphere;
    }

    pub fn deinit(sphere: *const SphereShape) void {
        sphere.destroy();
        sphere.deallocate();
    }

    pub const create = cbtShapeSphereCreate;
    extern fn cbtShapeSphereCreate(sphere: *const SphereShape, radius: f32) void;

    pub const getRadius = cbtShapeSphereGetRadius;
    extern fn cbtShapeSphereGetRadius(sphere: *const SphereShape) f32;

    pub const setUnscaledRadius = cbtShapeSphereSetUnscaledRadius;
    extern fn cbtShapeSphereSetUnscaledRadius(sphere: *const SphereShape, radius: f32) void;

    // zig fmt: off
    pub fn allocate() *const SphereShape { return @ptrCast(*const SphereShape, Shape.allocate(.sphere)); }
    pub fn deallocate(shape: *const SphereShape) void { @ptrCast(*const Shape, shape).deallocate(); }
    pub fn destroy(shape: *const SphereShape) void { @ptrCast(*const Shape, shape).destroy(); }
    pub fn isCreated(shape: *const SphereShape) bool { return @ptrCast(*const Shape, shape).isCreated(); }
    pub fn getType(shape: *const SphereShape) ShapeType { return @ptrCast(*const Shape, shape).getType(); }
    pub fn setMargin(shape: *const SphereShape, margin: f32) void { @ptrCast(*const Shape, shape).setMargin(margin); }
    pub fn getMargin(shape: *const SphereShape) f32 { return @ptrCast(*const Shape, shape).getMargin(); }
    // zig fmt: on
};

test "zbullet.world.gravity" {
    const world = World.init();
    defer world.deinit();

    world.setGravity(&.{ 0.0, -10.0, 0.0 });

    const num_substeps = world.stepSimulation(1.0 / 60.0, 1, 1.0 / 60.0);
    try expect(num_substeps == 1);

    var gravity: [3]f32 = undefined;
    world.getGravity(&gravity);

    try expect(gravity[0] == 0.0 and gravity[1] == -10.0 and gravity[2] == 0.0);
}

test "zbullet.shape.box" {
    {
        const box = BoxShape.init(&.{ 4.0, 4.0, 4.0 });
        defer box.deinit();
        try expect(box.isCreated());
        try expect(box.getType() == .box);
        box.setMargin(0.1);
        try expect(box.getMargin() == 0.1);

        var half_extents: [3]f32 = undefined;
        box.getHalfExtentsWithoutMargin(&half_extents);
        try expect(half_extents[0] == 3.9 and half_extents[1] == 3.9 and half_extents[2] == 3.9);

        box.getHalfExtentsWithMargin(&half_extents);
        try expect(half_extents[0] == 4.0 and half_extents[1] == 4.0 and half_extents[2] == 4.0);
    }
    {
        const box = BoxShape.allocate();
        defer box.deallocate();
        try expect(box.isCreated() == false);

        box.create(&.{ 1.0, 2.0, 3.0 });
        defer box.destroy();
        try expect(box.getType() == .box);
        try expect(box.isCreated() == true);
    }
}

test "zbullet.shape.sphere" {
    {
        const sphere = SphereShape.init(3.0);
        defer sphere.deinit();
        try expect(sphere.isCreated());
        try expect(sphere.getType() == .sphere);
        sphere.setMargin(0.1);
        try expect(sphere.getMargin() == 3.0); // For spheres margin == radius.

        try expect(sphere.getRadius() == 3.0);

        sphere.setUnscaledRadius(1.0);
        try expect(sphere.getRadius() == 1.0);
    }
    {
        const sphere = SphereShape.allocate();
        try expect(sphere.isCreated() == false);

        sphere.create(1.0);
        try expect(sphere.getType() == .sphere);
        try expect(sphere.isCreated() == true);
        try expect(sphere.getRadius() == 1.0);

        sphere.destroy();
        try expect(sphere.isCreated() == false);

        sphere.create(2.0);
        try expect(sphere.getType() == .sphere);
        try expect(sphere.isCreated() == true);
        try expect(sphere.getRadius() == 2.0);

        sphere.destroy();
        try expect(sphere.isCreated() == false);
        sphere.deallocate();
    }
}
