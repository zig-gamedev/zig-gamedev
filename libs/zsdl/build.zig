const std = @import("std");
const assert = std.debug.assert;

pub const Package = struct {
    zsdl: *std.Build.Module,

    pub fn build(
        b: *std.Build,
        _: struct {},
    ) Package {
        const zsdl = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/zsdl.zig" },
        });
        return .{
            .zsdl = zsdl,
        };
    }

    pub fn link(_: Package, exe: *std.Build.CompileStep) void {
        exe.linkLibC();

        const target = (std.zig.system.NativeTargetInfo.detect(exe.target) catch unreachable).target;

        switch (target.os.tag) {
            .windows => {
                assert(target.cpu.arch.isX86());

                exe.addIncludePath(thisDir() ++ "/libs/x86_64-windows-gnu/include");
                exe.addLibraryPath(thisDir() ++ "/libs/x86_64-windows-gnu/lib");
                exe.linkSystemLibraryName("SDL2");
                exe.linkSystemLibraryName("SDL2main");

                exe.step.dependOn(
                    &exe.builder.addInstallFile(
                        .{ .path = thisDir() ++ "/libs/x86_64-windows-gnu/bin/SDL2.dll" },
                        "bin/SDL2.dll",
                    ).step,
                );
            },
            .linux => {
                assert(target.cpu.arch.isX86());

                exe.addIncludePath(thisDir() ++ "/libs/x86_64-linux-gnu/include");
                exe.addLibraryPath(thisDir() ++ "/libs/x86_64-linux-gnu/lib");
                exe.linkSystemLibraryName("SDL2-2.0");
                exe.addRPath("$ORIGIN");

                exe.step.dependOn(
                    &exe.builder.addInstallFile(
                        .{ .path = thisDir() ++ "/libs/x86_64-linux-gnu/lib/libSDL2-2.0.so" },
                        "bin/libSDL2-2.0.so.0",
                    ).step,
                );
            },
            .macos => {
                exe.addFrameworkPath(thisDir() ++ "/libs/macos/Frameworks");
                exe.linkFramework("SDL2");
                exe.addRPath("@executable_path/Frameworks");

                const install_dir_step = exe.builder.addInstallDirectory(.{
                    .source_dir = thisDir() ++ "/libs/macos/Frameworks/SDL2.framework",
                    .install_dir = .{ .custom = "" },
                    .install_subdir = "bin/Frameworks/SDL2.framework",
                });
                exe.step.dependOn(&install_dir_step.step);
            },
            else => unreachable,
        }
    }
};

pub fn build(_: *std.Build) void {}

pub fn buildTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/zsdl.zig" },
        .target = target,
        .optimize = optimize,
    });
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
