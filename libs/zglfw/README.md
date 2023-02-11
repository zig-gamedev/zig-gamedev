# zglfw v0.5.2 - GLFW bindings

## Getting started

Copy `zglfw` and `system-sdk` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:
```zig
const zglfw = @import("libs/zglfw/build.zig");

pub fn build(b: *std.Build) void {
    ...
    const zglfw_pkg = zglfw.Package.build(b, target, optimize, .{});

    exe.addModule("zglfw", zglfw_pkg.zglfw);

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
