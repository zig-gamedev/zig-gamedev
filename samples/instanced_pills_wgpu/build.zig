const std = @import("std");

const Options = @import("../../build.zig").Options;

const demo_name = "instanced_pills_wgpu";
const demo_filename = "instanced_pills_wgpu";
const content_dir = demo_name ++ "_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    const emscripten = options.target.getOsTag() == .emscripten;
    const target = if (emscripten) std.zig.CrossTarget.parse(.{ .arch_os_abi = "wasm32-freestanding" }) catch unreachable else options.target;
    const exe_desc = .{
        .name = demo_name,
        .root_source_file = .{ .path = thisDir() ++ "/src/" ++ demo_filename ++ ".zig" },
        .target = target,
        .optimize = options.optimize,
    };
    const exe = if (emscripten) b.addStaticLibrary(exe_desc) else b.addExecutable(exe_desc);

    const zgui_pkg = @import("../../build.zig").zgui_pkg;
    const zmath_pkg = @import("../../build.zig").zmath_pkg;
    const zgpu_pkg = @import("../../build.zig").zgpu_pkg;
    const zglfw_pkg = @import("../../build.zig").zglfw_pkg;
    const zems_pkg = @import("../../build.zig").zems_pkg;

    zgui_pkg.link(exe);
    zgpu_pkg.link(exe);
    zglfw_pkg.link(exe);
    zmath_pkg.link(exe);
    zems_pkg.link(exe);

    const exe_options = b.addOptions();
    exe.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = thisDir() ++ "/" ++ content_dir,
        .install_dir = .{ .custom = "" },
        .install_subdir = "bin/" ++ content_dir,
    });
    exe.step.dependOn(&install_content_step.step);

    return exe;
}

const zems = @import("../../build.zig").zems;
pub fn buildEmscripten(b: *std.Build, options: Options)  *zems.EmscriptenStep {
    const exe = build(b, options);
    var ems_step = zems.EmscriptenStep.init(b);
    ems_step.args.setDefault(options.optimize, true);
    ems_step.args.setOrAssertOption("USE_GLFW", "3");
    ems_step.args.setOrAssertOption("USE_WEBGPU", "");
    ems_step.args.other_args.appendSlice(&.{"--preload-file", "samples/instanced_pills_wgpu/instanced_pills_wgpu_content" ++ "@instanced_pills_wgpu_content"}) catch unreachable;
    ems_step.link(exe);
    return ems_step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
