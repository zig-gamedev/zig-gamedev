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
    exe.root_module.addImport("zsdl2", zsdl.module("zsdl2"));
    @import("zsdl").link_SDL2(exe);

    switch (options.target.result.os.tag) {
        .windows => {
            if (options.target.result.cpu.arch.isX86()) {
                exe.addLibraryPath(
                    .{ .path = zsdl.path("libs/x86_64-windows-gnu/lib").getPath(b) },
                );
            }
        },
        .linux => {
            if (options.target.result.cpu.arch.isX86()) {
                exe.addLibraryPath(
                    .{ .path = zsdl.path("libs/x86_64-linux-gnu/lib").getPath(b) },
                );
            }
        },
        .macos => {
            exe.addFrameworkPath(
                .{ .path = zsdl.path("libs/macos/Frameworks").getPath(b) },
            );
        },
        else => {},
    }

    const zopengl = b.dependency("zopengl", .{});
    exe.root_module.addImport("zopengl", zopengl.module("root"));

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
