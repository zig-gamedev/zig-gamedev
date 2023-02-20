const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    const exe = b.addExecutable(.{
        .name = "minimal_sdl",
        .target = options.target,
        .optimize = options.optimize,
    });

    const target = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;

    switch (target.os.tag) {
        .windows => {
            exe.addIncludePath(thisDir() ++ "/../../libs/zsdl/libs/x86_64-windows-gnu/include");
            exe.addLibraryPath(thisDir() ++ "/../../libs/zsdl/libs/x86_64-windows-gnu/lib");
            exe.linkSystemLibraryName("SDL2");
            exe.linkSystemLibraryName("SDL2main");

            exe.step.dependOn(
                &exe.builder.addInstallFile(
                    .{ .path = thisDir() ++ "/../../libs/zsdl/libs/x86_64-windows-gnu/bin/SDL2.dll" },
                    "bin/SDL2.dll",
                ).step,
            );
        },
        .linux => {
            exe.addIncludePath(thisDir() ++ "/../../libs/zsdl/libs/x86_64-linux-gnu/include");
            exe.addLibraryPath(thisDir() ++ "/../../libs/zsdl/libs/x86_64-linux-gnu/lib");
            exe.linkSystemLibraryName("SDL2-2.0");
            exe.addRPath("$ORIGIN");

            exe.step.dependOn(
                &exe.builder.addInstallFile(
                    .{ .path = thisDir() ++ "/../../libs/zsdl/libs/x86_64-linux-gnu/lib/libSDL2-2.0.so" },
                    "bin/libSDL2-2.0.so.0",
                ).step,
            );
        },
        .macos => {
            exe.addFrameworkPath(thisDir() ++ "/../../libs/zsdl/libs/macos/Frameworks");
            exe.linkFramework("SDL2");
            exe.addRPath("@loader_path");

            const install_dir_step = b.addInstallDirectory(.{
                .source_dir = thisDir() ++ "/../../libs/zsdl/libs/macos/Frameworks/SDL2.framework",
                .install_dir = .{ .custom = "" },
                .install_subdir = "bin/SDL2.framework",
            });
            exe.step.dependOn(&install_dir_step.step);
        },
        else => unreachable,
    }

    exe.addCSourceFile(thisDir() ++ "/src/minimal_sdl.c", &.{});
    exe.linkLibC();

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
