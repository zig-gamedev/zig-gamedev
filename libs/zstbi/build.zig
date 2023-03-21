const std = @import("std");

pub const Package = struct {
    zstbi: *std.Build.Module,
    zstbi_c_cpp: *std.Build.CompileStep,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.linkLibrary(pkg.zstbi_c_cpp);
        exe.addModule("zstbi", pkg.zstbi);
    }
};

pub fn package(
    b: *std.Build,
    target: std.zig.CrossTarget,
    optimize: std.builtin.Mode,
    _: struct {},
) Package {
    const zstbi = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zstbi.zig" },
    });

    const zstbi_c_cpp = b.addStaticLibrary(.{
        .name = "zstbi",
        .target = target,
        .optimize = optimize,
    });
    if (optimize == .Debug) {
        // TODO: Workaround for Zig bug.
        zstbi_c_cpp.addCSourceFile(thisDir() ++ "/libs/stbi/stb_image.c", &.{
            "-std=c99",
            "-fno-sanitize=undefined",
            "-g",
            "-O0",
        });
    } else {
        zstbi_c_cpp.addCSourceFile(thisDir() ++ "/libs/stbi/stb_image.c", &.{
            "-std=c99",
            "-fno-sanitize=undefined",
        });
    }
    zstbi_c_cpp.linkLibC();

    return .{
        .zstbi = zstbi,
        .zstbi_c_cpp = zstbi_c_cpp,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zstbi tests");
    test_step.dependOn(runTests(b, optimize, target));
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "zstbi-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/zstbi.zig" },
        .target = target,
        .optimize = optimize,
    });

    const zstbi_pkg = package(b, target, optimize, .{});
    zstbi_pkg.link(tests);

    return &tests.run().step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
