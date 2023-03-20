const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    const exe = b.addExecutable(.{
        .name = "minimal_gl",
        .root_source_file = .{ .path = thisDir() ++ "/src/minimal_gl.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const zsdl_pkg = @import("../../build.zig").zsdl_pkg;
    const zopengl_pkg = @import("../../build.zig").zopengl_pkg;

    exe.addModule("zsdl", zsdl_pkg.zsdl);
    exe.addModule("zopengl", zopengl_pkg.zopengl);
    zsdl_pkg.link(b, exe);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
