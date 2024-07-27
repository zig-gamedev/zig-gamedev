const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});

    const optimize = b.standardOptimizeOption(.{
        .preferred_optimize_mode = .ReleaseSafe,
    });

    _ = b.addModule("root", .{
        .root_source_file = b.path("src/zopengl.zig"),
    });

    const lib = b.addStaticLibrary(.{
        .name = "zopengl",
        .root_source_file = b.path("src/zopengl.zig"),
        .target = target,
        .optimize = optimize,
    });
    _ = b.installArtifact(lib);
}
