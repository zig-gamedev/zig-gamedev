# zwin32 - Zig bindings for Win32 API

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

Copy `zwin32` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
const std = @import("std");
const zwin32 = @import("libs/zwin32/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    exe.addPackage(zwin32.pkg);
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
