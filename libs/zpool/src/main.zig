// zpool v0.1

pub const handles = @import("handles.zig");
pub const pools = @import("pools.zig");

pub const Handle = handles.Handle;
pub const Pool = pools.Pool;

// ensure transitive closure of test coverage
comptime {
    _ = Handle;
    _ = Pool;
}