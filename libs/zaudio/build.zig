const std = @import("std");

pub const Package = struct {
    module: *std.Build.Module,
};

pub fn package(b: *std.Build, _: struct {}) Package {
    const module = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zaudio.zig" },
    });
    return .{ .module = module };
}

pub fn build(_: *std.Build) void {}

pub fn link(exe: *std.Build.CompileStep) void {
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
    b: *std.Build,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/zaudio.zig" },
        .target = target,
        .optimize = build_mode,
    });
    link(tests);
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
