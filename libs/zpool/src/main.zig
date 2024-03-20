pub const Handle = @import("handle.zig").Handle;
pub const Pool = @import("pool.zig").Pool;

// ensure transitive closure of test coverage
comptime {
    _ = Handle;
    _ = Pool;
}
