const std = @import("std");

const zwindows = @import("zwindows");
const windows = zwindows.windows;
const dxgi = zwindows.dxgi;
const d3d12 = zwindows.d3d12;
const hrPanicOnFail = zwindows.hrPanicOnFail;

const zgui = @import("zgui");

const zd3d12 = @import("zd3d12");

pub export const D3D12SDKVersion: u32 = 610;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: minimal zgui win32 d3d12";

pub fn main() !void {
    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.posix.chdir(path) catch {};
    }

    _ = windows.CoInitializeEx(null, windows.COINIT_MULTITHREADED);
    defer windows.CoUninitialize();

    _ = windows.SetProcessDPIAware();

    var gpa_state = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa_state.deinit();
    const allocator = gpa_state.allocator();

    const window = createWindow(1600, 1200);

    var gctx = zd3d12.GraphicsContext.init(.{
        .allocator = allocator,
        .window = window,
    });
    defer gctx.deinit(allocator);

    zgui.init(allocator);
    defer zgui.deinit();

    _ = zgui.io.addFontFromFile(
        content_dir ++ "Roboto-Medium.ttf",
        std.math.floor(16.0),
    );

    const cbv_srv = gctx.cbv_srv_uav_gpu_heaps[0];
    zgui.backend.init(
        window,
        gctx.device,
        zd3d12.GraphicsContext.max_num_buffered_frames,
        @intFromEnum(dxgi.FORMAT.R8G8B8A8_UNORM),
        cbv_srv.heap.?,
        @bitCast(cbv_srv.base.cpu_handle),
        @bitCast(cbv_srv.base.gpu_handle),
    );
    defer zgui.backend.deinit();

    mainLoop: while (true) {
        var message = std.mem.zeroes(windows.MSG);
        while (windows.PeekMessageA(&message, null, 0, 0, windows.PM_REMOVE) == windows.TRUE) {
            _ = windows.TranslateMessage(&message);
            _ = windows.DispatchMessageA(&message);
            if (message.message == windows.WM_QUIT) {
                break :mainLoop;
            }
        }

        gctx.beginFrame();

        const back_buffer = gctx.getBackBuffer();
        gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
        gctx.flushResourceBarriers();

        gctx.cmdlist.OMSetRenderTargets(
            1,
            &.{back_buffer.descriptor_handle},
            windows.TRUE,
            null,
        );
        gctx.cmdlist.ClearRenderTargetView(back_buffer.descriptor_handle, &.{ 0.2, 0.4, 0.8, 1.0 }, 0, null);

        zgui.backend.newFrame(gctx.viewport_width, gctx.viewport_height);

        zgui.setNextWindowPos(.{ .x = 20.0, .y = 20.0, .cond = .first_use_ever });
        zgui.setNextWindowSize(.{ .w = -1.0, .h = -1.0, .cond = .first_use_ever });

        if (zgui.begin("My window", .{})) {
            if (zgui.button("Press me!", .{ .w = 200.0 })) {
                std.debug.print("Button pressed\n", .{});
            }
        }
        zgui.end();

        zgui.backend.draw(gctx.cmdlist);

        gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
        gctx.flushResourceBarriers();

        gctx.endFrame();
    }
}

fn processWindowMessage(
    window: windows.HWND,
    message: windows.UINT,
    wparam: windows.WPARAM,
    lparam: windows.LPARAM,
) callconv(windows.WINAPI) windows.LRESULT {
    switch (message) {
        windows.WM_KEYDOWN => {
            if (wparam == windows.VK_ESCAPE) {
                windows.PostQuitMessage(0);
                return 0;
            }
        },
        windows.WM_GETMINMAXINFO => {
            var info: *windows.MINMAXINFO = @ptrFromInt(@as(usize, @intCast(lparam)));
            info.ptMinTrackSize.x = 400;
            info.ptMinTrackSize.y = 400;
            return 0;
        },
        windows.WM_DESTROY => {
            windows.PostQuitMessage(0);
            return 0;
        },
        else => {},
    }
    return windows.DefWindowProcA(window, message, wparam, lparam);
}

fn createWindow(width: u32, height: u32) windows.HWND {
    const winclass = windows.WNDCLASSEXA{
        .style = 0,
        .lpfnWndProc = processWindowMessage,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = @ptrCast(windows.GetModuleHandleA(null)),
        .hIcon = null,
        .hCursor = windows.LoadCursorA(null, @ptrFromInt(32512)),
        .hbrBackground = null,
        .lpszMenuName = null,
        .lpszClassName = window_title,
        .hIconSm = null,
    };
    _ = windows.RegisterClassExA(&winclass);

    const style = windows.WS_OVERLAPPEDWINDOW;

    var rect = windows.RECT{
        .left = 0,
        .top = 0,
        .right = @intCast(width),
        .bottom = @intCast(height),
    };
    _ = windows.AdjustWindowRectEx(&rect, style, windows.FALSE, 0);

    const window = windows.CreateWindowExA(
        0,
        window_title,
        window_title,
        style + windows.WS_VISIBLE,
        windows.CW_USEDEFAULT,
        windows.CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        null,
        null,
        winclass.hInstance,
        null,
    ).?;

    return window;
}
