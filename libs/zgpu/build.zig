const std = @import("std");
const glfw = @import("../mach-glfw/build.zig");
const gpu_dawn = @import("../mach-gpu-dawn/build.zig");

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
        const bos = BuildOptionsStep{
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

fn buildLibrary(
    exe: *std.build.LibExeObjStep,
    bos: BuildOptionsStep,
) *std.build.LibExeObjStep {
    const lib = exe.builder.addStaticLibrary("zgpu", thisDir() ++ "/src/zgpu.zig");

    lib.setBuildMode(exe.build_mode);
    lib.setTarget(exe.target);

    glfw.link(exe.builder, lib, bos.options.glfw);
    gpu_dawn.link(exe.builder, lib, bos.options.dawn);

    if (bos.options.use_imgui) {
        lib.addIncludeDir(thisDir() ++ "/libs");
        lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui.cpp", &.{""});
        lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_widgets.cpp", &.{""});
        lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_tables.cpp", &.{""});
        lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_draw.cpp", &.{""});
        lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_demo.cpp", &.{""});
        lib.addCSourceFile(thisDir() ++ "/libs/imgui/cimgui.cpp", &.{""});
        lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_glfw.cpp", &.{""});
        lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_wgpu.cpp", &.{""});
    }

    if (bos.options.use_stb_image) {
        lib.addCSourceFile(thisDir() ++ "/libs/stb/stb_image.c", &.{"-std=c99"});
    }

    return lib;
}

pub fn link(exe: *std.build.LibExeObjStep, bos: BuildOptionsStep) void {
    bos.addTo(exe);

    glfw.link(exe.builder, exe, bos.options.glfw);
    gpu_dawn.link(exe.builder, exe, bos.options.dawn);

    const lib = buildLibrary(exe, bos);
    exe.linkLibrary(lib);

    if (bos.options.use_imgui) {
        exe.addIncludeDir(thisDir() ++ "/libs");
    }
}

pub fn getPkg(dependencies: []const std.build.Pkg) std.build.Pkg {
    return .{
        .name = "zgpu",
        .source = .{ .path = thisDir() ++ "/src/zgpu.zig" },
        .dependencies = dependencies,
    };
}

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
