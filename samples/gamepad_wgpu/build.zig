const std = @import("std");

const Options = @import("../../build.zig").Options;

const demo_name = "gamepad_wgpu";
const content_dir = demo_name ++ "_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = .{ .path = thisDir() ++ "/src/" ++ demo_name ++ ".zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    const zgui_pkg = @import("../../build.zig").zgui_glfw_wgpu_pkg;
    const zmath_pkg = @import("../../build.zig").zmath_pkg;
    const zgpu_pkg = @import("../../build.zig").zgpu_pkg;
    const zglfw_pkg = @import("../../build.zig").zglfw_pkg;

    zgui_pkg.link(exe);
    zgpu_pkg.link(exe);
    zglfw_pkg.link(exe);
    zmath_pkg.link(exe);

    const exe_options = b.addOptions();
    exe.root_module.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = .{ .path = thisDir() ++ "/" ++ content_dir },
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    exe.step.dependOn(&install_content_step.step);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
