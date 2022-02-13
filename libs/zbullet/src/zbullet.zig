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

    //pub fn stepSimulation(world: World, time_step: f32, max_sub_steps: u32, fixed_time_step: f32) u32 {
    //    return @intCast(u32, c.cbtWorldStepSimulation(
    //        world.handle,
    //        time_step,
    //        @intCast(c_int, max_sub_steps),
    //        fixed_time_step,
    //    ));
    //}
};

pub const ShapeType = enum(c_int) {
    box = 0,
    sphere = 8,
    capsule = 11,
};

pub const Shape = opaque {
    pub const allocate = cbtShapeAllocate;
    extern fn cbtShapeAllocate(stype: ShapeType) *const Shape;

    pub const destroy = cbtShapeDestroy;
    extern fn cbtShapeDestroy(shape: *const Shape) void;

    pub const deallocate = cbtShapeDeallocate;
    extern fn cbtShapeDeallocate(shape: *const Shape) void;
};

pub const BoxShape = opaque {
    pub fn allocate() *const BoxShape {
        return @ptrCast(*const BoxShape, Shape.allocate(.box));
    }

    pub fn deallocate(box: *const BoxShape) void {
        @ptrCast(*const Shape, box).deallocate();
    }

    pub const create = cbtShapeBoxCreate;
    extern fn cbtShapeBoxCreate(box: *const BoxShape, half_extents: *const [3]f32) void;

    pub fn destroy(box: *const BoxShape) void {
        @ptrCast(*const Shape, box).destroy();
    }

    pub fn init(half_extents: *const [3]f32) *const BoxShape {
        const box = allocate();
        box.create(half_extents);
        return box;
    }

    pub fn deinit(box: *const BoxShape) void {
        box.destroy();
        box.deallocate();
    }
};

test "zbullet.world.gravity" {
    const world = World.init();
    defer world.deinit();

    world.setGravity(&.{ 0.0, -10.0, 0.0 });

    var gravity: [3]f32 = undefined;
    world.getGravity(&gravity);

    try expect(gravity[0] == 0.0 and gravity[1] == -10.0 and gravity[2] == 0.0);
}

test "zbullet.shape.box" {
    {
        const box = BoxShape.init(&.{ 4.0, 4.0, 4.0 });
        defer box.deinit();
    }
    {
        const box = BoxShape.allocate();
        defer box.deallocate();
        box.create(&.{ 1.0, 2.0, 3.0 });
        defer box.destroy();
    }
}
