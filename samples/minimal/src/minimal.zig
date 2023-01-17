const std = @import("std");
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const d3d12 = zwin32.d3d12;

pub export const D3D12SDKVersion: u32 = 608;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: minimal";
const window_width = 1600;
const window_height = 1200;

fn processWindowMessage(
    window: w32.HWND,
    message: w32.UINT,
    wparam: w32.WPARAM,
    lparam: w32.LPARAM,
) callconv(w32.WINAPI) w32.LRESULT {
    switch (message) {
        w32.WM_KEYDOWN => {
            if (wparam == w32.VK_ESCAPE) {
                w32.PostQuitMessage(0);
                return 0;
            }
        },
        w32.WM_DESTROY => {
            w32.PostQuitMessage(0);
            return 0;
        },
        else => {},
    }
    return w32.DefWindowProcA(window, message, wparam, lparam);
}

pub fn main() !void {
    _ = w32.CoInitializeEx(null, w32.COINIT_MULTITHREADED);
    defer w32.CoUninitialize();

    _ = w32.SetProcessDPIAware();

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
        .lpszClassName = window_name,
        .hIconSm = null,
    };
    _ = w32.RegisterClassExA(&winclass);

    const style = w32.WS_OVERLAPPED + w32.WS_SYSMENU + w32.WS_CAPTION + w32.WS_MINIMIZEBOX;
    var rect = w32.RECT{
        .left = 0,
        .top = 0,
        .right = @intCast(w32.LONG, window_width),
        .bottom = @intCast(w32.LONG, window_height),
    };
    _ = w32.AdjustWindowRectEx(&rect, style, w32.FALSE, 0);

    _ = w32.CreateWindowExA(
        0,
        window_name,
        window_name,
        style + w32.WS_VISIBLE,
        -1,
        -1,
        rect.right - rect.left,
        rect.bottom - rect.top,
        null,
        null,
        winclass.hInstance,
        null,
    ).?;

    main_loop: while (true) {
        var message = std.mem.zeroes(w32.MSG);
        while (w32.PeekMessageA(&message, null, 0, 0, w32.PM_REMOVE) == w32.TRUE) {
            _ = w32.TranslateMessage(&message);
            _ = w32.DispatchMessageA(&message);
            if (message.message == w32.WM_QUIT) {
                break :main_loop;
            }
        }
    }
}
