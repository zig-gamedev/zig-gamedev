# znetwork

This library uses [zig-network](https://github.com/MasterQ32/zig-network) developed by [MasterQ32](https://github.com/MasterQ32)

## Getting started

Copy `znetwork` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const znet = @import("libs/znetwork/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    exe.addPackage(znet.pkg);
}
```

Now in your code you may import and use `znetwork`:

```zig
const znet = @import("znetwork");

pub fn main() !void {
    ...
}
```
