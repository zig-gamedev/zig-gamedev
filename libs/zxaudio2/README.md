# zxaudio2 - helper library for XAudio2

## Getting started

Copy `zxaudio2` and `zwin32` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zwin32 = @import("libs/zwin32/build.zig");
const zxaudio2 = @import("libs/zxaudio2/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    const zxaudio2_options = zxaudio2.BuildOptionsStep.init(b, .{
        .enable_debug_layer = false,
    });
    const zxaudio2_pkg = zxaudio2.getPkg(&.{ zwin32.pkg, zxaudio2_options.getPkg() });

    exe.addPackage(zwin32.pkg);
    exe.addPackage(zxaudio2_pkg);

    zxaudio2.link(exe, zxaudio2_options);
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
