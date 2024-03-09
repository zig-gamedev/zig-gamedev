const builtin = @import("builtin");
const std = @import("std");
const assert = std.debug.assert;

pub fn build(b: *std.Build) !void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = b.addModule("zsdl2", .{
        .root_source_file = .{ .path = "src/sdl2.zig" },
        .imports = &.{},
    });

    _ = b.addModule("zsdl2_ttf", .{
        .root_source_file = .{ .path = "src/sdl2.zig" },
        .imports = &.{},
    });

    _ = b.addModule("zsdl3", .{
        .root_source_file = .{ .path = "src/sdl3.zig" },
        .imports = &.{},
    });

    const test_step = b.step("test", "Run zsdl tests");

    { // SDL2 tests
        const tests_sdl2 = b.addTest(.{
            .name = "sdl2-tests",
            .root_source_file = .{ .path = "src/sdl2.zig" },
            .target = target,
            .optimize = optimize,
        });
        addLibraryPathsTo(tests_sdl2);
        link_SDL2(tests_sdl2);
        b.installArtifact(tests_sdl2);

        test_step.dependOn(&b.addRunArtifact(tests_sdl2).step);
    }

    { // SDL2_ttf tests
        const tests_sdl2_ttf = b.addTest(.{
            .name = "sdl2_ttf-tests",
            .root_source_file = .{ .path = "src/sdl2_ttf.zig" },
            .target = target,
            .optimize = optimize,
        });
        addLibraryPathsTo(tests_sdl2_ttf);
        link_SDL2(tests_sdl2_ttf);
        link_SDL2_ttf(tests_sdl2_ttf);
        b.installArtifact(tests_sdl2_ttf);

        test_step.dependOn(&b.addRunArtifact(tests_sdl2_ttf).step);
    }

    // TODO(hazeycode):
    // { // SDL3 tests
    //     const tests_sdl3 = b.addTest(.{
    //         .name = "sdl3-tests",
    //         .root_source_file = .{ .path = "src/sdl3.zig" },
    //         .target = target,
    //         .optimize = optimize,
    //     });
    //     b.installArtifact(tests_sdl3);

    //     addLibraryPathsTo(tests_sdl3);
    //     link_SDL3(tests_sdl3);

    //     test_step.dependOn(&b.addRunArtifact(tests_sdl3).step);
    // }

    { // SDL2 version check step
        const version_check_step = b.step(
            "version-check",
            "checks runtime library version is the same as the compiled version",
        );
        const tests_sdl2_version_check = b.addTest(.{
            .name = "sdl2-version-check",
            .root_source_file = .{ .path = "src/sdl2_version_check.zig" },
            .target = target,
            .optimize = optimize,
        });
        addLibraryPathsTo(tests_sdl2_version_check);
        link_SDL2(tests_sdl2_version_check);

        version_check_step.dependOn(&b.addRunArtifact(tests_sdl2_version_check).step);
    }
}

fn addLibraryPathsTo(compile_step: *std.Build.Step.Compile) void {
    const target = compile_step.rootModuleTarget();
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                compile_step.addLibraryPath(
                    .{ .path = "libs/x86_64-windows-gnu/lib" },
                );
                compile_step.addRPath(
                    .{ .path = "libs/x86_64-windows-gnu/bin" },
                );
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                compile_step.addLibraryPath(
                    .{ .path = "libs/x86_64-linux-gnu/lib" },
                );
                compile_step.addRPath(
                    .{ .path = "libs/x86_64-linux-gnu/lib" },
                );
            }
        },
        .macos => {
            compile_step.addFrameworkPath(
                .{ .path = "libs/macos/Frameworks" },
            );
            compile_step.addRPath(
                .{ .path = "libs/macos/Frameworks" },
            );
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
