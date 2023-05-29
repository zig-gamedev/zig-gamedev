const std = @import("std");

const Options = @import("../../build.zig").Options;
const content_dir = "triangle_wgpu_emscripten_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.CompileStep {
    if (options.emscripten) std.debug.assert(options.target.getCpuArch() == .wasm32 or options.target.getCpuArch() == .wasm64);
    const exe_desc = .{
        .name = "triangle_wgpu_emscripten",
        .root_source_file = .{ .path = thisDir() ++ "/src/triangle_wgpu.zig" },
        .target = options.target,
        .optimize = options.optimize,
    };
    const exe = if (options.emscripten) b.addStaticLibrary(exe_desc) else b.addExecutable(exe_desc);

    const zgui_pkg = @import("../../build.zig").zgui_pkg;
    const zmath_pkg = @import("../../build.zig").zmath_pkg;
    const zgpu_pkg = @import("../../build.zig").zgpu_pkg;
    const zglfw_pkg = @import("../../build.zig").zglfw_pkg;

    zgui_pkg.link(exe);
    zgpu_pkg.link(exe);
    zglfw_pkg.link(exe);
    zmath_pkg.link(exe);

    const exe_options = b.addOptions(); 
    exe.addOptions("build_options", exe_options);
    exe_options.addOption(bool, "emscripten", options.emscripten);
    // exe_options.addOption([]const u8, "content_dir", content_dir);

    // const install_content_step = b.addInstallDirectory(.{
    //     .source_dir = thisDir() ++ "/" ++ content_dir,
    //     .install_dir = .{ .custom = "" },
    //     .install_subdir = "bin/" ++ content_dir,
    // });
    // exe.step.dependOn(&install_content_step.step);

    return exe;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
