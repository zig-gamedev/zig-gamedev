const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = b.addModule("root", .{
        .root_source_file = b.path("src/zstbi.zig"),
    });

    const zstbi_lib = b.addStaticLibrary(.{
        .name = "zstbi",
        .target = target,
        .optimize = optimize,
    });
    zstbi_lib.addIncludePath(b.path("libs/stbi"));
    if (optimize == .Debug) {
        // TODO: Workaround for Zig bug.
        zstbi_lib.addCSourceFile(.{
            .file = b.path("src/zstbi.c"),
            .flags = &.{
                "-std=c99",
                "-fno-sanitize=undefined",
                "-g",
                "-O0",
            },
        });
    } else {
        zstbi_lib.addCSourceFile(.{
            .file = b.path("src/zstbi.c"),
            .flags = &.{
                "-std=c99",
                "-fno-sanitize=undefined",
            },
        });
    }
    zstbi_lib.linkLibC();
    b.installArtifact(zstbi_lib);

    const test_step = b.step("test", "Run zstbi tests");

    const tests = b.addTest(.{
        .name = "zstbi-tests",
        .root_source_file = b.path("src/zstbi.zig"),
        .target = target,
        .optimize = optimize,
    });
    tests.linkLibrary(zstbi_lib);
    b.installArtifact(tests);

    test_step.dependOn(&b.addRunArtifact(tests).step);
}
