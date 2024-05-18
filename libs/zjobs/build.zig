const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = b.addModule("root", .{
        .root_source_file = b.path("src/zjobs.zig"),
    });

    const test_step = b.step("test", "Run zjobs tests");

    const tests = b.addTest(.{
        .name = "zjobs-tests",
        .root_source_file = b.path("src/zjobs.zig"),
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(tests);

    test_step.dependOn(&b.addRunArtifact(tests).step);
}
