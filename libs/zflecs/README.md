# zflecs - bindings for flecs ECS (wip)

## Getting started

Copy `zflecs` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zsdl = @import("libs/zflecs/build.zig");

pub fn build(b: *std.Build) void {
    ...
    const zflecs_pkg = zflecs.Package.build(b, target, optimize, .{});

    exe.addModule("zflecs", zflecs_pkg.zflecs);

    zflecs_pkg.link(exe);
}
```

Now in your code you may import and use `zflecs`:

```zig
const std = @import("std");
const ecs = @import("zflecs");

const Eats = struct {};
const Apples = struct {};

fn move(it: *ecs.iter_t) callconv(.C) void {
    const p = ecs.field(it, Position, 1).?;
    const v = ecs.field(it, Velocity, 2).?;

    const type_str = ecs.table_str(it.world, it.table).?;
    std.debug.print("Move entities with [{s}]\n", .{type_str});
    defer ecs.os.free(type_str);

    for (0..it.count()) |i| {
        p[i].x += v[i].x;
        p[i].y += v[i].y;
    }
}

pub fn main() !void {
    const world = ecs.init();
    defer _ = ecs.fini(world);

    ecs.COMPONENT(world, Position);
    ecs.COMPONENT(world, Velocity);

    ecs.TAG(world, Eats);
    ecs.TAG(world, Apples);

    ecs.SYSTEM(world, "move system", move, ecs.EcsOnUpdate, .{
        .filter = .{
            .terms = [_]ecs.term_t{
                .{ .id = ecs.id(Position) },
                .{ .id = ecs.id(Velocity) },
            } ++ ecs.array(ecs.term_t, ecs.TERM_DESC_CACHE_SIZE - 2),
        },
    });

    const bob = ecs.new_entity(world, "Bob");
    _ = ecs.set(world, bob, Position, .{ .x = 0, .y = 0 });
    _ = ecs.set(world, bob, Velocity, .{ .x = 1, .y = 2 });
    ecs.add_pair(world, bob, ecs.id(Eats), ecs.id(Apples));

    _ = ecs.progress(world, 0);
    _ = ecs.progress(world, 0);

    const p = ecs.get(world, bob, Position).?;
    std.debug.print("Bob's position is ({d}, {d})\n", .{ p.x, p.y });
}
```
