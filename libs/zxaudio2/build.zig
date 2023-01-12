const std = @import("std");

pub fn getPkg(dependencies: []const std.build.Pkg) std.build.Pkg {
    return .{
        .name = "zxaudio2",
        .source = .{ .path = thisDir() ++ "/src/zxaudio2.zig" },
        .dependencies = dependencies,
    };
}

pub fn build(_: *std.build.Builder) void {}

pub fn link(exe: *std.build.LibExeObjStep, enable_debug_layer: bool) void {
    if (enable_debug_layer) {
        exe.step.dependOn(
            &exe.builder.addInstallFile(
                .{ .path = thisDir() ++ "/../zwin32/bin/x64/xaudio2_9redist_debug.dll" },
                "bin/xaudio2_9redist.dll",
            ).step,
        );
    } else {
        exe.step.dependOn(
            &exe.builder.addInstallFile(
                .{ .path = thisDir() ++ "/../zwin32/bin/x64/xaudio2_9redist.dll" },
                "bin/xaudio2_9redist.dll",
            ).step,
        );
    }
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
