const std = @import("std");
const expect = std.testing.expect;
const c = @cImport({
    @cInclude("cbullet.h");
});

pub const Vector3 = [3]f32;

pub const World = struct {
    handle: c.CbtWorldHandle,

    pub fn create() World {
        return .{ .handle = c.cbtWorldCreate() };
    }

    pub fn destroy(world: World) void {
        c.cbtWorldDestroy(world.handle);
    }

    pub fn setGravity(world: World, gravity: Vector3) void {
        c.cbtWorldSetGravity(world.handle, &gravity);
    }

    pub fn getGravity(world: World) Vector3 {
        var gravity: Vector3 = undefined;
        c.cbtWorldGetGravity(world.handle, &gravity);
        return gravity;
    }
};

test "zbullet.world.gravity" {
    const world = World.create();
    defer world.destroy();

    world.setGravity(.{ 0.0, -10.0, 0.0 });
    const gravity = world.getGravity();
    try expect(gravity[0] == 0.0 and gravity[1] == -10.0 and gravity[2] == 0.0);
}
