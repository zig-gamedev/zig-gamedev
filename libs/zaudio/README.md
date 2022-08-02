# zaudio v0.2 - Cross-platform audio

Zig friendly bindings for great [miniaudio](https://github.com/mackron/miniaudio) library. Tested on Windows, Linux and macOS but should also work on mobile/web platforms.

As an example program please see [audio experiments (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/audio_experiments_wgpu).

## Features

Provided structs:

- [x] `Device`
- [x] `Engine`
- [x] `Sound`
- [x] `SoundGroup`
- [x] `NodeGraph`
- [x] `Fence`
- [ ] `Context` (missing methods)
- [ ] `ResourceManager` (missing methods)
- [ ] `Log` (missing methods)
- [ ] `DataSource` (missing methods)
  - [x] `WaveformDataSource`
  - [x] `NoiseDataSource`
  - [ ] custom data sources
- [x] `Node`
  - [x] `DataSourceNode`
  - [x] `SplitterNode`
  - [x] `BiquadNode`
  - [x] `LpfNode`
  - [x] `HpfNode`
  - [x] `NotchNode`
  - [x] `PeakNode`
  - [x] `LoshelfNode`
  - [x] `HishelfNode`
  - [x] `DelayNode`
  - [ ] custom nodes

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
    const engine = try zaudio.createEngine(allocator, null);

    const music = try engine.createSoundFromFile(
        allocator,
        content_dir ++ "Broke For Free - Night Owl.mp3",
        .{ .flags = .{ .stream = true } },
    );
    try music.start();
    ...
}
```
