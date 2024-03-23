# zsdl v0.1.0 - bindings for SDL2 and SDL3 (wip)

## Getting started

Copy `zsdl` folder to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zsdl = .{ .path = "path/to/local/zsdl" },
```

Then in your `build.zig` add:

```zig
pub fn build(b: *std.Build) !void {
    const exe = b.addExecutable(.{ ... });

    const zsdl = b.dependency("zsdl", .{});
    const zsdl_path = zsdl.path("").getPath(b);

    exe.root_module.addImport("zsdl2", zsdl.module("zsdl2"));

    try @import("zsdl").addLibraryPathsTo(exe, zsdl_path);
    @import("zsdl").link_SDL2(exe);

    try @import("zsdl").install_sdl2(&exe.step, options.target.result, .bin, zsdl_path);
}
```

Now in your code you may import and use `zsdl2`:

```zig
const std = @import("std");
const sdl = @import("zsdl2");

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
