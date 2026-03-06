const std = @import("std");

const zwindows = @import("zwindows");
const dxgi = zwindows.dxgi;
const d3d12 = zwindows.d3d12;
const hrPanicOnFail = zwindows.hrPanicOnFail;

const zgui = @import("zgui");

const zd3d12 = @import("zd3d12");

const GuiSrvDescHandles = struct {
    cpu: zgui.backend.D3D12_CPU_DESCRIPTOR_HANDLE,
    gpu: zgui.backend.D3D12_GPU_DESCRIPTOR_HANDLE,
};

fn guiSrvDescAlloc(info: *zgui.backend.ImGui_ImplDX12_InitInfo, out_cpu: *zgui.backend.D3D12_CPU_DESCRIPTOR_HANDLE, out_gpu: *zgui.backend.D3D12_GPU_DESCRIPTOR_HANDLE) callconv(.c) void {
    const handles: *const GuiSrvDescHandles = @ptrCast(@alignCast(info.user_data.?));
    out_cpu.* = handles.cpu;
    out_gpu.* = handles.gpu;
}

fn guiSrvDescFree(_: *zgui.backend.ImGui_ImplDX12_InitInfo, _: zgui.backend.D3D12_CPU_DESCRIPTOR_HANDLE, _: zgui.backend.D3D12_GPU_DESCRIPTOR_HANDLE) callconv(.c) void {}

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

    _ = zwindows.CoInitializeEx(null, zwindows.COINIT_MULTITHREADED);
    defer zwindows.CoUninitialize();

    _ = zwindows.SetProcessDPIAware();

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
    var gui_srv_handles: GuiSrvDescHandles = .{
        .cpu = @bitCast(cbv_srv.base.cpu_handle),
        .gpu = @bitCast(cbv_srv.base.gpu_handle),
    };
    zgui.backend.init(
        window,
        .{
            .device = gctx.device,
            .command_queue = gctx.cmdqueue,
            .num_frames_in_flight = zd3d12.GraphicsContext.max_num_buffered_frames,
            .rtv_format = @intFromEnum(dxgi.FORMAT.R8G8B8A8_UNORM),
            .dsv_format = @intFromEnum(dxgi.FORMAT.D32_FLOAT),
            .cbv_srv_heap = cbv_srv.heap.?,
            .user_data = @ptrCast(&gui_srv_handles),
            .srv_desc_alloc_fn = &guiSrvDescAlloc,
            .srv_desc_free_fn = &guiSrvDescFree,
        },
    );
    defer zgui.backend.deinit();

    mainLoop: while (true) {
        var message = std.mem.zeroes(zwindows.MSG);
        while (zwindows.PeekMessageA(&message, null, 0, 0, zwindows.PM_REMOVE) == zwindows.TRUE) {
            _ = zwindows.TranslateMessage(&message);
            _ = zwindows.DispatchMessageA(&message);
            if (message.message == zwindows.WM_QUIT) {
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
            zwindows.TRUE,
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
    window: zwindows.HWND,
    message: zwindows.UINT,
    wparam: zwindows.WPARAM,
    lparam: zwindows.LPARAM,
) callconv(zwindows.WINAPI) zwindows.LRESULT {
    switch (message) {
        zwindows.WM_KEYDOWN => {
            if (wparam == zwindows.VK_ESCAPE) {
                zwindows.PostQuitMessage(0);
                return 0;
            }
        },
        zwindows.WM_GETMINMAXINFO => {
            var info: *zwindows.MINMAXINFO = @ptrFromInt(@as(usize, @intCast(lparam)));
            info.ptMinTrackSize.x = 400;
            info.ptMinTrackSize.y = 400;
            return 0;
        },
        zwindows.WM_DESTROY => {
            zwindows.PostQuitMessage(0);
            return 0;
        },
        else => {},
    }
    return zwindows.DefWindowProcA(window, message, wparam, lparam);
}

fn createWindow(width: u32, height: u32) zwindows.HWND {
    const winclass = zwindows.WNDCLASSEXA{
        .style = 0,
        .lpfnWndProc = processWindowMessage,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = @ptrCast(zwindows.GetModuleHandleA(null)),
        .hIcon = null,
        .hCursor = zwindows.LoadCursorA(null, @ptrFromInt(32512)),
        .hbrBackground = null,
        .lpszMenuName = null,
        .lpszClassName = window_title,
        .hIconSm = null,
    };
    _ = zwindows.RegisterClassExA(&winclass);

    const style = zwindows.WS_OVERLAPPEDWINDOW;

    var rect = zwindows.RECT{
        .left = 0,
        .top = 0,
        .right = @intCast(width),
        .bottom = @intCast(height),
    };
    _ = zwindows.AdjustWindowRectEx(&rect, style, zwindows.FALSE, 0);

    const window = zwindows.CreateWindowExA(
        0,
        window_title,
        window_title,
        style + zwindows.WS_VISIBLE,
        zwindows.CW_USEDEFAULT,
        zwindows.CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        null,
        null,
        winclass.hInstance,
        null,
    ).?;

    return window;
}
