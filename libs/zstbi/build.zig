const std = @import("std");

pub fn link(exe: *std.Build.CompileStep) void {
    exe.linkSystemLibraryName("c");
    exe.addCSourceFile(thisDir() ++ "/libs/stbi/stb_image.c", &.{
        "-std=c99",
        "-fno-sanitize=undefined",
    });
}

pub const pkg = std.Build.Pkg{
    .name = "zstbi",
    .source = .{ .path = thisDir() ++ "/src/zstbi.zig" },
};

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

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
