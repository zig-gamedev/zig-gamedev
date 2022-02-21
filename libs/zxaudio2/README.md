## Getting started

Copy `zxaudio2` and `zwin32` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
pub fn build(b: *std.build.Builder) void {
    ...
    const enable_dx_debug = b.option( bool, "enable-dx-debug", "Enable debug layer for XAudio2") orelse false;

    const exe_options = b.addOptions();
    exe_options.addOption(bool, "enable_dx_debug", enable_dx_debug);

    exe.addOptions("build_options", exe_options);

    const options_pkg = std.build.Pkg{
        .name = "build_options",
        .path = exe_options.getSource(),
    };

    const zwin32_pkg = std.build.Pkg{
        .name = "zwin32",
        .path = .{ .path = "libs/zwin32/zwin32.zig" },
    };
    exe.addPackage(zwin32_pkg);

    const zxaudio2_pkg = std.build.Pkg{
        .name = "zxaudio2",
        .path = .{ .path = "libs/zxaudio2/zxaudio2.zig" },
        .dependencies = &[_]std.build.Pkg{
            zwin32_pkg,
            options_pkg,
        },
    };
    exe.addPackage(zxaudio2_pkg);
    @import("libs/zxaudio2/build.zig").link(b, exe, .{ .enable_debug_layer = enable_dx_debug });
}
```

Now in your code you may import and use zxaudio2:

```zig
const zxaudio2 = @import("zxaudio2");

pub fn main() !void {
    ...
    var actx = zxaudio2.AudioContext.init(gpa_allocator);

    const sound1_data = zxaudio2.loadBufferData(gpa_allocator, L("content/drum_bass_hard.flac"));
    const sound2_data = zxaudio2.loadBufferData(gpa_allocator, L("content/tabla_tas1.flac"));
    const sound3_data = zxaudio2.loadBufferData(gpa_allocator, L("content/loop_mika.flac"));

    var music = zxaudio2.Stream.create(gpa_allocator, actx.device, L("content/Broke For Free - Night Owl.mp3"));
    hrPanicOnFail(music.voice.Start(0, xaudio2.COMMIT_NOW));
    ...
}
```
