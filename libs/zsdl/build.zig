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
        },
        .macos => {
            compile_step.linkFramework("SDL2");
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
        },
        .macos => {
            compile_step.linkFramework("SDL2_ttf");
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
        },
        .macos => {
            compile_step.linkFramework("SDL2_image");
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

pub fn addLibraryPathsTo(libs_source_path: []const u8, compile_step: *std.Build.Step.Compile) void {
    const b = compile_step.step.owner;
    const target = compile_step.rootModuleTarget();
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                compile_step.addLibraryPath(.{
                    .cwd_relative = b.pathJoin(&.{ libs_source_path, "x86_64-windows-gnu/lib" }),
                });
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                compile_step.addLibraryPath(.{
                    .cwd_relative = b.pathJoin(&.{ libs_source_path, "x86_64-linux-gnu/lib" }),
                });
            }
        },
        .macos => {
            compile_step.addFrameworkPath(.{
                .cwd_relative = b.pathJoin(&.{ libs_source_path, "macos/Frameworks" }),
            });
        },
        else => {},
    }
}

pub fn addRPathsTo(libs_source_path: []const u8, compile_step: *std.Build.Step.Compile) void {
    const b = compile_step.step.owner;
    const target = compile_step.rootModuleTarget();
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                compile_step.addRPath(.{
                    .cwd_relative = b.pathJoin(&.{ libs_source_path, "x86_64-windows-gnu/bin" }),
                });
            }
            compile_step.root_module.addRPathSpecial("$ORIGIN");
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                compile_step.addRPath(.{
                    .cwd_relative = b.pathJoin(&.{ libs_source_path, "x86_64-linux-gnu/lib" }),
                });
            }
            compile_step.root_module.addRPathSpecial("@executable_path");
        },
        .macos => {
            compile_step.addRPath(.{
                .cwd_relative = b.pathJoin(&.{ libs_source_path, "macos/Frameworks" }),
            });
        },
        else => {},
    }
}

pub fn install_SDL2(
    b: *std.Build,
    target: std.Target,
    libs_source_path: []const u8,
    install_dir: std.Build.InstallDir,
) ?*std.Build.Step {
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                return &b.addInstallFileWithDir(
                    .{ .cwd_relative = b.pathJoin(&.{ libs_source_path, "x86_64-windows-gnu/bin/SDL2.dll" }) },
                    install_dir,
                    "SDL2.dll",
                ).step;
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                return &b.addInstallFileWithDir(
                    .{ .cwd_relative = b.pathJoin(&.{ libs_source_path, "x86_64-linux-gnu/lib/libSDL2.so" }) },
                    install_dir,
                    "libSDL2.so",
                ).step;
            }
        },
        .macos => {
            return &b.addInstallDirectory(.{
                .source_dir = .{ .cwd_relative = b.pathJoin(&.{ libs_source_path, "macos/Frameworks/SDL2.framework" }) },
                .install_dir = install_dir,
                .install_subdir = "SDL2.framework",
            }).step;
        },
        else => {},
    }
    return null;
}

pub fn install_SDL2_ttf(
    b: *std.Build,
    target: std.Target,
    libs_source_path: []const u8,
    install_dir: std.Build.InstallDir,
) ?*std.Build.Step {
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                return &b.addInstallFileWithDir(
                    .{ .cwd_relative = b.pathJoin(&.{ libs_source_path, "x86_64-windows-gnu/bin/SDL2_ttf.dll" }) },
                    install_dir,
                    "SDL2_ttf.dll",
                ).step;
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                return &b.addInstallFileWithDir(
                    .{ .cwd_relative = b.pathJoin(&.{ libs_source_path, "x86_64-linux-gnu/lib/libSDL2_ttf.so" }) },
                    install_dir,
                    "libSDL2_ttf.so",
                ).step;
            }
        },
        .macos => {
            return &b.addInstallDirectory(.{
                .source_dir = .{ .cwd_relative = b.pathJoin(&.{ libs_source_path, "macos/Frameworks/SDL2_ttf.framework" }) },
                .install_dir = install_dir,
                .install_subdir = "SDL2_ttf.framework",
            }).step;
        },
        else => {},
    }
    return null;
}

pub fn install_SDL2_image(
    b: *std.Build,
    target: std.Target,
    libs_source_path: []const u8,
    install_dir: std.Build.InstallDir,
) ?*std.Build.Step {
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                return &b.addInstallFileWithDir(
                    .{ .cwd_relative = b.pathJoin(&.{ libs_source_path, "x86_64-windows-gnu/bin/SDL2_image.dll" }) },
                    install_dir,
                    "SDL2_image.dll",
                ).step;
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                return &b.addInstallFileWithDir(
                    .{ .cwd_relative = b.pathJoin(&.{ libs_source_path, "x86_64-linux-gnu/lib/libSDL2_image.so" }) },
                    install_dir,
                    "libSDL2_image.so",
                ).step;
            }
        },
        .macos => {
            return &b.addInstallDirectory(.{
                .source_dir = .{ .cwd_relative = b.pathJoin(&.{ libs_source_path, "macos/Frameworks/SDL2_image.framework" }) },
                .install_dir = install_dir,
                .install_subdir = "SDL2_image.framework",
            }).step;
        },
        else => {},
    }
    return null;
}

pub fn testVersionCheckSDL2(b: *std.Build, target: std.Build.ResolvedTarget) *std.Build.Step {
    const sdl2_prebuilt = b.dependency("sdl2-prebuilt", .{});
    const sdl2_libs_path = sdl2_prebuilt.path("").getPath(b);

    const test_sdl2_version_check = b.addTest(.{
        .name = "sdl2-version-check",
        .root_source_file = b.dependency("zsdl", .{}).path("src/sdl2_version_check.zig"),
        .target = target,
        .optimize = .ReleaseSafe,
    });

    link_SDL2(test_sdl2_version_check);

    addLibraryPathsTo(sdl2_libs_path, test_sdl2_version_check);
    addRPathsTo(sdl2_libs_path, test_sdl2_version_check);

    const version_check_run = b.addRunArtifact(test_sdl2_version_check);

    if (target.result.os.tag == .windows) {
        version_check_run.setCwd(.{
            .cwd_relative = b.getInstallPath(.bin, ""),
        });
    }

    version_check_run.step.dependOn(&test_sdl2_version_check.step);

    if (install_SDL2(b, target.result, sdl2_libs_path, .bin)) |install_sdl2_step| {
        version_check_run.step.dependOn(install_sdl2_step);
    }

    return &version_check_run.step;
}
