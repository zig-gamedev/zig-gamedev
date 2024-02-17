const std = @import("std");

pub const Package = struct {
    zcgltf: *std.Build.Module,
    cgltf_lib: *std.Build.Step.Compile,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.linkLibrary(pkg.cgltf_lib);
    }
};

pub fn package(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.Mode,
    _: struct {},
) Package {
    const module = b.addModule("root", .{
        .root_source_file = .{ .path = thisDir() ++ "/src/zcgltf.zig" },
    });

    const cgltf = b.addStaticLibrary(.{
        .name = "cgltf",
        .target = target,
        .optimize = optimize,
    });
    cgltf.addIncludePath(.{ .path = thisDir() ++ "/cgltf" });
    cgltf.addCSourceFile(.{
        .file = .{ .path = thisDir() ++ "/cgltf/cgltf.c" },
        .flags = &.{"-std=c99"},
    });
    cgltf.linkLibC();
    b.installArtifact(cgltf);

    return .{
        .zcgltf = module,
        .cgltf_lib = cgltf,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const pkg = package(b, target, optimize, .{});

    const test_step = b.step("test", "Run zcgltf tests");
    test_step.dependOn(runTests(b, optimize, target, pkg));
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,
    pkg: Package,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "zcgltf-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/zcgltf.zig" },
        .target = target,
        .optimize = optimize,
    });
    tests.addIncludePath(.{ .path = thisDir() ++ "/cgltf" });
    tests.linkLibrary(pkg.cgltf_lib);
    return &b.addRunArtifact(tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
