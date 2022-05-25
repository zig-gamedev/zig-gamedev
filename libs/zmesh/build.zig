const std = @import("std");

const BuildOptions = struct {
    shape_use_32bit_indices: bool = false,
};

pub fn getPkg(dependencies: []const std.build.Pkg) std.build.Pkg {
    return .{
        .name = "zmesh",
        .path = .{ .path = thisDir() ++ "/src/main.zig" },
        .dependencies = dependencies,
    };
}

pub fn build(b: *std.build.Builder) void {
    const build_mode = b.standardReleaseOptions();
    const target = b.standardTargetOptions(.{});
    const tests = buildTests(b, build_mode, target);

    const test_step = b.step("test", "Run zmesh tests");
    test_step.dependOn(&tests.step);
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/main.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests, .{});
    return tests;
}

fn buildLibrary(exe: *std.build.LibExeObjStep, options: BuildOptions) *std.build.LibExeObjStep {
    const lib = exe.builder.addStaticLibrary("zmesh", thisDir() ++ "/src/main.zig");

    const lib_options = exe.builder.addOptions();
    lib.addOptions("build_options", lib_options);
    lib_options.addOption(bool, "zmesh_shape_use_32bit_indices", options.shape_use_32bit_indices);

    lib.setBuildMode(exe.build_mode);
    lib.setTarget(exe.target);
    lib.linkSystemLibrary("c");
    lib.linkSystemLibrary("c++");

    const par_shapes_t = if (options.shape_use_32bit_indices) "-DPAR_SHAPES_T=uint32_t" else "";

    lib.addIncludeDir(thisDir() ++ "/libs/par_shapes");
    lib.addCSourceFile(
        thisDir() ++ "/libs/par_shapes/par_shapes.c",
        &.{ "-std=c99", "-fno-sanitize=undefined", par_shapes_t },
    );

    lib.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/clusterizer.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/indexgenerator.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/vcacheoptimizer.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/vcacheanalyzer.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/vfetchoptimizer.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/vfetchanalyzer.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/overdrawoptimizer.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/overdrawanalyzer.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/allocator.cpp", &.{""});

    lib.addIncludeDir(thisDir() ++ "/libs/cgltf");
    lib.addCSourceFile(thisDir() ++ "/libs/cgltf/cgltf.c", &.{"-std=c99"});

    return lib;
}

pub fn link(exe: *std.build.LibExeObjStep, options: BuildOptions) void {
    const lib = buildLibrary(exe, options);
    exe.linkLibrary(lib);
    exe.addIncludeDir(thisDir() ++ "/libs/cgltf");
}

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
