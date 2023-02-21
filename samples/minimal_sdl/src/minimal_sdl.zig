const std = @import("std");
const sdl = @import("zsdl");

pub fn main() !void {
    try sdl.init(.{ .audio = true, .video = true });
    defer sdl.quit();

    try sdl.setGlAttr(.context_profile_mask, @enumToInt(sdl.GlProfile.core));
    try sdl.setGlAttr(.context_major_version, 4);
    try sdl.setGlAttr(.context_minor_version, 1);
    try sdl.setGlAttr(.context_flags, @bitCast(i32, sdl.GlContextFlags{ .forward_compatible = true }));
    try sdl.setGlAttr(.red_size, 8);
    try sdl.setGlAttr(.green_size, 8);
    try sdl.setGlAttr(.blue_size, 8);
    const window = try sdl.Window.create(
        "zig-gamedev-window",
        sdl.Window.pos_undefined,
        200,
        400,
        400,
        .{ .shown = true, .opengl = true },
    );
    defer window.destroy();

    const gl_context = try sdl.createGlContext(window);
    defer sdl.deleteGlContext(gl_context);

    try sdl.makeGlContextCurrent(window, gl_context);

    _ = sdl.getGlProcAddress("glBindBuffer");

    sdl.swapGlWindow(window);

    std.debug.print("All OK\n", .{});
}
