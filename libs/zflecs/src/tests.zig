const std = @import("std");
const ecs = @import("zflecs.zig");

const expect = std.testing.expect;

test "zflecs.basic" {
    const world = ecs.init();
    defer _ = ecs.fini(world);

    try expect(ecs.is_fini(world) == false);

    ecs.dim(world, 100);

    const e0 = ecs.entity_init(world, &.{ .name = "aaa" });
    try expect(e0 != 0);
    try expect(ecs.is_alive(world, e0));
    try expect(ecs.is_valid(world, e0));

    const e1 = ecs.new_id(world);
    try expect(ecs.is_alive(world, e1));
    try expect(ecs.is_valid(world, e1));

    _ = ecs.clone(world, e1, e0, false);
    try expect(ecs.is_alive(world, e1));
    try expect(ecs.is_valid(world, e1));

    ecs.delete(world, e1);
    try expect(!ecs.is_alive(world, e1));
    try expect(!ecs.is_valid(world, e1));

    const e0_type_str = ecs.type_str(world, ecs.get_type(world, e0));
    defer ecs.os_free(e0_type_str);

    const e0_table_str = ecs.table_str(world, ecs.get_table(world, e0));
    defer ecs.os_free(e0_table_str);

    const e0_str = ecs.entity_str(world, e0);
    defer ecs.os_free(e0_str);

    try expect(ecs.table_str(world, null) == null);

    std.debug.print("\n", .{});
    std.debug.print("type str: {s}\n", .{e0_type_str});
    std.debug.print("table str: {?s}\n", .{e0_table_str});
    std.debug.print("entity str: {?s}\n", .{e0_str});

    const Position = struct {
        x: f32,
        y: f32,
    };
    std.debug.print("{d}\n", .{ecs.id(Position)});
}
