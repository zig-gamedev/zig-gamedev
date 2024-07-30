# zphysics v0.1.0 - Build package, [C API](libs/JoltC) and bindings for Jolt Physics

[Jolt Physics](https://github.com/jrouwe/JoltPhysics) is a fast and modern physics library written in C++.

This project aims to provide high-performance, consistent and roboust [C API](libs/JoltC) and Zig API for Jolt.

For a simple sample applications please see [here](https://github.com/michal-z/zig-gamedev/tree/main/samples/physics_test_wgpu/src/physics_test_wgpu.zig).

## Getting started

Copy `zphysics` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zphysics = .{ .path = "libs/zphysics" },
```

Then in your `build.zig` add:
```zig
pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    const zphysics = b.dependency("zphysics", .{
        .use_double_precision = false,
        .enable_cross_platform_determinism = true,
    });
    exe.root_module.addImport("zphysics", zphysics.module("root"));
    exe.linkLibrary(zphysics.artifact("joltc"));
}
```

Now in your code you may import and use `zphysics`:

```zig
const zphy = @import("zphysics");

pub fn main() !void {
    try zphy.init(allocator, .{});
    defer zphy.deinit();

    // Create physics system
    const physics_system = try zphy.PhysicsSystem.create(
        ... // layer interfaces - please see sample application
        .{
            .max_bodies = 1024,
            .num_body_mutexes = 0,
            .max_body_pairs = 1024,
            .max_contact_constraints = 1024,
        },
    );
    defer physics_system.destroy();

    // Create shape
    const body_interface = physics_system.getBodyInterfaceMut();

    const shape_settings = try zphy.BoxShapeSettings.create(.{ 1.0, 1.0, 1.0 });
    defer shape_settings.release();

    const shape = try shape_settings.createShape();
    defer shape.release();

    // Create body
    const body_id = try body_interface.createAndAddBody(.{
        .position = .{ 0.0, -1.0, 0.0, 1.0 },
        .rotation = .{ 0.0, 0.0, 0.0, 1.0 },
        .shape = shape,
        .motion_type = .dynamic,
        .object_layer = object_layers.non_moving,
    }, .activate);
    defer body_interface.removeAndDestroyBody(body_id);

    physics_system.optimizeBroadPhase();

    // Perform ray cast
    {
        const query = physics_system.getNarrowPhaseQuery();

        var result = query.castRay(.{ .origin = .{ 0, 10, 0, 1 }, .direction = .{ 0, -20, 0, 0 } }, .{});
        if (result.has_hit) {
            // result.hit.body_id
            // result.hit.fraction
            // result.hit.sub_shape_id
            ...
        }
    }

    // Main loop
    while (...) {
        physics_system.update(1.0 / 60.0, .{});

        // Draw all bodies
        const bodies = physics_system.getBodiesUnsafe();
        for (bodies) |body| {
            if (!zphy.isValidBodyPointer(body)) continue;

            const object_to_world = object_to_world: {
                const position = zm.loadArr4(body.position);
                const rotation = zm.loadArr4(body.rotation);
                var xform = zm.matFromQuat(rotation);
                xform[3] = position;
                xform[3][3] = 1.0;
                break :object_to_world xform;
            };

            // Issue a draw call
            ...
        }
    }
}
```

## Usage in a shared library

The `joltc` artifact can be built as a shared library by specifying the `shared` build option:

```
    const zphysics = b.dependency("zphysics", .{
        .shared = true,
    });
```

If your zig module uses `zphysics` and is itself part of a shared library that is reloaded at runtime, then some additional steps are required:

- Before unloading the shared library, call `preUnload` to export the internal global state
- After reloading the shared library, call `postReload` to import the internal state and update allocator vtables

If you use `registerTrace` or `registerAssertFailed`, these must also be called again to update their function pointers.
