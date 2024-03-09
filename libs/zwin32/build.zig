const std = @import("std");

pub fn build(b: *std.Build) !void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = b.addModule("root", .{
        .root_source_file = .{ .path = "src/zwin32.zig" },
    });

    const test_step = b.step("test", "Run zwin32 tests");

    const tests = b.addTest(.{
        .name = "zwin32-tests",
        .root_source_file = .{ .path = "src/zwin32.zig" },
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(tests);

    test_step.dependOn(&b.addRunArtifact(tests).step);
}
