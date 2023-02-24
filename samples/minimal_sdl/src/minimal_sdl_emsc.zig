const std = @import("std");

const minsdl = @import("minsdl.zig");

pub const std_options = struct {
    pub const logFn = logOverride;
};

fn logOverride(
    comptime message_level: std.log.Level,
    comptime scope: @Type(.EnumLiteral),
    comptime format: []const u8,
    args: anytype,
) void {
    _ = message_level;
    _ = scope;
    _ = format;
    _ = args;
}

export fn emsc_main() void {
    minsdl.run() catch unreachable;
}
