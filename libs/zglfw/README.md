# zglfw v0.9.0 - Zig build package and bindings for [GLFW 3.4](https://github.com/glfw/glfw/releases/tag/3.4)

## Getting started

Copy `zglfw` and `system-sdk` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zglfw = .{ .path = "libs/zglfw" },
    
    // Required for building glfw
    .@"system-sdk" = .{
        .url = "https://github.com/zig-gamedev/system-sdk/archive/refs/heads/main.tar.gz",
        .hash = "122035336d7535f655fe0ece31cea0fbf8ad2f1e42f64b073995050a425ca8f8fbdc",
    },
```

Then in your `build.zig` add:
```zig
pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    const zglfw = b.dependency("zglfw", .{});
    exe.root_module.addImport("zglfw", zglfw.module("root"));
    exe.linkLibrary(zglfw.artifact("glfw"));

    @import("system-sdk").addLibraryPathsTo(exe);
}
```

Now in your code you may import and use `zglfw`:
```zig
const glfw = @import("zglfw");

pub fn main() !void {
    try glfw.init();
    defer glfw.terminate();

    const window = try glfw.Window.create(600, 600, "zig-gamedev: minimal_glfw_gl", null);
    defer window.destroy();

    // setup your graphics context here

    while (!window.shouldClose()) {
        glfw.pollEvents();

        // render your things here
        
        window.swapBuffers();
    }
}
```

See [zig-gamedev samples](https://github.com/zig-gamedev/zig-gamedev/tree/main/samples) for more complete usage examples.
