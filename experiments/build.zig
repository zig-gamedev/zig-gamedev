const std = @import("std");

pub fn build(b: *std.Build, options: anytype) void {
    @import("genart/build.zig").build(b, options);
}
