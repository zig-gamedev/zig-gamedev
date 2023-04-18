const std = @import("std");

pub const Options = struct {
    shape_use_32bit_indices: bool = true,
    shared: bool = false,
};

pub const Package = struct {
    options: Options,
    zmesh: *std.Build.Module,
    zmesh_options: *std.Build.Module,
    zmesh_c_cpp: *std.Build.CompileStep,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.linkLibrary(pkg.zmesh_c_cpp);
        exe.addModule("zmesh", pkg.zmesh);
        exe.addModule("zmesh_options", pkg.zmesh_options);
    }
};

pub fn package(
    b: *std.Build,
    target: std.zig.CrossTarget,
    optimize: std.builtin.Mode,
    args: struct {
        options: Options = .{},
    },
) Package {
    const step = b.addOptions();
    step.addOption(bool, "shape_use_32bit_indices", args.options.shape_use_32bit_indices);
    step.addOption(bool, "shared", args.options.shared);

    const zmesh_options = step.createModule();

    const zmesh = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/main.zig" },
        .dependencies = &.{
            .{ .name = "zmesh_options", .module = zmesh_options },
        },
    });

    const zmesh_c_cpp = if (args.options.shared) blk: {
        const lib = b.addSharedLibrary(.{
            .name = "zmesh",
            .target = target,
            .optimize = optimize,
        });

        if (target.isWindows()) {
            lib.defineCMacro("CGLTF_API", "__declspec(dllexport)");
            lib.defineCMacro("MESHOPTIMIZER_API", "__declspec(dllexport)");
            lib.defineCMacro("ZMESH_API", "__declspec(dllexport)");
        }

        break :blk lib;
    } else b.addStaticLibrary(.{
        .name = "zmesh",
        .target = target,
        .optimize = optimize,
    });

    zmesh_c_cpp.linkLibC();
    zmesh_c_cpp.linkLibCpp();

    const par_shapes_t = if (args.options.shape_use_32bit_indices)
        "-DPAR_SHAPES_T=uint32_t"
    else
        "-DPAR_SHAPES_T=uint16_t";

    zmesh_c_cpp.addIncludePath(thisDir() ++ "/libs/par_shapes");
    zmesh_c_cpp.addCSourceFile(
        thisDir() ++ "/libs/par_shapes/par_shapes.c",
        &.{ "-std=c99", "-fno-sanitize=undefined", par_shapes_t },
    );

    zmesh_c_cpp.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/clusterizer.cpp", &.{""});
    zmesh_c_cpp.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/indexgenerator.cpp", &.{""});
    zmesh_c_cpp.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/vcacheoptimizer.cpp", &.{""});
    zmesh_c_cpp.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/vcacheanalyzer.cpp", &.{""});
    zmesh_c_cpp.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/vfetchoptimizer.cpp", &.{""});
    zmesh_c_cpp.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/vfetchanalyzer.cpp", &.{""});
    zmesh_c_cpp.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/overdrawoptimizer.cpp", &.{""});
    zmesh_c_cpp.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/overdrawanalyzer.cpp", &.{""});
    zmesh_c_cpp.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/simplifier.cpp", &.{""});
    zmesh_c_cpp.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/allocator.cpp", &.{""});

    zmesh_c_cpp.addIncludePath(thisDir() ++ "/libs/cgltf");
    zmesh_c_cpp.addCSourceFile(thisDir() ++ "/libs/cgltf/cgltf.c", &.{"-std=c99"});

    return .{
        .options = args.options,
        .zmesh = zmesh,
        .zmesh_options = zmesh_options,
        .zmesh_c_cpp = zmesh_c_cpp,
    };
}

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zmesh tests");
    test_step.dependOn(runTests(b, optimize, target));
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.Step {
    const tests = b.addTest(.{
        .name = "zmesh-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/main.zig" },
        .target = target,
        .optimize = optimize,
    });

    const zmesh_pkg = package(b, target, optimize, .{});
    zmesh_pkg.link(tests);

    return &b.addRunArtifact(tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
