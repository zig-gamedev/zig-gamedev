const std = @import("std");
const zgpu = @import("../../libs/zgpu/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const zpool = @import("../../libs/zpool/build.zig");
const zglfw = @import("../../libs/zglfw/build.zig");
const zgui = @import("../../libs/zgui/build.zig");

const Options = @import("../../build.zig").Options;

const demo_name = "gamepad_wgpu";
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

    const zmath_pkg = zmath.package(b, .{}, .{});
    const zglfw_pkg = zglfw.package(b, .{}, .{});
    const zpool_pkg = zpool.package(b, .{}, .{});
    const zgui_pkg = zgui.package(b, .{ .backend = .glfw_wgpu }, .{});
    const zgpu_pkg = zgpu.package(
        b,
        .{},
        .{ .zpool_module = zpool_pkg.module, .zglfw_module = zglfw_pkg.module },
    );

    exe.addModule("zgpu", zgpu_pkg.module);
    exe.addModule("zgui", zgui_pkg.module);
    exe.addModule("zmath", zmath_pkg.module);
    exe.addModule("zglfw", zglfw_pkg.module);

    zgpu.link(exe, zgpu_pkg.options);
    zgui.link(exe, zgui_pkg.options);
    zglfw.link(exe, .{});

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
