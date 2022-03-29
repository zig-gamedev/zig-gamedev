# zmath v0.3 - SIMD math library for game developers

## Features

Works on all OSes, works on x86_64 and ARM.

Provides ~140 optimized routines, ~70 extensive tests.

Can be used with any graphics API.

See functions list in the [code](https://github.com/michal-z/zig-gamedev/blob/main/libs/zmath/src/zmath.zig).

Read [intro article](https://github.com/michal-z/zig-gamedev/wiki/Fast,-multi-platform,-SIMD-math-library-in-Zig).

## Getting started

Copy `zmath` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
pub fn build(b: *std.build.Builder) void {
    ...
    const zmath_pkg = std.build.Pkg{
        .name = "zmath",
        .path = .{ .path = "libs/zmath/src/zmath.zig" },
    };
    exe.addPackage(zmath_pkg);
}
```

Now in your code you may import and use zmath:

```zig
const zm = @import("zmath");

pub fn main() !void {
    ...
    //
    // OpenGL/Vulkan convention
    //
    const proj = zm.perspectiveFovRh(
        0.25 * math.pi,
        @intToFloat(f32, gctx.viewport_width) / @intToFloat(f32, gctx.viewport_height),
        0.1,
        20.0,
    );
    const view_model = zm.mul(view, model);
    const proj_view_model = zm.mul(proj, view_model);

    gl.uniformMatrix4fv(0, 1, gl.FALSE, &zm.matToArray(proj_view_model));

    // zm.mul(mat, vec) `vec` is treated as a culumn vector

    //
    // DirectX convention
    //
    const object_to_world = zm.rotationY(@floatCast(f32, demo.frame_stats.time));
    const world_to_view = zm.lookAtLh(
        zm.f32x4(3.0, 3.0, -3.0, 1.0), // eye position
        zm.f32x4(0.0, 0.0, 0.0, 1.0), // focus point
        zm.f32x4(0.0, 1.0, 0.0, 0.0), // up direction ('w' coord is zero because this is a vector not a point)
    );
    const view_to_clip = zm.perspectiveFovLh(
        0.25 * math.pi,
        @intToFloat(f32, gctx.viewport_width) / @intToFloat(f32, gctx.viewport_height),
        0.1,
        20.0,
    );

    const object_to_view = zm.mul(object_to_world, world_to_view);
    const object_to_clip = zm.mul(object_to_view, view_to_clip);

    // zm.mul(vec, mat) `vec` is treated as a row vector
}
```
