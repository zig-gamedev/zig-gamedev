//--------------------------------------------------------------------------------------------------
//
// SIMD math library for game developers
// https://github.com/michal-z/zig-gamedev/tree/main/libs/zmath
//
// See zmath.zig for more details.
// See util.zig for additional functionality.
//
//--------------------------------------------------------------------------------------------------
pub usingnamespace @import("zmath.zig");
pub const util = @import("util.zig");

// ensure transitive closure of test coverage
comptime {
    _ = util;
}
