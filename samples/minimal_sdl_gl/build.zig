const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = "minimal_sdl_gl",
        .root_source_file = .{ .path = thisDir() ++ "/src/minimal_sdl_gl.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const zsdl = b.dependency("zsdl", .{
        .target = options.target,
    });
    const zsdl_path = zsdl.path("").getPath(b);

    exe.root_module.addImport("zsdl2", zsdl.module("zsdl2"));

    @import("zsdl").addLibraryPathsTo(exe, zsdl_path) catch unreachable;
    @import("zsdl").link_SDL2(exe);

    @import("zsdl").install_sdl2(&exe.step, options.target.result, .bin, zsdl_path) catch unreachable;

    const zopengl = b.dependency("zopengl", .{});
    exe.root_module.addImport("zopengl", zopengl.module("root"));

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
