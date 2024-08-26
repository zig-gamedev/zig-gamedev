# zxaudio2 v0.10.0 - helper library for XAudio2

## Getting started

Copy `zxaudio2` and `zwindows` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zxaudio2 = .{ .path = "libs/zxaudio2" },
    .zwindows = .{ .path = "libs/zwindows" },
```

Then in your `build.zig` add:

```zig
pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    // Optionally install xaudio2 libs to zig-out/bin (or somewhere else)
    try @import("zwindows").install_xaudio2(&tests.step, .bin, windows.path("").getPath(b));

    const zxaudio2 = b.dependency("zxaudio2", .{});
    exe.root_module.addImport("zxaudio2", zxaudio2.module("root"));
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
