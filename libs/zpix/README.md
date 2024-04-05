# zpix v0.10.0 - performance markers for PIX

## Getting started

Copy `zpix` and `zwin32` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zpix = .{ .path = "libs/zpix" },
    .zwin32 = .{ .path = "libs/zwin32" },
```

Then in your `build.zig` add:

```zig
pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    const zpix = b.dependency("zpix", .{
        .enable = true,
    });
    exe.root_module.addImport("zpix", zpix.module("root"));
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
