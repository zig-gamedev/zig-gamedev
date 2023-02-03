const std = @import("std");
const zgpu = @import("../../libs/zgpu/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const zpool = @import("../../libs/zpool/build.zig");
const zglfw = @import("../../libs/zglfw/build.zig");
const zgui = @import("../../libs/zgui/build.zig");
const zphysics = @import("../../libs/zphysics/build.zig");
const zmesh = @import("../../libs/zmesh/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "physics_test_wgpu_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    const exe = b.addExecutable(.{
        .name = "physics_test_wgpu",
        .root_source_file = .{ .path = thisDir() ++ "/src/physics_test_wgpu.zig" },
        .target = options.target,
        .optimize = options.build_mode,
    });

    const exe_options = b.addOptions();
    exe.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = thisDir() ++ "/" ++ content_dir,
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    exe.step.dependOn(&install_content_step.step);

    const zmesh_options = zmesh.BuildOptionsStep.init(b, .{});
    const zgpu_options = zgpu.BuildOptionsStep.init(b, .{});
    const zphysics_options = zphysics.BuildOptionsStep.init(b, .{ .use_double_precision = false });
    const zgui_options = zgui.BuildOptionsStep.init(b, .{ .backend = .glfw_wgpu });

    const zmesh_pkg = zmesh.getPkg(&.{zmesh_options.getPkg()});
    const zphysics_pkg = zphysics.getPkg(&.{zphysics_options.getPkg()});
    const zgpu_pkg = zgpu.getPkg(&.{ zgpu_options.getPkg(), zpool.pkg, zglfw.pkg });
    const zgui_pkg = zgui.getPkg(&.{zgui_options.getPkg()});

    exe.addPackage(zmesh_pkg);
    exe.addPackage(zgpu_pkg);
    exe.addPackage(zphysics_pkg);
    exe.addPackage(zgui_pkg);
    exe.addPackage(zmath.pkg);
    exe.addPackage(zglfw.pkg);

    zmesh.link(exe, zmesh_options);
    zgpu.link(exe, zgpu_options);
    zphysics.link(exe, zphysics_options);
    zgui.link(exe, zgui_options);
    zglfw.link(exe);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
