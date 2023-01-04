const std = @import("std");

pub fn link(exe: *std.build.LibExeObjStep) void {
    // TODO: Remove libc dependency by overriding std functions (also see TODOs in zstbtt.c and zstbtt.zig)
    exe.linkSystemLibraryName("c");
    exe.addCSourceFile(thisDir() ++ "/src/zstbtt.c", &.{
        "-std=c99",
        "-fno-sanitize=undefined",
    });
}

pub const pkg = std.build.Pkg{
    .name = "zstbtt",
    .source = .{ .path = thisDir() ++ "/src/zstbtt.zig" },
};

pub fn build(b: *std.build.Builder) void {
    const mode = b.standardReleaseOptions();

    const tests = buildTests(b, mode);

    const test_step = b.step("test", "Run tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
) *std.build.LibExeObjStep {
    const tests = b.addTest(pkg.source.path);
    tests.setBuildMode(build_mode);
    link(tests);
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
