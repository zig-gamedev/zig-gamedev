const std = @import("std");

extern "C" fn emmalloc_memalign(alignment: usize, size: usize) ?*anyopaque;
extern "C" fn emmalloc_realloc_try(ptr: ?*anyopaque, size: usize) ?*anyopaque;
extern "C" fn emmalloc_free(ptr: ?*anyopaque) void;

/// Zig Allocator that wraps emmalloc
/// use with linker flag -sMALLOC=emmalloc
pub const EmmallocAllocator = struct {
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
        const ptr_align = @as(usize, 1) << @as(u5, @intCast(ptr_align_log2));
        if (!std.math.isPowerOfTwo(ptr_align)) unreachable;
        const ptr = emmalloc_memalign(ptr_align, len) orelse return null;
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
        return emmalloc_realloc_try(buf.ptr, new_len) != null;
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
        return emmalloc_free(buf.ptr);
    }
};

extern "C" fn emscripten_console_log([*c]const u8) void;
extern "C" fn emscripten_console_warn([*c]const u8) void;
extern "C" fn emscripten_console_error([*c]const u8) void;

/// std.log function that writes to dev-tools console
pub fn log(
    comptime level: std.log.Level,
    comptime scope: @TypeOf(.EnumLiteral),
    comptime format: []const u8,
    args: anytype,
) void {
    const level_txt = comptime level.asText();
    const prefix2 = if (scope == .default) ": " else "(" ++ @tagName(scope) ++ "): ";
    const prefix = level_txt ++ prefix2;

    var buf: [1024 * 4]u8 = undefined;
    const slice = std.fmt.bufPrint(buf[0 .. buf.len - 1], prefix ++ format, args) catch {
        emscripten_console_error("zemsc: log message too long, skipped!");
        return;
    };
    buf[slice.len] = 0;
    switch (level) {
        .err => emscripten_console_error(@ptrCast(slice.ptr)),
        .warn => emscripten_console_warn(@ptrCast(slice.ptr)),
        else => emscripten_console_log(@ptrCast(slice.ptr)),
    }
}
