const std = @import("std");
const ecs = @import("zflecs.zig");

const expect = std.testing.expect;
const expectEqual = std.testing.expectEqual;

const print = std.log.info;
//const print = std.debug.print;

const Position = struct { x: f32, y: f32 };
const Velocity = struct { x: f32, y: f32 };
const Walking = struct {};
const Direction = enum { north, south, east, west };

test {
    std.testing.refAllDeclsRecursive(@This());
}

test "zflecs.entities.basics" {
    print("\n", .{});

    const world = ecs.init();
    defer _ = ecs.fini(world);

    ecs.COMPONENT(world, Position);
    ecs.TAG(world, Walking);

    const bob = ecs.set_name(world, 0, "Bob");

    _ = ecs.set(world, bob, Position, .{ .x = 10, .y = 20 });
    ecs.add(world, bob, Walking);

    const ptr = ecs.get(world, bob, Position).?;
    print("({d}, {d})\n", .{ ptr.x, ptr.y });

    _ = ecs.set(world, bob, Position, .{ .x = 20, .y = 30 });

    const alice = ecs.set_name(world, 0, "Alice");
    _ = ecs.set(world, alice, Position, .{ .x = 10, .y = 20 });
    ecs.add(world, alice, Walking);

    const str = ecs.type_str(world, ecs.get_type(world, alice)).?;
    defer ecs.os.free(str);
    print("[{s}]\n", .{str});

    ecs.remove(world, alice, Walking);

    {
        var term = ecs.term_t{ .id = ecs.id(Position) };
        var it = ecs.term_iter(world, &term);
        while (ecs.term_next(&it)) {
            if (ecs.field(&it, Position, 1)) |positions| {
                for (positions, it.entities()) |p, e| {
                    print(
                        "Term loop: {s}: ({d}, {d})\n",
                        .{ ecs.get_name(world, e).?, p.x, p.y },
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
            } ++ ecs.array(ecs.term_t, ecs.FLECS_TERM_DESC_MAX - 2),
        });
        defer ecs.filter_fini(filter);

        var it = ecs.filter_iter(world, filter);
        while (ecs.filter_next(&it)) {
            for (it.entities()) |e| {
                print("Filter loop: {s}\n", .{ecs.get_name(world, e).?});
            }
        }
    }

    {
        const query = _: {
            var desc = ecs.query_desc_t{};
            desc.filter.terms[0].id = ecs.id(Position);
            desc.filter.terms[1].id = ecs.id(Walking);
            break :_ try ecs.query_init(world, &desc);
        };
        defer ecs.query_fini(query);
    }

    {
        const query = try ecs.query_init(world, &.{
            .filter = .{
                .terms = [_]ecs.term_t{
                    .{ .id = ecs.id(Position) },
                    .{ .id = ecs.id(Walking) },
                } ++ ecs.array(ecs.term_t, ecs.FLECS_TERM_DESC_MAX - 2),
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
    print("\n", .{});

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

    {
        const p0 = ecs.pair(ecs.id(u31), e0);
        const p1 = ecs.pair(e0, e0);
        const p2 = ecs.pair(ecs.OnUpdate, ecs.id(Direction));
        {
            const str = ecs.id_str(world, p0).?;
            defer ecs.os.free(str);
            print("{s}\n", .{str});
        }
        {
            const str = ecs.id_str(world, p1).?;
            defer ecs.os.free(str);
            print("{s}\n", .{str});
        }
        {
            const str = ecs.id_str(world, p2).?;
            defer ecs.os.free(str);
            print("{s}\n", .{str});
        }
    }

    const S0 = struct {
        a: f32 = 3.0,
    };
    ecs.COMPONENT(world, S0);

    ecs.TAG(world, Walking);

    const PrintIdHelper = struct {
        fn printId(in_world: *ecs.world_t, comptime T: type) void {
            const id_str = ecs.id_str(in_world, ecs.id(T)).?;
            defer ecs.os.free(id_str);

            print("{s} id: {d}\n", .{ id_str, ecs.id(T) });
        }
    };

    PrintIdHelper.printId(world, *const Position);
    PrintIdHelper.printId(world, ?*const Position);
    PrintIdHelper.printId(world, *Position);
    PrintIdHelper.printId(world, Position);
    PrintIdHelper.printId(world, *Direction);
    PrintIdHelper.printId(world, *Walking);
    PrintIdHelper.printId(world, *u31);

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

    const e0_type_str = ecs.type_str(world, ecs.get_type(world, e0)).?;
    defer ecs.os.free(e0_type_str);

    const e0_table_str = ecs.table_str(world, ecs.get_table(world, e0)).?;
    defer ecs.os.free(e0_table_str);

    const e0_str = ecs.entity_str(world, e0).?;
    defer ecs.os.free(e0_str);

    print("type str: {s}\n", .{e0_type_str});
    print("table str: {s}\n", .{e0_table_str});
    print("entity str: {s}\n", .{e0_str});

    {
        const str = ecs.type_str(world, ecs.get_type(world, ecs.id(Position))).?;
        defer ecs.os.free(str);
        print("{s}\n", .{str});
    }
    {
        const str = ecs.id_str(world, ecs.id(Position)).?;
        defer ecs.os.free(str);
        print("{s}\n", .{str});
    }
}

const Eats = struct {};
const Apples = struct {};

fn move(it: *ecs.iter_t) callconv(.C) void {
    const p = ecs.field(it, Position, 1).?;
    const v = ecs.field(it, Velocity, 2).?;

    const type_str = ecs.table_str(it.world, it.table).?;
    print("Move entities with [{s}]\n", .{type_str});
    defer ecs.os.free(type_str);

    for (0..it.count()) |i| {
        p[i].x += v[i].x;
        p[i].y += v[i].y;
    }
}

test "zflecs.helloworld" {
    print("\n", .{});

    const world = ecs.init();
    defer _ = ecs.fini(world);

    ecs.COMPONENT(world, Position);
    ecs.COMPONENT(world, Velocity);

    ecs.TAG(world, Eats);
    ecs.TAG(world, Apples);

    {
        var system_desc = ecs.system_desc_t{};
        system_desc.callback = move;
        system_desc.query.filter.terms[0] = .{ .id = ecs.id(Position) };
        system_desc.query.filter.terms[1] = .{ .id = ecs.id(Velocity) };
        ecs.SYSTEM(world, "move system", ecs.OnUpdate, &system_desc);
    }

    const bob = ecs.new_entity(world, "Bob");
    _ = ecs.set(world, bob, Position, .{ .x = 0, .y = 0 });
    _ = ecs.set(world, bob, Velocity, .{ .x = 1, .y = 2 });
    ecs.add_pair(world, bob, ecs.id(Eats), ecs.id(Apples));

    _ = ecs.progress(world, 0);
    _ = ecs.progress(world, 0);

    const p = ecs.get(world, bob, Position).?;
    print("Bob's position is ({d}, {d})\n", .{ p.x, p.y });
}

test "zflecs.try_different_alignments" {
    const world = ecs.init();
    defer _ = ecs.fini(world);

    const AlignmentsToTest = [_]usize{ 1, 2, 4, 8, 16 };
    inline for (AlignmentsToTest) |component_alignment| {
        const AlignedComponent = struct {
            fn Component(comptime alignment: usize) type {
                return struct { dummy: u32 align(alignment) = 0 };
            }
        };

        const Component = AlignedComponent.Component(component_alignment);

        ecs.COMPONENT(world, Component);
        const entity = ecs.new_entity(world, "");

        _ = ecs.set(world, entity, Component, .{});
        _ = ecs.get(world, entity, Component);
    }
}

test "zflecs.pairs.tag-tag" {
    const world = ecs.init();
    defer _ = ecs.fini(world);

    const Slowly = struct {};
    ecs.TAG(world, Slowly);
    ecs.TAG(world, Walking);

    const entity = ecs.new_entity(world, "Bob");

    _ = ecs.add_pair(world, entity, ecs.id(Slowly), ecs.id(Walking));
    try expect(ecs.has_pair(world, entity, ecs.id(Slowly), ecs.id(Walking)));

    _ = ecs.remove_pair(world, entity, ecs.id(Slowly), ecs.id(Walking));
    try expect(!ecs.has_pair(world, entity, ecs.id(Slowly), ecs.id(Walking)));
}

test "zflecs.pairs.component-tag" {
    const world = ecs.init();
    defer _ = ecs.fini(world);

    const Speed = u8;
    ecs.COMPONENT(world, Speed);
    ecs.TAG(world, Walking);

    const entity = ecs.new_entity(world, "Bob");

    _ = ecs.set_pair(world, entity, ecs.id(Speed), ecs.id(Walking), Speed, 2);
    try expect(ecs.has_pair(world, entity, ecs.id(Speed), ecs.id(Walking)));
    try expectEqual(@as(u8, 2), ecs.get_pair(world, entity, ecs.id(Speed), ecs.id(Walking), Speed).?.*);

    _ = ecs.remove_pair(world, entity, ecs.id(Speed), ecs.id(Walking));
    try expect(!ecs.has_pair(world, entity, ecs.id(Speed), ecs.id(Walking)));
    try expectEqual(@as(?*const u8, null), ecs.get_pair(world, entity, ecs.id(Speed), ecs.id(Walking), Speed));
}

test "zflecs.pairs.delete-children" {
    const world = ecs.init();
    defer _ = ecs.fini(world);

    const Camera = struct { id: u8 };

    ecs.COMPONENT(world, Camera);

    const entity = ecs.new_entity(world, "scene");

    const fps = ecs.new_w_pair(world, ecs.ChildOf, entity);
    _ = ecs.set(world, fps, Camera, .{ .id = 1 });
    const third_person = ecs.new_w_pair(world, ecs.ChildOf, entity);
    _ = ecs.set(world, third_person, Camera, .{ .id = 2 });

    var found: u8 = 0;
    var it = ecs.children(world, entity);
    while (ecs.children_next(&it)) {
        for (0..it.count()) |i| {
            const child_entity = it.entities()[i];
            const p: ?*const Camera = ecs.get(world, child_entity, Camera);
            try expectEqual(@as(u8, @intCast(i)), p.?.id - @as(u8, 1));
            found += 1;
        }
    }
    try expectEqual(@as(u8, 2), found);
    ecs.delete_children(world, entity);

    found = 0;
    it = ecs.children(world, entity);
    while (ecs.children_next(&it)) {
        for (0..it.count()) |_| {
            found += 1;
        }
    }
    try expectEqual(@as(u8, 0), found);
}

test "zflecs.struct-dtor-hook" {
    const world = ecs.init();
    defer _ = ecs.fini(world);

    const Chat = struct {
        messages: std.ArrayList([]const u8),

        pub fn init(allocator: std.mem.Allocator) @This() {
            return @This(){
                .messages = std.ArrayList([]const u8).init(allocator),
            };
        }

        pub fn dtor(self: @This()) void {
            self.messages.deinit();
        }
    };

    ecs.COMPONENT(world, Chat);
    {
        var system_desc = ecs.system_desc_t{};
        system_desc.callback = struct {
            pub fn chatSystem(it: *ecs.iter_t) callconv(.C) void {
                const chat_components = ecs.field(it, Chat, 1).?;
                for (0..it.count()) |i| {
                    chat_components[i].messages.append("some words hi") catch @panic("whomp");
                }
            }
        }.chatSystem;
        system_desc.query.filter.terms[0] = .{ .id = ecs.id(Chat) };
        ecs.SYSTEM(world, "Chat system", ecs.OnUpdate, &system_desc);
    }

    const chat_entity = ecs.new_entity(world, "Chat entity");
    _ = ecs.set(world, chat_entity, Chat, Chat.init(std.testing.allocator));

    _ = ecs.progress(world, 0);

    const chat_component = ecs.get(world, chat_entity, Chat).?;
    try std.testing.expect(chat_component.messages.items.len == 1);

    // This test fails if the ".hooks = .{ .dtor = ... }" from COMPONENT is
    // commented out since the cleanup is never called to free the ArrayList
    // memory.
}
