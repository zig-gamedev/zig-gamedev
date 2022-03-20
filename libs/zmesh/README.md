# zmesh - Zig bindings for par_shapes 

## Getting started

Copy `zmesh` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
pub fn build(b: *std.build.Builder) void {
    ...
    const zmesh_pkg = std.build.Pkg{
        .name = "zmesh",
        .path = .{ .path = "libs/zmesh/src/zmesh.zig" },
    };
    exe.addPackage(zmesh_pkg);
    @import("libs/zmesh/build.zig").link(b, exe);
}
```

Now in your code you may import and use zmesh:

```zig
const zmesh = @import("zmesh");

pub fn main() !void {
    ...
    const mesh = try zmesh.initCylinder(10, 10);
    mesh.saveToObj("cylinder.obj");
}
```
