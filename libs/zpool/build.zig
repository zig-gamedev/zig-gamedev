const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = b.addModule("root", .{
        .root_source_file = b.path("src/main.zig"),
    });

    const test_step = b.step("test", "Run zpool tests");

    const tests = b.addTest(.{
        .name = "zpool-tests",
        .root_source_file = b.path("src/main.zig"),
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(tests);

    test_step.dependOn(&b.addRunArtifact(tests).step);
}
