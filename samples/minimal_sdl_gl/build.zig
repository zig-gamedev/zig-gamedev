const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = "minimal_sdl_gl",
        .root_source_file = .{ .path = thisDir() ++ "/src/minimal_sdl_gl.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const zsdl_pkg = @import("../../build.zig").zsdl_pkg;
    const zopengl_pkg = @import("../../build.zig").zopengl_pkg;

    zsdl_pkg.link(exe);
    zopengl_pkg.link(exe);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
