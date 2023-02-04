const std = @import("std");

pub const pkg = std.Build.Pkg{
    .name = "znoise",
    .source = .{ .path = thisDir() ++ "/src/znoise.zig" },
};

pub fn build(_: *std.Build) void {}

pub fn buildTests(
    b: *std.Build,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = pkg.source.path },
        .target = target,
        .optimize = build_mode,
    });
    link(tests);
    return tests;
}

pub fn link(exe: *std.Build.CompileStep) void {
    exe.addIncludePath(thisDir() ++ "/libs/FastNoiseLite");
    exe.linkSystemLibraryName("c");

    exe.addCSourceFile(
        thisDir() ++ "/libs/FastNoiseLite/FastNoiseLite.c",
        &.{ "-std=c99", "-fno-sanitize=undefined" },
    );
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
