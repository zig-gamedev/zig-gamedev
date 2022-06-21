const std = @import("std");
const zgpu = @import("../../libs/zgpu/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const zpool = @import("../../libs/zpool/build.zig");
const zaudio = @import("../../libs/zaudio/build.zig");

const Options = @import("../../build.zig").Options;

const demo_name = "audio_experiments_wgpu";
const content_dir = demo_name ++ "_content/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe = b.addExecutable(demo_name, thisDir() ++ "/src/" ++ demo_name ++ ".zig");

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
    exe.addPackage(zaudio.pkg);

    zgpu.link(exe, zgpu_options);
    zaudio.link(exe);

    return exe;
}

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
