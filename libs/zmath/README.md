# zmath v0.9.6 - SIMD math library for game developers

Tested on x86_64 and AArch64.

Provides ~140 optimized routines and ~70 extensive tests.

Can be used with any graphics API.

Documentation can be found [here](https://github.com/michal-z/zig-gamedev/blob/main/libs/zmath/src/zmath.zig).

Benchamrks can be found [here](https://github.com/michal-z/zig-gamedev/blob/main/libs/zmath/src/benchmark.zig).

An intro article can be found [here](https://zig.news/michalz/fast-multi-platform-simd-math-library-in-zig-2adn).

## Getting started

Copy `zmath` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zmath = @import("libs/zmath/build.zig");

pub fn build(b: *std.Build) void {
    ...
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    zmath_pkg = zmath.package(b, target, optimize, .{
        .options = .{ .enable_cross_platform_determinism = true },
    });

    zmath_pkg.link(exe);
}
```

Now in your code you may import and use zmath:

```zig
const zm = @import("zmath");

pub fn main() !void {
    //
    // OpenGL/Vulkan example
    //
    const object_to_world = zm.rotationY(..);
    const world_to_view = zm.lookAtRh(
        zm.f32x4(3.0, 3.0, 3.0, 1.0), // eye position
        zm.f32x4(0.0, 0.0, 0.0, 1.0), // focus point
        zm.f32x4(0.0, 1.0, 0.0, 0.0), // up direction ('w' coord is zero because this is a vector not a point)
    );
    // `perspectiveFovRhGl` produces Z values in [-1.0, 1.0] range (Vulkan app should use `perspectiveFovRh`)
    const view_to_clip = zm.perspectiveFovRhGl(0.25 * math.pi, aspect_ratio, 0.1, 20.0);

    const object_to_view = zm.mul(object_to_world, world_to_view);
    const object_to_clip = zm.mul(object_to_view, view_to_clip);

    // Transposition is needed because GLSL uses column-major matrices by default
    gl.uniformMatrix4fv(0, 1, gl.TRUE, zm.arrNPtr(&object_to_clip));
    
    // In GLSL: gl_Position = vec4(in_position, 1.0) * object_to_clip;
    
    //
    // DirectX example
    //
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
    
    // In HLSL: out_position_sv = mul(float4(in_position, 1.0), object_to_clip);
    
    //
    // 'WASD' camera movement example
    //
    {
        const speed = zm.f32x4s(10.0);
        const delta_time = zm.f32x4s(demo.frame_stats.delta_time);
        const transform = zm.mul(zm.rotationX(demo.camera.pitch), zm.rotationY(demo.camera.yaw));
        var forward = zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), transform));

        zm.storeArr3(&demo.camera.forward, forward);

        const right = speed * delta_time * zm.normalize3(zm.cross3(zm.f32x4(0.0, 1.0, 0.0, 0.0), forward));
        forward = speed * delta_time * forward;

        var cam_pos = zm.loadArr3(demo.camera.position);

        if (keyDown('W')) {
            cam_pos += forward;
        } else if (keyDown('S')) {
            cam_pos -= forward;
        }
        if (keyDown('D')) {
            cam_pos += right;
        } else if (keyDown('A')) {
            cam_pos -= right;
        }

        zm.storeArr3(&demo.camera.position, cam_pos);
    }
   
    //
    // SIMD wave equation solver example (works with vector width 4, 8 and 16)
    // 'T' can be F32x4, F32x8 or F32x16
    //
    var z_index: i32 = 0;
    while (z_index < grid_size) : (z_index += 1) {
        const z = scale * @intToFloat(f32, z_index - grid_size / 2);
        const vz = zm.splat(T, z);

        var x_index: i32 = 0;
        while (x_index < grid_size) : (x_index += zm.veclen(T)) {
            const x = scale * @intToFloat(f32, x_index - grid_size / 2);
            const vx = zm.splat(T, x) + voffset * zm.splat(T, scale);

            const d = zm.sqrt(vx * vx + vz * vz);
            const vy = zm.sin(d - vtime);

            const index = @intCast(usize, x_index + z_index * grid_size);
            zm.store(xslice[index..], vx, 0);
            zm.store(yslice[index..], vy, 0);
            zm.store(zslice[index..], vz, 0);
        }
    }
}
```
