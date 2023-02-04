# zphysics - Zig API and C API for Jolt Physics (v0.0.4)

[Jolt Physics](https://github.com/jrouwe/JoltPhysics) is a fast and modern physics library written in C++.

This project aims to provide high-performance, consistent and roboust [C API](libs) and Zig API for Jolt.

## Getting started

Copy `zphysics` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zphy = @import("libs/zphysics/build.zig");

pub fn build(b: *std.Build) void {
    ...
    const zphy_options = zphy.BuildOptionsStep.init(b, .{
        .use_double_precision = false,
    });
    const zphy_pkg = zphy.getPkg(&.{zphy_options.getPkg()});

    exe.addPackage(zphy_pkg);

    zphy.link(exe, zphy_options);
}
```

Now in your code you may import and use `zphysics`:

```zig
const zphy = @import("zphysics");

pub fn main() !void {
    try zphy.init(allocator, .{});
    defer zphy.deinit();

    ...

    const physics_system = try zphy.PhysicsSystem.create(
        @ptrCast(*const zphy.BroadPhaseLayerInterface, broad_phase_layer_interface),
        @ptrCast(*const zphy.ObjectVsBroadPhaseLayerFilter, object_vs_broad_phase_layer_filter),
        @ptrCast(*const zphy.ObjectLayerPairFilter, object_layer_pair_filter),
        .{
            .max_bodies = 1024,
            .num_body_mutexes = 0,
            .max_body_pairs = 1024,
            .max_contact_constraints = 1024,
        },
    );
    defer physics_system.destroy();
}
```
