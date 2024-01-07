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
        exe.root_module.linkLibrary(pkg.zgui_c_cpp);
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

    const zgui = b.addModule("zgui", .{
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

        if (target.result.os.tag == .macos) {
            lib.linker_allow_shlib_undefined = true;
        }

        break :blk lib;
    } else b.addStaticLibrary(.{
        .name = "zgui",
        .target = target,
        .optimize = optimize,
    });

    zgui_c_cpp.root_module.addIncludePath(.{ .path = thisDir() ++ "/libs" });
    zgui_c_cpp.root_module.addIncludePath(.{ .path = thisDir() ++ "/libs/imgui" });

    zgui_c_cpp.root_module.link_libc = true;
    if (target.result.abi != .msvc)
        zgui_c_cpp.root_module.link_libcpp = true;

    const cflags = &.{"-fno-sanitize=undefined"};

    zgui_c_cpp.root_module.addCSourceFile(.{
        .file = .{ .path = thisDir() ++ "/src/zgui.cpp" },
        .flags = cflags,
    });

    if (args.options.with_imgui) {
        zgui_c_cpp.root_module.addCSourceFiles(.{
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
        zgui_c_cpp.root_module.addCSourceFiles(.{
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
            zgui_c_cpp.root_module.addIncludePath(.{
                .path = thisDir() ++ "/../zglfw/libs/glfw/include",
            });
            zgui_c_cpp.root_module.addIncludePath(.{
                .path = thisDir() ++ "/../zgpu/libs/dawn/include",
            });
            zgui_c_cpp.root_module.addCSourceFiles(.{
                .files = &.{
                    thisDir() ++ "/libs/imgui/backends/imgui_impl_glfw.cpp",
                    thisDir() ++ "/libs/imgui/backends/imgui_impl_wgpu.cpp",
                },
                .flags = cflags,
            });
        },
        .glfw_opengl3 => {
            zgui_c_cpp.root_module.addIncludePath(.{
                .path = thisDir() ++ "/../zglfw/libs/glfw/include",
            });
            zgui_c_cpp.root_module.addIncludePath(.{
                .path = thisDir() ++ "/../zgpu/libs/dawn/include",
            });
            zgui_c_cpp.root_module.addCSourceFiles(.{
                .files = &.{
                    thisDir() ++ "/libs/imgui/backends/imgui_impl_glfw.cpp",
                    thisDir() ++ "/libs/imgui/backends/imgui_impl_opengl3.cpp",
                },
                .flags = &(cflags.* ++ .{"-DIMGUI_IMPL_OPENGL_LOADER_CUSTOM"}),
            });
        },
        .win32_dx12 => {
            zgui_c_cpp.root_module.addCSourceFiles(.{
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

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const test_step = b.step("test", "Run zgui tests");
    test_step.dependOn(runTests(b, optimize, target));

    _ = package(b, target, optimize, .{
        .options = .{
            .backend = b.option(Backend, "backend", "Select a backend") orelse .no_backend,
            .shared = b.option(bool, "shared", "Bulid as a shared library") orelse false,
            .with_imgui = b.option(bool, "with_imgui", "Build with bundled imgui source") orelse true,
            .with_implot = b.option(bool, "with_implot", "Build with bundled implot source") orelse false,
        },
    });
}

pub fn runTests(
    b: *std.Build,
    optimize: std.builtin.Mode,
    target: std.Build.ResolvedTarget,
) *std.Build.Step {
    const gui_tests = b.addTest(.{
        .name = "gui-tests",
        .root_source_file = .{ .path = thisDir() ++ "/src/gui.zig" },
        .target = target,
        .optimize = optimize,
    });

    const zgui_pkg = package(b, target, optimize, .{
        .options = .{ .backend = .no_backend },
    });
    gui_tests.root_module.addImport("zgui_options", zgui_pkg.zgui_options);
    zgui_pkg.link(gui_tests);

    return &b.addRunArtifact(gui_tests).step;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
