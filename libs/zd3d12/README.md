## Getting started

Copy `zd3d12` and `zwin32` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
pub fn build(b: *std.build.Builder) void {
    ...
    const enable_dx_debug = b.option(
        bool,
        "enable-dx-debug",
        "Enable debug layer for D3D12, D2D1, and DXGI",
    ) orelse false;
    const enable_dx_gpu_debug = b.option(
        bool,
        "enable-dx-gpu-debug",
        "Enable GPU-based validation for D3D12",
    ) orelse false;

    const exe_options = b.addOptions();
    exe_options.addOption(bool, "enable_dx_debug", enable_dx_debug);
    exe_options.addOption(bool, "enable_dx_gpu_debug", enable_dx_gpu_debug);
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

    const zd3d12_pkg = std.build.Pkg{
        .name = "zd3d12",
        .path = .{ .path = "libs/zd3d12/zd3d12.zig" },
        .dependencies = &[_]Pkg{
            zwin32_pkg,
            options_pkg,
        },
    };
    exe.addPackage(zd3d12_pkg);
}
```

Now in your code you may import and use zd3d12:

```zig
const zd3d12 = @import("zd3d12");

pub fn main() !void {
    ...
    var gctx = zd3d12.GraphicsContext.init(window);
    gctx.present_flags = 0;
    gctx.present_interval = 1;
    ...
}
```
