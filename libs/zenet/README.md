# zenet - Zig bindings for ENet

Bindings developed by Martin Wickham: https://github.com/SpexGuy/Zig-ENet

For test client/server application see: https://github.com/michal-z/zig-gamedev/tree/main/samples/network_test

## Getting started

Copy `zenet` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
pub fn build(b: *std.build.Builder) void {
    ...
    const zenet_pkg = std.build.Pkg{
        .name = "zenet",
        .path = .{ .path = "libs/zenet/src/zenet.zig" },
    };
    exe.addPackage(zenet_pkg);
    @import("libs/zenet/build.zig").link(b, exe);
}
```

Now in your code you may import and use znoise:

```zig
const zenet = @import("zenet");

pub fn main() !void {
    try zenet.initialize();
    defer zenet.deinitialize();
}
```
