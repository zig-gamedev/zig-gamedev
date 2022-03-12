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
    var state = zns.createState();

    const n2 = zns.noise2(&state, 0.1, 0.2);
    const n3 = zns.noise3(&state, 1.0, 2.0, 3.0);

    state.fractal_type = .fbm;

    const f2 = zns.noise2(&state, 0.1, 0.2);
}
```
