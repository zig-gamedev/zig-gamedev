const std = @import("std");

pub const Package = struct {
    znoise: *std.Build.Module,
    znoise_c_cpp: *std.Build.CompileStep,

    pub fn build(
        b: *std.Build,
        target: std.zig.CrossTarget,
        optimize: std.builtin.Mode,
        _: struct {},
    ) Package {
        const znoise = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/znoise.zig" },
        });

        const znoise_c_cpp = b.addStaticLibrary(.{
            .name = "znoise",
            .target = target,
            .optimize = optimize,
        });
        znoise_c_cpp.linkLibC();
        znoise_c_cpp.addIncludePath(thisDir() ++ "/libs/FastNoiseLite");
        znoise_c_cpp.addCSourceFile(
            thisDir() ++ "/libs/FastNoiseLite/FastNoiseLite.c",
            &.{ "-std=c99", "-fno-sanitize=undefined" },
        );

        return .{
            .znoise = znoise,
            .znoise_c_cpp = znoise_c_cpp,
        };
    }

    pub fn link(znoise_pkg: Package, exe: *std.Build.CompileStep) void {
        exe.linkLibrary(znoise_pkg.znoise_c_cpp);
    }
};

pub fn build(_: *std.Build) void {}

pub fn buildTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/znoise.zig" },
        .target = target,
        .optimize = optimize,
    });

    const znoise_pkg = Package.build(b, target, optimize, .{});
    znoise_pkg.link(tests);

    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
