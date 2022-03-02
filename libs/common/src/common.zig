const std = @import("std");
const panic = std.debug.panic;
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w = zwin32.base;
const dwrite = zwin32.dwrite;
const d2d1 = zwin32.d2d1;

pub const GuiRenderer = @import("GuiRenderer.zig");
pub const vectormath = @import("vectormath.zig");

pub const c = @cImport({
    @cDefine("CIMGUI_DEFINE_ENUMS_AND_STRUCTS", "");
    @cDefine("CIMGUI_NO_EXPORT", "");
    @cInclude("cimgui.h");
    @cInclude("cgltf.h");
    @cInclude("meshoptimizer.h");
    @cInclude("stb_image.h");
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

    pub fn update(self: *FrameStats, window: w.HWND, window_name: []const u8) void {
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
        }
        self.frame_counter += 1;

        {
            var buffer = [_]u8{0} ** 128;
            const text = std.fmt.bufPrint(
                buffer[0..],
                "FPS: {d:.1}  CPU time: {d:.3} ms | {s}",
                .{ self.fps, self.average_cpu_time, window_name },
            ) catch unreachable;
            _ = w.SetWindowTextA(window, @ptrCast([*:0]const u8, text.ptr));
        }
    }
};

fn processWindowMessage(
    window: w.HWND,
    message: w.UINT,
    wparam: w.WPARAM,
    lparam: w.LPARAM,
) callconv(w.WINAPI) w.LRESULT {
    assert(c.igGetCurrentContext() != null);
    var ui = c.igGetIO().?;
    var ui_backend = @ptrCast(*GuiBackendState, @alignCast(8, ui.*.BackendPlatformUserData));
    switch (message) {
        w.user32.WM_LBUTTONDOWN,
        w.user32.WM_RBUTTONDOWN,
        w.user32.WM_MBUTTONDOWN,
        w.user32.WM_LBUTTONDBLCLK,
        w.user32.WM_RBUTTONDBLCLK,
        w.user32.WM_MBUTTONDBLCLK,
        => {
            var button: u32 = 0;
            if (message == w.user32.WM_LBUTTONDOWN or message == w.user32.WM_LBUTTONDBLCLK) button = 0;
            if (message == w.user32.WM_RBUTTONDOWN or message == w.user32.WM_RBUTTONDBLCLK) button = 1;
            if (message == w.user32.WM_MBUTTONDOWN or message == w.user32.WM_MBUTTONDBLCLK) button = 2;
            if (ui_backend.*.mouse_buttons_down == 0 and w.GetCapture() == null) {
                _ = w.SetCapture(window);
            }
            ui_backend.*.mouse_buttons_down |= @as(u32, 1) << @intCast(u5, button);
            c.ImGuiIO_AddMouseButtonEvent(ui, @intCast(i32, button), true);
        },
        w.user32.WM_LBUTTONUP,
        w.user32.WM_RBUTTONUP,
        w.user32.WM_MBUTTONUP,
        => {
            var button: u32 = 0;
            if (message == w.user32.WM_LBUTTONUP) button = 0;
            if (message == w.user32.WM_RBUTTONUP) button = 1;
            if (message == w.user32.WM_MBUTTONUP) button = 2;
            ui_backend.*.mouse_buttons_down &= ~(@as(u32, 1) << @intCast(u5, button));
            if (ui_backend.*.mouse_buttons_down == 0 and w.GetCapture() == window) {
                _ = w.ReleaseCapture();
            }
            c.ImGuiIO_AddMouseButtonEvent(ui, @intCast(i32, button), false);
        },
        w.user32.WM_MOUSEWHEEL => {
            c.ImGuiIO_AddMouseWheelEvent(
                ui,
                0.0,
                @intToFloat(f32, w.GET_WHEEL_DELTA_WPARAM(wparam)) / @intToFloat(f32, w.WHEEL_DELTA),
            );
        },
        w.user32.WM_MOUSEMOVE => {
            ui_backend.*.mouse_window = window;
            if (ui_backend.*.mouse_tracked == false) {
                var tme = w.TRACKMOUSEEVENT{
                    .cbSize = @sizeOf(w.TRACKMOUSEEVENT),
                    .dwFlags = w.TME_LEAVE,
                    .hwndTrack = window,
                    .dwHoverTime = 0,
                };
                _ = w.TrackMouseEvent(&tme);
                ui_backend.*.mouse_tracked = true;
            }
            c.ImGuiIO_AddMousePosEvent(
                ui,
                @intToFloat(f32, w.GET_X_LPARAM(lparam)),
                @intToFloat(f32, w.GET_Y_LPARAM(lparam)),
            );
        },
        w.user32.WM_MOUSELEAVE => {
            if (ui_backend.*.mouse_window == window) {
                ui_backend.*.mouse_window = null;
            }
            ui_backend.*.mouse_tracked = false;
            c.ImGuiIO_AddMousePosEvent(ui, -c.igGET_FLT_MAX(), -c.igGET_FLT_MAX());
        },
        w.user32.WM_KEYDOWN,
        w.user32.WM_KEYUP,
        w.user32.WM_SYSKEYDOWN,
        w.user32.WM_SYSKEYUP,
        => {
            if (wparam == w.VK_ESCAPE) {
                w.user32.PostQuitMessage(0);
            }
            const down = if (message == w.user32.WM_KEYDOWN or message == w.user32.WM_SYSKEYDOWN) true else false;
            if (wparam < 256) {
                c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_ModCtrl, isVkKeyDown(w.VK_CONTROL));
                c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_ModShift, isVkKeyDown(w.VK_SHIFT));
                c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_ModAlt, isVkKeyDown(w.VK_MENU));
                c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_ModSuper, isVkKeyDown(w.VK_APPS));

                var vk = @intCast(i32, wparam);
                if (wparam == w.VK_RETURN and (((lparam >> 16) & 0xffff) & w.KF_EXTENDED) != 0) {
                    vk = w.IM_VK_KEYPAD_ENTER;
                }
                const key = vkKeyToImGuiKey(wparam);

                if (key != c.ImGuiKey_None)
                    c.ImGuiIO_AddKeyEvent(ui, key, down);

                if (vk == w.VK_SHIFT) {
                    if (isVkKeyDown(w.VK_LSHIFT) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftShift, down);
                    if (isVkKeyDown(w.VK_RSHIFT) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightShift, down);
                } else if (vk == w.VK_CONTROL) {
                    if (isVkKeyDown(w.VK_LCONTROL) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftCtrl, down);
                    if (isVkKeyDown(w.VK_RCONTROL) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightCtrl, down);
                } else if (vk == w.VK_MENU) {
                    if (isVkKeyDown(w.VK_LMENU) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftAlt, down);
                    if (isVkKeyDown(w.VK_RMENU) == down)
                        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightAlt, down);
                }
            }
        },
        w.user32.WM_SETFOCUS,
        w.user32.WM_KILLFOCUS,
        => {
            c.ImGuiIO_AddFocusEvent(ui, if (message == w.user32.WM_SETFOCUS) true else false);
        },
        w.user32.WM_CHAR => {
            if (wparam > 0 and wparam < 0x10000) {
                c.ImGuiIO_AddInputCharacterUTF16(ui, @intCast(u16, wparam & 0xffff));
            }
        },
        w.user32.WM_DESTROY => {
            w.user32.PostQuitMessage(0);
        },
        else => {
            return w.user32.defWindowProcA(window, message, wparam, lparam);
        },
    }
    return 0;
}

const GuiBackendState = struct {
    window: ?w.HWND,
    mouse_window: ?w.HWND,
    mouse_tracked: bool,
    mouse_buttons_down: u32,
};

pub fn initWindow(allocator: std.mem.Allocator, name: [*:0]const u8, width: u32, height: u32) !w.HWND {
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
        .lpszClassName = name,
        .hIconSm = null,
    };
    _ = try w.user32.registerClassExA(&winclass);

    const style = w.user32.WS_OVERLAPPED +
        w.user32.WS_SYSMENU +
        w.user32.WS_CAPTION +
        w.user32.WS_MINIMIZEBOX;

    var rect = w.RECT{ .left = 0, .top = 0, .right = @intCast(i32, width), .bottom = @intCast(i32, height) };
    // HACK(mziulek): For exact FullHD window size it is better to stick to requested total window size
    // (looks better on 1920x1080 displays).
    if (width != 1920 and height != 1080) {
        try w.user32.adjustWindowRectEx(&rect, style, false, 0);
    }

    const window = try w.user32.createWindowExA(
        0,
        name,
        name,
        style + w.user32.WS_VISIBLE,
        -1,
        -1,
        rect.right - rect.left,
        rect.bottom - rect.top,
        null,
        null,
        winclass.hInstance,
        null,
    );
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
    var message = std.mem.zeroes(w.user32.MSG);
    while (w.user32.peekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) catch false) {
        _ = w.user32.translateMessage(&message);
        _ = w.user32.dispatchMessageA(&message);
        if (message.message == w.user32.WM_QUIT) {
            return false;
        }
    }
    return true;
}

fn isVkKeyDown(vk: c_int) bool {
    return (@bitCast(u16, w.GetKeyState(vk)) & 0x8000) != 0;
}

fn vkKeyToImGuiKey(wparam: w.WPARAM) c.ImGuiKey {
    switch (wparam) {
        w.VK_TAB => return c.ImGuiKey_Tab,
        w.VK_LEFT => return c.ImGuiKey_LeftArrow,
        w.VK_RIGHT => return c.ImGuiKey_RightArrow,
        w.VK_UP => return c.ImGuiKey_UpArrow,
        w.VK_DOWN => return c.ImGuiKey_DownArrow,
        w.VK_PRIOR => return c.ImGuiKey_PageUp,
        w.VK_NEXT => return c.ImGuiKey_PageDown,
        w.VK_HOME => return c.ImGuiKey_Home,
        w.VK_END => return c.ImGuiKey_End,
        w.VK_INSERT => return c.ImGuiKey_Insert,
        w.VK_DELETE => return c.ImGuiKey_Delete,
        w.VK_BACK => return c.ImGuiKey_Backspace,
        w.VK_SPACE => return c.ImGuiKey_Space,
        w.VK_RETURN => return c.ImGuiKey_Enter,
        w.VK_ESCAPE => return c.ImGuiKey_Escape,
        w.VK_OEM_7 => return c.ImGuiKey_Apostrophe,
        w.VK_OEM_COMMA => return c.ImGuiKey_Comma,
        w.VK_OEM_MINUS => return c.ImGuiKey_Minus,
        w.VK_OEM_PERIOD => return c.ImGuiKey_Period,
        w.VK_OEM_2 => return c.ImGuiKey_Slash,
        w.VK_OEM_1 => return c.ImGuiKey_Semicolon,
        w.VK_OEM_PLUS => return c.ImGuiKey_Equal,
        w.VK_OEM_4 => return c.ImGuiKey_LeftBracket,
        w.VK_OEM_5 => return c.ImGuiKey_Backslash,
        w.VK_OEM_6 => return c.ImGuiKey_RightBracket,
        w.VK_OEM_3 => return c.ImGuiKey_GraveAccent,
        w.VK_CAPITAL => return c.ImGuiKey_CapsLock,
        w.VK_SCROLL => return c.ImGuiKey_ScrollLock,
        w.VK_NUMLOCK => return c.ImGuiKey_NumLock,
        w.VK_SNAPSHOT => return c.ImGuiKey_PrintScreen,
        w.VK_PAUSE => return c.ImGuiKey_Pause,
        w.VK_NUMPAD0 => return c.ImGuiKey_Keypad0,
        w.VK_NUMPAD1 => return c.ImGuiKey_Keypad1,
        w.VK_NUMPAD2 => return c.ImGuiKey_Keypad2,
        w.VK_NUMPAD3 => return c.ImGuiKey_Keypad3,
        w.VK_NUMPAD4 => return c.ImGuiKey_Keypad4,
        w.VK_NUMPAD5 => return c.ImGuiKey_Keypad5,
        w.VK_NUMPAD6 => return c.ImGuiKey_Keypad6,
        w.VK_NUMPAD7 => return c.ImGuiKey_Keypad7,
        w.VK_NUMPAD8 => return c.ImGuiKey_Keypad8,
        w.VK_NUMPAD9 => return c.ImGuiKey_Keypad9,
        w.VK_DECIMAL => return c.ImGuiKey_KeypadDecimal,
        w.VK_DIVIDE => return c.ImGuiKey_KeypadDivide,
        w.VK_MULTIPLY => return c.ImGuiKey_KeypadMultiply,
        w.VK_SUBTRACT => return c.ImGuiKey_KeypadSubtract,
        w.VK_ADD => return c.ImGuiKey_KeypadAdd,
        w.IM_VK_KEYPAD_ENTER => return c.ImGuiKey_KeypadEnter,
        w.VK_LSHIFT => return c.ImGuiKey_LeftShift,
        w.VK_LCONTROL => return c.ImGuiKey_LeftCtrl,
        w.VK_LMENU => return c.ImGuiKey_LeftAlt,
        w.VK_LWIN => return c.ImGuiKey_LeftSuper,
        w.VK_RSHIFT => return c.ImGuiKey_RightShift,
        w.VK_RCONTROL => return c.ImGuiKey_RightCtrl,
        w.VK_RMENU => return c.ImGuiKey_RightAlt,
        w.VK_RWIN => return c.ImGuiKey_RightSuper,
        w.VK_APPS => return c.ImGuiKey_Menu,
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
        w.VK_F1 => return c.ImGuiKey_F1,
        w.VK_F2 => return c.ImGuiKey_F2,
        w.VK_F3 => return c.ImGuiKey_F3,
        w.VK_F4 => return c.ImGuiKey_F4,
        w.VK_F5 => return c.ImGuiKey_F5,
        w.VK_F6 => return c.ImGuiKey_F6,
        w.VK_F7 => return c.ImGuiKey_F7,
        w.VK_F8 => return c.ImGuiKey_F8,
        w.VK_F9 => return c.ImGuiKey_F9,
        w.VK_F10 => return c.ImGuiKey_F10,
        w.VK_F11 => return c.ImGuiKey_F11,
        w.VK_F12 => return c.ImGuiKey_F12,
        else => return c.ImGuiKey_None,
    }
}

pub fn newImGuiFrame(delta_time: f32) void {
    assert(c.igGetCurrentContext() != null);

    var ui = c.igGetIO().?;
    var ui_backend = @ptrCast(*GuiBackendState, @alignCast(@sizeOf(usize), ui.*.BackendPlatformUserData));
    assert(ui_backend.*.window != null);

    var rect: w.RECT = undefined;
    _ = w.GetClientRect(ui_backend.*.window.?, &rect);
    const viewport_width = @intToFloat(f32, rect.right - rect.left);
    const viewport_height = @intToFloat(f32, rect.bottom - rect.top);

    ui.*.DisplaySize = c.ImVec2{ .x = viewport_width, .y = viewport_height };
    ui.*.DeltaTime = delta_time;
    c.igNewFrame();

    if (c.igIsKeyDown(c.ImGuiKey_LeftShift) and !isVkKeyDown(w.VK_LSHIFT)) {
        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftShift, false);
    }
    if (c.igIsKeyDown(c.ImGuiKey_RightShift) and !isVkKeyDown(w.VK_RSHIFT)) {
        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_RightShift, false);
    }

    if (c.igIsKeyDown(c.ImGuiKey_LeftSuper) and !isVkKeyDown(w.VK_LWIN)) {
        c.ImGuiIO_AddKeyEvent(ui, c.ImGuiKey_LeftSuper, false);
    }
    if (c.igIsKeyDown(c.ImGuiKey_LeftSuper) and !isVkKeyDown(w.VK_RWIN)) {
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
    _ = w.ole32.CoInitializeEx(
        null,
        @enumToInt(w.COINIT_APARTMENTTHREADED) | @enumToInt(w.COINIT_DISABLE_OLE1DDE),
    );
    _ = w.SetProcessDPIAware();

    // Check if Windows version is supported.
    var version: w.OSVERSIONINFOW = undefined;
    _ = w.ntdll.RtlGetVersion(&version);

    var os_is_supported = false;
    if (version.dwMajorVersion > 10) {
        os_is_supported = true;
    } else if (version.dwMajorVersion == 10 and version.dwBuildNumber >= 18363) {
        os_is_supported = true;
    }

    const d3d12core_dll = w.kernel32.LoadLibraryW(L("D3D12Core.dll"));
    if (d3d12core_dll == null) {
        os_is_supported = false;
    } else {
        _ = w.kernel32.FreeLibrary(d3d12core_dll.?);
    }

    if (!os_is_supported) {
        _ = w.user32.messageBoxA(
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
            w.user32.MB_OK | w.user32.MB_ICONERROR,
        ) catch 0;
        w.kernel32.ExitProcess(0);
    }

    // Change directory to where an executable is located.
    var exe_path_buffer: [1024]u8 = undefined;
    const exe_path = std.fs.selfExeDirPath(exe_path_buffer[0..]) catch "./";
    std.os.chdir(exe_path) catch {};

    // Check if 'd3d12' folder is present next to an executable.
    const local_d3d12core_dll = w.kernel32.LoadLibraryW(L("d3d12/D3D12Core.dll"));
    if (local_d3d12core_dll == null) {
        _ = w.user32.messageBoxA(
            null,
            \\Looks like 'd3d12' folder is missing. It has to be distributed together with an application.
        ,
            "Error",
            w.user32.MB_OK | w.user32.MB_ICONERROR,
        ) catch 0;
        w.kernel32.ExitProcess(0);
    } else {
        _ = w.kernel32.FreeLibrary(local_d3d12core_dll.?);
    }
}

pub fn deinit() void {
    w.ole32.CoUninitialize();
}

pub fn parseAndLoadGltfFile(gltf_path: []const u8) *c.cgltf_data {
    var data: *c.cgltf_data = undefined;
    const options = std.mem.zeroes(c.cgltf_options);
    // Parse.
    {
        const result = c.cgltf_parse_file(&options, gltf_path.ptr, @ptrCast([*c][*c]c.cgltf_data, &data));
        assert(result == c.cgltf_result_success);
    }
    // Load.
    {
        const result = c.cgltf_load_buffers(&options, data, gltf_path.ptr);
        assert(result == c.cgltf_result_success);
    }
    return data;
}

pub fn appendMeshPrimitive(
    data: *c.cgltf_data,
    mesh_index: u32,
    prim_index: u32,
    indices: *std.ArrayList(u32),
    positions: *std.ArrayList([3]f32),
    normals: ?*std.ArrayList([3]f32),
    texcoords0: ?*std.ArrayList([2]f32),
    tangents: ?*std.ArrayList([4]f32),
) void {
    assert(mesh_index < data.meshes_count);
    assert(prim_index < data.meshes[mesh_index].primitives_count);
    const num_vertices: u32 = @intCast(u32, data.meshes[mesh_index].primitives[prim_index].attributes[0].data.*.count);
    const num_indices: u32 = @intCast(u32, data.meshes[mesh_index].primitives[prim_index].indices.*.count);

    // Indices.
    {
        indices.ensureTotalCapacity(indices.items.len + num_indices) catch unreachable;

        const accessor = data.meshes[mesh_index].primitives[prim_index].indices;

        assert(accessor.*.buffer_view != null);
        assert(accessor.*.stride == accessor.*.buffer_view.*.stride or accessor.*.buffer_view.*.stride == 0);
        assert((accessor.*.stride * accessor.*.count) == accessor.*.buffer_view.*.size);
        assert(accessor.*.buffer_view.*.buffer.*.data != null);

        const data_addr = @alignCast(4, @ptrCast([*]const u8, accessor.*.buffer_view.*.buffer.*.data) +
            accessor.*.offset + accessor.*.buffer_view.*.offset);

        if (accessor.*.stride == 1) {
            assert(accessor.*.component_type == c.cgltf_component_type_r_8u);
            const src = @ptrCast([*]const u8, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.appendAssumeCapacity(src[i]);
            }
        } else if (accessor.*.stride == 2) {
            assert(accessor.*.component_type == c.cgltf_component_type_r_16u);
            const src = @ptrCast([*]const u16, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.appendAssumeCapacity(src[i]);
            }
        } else if (accessor.*.stride == 4) {
            assert(accessor.*.component_type == c.cgltf_component_type_r_32u);
            const src = @ptrCast([*]const u32, data_addr);
            var i: u32 = 0;
            while (i < num_indices) : (i += 1) {
                indices.appendAssumeCapacity(src[i]);
            }
        } else {
            unreachable;
        }
    }

    // Attributes.
    {
        positions.resize(positions.items.len + num_vertices) catch unreachable;
        if (normals != null) normals.?.resize(normals.?.items.len + num_vertices) catch unreachable;
        if (texcoords0 != null) texcoords0.?.resize(texcoords0.?.items.len + num_vertices) catch unreachable;
        if (tangents != null) tangents.?.resize(tangents.?.items.len + num_vertices) catch unreachable;

        const num_attribs: u32 = @intCast(u32, data.meshes[mesh_index].primitives[prim_index].attributes_count);

        var attrib_index: u32 = 0;
        while (attrib_index < num_attribs) : (attrib_index += 1) {
            const attrib = &data.meshes[mesh_index].primitives[prim_index].attributes[attrib_index];
            const accessor = attrib.data;

            assert(accessor.*.buffer_view != null);
            assert(accessor.*.stride == accessor.*.buffer_view.*.stride or accessor.*.buffer_view.*.stride == 0);
            assert((accessor.*.stride * accessor.*.count) == accessor.*.buffer_view.*.size);
            assert(accessor.*.buffer_view.*.buffer.*.data != null);

            const data_addr = @ptrCast([*]const u8, accessor.*.buffer_view.*.buffer.*.data) +
                accessor.*.offset + accessor.*.buffer_view.*.offset;

            if (attrib.*.type == c.cgltf_attribute_type_position) {
                assert(accessor.*.type == c.cgltf_type_vec3);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                @memcpy(
                    @ptrCast([*]u8, &positions.items[positions.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            } else if (attrib.*.type == c.cgltf_attribute_type_normal and normals != null) {
                assert(accessor.*.type == c.cgltf_type_vec3);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                @memcpy(
                    @ptrCast([*]u8, &normals.?.items[normals.?.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            } else if (attrib.*.type == c.cgltf_attribute_type_texcoord and texcoords0 != null) {
                assert(accessor.*.type == c.cgltf_type_vec2);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                @memcpy(
                    @ptrCast([*]u8, &texcoords0.?.items[texcoords0.?.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            } else if (attrib.*.type == c.cgltf_attribute_type_tangent and tangents != null) {
                assert(accessor.*.type == c.cgltf_type_vec4);
                assert(accessor.*.component_type == c.cgltf_component_type_r_32f);
                @memcpy(
                    @ptrCast([*]u8, &tangents.?.items[tangents.?.items.len - num_vertices]),
                    data_addr,
                    accessor.*.count * accessor.*.stride,
                );
            }
        }
    }
}
