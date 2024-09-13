const std = @import("std");

pub fn build(_: *std.Build) void {}

/// Deprecated: This function invites the user to enforce a dependency on system-sdk even when
/// library paths may not be required for some build environments / target platforms. Instead
/// of calling this, users should conditionally use lazy dependencies, see README.md for example.
pub fn addLibraryPathsTo(compile_step: *std.Build.Step.Compile) void {
    const b = compile_step.step.owner;
    const target = compile_step.rootModuleTarget();

    const system_sdk = b.lazyDependency("system_sdk", .{}).?;

    switch (target.os.tag) {
        .windows => {
            if (target.cpu.arch.isX86()) {
                if (target.abi.isGnu() or target.abi.isMusl()) {
                    compile_step.addLibraryPath(system_sdk.path("windows/lib/x86_64-windows-gnu"));
                }
            }
        },
        .macos => {
            compile_step.addLibraryPath(system_sdk.path("macos12/usr/lib"));
            compile_step.addFrameworkPath(system_sdk.path("macos12/System/Library/Frameworks"));
        },
        .linux => {
            if (target.cpu.arch.isX86()) {
                compile_step.addLibraryPath(system_sdk.path("linux/lib/x86_64-linux-gnu"));
            } else if (target.cpu.arch == .aarch64) {
                compile_step.addLibraryPath(system_sdk.path("linux/lib/aarch64-linux-gnu"));
            }
        },
        else => {},
    }
}
