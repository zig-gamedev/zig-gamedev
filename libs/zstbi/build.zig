const std = @import("std");

pub const Package = struct {
    zstbi: *std.Build.Module,
    zstbi_c_cpp: *std.Build.CompileStep,

    pub fn build(
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
        zstbi_c_cpp.addCSourceFile(thisDir() ++ "/libs/stbi/stb_image.c", &.{
            "-std=c99",
            "-fno-sanitize=undefined",
        });
        zstbi_c_cpp.linkLibC();

        return .{
            .zstbi = zstbi,
            .zstbi_c_cpp = zstbi_c_cpp,
        };
    }

    pub fn link(zstbi_pkg: Package, exe: *std.Build.CompileStep) void {
        exe.linkLibrary(zstbi_pkg.zstbi_c_cpp);
    }
};

pub fn build(_: *std.Build) void {}

pub fn buildTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/zstbi.zig" },
        .target = target,
        .optimize = optimize,
    });
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
