// zmesh v0.2

pub const Shape = @import("Shape.zig");
pub const gltf = @import("gltf.zig");
pub usingnamespace @import("meshoptimizer.zig");

pub const Error = error{
    FileNotFound,
};

pub fn init(alloc: std.mem.Allocator) void {
    mem.init(alloc);
}

pub fn deinit() void {
    mem.deinit();
}

const std = @import("std");
const mem = @import("memory.zig");

comptime {
    _ = Shape;
}
