const std = @import("std");

pub fn build(b: *std.Build) void {
    const zwindows = b.dependency("zwindows", .{
        .zxaudio2_debug_layer = b.option(
            bool,
            "zxaudio2_debug_layer",
            "Enable XAudio2 debug layer",
        ) orelse false,
        .zd3d12_debug_layer = b.option(
            bool,
            "zd3d12_debug_layer",
            "Enable DirectX 12 debug layer",
        ) orelse false,
        .zd3d12_gbv = b.option(
            bool,
            "zd3d12_gbv",
            "Enable DirectX 12 GPU-Based Validation (GBV)",
        ) orelse false,
    });
    _ = b.addModule("root", .{
        .root_source_file = b.path("src/openvr.zig"),
        .imports = &.{
            .{ .name = "zwindows", .module = zwindows.module("zwindows") },
        },
    });
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
