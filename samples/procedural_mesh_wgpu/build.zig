const std = @import("std");
const glfw = @import("../../libs/mach-glfw/build.zig");
const zgpu = @import("../../libs/zgpu/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const zmesh = @import("../../libs/zmesh/build.zig");
const znoise = @import("../../libs/znoise/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "procedural_mesh_wgpu_content/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe_options = b.addOptions();
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const exe = b.addExecutable("procedural_mesh_wgpu", comptime thisDir() ++ "/src/procedural_mesh_wgpu.zig");
    exe.addOptions("build_options", exe_options);

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = comptime thisDir() ++ "/" ++ content_dir,
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    exe.step.dependOn(&install_content_step.step);

    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);

    exe.addPackage(glfw.pkg);
    exe.addPackage(zgpu.pkg);
    exe.addPackage(zmath.pkg);
    exe.addPackage(zmesh.pkg);
    exe.addPackage(znoise.pkg);

    zgpu.link(exe, .{
        .glfw_options = .{},
        .gpu_dawn_options = .{ .from_source = options.dawn_from_source },
    });
    zmesh.link(exe);
    znoise.link(exe);

    return exe;
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
