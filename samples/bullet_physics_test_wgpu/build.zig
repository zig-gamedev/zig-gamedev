const std = @import("std");
const glfw = @import("../../libs/mach-glfw/build.zig");
const zgpu = @import("../../libs/zgpu/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const zmesh = @import("../../libs/zmesh/build.zig");

const Options = @import("../../build.zig").Options;
const demo_name = "bullet_physics_test_wgpu";
const content_dir = demo_name ++ "_content/";
const use_32bit_indices = true;

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe_options = b.addOptions();
    exe_options.addOption([]const u8, "content_dir", content_dir);
    exe_options.addOption(bool, "zmesh_shape_use_32bit_indices", use_32bit_indices);

    const exe = b.addExecutable(
        demo_name,
        thisDir() ++ "/src/" ++ demo_name ++ ".zig",
    );
    exe.addOptions("build_options", exe_options);

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = thisDir() ++ "/" ++ content_dir,
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    exe.step.dependOn(&install_content_step.step);

    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);

    const options_pkg = exe_options.getPackage("build_options");
    const zmesh_pkg = zmesh.getPkg(&.{options_pkg});
    const zgpu_pkg = zgpu.getPkg(&.{glfw.pkg});

    exe.addPackage(zmesh_pkg);
    exe.addPackage(glfw.pkg);
    exe.addPackage(zgpu_pkg);
    exe.addPackage(zmath.pkg);

    zgpu.link(exe, .{
        .glfw_options = .{},
        .gpu_dawn_options = .{ .from_source = options.dawn_from_source },
    });
    zmesh.link(exe, .{ .shape_use_32bit_indices = use_32bit_indices });

    return exe;
}

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
