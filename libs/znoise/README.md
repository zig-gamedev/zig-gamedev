# znoise - Zig bindings for FastNoiseLite

## Getting started

Copy `znoise` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
pub fn build(b: *std.build.Builder) void {
    ...
    const znoise_pkg = std.build.Pkg{
        .name = "znoise",
        .path = .{ .path = "libs/znoise/src/znoise.zig" },
    };
    exe.addPackage(znoise_pkg);
    @import("libs/znoise/build.zig").link(b, exe);
}
```

Now in your code you may import and use znoise:

```zig
const zns = @import("znoise");

pub fn main() !void {
    ...
    {
        const state = zns.State{};
        const n2 = zns.noise2(&state, 0.1, 0.2);
        const n3 = zns.noise3(&state, 1.0, 2.0, 3.0);
    }

    {
        const state = zns.State{
            .seed = 1337,
            .frequency = 0.01,
            .noise_type = .opensimplex2,
            .rotation_type3 = .none,
            .fractal_type = .none,
            .octaves = 3,
            .lacunarity = 2.0,
            .gain = 0.5,
            .weighted_strength = 0.0,
            .ping_pong_strength = 2.0,
            .cellular_distance_func = .euclideansq,
            .cellular_return_type = .distance,
            .cellular_jitter_mod = 1.0,
            .domain_warp_type = .opensimplex2,
            .domain_warp_amp = 1.0,
        };
        const n = zns.noise2(&state, 0.1, 0.2);
    }
}
```
