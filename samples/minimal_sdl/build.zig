const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    const exe = b.addExecutable(.{
        .name = "minimal_sdl",
        .root_source_file = .{ .path = thisDir() ++ "/src/minimal_sdl.zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
