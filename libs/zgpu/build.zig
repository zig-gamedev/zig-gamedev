const std = @import("std");

pub fn link(exe: *std.build.LibExeObjStep) void {
    linkDawn(exe);

    exe.addIncludePath(thisDir() ++ "/src");
    exe.addCSourceFile(thisDir() ++ "/src/dawn.cpp", &.{"-std=c++17"});
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

pub fn getPkg(dependencies: []const std.build.Pkg) std.build.Pkg {
    return .{
        .name = "zgpu",
        .source = .{ .path = thisDir() ++ "/src/zgpu.zig" },
        .dependencies = dependencies,
    };
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}

const DawnOptions = struct {
    d3d12: ?bool = null,
    metal: ?bool = null,
    vulkan: ?bool = null,

    fn init(target: std.Target) DawnOptions {
        const tag = target.os.tag;
        var options = DawnOptions{};
        options.d3d12 = (tag == .windows);
        options.metal = (tag == .macos);
        options.vulkan = (tag == .linux);
        return options;
    }
};

fn linkDawn(exe: *std.build.LibExeObjStep) void {
    const target = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;
    const options = DawnOptions.init(target);
    linkFromBinary(exe.builder, exe, options);
}

fn linkFromBinary(b: *std.build.Builder, step: *std.build.LibExeObjStep, options: DawnOptions) void {
    const target = (std.zig.system.NativeTargetInfo.detect(
        step.target,
    ) catch unreachable).target;
    const binaries_available = switch (target.os.tag) {
        .windows => target.abi.isGnu(),
        .linux => target.cpu.arch.isX86() and target.abi.isGnu(),
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
        const zig_triple = target.zigTriple(b.allocator) catch unreachable;
        std.log.err("Dawn binaries for {s} not available.", .{zig_triple});
        if (target.os.tag == .macos) {
            std.log.err("", .{});
            if (target.cpu.arch.isX86()) std.log.err(
                "-> Did you mean to use -Dtarget=x86_64-macos.12 ?",
                .{},
            );
            if (target.cpu.arch.isAARCH64()) std.log.err(
                "-> Did you mean to use -Dtarget=aarch64-macos.12 ?",
                .{},
            );
        }
        std.process.exit(1);
    }

    switch (target.os.tag) {
        .windows => step.addLibraryPath(thisDir() ++ "/libs/dawn/x86_64-windows-gnu"),
        .linux => step.addLibraryPath(thisDir() ++ "/libs/dawn/x86_64-linux-gnu"),
        .macos => {
            if (target.cpu.arch.isX86())
                step.addLibraryPath(thisDir() ++ "/libs/dawn/x86_64-macos-none")
            else
                step.addLibraryPath(thisDir() ++ "/libs/dawn/aarch64-macos-none");
        },
        else => unreachable,
    }

    step.linkSystemLibraryName("dawn");
    step.linkSystemLibraryName("c");
    step.linkSystemLibraryName("c++");

    step.addIncludePath(thisDir() ++ "/libs/dawn/include");

    if (options.vulkan.?) {
        step.linkSystemLibraryName("X11");
    }
    if (options.metal.?) {
        step.linkFramework("Metal");
        step.linkFramework("CoreGraphics");
        step.linkFramework("Foundation");
        step.linkFramework("IOKit");
        step.linkFramework("IOSurface");
        step.linkFramework("QuartzCore");
    }
    if (options.d3d12.?) {
        step.linkSystemLibraryName("ole32");
        step.linkSystemLibraryName("dxguid");
    }
}
