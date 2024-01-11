# zd3d12 v0.9.0 - helper library for DirectX 12

## Features

* Basic DirectX 12 context management (descriptor heaps, memory heaps, swapchain, CPU and GPU sync, etc.)
* Basic DirectX 12 resource management (handle-based resources and pipelines)
* Basic resource barriers management with simple state-tracking
* Transient and persistent descriptor allocation
* Fast image loading using WIC (Windows Imaging Component)
* Helpers for uploading data to the GPU
* Fast mipmap generator running on the GPU
* Interop with Direct2D and DirectWrite for high-quality vector graphics and text rendering (optional)

Example programs: https://github.com/michal-z/zig-gamedev/tree/main/samples/intro

## Getting started

Copy `zd3d12` and `zwin32` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zwin32 = @import("libs/zwin32/build.zig");
const zd3d12 = @import("libs/zd3d12/build.zig");

pub fn build(b: *std.Build) void {
    ...
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zwin32_pkg = zwin32.package(b, target, optimize, .{});
    const zd3d12_pkg = zd3d12.package(b, target, optimize, .{
        .options = .{
            .enable_debug_layer = false,
            .enable_gbv = false,
        },
        .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
    });

    zwin32_pkg.link(exe, .{ .d3d12 = true });
    zd3d12_pkg.link(exe);
}
```

Now in your code you may import and use zd3d12:

```zig
const zd3d12 = @import("zd3d12");

// We need to export below symbols for DirectX 12 Agility SDK.
pub export const D3D12SDKVersion: u32 = 610;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

pub fn main() !void {
    ...
    var gctx = zd3d12.GraphicsContext.init(allocator, window);
    defer gctx.deinit(allocator);

    while (...) {
        gctx.beginFrame();

        const back_buffer = gctx.getBackBuffer();
        gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
        gctx.flushResourceBarriers();

        gctx.cmdlist.OMSetRenderTargets(
            1,
            &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
            w32.TRUE,
            null,
        );
        gctx.cmdlist.ClearRenderTargetView(back_buffer.descriptor_handle, &.{ 0.2, 0.4, 0.8, 1.0 }, 0, null);

        gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
        gctx.flushResourceBarriers();

        gctx.endFrame();
    }
}
```
