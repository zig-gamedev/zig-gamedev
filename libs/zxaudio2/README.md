# zxaudio2 - helper library for working with XAudio2

## Getting started

Copy `zxaudio2` and `zwin32` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zwin32 = @import("libs/zwin32/build.zig");
const zxaudio2 = @import("libs/zxaudio2/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    const enable_dx_debug = b.option( bool, "enable-dx-debug", "Enable debug layer for XAudio2") orelse false;

    const exe_options = b.addOptions();
    exe_options.addOption(bool, "enable_dx_debug", enable_dx_debug);
    exe.addOptions("build_options", exe_options);

    const options_pkg = exe_options.getPackage("build_options");
    exe.addPackage(zwin32.pkg);
    exe.addPackage(zxaudio2.getPackage(b, options_pkg));

    zxaudio2.link(exe, enable_dx_debug);
}
```

Now in your code you may import and use zxaudio2:

```zig
const zxaudio2 = @import("zxaudio2");

pub fn main() !void {
    ...
    var actx = zxaudio2.AudioContext.init(allocator);

    const sound_handle = actx.loadSound(L("content/drum_bass_hard.flac"));
    actx.playSound(sound_handle, .{});

    var music = zxaudio2.Stream.create(allocator, actx.device, L("content/Broke For Free - Night Owl.mp3"));
    hrPanicOnFail(music.voice.Start(0, xaudio2.COMMIT_NOW));
    ...
}
```
