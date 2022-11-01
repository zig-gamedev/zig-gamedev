const std = @import("std");
const zgpu = @import("../../libs/zgpu/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const zmesh = @import("../../libs/zmesh/build.zig");
const znoise = @import("../../libs/znoise/build.zig");
const zpool = @import("../../libs/zpool/build.zig");
const zglfw = @import("../../libs/zglfw/build.zig");
const zgui = @import("../../libs/zgui/build.zig");
const ztracy = @import("../../libs/ztracy/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "procedural_mesh_wgpu_content/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe = b.addExecutable("procedural_mesh_wgpu", thisDir() ++ "/src/procedural_mesh_wgpu.zig");

    const exe_options = b.addOptions();
    exe.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = thisDir() ++ "/" ++ content_dir,
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    exe.step.dependOn(&install_content_step.step);

    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);

    const zmesh_options = zmesh.BuildOptionsStep.init(b, .{});
    const zgpu_options = zgpu.BuildOptionsStep.init(b, .{});
    const ztracy_options = ztracy.BuildOptionsStep.init(b, .{ .enable_ztracy = true, .enable_fibers = true });

    const zmesh_pkg = zmesh.getPkg(&.{zmesh_options.getPkg()});
    const zgpu_pkg = zgpu.getPkg(&.{ zgpu_options.getPkg(), zpool.pkg, zglfw.pkg });
    const ztracy_pkg = ztracy.getPkg(&.{ztracy_options.getPkg()});

    exe.addPackage(zmesh_pkg);
    exe.addPackage(zgpu_pkg);
    exe.addPackage(zgui.pkg);
    exe.addPackage(zmath.pkg);
    exe.addPackage(znoise.pkg);
    exe.addPackage(zglfw.pkg);
    exe.addPackage(ztracy_pkg);

    ztracy.link(exe, ztracy_options);
    zgpu.link(exe, zgpu_options);
    zmesh.link(exe, zmesh_options);
    znoise.link(exe);
    zglfw.link(exe);
    zgui.link(exe);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
