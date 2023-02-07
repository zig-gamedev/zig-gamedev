const std = @import("std");
const zgpu = @import("../../libs/zgpu/build.zig");
const zmath = @import("../../libs/zmath/build.zig");
const zpool = @import("../../libs/zpool/build.zig");
const zglfw = @import("../../libs/zglfw/build.zig");
const zgui = @import("../../libs/zgui/build.zig");

const Options = @import("../../build.zig").Options;
const content_dir = "triangle_wgpu_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    const exe = b.addExecutable(.{
        .name = "triangle_wgpu",
        .root_source_file = .{ .path = thisDir() ++ "/src/triangle_wgpu.zig" },
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

    const zmath_pkg = zmath.package(b, .{});
    const zglfw_pkg = zglfw.package(b, .{});
    const zpool_pkg = zpool.package(b, .{});
    const zgui_pkg = zgui.package(b, .{
        .options = .{ .backend = .glfw_wgpu },
    });
    const zgpu_pkg = zgpu.package(b, .{
        .options = .{ .uniforms_buffer_size = 4 * 1024 * 1024 },
        .deps = .{ .zpool = zpool_pkg.module, .zglfw = zglfw_pkg.module },
    });

    exe.addModule("zgpu", zgpu_pkg.module);
    exe.addModule("zgui", zgui_pkg.module);
    exe.addModule("zmath", zmath_pkg.module);
    exe.addModule("zglfw", zglfw_pkg.module);

    zgpu.link(exe);
    zglfw.link(exe);
    zgui.link(exe, zgui_pkg.options);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
