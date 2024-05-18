const builtin = @import("builtin");
const std = @import("std");
const assert = std.debug.assert;

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = b.addModule("zsdl2", .{
        .root_source_file = b.path("src/sdl2.zig"),
        .imports = &.{},
    });

    _ = b.addModule("zsdl2_ttf", .{
        .root_source_file = b.path("src/sdl2.zig"),
        .imports = &.{},
    });

    _ = b.addModule("zsdl3", .{
        .root_source_file = b.path("src/sdl3.zig"),
        .imports = &.{},
    });

    {
        const unit_tests = b.step("test", "Run zsdl tests");

        { // SDL2 tests
            const tests_sdl2 = b.addTest(.{
                .name = "sdl2-tests",
                .root_source_file = b.path("src/sdl2.zig"),
                .target = target,
                .optimize = optimize,
            });
            addLibraryPathsTo(tests_sdl2);
            addRPathsTo(tests_sdl2);
            link_SDL2(tests_sdl2);
            b.installArtifact(tests_sdl2);

            const tests_exe = b.addRunArtifact(tests_sdl2);
            if (target.result.os.tag == .windows) {
                tests_exe.setCwd(.{
                    .cwd_relative = b.getInstallPath(.bin, ""),
                });
            }
            unit_tests.dependOn(&tests_exe.step);
        }

        { // SDL2_ttf tests
            const tests_sdl2_ttf = b.addTest(.{
                .name = "sdl2_ttf-tests",
                .root_source_file = b.path("src/sdl2_ttf.zig"),
                .target = target,
                .optimize = optimize,
            });
            addLibraryPathsTo(tests_sdl2_ttf);
            addRPathsTo(tests_sdl2_ttf);
            link_SDL2(tests_sdl2_ttf);
            link_SDL2_ttf(tests_sdl2_ttf);
            b.installArtifact(tests_sdl2_ttf);

            const tests_exe = b.addRunArtifact(tests_sdl2_ttf);
            if (target.result.os.tag == .windows) {
                tests_exe.setCwd(.{
                    .cwd_relative = b.getInstallPath(.bin, ""),
                });
            }
            unit_tests.dependOn(&tests_exe.step);
        }

        // TODO(hazeycode):
        // { // SDL3 tests
        //     const tests_sdl3 = b.addTest(.{
        //         .name = "sdl3-tests",
        //         .root_source_file = b.path("src/sdl3.zig" ),
        //         .target = target,
        //         .optimize = optimize,
        //     });
        //     b.installArtifact(tests_sdl3);

        //     addLibraryPathsTo(tests_sdl3, "");
        //     link_SDL3(tests_sdl3);

        //     unit_tests.dependOn(&b.addRunArtifact(tests_sdl3).step);
        // }

        install_sdl2(unit_tests, target.result, .bin);
        install_sdl2_ttf(unit_tests, target.result, .bin);
    }

    { // SDL2 version check step
        const version_check_step = b.step(
            "version-check",
            "checks runtime library version is the same as the compiled version",
        );
        const tests_sdl2_version_check = b.addTest(.{
            .name = "sdl2-version-check",
            .root_source_file = b.path("src/sdl2_version_check.zig"),
            .target = target,
            .optimize = optimize,
        });
        addLibraryPathsTo(tests_sdl2_version_check);
        addRPathsTo(tests_sdl2_version_check);
        link_SDL2(tests_sdl2_version_check);

        const version_check = b.addRunArtifact(tests_sdl2_version_check);
        if (target.result.os.tag == .windows) {
            version_check.setCwd(.{
                .cwd_relative = b.getInstallPath(.bin, ""),
            });
        }
        version_check_step.dependOn(&version_check.step);

        install_sdl2(version_check_step, target.result, .bin);
    }
}

pub fn addLibraryPathsTo(
    compile_step: *std.Build.Step.Compile,
) void {
    const b = compile_step.step.owner;
    const target = compile_step.rootModuleTarget();
    const source_path_prefix = comptime std.fs.path.dirname(@src().file) orelse ".";
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                compile_step.addLibraryPath(.{
                    .cwd_relative = b.pathJoin(
                        &.{ source_path_prefix, "libs/x86_64-windows-gnu/lib" },
                    ),
                });
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                compile_step.addLibraryPath(.{
                    .cwd_relative = b.pathJoin(
                        &.{ source_path_prefix, "libs/x86_64-linux-gnu/lib" },
                    ),
                });
            }
        },
        .macos => {
            compile_step.addFrameworkPath(.{
                .cwd_relative = b.pathJoin(
                    &.{ source_path_prefix, "libs/macos/Frameworks" },
                ),
            });
        },
        else => {},
    }
}

pub fn addRPathsTo(
    compile_step: *std.Build.Step.Compile,
) void {
    const b = compile_step.step.owner;
    const target = compile_step.rootModuleTarget();
    const source_path_prefix = comptime std.fs.path.dirname(@src().file) orelse ".";
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                compile_step.addRPath(.{
                    .cwd_relative = b.pathJoin(
                        &.{ source_path_prefix, "libs/x86_64-windows-gnu/bin" },
                    ),
                });
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                compile_step.addRPath(.{
                    .cwd_relative = b.pathJoin(
                        &.{ source_path_prefix, "libs/x86_64-linux-gnu/lib" },
                    ),
                });
            }
        },
        .macos => {
            compile_step.addRPath(.{
                .cwd_relative = b.pathJoin(
                    &.{ source_path_prefix, "libs/macos/Frameworks" },
                ),
            });
        },
        else => {},
    }
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

pub fn install_sdl2(
    step: *std.Build.Step,
    target: std.Target,
    install_dir: std.Build.InstallDir,
) void {
    const b = step.owner;
    const source_path_prefix = comptime std.fs.path.dirname(@src().file) orelse ".";
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                step.dependOn(
                    &b.addInstallFileWithDir(
                        .{
                            .cwd_relative = b.pathJoin(
                                &.{ source_path_prefix, "libs/x86_64-windows-gnu/bin/SDL2.dll" },
                            ),
                        },
                        install_dir,
                        "SDL2.dll",
                    ).step,
                );
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                step.dependOn(
                    &b.addInstallFileWithDir(
                        .{
                            .cwd_relative = b.pathJoin(
                                &.{ source_path_prefix, "libs/x86_64-linux-gnu/lib/libSDL2.so" },
                            ),
                        },
                        install_dir,
                        "libSDL2.so",
                    ).step,
                );
            }
        },
        .macos => {
            step.dependOn(
                &b.addInstallDirectory(.{
                    .source_dir = .{
                        .cwd_relative = b.pathJoin(
                            &.{ source_path_prefix, "libs/macos/Frameworks/SDL2.framework" },
                        ),
                    },
                    .install_dir = install_dir,
                    .install_subdir = "SDL2.framework",
                }).step,
            );
        },
        else => {},
    }
}

pub fn install_sdl2_ttf(
    step: *std.Build.Step,
    target: std.Target,
    install_dir: std.Build.InstallDir,
) void {
    const b = step.owner;
    const source_path_prefix = comptime std.fs.path.dirname(@src().file) orelse ".";
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                step.dependOn(
                    &b.addInstallFileWithDir(
                        .{
                            .cwd_relative = b.pathJoin(
                                &.{ source_path_prefix, "libs/x86_64-windows-gnu/bin/SDL2_ttf.dll" },
                            ),
                        },
                        install_dir,
                        "SDL2_ttf.dll",
                    ).step,
                );
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                step.dependOn(
                    &b.addInstallFileWithDir(
                        .{
                            .cwd_relative = b.pathJoin(
                                &.{ source_path_prefix, "libs/x86_64-linux-gnu/lib/libSDL2_ttf.so" },
                            ),
                        },
                        install_dir,
                        "libSDL2_ttf.so",
                    ).step,
                );
            }
        },
        .macos => {
            step.dependOn(
                &b.addInstallDirectory(.{
                    .source_dir = .{
                        .cwd_relative = b.pathJoin(
                            &.{ source_path_prefix, "libs/macos/Frameworks/SDL2_ttf.framework" },
                        ),
                    },
                    .install_dir = install_dir,
                    .install_subdir = "SDL2_ttf.framework",
                }).step,
            );
        },
        else => {},
    }
}
