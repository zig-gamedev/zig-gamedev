const std = @import("std");
const zwin32 = @import("../zwin32/zwin32.zig");

pub fn getPkg(b: *std.build.Builder, options_pkg: std.build.Pkg) std.build.Pkg {
    const pkg = std.build.Pkg{
        .name = "zxaudio2",
        .path = .{ .path = thisDir() ++ "/src/zxaudio2.zig" },
        .dependencies = &[_]std.build.Pkg{
            zwin32.pkg,
            options_pkg,
        },
    };
    return b.dupePkg(pkg);
}

pub fn build(b: *std.build.Builder) void {
    _ = b;
}

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

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
