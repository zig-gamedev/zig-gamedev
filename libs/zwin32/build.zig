const std = @import("std");

pub const Libs = packed struct(u32) {
    d3d12: bool = false,
    xaudio2: bool = false,
    directml: bool = false,
    __unused: u29 = 0,
};

pub const Package = struct {
    zwin32: *std.Build.Module,
    install_d3d12: *std.Build.Step,
    install_xaudio2: *std.Build.Step,
    install_directml: *std.Build.Step,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile, libs: Libs) void {
        exe.root_module.addImport("zwin32", pkg.zwin32);
        if (libs.d3d12) exe.step.dependOn(pkg.install_d3d12);
        if (libs.xaudio2) exe.step.dependOn(pkg.install_xaudio2);
        if (libs.directml) exe.step.dependOn(pkg.install_directml);
    }
};

pub fn package(
    b: *std.Build,
    _: std.Build.ResolvedTarget,
    _: std.builtin.Mode,
    _: struct {},
) Package {
    const install_d3d12 = b.allocator.create(std.Build.Step) catch @panic("OOM");
    install_d3d12.* = std.Build.Step.init(.{
        .id = .custom,
        .name = "zwin32-install-d3d12",
        .owner = b,
    });

    const install_xaudio2 = b.allocator.create(std.Build.Step) catch @panic("OOM");
    install_xaudio2.* = std.Build.Step.init(.{
        .id = .custom,
        .name = "zwin32-install-xaudio2",
        .owner = b,
    });

    const install_directml = b.allocator.create(std.Build.Step) catch @panic("OOM");
    install_directml.* = std.Build.Step.init(.{
        .id = .custom,
        .name = "zwin32-install-directml",
        .owner = b,
    });

    install_d3d12.dependOn(
        &b.addInstallFile(
            .{ .path = thisDir() ++ "/bin/x64/D3D12Core.dll" },
            "bin/d3d12/D3D12Core.dll",
        ).step,
    );
    install_d3d12.dependOn(
        &b.addInstallFile(
            .{ .path = thisDir() ++ "/bin/x64/D3D12SDKLayers.dll" },
            "bin/d3d12/D3D12SDKLayers.dll",
        ).step,
    );

    install_xaudio2.dependOn(
        &b.addInstallFile(
            .{ .path = thisDir() ++ "/bin/x64/xaudio2_9redist.dll" },
            "bin/xaudio2_9redist.dll",
        ).step,
    );

    install_directml.dependOn(
        &b.addInstallFile(
            .{ .path = thisDir() ++ "/bin/x64/DirectML.dll" },
            "bin/DirectML.dll",
        ).step,
    );
    install_directml.dependOn(
        &b.addInstallFile(
            .{ .path = thisDir() ++ "/bin/x64/DirectML.Debug.dll" },
            "bin/DirectML.Debug.dll",
        ).step,
    );

    return .{
        .zwin32 = b.addModule("zwin32", .{
            .root_source_file = .{ .path = thisDir() ++ "/src/zwin32.zig" },
        }),
        .install_d3d12 = install_d3d12,
        .install_xaudio2 = install_xaudio2,
        .install_directml = install_directml,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    _ = package(b, target, optimize, .{});
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
