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

pub fn addLibraryPathsTo(zopenvr: *std.Build.Dependency, compile_step: *std.Build.Step.Compile) void {
    const target = compile_step.rootModuleTarget();
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                compile_step.addLibraryPath(.{ .dependency = .{
                    .dependency = zopenvr,
                    .sub_path = "libs/openvr/lib/win64",
                } });
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                compile_step.addLibraryPath(.{ .dependency = .{
                    .dependency = zopenvr,
                    .sub_path = "libs/openvr/lib/linux64",
                } });
            }
        },
        else => {},
    }
}

pub fn addRPathsTo(zopenvr: *std.Build.Dependency, compile_step: *std.Build.Step.Compile) void {
    const target = compile_step.rootModuleTarget();
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                compile_step.addRPath(.{ .dependency = .{
                    .dependency = zopenvr,
                    .sub_path = "libs/openvr/bin/win64",
                } });
            }
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                compile_step.addRPath(.{ .dependency = .{
                    .dependency = zopenvr,
                    .sub_path = "libs/openvr/bin/linux64",
                } });
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
    zopenvr: *std.Build.Dependency,
    step: *std.Build.Step,
    target: std.Target,
    install_dir: std.Build.InstallDir,
) void {
    const b = step.owner;
    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                step.dependOn(
                    &b.addInstallFileWithDir(
                        .{ .dependency = .{
                            .dependency = zopenvr,
                            .sub_path = "libs/openvr/bin/win64/openvr_api.dll",
                        } },
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
                        .{ .dependency = .{
                            .dependency = zopenvr,
                            .sub_path = "libs/openvr/bin/linux64/libopenvr_api.so",
                        } },
                        install_dir,
                        "libopenvr_api.so.0",
                    ).step,
                );
            }
        },
        else => {},
    }
}
