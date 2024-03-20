# zopengl v0.5.0 - OpenGL loader, bindings and optional Zig wrapper

Supports:
  * OpenGL Core Profile up to version 4.2
  * OpenGL ES up to version 2.0

## Getting started

Copy `zopengl` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zopengl = .{ .path = "local/path/to/zopengl" },
```

Then in your `build.zig` add:

```zig
pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    const zopengl = b.dependency("zopengl", .{});
    exe.root_module.addImport("zopengl", zopengl.module("root"));
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
