const std = @import("std");

pub fn build(b: *std.Build) !void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = b.addModule("root", .{
        .root_source_file = .{ .path = "src/zwin32.zig" },
    });

    const test_step = b.step("test", "Run zwin32 tests");

    const tests = b.addTest(.{
        .name = "zwin32-tests",
        .root_source_file = .{ .path = "src/zwin32.zig" },
        .target = target,
        .optimize = optimize,
    });

    b.installArtifact(tests);

    test_step.dependOn(&b.addRunArtifact(tests).step);
}

pub fn install_xaudio2(
    step: *std.Build.Step,
    install_dir: std.Build.InstallDir,
    source_path_prefix: []const u8,
) !void {
    const b = step.owner;
    step.dependOn(
        &b.addInstallFileWithDir(
            .{ .path = try std.fs.path.join(
                b.allocator,
                &.{ source_path_prefix, "bin/x64/xaudio2_9redist.dll" },
            ) },
            install_dir,
            "xaudio2_9redist.dll",
        ).step,
    );
}

pub fn install_d3d12(
    step: *std.Build.Step,
    install_dir: std.Build.InstallDir,
    source_path_prefix: []const u8,
) !void {
    const b = step.owner;
    step.dependOn(
        &b.addInstallFileWithDir(
            .{ .path = try std.fs.path.join(
                b.allocator,
                &.{ source_path_prefix, "bin/x64/D3D12Core.dll" },
            ) },
            install_dir,
            "d3d12/D3D12Core.dll",
        ).step,
    );
    step.dependOn(
        &b.addInstallFileWithDir(
            .{ .path = try std.fs.path.join(
                b.allocator,
                &.{ source_path_prefix, "bin/x64/D3D12SDKLayers.dll" },
            ) },
            install_dir,
            "d3d12/D3D12SDKLayers.dll",
        ).step,
    );
}

pub fn install_directml(
    step: *std.Build.Step,
    install_dir: std.Build.InstallDir,
    source_path_prefix: []const u8,
) !void {
    const b = step.owner;
    step.dependOn(
        &b.addInstallFileWithDir(
            .{ .path = try std.fs.path.join(
                b.allocator,
                &.{ source_path_prefix, "bin/x64/DirectML.dll" },
            ) },
            install_dir,
            "DirectML.dll",
        ).step,
    );
    step.dependOn(
        &b.addInstallFileWithDir(
            .{ .path = try std.fs.path.join(
                b.allocator,
                &.{ source_path_prefix, "bin/x64/DirectML.Debug.dll" },
            ) },
            install_dir,
            "DirectML.Debug.dll",
        ).step,
    );
}
