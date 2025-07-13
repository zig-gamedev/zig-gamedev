const std = @import("std");
const glfw = @import("zglfw");
const zopengl = @import("zopengl");

pub var window: *glfw.Window = undefined;

pub fn init() !void {
    window = try glfw.Window.create(600, 600, "zig-gamedev: minimal_glfw_gl", null);

    glfw.makeContextCurrent(window);

    glfw.swapInterval(1);
}

pub fn deinit() void {
    window.destroy();
}

pub fn updateAndRender() void {
    glfw.pollEvents();

    const gl = zopengl.bindings;

    gl.clearColor(0.12, 0.24, 0.36, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    window.swapBuffers();
}
