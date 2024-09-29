const std = @import("std");

const zemscripten = @import("zemscripten");
pub const panic = zemscripten.panic;

pub const std_options = std.Options{
    .logFn = zemscripten.log,
};

const sdl2_demo = @import("sdl2_demo.zig");

var initialised = false;

export fn main() c_int {
    zemscripten.setMainLoop(mainLoopCallback, null, false);
    return 0;
}

export fn mainLoopCallback() void {
    if (initialised == false) {
        sdl2_demo.init() catch |err| {
            std.log.err("sdl_demo.init failed with error: {s}", .{@errorName(err)});
            return;
        };
        initialised = true;
    }
    sdl2_demo.updateAndRender() catch |err| {
        std.log.err("sdl_demo.updateAndRender failed with error: {s}", .{@errorName(err)});
    };
}
