pub const version = @import("std").SemanticVersion{ .major = 0, .minor = 9, .patch = 0 };

pub const Shape = @import("Shape.zig");
pub const io = @import("io.zig");
pub const opt = @import("zmeshoptimizer.zig");

const std = @import("std");
pub const mem = @import("memory.zig");

pub fn init(alloc: std.mem.Allocator) void {
    mem.init(alloc);
}

pub fn deinit() void {
    mem.deinit();
}

comptime {
    _ = Shape;
}
