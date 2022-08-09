const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zaudio",
    .source = .{ .path = thisDir() ++ "/src/zaudio.zig" },
};

pub fn link(exe: *std.build.LibExeObjStep) void {
    const lib = buildLibrary(exe);
    exe.linkLibrary(lib);
    exe.addIncludeDir(thisDir() ++ "/libs/miniaudio");
}

fn buildLibrary(exe: *std.build.LibExeObjStep) *std.build.LibExeObjStep {
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
        exe.linkFramework("AudioToolbox");
    } else if (exe.target.isLinux()) {
        exe.linkSystemLibrary("pthread");
        exe.linkSystemLibrary("m");
        exe.linkSystemLibrary("dl");
    }

    lib.addCSourceFile(thisDir() ++ "/libs/miniaudio/cabi_workarounds.c", &.{});
    lib.addCSourceFile(thisDir() ++ "/libs/miniaudio/miniaudio.c", &.{
        "-DMA_NO_WEBAUDIO",
        "-DMA_NO_ENCODING",
        "-DMA_NO_NULL",
        "-DMA_NO_JACK",
        "-fno-sanitize=undefined",
        if (@import("builtin").target.os.tag == .macos) "-DMA_NO_RUNTIME_LINKING" else "",
    });

    return lib;
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/zaudio.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
