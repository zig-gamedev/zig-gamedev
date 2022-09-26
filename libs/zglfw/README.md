# zglfw v0.1 - GLFW bindings

## Getting started

Copy `zglfw` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:
```zig
const zglfw = @import("libs/zglfw/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    exe.addPackage(zglfw.pkg);

    zglfw.link(exe);
}
```
Now in your code you may import and use `zglfw`:
```zig
const zglfw = @import("zglfw");

pub fn main() !void {
    ...
}
```
