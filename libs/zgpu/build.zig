const std = @import("std");
const glfw = @import("libs/mach-glfw/build.zig");
const gpu_dawn = @import("libs/mach-gpu-dawn/build.zig");

pub const BuildOptions = struct {
    use_imgui: bool = true,
    use_stb_image: bool = true,

    glfw: glfw.Options = .{},
    dawn: gpu_dawn.Options = .{},
};

pub const BuildOptionsStep = struct {
    options: BuildOptions,
    step: *std.build.OptionsStep,

    pub fn init(b: *std.build.Builder, options: BuildOptions) BuildOptionsStep {
        const bos = .{
            .options = options,
            .step = b.addOptions(),
        };
        return bos;
    }

    pub fn getPkg(bos: BuildOptionsStep) std.build.Pkg {
        return bos.step.getPackage("zgpu_options");
    }

    fn addTo(bos: BuildOptionsStep, target_step: *std.build.LibExeObjStep) void {
        target_step.addOptions("zgpu_options", bos.step);
    }
};

pub fn link(exe: *std.build.LibExeObjStep, bos: BuildOptionsStep) void {
    bos.addTo(exe);

    // When user links with `zgpu` we automatically inject dependency to `glfw`.
    exe.addPackage(glfw.pkg);

    glfw.link(exe.builder, exe, bos.options.glfw);
    gpu_dawn.link(exe.builder, exe, bos.options.dawn);

    exe.addIncludeDir(thisDir() ++ "/src");
    exe.addCSourceFile(thisDir() ++ "/src/dawn.cpp", &.{"-std=c++17"});

    if (bos.options.use_imgui) {
        exe.addCSourceFile(thisDir() ++ "/src/zgui.cpp", &.{""});

        exe.addIncludeDir(thisDir() ++ "/libs");
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_widgets.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_tables.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_draw.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_demo.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_glfw.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_wgpu.cpp", &.{""});
    }

    if (bos.options.use_stb_image) {
        exe.addCSourceFile(thisDir() ++ "/libs/stb/stb_image.c", &.{"-std=c99"});
    }
}

pub fn buildTests(
    b: *std.build.Builder,
    build_mode: std.builtin.Mode,
    target: std.zig.CrossTarget,
) *std.build.LibExeObjStep {
    const tests = b.addTest(thisDir() ++ "/src/zgpu.zig");
    tests.setBuildMode(build_mode);
    tests.setTarget(target);
    link(tests, BuildOptionsStep.init(b, .{}));
    return tests;
}

pub fn getPkg(dependencies: []const std.build.Pkg) std.build.Pkg {
    const static = struct {
        var deps: [8]std.build.Pkg = undefined;
    };
    std.debug.assert(dependencies.len < static.deps.len);

    // Copy `dependencies` to a static memory.
    for (dependencies) |dep, i| {
        static.deps[i] = dep;
    }
    // When user links with `zgpu` we automatically inject dependency to `glfw`.
    static.deps[dependencies.len] = glfw.pkg;

    return .{
        .name = "zgpu",
        .source = .{ .path = thisDir() ++ "/src/zgpu.zig" },
        .dependencies = static.deps[0 .. dependencies.len + 1],
    };
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
