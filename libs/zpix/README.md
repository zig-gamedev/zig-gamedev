# zpix - performance markers for PIX

## Getting started

Copy `zpix` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zpix = @import("libs/zpix/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    const zpix_enable = b.option(bool, "zpix-enable", "Enable PIX GPU events and markers") orelse false;

    const zpix_options = zpix.BuildOptionsStep.init(b, .{ .enable_zpix = options.zpix_enable });

    const zpix_pkg = zpix.getPkg(&.{zpix_options.getPkg()});

    exe.addPackage(zpix_pkg);

    zpix.link(exe, zpix_options);
}
```

Now in your code you may import and use `zpix`:

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
