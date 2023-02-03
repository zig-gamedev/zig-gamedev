const std = @import("std");
const zgpu = @import("../../libs/zgpu/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const zmesh = @import("../../libs/zmesh/build.zig");
const zpool = @import("../../libs/zpool/build.zig");
const ztracy = @import("../../libs/ztracy/build.zig");
const zbullet = @import("../../libs/zbullet/build.zig");
const zglfw = @import("../../libs/zglfw/build.zig");
const zgui = @import("../../libs/zgui/build.zig");

const Options = @import("../../build.zig").Options;

const demo_name = "bullet_physics_test_wgpu";
const content_dir = demo_name ++ "_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = .{ .path = thisDir() ++ "/src/" ++ demo_name ++ ".zig" },
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

    const zmesh_options = zmesh.BuildOptionsStep.init(b, .{ .shape_use_32bit_indices = true });
    const ztracy_options = ztracy.BuildOptionsStep.init(b, .{ .enable_ztracy = options.ztracy_enable });
    const zgpu_options = zgpu.BuildOptionsStep.init(b, .{});
    const zgui_options = zgui.BuildOptionsStep.init(b, .{ .backend = .glfw_wgpu });

    const zmesh_pkg = zmesh.getPkg(&.{zmesh_options.getPkg()});
    const ztracy_pkg = ztracy.getPkg(&.{ztracy_options.getPkg()});
    const zgpu_pkg = zgpu.getPkg(&.{ zgpu_options.getPkg(), zpool.pkg, zglfw.pkg });
    const zgui_pkg = zgui.getPkg(&.{zgui_options.getPkg()});

    exe.addPackage(zmesh_pkg);
    exe.addPackage(ztracy_pkg);
    exe.addPackage(zgpu_pkg);
    exe.addPackage(zgui_pkg);
    exe.addPackage(zmath.pkg);
    exe.addPackage(zbullet.pkg);
    exe.addPackage(zglfw.pkg);

    zmesh.link(exe, zmesh_options);
    ztracy.link(exe, ztracy_options);
    zgpu.link(exe, zgpu_options);
    zgui.link(exe, zgui_options);
    zbullet.link(exe);
    zglfw.link(exe);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
