const std = @import("std");
const sdl = @import("zsdl2");

comptime {
    _ = std.testing.refAllDecls(@This());
}

/// Load an image from a filesystem path into a software surface.
pub fn load(file: [:0]const u8) !*sdl.Surface {
    return IMG_Load(file) orelse sdl.makeError();
}
extern fn IMG_Load(file: [*:0]const u8) ?*sdl.Surface;

/// Load an XPM image from a memory array.
pub fn readXpmFromArray(xpm: *[*:0]const u8) !*sdl.Surface {
    return IMG_ReadXPMFromArray(xpm) orelse sdl.makeError();
}
extern fn IMG_ReadXPMFromArray(xpm: *[*:0]const u8) ?*sdl.Surface;
