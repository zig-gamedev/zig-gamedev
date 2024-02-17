# zcgltf 0.1.0 - Zig build system and bindings for [cgltf 1.13](https://github.com/jkuhlmann/cgltf)

## Getting started

Copy `zcgltf` folder to a `libs` subdirectory of the root of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zcgltf = .{ .path = "libs/zcgltf" },
```

Then in your `build.zig` add:

```zig
const std = @import("std");
const zcgltf = @import("zcgltf");

pub fn build(b: *std.Build) void {
    ...
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zcgltf_pkg = zcgltf.package(b, target, optimize, .{});

    zcgltf_pkg.link(exe);
}
```