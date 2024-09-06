const std = @import("std");
const panic = std.debug.panic;
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

const zwindows = @import("zwindows");
const windows = zwindows.windows;
const dwrite = zwindows.dwrite;
const d2d1 = zwindows.d2d1;

pub const GuiRenderer = @import("GuiRenderer.zig");
pub const vectormath = @import("vectormath.zig");

pub const c = @cImport({
    @cDefine("CIMGUI_DEFINE_ENUMS_AND_STRUCTS", "");
    @cDefine("CIMGUI_NO_EXPORT", "");
    @cInclude("cimgui.h");
    @cInclude("cgltf.h");
    //@cInclude("stb_image.h");
});

pub const FrameStats = struct {
    time: f64,
    delta_time: f32,
    fps: f32,
    average_cpu_time: f32,
    timer: std.time.Timer,
    previous_time_ns: u64,
    fps_refresh_time_ns: u64,
    frame_counter: u64,

    pub fn init() FrameStats {
        return .{
            .time = 0.0,
            .delta_time = 0.0,
            .fps = 0.0,
            .average_cpu_time = 0.0,
            .timer = std.time.Timer.start() catch unreachable,
            .previous_time_ns = 0,
            .fps_refresh_time_ns = 0,
            .frame_counter = 0,
        };
    }

    pub fn update(self: *FrameStats, window: windows.HWND, window_name: []const u8) void {
        const now_ns = self.timer.read();
        self.time = @as(f64, @floatFromInt(now_ns)) / std.time.ns_per_s;
        self.delta_time = @as(f32, @floatFromInt(now_ns - self.previous_time_ns)) / std.time.ns_per_s;
        self.previous_time_ns = now_ns;

        if ((now_ns - self.fps_refresh_time_ns) >= std.time.ns_per_s) {
            const t = @as(f64, @floatFromInt(now_ns - self.fps_refresh_time_ns)) / std.time.ns_per_s;
            const fps = @as(f64, @floatFromInt(self.frame_counter)) / t;
            const ms = (1.0 / fps) * 1000.0;

            self.fps = @as(f32, @floatCast(fps));
            self.average_cpu_time = @as(f32, @floatCast(ms));
            self.fps_refresh_time_ns = now_ns;
            self.frame_counter = 0;

            var buffer = [_]u8{0} ** 128;
            const text = std.fmt.bufPrint(
                buffer[0..],
                "FPS: {d:.1}  CPU time: {d:.3} ms | {s}",
                .{ self.fps, self.average_cpu_time, window_name },
            ) catch unreachable;
            _ = windows.SetWindowTextA(window, @as([*:0]const u8, @ptrCast(text.ptr)));
        }
        self.frame_counter += 1;
    }
};

fn processWindowMessage(
    window: windows.HWND,
    message: windows.UINT,
    wparam: windows.WPARAM,
    lparam: windows.LPARAM,
) callconv(windows.WINAPI) windows.LRESULT {
    assert(c.igGetCurrentContext() != null);
    const ui = c.igGetIO().?;
    const ui_backend = @as(*GuiBackendState, @ptrCast(@alignCast(ui.*.BackendPlatformUserData)));
    switch (message) {
        windows.WM_LBUTTONDOWN,
        windows.WM_RBUTTONDOWN,
        windows.WM_MBUTTONDOWN,
        windows.WM_LBUTTONDBLCLK,
        windows.WM_RBUTTONDBLCLK,
        windows.WM_MBUTTONDBLCLK,
        => {
            var button: u32 = 0;
            if (message == windows.WM_LBUTTONDOWN or message == windows.WM_LBUTTONDBLCLK) button = 0;
            if (message == windows.WM_RBUTTONDOWN or message == windows.WM_RBUTTONDBLCLK) button = 1;
            if (message == windows.WM_MBUTTONDOWN or message == windows.WM_MBUTTONDBLCLK) button = 2;
            if (ui_backend.*.mouse_buttons_down == 0 and windows.GetCapture() == null) {
                _ = windows.SetCapture(window);
            }
            ui_backend.*.mouse_buttons_down |= @as(u32, 1) << @as(u5, @intCast(button));
            c.ImGuiIO_AddMouseButtonEvent(ui, @as(i32, @intCast(button)), true);
        },
        windows.WM_LBUTTONUP,
        windows.WM_RBUTTONUP,
        windows.WM_MBUTTONUP,
        => {
            var button: u32 = 0;
            if (message == windows.WM_LBUTTONUP) button = 0;
            if (message == windows.WM_RBUTTONUP) button = 1;
            if (message == windows.WM_MBUTTONUP) button = 2;
            ui_backend.*.mouse_buttons_down &= ~(@as(u32, 1) << @as(u5, @intCast(button)));
            if (ui_backend.*.mouse_buttons_down == 0 and windows.GetCapture() == window) {
                _ = windows.ReleaseCapture();
            }
            c.ImGuiIO_AddMouseButtonEvent(ui, @as(i32, @intCast(button)), false);
        },
        windows.WM_MOUSEWHEEL => {
            c.ImGuiIO_AddMouseWheelEvent(
                ui,
                0.0,
                @as(f32, @floatFromInt(windows.GET_WHEEL_DELTA_WPARAM(wparam))) / @as(f32, @floatFromInt(windows.WHEEL_DELTA)),
            );
        },
        windows.WM_MOUSEMOVE => {
            ui_backend.*.mouse_window = window;
            if (ui_backend.*.mouse_tracked == false) {
                var tme = windows.TRACKMOUSEEVENT{
                    .cbSize = @sizeOf(windows.TRACKMOUSEEVENT),
                    .dwFlags = windows.TME_LEAVE,
                    .hwndTrack = window,
                    .dwHoverTime = 0,
                };
                _ = windows.TrackMouseEvent(&tme);
                ui_backend.*.mouse_tracked = true;
            }
            c.ImGuiIO_AddMousePosEvent(
                ui,
                @as(f32, @floatFromInt(windows.GET_X_LPARAM(lparam))),
                @as(f32, @floatFromInt(windows.GET_Y_LPARAM(lparam))),
            );
        },
        windows.WM_MOUSELEAVE => {
            if (ui_backend.*.mouse_window == window) {
                ui_backend.*.mouse_window = null;
            }
            ui_backend.*.mouse_tracked = false;
            c.ImGuiIO_AddMousePosEvent(ui, -c.igGET_FLT_MAX(), -c.igGET_FLT_MAX());
        },
        windows.WM_KEYDOWN,
        windows.WM_KEYUP,
        windows.WM_SYSKEYDOWN,
        windows.WM_SYSKEYUP,
        => {
            if (wparam == windows.VK_ESCAPE) {
                windows.PostQuitMessage(0);
            }
            const down = if (message == windows.WM_KEYDOWN or message == windows.WM_SYSKEYDOWN) true else false;
            if (wparam < 256) {
                c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_ModCtrl, isVkKeyDown(windows.VK_CONTROL));
                c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_ModShift, isVkKeyDown(windows.VK_SHIFT));
                c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_ModAlt, isVkKeyDown(windows.VK_MENU));
                c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_ModSuper, isVkKeyDown(windows.VK_APPS));

                var vk = @as(i32, @intCast(wparam));
                if (wparam == windows.VK_RETURN and (((lparam >> 16) & 0xffff) & windows.KF_EXTENDED) != 0) {
                    vk = windows.IM_VK_KEYPAD_ENTER;
                }
                const key = vkKeyToImGuiKey(wparam);

                if (key != c.ImGuiKey_None)
                    c.ImGuiIO_AddKeyEvent(ui, key, down);

                if (vk == windows.VK_SHIFT) {
                    if (isVkKeyDown(windows.VK_LSHIFT) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftShift, down);
                    if (isVkKeyDown(windows.VK_RSHIFT) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightShift, down);
                } else if (vk == windows.VK_CONTROL) {
                    if (isVkKeyDown(windows.VK_LCONTROL) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftCtrl, down);
                    if (isVkKeyDown(windows.VK_RCONTROL) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightCtrl, down);
                } else if (vk == windows.VK_MENU) {
                    if (isVkKeyDown(windows.VK_LMENU) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftAlt, down);
                    if (isVkKeyDown(windows.VK_RMENU) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightAlt, down);
                }
            }
        },
        windows.WM_SETFOCUS,
        windows.WM_KILLFOCUS,
        => {
            c.ImGuiIO_AddFocusEvent(ui, if (message == windows.WM_SETFOCUS) true else false);
        },
        windows.WM_CHAR => {
            if (wparam > 0 and wparam < 0x10000) {
                c.ImGuiIO_AddInputCharacterUTF16(ui, @as(u16, @intCast(wparam & 0xffff)));
            }
        },
        windows.WM_DESTROY => {
            windows.PostQuitMessage(0);
        },
        else => {
            return windows.DefWindowProcA(window, message, wparam, lparam);
        },
    }
    return 0;
}

const GuiBackendState = struct {
    window: ?windows.HWND,
    mouse_window: ?windows.HWND,
    mouse_tracked: bool,
    mouse_buttons_down: u32,
};

pub fn initWindow(allocator: std.mem.Allocator, name: [*:0]const u8, width: u32, height: u32) !windows.HWND {
    assert(c.igGetCurrentContext() == null);
    _ = c.igCreateContext(null);

    const ui = c.igGetIO().?;
    assert(ui.*.BackendPlatformUserData == null);

    const ui_backend = allocator.create(GuiBackendState) catch unreachable;
    errdefer allocator.destroy(ui_backend);
    ui_backend.* = .{
        .window = null,
        .mouse_window = null,
        .mouse_tracked = false,
        .mouse_buttons_down = 0,
    };

    ui.*.BackendPlatformUserData = ui_backend;
    ui.*.BackendFlags |= c.ImGuiBackendFlags_RendererHasVtxOffset;

    const winclass = windows.WNDCLASSEXA{
        .style = 0,
        .lpfnWndProc = processWindowMessage,
        .cbClsExtra = 0,
        .cbWndExtra = 0,
        .hInstance = @as(windows.HINSTANCE, @ptrCast(windows.GetModuleHandleA(null))),
        .hIcon = null,
        .hCursor = windows.LoadCursorA(null, @as(windows.LPCSTR, @ptrFromInt(32512))),
        .hbrBackground = null,
        .lpszMenuName = null,
        .lpszClassName = name,
        .hIconSm = null,
    };
    _ = windows.RegisterClassExA(&winclass);

    const style = windows.WS_OVERLAPPED +
        windows.WS_SYSMENU +
        windows.WS_CAPTION +
        windows.WS_MINIMIZEBOX;

    var rect = windows.RECT{ .left = 0, .top = 0, .right = @as(i32, @intCast(width)), .bottom = @as(i32, @intCast(height)) };
    // HACK(mziulek): For exact FullHD window size it is better to stick to requested total window size
    // (looks better on 1920x1080 displays).
    if (width != 1920 and height != 1080) {
        _ = windows.AdjustWindowRectEx(&rect, style, windows.FALSE, 0);
    }

    const window = windows.CreateWindowExA(
        0,
        name,
        name,
        style + windows.WS_VISIBLE,
        -1,
        -1,
        rect.right - rect.left,
        rect.bottom - rect.top,
        null,
        null,
        winclass.hInstance,
        null,
    ).?;
    ui_backend.*.window = window;

    c.igGetStyle().?.*.WindowRounding = 0.0;

    return window;
}

pub fn deinitWindow(allocator: std.mem.Allocator) void {
    const ui = c.igGetIO().?;
    assert(ui.*.BackendPlatformUserData != null);
    allocator.destroy(@as(*GuiBackendState, @ptrCast(@alignCast(ui.*.BackendPlatformUserData))));
    c.igDestroyContext(null);
}

pub fn handleWindowEvents() bool {
    var message = std.mem.zeroes(windows.MSG);
    while (windows.PeekMessageA(&message, null, 0, 0, windows.PM_REMOVE) == windows.TRUE) {
        _ = windows.TranslateMessage(&message);
        _ = windows.DispatchMessageA(&message);
        if (message.message == windows.WM_QUIT) {
            return false;
        }
    }
    return true;
}

fn isVkKeyDown(vk: c_int) bool {
    return (@as(u16, @bitCast(windows.GetKeyState(vk))) & 0x8000) != 0;
}

fn vkKeyToImGuiKey(wparam: windows.WPARAM) c.ImGuiKey {
    switch (wparam) {
        windows.VK_TAB => return c.ImGuiKey_Tab,
        windows.VK_LEFT => return c.ImGuiKey_LeftArrow,
        windows.VK_RIGHT => return c.ImGuiKey_RightArrow,
        windows.VK_UP => return c.ImGuiKey_UpArrow,
        windows.VK_DOWN => return c.ImGuiKey_DownArrow,
        windows.VK_PRIOR => return c.ImGuiKey_PageUp,
        windows.VK_NEXT => return c.ImGuiKey_PageDown,
        windows.VK_HOME => return c.ImGuiKey_Home,
        windows.VK_END => return c.ImGuiKey_End,
        windows.VK_INSERT => return c.ImGuiKey_Insert,
        windows.VK_DELETE => return c.ImGuiKey_Delete,
        windows.VK_BACK => return c.ImGuiKey_Backspace,
        windows.VK_SPACE => return c.ImGuiKey_Space,
        windows.VK_RETURN => return c.ImGuiKey_Enter,
        windows.VK_ESCAPE => return c.ImGuiKey_Escape,
        windows.VK_OEM_7 => return c.ImGuiKey_Apostrophe,
        windows.VK_OEM_COMMA => return c.ImGuiKey_Comma,
        windows.VK_OEM_MINUS => return c.ImGuiKey_Minus,
        windows.VK_OEM_PERIOD => return c.ImGuiKey_Period,
        windows.VK_OEM_2 => return c.ImGuiKey_Slash,
        windows.VK_OEM_1 => return c.ImGuiKey_Semicolon,
        windows.VK_OEM_PLUS => return c.ImGuiKey_Equal,
        windows.VK_OEM_4 => return c.ImGuiKey_LeftBracket,
        windows.VK_OEM_5 => return c.ImGuiKey_Backslash,
        windows.VK_OEM_6 => return c.ImGuiKey_RightBracket,
        windows.VK_OEM_3 => return c.ImGuiKey_GraveAccent,
        windows.VK_CAPITAL => return c.ImGuiKey_CapsLock,
        windows.VK_SCROLL => return c.ImGuiKey_ScrollLock,
        windows.VK_NUMLOCK => return c.ImGuiKey_NumLock,
        windows.VK_SNAPSHOT => return c.ImGuiKey_PrintScreen,
        windows.VK_PAUSE => return c.ImGuiKey_Pause,
        windows.VK_NUMPAD0 => return c.ImGuiKey_Keypad0,
        windows.VK_NUMPAD1 => return c.ImGuiKey_Keypad1,
        windows.VK_NUMPAD2 => return c.ImGuiKey_Keypad2,
        windows.VK_NUMPAD3 => return c.ImGuiKey_Keypad3,
        windows.VK_NUMPAD4 => return c.ImGuiKey_Keypad4,
        windows.VK_NUMPAD5 => return c.ImGuiKey_Keypad5,
        windows.VK_NUMPAD6 => return c.ImGuiKey_Keypad6,
        windows.VK_NUMPAD7 => return c.ImGuiKey_Keypad7,
        windows.VK_NUMPAD8 => return c.ImGuiKey_Keypad8,
        windows.VK_NUMPAD9 => return c.ImGuiKey_Keypad9,
        windows.VK_DECIMAL => return c.ImGuiKey_KeypadDecimal,
        windows.VK_DIVIDE => return c.ImGuiKey_KeypadDivide,
        windows.VK_MULTIPLY => return c.ImGuiKey_KeypadMultiply,
        windows.VK_SUBTRACT => return c.ImGuiKey_KeypadSubtract,
        windows.VK_ADD => return c.ImGuiKey_KeypadAdd,
        windows.IM_VK_KEYPAD_ENTER => return c.ImGuiKey_KeypadEnter,
        windows.VK_LSHIFT => return c.ImGuiKey_LeftShift,
        windows.VK_LCONTROL => return c.ImGuiKey_LeftCtrl,
        windows.VK_LMENU => return c.ImGuiKey_LeftAlt,
        windows.VK_LWIN => return c.ImGuiKey_LeftSuper,
        windows.VK_RSHIFT => return c.ImGuiKey_RightShift,
        windows.VK_RCONTROL => return c.ImGuiKey_RightCtrl,
        windows.VK_RMENU => return c.ImGuiKey_RightAlt,
        windows.VK_RWIN => return c.ImGuiKey_RightSuper,
        windows.VK_APPS => return c.ImGuiKey_Menu,
        '0' => return c.ImGuiKey_0,
        '1' => return c.ImGuiKey_1,
        '2' => return c.ImGuiKey_2,
        '3' => return c.ImGuiKey_3,
        '4' => return c.ImGuiKey_4,
        '5' => return c.ImGuiKey_5,
        '6' => return c.ImGuiKey_6,
        '7' => return c.ImGuiKey_7,
        '8' => return c.ImGuiKey_8,
        '9' => return c.ImGuiKey_9,
        'A' => return c.ImGuiKey_A,
        'B' => return c.ImGuiKey_B,
        'C' => return c.ImGuiKey_C,
        'D' => return c.ImGuiKey_D,
        'E' => return c.ImGuiKey_E,
        'F' => return c.ImGuiKey_F,
        'G' => return c.ImGuiKey_G,
        'H' => return c.ImGuiKey_H,
        'I' => return c.ImGuiKey_I,
        'J' => return c.ImGuiKey_J,
        'K' => return c.ImGuiKey_K,
        'L' => return c.ImGuiKey_L,
        'M' => return c.ImGuiKey_M,
        'N' => return c.ImGuiKey_N,
        'O' => return c.ImGuiKey_O,
        'P' => return c.ImGuiKey_P,
        'Q' => return c.ImGuiKey_Q,
        'R' => return c.ImGuiKey_R,
        'S' => return c.ImGuiKey_S,
        'T' => return c.ImGuiKey_T,
        'U' => return c.ImGuiKey_U,
        'V' => return c.ImGuiKey_V,
        'W' => return c.ImGuiKey_W,
        'X' => return c.ImGuiKey_X,
        'Y' => return c.ImGuiKey_Y,
        'Z' => return c.ImGuiKey_Z,
        windows.VK_F1 => return c.ImGuiKey_F1,
        windows.VK_F2 => return c.ImGuiKey_F2,
        windows.VK_F3 => return c.ImGuiKey_F3,
        windows.VK_F4 => return c.ImGuiKey_F4,
        windows.VK_F5 => return c.ImGuiKey_F5,
        windows.VK_F6 => return c.ImGuiKey_F6,
        windows.VK_F7 => return c.ImGuiKey_F7,
        windows.VK_F8 => return c.ImGuiKey_F8,
        windows.VK_F9 => return c.ImGuiKey_F9,
        windows.VK_F10 => return c.ImGuiKey_F10,
        windows.VK_F11 => return c.ImGuiKey_F11,
        windows.VK_F12 => return c.ImGuiKey_F12,
        else => return c.ImGuiKey_None,
    }
}

pub fn newImGuiFrame(delta_time: f32) void {
    assert(c.igGetCurrentContext() != null);

    const ui = c.igGetIO().?;
    const ui_backend = @as(*GuiBackendState, @ptrCast(@alignCast(ui.*.BackendPlatformUserData)));
    assert(ui_backend.*.window != null);

    var rect: windows.RECT = undefined;
    _ = windows.GetClientRect(ui_backend.*.window.?, &rect);
    const viewport_width = @as(f32, @floatFromInt(rect.right - rect.left));
    const viewport_height = @as(f32, @floatFromInt(rect.bottom - rect.top));

    ui.*.DisplaySize = c.ImVec2{ .x = viewport_width, .y = viewport_height };
    ui.*.DeltaTime = delta_time;
    c.igNewFrame();

    if (c.igIsKeyDown(c.ImGuiKey_LeftShift) and !isVkKeyDown(windows.VK_LSHIFT)) {
        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftShift, false);
    }
    if (c.igIsKeyDown(c.ImGuiKey_RightShift) and !isVkKeyDown(windows.VK_RSHIFT)) {
        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightShift, false);
    }

    if (c.igIsKeyDown(c.ImGuiKey_LeftSuper) and !isVkKeyDown(windows.VK_LWIN)) {
        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftSuper, false);
    }
    if (c.igIsKeyDown(c.ImGuiKey_LeftSuper) and !isVkKeyDown(windows.VK_RWIN)) {
        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightSuper, false);
    }
}

pub fn drawText(
    devctx: *d2d1.IDeviceContext6,
    text: []const u8,
    format: *dwrite.ITextFormat,
    layout_rect: *const d2d1.RECT_F,
    brush: *d2d1.IBrush,
) void {
    var utf16: [128:0]u16 = undefined;
    assert(text.len < utf16.len);
    const len = std.unicode.utf8ToUtf16Le(utf16[0..], text) catch unreachable;
    utf16[len] = 0;
    devctx.DrawText(
        &utf16,
        @as(u32, @intCast(len)),
        format,
        layout_rect,
        brush,
        d2d1.DRAW_TEXT_OPTIONS_NONE,
        .NATURAL,
    );
}

pub fn readContentDirFileAlloc(allocator: std.mem.Allocator, content_dir: []const u8, relpath: []const u8, max_bytes: ?usize) ![]u8 {
    const self_exe_dir_path = try std.fs.selfExeDirPathAlloc(allocator);
    defer allocator.free(self_exe_dir_path);

    const content_dir_path = try std.fs.path.join(allocator, &.{ self_exe_dir_path, content_dir });
    defer allocator.free(content_dir_path);

    const self_exe_dir = try std.fs.openDirAbsolute(content_dir_path, .{});

    return self_exe_dir.readFileAlloc(allocator, relpath, max_bytes orelse 256 * 1024);
}

pub fn init() void {
    _ = windows.CoInitializeEx(null, windows.COINIT_APARTMENTTHREADED | windows.COINIT_DISABLE_OLE1DDE);
    _ = windows.SetProcessDPIAware();

    if (false and @import("builtin").target.os.tag == .windows) {
        // Check if Windows version is supported.
        var version: windows.OSVERSIONINFOW = undefined;
        _ = windows.RtlGetVersion(&version);

        var os_is_supported = false;
        if (version.dwMajorVersion > 10) {
            os_is_supported = true;
        } else if (version.dwMajorVersion == 10 and version.dwBuildNumber >= 18363) {
            os_is_supported = true;
        }

        const d3d12core_dll = windows.LoadLibraryA("D3D12Core.dll");
        if (d3d12core_dll == null) {
            os_is_supported = false;
        } else {
            _ = windows.FreeLibrary(d3d12core_dll.?);
        }

        if (!os_is_supported) {
            _ = windows.MessageBoxA(
                null,
                \\This application can't run on currently installed version of Windows.
                \\Following versions are supported:
                \\
                \\Windows 10 May 2021 (Build 19043) or newer
                \\Windows 10 October 2020 (Build 19042.789+)
                \\Windows 10 May 2020 (Build 19041.789+)
                \\Windows 10 November 2019 (Build 18363.1350+)
                \\
                \\Please update your Windows version and try again.
            ,
                "Error",
                windows.MB_OK | windows.MB_ICONERROR,
            );
            windows.ExitProcess(0);
        }

        // Check if 'd3d12' folder is present next to an executable.
        const local_d3d12core_dll = windows.LoadLibraryA("d3d12/D3D12Core.dll");
        if (local_d3d12core_dll == null) {
            _ = windows.MessageBoxA(
                null,
                \\Looks like 'd3d12' folder is missing. It has to be distributed together with an application.
            ,
                "Error",
                windows.MB_OK | windows.MB_ICONERROR,
            );
            windows.ExitProcess(0);
        } else {
            _ = windows.FreeLibrary(local_d3d12core_dll.?);
        }
    }
}

pub fn deinit() void {
    windows.CoUninitialize();
}
