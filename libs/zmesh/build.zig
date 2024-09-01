const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const options = .{
        .shape_use_32bit_indices = b.option(
            bool,
            "shape_use_32bit_indices",
            "Enable par shapes 32-bit indices",
        ) orelse true,
        .shared = b.option(
            bool,
            "shared",
            "Build as shared library",
        ) orelse false,
    };

    const options_step = b.addOptions();
    inline for (std.meta.fields(@TypeOf(options))) |field| {
        options_step.addOption(field.type, field.name, @field(options, field.name));
    }

    const options_module = options_step.createModule();

    _ = b.addModule("root", .{
        .root_source_file = b.path("src/root.zig"),
        .imports = &.{
            .{ .name = "zmesh_options", .module = options_module },
        },
    });

    const zmesh_lib = if (options.shared) blk: {
        const lib = b.addSharedLibrary(.{
            .name = "zmesh",
            .target = target,
            .optimize = optimize,
        });

        if (target.result.os.tag == .windows) {
            lib.defineCMacro("PAR_SHAPES_API", "__declspec(dllexport)");
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
    b.installArtifact(zmesh_lib);

    zmesh_lib.linkLibC();
    if (target.result.abi != .msvc)
        zmesh_lib.linkLibCpp();

    const par_shapes_t = if (options.shape_use_32bit_indices)
        "-DPAR_SHAPES_T=uint32_t"
    else
        "-DPAR_SHAPES_T=uint16_t";

    zmesh_lib.addIncludePath(b.path("libs/par_shapes"));
    zmesh_lib.addCSourceFile(.{
        .file = b.path("libs/par_shapes/par_shapes.c"),
        .flags = &.{ "-std=c99", "-fno-sanitize=undefined", par_shapes_t },
    });

    zmesh_lib.addCSourceFiles(.{
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
    zmesh_lib.addIncludePath(b.path("libs/cgltf"));
    zmesh_lib.addCSourceFile(.{
        .file = b.path("libs/cgltf/cgltf.c"),
        .flags = &.{"-std=c99"},
    });

    const test_step = b.step("test", "Run zmesh tests");

    const tests = b.addTest(.{
        .name = "zmesh-tests",
        .root_source_file = b.path("src/root.zig"),
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(tests);

    tests.linkLibrary(zmesh_lib);
    tests.addIncludePath(b.path("libs/cgltf"));

    test_step.dependOn(&b.addRunArtifact(tests).step);
}
