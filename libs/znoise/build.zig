const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "znoise",
    .source = .{ .path = thisDir() ++ "/src/znoise.zig" },
};

pub fn build(_: *std.build.Builder) void {}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(pkg.source.path);
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

pub fn link(exe: *std.build.LibExeObjStep) void {
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
