## Getting started

Copy `zbullet` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
pub fn build(b: *Builder) void {
    ...
    const zbullet = std.build.Pkg{
        .name = "zbullet",
        .path = .{ .path = "libs/zbullet/src/zbullet.zig" },
    };
    exe.addPackage(zbullet);
    @import("libs/zbullet/build.zig").link(b, exe);
}
```

Now in your code you may import and use zbullet:

```zig
const zbt = @import("zbullet");

pub fn main() !void {
    const world = try zbt.World.init(.{});
    defer world.deinit();

    // Create unit cube shape.
    const box_shape = try zbt.BoxShape.init(&.{ 0.5, 0.5, 0.5 });
    defer box_shape.deinit();

    // Create rigid body that will use above shape.
    const initial_transform = [_]f32{
        1.0, 0.0, 0.0, // orientation
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        2.0, 2.0, 2.0, // translation
    };
    const box_body = try zbt.Body.init(
        1.0, // mass (must be 0.0 for static objects)
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
        _ = world.stepSimulation(time_step, 1, 1.0 / 60.0);
        ...
    }
}
```
