const std = @import("std");
const zgpu = @import("../../libs/zgpu/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const zpool = @import("../../libs/zpool/build.zig");
const zglfw = @import("../../libs/zglfw/build.zig");
const zstbi = @import("../../libs/zstbi/build.zig");
const zgui = @import("../../libs/zgui/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "transparent_quads_wgpu_content/";

pub fn build(b: *std.build.Builder, options: Options) *std.build.LibExeObjStep {
    const exe = b.addExecutable("transparent_quads_wgpu", thisDir() ++ "/src/transparent_quads_wgpu.zig");

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

    const zgpu_options = zgpu.BuildOptionsStep.init(b, .{});
    const zgpu_pkg = zgpu.getPkg(&.{ zgpu_options.getPkg(), zpool.pkg, zglfw.pkg });

    const zgui_options = zgui.BuildOptionsStep.init(b, .{ .backend = .glfw_wgpu });
    const zgui_pkg = zgui.getPkg(&.{zgui_options.getPkg()});

    exe.addPackage(zgpu_pkg);
    exe.addPackage(zgui_pkg);
    exe.addPackage(zmath.pkg);
    exe.addPackage(zglfw.pkg);
    exe.addPackage(zstbi.pkg);

    zgpu.link(exe, zgpu_options);
    zgui.link(exe, zgui_options);
    zglfw.link(exe);
    zstbi.link(exe);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
