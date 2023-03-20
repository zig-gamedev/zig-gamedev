const std = @import("std");
const assert = std.debug.assert;

pub const Package = struct {
    zsdl: *std.Build.Module,
    install: *std.Build.Step,

    pub fn build(
        b: *std.Build,
        target: std.zig.CrossTarget,
        _: std.builtin.Mode,
        _: struct {},
    ) Package {
        const zsdl = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/zsdl.zig" },
        });

        const install_step = b.step("zsdl-install", "");

        if (target.isWindows()) {
            install_step.dependOn(
                &b.addInstallFile(
                    .{ .path = thisDir() ++ "/libs/x86_64-windows-gnu/bin/SDL2.dll" },
                    "bin/SDL2.dll",
                ).step,
            );
        } else if (target.isLinux()) {
            install_step.dependOn(
                &b.addInstallFile(
                    .{ .path = thisDir() ++ "/libs/x86_64-linux-gnu/lib/libSDL2-2.0.so" },
                    "bin/libSDL2-2.0.so.0",
                ).step,
            );
        } else if (target.isDarwin()) {
            const install_dir_step = b.addInstallDirectory(.{
                .source_dir = thisDir() ++ "/libs/macos/Frameworks/SDL2.framework",
                .install_dir = .bin,
                .install_subdir = "Frameworks/SDL2.framework",
            });
            install_step.dependOn(&install_dir_step.step);
        } else unreachable;

        return .{
            .zsdl = zsdl,
            .install = install_step,
        };
    }

    pub fn link(zsdl_pkg: Package, b: *std.Build, exe: *std.Build.CompileStep) void {
        exe.linkLibC();

        exe.step.dependOn(zsdl_pkg.install);

        const target = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;

        switch (target.os.tag) {
            .windows => {
                assert(target.cpu.arch.isX86());

                exe.addIncludePath(thisDir() ++ "/libs/x86_64-windows-gnu/include");
                exe.addLibraryPath(thisDir() ++ "/libs/x86_64-windows-gnu/lib");
                exe.linkSystemLibraryName("SDL2");
                exe.linkSystemLibraryName("SDL2main");
            },
            .linux => {
                assert(target.cpu.arch.isX86());

                exe.addIncludePath(thisDir() ++ "/libs/x86_64-linux-gnu/include");
                exe.addLibraryPath(thisDir() ++ "/libs/x86_64-linux-gnu/lib");
                exe.linkSystemLibraryName("SDL2-2.0");
                exe.addRPath("$ORIGIN");
            },
            .macos => {
                exe.addFrameworkPath(thisDir() ++ "/libs/macos/Frameworks");
                exe.linkFramework("SDL2");
                exe.addRPath(b.getInstallPath(.bin, "Frameworks"));
            },
            else => unreachable,
        }
    }
};

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
