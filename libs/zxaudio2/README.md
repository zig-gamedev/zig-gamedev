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

    const options_pkg = Pkg{
        .name = "build_options",
        .path = exe_options.getSource(),
    };

    const zwin32_pkg = Pkg{
        .name = "zwin32",
        .path = .{ .path = "libs/zwin32/zwin32.zig" },
    };
    exe.addPackage(zwin32_pkg);

    const zxaudio2_pkg = std.build.Pkg{
        .name = "zxaudio2",
        .path = .{ .path = "libs/zxaudio2/zxaudio2.zig" },
        .dependencies = &[_]Pkg{
            zwin32_pkg,
            options_pkg,
        },
    };
    exe.addPackage(zxaudio2_pkg);
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

Note that you also need to ship `xaudio2_9redist.dll` file in `d3d12` folder that is placed next to your application executable. Directory structue should look like this:

```
my-game\
  d3d12\
    xaudio2_9redist.dll
  my-game.exe
```

You can use below code in your `build.zig` to copy the DLLs:

```zig
    // Copy DLLs
    if (enable_dx_debug) {
        b.installFile("../../external/bin/d3d12/xaudio2_9redist_debug.dll", "bin/d3d12/xaudio2_9redist.dll");
    } else {
        b.installFile("../../external/bin/d3d12/xaudio2_9redist.dll", "bin/d3d12/xaudio2_9redist.dll");
    }
    // Copy `content` folder
    const install_content_step = b.addInstallDirectory(
        .{ .source_dir = "content", .install_dir = .{ .custom = "" }, .install_subdir = "bin/content" },
    );
    b.getInstallStep().dependOn(&install_content_step.step);
```
