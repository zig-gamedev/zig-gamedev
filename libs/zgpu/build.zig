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

    const binaries_available = switch (target.os.tag) {
        .windows => target.cpu.arch.isX86() and target.abi.isGnu(),
        .linux => (target.cpu.arch.isX86() or target.cpu.arch.isAARCH64()) and target.abi.isGnu(),
        .macos => blk: {
            if (!target.cpu.arch.isX86() and !target.cpu.arch.isAARCH64()) break :blk false;

            // If min. target macOS version is lesser than the min version we have available, then
            // our binary is incompatible with the target.
            const min_available = std.builtin.Version{ .major = 12, .minor = 0 };
            if (target.os.version_range.semver.min.order(min_available) == .lt) break :blk false;
            break :blk true;
        },
        else => false,
    };
    if (!binaries_available) {
        const zig_triple = target.zigTriple(exe.builder.allocator) catch unreachable;
        std.debug.print("Dawn binaries for {s} not available.", .{zig_triple});
        if (target.os.tag == .macos) {
            if (target.cpu.arch.isX86()) std.debug.print(
                "-> Did you mean to use -Dtarget=x86_64-macos.12 ?",
                .{},
            );
            if (target.cpu.arch.isAARCH64()) std.debug.print(
                "-> Did you mean to use -Dtarget=aarch64-macos.12 ?",
                .{},
            );
        }
        std.process.exit(1);
    }

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
