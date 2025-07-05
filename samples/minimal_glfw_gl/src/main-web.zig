const std = @import("std");

const zemscripten = @import("zemscripten");
pub const panic = zemscripten.panic;

pub const std_options = std.Options{
    .logFn = zemscripten.log,
};

const minimal_glfw_gl = @import("minimal_glfw_gl.zig");

var initialised = false;

export fn main() c_int {
    zemscripten.setMainLoop(mainLoopCallback, null, false);
    return 0;
}

export fn mainLoopCallback() void {
    if (initialised == false) {
        minimal_glfw_gl.init(.{
            .api = .opengl_es_api,
            .version_major = 2,
            .version_minor = 0,
        }) catch |err| {
            std.log.err("minimal_glfw_gl.init failed with error: {s}", .{@errorName(err)});
            return;
        };
        initialised = true;
    }

    minimal_glfw_gl.updateAndRender();
}
