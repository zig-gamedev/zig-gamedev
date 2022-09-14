const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zaudio",
    .source = .{ .path = thisDir() ++ "/src/zaudio.zig" },
};

pub fn link(exe: *std.build.LibExeObjStep) void {
    exe.addIncludeDir(thisDir() ++ "/libs/miniaudio");
    exe.linkSystemLibraryName("c");

    const target = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;

    if (target.os.tag == .macos) {
        const system_sdk = @import("system_sdk.zig");
        system_sdk.include(exe.builder, exe, .{});
        exe.linkFramework("CoreAudio");
        exe.linkFramework("CoreFoundation");
        exe.linkFramework("AudioUnit");
        exe.linkFramework("AudioToolbox");
    } else if (target.os.tag == .linux) {
        exe.linkSystemLibraryName("pthread");
        exe.linkSystemLibraryName("m");
        exe.linkSystemLibraryName("dl");
    }

    exe.addCSourceFile(thisDir() ++ "/libs/miniaudio/zaudio.c", &.{"-std=c99"});
    exe.addCSourceFile(thisDir() ++ "/libs/miniaudio/miniaudio.c", &.{
        "-DMA_NO_WEBAUDIO",
        "-DMA_NO_ENCODING",
        "-DMA_NO_NULL",
        "-DMA_NO_JACK",
        "-std=c99",
        "-fno-sanitize=undefined",
        if (target.os.tag == .macos) "-DMA_NO_RUNTIME_LINKING" else "",
    });
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(pkg.source.path);
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
