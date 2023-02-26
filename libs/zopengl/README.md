# zopengl - OpenGL 3.3 (Core Profile) bindings

## Getting started

Copy `zopengl` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zopengl = @import("libs/zopengl/build.zig");

pub fn build(b: *std.Build) void {
    ...
    const zopengl_pkg = zopengl.Package.build(b, .{});

    exe.addModule("zopengl", zopengl_pkg.zopengl);
}
```

Now in your code you may import and use `zopengl`:

```zig
const gl = @import("zopengl");

pub fn main() !void {
    // Create window and OpenGL context here... (you can use our `zsdl` or `zglfw` libs for this)

    try gl.loadCoreProfile(getProcAddress, 3, 3);

    gl.clearBufferfv(gl.COLOR, 0, &[_]f32{ 0.2, 0.4, 0.8, 1.0 });
}
```
