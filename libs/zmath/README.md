# zmath v0.3 - SIMD math library for game developers

## Features

Should work on all OSes supported by Zig. Works on x86_64 and ARM.

Provides ~140 optimized routines and ~70 extensive tests.

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
    // OpenGL
    //
    const object_to_world = zm.rotationY(..);
    const world_to_view = zm.lookAtRh(
        zm.f32x4(3.0, 3.0, 3.0, 1.0), // eye position
        zm.f32x4(0.0, 0.0, 0.0, 1.0), // focus point
        zm.f32x4(0.0, 1.0, 0.0, 0.0), // up direction ('w' coord is zero because this is a vector not a point)
    );
    // `perspectiveFovRhGl` produces Z values in [-1.0, 1.0] range
    const view_to_clip = zm.perspectiveFovRhGl(0.25 * math.pi, aspect_ratio, 0.1, 20.0);

    const object_to_view = zm.mul(object_to_world, world_to_view);
    const object_to_clip = zm.mul(object_to_view, view_to_clip);

    // Transposition is needed because GLSL uses column-major matrices by default
    gl.uniformMatrix4fv(0, 1, gl.TRUE, zm.f32Ptr(&object_to_clip));
    ...
    //
    // DirectX
    //
    // zm.mul(vec, mat) `vec` is treated as a row vector
    const object_to_world = zm.rotationY(..);
    const world_to_view = zm.lookAtLh(
        zm.f32x4(3.0, 3.0, -3.0, 1.0), // eye position
        zm.f32x4(0.0, 0.0, 0.0, 1.0), // focus point
        zm.f32x4(0.0, 1.0, 0.0, 0.0), // up direction ('w' coord is zero because this is a vector not a point)
    );
    const view_to_clip = zm.perspectiveFovLh(0.25 * math.pi, aspect_ratio, 0.1, 20.0);

    const object_to_view = zm.mul(object_to_world, world_to_view);
    const object_to_clip = zm.mul(object_to_view, view_to_clip);
    
    // Transposition is needed because HLSL uses column-major matrices by default
    const mem = allocateUploadMemory(...);
    zm.storeMat(mem, zm.transpose(object_to_clip));
    ...
    //
    // 'WASD' camera movement
    //
    {
        const speed = zm.f32x4s(10.0);
        const delta_time = zm.f32x4s(demo.frame_stats.delta_time);
        const transform = zm.mul(zm.rotationX(demo.camera.pitch), zm.rotationY(demo.camera.yaw));
        var forward = zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), transform));

        zm.store(demo.camera.forward[0..], forward, 3);

        const right = speed * delta_time * zm.normalize3(zm.cross3(zm.f32x4(0.0, 1.0, 0.0, 0.0), forward));
        forward = speed * delta_time * forward;

        var campos = zm.load(demo.camera.position[0..], zm.Vec, 3);

        if (keyDown('W')) {
            campos += forward;
        } else if (keyDown('S')) {
            campos -= forward;
        }
        if (keyDown('D')) {
            campos += right;
        } else if (keyDown('A')) {
            campos -= right;
        }

        zm.store(demo.camera.position[0..], campos, 3);
    }
}
```
