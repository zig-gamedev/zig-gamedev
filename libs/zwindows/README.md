# zwindows

Windows development SDK for Zig game developers.

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

## Using the Zig package

Copy `zwindows` to a subdirectory of your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zwindows = .{ .path = "libs/zwindows" },
```

Example build.zig
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

### Bindings Usage Example
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

## zd3d12
zd3d12 is an optional helper library for Direct3d 12 build ontop of the zwindows bindings

### Features
- Basic DirectX 12 context management (descriptor heaps, memory heaps, swapchain, CPU and GPU sync, etc.)
- Basic DirectX 12 resource management (handle-based resources and pipelines)
- Basic resource barriers management with simple state-tracking
- Transient and persistent descriptor allocation
- Fast image loading using WIC (Windows Imaging Component)
- Helpers for uploading data to the GPU
- Fast mipmap generator running on the GPU
- Interop with Direct2D and DirectWrite for high-quality vector graphics and text rendering (optional)

### Example applications
- https://github.com/zig-gamedev/zig-gamedev/tree/main/samples/triangle
- https://github.com/zig-gamedev/zig-gamedev/tree/main/samples/textured_quad
- https://github.com/zig-gamedev/zig-gamedev/tree/main/samples/vector_graphics_test
- https://github.com/zig-gamedev/zig-gamedev/blob/main/samples/rasterization
- https://github.com/zig-gamedev/zig-gamedev/tree/main/samples/bindless
- https://github.com/zig-gamedev/zig-gamedev/blob/main/samples/mesh_shader_test
- https://github.com/zig-gamedev/zig-gamedev/blob/main/samples/directml_convolution_test

### Usage Example
```zig
const zd3d12 = @import("zd3d12");

// We need to export below symbols for DirectX 12 Agility SDK.
pub export const D3D12SDKVersion: u32 = 610;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

pub fn main() !void {
    ...
    var gctx = zd3d12.GraphicsContext.init(.{
        .allocator = allocator, 
        .window = win32_window,
    });
    defer gctx.deinit(allocator);

    while (...) {
        gctx.beginFrame();

        const back_buffer = gctx.getBackBuffer();
        gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
        gctx.flushResourceBarriers();

        gctx.cmdlist.OMSetRenderTargets(
            1,
            &.{back_buffer.descriptor_handle},
            TRUE,
            null,
        );
        gctx.cmdlist.ClearRenderTargetView(back_buffer.descriptor_handle, &.{ 0.2, 0.4, 0.8, 1.0 }, 0, null);

        gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
        gctx.flushResourceBarriers();

        gctx.endFrame();
    }
}
```

## zxaudio2
zxaudio2 is an optional helper library for XAudio2 build ontop of the zwindows bindings

### Usage Example
```zig
const zxaudio2 = @import("zxaudio2");

pub fn main() !void {
    ...
    var actx = zxaudio2.AudioContext.init(allocator);

    const sound_handle = actx.loadSound("content/drum_bass_hard.flac");
    actx.playSound(sound_handle, .{});

    var music = zxaudio2.Stream.create(allocator, actx.device, "content/Broke For Free - Night Owl.mp3");
    hrPanicOnFail(music.voice.Start(0, xaudio2.COMMIT_NOW));
    ...
}
```
