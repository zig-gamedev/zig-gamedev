const std = @import("std");

pub const demo_name = "minimal_sdl_gl";

pub fn build(b: *std.Build, options: anytype) *std.Build.Step.Compile {
    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_module = b.createModule(.{
            .root_source_file = b.path(b.pathJoin(&.{ src_path, demo_name ++ ".zig" })),
            .target = options.target,
            .optimize = options.optimize,
            .link_libc = true,
        }),
    });

    const zsdl = b.dependency("zsdl", .{});
    exe.root_module.addImport("zsdl2", zsdl.module("zsdl2"));

    @import("zsdl").prebuilt_sdl2.addLibraryPathsTo(exe);
    switch (exe.rootModuleTarget().os.tag) {
        .windows => {
            exe.root_module.linkSystemLibrary("SDL2", .{});
            exe.root_module.linkSystemLibrary("SDL2main", .{});
            exe.root_module.linkSystemLibrary("SDL2_ttf", .{});
            exe.root_module.linkSystemLibrary("SDL2_image", .{});
        },
        .linux => {
            exe.root_module.linkSystemLibrary("SDL2", .{});
            exe.root_module.linkSystemLibrary("SDL2_ttf", .{});
            exe.root_module.linkSystemLibrary("SDL2_image", .{});
            exe.root_module.addRPathSpecial("$ORIGIN");
        },
        .macos => {
            exe.root_module.linkFramework("SDL2", .{});
            exe.root_module.linkFramework("SDL2_ttf", .{});
            exe.root_module.linkFramework("SDL2_image", .{});
            exe.root_module.addRPathSpecial("@executable_path");
        },
        else => {},
    }

    const zopengl = b.dependency("zopengl", .{});
    exe.root_module.addImport("zopengl", zopengl.module("root"));

    return exe;
}
