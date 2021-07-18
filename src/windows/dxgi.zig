const std = @import("std");
usingnamespace std.os.windows;
usingnamespace @import("windows.zig");

pub const DXGI_USAGE = packed struct {
    __reserved0: bool align(4) = false,
    __reserved1: bool = false,
    __reserved2: bool = false,
    __reserved3: bool = false,
    SHADER_INPUT: bool = false, // 0x10
    RENDER_TARGET_OUTPUT: bool = false, // 0x20
    BACK_BUFFER: bool = false, // 0x40
    SHARED: bool = false, // 0x80
    READ_ONLY: bool = false, // 0x100
    DISCARD_ON_PRESENT: bool = false, // 0x200
    UNORDERED_ACCESS: bool = false, // 0x400
    __reserved12: bool = false,
    __reserved13: bool = false,
    __reserved14: bool = false,
    __reserved15: bool = false,
    __reserved16: bool = false,
    __reserved17: bool = false,
    __reserved18: bool = false,
    __reserved19: bool = false,
    __reserved20: bool = false,
    __reserved21: bool = false,
    __reserved22: bool = false,
    __reserved23: bool = false,
    __reserved24: bool = false,
    __reserved25: bool = false,
    __reserved26: bool = false,
    __reserved27: bool = false,
    __reserved28: bool = false,
    __reserved29: bool = false,
    __reserved30: bool = false,
    __reserved31: bool = false,
};
comptime {
    std.debug.assert(@sizeOf(DXGI_USAGE) == 4);
    std.debug.assert(@alignOf(DXGI_USAGE) == 4);
}
