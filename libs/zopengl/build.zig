const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = b.addModule("root", .{
        .root_source_file = b.path("src/zopengl.zig"),
    });

    const test_step = b.step("test", "Run zopengl tests");

    const tests = b.addTest(.{
        .name = "zopengl-tests",
        .root_source_file = b.path("src/zopengl.zig"),
        .target = target,
        .optimize = optimize,
    });

    _ = b.installArtifact(tests);

    test_step.dependOn(&b.addRunArtifact(tests).step);
}
