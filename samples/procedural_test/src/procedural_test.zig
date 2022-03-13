const glfw = @import("glfw");
const zgl = @import("zminigl");

const window_name = "zig-gamedev: procedural test";
const window_width = 1800;
const window_height = 1000;

pub fn main() !void {
    try glfw.init(.{});
    defer glfw.terminate();

    const window = try glfw.Window.create(window_width, window_height, window_name, null, null, .{
        .opengl_profile = .opengl_core_profile,
        .context_version_major = 4,
        .context_version_minor = 6,
        .opengl_forward_compat = true,
    });
    defer window.destroy();

    try glfw.makeContextCurrent(window);

    zgl.init(glfw.getProcAddress);

    while (!window.shouldClose()) {
        try glfw.pollEvents();
        zgl.clearNamedFramebufferfv(zgl.default_framebuffer, .color, 0, &.{ 0.0, 0.6, 0.0, 1.0 });
        try window.swapBuffers();
    }
}
