## Getting started

Copy `zpix` and `zwin32` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
pub fn build(b: *std.build.Builder) void {
    ...
    const enable_pix = b.option(bool, "enable-pix", "Enable PIX GPU events and markers") orelse false;

    const exe_options = b.addOptions();
    exe_options.addOption(bool, "enable_pix", enable_pix);
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

    const zpix_pkg = std.build.Pkg{
        .name = "zpix",
        .path = .{ .path = "libs/zpix/zpix.zig" },
        .dependencies = &[_]Pkg{
            zwin32_pkg,
            options_pkg,
        },
    };
    exe.addPackage(zpix_pkg);
}
```

Now in your code you may import and use zpix:

```zig
const zpix = @import("zpix");

pub fn main() !void {
    ...
    _ = zpix.loadGpuCapturerLibrary();
    _ = zpix.setTargetWindow(window);
    _ = zpix.beginCapture(
        zpix.CAPTURE_GPU,
        &zpix.CaptureParameters{ .gpu_capture_params = .{ .FileName = L("capture.wpix") } },
    );
    ...
    _ = zpix.endCapture();
    ...
    // Z Pre Pass.
    {
        ...
        zpix.beginEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, gctx.cmdlist), "Z Pre Pass");
        defer zpix.endEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, gctx.cmdlist));
        ...
    }
}
```
