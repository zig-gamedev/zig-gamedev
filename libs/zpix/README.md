# zpix v0.9.0 - performance markers for PIX

## Getting started

Copy `zpix` and `zwin32` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zpix = @import("libs/zpix/build.zig");
const zwin32 = @import("libs/zwin32/build.zig");

pub fn build(b: *std.Build) void {
    ...
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zwin32_pkg = zwin32.package(b, target, optimize, .{});
    const zpix_pkg = zpix.package(b, target, optimize, .{
        .options = .{ .enable = true },
        .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
    });

    zpix_pkg.link(exe);
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
