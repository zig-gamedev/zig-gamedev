const std = @import("std");
const Options = @import("../build.zig").Options;

pub fn build(b: *std.Build, options: Options) void {
    @import("genart/build.zig").build(b, options);
}
