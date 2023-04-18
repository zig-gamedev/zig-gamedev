const std = @import("std");

pub const Package = struct {
    zaudio: *std.Build.Module,
    zaudio_c_cpp: *std.Build.CompileStep,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.linkLibrary(pkg.zaudio_c_cpp);
        exe.addModule("zaudio", pkg.zaudio);
    }
};

pub fn package(
    b: *std.Build,
    target: std.zig.CrossTarget,
    optimize: std.builtin.Mode,
    _: struct {},
) Package {
    const zaudio = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zaudio.zig" },
    });

    const zaudio_c_cpp = b.addStaticLibrary(.{
        .name = "zaudio",
        .target = target,
        .optimize = optimize,
    });

    zaudio_c_cpp.addIncludePath(thisDir() ++ "/libs/miniaudio");
    zaudio_c_cpp.linkLibC();

    const host = (std.zig.system.NativeTargetInfo.detect(zaudio_c_cpp.target) catch unreachable).target;

    if (host.os.tag == .macos) {
        zaudio_c_cpp.addFrameworkPath(thisDir() ++ "/../system-sdk/macos12/System/Library/Frameworks");
        zaudio_c_cpp.addSystemIncludePath(thisDir() ++ "/../system-sdk/macos12/usr/include");
        zaudio_c_cpp.addLibraryPath(thisDir() ++ "/../system-sdk/macos12/usr/lib");
        zaudio_c_cpp.linkFramework("CoreAudio");
        zaudio_c_cpp.linkFramework("CoreFoundation");
        zaudio_c_cpp.linkFramework("AudioUnit");
        zaudio_c_cpp.linkFramework("AudioToolbox");
    } else if (host.os.tag == .linux) {
        zaudio_c_cpp.linkSystemLibraryName("pthread");
        zaudio_c_cpp.linkSystemLibraryName("m");
        zaudio_c_cpp.linkSystemLibraryName("dl");
    }

    zaudio_c_cpp.addCSourceFile(thisDir() ++ "/src/zaudio.c", &.{"-std=c99"});
    zaudio_c_cpp.addCSourceFile(thisDir() ++ "/libs/miniaudio/miniaudio.c", &.{
        "-DMA_NO_WEBAUDIO",
        "-DMA_NO_ENCODING",
        "-DMA_NO_NULL",
        "-DMA_NO_JACK",
        "-DMA_NO_DSOUND",
        "-DMA_NO_WINMM",
        "-std=c99",
        "-fno-sanitize=undefined",
        if (host.os.tag == .macos) "-DMA_NO_RUNTIME_LINKING" else "",
    });

    return .{
        .zaudio = zaudio,
        .zaudio_c_cpp = zaudio_c_cpp,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zaudio tests");
    test_step.dependOn(runTests(b, optimize, target));
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "zaudio-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/zaudio.zig" },
        .target = target,
        .optimize = optimize,
    });

    const zaudio_pkg = package(b, target, optimize, .{});
    zaudio_pkg.link(tests);

    return &b.addRunArtifact(tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
