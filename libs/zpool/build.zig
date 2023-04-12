const std = @import("std");

pub const Package = struct {
    zpool: *std.Build.Module,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.addModule("zpool", pkg.zpool);
    }
};

pub fn package(
    b: *std.Build,
    _: std.zig.CrossTarget,
    _: std.builtin.Mode,
    _: struct {},
) Package {
    const zpool = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/main.zig" },
    });
    return .{ .zpool = zpool };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zpool tests");
    test_step.dependOn(runTests(b, optimize, target));
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "zpool-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/main.zig" },
        .target = target,
        .optimize = optimize,
    });
    return &b.addRunArtifact(tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
