const std = @import("std");

pub const Package = struct {
    pub const Options = struct {
        shape_use_32bit_indices: bool = true,
    };

    options: Options,
    zmesh: *std.Build.Module,
    zmesh_options: *std.Build.Module,
    zmesh_c_cpp: *std.Build.CompileStep,

    pub fn build(
        b: *std.Build,
        target: std.zig.CrossTarget,
        optimize: std.builtin.Mode,
        args: struct {
            options: Options = .{},
        },
    ) Package {
        const step = b.addOptions();
        step.addOption(bool, "shape_use_32bit_indices", args.options.shape_use_32bit_indices);

        const zmesh_options = step.createModule();

        const zmesh = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/main.zig" },
            .dependencies = &.{
                .{ .name = "zmesh_options", .module = zmesh_options },
            },
        });

        const zmesh_c_cpp = b.addStaticLibrary(.{
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

    pub fn link(zmesh_pkg: Package, exe: *std.Build.CompileStep) void {
        exe.linkLibrary(zmesh_pkg.zmesh_c_cpp);
    }
};

pub fn build(_: *std.Build) void {}

pub fn buildTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/main.zig" },
        .target = target,
        .optimize = optimize,
    });
    return tests;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
