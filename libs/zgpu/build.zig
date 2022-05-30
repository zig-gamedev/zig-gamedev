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

    // We treat `zgpu` and `glfw` as a single package so we inject dependency to `glfw` here.
    exe.addPackage(glfw.pkg);

    glfw.link(exe.builder, exe, bos.options.glfw);
    gpu_dawn.link(exe.builder, exe, bos.options.dawn);

    if (bos.options.use_imgui) {
        exe.addIncludeDir(thisDir() ++ "/libs");
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_widgets.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_tables.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_draw.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_demo.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/cimgui.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_glfw.cpp", &.{""});
        exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_wgpu.cpp", &.{""});
    }

    if (bos.options.use_stb_image) {
        exe.addCSourceFile(thisDir() ++ "/libs/stb/stb_image.c", &.{"-std=c99"});
    }
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
    // We treat `zgpu` and `glfw` as a single package so we inject dependency to `glfw` here.
    static.deps[dependencies.len] = glfw.pkg;

    return .{
        .name = "zgpu",
        .source = .{ .path = thisDir() ++ "/src/zgpu.zig" },
        .dependencies = static.deps[0 .. dependencies.len + 1],
    };
}

fn thisDir() []const u8 {
    comptime {
        return std.fs.path.dirname(@src().file) orelse ".";
    }
}
