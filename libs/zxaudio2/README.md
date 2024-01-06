# zxaudio2 v0.9.0 - helper library for XAudio2

## Getting started

Copy `zxaudio2` and `zwin32` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zwin32 = @import("libs/zwin32/build.zig");
const zxaudio2 = @import("libs/zxaudio2/build.zig");

pub fn build(b: *std.Build) void {
    ...
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zwin32_pkg = zwin32.package(b, target, optimize, .{});
    const zxaudio2_pkg = zxaudio2.package(b, target, optimize, .{
        .options = .{ .enable_debug_layer = false },
        .deps = .{ .zwin32 = zwin32_pkg.zwin32},
    });

    zwin32_pkg.link(exe, .{ .xaudio2 = true });
    zxaudio2_pkg.link(exe);
}
```

Now in your code you may import and use zxaudio2:

```zig
const std = @import("std");
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zxaudio2 = @import("zxaudio2");

pub fn main() !void {
    ...
    var actx = zxaudio2.AudioContext.init(allocator);

    const sound_handle = actx.loadSound("content/drum_bass_hard.flac");
    actx.playSound(sound_handle, .{});

    var music = zxaudio2.Stream.create(allocator, actx.device, "content/Broke For Free - Night Owl.mp3");
    hrPanicOnFail(music.voice.Start(0, xaudio2.COMMIT_NOW));
    ...
}
```
