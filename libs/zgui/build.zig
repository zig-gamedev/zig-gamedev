const std = @import("std");

pub const Backend = enum {
    no_backend,
    glfw_wgpu,
    glfw_opengl3,
    glfw_dx12,
    win32_dx12,
};

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{});
    const target = b.standardTargetOptions(.{});

    const options = .{
        .backend = b.option(Backend, "backend", "Backend to build (default: no_backend)") orelse .no_backend,
        .shared = b.option(
            bool,
            "shared",
            "Bulid as a shared library",
        ) orelse false,
        .with_implot = b.option(
            bool,
            "with_implot",
            "Build with bundled implot source",
        ) orelse true,
    };

    const options_step = b.addOptions();
    inline for (std.meta.fields(@TypeOf(options))) |field| {
        options_step.addOption(field.type, field.name, @field(options, field.name));
    }

    const options_module = options_step.createModule();

    _ = b.addModule("root", .{
        .root_source_file = .{ .path = "src/gui.zig" },
        .imports = &.{
            .{ .name = "zgui_options", .module = options_module },
        },
    });

    const cflags = &.{"-fno-sanitize=undefined"};

    const imgui = if (options.shared) blk: {
        const lib = b.addSharedLibrary(.{
            .name = "imgui",
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
        .name = "imgui",
        .target = target,
        .optimize = optimize,
    });

    b.installArtifact(imgui);

    imgui.addIncludePath(.{ .path = "libs" });
    imgui.addIncludePath(.{ .path = "libs/imgui" });

    imgui.linkLibC();
    if (target.result.abi != .msvc)
        imgui.linkLibCpp();

    imgui.addCSourceFile(.{
        .file = .{ .path = "src/zgui.cpp" },
        .flags = cflags,
    });

    imgui.addCSourceFiles(.{
        .files = &.{
            "libs/imgui/imgui.cpp",
            "libs/imgui/imgui_widgets.cpp",
            "libs/imgui/imgui_tables.cpp",
            "libs/imgui/imgui_draw.cpp",
            "libs/imgui/imgui_demo.cpp",
        },
        .flags = cflags,
    });

    if (options.with_implot) {
        imgui.defineCMacro("ZGUI_IMPLOT", "1");
        imgui.addCSourceFiles(.{
            .files = &.{
                "libs/imgui/implot_demo.cpp",
                "libs/imgui/implot.cpp",
                "libs/imgui/implot_items.cpp",
            },
            .flags = cflags,
        });
    } else {
        imgui.defineCMacro("ZGUI_IMPLOT", "0");
    }

    switch (options.backend) {
        .glfw_wgpu => {
            const zglfw = b.dependency("zglfw", .{});
            const zgpu = b.dependency("zgpu", .{});
            imgui.addIncludePath(.{ .path = zglfw.path("libs/glfw/include").getPath(b) });
            imgui.addIncludePath(.{ .path = zgpu.path("libs/dawn/include").getPath(b) });
            imgui.addCSourceFiles(.{
                .files = &.{
                    "libs/imgui/backends/imgui_impl_glfw.cpp",
                    "libs/imgui/backends/imgui_impl_wgpu.cpp",
                },
                .flags = cflags,
            });
        },
        .glfw_opengl3 => {
            const zglfw = b.dependency("zglfw", .{});
            imgui.addIncludePath(.{ .path = zglfw.path("libs/glfw/include").getPath(b) });
            imgui.addCSourceFiles(.{
                .files = &.{
                    "libs/imgui/backends/imgui_impl_glfw.cpp",
                    "libs/imgui/backends/imgui_impl_opengl3.cpp",
                },
                .flags = &(cflags.* ++ .{"-DIMGUI_IMPL_OPENGL_LOADER_CUSTOM"}),
            });
        },
        .glfw_dx12 => {
            const zglfw = b.dependency("zglfw", .{});
            imgui.addIncludePath(.{ .path = zglfw.path("libs/glfw/include").getPath(b) });
            imgui.addCSourceFiles(.{
                .files = &.{
                    "libs/imgui/backends/imgui_impl_glfw.cpp",
                    "libs/imgui/backends/imgui_impl_dx12.cpp",
                },
                .flags = cflags,
            });
            imgui.linkSystemLibrary("d3dcompiler_47");
        },
        .win32_dx12 => {
            imgui.addCSourceFiles(.{
                .files = &.{
                    "libs/imgui/backends/imgui_impl_win32.cpp",
                    "libs/imgui/backends/imgui_impl_dx12.cpp",
                },
                .flags = cflags,
            });
            imgui.linkSystemLibrary("d3dcompiler_47");
            imgui.linkSystemLibrary("dwmapi");
        },
        .no_backend => {},
    }

    const test_step = b.step("test", "Run zgui tests");

    const tests = b.addTest(.{
        .name = "zgui-tests",
        .root_source_file = .{ .path = "src/gui.zig" },
        .target = target,
        .optimize = optimize,
    });
    b.installArtifact(tests);

    tests.root_module.addImport("zgui_options", options_module);
    tests.linkLibrary(imgui);

    test_step.dependOn(&b.addRunArtifact(tests).step);
}
