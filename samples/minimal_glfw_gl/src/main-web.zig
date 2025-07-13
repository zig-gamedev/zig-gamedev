const std = @import("std");
const glfw = @import("zglfw");
const zopengl = @import("zopengl");

const zemscripten = @import("zemscripten");
pub const panic = zemscripten.panic;

pub const std_options = std.Options{
    .logFn = zemscripten.log,
};

const minimal_glfw_gl = @import("minimal_glfw_gl.zig");

const gl_es_version_major: u16 = 2;
const gl_es_version_minor: u16 = 0;

export fn main() c_int {
    init() catch |err| {
        std.log.err("Initialization failed with error: {s}", .{@errorName(err)});
        return -1;
    };

    return 0;
}

export fn mainLoopCallback() void {
    minimal_glfw_gl.updateAndRender();
}

fn init() !void {
    try glfw.init();

    glfw.windowHint(.context_version_major, gl_es_version_major);
    glfw.windowHint(.context_version_minor, gl_es_version_minor);
    glfw.windowHint(.doublebuffer, true);

    try minimal_glfw_gl.init();

    try zopengl.loadEsProfile(glfw.getProcAddress, gl_es_version_major, gl_es_version_minor);

    zemscripten.setMainLoop(mainLoopCallback, null, false);
}
