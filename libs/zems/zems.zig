pub const is_emscripten = true;

const std = @import("std");
pub const c = @cImport({
    @cInclude("emscripten/emscripten.h");
    @cInclude("emscripten/console.h");
    @cInclude("emscripten/html5.h");
    @cInclude("emscripten/emmalloc.h");
});

// Most common bindings, for others see emscripten headers included within `c`
// https://emscripten.org/docs/api_reference/index.html
pub const EmBool = enum(c_int) {
    true = 1,
    false = 0,
};

pub const AnimationFrameCallback = *const fn (time: f64, user_data: ?*anyopaque) callconv(.C) EmBool;

pub inline fn requestAnimationFrame(cb: AnimationFrameCallback, user_data: ?*anyopaque) c_long {
    return c.emscripten_request_animation_frame(cb, user_data);
}

pub inline fn requestAnimationFrameLoop(cb: AnimationFrameCallback, user_data: ?*anyopaque) void {
    return c.emscripten_request_animation_frame_loop(cb, user_data);
}

pub inline fn cancelAnimationFrame(request_animation_frame_id: c_long) void {
    c.emscripten_cancel_animation_frame(request_animation_frame_id);
}

pub inline fn sleep(ms: u32) void {
    c.emscripten_sleep(ms);
}

/// EmmalocAllocator allocator
/// use with linker flag -sMALLOC=emmalloc
/// for details see docs: https://github.com/emscripten-core/emscripten/blob/main/system/lib/emmalloc.c
pub const EmmalocAllocator = struct {
    const Self = @This();
    dummy: u32 = undefined,

    pub fn allocator(self: *Self) std.mem.Allocator {
        return .{
            .ptr = self,
            .vtable = &.{
                .alloc = &alloc,
                .resize = &resize,
                .free = &free,
            },
        };
    }

    fn alloc(
        ctx: *anyopaque,
        len: usize,
        ptr_align_log2: u8,
        return_address: usize,
    ) ?[*]u8 {
        _ = ctx;
        _ = return_address;
        const ptr_align: u32 = @as(u32, 1) << @as(u5, @intCast(ptr_align_log2));
        if (!std.math.isPowerOfTwo(ptr_align)) unreachable;
        const ptr = c.emmalloc_memalign(ptr_align, len) orelse return null;
        return @ptrCast(ptr);
    }

    fn resize(
        ctx: *anyopaque,
        buf: []u8,
        buf_align_log2: u8,
        new_len: usize,
        return_address: usize,
    ) bool {
        _ = ctx;
        _ = return_address;
        _ = buf_align_log2;
        return c.emmalloc_realloc_try(buf.ptr, new_len) != null;
    }

    fn free(
        ctx: *anyopaque,
        buf: []u8,
        buf_align_log2: u8,
        return_address: usize,
    ) void {
        _ = ctx;
        _ = buf_align_log2;
        _ = return_address;
        return c.emmalloc_free(buf.ptr);
    }
};

/// std.log function that writes to dev-tools console
pub fn emscriptenLog(
    comptime level: std.log.Level,
    comptime scope: @TypeOf(.EnumLiteral),
    comptime format: []const u8,
    args: anytype,
) void {
    const level_txt = comptime level.asText();
    const prefix2 = if (scope == .default) ": " else "(" ++ @tagName(scope) ++ "): ";
    const prefix = level_txt ++ prefix2;

    var buf: [1024 * 8]u8 = undefined;
    var slice = std.fmt.bufPrint(buf[0 .. buf.len - 1], prefix ++ format, args) catch {
        c.emscripten_console_error("emscriptenLog: formatting message failed - log message skipped!");
        return;
    };
    buf[slice.len] = 0;
    switch (level) {
        .err => c.emscripten_console_error(@ptrCast(slice.ptr)),
        .warn => c.emscripten_console_warn(@ptrCast(slice.ptr)),
        else => c.emscripten_console_log(@ptrCast(slice.ptr)),
    }
}

