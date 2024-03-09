# zglfw v0.9.0 - GLFW 3.4 build system & bindings

## Getting started

Copy `zglfw` and `system-sdk` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zglfw = .{ .path = "libs/zglfw" },
    
    // Required for building glfw
    .system_sdk = .{ .path = "libs/system-sdk" },
```

Then in your `build.zig` add:
```zig
pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    const zglfw = b.dependency("zglfw", .{});
    exe.root_module.addImport("zglfw", zglfw.module("root"));
    exe.linkLibrary(zglfw.artifact("glfw"));

    @import("system_sdk").addLibraryPathsTo(exe);
}
```
Now in your code you may import and use `zglfw`:
```zig
const zglfw = @import("zglfw");

pub fn main() !void {
    ...
}
```
