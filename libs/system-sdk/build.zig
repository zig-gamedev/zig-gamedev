const std = @import("std");

pub const path = getPath();

inline fn getPath() []const u8 {
    return std.fs.path.dirname(@src().file) orelse unreachable;
}

pub fn build(_: *std.Build) void {}
