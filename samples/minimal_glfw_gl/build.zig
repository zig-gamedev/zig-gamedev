const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = "minimal_glfw_gl",
        .root_source_file = .{ .path = thisDir() ++ "/src/minimal_glfw_gl.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    @import("system_sdk").addLibraryPathsTo(exe);

    const zglfw = b.dependency("zglfw", .{
        .target = options.target,
    });
    exe.root_module.addImport("zglfw", zglfw.module("root"));
    exe.linkLibrary(zglfw.artifact("glfw"));

    const zopengl = b.dependency("zopengl", .{
        .target = options.target,
    });
    exe.root_module.addImport("zopengl", zopengl.module("root"));

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
