const std = @import("std");

pub fn buildLibrary(exe: *std.build.LibExeObjStep) void {
    const lib = exe.builder.addStaticLibrary("zaudio", thisDir() ++ "/src/zaudio.zig");

    lib.setBuildMode(exe.build_mode);
    lib.setTarget(exe.target);
    lib.addIncludeDir(thisDir() ++ "/libs/miniaudio");
    lib.linkSystemLibrary("c");

    if (exe.target.isDarwin()) {
        const system_sdk = @import("system_sdk.zig");
        system_sdk.include(exe.builder, lib, .{});
        exe.linkFramework("CoreAudio");
        exe.linkFramework("CoreFoundation");
        exe.linkFramework("AudioUnit");
    } else if (exe.target.isLinux()) {
        exe.linkSystemLibrary("pthread");
        exe.linkSystemLibrary("m");
        exe.linkSystemLibrary("dl");
    }

    lib.addCSourceFile(thisDir() ++ "libs/miniaudio/miniaudio.c", &.{
        "-DMA_NO_FLAC",
        "-DMA_NO_WEBAUDIO",
        "-DMA_NO_ENCODING",
        "-DMA_NO_NULL",
        "-DMA_NO_RUNTIME_LINKING",
    });
}

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
