# znoise v0.2.0 - Zig bindings for FastNoiseLite

## Getting started

Copy `znoise` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .znoise = .{ .path = "libs/znoise" },
```

Then in your `build.zig` add:

```zig
pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    const znoise = b.dependency("znoise", .{});
    exe.root_module.addImport("znoise", znoise.module("root"));
    exe.linkLibrary(znoise.artifact("FastNoiseLite"));
}
```

Now in your code you may import and use znoise:

```zig
const znoise = @import("znoise");

pub fn main() !void {
    ...
    {
        const gen = znoise.FnlGenerator{};
        const n2 = gen.noise2(0.1, 0.2);
        const n3 = gen.noise3(1.0, 2.0, 3.0);

        var x: f32 = 1.0;
        var y: f32 = 2.0;
        var z: f32 = 3.0;
        gen.domainWarp3(&x, &y, &z);
    }

    {
        const gen = znoise.FnlGenerator{
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
        const n = gen.noise2(0.1, 0.2);
    }
}
```
