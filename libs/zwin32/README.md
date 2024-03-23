# zwin32 v0.10.0 - Zig bindings for Win32 API

Can be used on Windows and Linux. Contains partial bindings for:
* Win32 API
* Direct3D 12
* Direct3D 11
* DXGI
* DirectML
* Direct2D
* XAudio2
* Wincodec (WIC)
* WASAPI
* Media Foundation
* DirectWrite

## Getting started

Copy `zwin32` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zwin32 = .{ .path = "libs/zwin32" },
```

Then in your `build.zig` add:

```zig
pub fn build(b: *std.Build) !void {
    const exe = b.addExecutable(.{ ... });

    const zwin32 = b.dependency("zwin32", .{});
    const zwin32_path = zwin32.path("").getPath(b);
    
    exe.root_module.addImport("zwin32", zwin32.module("root"));
    
    try @import("zwin32").install_xaudio2(&tests.step, .bin, zwin32_path);

    try @import("zwin32").install_d3d12(&tests.step, .bin, zwin32_path);

    try @import("zwin32").install_directml(&tests.step, .bin, zwin32_path);
}
```

Now in your code you may import and use `zwin32`:

```zig
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const dwrite = zwin32.dwrite;
const dxgi = zwin32.dxgi;
const d3d12 = zwin32.d3d12;
const d3d12d = zwin32.d3d12d;
const dml = zwin32.directml;

pub fn main() !void {
    ...
    const winclass = w32.WNDCLASSEXA{
        .style = 0,
        .lpfnWndProc = processWindowMessage,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = @ptrCast(w32.HINSTANCE, w32.GetModuleHandleA(null)),
        .hIcon = null,
        .hCursor = w32.LoadCursorA(null, @intToPtr(w32.LPCSTR, 32512)),
        .hbrBackground = null,
        .lpszMenuName = null,
        .lpszClassName = name,
        .hIconSm = null,
    };
    _ = w32.RegisterClassExA(&winclass);
}
```
