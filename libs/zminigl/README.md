# zminigl - Zig OpenGL bindings (very custom and experimenal)

## Getting started

Copy `zminigl` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
pub fn build(b: *std.build.Builder) void {
    ...
    const zmini_pkg = std.build.Pkg{
        .name = "znoise",
        .path = .{ .path = "libs/zminigl/src/zminigl.zig" },
    };
    exe.addPackage(zminigl_pkg);
}
```

Now in your code you may import and use zminigl:

```zig
const zgl = @import("zminigl");

pub fn main() !void {
    ...
    zgl.init(glfw.getProcAddress);

    ...
    zgl.clearNamedFramebufferfv(zgl.default_framebuffer, .color, 0, &.{ 0.0, 0.6, 0.0, 1.0 });
}
```
