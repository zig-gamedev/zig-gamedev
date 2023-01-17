const std = @import("std");
const panic = std.debug.panic;
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const dwrite = zwin32.dwrite;
const d2d1 = zwin32.d2d1;

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

    pub fn update(self: *FrameStats, window: w32.HWND, window_name: []const u8) void {
        const now_ns = self.timer.read();
        self.time = @intToFloat(f64, now_ns) / std.time.ns_per_s;
        self.delta_time = @intToFloat(f32, now_ns - self.previous_time_ns) / std.time.ns_per_s;
        self.previous_time_ns = now_ns;

        if ((now_ns - self.fps_refresh_time_ns) >= std.time.ns_per_s) {
            const t = @intToFloat(f64, now_ns - self.fps_refresh_time_ns) / std.time.ns_per_s;
            const fps = @intToFloat(f64, self.frame_counter) / t;
            const ms = (1.0 / fps) * 1000.0;

            self.fps = @floatCast(f32, fps);
            self.average_cpu_time = @floatCast(f32, ms);
            self.fps_refresh_time_ns = now_ns;
            self.frame_counter = 0;

            var buffer = [_]u8{0} ** 128;
            const text = std.fmt.bufPrint(
                buffer[0..],
                "FPS: {d:.1}  CPU time: {d:.3} ms | {s}",
                .{ self.fps, self.average_cpu_time, window_name },
            ) catch unreachable;
            _ = w32.SetWindowTextA(window, @ptrCast([*:0]const u8, text.ptr));
        }
        self.frame_counter += 1;
    }
};

fn processWindowMessage(
    window: w32.HWND,
    message: w32.UINT,
    wparam: w32.WPARAM,
    lparam: w32.LPARAM,
) callconv(w32.WINAPI) w32.LRESULT {
    assert(c.igGetCurrentContext() != null);
    var ui = c.igGetIO().?;
    var ui_backend = @ptrCast(*GuiBackendState, @alignCast(8, ui.*.BackendPlatformUserData));
    switch (message) {
        w32.WM_LBUTTONDOWN,
        w32.WM_RBUTTONDOWN,
        w32.WM_MBUTTONDOWN,
        w32.WM_LBUTTONDBLCLK,
        w32.WM_RBUTTONDBLCLK,
        w32.WM_MBUTTONDBLCLK,
        => {
            var button: u32 = 0;
            if (message == w32.WM_LBUTTONDOWN or message == w32.WM_LBUTTONDBLCLK) button = 0;
            if (message == w32.WM_RBUTTONDOWN or message == w32.WM_RBUTTONDBLCLK) button = 1;
            if (message == w32.WM_MBUTTONDOWN or message == w32.WM_MBUTTONDBLCLK) button = 2;
            if (ui_backend.*.mouse_buttons_down == 0 and w32.GetCapture() == null) {
                _ = w32.SetCapture(window);
            }
            ui_backend.*.mouse_buttons_down |= @as(u32, 1) << @intCast(u5, button);
            c.ImGuiIO_AddMouseButtonEvent(ui, @intCast(i32, button), true);
        },
        w32.WM_LBUTTONUP,
        w32.WM_RBUTTONUP,
        w32.WM_MBUTTONUP,
        => {
            var button: u32 = 0;
            if (message == w32.WM_LBUTTONUP) button = 0;
            if (message == w32.WM_RBUTTONUP) button = 1;
            if (message == w32.WM_MBUTTONUP) button = 2;
            ui_backend.*.mouse_buttons_down &= ~(@as(u32, 1) << @intCast(u5, button));
            if (ui_backend.*.mouse_buttons_down == 0 and w32.GetCapture() == window) {
                _ = w32.ReleaseCapture();
            }
            c.ImGuiIO_AddMouseButtonEvent(ui, @intCast(i32, button), false);
        },
        w32.WM_MOUSEWHEEL => {
            c.ImGuiIO_AddMouseWheelEvent(
                ui,
                0.0,
                @intToFloat(f32, w32.GET_WHEEL_DELTA_WPARAM(wparam)) / @intToFloat(f32, w32.WHEEL_DELTA),
            );
        },
        w32.WM_MOUSEMOVE => {
            ui_backend.*.mouse_window = window;
            if (ui_backend.*.mouse_tracked == false) {
                var tme = w32.TRACKMOUSEEVENT{
                    .cbSize = @sizeOf(w32.TRACKMOUSEEVENT),
                    .dwFlags = w32.TME_LEAVE,
                    .hwndTrack = window,
                    .dwHoverTime = 0,
                };
                _ = w32.TrackMouseEvent(&tme);
                ui_backend.*.mouse_tracked = true;
            }
            c.ImGuiIO_AddMousePosEvent(
                ui,
                @intToFloat(f32, w32.GET_X_LPARAM(lparam)),
                @intToFloat(f32, w32.GET_Y_LPARAM(lparam)),
            );
        },
        w32.WM_MOUSELEAVE => {
            if (ui_backend.*.mouse_window == window) {
                ui_backend.*.mouse_window = null;
            }
            ui_backend.*.mouse_tracked = false;
            c.ImGuiIO_AddMousePosEvent(ui, -c.igGET_FLT_MAX(), -c.igGET_FLT_MAX());
        },
        w32.WM_KEYDOWN,
        w32.WM_KEYUP,
        w32.WM_SYSKEYDOWN,
        w32.WM_SYSKEYUP,
        => {
            if (wparam == w32.VK_ESCAPE) {
                w32.PostQuitMessage(0);
            }
            const down = if (message == w32.WM_KEYDOWN or message == w32.WM_SYSKEYDOWN) true else false;
            if (wparam < 256) {
                c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_ModCtrl, isVkKeyDown(w32.VK_CONTROL));
                c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_ModShift, isVkKeyDown(w32.VK_SHIFT));
                c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_ModAlt, isVkKeyDown(w32.VK_MENU));
                c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_ModSuper, isVkKeyDown(w32.VK_APPS));

                var vk = @intCast(i32, wparam);
                if (wparam == w32.VK_RETURN and (((lparam >> 16) & 0xffff) & w32.KF_EXTENDED) != 0) {
                    vk = w32.IM_VK_KEYPAD_ENTER;
                }
                const key = vkKeyToImGuiKey(wparam);

                if (key != c.ImGuiKey_None)
                    c.ImGuiIO_AddKeyEvent(ui, key, down);

                if (vk == w32.VK_SHIFT) {
                    if (isVkKeyDown(w32.VK_LSHIFT) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftShift, down);
                    if (isVkKeyDown(w32.VK_RSHIFT) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightShift, down);
                } else if (vk == w32.VK_CONTROL) {
                    if (isVkKeyDown(w32.VK_LCONTROL) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftCtrl, down);
                    if (isVkKeyDown(w32.VK_RCONTROL) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightCtrl, down);
                } else if (vk == w32.VK_MENU) {
                    if (isVkKeyDown(w32.VK_LMENU) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftAlt, down);
                    if (isVkKeyDown(w32.VK_RMENU) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightAlt, down);
                }
            }
        },
        w32.WM_SETFOCUS,
        w32.WM_KILLFOCUS,
        => {
            c.ImGuiIO_AddFocusEvent(ui, if (message == w32.WM_SETFOCUS) true else false);
        },
        w32.WM_CHAR => {
            if (wparam > 0 and wparam < 0x10000) {
                c.ImGuiIO_AddInputCharacterUTF16(ui, @intCast(u16, wparam & 0xffff));
            }
        },
        w32.WM_DESTROY => {
            w32.PostQuitMessage(0);
        },
        else => {
            return w32.DefWindowProcA(window, message, wparam, lparam);
        },
    }
    return 0;
}

const GuiBackendState = struct {
    window: ?w32.HWND,
    mouse_window: ?w32.HWND,
    mouse_tracked: bool,
    mouse_buttons_down: u32,
};

pub fn initWindow(allocator: std.mem.Allocator, name: [*:0]const u8, width: u32, height: u32) !w32.HWND {
    assert(c.igGetCurrentContext() == null);
    _ = c.igCreateContext(null);

    var ui = c.igGetIO().?;
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

    const style = w32.WS_OVERLAPPED +
        w32.WS_SYSMENU +
        w32.WS_CAPTION +
        w32.WS_MINIMIZEBOX;

    var rect = w32.RECT{ .left = 0, .top = 0, .right = @intCast(i32, width), .bottom = @intCast(i32, height) };
    // HACK(mziulek): For exact FullHD window size it is better to stick to requested total window size
    // (looks better on 1920x1080 displays).
    if (width != 1920 and height != 1080) {
        _ = w32.AdjustWindowRectEx(&rect, style, w32.FALSE, 0);
    }

    const window = w32.CreateWindowExA(
        0,
        name,
        name,
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
    ui_backend.*.window = window;

    c.igGetStyle().?.*.WindowRounding = 0.0;

    return window;
}

pub fn deinitWindow(allocator: std.mem.Allocator) void {
    var ui = c.igGetIO().?;
    assert(ui.*.BackendPlatformUserData != null);
    allocator.destroy(@ptrCast(*GuiBackendState, @alignCast(@sizeOf(usize), ui.*.BackendPlatformUserData)));
    c.igDestroyContext(null);
}

pub fn handleWindowEvents() bool {
    var message = std.mem.zeroes(w32.MSG);
    while (w32.PeekMessageA(&message, null, 0, 0, w32.PM_REMOVE) == w32.TRUE) {
        _ = w32.TranslateMessage(&message);
        _ = w32.DispatchMessageA(&message);
        if (message.message == w32.WM_QUIT) {
            return false;
        }
    }
    return true;
}

fn isVkKeyDown(vk: c_int) bool {
    return (@bitCast(u16, w32.GetKeyState(vk)) & 0x8000) != 0;
}

fn vkKeyToImGuiKey(wparam: w32.WPARAM) c.ImGuiKey {
    switch (wparam) {
        w32.VK_TAB => return c.ImGuiKey_Tab,
        w32.VK_LEFT => return c.ImGuiKey_LeftArrow,
        w32.VK_RIGHT => return c.ImGuiKey_RightArrow,
        w32.VK_UP => return c.ImGuiKey_UpArrow,
        w32.VK_DOWN => return c.ImGuiKey_DownArrow,
        w32.VK_PRIOR => return c.ImGuiKey_PageUp,
        w32.VK_NEXT => return c.ImGuiKey_PageDown,
        w32.VK_HOME => return c.ImGuiKey_Home,
        w32.VK_END => return c.ImGuiKey_End,
        w32.VK_INSERT => return c.ImGuiKey_Insert,
        w32.VK_DELETE => return c.ImGuiKey_Delete,
        w32.VK_BACK => return c.ImGuiKey_Backspace,
        w32.VK_SPACE => return c.ImGuiKey_Space,
        w32.VK_RETURN => return c.ImGuiKey_Enter,
        w32.VK_ESCAPE => return c.ImGuiKey_Escape,
        w32.VK_OEM_7 => return c.ImGuiKey_Apostrophe,
        w32.VK_OEM_COMMA => return c.ImGuiKey_Comma,
        w32.VK_OEM_MINUS => return c.ImGuiKey_Minus,
        w32.VK_OEM_PERIOD => return c.ImGuiKey_Period,
        w32.VK_OEM_2 => return c.ImGuiKey_Slash,
        w32.VK_OEM_1 => return c.ImGuiKey_Semicolon,
        w32.VK_OEM_PLUS => return c.ImGuiKey_Equal,
        w32.VK_OEM_4 => return c.ImGuiKey_LeftBracket,
        w32.VK_OEM_5 => return c.ImGuiKey_Backslash,
        w32.VK_OEM_6 => return c.ImGuiKey_RightBracket,
        w32.VK_OEM_3 => return c.ImGuiKey_GraveAccent,
        w32.VK_CAPITAL => return c.ImGuiKey_CapsLock,
        w32.VK_SCROLL => return c.ImGuiKey_ScrollLock,
        w32.VK_NUMLOCK => return c.ImGuiKey_NumLock,
        w32.VK_SNAPSHOT => return c.ImGuiKey_PrintScreen,
        w32.VK_PAUSE => return c.ImGuiKey_Pause,
        w32.VK_NUMPAD0 => return c.ImGuiKey_Keypad0,
        w32.VK_NUMPAD1 => return c.ImGuiKey_Keypad1,
        w32.VK_NUMPAD2 => return c.ImGuiKey_Keypad2,
        w32.VK_NUMPAD3 => return c.ImGuiKey_Keypad3,
        w32.VK_NUMPAD4 => return c.ImGuiKey_Keypad4,
        w32.VK_NUMPAD5 => return c.ImGuiKey_Keypad5,
        w32.VK_NUMPAD6 => return c.ImGuiKey_Keypad6,
        w32.VK_NUMPAD7 => return c.ImGuiKey_Keypad7,
        w32.VK_NUMPAD8 => return c.ImGuiKey_Keypad8,
        w32.VK_NUMPAD9 => return c.ImGuiKey_Keypad9,
        w32.VK_DECIMAL => return c.ImGuiKey_KeypadDecimal,
        w32.VK_DIVIDE => return c.ImGuiKey_KeypadDivide,
        w32.VK_MULTIPLY => return c.ImGuiKey_KeypadMultiply,
        w32.VK_SUBTRACT => return c.ImGuiKey_KeypadSubtract,
        w32.VK_ADD => return c.ImGuiKey_KeypadAdd,
        w32.IM_VK_KEYPAD_ENTER => return c.ImGuiKey_KeypadEnter,
        w32.VK_LSHIFT => return c.ImGuiKey_LeftShift,
        w32.VK_LCONTROL => return c.ImGuiKey_LeftCtrl,
        w32.VK_LMENU => return c.ImGuiKey_LeftAlt,
        w32.VK_LWIN => return c.ImGuiKey_LeftSuper,
        w32.VK_RSHIFT => return c.ImGuiKey_RightShift,
        w32.VK_RCONTROL => return c.ImGuiKey_RightCtrl,
        w32.VK_RMENU => return c.ImGuiKey_RightAlt,
        w32.VK_RWIN => return c.ImGuiKey_RightSuper,
        w32.VK_APPS => return c.ImGuiKey_Menu,
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
        w32.VK_F1 => return c.ImGuiKey_F1,
        w32.VK_F2 => return c.ImGuiKey_F2,
        w32.VK_F3 => return c.ImGuiKey_F3,
        w32.VK_F4 => return c.ImGuiKey_F4,
        w32.VK_F5 => return c.ImGuiKey_F5,
        w32.VK_F6 => return c.ImGuiKey_F6,
        w32.VK_F7 => return c.ImGuiKey_F7,
        w32.VK_F8 => return c.ImGuiKey_F8,
        w32.VK_F9 => return c.ImGuiKey_F9,
        w32.VK_F10 => return c.ImGuiKey_F10,
        w32.VK_F11 => return c.ImGuiKey_F11,
        w32.VK_F12 => return c.ImGuiKey_F12,
        else => return c.ImGuiKey_None,
    }
}

pub fn newImGuiFrame(delta_time: f32) void {
    assert(c.igGetCurrentContext() != null);

    var ui = c.igGetIO().?;
    var ui_backend = @ptrCast(*GuiBackendState, @alignCast(@sizeOf(usize), ui.*.BackendPlatformUserData));
    assert(ui_backend.*.window != null);

    var rect: w32.RECT = undefined;
    _ = w32.GetClientRect(ui_backend.*.window.?, &rect);
    const viewport_width = @intToFloat(f32, rect.right - rect.left);
    const viewport_height = @intToFloat(f32, rect.bottom - rect.top);

    ui.*.DisplaySize = c.ImVec2{ .x = viewport_width, .y = viewport_height };
    ui.*.DeltaTime = delta_time;
    c.igNewFrame();

    if (c.igIsKeyDown(c.ImGuiKey_LeftShift) and !isVkKeyDown(w32.VK_LSHIFT)) {
        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftShift, false);
    }
    if (c.igIsKeyDown(c.ImGuiKey_RightShift) and !isVkKeyDown(w32.VK_RSHIFT)) {
        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightShift, false);
    }

    if (c.igIsKeyDown(c.ImGuiKey_LeftSuper) and !isVkKeyDown(w32.VK_LWIN)) {
        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftSuper, false);
    }
    if (c.igIsKeyDown(c.ImGuiKey_LeftSuper) and !isVkKeyDown(w32.VK_RWIN)) {
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
        @intCast(u32, len),
        format,
        layout_rect,
        brush,
        d2d1.DRAW_TEXT_OPTIONS_NONE,
        .NATURAL,
    );
}

pub fn init() void {
    _ = w32.CoInitializeEx(null, w32.COINIT_APARTMENTTHREADED | w32.COINIT_DISABLE_OLE1DDE);
    _ = w32.SetProcessDPIAware();

    // Check if Windows version is supported.
    var version: w32.OSVERSIONINFOW = undefined;
    _ = w32.RtlGetVersion(&version);

    var os_is_supported = false;
    if (version.dwMajorVersion > 10) {
        os_is_supported = true;
    } else if (version.dwMajorVersion == 10 and version.dwBuildNumber >= 18363) {
        os_is_supported = true;
    }

    const d3d12core_dll = w32.LoadLibraryA("D3D12Core.dll");
    if (d3d12core_dll == null) {
        os_is_supported = false;
    } else {
        _ = w32.FreeLibrary(d3d12core_dll.?);
    }

    if (!os_is_supported) {
        _ = w32.MessageBoxA(
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
            w32.MB_OK | w32.MB_ICONERROR,
        );
        w32.ExitProcess(0);
    }

    // Change directory to where an executable is located.
    var exe_path_buffer: [1024]u8 = undefined;
    const exe_path = std.fs.selfExeDirPath(exe_path_buffer[0..]) catch "./";
    std.os.chdir(exe_path) catch {};

    // Check if 'd3d12' folder is present next to an executable.
    const local_d3d12core_dll = w32.LoadLibraryA("d3d12/D3D12Core.dll");
    if (local_d3d12core_dll == null) {
        _ = w32.MessageBoxA(
            null,
            \\Looks like 'd3d12' folder is missing. It has to be distributed together with an application.
        ,
            "Error",
            w32.MB_OK | w32.MB_ICONERROR,
        );
        w32.ExitProcess(0);
    } else {
        _ = w32.FreeLibrary(local_d3d12core_dll.?);
    }
}

pub fn deinit() void {
    w32.CoUninitialize();
}
