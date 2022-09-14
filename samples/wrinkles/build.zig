const std = @import("std");
const zgpu = @import("../../libs/zgpu/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const zpool = @import("../../libs/zpool/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "textured_quad_wgpu_content/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe = b.addExecutable("wrinkles", thisDir() ++ "/src/wrinkles.zig");

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

    const zgpu_options = zgpu.BuildOptionsStep.init(b, .{
        .dawn = .{ .from_source = options.zgpu_dawn_from_source },
    });
    const zgpu_pkg = zgpu.getPkg(&.{ zgpu_options.getPkg(), zpool.pkg });

    exe.addPackage(zgpu_pkg);
    exe.addPackage(zmath.pkg);

    zgpu.link(exe, zgpu_options);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
