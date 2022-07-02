# zaudio v0.1 - Cross-platform audio

Zig friendly bindings for great [miniaudio](https://github.com/mackron/miniaudio) library. Tested on Windows, Linux and macOS but should also work on mobile/web platforms.

As an example program please see [audio experiments (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/audio_experiments_wgpu).

## Getting started

Copy `zaudio` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zaudio = @import("libs/zaudio/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    exe.addPackage(zaudio.pkg);

    zaudio.link(exe);
}
```

Now in your code you may import and use `zaudio`:

```zig
const zaudio = @import("zaudio");

pub fn main() !void {
    ...
    const engine =  try zaudio.Engine.init(allocator, null);

    const music = try zaudio.Sound.initFile(
        allocator,
        engine,
        "Broke For Free - Night Owl.mp3",
        .{ .flags = .{ .stream = true } },
    );
    try music.start();
    ...
}
```
