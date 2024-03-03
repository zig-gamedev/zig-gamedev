const std = @import("std");

pub const Options = struct {
    shape_use_32bit_indices: bool = true,
    shared: bool = false,
};

pub fn link(d: *std.Build.Dependency, exe: *std.Build.Step.Compile) void {
    exe.linkLibrary(d.artifact("zmesh"));
    exe.root_module.addImport("zmesh", d.module("zmesh"));
    exe.root_module.addImport("zmesh_options", d.module("zmesh_options"));
}

/// deprecated: use b.dependency and call link on the result
pub const Package = struct {
    options: Options,
    zmesh: *std.Build.Module,
    zmesh_options: *std.Build.Module,
    zmesh_c_cpp: *std.Build.Step.Compile,

    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.linkLibrary(pkg.zmesh_c_cpp);
        exe.root_module.addImport("zmesh", pkg.zmesh);
        exe.root_module.addImport("zmesh_options", pkg.zmesh_options);
    }
};

pub fn package(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.Mode,
    args: struct {
        options: Options = .{},
    },
) Package {
    const step = b.addOptions();
    step.addOption(bool, "shape_use_32bit_indices", args.options.shape_use_32bit_indices);
    step.addOption(bool, "shared", args.options.shared);

    const zmesh_options = step.createModule();
    _ = b.addModule("zmesh_options", .{
        .root_source_file = step.getOutput(),
    });

    const zmesh = b.addModule("zmesh", .{
        .root_source_file = .{ .path = "src/main.zig" },
        .imports = &.{
            .{ .name = "zmesh_options", .module = zmesh_options },
        },
    });

    const zmesh_c_cpp = if (args.options.shared) blk: {
        const lib = b.addSharedLibrary(.{
            .name = "zmesh",
            .target = target,
            .optimize = optimize,
        });

        if (target.result.os.tag == .windows) {
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

    b.getInstallStep().dependOn(&b.addInstallArtifact(zmesh_c_cpp, .{}).step);

    zmesh_c_cpp.linkLibC();
    if (target.result.abi != .msvc)
        zmesh_c_cpp.linkLibCpp();

    const par_shapes_t = if (args.options.shape_use_32bit_indices)
        "-DPAR_SHAPES_T=uint32_t"
    else
        "-DPAR_SHAPES_T=uint16_t";

    zmesh_c_cpp.addIncludePath(.{ .path = "libs/par_shapes" });
    zmesh_c_cpp.addCSourceFile(.{
        .file = .{ .path = "libs/par_shapes/par_shapes.c" },
        .flags = &.{ "-std=c99", "-fno-sanitize=undefined", par_shapes_t },
    });

    zmesh_c_cpp.addCSourceFiles(.{
        .files = &.{
            "libs/meshoptimizer/clusterizer.cpp",
            "libs/meshoptimizer/indexgenerator.cpp",
            "libs/meshoptimizer/vcacheoptimizer.cpp",
            "libs/meshoptimizer/vcacheanalyzer.cpp",
            "libs/meshoptimizer/vfetchoptimizer.cpp",
            "libs/meshoptimizer/vfetchanalyzer.cpp",
            "libs/meshoptimizer/overdrawoptimizer.cpp",
            "libs/meshoptimizer/overdrawanalyzer.cpp",
            "libs/meshoptimizer/simplifier.cpp",
            "libs/meshoptimizer/allocator.cpp",
        },
        .flags = &.{""},
    });
    zmesh_c_cpp.addIncludePath(.{ .path = "libs/cgltf" });
    zmesh_c_cpp.addCSourceFile(.{
        .file = .{ .path = "libs/cgltf/cgltf.c" },
        .flags = &.{"-std=c99"},
    });

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

    const pkg  = package(b, target, optimize, .{
        .options = .{
            .shape_use_32bit_indices = b.option(bool, "shape_use_32bit_indices", "Enable par shapes 32-bit indices") orelse true,
            .shared = b.option(bool, "shared", "Build as shared library") orelse false,
        },
    });

    const tests = b.addTest(.{
        .name = "zmesh-tests",
        .root_source_file = .{ .path = "src/main.zig" },
        .target = target,
        .optimize = optimize,
    });

    tests.addIncludePath(.{ .path = "libs/cgltf" });
    pkg.link(tests);

    const test_step = b.step("test", "Run zmesh tests");
    test_step.dependOn(&b.addRunArtifact(tests).step);

    b.getInstallStep().dependOn(&b.addInstallArtifact(tests, .{}).step);
}
