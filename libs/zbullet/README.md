# zbullet v0.1 - Zig bindings for Bullet Physics SDK

## Features

See tests in the [source code](https://github.com/michal-z/zig-gamedev/blob/main/libs/zbullet/src/zbullet.zig).

As an example programs see [intro 6](https://github.com/michal-z/zig-gamedev/blob/main/samples/intro/src/intro6.zig) and our [virtual physics lab](https://github.com/michal-z/zig-gamedev/tree/main/samples/bullet_physics_test) (uses cbullet C API directly).

## Getting started

Copy `zbullet` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zbullet = @import("libs/zbullet/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    exe.addPackage(zbullet.pkg);
    zbullet.link(exe);
}
```

Now in your code you may import and use zbullet:

```zig
const zbt = @import("zbullet");

pub fn main() !void {
    ...
    zbt.init(allocator);
    defer zbt.deinit();

    const world = zbt.World.init(.{});
    defer world.deinit();

    // Create unit cube shape.
    const box_shape = zbt.BoxShape.init(&.{ 0.5, 0.5, 0.5 });
    defer box_shape.deinit();

    // Create rigid body that will use above shape.
    const initial_transform = [_]f32{
        1.0, 0.0, 0.0, // orientation
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        2.0, 2.0, 2.0, // translation
    };
    const box_body = zbt.Body.init(
        1.0, // mass (0.0 for static objects)
        &initial_transform,
        box_shape.asShape(),
    );
    defer body.deinit();

    // Add body to the physics world.
    world.addBody(box_body);
    defer world.removeBody(box_body);

    while (...) {
        ...
        // Perform a simulation step.
        _ = world.stepSimulation(time_step, .{});
        ...
    }
}
```
