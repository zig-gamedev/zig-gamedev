const std = @import("std");
const sdl2 = @import("sdl2.zig");

test "compiled version should be same as linked version" {
    try std.testing.expectEqual(sdl2.VERSION, sdl2.getVersion());
}
