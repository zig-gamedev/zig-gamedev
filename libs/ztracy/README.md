# ztracy - performance markers for Tracy 0.8.1

Zig bindings taken from: https://github.com/SpexGuy/Zig-Tracy

## Getting started

Copy `ztracy` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const ztracy = @import("libs/ztracy/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    const ztracy_enable = builder.option(bool, "ztracy-enable", "Enable Tracy profiler") orelse false;

    const ztracy_options = ztracy.BuildOptionsStep.init(builder, .{ .enable_ztracy = ztracy_enable });

    const ztracy_pkg = ztracy.getPkg(&.{ztracy_options.getPkg()});

    exe.addPackage(ztracy_pkg);

    ztracy.link(exe, ztracy_options);
}
```

Now in your code you may import and use `ztracy`. To build your project with Tracy enabled run:

`zig build -Dztracy-enable=true`

```zig
const ztracy = @import("ztracy");

pub fn main() !void {
    {
        const tracy_zone = ztracy.ZoneNC(@src(), "Compute Magic", 0x00_ff_00_00);
        defer tracy_zone.End();
        ...
    }
}
```

## Async "Fibers" support

Tracy v0.8.0 added support for marking fibers (also called green threads,
coroutines, and other forms of cooperative multitasking). This support requires
an additional option passed through when compiling the Tracy library, so change
the `link()` call in your `build.zig` to:

```zig
const ztracy_options = ztracy.BuildOptionsStep.init(
    builder,
    .{ .enable_ztracy = true, .enable_fibers = true },
);

const ztracy_pkg = ztracy.getPkg(&.{ztracy_options.getPkg()});

exe.addPackage(ztracy_pkg);

ztracy.link(exe, ztracy_options);
```
