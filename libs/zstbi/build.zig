const std = @import("std");

pub fn link(exe: *std.build.LibExeObjStep) void {
    exe.linkSystemLibraryName("c");
    exe.addCSourceFile(thisDir() ++ "/libs/stbi/stb_image.c", &.{
        "-std=c99",
        "-fno-sanitize=undefined",
    });
}

pub const pkg = std.build.Pkg{
    .name = "zstbi",
    .source = .{ .path = thisDir() ++ "/src/zstbi.zig" },
};

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

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
