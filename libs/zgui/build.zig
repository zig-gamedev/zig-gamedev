const std = @import("std");

pub const Backend = enum {
    no_backend,
    glfw_wgpu,
    glfw_opengl3,
    win32_dx12,
};

pub const Options = struct {
    backend: Backend,
    shared: bool = false,
    /// use bundled imgui source
    with_imgui: bool = true,
    /// use bundled implot source
    with_implot: bool = true,
};

pub const Package = struct {
    options: Options,
    zgui: *std.Build.Module,
    zgui_options: *std.Build.Module,
    zgui_c_cpp: *std.Build.Step.Compile,
    
    pub fn link(pkg: Package, exe: *std.Build.Step.Compile) void {
        exe.linkLibrary(pkg.zgui_c_cpp);
        exe.root_module.addImport("zgui", pkg.zgui);
    }
};

pub fn package(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.Mode,
    args: struct {
        options: Options,
    },
) Package {
    const step = b.addOptions();
    step.addOption(Backend, "backend", args.options.backend);
    step.addOption(bool, "shared", args.options.shared);
    
    const zgui_options = step.createModule();
    
    const zgui = b.createModule(.{
        .root_source_file = .{ .path = thisDir() ++ "/src/gui.zig" },
                                .imports = &.{
                                    .{ .name = "zgui_options", .module = zgui_options },
                                },
    });
    
    const zgui_c_cpp = if (args.options.shared) blk: {
        const lib = b.addSharedLibrary(.{
            .name = "zgui",
            .target = target,
            .optimize = optimize,
        });
        
        b.installArtifact(lib);
        if (target.result.os.tag == .windows) {
            lib.defineCMacro("IMGUI_API", "__declspec(dllexport)");
            lib.defineCMacro("IMPLOT_API", "__declspec(dllexport)");
            lib.defineCMacro("ZGUI_API", "__declspec(dllexport)");
        }
        
        break :blk lib;
    } else b.addStaticLibrary(.{
        .name = "zgui",
        .target = target,
        .optimize = optimize,
    });
    
    zgui_c_cpp.addIncludePath(.{ .path = thisDir() ++ "/libs" });
    zgui_c_cpp.addIncludePath(.{ .path = thisDir() ++ "/libs/imgui" });
    
    const abi = (std.zig.system.resolveTargetQuery(target.query) catch unreachable).abi;
    zgui_c_cpp.linkLibC();
    if (abi != .msvc)
        zgui_c_cpp.linkLibCpp();
    
    const cflags = &.{"-fno-sanitize=undefined"};
    
    zgui_c_cpp.addCSourceFile(.{
        .file = .{ .path = thisDir() ++ "/src/zgui.cpp" },
                              .flags = cflags,
    });
    
    if (args.options.with_imgui) {
        zgui_c_cpp.addCSourceFiles(.{
            .files = &.{
                thisDir() ++ "/libs/imgui/imgui.cpp",
                                   thisDir() ++ "/libs/imgui/imgui_widgets.cpp",
                                   thisDir() ++ "/libs/imgui/imgui_tables.cpp",
                                   thisDir() ++ "/libs/imgui/imgui_draw.cpp",
                                   thisDir() ++ "/libs/imgui/imgui_demo.cpp",
            },
            .flags = cflags,
        });
    }
    
    if (args.options.with_implot) {
        zgui_c_cpp.addCSourceFiles(.{
            .files = &.{
                thisDir() ++ "/libs/imgui/implot_demo.cpp",
                                   thisDir() ++ "/libs/imgui/implot.cpp",
                                   thisDir() ++ "/libs/imgui/implot_items.cpp",
            },
            .flags = cflags,
        });
    }
    
    switch (args.options.backend) {
        .glfw_wgpu => {
            zgui_c_cpp.addIncludePath(.{ .path = thisDir() ++ "/../zglfw/libs/glfw/include" });
            zgui_c_cpp.addIncludePath(.{ .path = thisDir() ++ "/../zgpu/libs/dawn/include" });
            zgui_c_cpp.addCSourceFiles(.{
                .files = &.{
                    thisDir() ++ "/libs/imgui/backends/imgui_impl_glfw.cpp",
                                       thisDir() ++ "/libs/imgui/backends/imgui_impl_wgpu.cpp",
                },
                .flags = cflags,
            });
        },
        .glfw_opengl3 => {
            zgui_c_cpp.addIncludePath(.{ .path = thisDir() ++ "/../zglfw/libs/glfw/include" });
            zgui_c_cpp.addIncludePath(.{ .path = thisDir() ++ "/../zgpu/libs/dawn/include" });
            zgui_c_cpp.addCSourceFiles(.{
                .files = &.{
                    thisDir() ++ "/libs/imgui/backends/imgui_impl_glfw.cpp",
                                       thisDir() ++ "/libs/imgui/backends/imgui_impl_opengl3.cpp",
                },
                .flags = &(cflags.* ++ .{"-DIMGUI_IMPL_OPENGL_LOADER_CUSTOM"}),
            });
        },
        .win32_dx12 => {
            zgui_c_cpp.addCSourceFiles(.{
                .files = &.{
                    thisDir() ++ "/libs/imgui/backends/imgui_impl_win32.cpp",
                                       thisDir() ++ "/libs/imgui/backends/imgui_impl_dx12.cpp",
                },
                .flags = cflags,
            });
            zgui_c_cpp.root_module.linkSystemLibrary("d3dcompiler_47", .{});
            zgui_c_cpp.root_module.linkSystemLibrary("dwmapi", .{});
        },
        .no_backend => {},
    }
    
    return .{
        .options = args.options,
        .zgui = zgui,
        .zgui_options = zgui_options,
        .zgui_c_cpp = zgui_c_cpp,
    };
}

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
