# zsdl - bindings for SDL2 (wip)

## Getting started

Copy `zsdl` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zsdl = @import("libs/zsdl/build.zig");

pub fn build(b: *std.Build) void {
    ...
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zsdl_pkg = zsdl.package(b, target, optimize, .{});

    zsdl_pkg.link(exe);
}
```

Now in your code you may import and use `zsdl`:

```zig
const std = @import("std");
const sdl = @import("zsdl");

pub fn main() !void {
    ...
    try sdl.init(.{ .audio = true, .video = true });
    defer sdl.quit();

    const window = try sdl.Window.create(
        "zig-gamedev-window",
        sdl.Window.pos_undefined,
        sdl.Window.pos_undefined,
        600,
        600,
        .{ .opengl = true, .allow_highdpi = true },
    );
    defer window.destroy();
    ...
}
```
