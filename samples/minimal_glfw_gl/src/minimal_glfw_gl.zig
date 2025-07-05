const std = @import("std");
const glfw = @import("zglfw");
const zopengl = @import("zopengl");

var window: *glfw.Window = undefined;

pub fn init(gl_settings: struct {
    api: glfw.ClientApi,
    version_major: u16,
    version_minor: u16,
}) !void {
    try glfw.init();

    glfw.windowHint(.client_api, gl_settings.api);
    glfw.windowHint(.context_version_major, gl_settings.version_major);
    glfw.windowHint(.context_version_minor, gl_settings.version_minor);
    glfw.windowHint(.opengl_profile, .opengl_core_profile);
    glfw.windowHint(.opengl_forward_compat, true);
    glfw.windowHint(.doublebuffer, true);

    window = try glfw.Window.create(600, 600, "zig-gamedev: minimal_glfw_gl", null);

    glfw.makeContextCurrent(window);

    switch (gl_settings.api) {
        .no_api => unreachable,
        .opengl_api => {
            try zopengl.loadCoreProfile(
                glfw.getProcAddress,
                gl_settings.version_major,
                gl_settings.version_minor,
            );
        },
        .opengl_es_api => {
            try zopengl.loadEsProfile(
                glfw.getProcAddress,
                gl_settings.version_major,
                gl_settings.version_minor,
            );
        },
    }

    glfw.swapInterval(1);
}

pub fn deinit() void {
    window.destroy();
    glfw.terminate();
}

pub fn shouldQuit() bool {
    return window.shouldClose();
}

pub fn updateAndRender() void {
    glfw.pollEvents();

    const gl = zopengl.bindings;
    
    gl.clearColor(0.12, 0.24, 0.36, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    window.swapBuffers();
}
