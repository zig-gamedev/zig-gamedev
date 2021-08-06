const std = @import("std");
const w = @import("../win32/win32.zig");
const c = @import("c.zig");

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

    pub fn update(self: *FrameStats) void {
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
    }
};

fn processWindowMessage(
    window: w.HWND,
    message: w.UINT,
    wparam: w.WPARAM,
    lparam: w.LPARAM,
) callconv(w.WINAPI) w.LRESULT {
    var ui = if (c.igGetCurrentContext() != null) c.igGetIO() else null;
    const processed = switch (message) {
        w.user32.WM_LBUTTONDOWN => blk: {
            if (ui != null) ui.?.*.MouseDown[0] = true;
            break :blk true;
        },
        w.user32.WM_LBUTTONUP => blk: {
            if (ui != null) ui.?.*.MouseDown[0] = false;
            break :blk true;
        },
        w.user32.WM_RBUTTONDOWN => blk: {
            if (ui != null) ui.?.*.MouseDown[1] = true;
            break :blk true;
        },
        w.user32.WM_RBUTTONUP => blk: {
            if (ui != null) ui.?.*.MouseDown[1] = false;
            break :blk true;
        },
        w.user32.WM_MBUTTONDOWN => blk: {
            if (ui != null) ui.?.*.MouseDown[2] = true;
            break :blk true;
        },
        w.user32.WM_MBUTTONUP => blk: {
            if (ui != null) ui.?.*.MouseDown[2] = false;
            break :blk true;
        },
        w.user32.WM_MOUSEWHEEL => blk: {
            if (ui != null) {
                const get_wheel_delta_wparam = @intCast(u16, ((wparam >> 16) & 0xffff));
                ui.?.*.MouseWheel += if ((get_wheel_delta_wparam & 0x8000) > 0) @as(f32, -1.0) else @as(f32, 1.0);
            }
            break :blk true;
        },
        w.user32.WM_MOUSEMOVE => blk: {
            if (ui != null) {
                ui.?.*.MousePos.x = @intToFloat(f32, @intCast(i16, lparam & 0xffff));
                ui.?.*.MousePos.y = @intToFloat(f32, @intCast(i16, (lparam & 0xffff_0000) >> 16));
            }
            break :blk true;
        },
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

pub fn initWindow(name: [*:0]const u8, width: u32, height: u32) !w.HWND {
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
    try w.user32.adjustWindowRectEx(&rect, style, false, 0);

    return try w.user32.createWindowExA(
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
}
