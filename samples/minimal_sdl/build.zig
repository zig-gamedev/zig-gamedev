const std = @import("std");

const Options = @import("../../build.zig").Options;

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    if (options.target.os_tag != null and options.target.os_tag.? == .freestanding) {
        const bin = b.addStaticLibrary(.{
            .name = "minimal_sdl",
            .root_source_file = .{ .path = thisDir() ++ "/src/minimal_sdl_emsc.zig" },
            .target = options.target,
            .optimize = options.optimize,
        });
        // bin.single_threaded = true;
        // bin.rdynamic = true;
        return bin;
    } else {
        return b.addExecutable(.{
            .name = "minimal_sdl",
            .root_source_file = .{ .path = thisDir() ++ "/src/minimal_sdl.zig" },
            .target = options.target,
            .optimize = options.optimize,
        });
    }
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
