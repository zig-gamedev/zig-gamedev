# zpix - performance markers for PIX

## Getting started

Copy `zpix` and `zwin32` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zpix = @import("libs/zpix/build.zig");
const zwin32 = @import("libs/zwin32/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    const zpix_options = zpix.BuildOptionsStep.init(b, .{ .enable = true });
    const zpix_pkg = zpix.getPkg(&.{ zwin32.pkg, zpix_options.getPkg() });

    exe.addPackage(zpix_pkg);
    exe.addPackage(zwin32.pkg);

    zpix.link(exe, zpix_options);
}
```

Now in your code you may import and use `zpix`:

```zig
const std = @import("std");
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zpix = @import("zpix");

pub fn main() !void {
    ...
    _ = zpix.loadGpuCapturerLibrary();
    _ = zpix.setTargetWindow(window);
    _ = zpix.beginCapture(
        .{ .GPU = true },
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
