const std = @import("std");
const expect = std.testing.expect;
const assert = std.debug.assert;
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

    pub fn setGravity(world: World, gravity: []const f32) void {
        assert(gravity.len >= 3);
        c.cbtWorldSetGravity(world.handle, gravity.ptr);
    }

    pub fn getGravity(world: World, gravity: []f32) void {
        assert(gravity.len >= 3);
        c.cbtWorldGetGravity(world.handle, gravity.ptr);
    }
};

test "zbullet.world.gravity" {
    const zm = @import("zmath");

    const world = World.create();
    defer world.destroy();

    {
        const v = zm.f32x4(0.0, -10.0, 0.0, 0.0);
        var gravity: [3]f32 = undefined;
        zm.store(gravity[0..], v, 3);
        world.setGravity(gravity[0..]);
    }

    const gravity = blk: {
        var gravity: [3]f32 = undefined;
        world.getGravity(gravity[0..]);
        break :blk zm.load(gravity[0..], zm.F32x4, 3);
    };
    try expect(gravity[0] == 0.0 and gravity[1] == -10.0 and gravity[2] == 0.0);
}
