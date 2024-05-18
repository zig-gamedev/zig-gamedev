const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const zwin32 = b.dependency("zwin32", .{
        .target = target,
    });
    _ = b.addModule("root", .{
        .root_source_file = b.path("src/openvr.zig"),
        .imports = &.{
            .{ .name = "zwin32", .module = zwin32.module("root") },
        },
    });

    {
        const unit_tests = b.step("test", "Run zopenvr tests");
        {
            const tests = b.addTest(.{
                .name = "openvr-tests",
                .root_source_file = b.path("src/openvr.zig"),
                .target = target,
                .optimize = optimize,
            });
            addLibraryPathsTo(tests);
            addRPathsTo(tests);
            linkOpenVR(tests);
            b.installArtifact(tests);

            const tests_exe = b.addRunArtifact(tests);
            if (target.result.os.tag == .windows) {
                tests_exe.setCwd(.{
                    .cwd_relative = b.getInstallPath(.bin, ""),
                });
            }
            unit_tests.dependOn(&tests_exe.step);
        }

        installOpenVR(unit_tests, target.result, .bin);
    }
}

// in future zig version e342433
pub fn pathResolve(b: *std.Build, paths: []const []const u8) []u8 {
    return std.fs.path.resolve(b.allocator, paths) catch @panic("OOM");
}

pub fn addLibraryPathsTo(compile_step: *std.Build.Step.Compile) void {
    const b = compile_step.step.owner;
    const target = compile_step.rootModuleTarget();
    const source_path_prefix = comptime std.fs.path.dirname(@src().file) orelse ".";
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                compile_step.addLibraryPath(.{
                    .cwd_relative = b.pathJoin(
                        &.{ source_path_prefix, "libs/openvr/lib/win64" },
                    ),
                });
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                compile_step.addLibraryPath(.{
                    .cwd_relative = b.pathJoin(
                        &.{ source_path_prefix, "libs/openvr/lib/linux64" },
                    ),
                });
            }
        },
        else => {},
    }
}

pub fn addRPathsTo(compile_step: *std.Build.Step.Compile) void {
    const b = compile_step.step.owner;
    const target = compile_step.rootModuleTarget();
    const source_path_prefix = comptime std.fs.path.dirname(@src().file) orelse ".";
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                compile_step.addRPath(.{
                    .cwd_relative = b.pathJoin(
                        &.{ source_path_prefix, "libs/openvr/bin/win64" },
                    ),
                });
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                compile_step.addRPath(.{
                    .cwd_relative = b.pathJoin(
                        &.{ source_path_prefix, "libs/openvr/bin/linux64" },
                    ),
                });
            }
        },
        else => {},
    }
}

pub fn linkOpenVR(compile_step: *std.Build.Step.Compile) void {
    switch (compile_step.rootModuleTarget().os.tag) {
        .windows, .linux => {
            compile_step.linkSystemLibrary("openvr_api");
        },
        else => {},
    }
}

pub fn installOpenVR(
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
                                &.{ source_path_prefix, "libs/openvr/bin/win64/openvr_api.dll" },
                            ),
                        },
                        install_dir,
                        "openvr_api.dll",
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
                                &.{ source_path_prefix, "libs/openvr/bin/linux64/libopenvr_api.so" },
                            ),
                        },
                        install_dir,
                        "libopenvr_api.so.0",
                    ).step,
                );
            }
        },
        else => {},
    }
}
