# zopengl - OpenGL loader

Supports:
  * OpenGL Core Profile up to version 4.0
  * OpenGL ES up to version 2.0

## Getting started

Copy `zopengl` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zopengl = @import("libs/zopengl/build.zig");

pub fn build(b: *std.Build) void {
    ...
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zopengl_pkg = zopengl.package(b, target, optimize, .{});

    zopengl_pkg.link(exe);
}
```

Now in your code you may import and use `zopengl`:

```zig
const gl = @import("zopengl");

pub fn main() !void {
    // Create window and OpenGL context here... (you can use our `zsdl` or `zglfw` libs for this)

    try gl.loadCoreProfile(getProcAddress, 4, 0);

    gl.clearBufferfv(gl.COLOR, 0, &[_]f32{ 0.2, 0.4, 0.8, 1.0 });
}

fn getProcAddress(name: [:0]const u8) ?*const anyopaque {
    // Load GL function pointer here
    // You could use `zsdl.gl.getProcAddress() or `zglfw.getProcAddress()`
}
```
