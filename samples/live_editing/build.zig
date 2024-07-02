const builtin = @import("builtin");
const std = @import("std");

const Options = @import("../../build.zig").Options;

const demo_name = "live_editing";
const content_dir = demo_name ++ "_content/";

// in future zig version e342433
pub fn pathResolve(b: *std.Build, paths: []const []const u8) []u8 {
    return std.fs.path.resolve(b.allocator, paths) catch @panic("OOM");
}

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });
    const exports = buildExports(b, options) catch unreachable;
    if (b.option(bool, "live-editing-exports-only", "") orelse false) {
        return exports;
    }
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = b.path(b.pathJoin(&.{ src_path, demo_name ++ ".zig" })),
        .target = options.target,
        .optimize = options.optimize,
    });

    @import("system_sdk").addLibraryPathsTo(exe);

    const zglfw = b.dependency("zglfw", .{
        .target = options.target,
    });
    exe.root_module.addImport("zglfw", zglfw.module("root"));
    exe.linkLibrary(zglfw.artifact("glfw"));

    const zwin32 = b.dependency("zwin32", .{
        .target = options.target,
    });
    const zwin32_module = zwin32.module("root");
    exe.root_module.addImport("zwin32", zwin32_module);

    const zd3d12 = b.dependency("zd3d12", .{
        .target = options.target,
        .debug_layer = options.zd3d12_enable_debug_layer,
        .gbv = options.zd3d12_enable_gbv,
    });
    const zd3d12_module = zd3d12.module("root");
    exe.root_module.addImport("zd3d12", zd3d12_module);

    const install_exports = b.addInstallArtifact(exports, .{});
    exe.step.dependOn(&install_exports.step);

    // This is needed to export symbols from an .exe file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exe.rdynamic = true;

    @import("zwin32").install_d3d12(&exe.step, .bin);

    const exe_options = b.addOptions();
    exe.root_module.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);
    exe_options.addOption([]const []const u8, "reloadable_files", reloadable_files);
    exe_options.addOption([]const u8, "exports_lib_path", b.pathJoin(&.{
        std.fs.path.basename(b.getInstallPath(.prefix, "")),
        "exports-lib",
    }));

    return exe;
}

pub const reloadable_files: []const []const u8 = &.{ "entry.zig", "exports.zig", "live_editing.hlsl" };

fn buildExports(b: *std.Build, options: Options) !*std.Build.Step.Compile {
    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });
    const exports_name = exports_name: {
        var hash = std.crypto.hash.sha2.Sha256.init(.{});
        for (reloadable_files) |reloadable_file| {
            const source = try std.fs.cwd().readFileAlloc(b.allocator, b.pathJoin(&.{
                src_path,
                reloadable_file,
            }), 1024 * 1024);

            hash.update(source);
        }

        break :exports_name &std.fmt.bytesToHex(hash.finalResult(), .lower);
    };

    std.fs.deleteFileAbsolute(b.getInstallPath(.prefix, "exports-lib")) catch |err| switch (err) {
        error.FileNotFound => {},
        else => return err,
    };

    std.fs.makeDirAbsolute(b.getInstallPath(.prefix, "")) catch |err| switch (err) {
        error.PathAlreadyExists => {},
        else => return err,
    };

    const building_exports_lib_path = b.pathJoin(&.{
        std.fs.path.basename(b.getInstallPath(.prefix, "")),
        "building-exports-lib",
    });
    try std.fs.cwd().writeFile(
        building_exports_lib_path,
        b.getInstallPath(.bin, b.fmt("{s}.dll", .{exports_name})),
    );

    const install_exports_lib = b.addInstallFile(b.path(building_exports_lib_path), "exports-lib");

    var exports = b.addSharedLibrary(.{
        .name = exports_name,
        .root_source_file = b.path(b.pathJoin(&.{ src_path, "exports.zig" })),
        .target = options.target,
        .optimize = options.optimize,
    });
    exports.step.dependOn(&install_exports_lib.step);

    @import("system_sdk").addLibraryPathsTo(exports);

    const zglfw = b.dependency("zglfw", .{
        .target = options.target,
    });
    exports.root_module.addImport("zglfw", zglfw.module("root"));
    exports.linkLibrary(zglfw.artifact("glfw"));

    const zwin32 = b.dependency("zwin32", .{
        .target = options.target,
    });
    const zwin32_module = zwin32.module("root");
    exports.root_module.addImport("zwin32", zwin32_module);

    const zd3d12 = b.dependency("zd3d12", .{
        .target = options.target,
        .debug_layer = options.zd3d12_enable_debug_layer,
        .gbv = options.zd3d12_enable_gbv,
    });
    const zd3d12_module = zd3d12.module("root");
    exports.root_module.addImport("zd3d12", zd3d12_module);

    const content_path = b.pathJoin(&.{ cwd_path, content_dir });
    const install_content_step = b.addInstallDirectory(.{
        .source_dir = b.path(content_path),
        .install_dir = .{ .custom = "" },
        .install_subdir = b.pathJoin(&.{ "bin", content_dir }),
    });

    if (builtin.os.tag == .windows or builtin.os.tag == .linux) {
        const compile_shaders = @import("zwin32").addCompileShaders(b, demo_name, .{ .shader_ver = "6_6" });
        const root_path = pathResolve(b, &.{ @src().file, "..", "..", ".." });

        const hlsl_path = b.pathJoin(&.{ root_path, src_path, demo_name ++ ".hlsl" });
        compile_shaders.addVsShader(hlsl_path, "vsMain", b.pathJoin(&.{ root_path, content_path, demo_name ++ ".vs.cso" }), "");
        compile_shaders.addPsShader(hlsl_path, "psMain", b.pathJoin(&.{ root_path, content_path, demo_name ++ ".ps.cso" }), "");

        install_content_step.step.dependOn(compile_shaders.step);
    }
    exports.step.dependOn(&install_content_step.step);

    // This is needed to export symbols from an .exports file.
    // We export D3D12SDKVersion and D3D12SDKPath symbols which
    // is required by DirectX 12 Agility SDK.
    exports.rdynamic = true;

    const lib_options = b.addOptions();
    exports.root_module.addOptions("build_options", lib_options);
    lib_options.addOption([]const u8, "content_dir", content_dir);

    return exports;
}
