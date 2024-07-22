# zflecs v0.1.0
Zig build package and bindings for [flecs](https://github.com/SanderMertens/flecs) ECS v3.2.11

## Getting started

Copy `zflecs` folder to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zflecs = .{ .path = "libs/zflecs" },
```

Then in your `build.zig` add:

```zig
pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    const zflecs = b.dependency("zflecs", .{});
    exe.root_module.addImport("zflecs", zflecs.module("root"));
    exe.linkLibrary(zflecs.artifact("flecs"));
}
```

Now in your code you may import and use `zflecs`:

```zig
const std = @import("std");
const ecs = @import("zflecs");

const Position = struct { x: f32, y: f32 };
const Velocity = struct { x: f32, y: f32 };
const Eats = struct {};
const Apples = struct {};

fn move_system(positions: []Position, velocities: []const Velocity) void {
    for (positions, velocities) |*p, v| {
        p.x += v.x;
        p.y += v.y;
    }
}

//Optionally, systems can receive the components iterator (usually not necessary)
fn move_system_with_it(it: *ecs.iter_t, positions: []Position, velocities: []const Velocity) void {
    const type_str = ecs.table_str(it.world, it.table).?;
    print("Move entities with [{s}]\n", .{type_str});
    defer ecs.os.free(type_str);

    for (positions, velocities) |*p, v| {
        p.x += v.x;
        p.y += v.y;
    }
}

pub fn main() !void {
    const world = ecs.init();
    defer _ = ecs.fini(world);

    ecs.COMPONENT(world, Position);
    ecs.COMPONENT(world, Velocity);

    ecs.TAG(world, Eats);
    ecs.TAG(world, Apples);

    ecs.ADD_SYSTEM(world, "move system", ecs.OnUpdate, move_system);
    ecs.ADD_SYSTEM(world, "move system with iterator", ecs.OnUpdate, move_system_with_it);

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

`zig build run` should result in:

```
Move entities with [main.Position, main.Velocity, (Identifier,Name), (main.Eats,main.Apples)]
Move entities with [main.Position, main.Velocity, (Identifier,Name), (main.Eats,main.Apples)]
Bob's position is (4, 8)
```
