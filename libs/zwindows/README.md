# zwindows - Windows development SDK for Zig game developers

- Vendored DirectX Compiler binaries for Windows and Linux
- Vendored DirectX and DirectML runtime libraries
- Lightweight partial bindings for:
    * Win32 API (extends std.os.windows)
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

Copy `zwindows` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zwindows = .{ .path = "libs/zwindows" },
```

Then in your `build.zig` add:

```zig
pub fn build(b: *std.Build) !void {
    const exe = b.addExecutable(.{ ... });

    const zwindows = b.dependency("zwindows", .{});
    const zwindows_path = windows.path("").getPath(b);
    
    exe.root_module.addImport("windows", zwindows.module("bindings"));
    
    try @import("zwindows").install_xaudio2(&tests.step, .bin, zwindows_path);

    try @import("zwindows").install_d3d12(&tests.step, .bin, zwindows_path);

    try @import("zwindows").install_directml(&tests.step, .bin, zwindows_path);
}
```

Now in your code you may import and use `zwindows`:

```zig
const windows = @import("windows");
const dwrite = windows.dwrite;
const dxgi = windows.dxgi;
const d3d12 = windows.d3d12;
const d3d12d = windows.d3d12d;
const dml = windows.directml;

pub fn main() !void {
    ...
    const winclass = windows.WNDCLASSEXA{
        .style = 0,
        .lpfnWndProc = processWindowMessage,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = @ptrCast(windows.HINSTANCE, windows.GetModuleHandleA(null)),
        .hIcon = null,
        .hCursor = windows.LoadCursorA(null, @intToPtr(windows.LPCSTR, 32512)),
        .hbrBackground = null,
        .lpszMenuName = null,
        .lpszClassName = name,
        .hIconSm = null,
    };
    _ = windows.RegisterClassExA(&winclass);
}
```
