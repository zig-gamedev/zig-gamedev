const std = @import("std");
const expect = std.testing.expect;
const assert = std.debug.assert;
const c = @cImport({
    @cInclude("cbullet.h");
});

pub const World = struct {
    handle: c.CbtWorldHandle,

    pub fn allocateAndCreate() World {
        return .{ .handle = c.cbtWorldCreate() };
    }

    pub fn destroyAndDeallocate(world: World) void {
        c.cbtWorldDestroy(world.handle);
    }

    pub fn setGravity(world: World, gravity: [3]f32) void {
        c.cbtWorldSetGravity(world.handle, &gravity);
    }

    pub fn getGravity(world: World) [3]f32 {
        var gravity: [3]f32 = undefined;
        c.cbtWorldGetGravity(world.handle, &gravity);
        return gravity;
    }

    pub fn stepSimulation(world: World, time_step: f32, max_sub_steps: u32, fixed_time_step: f32) u32 {
        return @intCast(u32, c.cbtWorldStepSimulation(
            world.handle,
            time_step,
            @intCast(c_int, max_sub_steps),
            fixed_time_step,
        ));
    }
};

test "zbullet.world.gravity" {
    const zm = @import("zmath");

    const world = World.allocateAndCreate();
    defer world.destroyAndDeallocate();

    {
        const v = zm.f32x4(0.0, -10.0, 0.0, 0.0);
        var gravity: [3]f32 = undefined;
        zm.store(gravity[0..], v, 3);
        world.setGravity(gravity);
    }

    const num_steps = world.stepSimulation(1.0 / 60.0, 1, 1.0 / 60.0);
    try expect(num_steps == 1);

    const gravity = blk: {
        const gravity = world.getGravity();
        break :blk zm.load(gravity[0..], zm.F32x4, 3);
    };
    try expect(gravity[0] == 0.0 and gravity[1] == -10.0 and gravity[2] == 0.0);
}
