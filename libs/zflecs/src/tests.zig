const std = @import("std");
const zflecs = @import("zflecs.zig");

const expect = std.testing.expect;

test "zflecs.basic" {
    const world = zflecs.init();
    defer _ = zflecs.fini(world);
    try expect(zflecs.is_fini(world) == false);

    const world_info = zflecs.get_world_info(world);
    std.debug.print("\n{any}\n", .{world_info});
}
