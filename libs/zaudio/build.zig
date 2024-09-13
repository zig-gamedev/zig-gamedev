const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = b.addModule("root", .{
        .root_source_file = b.path("src/zaudio.zig"),
    });

    const miniaudio = b.addStaticLibrary(.{
        .name = "miniaudio",
        .target = target,
        .optimize = optimize,
    });

    b.installArtifact(miniaudio);

    miniaudio.addIncludePath(b.path("libs/miniaudio"));
    miniaudio.linkLibC();

    if (target.result.os.tag == .macos) {
        if (b.lazyDependency("system_sdk", .{})) |system_sdk| {
            miniaudio.addFrameworkPath(system_sdk.path("macos12/System/Library/Frameworks"));
            miniaudio.addSystemIncludePath(system_sdk.path("macos12/usr/include"));
            miniaudio.addLibraryPath(system_sdk.path("macos12/usr/lib"));
        }
        miniaudio.linkFramework("CoreAudio");
        miniaudio.linkFramework("CoreFoundation");
        miniaudio.linkFramework("AudioUnit");
        miniaudio.linkFramework("AudioToolbox");
    } else if (target.result.os.tag == .linux) {
        miniaudio.linkSystemLibrary("pthread");
        miniaudio.linkSystemLibrary("m");
        miniaudio.linkSystemLibrary("dl");
    }

    miniaudio.addCSourceFile(.{
        .file = b.path("src/zaudio.c"),
        .flags = &.{"-std=c99"},
    });
    miniaudio.addCSourceFile(.{
        .file = b.path("libs/miniaudio/miniaudio.c"),
        .flags = &.{
            "-DMA_NO_WEBAUDIO",
            "-DMA_NO_ENCODING",
            "-DMA_NO_NULL",
            "-DMA_NO_JACK",
            "-DMA_NO_DSOUND",
            "-DMA_NO_WINMM",
            "-std=c99",
            "-fno-sanitize=undefined",
            if (target.result.os.tag == .macos) "-DMA_NO_RUNTIME_LINKING" else "",
        },
    });

    const test_step = b.step("test", "Run zaudio tests");

    const tests = b.addTest(.{
        .name = "zaudio-tests",
        .root_source_file = b.path("src/zaudio.zig"),
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(tests);

    tests.linkLibrary(miniaudio);

    test_step.dependOn(&b.addRunArtifact(tests).step);
}
