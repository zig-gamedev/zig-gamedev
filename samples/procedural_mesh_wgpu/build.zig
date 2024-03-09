const std = @import("std");

const Options = @import("../../build.zig").Options;
const content_dir = "procedural_mesh_wgpu_content/";

pub fn build(b: *std.Build, options: Options) *std.Build.Step.Compile {
    const exe = b.addExecutable(.{
        .name = "procedural_mesh_wgpu",
        .root_source_file = .{ .path = thisDir() ++ "/src/procedural_mesh_wgpu.zig" },
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

    const zmesh = b.dependency("zmesh", .{
        .target = options.target,
    });
    exe.root_module.addImport("zmesh", zmesh.module("root"));
    exe.linkLibrary(zmesh.artifact("zmesh"));

    const znoise = b.dependency("znoise", .{
        .target = options.target,
    });
    exe.root_module.addImport("znoise", znoise.module("root"));
    exe.linkLibrary(znoise.artifact("FastNoiseLite"));

    const ztracy = b.dependency("ztracy", .{
        .target = options.target,
    });
    exe.root_module.addImport("ztracy", ztracy.module("root"));
    exe.linkLibrary(ztracy.artifact("tracy"));

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
