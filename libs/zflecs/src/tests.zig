const std = @import("std");
const ecs = @import("zflecs.zig");

const expect = std.testing.expect;

test "zflecs.basic" {
    const world = ecs.init();
    defer _ = ecs.fini(world);

    try expect(ecs.is_fini(world) == false);

    ecs.dim(world, 100);

    const e = ecs.entity_init(world, &.{ .name = "aaa" });
    try expect(e != 0);
}
