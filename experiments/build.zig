const std = @import("std");

pub const Options = struct {
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
};

pub fn buildWithOptions(
    b: *std.Build,
    options: Options,
) void {
    @import("genart/build.zig").buildWithOptions(b, options);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
