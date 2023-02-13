const std = @import("std");
const ecs = @import("zflecs.zig");

const expect = std.testing.expect;

test "zflecs.basic" {
    const world = ecs.init();
    defer _ = ecs.fini(world);
    try expect(ecs.is_fini(world) == false);

    const world_info = ecs.get_world_info(world);
    std.debug.print("\n{any}\n", .{world_info});
}
