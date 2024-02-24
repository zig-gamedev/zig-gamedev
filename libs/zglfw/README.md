# zglfw v0.8.0 - GLFW 3.4 build system & bindings

## Getting started

Copy `zglfw` and `system-sdk` folders to a `libs` subdirectory of the root of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .system_sdk = .{ .path = "libs/system-sdk" },
    .zglfw = .{ .path = "libs/zglfw" },
```

Then in your `build.zig` add:
```zig
const zglfw = @import("zglfw");

pub fn build(b: *std.Build) void {
    ...
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zglfw_pkg = zglfw.package(b, target, optimize, .{});

    zglfw_pkg.link(exe);
}
```
Now in your code you may import and use `zglfw`:
```zig
const zglfw = @import("zglfw");

pub fn main() !void {
    ...
}
```
