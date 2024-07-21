# zsdl v0.1.0 - bindings for SDL2 and SDL3 (wip)

## Getting started

Copy `zsdl` folder to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
        .zsdl = .{ .path = "path/to/local/zsdl" },
```
also add `SDL2-prebuilt` if you want to use our prebuilt libraries instead of system installed
```zig
        .sdl2_prebuilt = .{
            .url = "https://github.com/zig-gamedev/SDL2-prebuilt/archive/49b2267a0fedee9d594733617255dd979da51813.tar.gz",
            .hash = "1220930ce0d568bd606112d38c3f16a38841a5fd9de5c224f627cd953e7febb90bfa",
        },
```

Then in your `build.zig` add:

```zig
pub fn build(b: *std.Build) !void {
    const exe = b.addExecutable(.{ ... });

    const zsdl = b.dependency("zsdl", .{});
    exe.root_module.addImport("zsdl2", zsdl.module("zsdl2"));

    @import("zsdl").addLibraryPathsTo(exe);
    @import("zsdl").link_SDL2(exe);

    // Install prebuilt SDL2 lib
    @import("zsdl").install_sdl2(&exe.step, target.result, .bin);
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
