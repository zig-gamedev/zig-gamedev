const std = @import("std");

pub const Package = struct {
    zopengl: *std.Build.Module,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.root_module.addImport("zopengl", pkg.zopengl);
    }
};

pub fn package(
    b: *std.Build,
    _: std.Build.ResolvedTarget,
    _: std.builtin.Mode,
    _: struct {},
) Package {
    const zopengl = b.addModule("zopengl", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/zopengl.zig" },
    });
    return .{
        .zopengl = zopengl,
    };
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "zopengl-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/zopengl.zig" },
        .target = target,
        .optimize = optimize,
    });
    const zopengl_pkg = package(b, target, optimize, .{});
    zopengl_pkg.link(tests);

    return &b.addRunArtifact(tests).step;
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zopengl tests");
    test_step.dependOn(runTests(b, optimize, target));

    _ = package(b, target, optimize, .{});
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
