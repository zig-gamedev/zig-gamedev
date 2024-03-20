const std = @import("std");

const Options = @import("../../build.zig").Options;

const demo_name = "gui_test_wgpu";
const content_dir = demo_name ++ "_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = .{ .path = thisDir() ++ "/src/" ++ demo_name ++ ".zig" },
        .target = options.target,
        .optimize = options.optimize,
    });

    @import("system_sdk").addLibraryPathsTo(exe);

    const zglfw = b.dependency("zglfw", .{
        .target = options.target,
    });
    exe.root_module.addImport("zglfw", zglfw.module("root"));
    exe.linkLibrary(zglfw.artifact("glfw"));

    @import("zgpu").addLibraryPathsTo(exe);
    const zgpu = b.dependency("zgpu", .{
        .target = options.target,
    });
    exe.root_module.addImport("zgpu", zgpu.module("root"));
    exe.linkLibrary(zgpu.artifact("zdawn"));

    const zgui = b.dependency("zgui", .{
        .target = options.target,
        .backend = .glfw_wgpu,
    });
    exe.root_module.addImport("zgui", zgui.module("root"));
    exe.linkLibrary(zgui.artifact("imgui"));

    const zmath = b.dependency("zmath", .{
        .target = options.target,
    });
    exe.root_module.addImport("zmath", zmath.module("root"));

    const zstbi = b.dependency("zstbi", .{
        .target = options.target,
    });
    exe.root_module.addImport("zstbi", zstbi.module("root"));
    exe.linkLibrary(zstbi.artifact("zstbi"));

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
