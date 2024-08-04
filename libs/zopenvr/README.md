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
    exe.root_module.addImport("zopenvr", zopenvr.module("root"));

    @import("zopenvr").addLibraryPathsTo(exe);
    @import("zopenvr").linkOpenVR(exe);
    @import("zopenvr").installOpenVR(&exe.step, exe.rootModuleTarget(), .bin);
}
```
<!-- @import("zopenvr").addRPathsTo(exe); -->

Now in your code you may import and use `zopenvr`:

```zig
const std = @import("std");
const OpenVR = @import("zopenvr");

pub fn main() !void {
    ...

    const openvr = try OpenVR.init(.scene);
    defer openvr.deinit();

    const system = try openvr.system();

    const name = try system.allocTrackedDevicePropertyString(allocator, OpenVR.hmd, .tracking_system_name);
    defer allocator.free(name);

    ...
}
```

For better types on render structs, enable the corresponding options when importing the dependency and ensure that the lib is present in the libs folder.
```zig
    const zopenvr = b.dependency("zopenvr", .{
        .d3d11 = true,    // requires zwin32
        .d3d12 = true,    // requires zwin32
    });
```

## Implementation progress

| Interface       |       Status       |
| --------------- | :----------------: |
| Applications    |         ✅         |
| BlockQueue      |                    |
| Chaperone       |         ✅         |
| ChaperoneSetup  |                    |
| Compositor      | ✅<br/>(see below) |
| Debug           |                    |
| DriverManager   |                    |
| ExtendedDisplay |                    |
| HeadsetView     |                    |
| Input           |         ✅         |
| IOBuffer        |                    |
| Notifications   |                    |
| Overlay         |         ✅         |
| OverlayView     |         ✅         |
| Paths           |                    |
| Properties      |                    |
| RenderModels    |         ✅         |
| Resources       |                    |
| Screenshots     |                    |
| Settings        |                    |
| SpatialAnchors  |                    |
| System          |         ✅         |
| TrackedCamera   |                    |

### Compositor supported renderers
| Renderer           | Handle type           | Zig handle name                 | Support |
|--------------------|-----------------------|---------------------------------|:-------:|
| DirectX 11 (d3d11) | ID3D11Texture2D       | zwin32.d3d11.ITexture2D         |    ✅   |
| OpenGL             | GLUint \| buffer name | zopengl.bindings.Uint           |    ✅   |
| Vulkan             | VRVulkanTextureData_t |                                 |         |
| IOSurface          |                       |                                 |         |
| DirectX 12 (d3d12) | D3D12TextureData_t    | zopenvr.common.D3D12TextureData |    ✅   |
| DXGI               |                       |                                 |         |
| Metal              |                       |                                 |         |

## todo
- generate bindings from original json
- maybe get doc comments from openvr.h
