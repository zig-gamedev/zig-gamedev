# zwin32 - standalone Zig bindings for Win32 API

## Getting started

Copy `zwin32` folder to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:

```zig
pub fn build(b: *std.build.Builder) void {
    ...
    const zwin32_pkg = std.build.Pkg{
        .name = "zwin32",
        .path = .{ .path = "libs/zwin32/zwin32.zig" },
    };
    exe.addPackage(zwin32_pkg);
}
```

Now in your code you may import and use zwin32:

```zig
const zwin32 = @import("zwin32");
const w32 = zwin32.base;
const dwrite = zwin32.dwrite;
const dxgi = zwin32.dxgi;
const d3d12 = zwin32.d3d12;
const d3d12d = zwin32.d3d12d;
const dml = zwin32.directml;

pub fn main() !void {
    ...
    const winclass = w32.user32.WNDCLASSEXA{
        .style = 0,
        .lpfnWndProc = processWindowMessage,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = @ptrCast(w32.HINSTANCE, w32.kernel32.GetModuleHandleW(null)),
        .hIcon = null,
        .hCursor = w.LoadCursorA(null, @intToPtr(w32.LPCSTR, 32512)),
        .hbrBackground = null,
        .lpszMenuName = null,
        .lpszClassName = name,
        .hIconSm = null,
    };
    _ = try w32.user32.registerClassExA(&winclass);
}
```
