const std = @import("std");

pub const BuildOptions = struct {
    shape_use_32bit_indices: bool = false,
};

pub const BuildOptionsStep = struct {
    options: BuildOptions,
    step: *std.Build.OptionsStep,

    pub fn init(b: *std.Build, options: BuildOptions) BuildOptionsStep {
        const bos = .{
            .options = options,
            .step = b.addOptions(),
        };
        bos.step.addOption(bool, "shape_use_32bit_indices", bos.options.shape_use_32bit_indices);
        return bos;
    }

    pub fn getPkg(bos: BuildOptionsStep) std.Build.Pkg {
        return bos.step.getPackage("zmesh_options");
    }

    fn addTo(bos: BuildOptionsStep, target_step: *std.Build.CompileStep) void {
        target_step.addOptions("zmesh_options", bos.step);
    }
};

pub fn getPkg(dependencies: []const std.Build.Pkg) std.Build.Pkg {
    return .{
        .name = "zmesh",
        .source = .{ .path = thisDir() ++ "/src/main.zig" },
        .dependencies = dependencies,
    };
}

pub fn build(_: *std.Build) void {}

pub fn buildTests(
    b: *std.Build,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.Build.CompileStep {
    const tests = b.addTest(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/main.zig" },
        .target = target,
        .optimize = build_mode,
    });
    link(tests, BuildOptionsStep.init(b, .{}));
    return tests;
}

pub fn link(exe: *std.Build.CompileStep, bos: BuildOptionsStep) void {
    bos.addTo(exe);

    exe.linkSystemLibraryName("c");
    exe.linkSystemLibraryName("c++");

    const par_shapes_t = if (bos.options.shape_use_32bit_indices) "-DPAR_SHAPES_T=uint32_t" else "";

    exe.addIncludePath(thisDir() ++ "/libs/par_shapes");
    exe.addCSourceFile(
        thisDir() ++ "/libs/par_shapes/par_shapes.c",
        &.{ "-std=c99", "-fno-sanitize=undefined", par_shapes_t },
    );

    exe.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/clusterizer.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/indexgenerator.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/vcacheoptimizer.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/vcacheanalyzer.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/vfetchoptimizer.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/vfetchanalyzer.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/overdrawoptimizer.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/overdrawanalyzer.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/meshoptimizer/allocator.cpp", &.{""});

    exe.addIncludePath(thisDir() ++ "/libs/cgltf");
    exe.addCSourceFile(thisDir() ++ "/libs/cgltf/cgltf.c", &.{"-std=c99"});
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
