const std = @import("std");
const ecs = @import("zflecs.zig");

const expect = std.testing.expect;

const Position = struct { x: f32, y: f32 };
const Walking = struct {};
const Direction = enum { north, south, east, west };

test "zflecs.entities.basics" {
    std.debug.print("\n", .{});

    const world = ecs.init();
    defer _ = ecs.fini(world);

    ecs.COMPONENT(world, Position);
    ecs.TAG(world, Walking);

    const bob = ecs.set_name(world, 0, "Bob");

    _ = ecs.set(world, bob, Position, .{ .x = 10, .y = 20 });
    ecs.add(world, bob, Walking);

    const ptr = ecs.get(world, bob, Position).?;
    std.debug.print("({d}, {d})\n", .{ ptr.x, ptr.y });

    _ = ecs.set(world, bob, Position, .{ .x = 20, .y = 30 });

    const alice = ecs.set_name(world, 0, "Alice");
    _ = ecs.set(world, alice, Position, .{ .x = 10, .y = 20 });
    ecs.add(world, alice, Walking);

    const str = ecs.type_str(world, ecs.get_type(world, alice));
    defer ecs.os.free(str);
    std.debug.print("[{s}]\n", .{str});

    ecs.remove(world, alice, Walking);

    {
        var it = ecs.term_iter(world, &.{ .id = ecs.id(Position) });
        while (ecs.term_next(&it)) {
            if (ecs.field(&it, Position, 1)) |positions| {
                for (positions, 0..) |p, i| {
                    std.debug.print(
                        "Term loop: {?s}: ({d}, {d})\n",
                        .{ ecs.get_name(world, it.entities.?[i]), p.x, p.y },
                    );
                }
            }
        }
    }

    {
        var desc = ecs.filter_desc_t{};
        desc.terms[0].id = ecs.id(Position);
        const filter = try ecs.filter_init(world, &desc);
        defer ecs.filter_fini(filter);
    }

    {
        const filter = try ecs.filter_init(world, &.{
            .terms = [_]ecs.term_t{
                .{ .id = ecs.id(Position) },
                .{ .id = ecs.id(Walking) },
            } ++ ecs.array(ecs.term_t, ecs.TERM_DESC_CACHE_SIZE - 2),
        });
        defer ecs.filter_fini(filter);

        var it = ecs.filter_iter(world, filter);
        while (ecs.filter_next(&it)) {
            for (it.get_entities().?) |e| {
                std.debug.print("Filter loop: {?s}\n", .{ecs.get_name(world, e)});
            }
        }
    }

    {
        const query = _: {
            var desc = ecs.query_desc_t{};
            desc.filter.terms[0].id = ecs.id(Position);
            break :_ try ecs.query_init(world, &desc);
        };
        defer ecs.query_fini(query);
    }

    {
        const query = try ecs.query_init(world, &.{
            .filter = .{
                .terms = [_]ecs.term_t{.{ .id = ecs.id(Position) }} ++
                    ecs.array(ecs.term_t, ecs.TERM_DESC_CACHE_SIZE - 1),
            },
        });
        defer ecs.query_fini(query);
    }
}

fn registerComponents(world: *ecs.world_t) void {
    ecs.COMPONENT(world, *const Position);
    ecs.COMPONENT(world, ?*const Position);
}

test "zflecs.basic" {
    std.debug.print("\n", .{});

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

    try expect(ecs.table_str(world, null) == null);

    registerComponents(world);
    ecs.COMPONENT(world, *Position);
    ecs.COMPONENT(world, Position);
    ecs.COMPONENT(world, ?*const Position);
    ecs.COMPONENT(world, Direction);
    ecs.COMPONENT(world, f64);
    ecs.COMPONENT(world, u31);
    ecs.COMPONENT(world, u32);
    ecs.COMPONENT(world, f32);
    ecs.COMPONENT(world, f64);
    ecs.COMPONENT(world, i8);
    ecs.COMPONENT(world, ?*const i8);

    const S0 = struct {
        a: f32 = 3.0,
    };
    ecs.COMPONENT(world, S0);

    ecs.TAG(world, Walking);

    std.debug.print("{?s} id: {d}\n", .{ ecs.id_str(world, ecs.id(*const Position)), ecs.id(*const Position) });
    std.debug.print("{?s} id: {d}\n", .{ ecs.id_str(world, ecs.id(?*const Position)), ecs.id(?*const Position) });
    std.debug.print("{?s} id: {d}\n", .{ ecs.id_str(world, ecs.id(*Position)), ecs.id(*Position) });
    std.debug.print("{?s} id: {d}\n", .{ ecs.id_str(world, ecs.id(Position)), ecs.id(Position) });
    std.debug.print("{?s} id: {d}\n", .{ ecs.id_str(world, ecs.id(Direction)), ecs.id(Direction) });
    std.debug.print("{?s} id: {d}\n", .{ ecs.id_str(world, ecs.id(Walking)), ecs.id(Walking) });
    std.debug.print("{?s} id: {d}\n", .{ ecs.id_str(world, ecs.id(u31)), ecs.id(u31) });

    const p: Position = .{ .x = 1.0, .y = 2.0 };
    _ = ecs.set(world, e0, *const Position, &p);
    _ = ecs.set(world, e0, ?*const Position, null);
    _ = ecs.set(world, e0, Position, .{ .x = 1.0, .y = 2.0 });
    _ = ecs.set(world, e0, Direction, .west);
    _ = ecs.set(world, e0, u31, 123);
    _ = ecs.set(world, e0, u31, 1234);
    _ = ecs.set(world, e0, u32, 987);
    _ = ecs.set(world, e0, S0, .{});

    ecs.add(world, e0, Walking);

    try expect(ecs.get(world, e0, u31).?.* == 1234);
    try expect(ecs.get(world, e0, u32).?.* == 987);
    try expect(ecs.get(world, e0, S0).?.a == 3.0);
    try expect(ecs.get(world, e0, ?*const Position).?.* == null);
    try expect(ecs.get(world, e0, *const Position).?.* == &p);
    if (ecs.get(world, e0, Position)) |pos| {
        try expect(pos.x == p.x and pos.y == p.y);
    }

    const e0_type_str = ecs.type_str(world, ecs.get_type(world, e0));
    defer ecs.os.free(e0_type_str);

    const e0_table_str = ecs.table_str(world, ecs.get_table(world, e0));
    defer ecs.os.free(e0_table_str);

    const e0_str = ecs.entity_str(world, e0);
    defer ecs.os.free(e0_str);

    std.debug.print("type str: {s}\n", .{e0_type_str});
    std.debug.print("table str: {?s}\n", .{e0_table_str});
    std.debug.print("entity str: {?s}\n", .{e0_str});

    {
        const str = ecs.type_str(world, ecs.get_type(world, ecs.id(Position)));
        defer ecs.os.free(str);
        std.debug.print("{s}\n", .{str});
    }
    {
        const str = ecs.id_str(world, ecs.id(Position));
        defer ecs.os.free(str);
        std.debug.print("{?s}\n", .{str});
    }
}
