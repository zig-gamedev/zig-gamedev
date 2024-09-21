const builtin = @import("builtin");
const std = @import("std");
const assert = std.debug.assert;

pub fn build(b: *std.Build) void {
    const zsdl2_module = b.addModule("zsdl2", .{
        .root_source_file = b.path("src/sdl2.zig"),
    });

    _ = b.addModule("zsdl2_ttf", .{
        .root_source_file = b.path("src/sdl2_ttf.zig"),
        .imports = &.{
            .{ .name = "zsdl2", .module = zsdl2_module },
        },
    });

    _ = b.addModule("zsdl2_image", .{
        .root_source_file = b.path("src/sdl2_image.zig"),
        .imports = &.{
            .{ .name = "zsdl2", .module = zsdl2_module },
        },
    });

    _ = b.addModule("zsdl3", .{
        .root_source_file = b.path("src/sdl3.zig"),
    });
}

pub fn link_SDL2(compile_step: *std.Build.Step.Compile) void {
    switch (compile_step.rootModuleTarget().os.tag) {
        .windows => {
            compile_step.linkSystemLibrary("SDL2");
            compile_step.linkSystemLibrary("SDL2main");
        },
        .linux => {
            compile_step.linkSystemLibrary("SDL2");
            compile_step.root_module.addRPathSpecial("$ORIGIN");
        },
        .macos => {
            compile_step.linkFramework("SDL2");
            compile_step.root_module.addRPathSpecial("@executable_path");
        },
        else => {},
    }
}

pub fn link_SDL2_ttf(compile_step: *std.Build.Step.Compile) void {
    switch (compile_step.rootModuleTarget().os.tag) {
        .windows => {
            compile_step.linkSystemLibrary("SDL2_ttf");
        },
        .linux => {
            compile_step.linkSystemLibrary("SDL2_ttf");
            compile_step.root_module.addRPathSpecial("$ORIGIN");
        },
        .macos => {
            compile_step.linkFramework("SDL2_ttf");
            compile_step.root_module.addRPathSpecial("@executable_path");
        },
        else => {},
    }
}

pub fn link_SDL2_image(compile_step: *std.Build.Step.Compile) void {
    switch (compile_step.rootModuleTarget().os.tag) {
        .windows => {
            compile_step.linkSystemLibrary("SDL2_image");
        },
        .linux => {
            compile_step.linkSystemLibrary("SDL2_image");
            compile_step.root_module.addRPathSpecial("$ORIGIN");
        },
        .macos => {
            compile_step.linkFramework("SDL2_image");
            compile_step.root_module.addRPathSpecial("@executable_path");
        },
        else => {},
    }
}

pub fn link_SDL3(compile_step: *std.Build.Step.Compile) void {
    switch (compile_step.rootModuleTarget().os.tag) {
        .windows => {
            compile_step.linkSystemLibrary("SDL3");
        },
        .linux => {
            compile_step.linkSystemLibrary("SDL3");
            compile_step.root_module.addRPathSpecial("$ORIGIN");
        },
        .macos => {
            compile_step.linkFramework("SDL3");
            compile_step.root_module.addRPathSpecial("@executable_path");
        },
        else => {},
    }
}

pub fn testVersionCheckSDL2(b: *std.Build, target: std.Build.ResolvedTarget) *std.Build.Step {
    const test_sdl2_version_check = b.addTest(.{
        .name = "sdl2-version-check",
        .root_source_file = b.dependency("zsdl", .{}).path("src/sdl2_version_check.zig"),
        .target = target,
        .optimize = .ReleaseSafe,
    });

    link_SDL2(test_sdl2_version_check);

    prebuilt.addLibraryPathsTo(test_sdl2_version_check);

    const version_check_run = b.addRunArtifact(test_sdl2_version_check);

    if (target.result.os.tag == .windows) {
        version_check_run.setCwd(.{
            .cwd_relative = b.getInstallPath(.bin, ""),
        });
    }

    version_check_run.step.dependOn(&test_sdl2_version_check.step);

    if (prebuilt.install_SDL2(b, target.result, .bin)) |install_sdl2_step| {
        version_check_run.step.dependOn(install_sdl2_step);
    }

    return &version_check_run.step;
}

pub const prebuilt = struct {
    pub fn addLibraryPathsTo(compile_step: *std.Build.Step.Compile) void {
        const b = compile_step.step.owner;
        const target = compile_step.rootModuleTarget();
        switch (target.os.tag) {
            .windows => {
                if (target.cpu.arch.isX86()) {
                    if (b.lazyDependency("sdl2-prebuilt-x86_64-windows-gnu", .{})) |sdl2_prebuilt| {
                        compile_step.addLibraryPath(sdl2_prebuilt.path("lib"));
                    }
                }
            },
            .linux => {
                if (target.cpu.arch.isX86()) {
                    if (b.lazyDependency("sdl2-prebuilt-x86_64-linux-gnu", .{})) |sdl2_prebuilt| {
                        compile_step.addLibraryPath(sdl2_prebuilt.path("lib"));
                    }
                }
            },
            .macos => {
                if (b.lazyDependency("sdl2-prebuilt-macos", .{})) |sdl2_prebuilt| {
                    compile_step.addFrameworkPath(sdl2_prebuilt.path("Frameworks"));
                }
            },
            else => {},
        }
    }

    pub fn install_SDL2(
        b: *std.Build,
        target: std.Target,
        install_dir: std.Build.InstallDir,
    ) ?*std.Build.Step {
        switch (target.os.tag) {
            .windows => {
                if (target.cpu.arch.isX86()) {
                    if (b.lazyDependency("sdl2-prebuilt-x86_64-windows-gnu", .{})) |sdl2_prebuilt| {
                        return &b.addInstallFileWithDir(
                            sdl2_prebuilt.path("bin/SDL2.dll"),
                            install_dir,
                            "SDL2.dll",
                        ).step;
                    }
                }
            },
            .linux => {
                if (target.cpu.arch.isX86()) {
                    if (b.lazyDependency("sdl2-prebuilt-x86_64-linux-gnu", .{})) |sdl2_prebuilt| {
                        return &b.addInstallFileWithDir(
                            sdl2_prebuilt.path("lib/libSDL2.so"),
                            install_dir,
                            "libSDL2.so",
                        ).step;
                    }
                }
            },
            .macos => {
                if (b.lazyDependency("sdl2-prebuilt-macos", .{})) |sdl2_prebuilt| {
                    return &b.addInstallDirectory(.{
                        .source_dir = sdl2_prebuilt.path("Frameworks/SDL2.framework"),
                        .install_dir = install_dir,
                        .install_subdir = "SDL2.framework",
                    }).step;
                }
            },
            else => {},
        }
        return null;
    }

    pub fn install_SDL2_ttf(
        b: *std.Build,
        target: std.Target,
        install_dir: std.Build.InstallDir,
    ) ?*std.Build.Step {
        switch (target.os.tag) {
            .windows => {
                if (target.cpu.arch.isX86()) {
                    if (b.lazyDependency("sdl2-prebuilt-x86_64-windows-gnu", .{})) |sdl2_prebuilt| {
                        return &b.addInstallFileWithDir(
                            sdl2_prebuilt.path("bin/SDL2_ttf.dll"),
                            install_dir,
                            "SDL2_ttf.dll",
                        ).step;
                    }
                }
            },
            .linux => {
                if (target.cpu.arch.isX86()) {
                    if (b.lazyDependency("sdl2-prebuilt-x86_64-linux-gnu", .{})) |sdl2_prebuilt| {
                        return &b.addInstallFileWithDir(
                            sdl2_prebuilt.path("lib/libSDL2_ttf.so"),
                            install_dir,
                            "libSDL2_ttf.so",
                        ).step;
                    }
                }
            },
            .macos => {
                if (b.lazyDependency("sdl2-prebuilt-macos", .{})) |sdl2_prebuilt| {
                    return &b.addInstallDirectory(.{
                        .source_dir = sdl2_prebuilt.path("Frameworks/SDL2_ttf.framework"),
                        .install_dir = install_dir,
                        .install_subdir = "SDL2_ttf.framework",
                    }).step;
                }
            },
            else => {},
        }
        return null;
    }

    pub fn install_SDL2_image(
        b: *std.Build,
        target: std.Target,
        install_dir: std.Build.InstallDir,
    ) ?*std.Build.Step {
        switch (target.os.tag) {
            .windows => {
                if (target.cpu.arch.isX86()) {
                    if (b.lazyDependency("sdl2-prebuilt-x86_64-windows-gnu", .{})) |sdl2_prebuilt| {
                        return &b.addInstallFileWithDir(
                            sdl2_prebuilt.path("bin/SDL2_image.dll"),
                            install_dir,
                            "SDL2_image.dll",
                        ).step;
                    }
                }
            },
            .linux => {
                if (target.cpu.arch.isX86()) {
                    if (b.lazyDependency("sdl2-prebuilt-x86_64-linux-gnu", .{})) |sdl2_prebuilt| {
                        return &b.addInstallFileWithDir(
                            sdl2_prebuilt.path("lib/libSDL2_image.so"),
                            install_dir,
                            "libSDL2_image.so",
                        ).step;
                    }
                }
            },
            .macos => {
                if (b.lazyDependency("sdl2-prebuilt-macos", .{})) |sdl2_prebuilt| {
                    return &b.addInstallDirectory(.{
                        .source_dir = sdl2_prebuilt.path("Frameworks/SDL2_image.framework"),
                        .install_dir = install_dir,
                        .install_subdir = "SDL2_image.framework",
                    }).step;
                }
            },
            else => {},
        }
        return null;
    }
};
