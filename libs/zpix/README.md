# zpix

Performance markers for [Microsoft's PIX profiler](https://devblogs.microsoft.com/pix/documentation/)

## Getting started

Copy `zpix` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zpix = .{ .path = "libs/zpix" },
```

Then in your `build.zig` add:

```zig
pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    const zpix = b.dependency("zpix", .{
        .enable = true,
        .path = @as([]const u8, ...folder containing WinPixGpuCapturer.dll, typically directory under C:\Program Files\\Microsoft PIX),
    });
    exe.root_module.addImport("zpix", zpix.module("root"));
}
```

Load GPU capture library before making any D3D12 calls:

```zig
const zpix = @import("zpix");

pub fn main() !void {
    const pix_library = try zpix.loadGpuCapturerLibrary();
    defer pix_library.deinit();
    ...
}
```

Then using the PIX UI:
1. Under Select Target Process --> Attach
2. Select process
3. Select Attach
4. Under GPU Capture, click on camera icon

## Advanced usage
For [programmic capture](https://devblogs.microsoft.com/pix/programmatic-capture/) use `beginCapture`/`endCapture`.

If process has multiple windows, target one for GPU capture using `setTargetWindow`.

[Full PIX documentation](https://devblogs.microsoft.com/pix/documentation/)
