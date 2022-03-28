const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    _ = b;
}

pub const Options = struct {
    enable_debug_layer: bool = false,
};

pub fn link(b: *std.build.Builder, exe: *std.build.LibExeObjStep, options: Options) void {
    if (options.enable_debug_layer) {
        exe.step.dependOn(
            &b.addInstallFile(
                .{ .path = thisDir() ++ "/../zwin32/bin/x64/xaudio2_9redist_debug.dll" },
                "bin/xaudio2_9redist.dll",
            ).step,
        );
    } else {
        exe.step.dependOn(
            &b.addInstallFile(
                .{ .path = thisDir() ++ "/../zwin32/bin/x64/xaudio2_9redist.dll" },
                "bin/xaudio2_9redist.dll",
            ).step,
        );
    }
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
