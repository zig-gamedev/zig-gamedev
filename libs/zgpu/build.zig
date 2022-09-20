const std = @import("std");

pub fn getPkg(dependencies: []const std.build.Pkg) std.build.Pkg {
    return .{
        .name = "zgpu",
        .source = .{ .path = thisDir() ++ "/src/zgpu.zig" },
        .dependencies = dependencies,
    };
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/zgpu.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests);
    return tests;
}

pub fn link(exe: *std.build.LibExeObjStep) void {
    const target = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;

    switch (target.os.tag) {
        .windows => {
            exe.addLibraryPath(thisDir() ++ "/libs/dawn/x86_64-windows-gnu");

            exe.linkSystemLibraryName("ole32");
            exe.linkSystemLibraryName("dxguid");
        },
        .linux => {
            if (target.cpu.arch.isX86())
                exe.addLibraryPath(thisDir() ++ "/libs/dawn/x86_64-linux-gnu")
            else
                exe.addLibraryPath(thisDir() ++ "/libs/dawn/aarch64-linux-gnu");

            exe.linkSystemLibraryName("X11");
        },
        .macos => {
            if (target.cpu.arch.isX86())
                exe.addLibraryPath(thisDir() ++ "/libs/dawn/x86_64-macos-none")
            else
                exe.addLibraryPath(thisDir() ++ "/libs/dawn/aarch64-macos-none");

            exe.linkFramework("Metal");
            exe.linkFramework("CoreGraphics");
            exe.linkFramework("Foundation");
            exe.linkFramework("IOKit");
            exe.linkFramework("IOSurface");
            exe.linkFramework("QuartzCore");
        },
        else => unreachable,
    }

    exe.linkSystemLibraryName("dawn");
    exe.linkSystemLibraryName("c");
    exe.linkSystemLibraryName("c++");

    exe.addIncludePath(thisDir() ++ "/libs/dawn/include");
    exe.addIncludePath(thisDir() ++ "/src");

    exe.addCSourceFile(thisDir() ++ "/src/dawn.cpp", &.{"-std=c++17"});
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
