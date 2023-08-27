const std = @import("std");
const glfw = @import("zglfw");
const gl = @import("zopengl");

pub fn main() !void {
    try glfw.init();
    defer glfw.terminate();

    const gl_major = 3;
    const gl_minor = 3;
    glfw.WindowHint.set(.context_version_major, gl_major);
    glfw.WindowHint.set(.context_version_minor, gl_minor);
    glfw.WindowHint.set(.opengl_profile, @intFromEnum(glfw.OpenGLProfile.opengl_core_profile));
    glfw.WindowHint.set(.opengl_forward_compat, 1);
    glfw.WindowHint.set(.client_api, @intFromEnum(glfw.ClientApi.opengl_api));
    glfw.WindowHint.set(.doublebuffer, 1);

    const window = try glfw.Window.create(600, 600, "zig-gamedev: minimal_glfw_gl", null);
    defer window.destroy();

    glfw.makeContextCurrent(window);

    try gl.loadCoreProfile(glfw.getProcAddress, gl_major, gl_minor);

    glfw.swapInterval(1);

    while (!window.shouldClose()) {
        glfw.pollEvents();

        gl.clearBufferfv(gl.COLOR, 0, &[_]f32{ 0.2, 0.6, 0.4, 1.0 });

        window.swapBuffers();
    }
}
