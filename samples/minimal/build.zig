const std = @import("std");
const zwin32 = @import("../../libs/zwin32/build.zig");
const ztracy = @import("../../libs/ztracy/build.zig");
const zd3d12 = @import("../../libs/zd3d12/build.zig");
const common = @import("../../libs/common/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "minimal/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe_options = b.addOptions();
    exe_options.addOption(bool, "enable_dx_debug", options.enable_dx_debug);
    exe_options.addOption(bool, "enable_dx_gpu_debug", options.enable_dx_gpu_debug);
    exe_options.addOption(bool, "enable_d2d", false);
    exe_options.addOption([]const u8, "content_dir", content_dir);
    exe_options.addOption(bool, "zpix_enable", options.zpix_enable);

    const exe = b.addExecutable("minimal", thisDir() ++ "/src/minimal.zig");
    exe.setBuildMode(options.build_mode);
    exe.setTarget(options.target);
    exe.addOptions("build_options", exe_options);

    exe.rdynamic = true;
    exe.want_lto = false;

    const ztracy_options = ztracy.BuildOptionsStep.init(b, .{ .enable_ztracy = options.ztracy_enable });

    const options_pkg = exe_options.getPackage("build_options");
    const ztracy_pkg = ztracy.getPkg(&.{ztracy_options.getPkg()});
    const zd3d12_pkg = zd3d12.getPkg(&.{ ztracy_pkg, zwin32.pkg, options_pkg });
    const common_pkg = common.getPkg(&.{ zd3d12_pkg, ztracy_pkg, zwin32.pkg, options_pkg });

    exe.addPackage(ztracy_pkg);
    exe.addPackage(zd3d12_pkg);
    exe.addPackage(common_pkg);
    exe.addPackage(zwin32.pkg);

    ztracy.link(exe, ztracy_options);
    zd3d12.link(exe);
    common.link(exe);

    return exe;
}

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
