const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    const exe = b.addExecutable(.{
        .name = "minimal_sdl",
        .target = options.target,
        .optimize = options.optimize,
    });

    exe.linkLibC();
    exe.addIncludePath(thisDir() ++ "/../../libs/system-sdk/include");
    exe.addIncludePath(thisDir() ++ "/../../libs/system-sdk/linux/include/x86_64-linux-gnu");
    exe.addLibraryPath(thisDir() ++ "/../../libs/system-sdk/linux/lib/x86_64-linux-gnu");
    exe.linkSystemLibraryName("SDL2-2.0");
    exe.addCSourceFile(thisDir() ++ "/src/minimal_sdl.c", &.{});
    exe.addRPath(".");

    exe.step.dependOn(
        &exe.builder.addInstallFile(
            .{ .path = thisDir() ++ "/../../libs/system-sdk/linux/lib/x86_64-linux-gnu/libSDL2-2.0.so.0" },
            "bin/libSDL2-2.0.so.0",
        ).step,
    );

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
