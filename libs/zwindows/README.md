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
- Optional D3D12 helper library (zd3d12)
- Optional XAudio2 helper library (zxaudio2)

## Getting started

Copy `zwindows` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zwindows = .{ .path = "libs/zwindows" },
```

### Using the zwindows build package
```zig
pub fn build(b: *std.Build) !void {

    ...

    const zwindows_dependency = b.dependency("zwindows", .{
        .zxaudio2_debug_layer = (builtin.mode == .Debug),
        .zd3d12_debug_layer = (builtin.mode == .Debug),
        .zd3d12_gbv = b.option("zd3d12_gbv", "Enable GPU-Based Validation") orelse false,
    });
    
    // Import the Windows API bindings
    exe.root_module.addImport("zwindows", zwindows_dependency.module("zwindows"));

    // Import the optional zd3d12 helper library
    exe.root_module.addImport("zd3d12", zwindows_dependency.module("zd3d12"));

    // Import the optional zxaudio2 helper library
    exe.root_module.addImport("zxaudio2", zwindows_dependency.module("zxaudio2"));
    
    // Install vendored binaries
    const zwindows = @import("zwindows");
    try zwindows.install_xaudio2(&exe.step, .bin);
    try zwindows.install_d3d12(&exe.step, .bin);
    try zwindows.install_directml(&exe.step, .bin);
}
```

### Importing and using the bindings
```zig
const zwindows = @import("zwindows");
const windows = zwindows.windows;
const dwrite = zwindows.dwrite;
const dxgi = zwindows.dxgi;
const d3d12 = zwindows.d3d12;
const d3d12d = zwindows.d3d12d;
const dml = zwindows.directml;
// etc

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
