const std = @import("std");

const demo_name = "minimal_sdl_gl";

pub fn build(b: *std.Build, options: anytype) *std.Build.Step.Compile {
    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = b.path(b.pathJoin(&.{ src_path, demo_name ++ ".zig" })),
        .target = options.target,
        .optimize = options.optimize,
    });

    const zsdl = b.dependency("zsdl", .{});
    exe.root_module.addImport("zsdl2", zsdl.module("zsdl2"));

    @import("zsdl").link_SDL2(exe);

    const sdl2_libs_path = b.dependency("sdl2-prebuilt", .{}).path("").getPath(b);

    @import("zsdl").addLibraryPathsTo(sdl2_libs_path, exe);
    @import("zsdl").addRPathsTo(sdl2_libs_path, exe);

    if (@import("zsdl").install_SDL2(b, options.target.result, sdl2_libs_path, .bin)) |install_sdl2_step| {
        b.getInstallStep().dependOn(install_sdl2_step);
    }

    const zopengl = b.dependency("zopengl", .{});
    exe.root_module.addImport("zopengl", zopengl.module("root"));

    return exe;
}
