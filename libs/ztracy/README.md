## Getting started

Copy `ztracy` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
pub fn build(b: *std.build.Builder) void {
    ...
    const tracy = b.option([]const u8, "tracy", "Enable Tracy profiler integration (supply path to Tracy source)");

    const exe_options = b.addOptions();
    exe_options.addOption(bool, "enable_tracy", tracy != null);
    exe.addOptions("build_options", exe_options);

    if (tracy) |tracy_path| {
        const client_cpp = std.fs.path.join(
            b.allocator,
            &[_][]const u8{ tracy_path, "TracyClient.cpp" },
        ) catch unreachable;
        exe.addIncludeDir(tracy_path);
        exe.addCSourceFile(client_cpp, &[_][]const u8{
            "-DTRACY_ENABLE=1",
            "-fno-sanitize=undefined",
            "-D_WIN32_WINNT=0x601",
        });
        exe.linkSystemLibrary("ws2_32");
        exe.linkSystemLibrary("dbghelp");
    }

    const options_pkg = Pkg{
        .name = "build_options",
        .path = exe_options.getSource(),
    };

    const ztracy_pkg = std.build.Pkg{
        .name = "ztracy",
        .path = .{ .path = "libs/ztracy/ztracy.zig" },
        .dependencies = &[_]Pkg{
            options_pkg,
        },
    };
    exe.addPackage(ztracy_pkg);
}
```

Now in your code you may import and use ztracy:

```zig
const ztracy = @import("ztracy");

pub fn main() !void {
    {
        const tracy_zone = ztracy.zoneNC(@src(), "Compute Magic", 0x00_ff_00_00, 1);
        defer tracy_zone.end();
        ...
    }
}
```
