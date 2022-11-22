const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zaudio",
    .source = .{ .path = thisDir() ++ "/src/zaudio.zig" },
};

pub fn build(_: *std.build.Builder) void {}

pub fn link(exe: *std.build.LibExeObjStep) void {
    exe.addIncludePath(thisDir() ++ "/libs/miniaudio");
    exe.linkSystemLibraryName("c");

    const target = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;

    if (target.os.tag == .macos) {
        exe.addFrameworkPath(thisDir() ++ "/../system-sdk/macos12/System/Library/Frameworks");
        exe.addSystemIncludePath(thisDir() ++ "/../system-sdk/macos12/usr/include");
        exe.addLibraryPath(thisDir() ++ "/../system-sdk/macos12/usr/lib");
        exe.linkFramework("CoreAudio");
        exe.linkFramework("CoreFoundation");
        exe.linkFramework("AudioUnit");
        exe.linkFramework("AudioToolbox");
    } else if (target.os.tag == .linux) {
        exe.linkSystemLibraryName("pthread");
        exe.linkSystemLibraryName("m");
        exe.linkSystemLibraryName("dl");
    }

    exe.addCSourceFile(thisDir() ++ "/src/zaudio.c", &.{"-std=c99"});
    exe.addCSourceFile(thisDir() ++ "/libs/miniaudio/miniaudio.c", &.{
        "-DMA_NO_WEBAUDIO",
        "-DMA_NO_ENCODING",
        "-DMA_NO_NULL",
        "-DMA_NO_JACK",
        "-DMA_NO_DSOUND",
        "-DMA_NO_WINMM",
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
