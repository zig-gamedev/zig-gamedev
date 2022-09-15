const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "znetwork",
    .source = .{ .path = thisDir() ++ "/src/main.zig" },
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
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
