# zopengl v0.4.3 - OpenGL loader

Supports:
  * OpenGL Core Profile up to version 4.2
  * OpenGL ES up to version 2.0

## Getting started

Copy `zopengl` folder to a `libs` subdirectory of the root of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zopengl = .{ .path = "libs/zopengl" },
```

Then in your `build.zig` add:

```zig
const std = @import("std");
const zopengl = @import("zopengl");

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
const zopengl = @import("zopengl");

pub fn main() !void {
    // Create window and OpenGL context here... (you can use our `zsdl` or `zglfw` libs for this)

    try zopengl.loadCoreProfile(getProcAddress, 4, 0);

    const gl = zopengl.bindings; // or zopengl.wrapper (experimental)

    gl.clearBufferfv(gl.COLOR, 0, &[_]f32{ 0.2, 0.4, 0.8, 1.0 });
}

fn getProcAddress(name: [:0]const u8) ?*const anyopaque {
    // Load GL function pointer here
    // You could use `zsdl.gl.getProcAddress() or `zglfw.getProcAddress()`
}
```
