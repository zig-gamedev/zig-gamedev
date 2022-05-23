const std = @import("std");
const glfw = @import("../../libs/mach-glfw/build.zig");
const zgpu = @import("../../libs/zgpu/build.zig");
const zmath = @import("../../libs/zmath/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "triangle_wgpu_content/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe_options = b.addOptions();
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const exe = b.addExecutable("triangle_wgpu", thisDir() ++ "/src/triangle_wgpu.zig");
    exe.addOptions("build_options", exe_options);

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = thisDir() ++ "/" ++ content_dir,
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    exe.step.dependOn(&install_content_step.step);

    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);

    exe.addPackage(glfw.pkg);
    exe.addPackage(zgpu.pkg);
    exe.addPackage(zmath.pkg);

    const zgpu_options = zgpu.Options{
        .glfw_options = .{},
        .gpu_dawn_options = .{ .from_source = options.dawn_from_source },
    };
    zgpu.link(exe, zgpu_options);

    return exe;
}

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
