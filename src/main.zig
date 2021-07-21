const builtin = @import("builtin");
const std = @import("std");
const w = struct {
    usingnamespace std.os.windows;
    usingnamespace @import("windows/windows.zig");
    usingnamespace @import("windows/d3d12.zig");
    usingnamespace @import("windows/d3d12sdklayers.zig");
    usingnamespace @import("windows/d3dcommon.zig");
    usingnamespace @import("windows/dxgi.zig");
    usingnamespace @import("windows/dxgi1_2.zig");
    usingnamespace @import("windows/dxgi1_4.zig");
};

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*c]const u8 = ".\\D3D12\\";

fn processWindowMessage(
    window: w.HWND,
    message: w.UINT,
    wparam: w.WPARAM,
    lparam: w.LPARAM,
) callconv(w.WINAPI) w.LRESULT {
    const processed = switch (message) {
        w.user32.WM_DESTROY => blk: {
            w.user32.PostQuitMessage(0);
            break :blk true;
        },
        w.user32.WM_KEYDOWN => blk: {
            if (wparam == w.VK_ESCAPE) {
                w.user32.PostQuitMessage(0);
                break :blk true;
            }
            break :blk false;
        },
        else => false,
    };
    return if (processed) 0 else w.user32.DefWindowProcA(window, message, wparam, lparam);
}

const window_name = "zig d3d12 test";
const window_width = 800;
const window_height = 800;

fn createWindow() !w.HWND {
    const winclass = w.user32.WNDCLASSEXA{
        .style = 0,
        .lpfnWndProc = processWindowMessage,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = @ptrCast(w.HINSTANCE, w.kernel32.GetModuleHandleW(null)),
        .hIcon = null,
        .hCursor = w.LoadCursorA(null, @intToPtr(w.LPCSTR, 32512)),
        .hbrBackground = null,
        .lpszMenuName = null,
        .lpszClassName = window_name,
        .hIconSm = null,
    };
    _ = try w.user32.registerClassExA(&winclass);

    const style = w.user32.WS_OVERLAPPED +
        w.user32.WS_SYSMENU +
        w.user32.WS_CAPTION +
        w.user32.WS_MINIMIZEBOX;

    var rect = w.RECT{ .left = 0, .top = 0, .right = window_width, .bottom = window_height };
    try w.user32.adjustWindowRectEx(&rect, style, false, 0);

    return try w.user32.createWindowExA(
        0,
        window_name,
        window_name,
        style + w.WS_VISIBLE,
        -1,
        -1,
        rect.right - rect.left,
        rect.bottom - rect.top,
        null,
        null,
        winclass.hInstance,
        null,
    );
}

pub fn main() !void {
    _ = w.SetProcessDPIAware();

    try w.dxgi_load_dll();
    try w.d3d12_load_dll();

    const factory = blk: {
        var maybe_factory: ?*w.IDXGIFactory1 = null;
        _ = w.CreateDXGIFactory2(1, &w.IID_IDXGIFactory1, @ptrCast(*?*c_void, &maybe_factory));
        break :blk maybe_factory.?;
    };

    const debug = blk: {
        var maybe_debug: ?*w.ID3D12Debug1 = null;
        _ = w.D3D12GetDebugInterface(&w.IID_ID3D12Debug1, @ptrCast(*?*c_void, &maybe_debug));
        break :blk maybe_debug.?;
    };
    debug.EnableDebugLayer();
    debug.SetEnableGPUBasedValidation(w.TRUE);
    _ = debug.Release();

    const device = blk: {
        var maybe_device: ?*w.ID3D12Device = null;
        _ = w.D3D12CreateDevice(null, ._11_1, &w.IID_ID3D12Device, @ptrCast(*?*c_void, &maybe_device));
        break :blk maybe_device.?;
    };
    defer _ = device.Release();

    const cmdqueue = blk: {
        var maybe_cmdqueue: ?*w.ID3D12CommandQueue = null;
        _ = device.CreateCommandQueue(&.{
            .Type = .DIRECT,
            .Priority = @enumToInt(w.D3D12_COMMAND_QUEUE_PRIORITY.NORMAL),
            .Flags = .{},
            .NodeMask = 0,
        }, &w.IID_ID3D12CommandQueue, @ptrCast(*?*c_void, &maybe_cmdqueue));
        break :blk maybe_cmdqueue.?;
    };
    defer _ = cmdqueue.Release();

    const window = try createWindow();

    const swapchain = blk: {
        var maybe_swapchain: ?*w.IDXGISwapChain = null;
        _ = factory.CreateSwapChain(
            @ptrCast(*w.IUnknown, cmdqueue),
            &w.DXGI_SWAP_CHAIN_DESC{
                .BufferDesc = .{
                    .Width = window_width,
                    .Height = window_height,
                    .RefreshRate = .{
                        .Numerator = 0,
                        .Denominator = 0,
                    },
                    .Format = .R8G8B8A8_UNORM,
                    .ScanlineOrdering = .UNSPECIFIED,
                    .Scaling = .UNSPECIFIED,
                },
                .SampleDesc = .{
                    .Count = 1,
                    .Quality = 0,
                },
                .BufferUsage = .{ .RENDER_TARGET_OUTPUT = true },
                .BufferCount = 4,
                .OutputWindow = window,
                .Windowed = w.TRUE,
                .SwapEffect = .FLIP_DISCARD,
                .Flags = .{},
            },
            &maybe_swapchain,
        );
        defer _ = maybe_swapchain.?.Release();
        var maybe_swapchain3: ?*w.IDXGISwapChain3 = null;
        _ = maybe_swapchain.?.QueryInterface(&w.IID_IDXGISwapChain3, @ptrCast(*?*c_void, &maybe_swapchain3));
        break :blk maybe_swapchain3.?;
    };
    _ = factory.Release();
    defer _ = swapchain.Release();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa.deinit();
        std.debug.assert(leaked == false);
    }

    while (true) {
        var message = std.mem.zeroes(w.user32.MSG);
        if (w.user32.PeekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) > 0) {
            _ = w.user32.DispatchMessageA(&message);
            if (message.message == w.user32.WM_QUIT)
                break;
        } else {}
    }

    std.debug.print("All OK!\n", .{});
}
