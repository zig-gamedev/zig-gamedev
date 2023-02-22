const std = @import("std");
const sdl = @import("zsdl");

pub fn main() !void {
    try sdl.init(.{ .audio = true, .video = true });
    defer sdl.quit();

    try sdl.gl.setAttribute(.context_profile_mask, @enumToInt(sdl.gl.Profile.core));
    try sdl.gl.setAttribute(.context_major_version, 4);
    try sdl.gl.setAttribute(.context_minor_version, 1);
    try sdl.gl.setAttribute(.context_flags, @bitCast(i32, sdl.gl.ContextFlags{ .forward_compatible = true }));
    try sdl.gl.setAttribute(.red_size, 8);
    try sdl.gl.setAttribute(.green_size, 8);
    try sdl.gl.setAttribute(.blue_size, 8);

    const window = try sdl.Window.create(
        "zig-gamedev-window",
        sdl.Window.pos_undefined,
        200,
        400,
        400,
        .{ .shown = true, .opengl = true },
    );
    defer window.destroy();

    _ = sdl.Scancode;
    _ = sdl.Keycode;

    const gl_context = try sdl.gl.createContext(window);
    defer sdl.gl.deleteContext(gl_context);

    try sdl.gl.makeCurrent(window, gl_context);

    _ = sdl.gl.getProcAddress("glBindBuffer");

    sdl.gl.swapWindow(window);

    std.debug.print("All OK\n", .{});
}
