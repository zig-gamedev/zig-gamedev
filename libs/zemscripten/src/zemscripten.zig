//! Zig bindings and glue for Emscripten

const std = @import("std");

comptime {
    _ = std.testing.refAllDeclsRecursive(@This());
}

extern fn emscripten_err([*c]const u8) void;
extern fn emscripten_console_error([*c]const u8) void;
extern fn emscripten_console_warn([*c]const u8) void;
extern fn emscripten_console_log([*c]const u8) void;

pub const MainLoopCallback = *const fn () callconv(.C) void;
extern fn emscripten_set_main_loop(MainLoopCallback, c_int, c_int) void;
pub fn setMainLoop(cb: MainLoopCallback, maybe_fps: ?i16, simulate_infinite_loop: bool) void {
    emscripten_set_main_loop(cb, if (maybe_fps) |fps| fps else -1, @intFromBool(simulate_infinite_loop));
}

pub const AnimationFrameCallback = *const fn (f64, ?*anyopaque) callconv(.C) c_int;
extern fn emscripten_request_animation_frame_loop(AnimationFrameCallback, ?*anyopaque) void;
pub const requestAnimationFrameLoop = emscripten_request_animation_frame_loop;

/// std.panic impl
pub fn panic(msg: []const u8, error_return_trace: ?*std.builtin.StackTrace, ret_addr: ?usize) noreturn {
    _ = error_return_trace;
    _ = ret_addr;

    var buf: [1024]u8 = undefined;
    const error_msg: [:0]u8 = std.fmt.bufPrintZ(&buf, "PANIC! {s}", .{msg}) catch unreachable;
    emscripten_err(error_msg.ptr);

    while (true) {
        @breakpoint();
    }
}

/// std.log impl
pub fn log(
    comptime level: std.log.Level,
    comptime scope: @TypeOf(.EnumLiteral),
    comptime format: []const u8,
    args: anytype,
) void {
    const level_txt = comptime level.asText();
    const prefix2 = if (scope == .default) ": " else "(" ++ @tagName(scope) ++ "): ";
    const prefix = level_txt ++ prefix2;

    var buf: [1024]u8 = undefined;
    const msg = std.fmt.bufPrintZ(buf[0 .. buf.len - 1], prefix ++ format, args) catch |err| {
        switch (err) {
            error.NoSpaceLeft => {
                emscripten_console_error("log message too long, skipped.");
                return;
            },
        }
    };
    switch (level) {
        .err => emscripten_console_error(@ptrCast(msg.ptr)),
        .warn => emscripten_console_warn(@ptrCast(msg.ptr)),
        else => emscripten_console_log(@ptrCast(msg.ptr)),
    }
}
