const std = @import("std");

const Options = @import("../../build.zig").Options;

const demo_name = "minimal_sdl_gl";

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = b.path(b.pathJoin(&.{ src_path, demo_name ++ ".zig" })),
        .target = options.target,
        .optimize = options.optimize,
    });

    const zsdl = b.dependency("zsdl", .{
        .target = options.target,
    });
    exe.root_module.addImport("zsdl2", zsdl.module("zsdl2"));

    @import("zsdl").addLibraryPathsTo(exe);
    @import("zsdl").link_SDL2(exe);

    @import("zsdl").install_sdl2(&exe.step, options.target.result, .bin);

    const zopengl = b.dependency("zopengl", .{});
    exe.root_module.addImport("zopengl", zopengl.module("root"));

    return exe;
}
