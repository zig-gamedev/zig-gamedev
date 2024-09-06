const std = @import("std");
const builtin = @import("builtin");

pub fn build(b: *std.Build) !void {
    const options = .{
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
    };

    const options_step = b.addOptions();
    inline for (std.meta.fields(@TypeOf(options))) |field| {
        options_step.addOption(field.type, field.name, @field(options, field.name));
    }

    const zwindows_module = b.addModule("zwindows", .{
        .root_source_file = b.path("src/zwindows.zig"),
    });

    const options_module = options_step.createModule();

    _ = b.addModule("zd3d12", .{
        .root_source_file = b.path("src/zd3d12.zig"),
        .imports = &.{
            .{ .name = "options", .module = options_module },
            .{ .name = "zwindows", .module = zwindows_module },
        },
    });

    _ = b.addModule("zxaudio2", .{
        .root_source_file = b.path("src/zxaudio2.zig"),
        .imports = &.{
            .{ .name = "options", .module = options_module },
            .{ .name = "zwindows", .module = zwindows_module },
        },
    });
}

pub fn install_xaudio2(
    step: *std.Build.Step,
    install_dir: std.Build.InstallDir,
) void {
    const b = step.owner;
    const source_path_prefix = comptime std.fs.path.dirname(@src().file) orelse ".";
    step.dependOn(
        &b.addInstallFileWithDir(
            .{
                .cwd_relative = b.pathJoin(
                    &.{ source_path_prefix, "bin/x64/xaudio2_9redist.dll" },
                ),
            },
            install_dir,
            "xaudio2_9redist.dll",
        ).step,
    );
}

pub fn install_d3d12(
    step: *std.Build.Step,
    install_dir: std.Build.InstallDir,
) void {
    const b = step.owner;
    const source_path_prefix = comptime std.fs.path.dirname(@src().file) orelse ".";
    step.dependOn(
        &b.addInstallFileWithDir(
            .{
                .cwd_relative = b.pathJoin(
                    &.{ source_path_prefix, "bin/x64/D3D12Core.dll" },
                ),
            },
            install_dir,
            "d3d12/D3D12Core.dll",
        ).step,
    );
    step.dependOn(
        &b.addInstallFileWithDir(
            .{
                .cwd_relative = b.pathJoin(
                    &.{ source_path_prefix, "bin/x64/D3D12SDKLayers.dll" },
                ),
            },
            install_dir,
            "d3d12/D3D12SDKLayers.dll",
        ).step,
    );
}

pub fn install_directml(
    step: *std.Build.Step,
    install_dir: std.Build.InstallDir,
) void {
    const b = step.owner;
    const source_path_prefix = comptime std.fs.path.dirname(@src().file) orelse ".";
    step.dependOn(
        &b.addInstallFileWithDir(
            .{
                .cwd_relative = b.pathJoin(
                    &.{ source_path_prefix, "bin/x64/DirectML.dll" },
                ),
            },
            install_dir,
            "DirectML.dll",
        ).step,
    );
    step.dependOn(
        &b.addInstallFileWithDir(
            .{
                .cwd_relative = b.pathJoin(
                    &.{ source_path_prefix, "bin/x64/DirectML.Debug.dll" },
                ),
            },
            install_dir,
            "DirectML.Debug.dll",
        ).step,
    );
}

pub const CompileShaders = struct {
    step: *std.Build.Step,
    shader_ver: []const u8,

    pub fn addVsShader(
        self: CompileShaders,
        input_path: []const u8,
        entry_point: []const u8,
        output_filename: []const u8,
        define: []const u8,
    ) void {
        self.addShader(
            input_path,
            entry_point,
            output_filename,
            "vs",
            define,
        );
    }
    pub fn addPsShader(
        self: CompileShaders,
        input_path: []const u8,
        entry_point: []const u8,
        output_filename: []const u8,
        define: []const u8,
    ) void {
        self.addShader(
            input_path,
            entry_point,
            output_filename,
            "ps",
            define,
        );
    }

    pub fn addCsShader(
        self: CompileShaders,
        input_path: []const u8,
        entry_point: []const u8,
        output_filename: []const u8,
        define: []const u8,
    ) void {
        self.addShader(
            input_path,
            entry_point,
            output_filename,
            "cs",
            define,
        );
    }

    pub fn addMsShader(
        self: CompileShaders,
        input_path: []const u8,
        entry_point: []const u8,
        output_filename: []const u8,
        define: []const u8,
    ) void {
        self.addShader(
            input_path,
            entry_point,
            output_filename,
            "ms",
            define,
        );
    }

    pub fn addShader(
        self: CompileShaders,
        input_path: []const u8,
        entry_point: []const u8,
        output_filename: []const u8,
        profile: []const u8,
        define: []const u8,
    ) void {
        const b = self.step.owner;

        const zwindows_path = comptime std.fs.path.dirname(@src().file) orelse ".";
        const dxc_path = switch (builtin.target.os.tag) {
            .windows => zwindows_path ++ "/bin/x64/dxc.exe",
            .linux => zwindows_path ++ "/bin/x64/dxc",
            else => @panic("Unsupported target"),
        };

        const dxc_command = [9][]const u8{
            dxc_path,
            input_path,
            b.fmt("/E {s}", .{entry_point}),
            b.fmt("/Fo {s}", .{output_filename}),
            b.fmt("/T {s}_{s}", .{ profile, self.shader_ver }),
            if (define.len == 0) "" else b.fmt("/D {s}", .{define}),
            "/WX",
            "/Ges",
            "/O3",
        };

        const cmd_step = b.addSystemCommand(&dxc_command);
        if (builtin.target.os.tag == .linux) {
            cmd_step.setEnvironmentVariable(
                "LD_LIBRARY_PATH",
                zwindows_path ++ "/bin/x64",
            );
        }
        self.step.dependOn(&cmd_step.step);
    }
};

pub fn addCompileShaders(b: *std.Build, comptime name: []const u8, options: struct { shader_ver: []const u8 }) CompileShaders {
    return .{
        .step = b.step(name ++ "-dxc", "Build shaders for '" ++ name ++ "'"),
        .shader_ver = options.shader_ver,
    };
}
