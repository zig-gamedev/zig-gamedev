const std = @import("std");
const assert = std.debug.assert;

pub const Package = struct {
    zsdl: *std.Build.Module,
    install: *std.Build.Step,
    ttf: bool,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.linkLibC();

        exe.addModule("zsdl", pkg.zsdl);

        exe.step.dependOn(pkg.install);

        const target = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;

        switch (target.os.tag) {
            .windows => {
                assert(target.cpu.arch.isX86());

                exe.addIncludePath(thisDir() ++ "/libs/x86_64-windows-gnu/include");
                exe.addLibraryPath(thisDir() ++ "/libs/x86_64-windows-gnu/lib");
                exe.linkSystemLibraryName("SDL2");
                exe.linkSystemLibraryName("SDL2main");

                if (pkg.ttf) {
                    exe.linkSystemLibraryName("SDL2_ttf");
                }
            },
            .linux => {
                assert(target.cpu.arch.isX86());

                exe.addIncludePath(thisDir() ++ "/libs/x86_64-linux-gnu/include");
                exe.addLibraryPath(thisDir() ++ "/libs/x86_64-linux-gnu/lib");
                exe.linkSystemLibraryName("SDL2-2.0");
                exe.addRPath("$ORIGIN");

                if (pkg.ttf) {
                    exe.linkSystemLibraryName("SDL2_ttf-2.0");
                }
            },
            .macos => {
                exe.addFrameworkPath(thisDir() ++ "/libs/macos/Frameworks");
                exe.linkFramework("SDL2");
                exe.addRPath("@executable_path/Frameworks");

                if (pkg.ttf) {
                    exe.linkFramework("SDL2_ttf");
                }
            },
            else => unreachable,
        }
    }
};

pub fn package(
    b: *std.Build,
    target: std.zig.CrossTarget,
    _: std.builtin.Mode,
    args: struct {
        ttf: bool = false,
    },
) Package {
    const zsdl = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/zsdl.zig" },
    });

    const install_step = b.allocator.create(std.Build.Step) catch @panic("OOM");
    install_step.* = std.Build.Step.init(.{ .id = .custom, .name = "zsdl-install", .owner = b });

    if (target.isWindows()) {
        install_step.dependOn(
            &b.addInstallFile(
                .{ .path = thisDir() ++ "/libs/x86_64-windows-gnu/bin/SDL2.dll" },
                "bin/SDL2.dll",
            ).step,
        );
        if (args.ttf) {
            install_step.dependOn(
                &b.addInstallFile(
                    .{ .path = thisDir() ++ "/libs/x86_64-windows-gnu/bin/SDL2_ttf.dll" },
                    "bin/SDL2_ttf.dll",
                ).step,
            );
        }
    } else if (target.isLinux()) {
        install_step.dependOn(
            &b.addInstallFile(
                .{ .path = thisDir() ++ "/libs/x86_64-linux-gnu/lib/libSDL2-2.0.so" },
                "bin/libSDL2-2.0.so.0",
            ).step,
        );
        if (args.ttf) {
            install_step.dependOn(
                &b.addInstallFile(
                    .{ .path = thisDir() ++ "/libs/x86_64-linux-gnu/lib/libSDL2_ttf-2.0.so" },
                    "bin/libSDL2_ttf-2.0.so.0",
                ).step,
            );
        }
    } else if (target.isDarwin()) {
        install_step.dependOn(
            &b.addInstallDirectory(.{
                .source_dir = .{ .path = thisDir() ++ "/libs/macos/Frameworks/SDL2.framework" },
                .install_dir = .{ .custom = "" },
                .install_subdir = "bin/Frameworks/SDL2.framework",
            }).step,
        );
        if (args.ttf) {
            install_step.dependOn(
                &b.addInstallDirectory(.{
                    .source_dir = .{ .path = thisDir() ++ "/libs/macos/Frameworks/SDL2_ttf.framework" },
                    .install_dir = .{ .custom = "" },
                    .install_subdir = "bin/Frameworks/SDL2_ttf.framework",
                }).step,
            );
        }
    } else unreachable;

    return .{
        .zsdl = zsdl,
        .install = install_step,
        .ttf = args.ttf,
    };
}

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
