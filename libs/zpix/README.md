# zpix - performance markers for PIX

## Getting started

Copy `zpix` and `zwin32` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zwin32 = @import("libs/zwin32/build.zig");
const zpix = @import("libs/zpix/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    const zpix_enable = b.option(bool, "zpix-enable", "Enable PIX GPU events and markers") orelse false;

    const exe_options = b.addOptions();
    exe.addOptions("build_options", exe_options);
    exe_options.addOption(bool, "zpix_enable", zpix_enable);

    const options_pkg = exe_options.getPackage("build_options");
    const zpix_pkg = zpix.getPkg(&.{ options_pkg, zwin32.pkg });

    exe.addPackage(zwin32.pkg);
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
        zpix.beginEvent(gctx.cmdlist, "Z Pre Pass");
        defer zpix.endEvent(gctx.cmdlist);
        ...
    }
}
```
