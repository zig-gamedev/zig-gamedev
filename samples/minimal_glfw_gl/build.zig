const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = "minimal_glfw_gl",
        .root_source_file = .{ .path = thisDir() ++ "/src/minimal_glfw_gl.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const zglfw_pkg = @import("../../build.zig").zglfw_pkg;
    const zopengl_pkg = @import("../../build.zig").zopengl_pkg;

    zglfw_pkg.link(exe);
    zopengl_pkg.link(exe);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
