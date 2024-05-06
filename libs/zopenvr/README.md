# zopenvr v0.0.1

(WIP)
Bindings for [OpenVR](https://github.com/ValveSoftware/openvr) v2.2.3

## Getting started

Copy `zopenvr` folder to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:

```zig
    .zopenvr = .{ .path = "path/to/local/zopenvr" },
```

Then in your `build.zig` add:

```zig
pub fn build(b: *std.Build) !void {
    const exe = b.addExecutable(.{ ... });

    const zopenvr = b.dependency("zopenvr", .{});
    const zopenvr_path = zopenvr.path("").getPath(b);

    exe.root_module.addImport("zopenvr", zopenvr.module("zopenvr"));

    try @import("zopenvr").addLibraryPathsTo(exe, zopenvr_path);
    @import("zopenvr").linkOpenvr(exe);
    try @import("zopenvr").installOpenvr(&exe.step, options.target.result, .bin, zopenvr_path);
}
```

Now in your code you may import and use `zopenvr`:

```zig
const std = @import("std");
const OpenVR = @import("zopenvr");

pub fn main() !void {
    ...

    const openvr = try OpenVR.init(.scene);
    defer openvr.deinit();

    const system = try openvr.system();

    const name = try app.system.allocTrackedDevicePropertyString(allocator, OpenVR.hmd, .tracking_system_name);
    defer allocator.free(name);

    ...
}
```

## Implementation progress

| Interface       |       Status        |
| --------------- | :-----------------: |
| Applications    |         ✅          |
| BlockQueue      |                     |
| Chaperone       |         ✅          |
| ChaperoneSetup  |                     |
| Compositor      | ✅<br/>(d3d12 only) |
| Debug           |                     |
| DriverManager   |                     |
| ExtendedDisplay |                     |
| HeadsetView     |                     |
| Input           |         ✅          |
| IOBuffer        |                     |
| Notifications   |                     |
| Overlay         |                     |
| OverlayView     |                     |
| Paths           |                     |
| Properties      |                     |
| RenderModels    |         ✅          |
| Resources       |                     |
| Screenshots     |                     |
| Settings        |                     |
| SpatialAnchors  |                     |
| System          |         ✅          |
| TrackedCamera   |                     |
