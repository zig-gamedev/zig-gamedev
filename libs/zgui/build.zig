const std = @import("std");

pub const Package = struct {
    pub const Backend = enum {
        no_backend,
        glfw_wgpu,
        sdl2_opengl3,
        win32_dx12,
    };
    pub const Options = struct {
        backend: Backend,
        shared: bool = false,
    };

    options: Options,
    zgui: *std.Build.Module,
    zgui_options: *std.Build.Module,
    zgui_c_cpp: *std.Build.CompileStep,

    pub fn build(
        b: *std.Build,
        target: std.zig.CrossTarget,
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
            .source_file = .{ .path = thisDir() ++ "/src/main.zig" },
            .dependencies = &.{
                .{ .name = "zgui_options", .module = zgui_options },
            },
        });

        const zgui_c_cpp = if (args.options.shared) blk: {
            const lib = b.addSharedLibrary(.{
                .name = "zgui",
                .target = target,
                .optimize = optimize,
            });

            lib.install();
            if (target.isWindows()) {
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

        zgui_c_cpp.addIncludePath(thisDir() ++ "/libs");
        zgui_c_cpp.addIncludePath(thisDir() ++ "/libs/imgui");

        zgui_c_cpp.linkLibC();
        zgui_c_cpp.linkLibCpp();

        const cflags = &.{"-fno-sanitize=undefined"};

        zgui_c_cpp.addCSourceFile(thisDir() ++ "/src/zgui.cpp", cflags);

        zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/imgui.cpp", cflags);
        zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_widgets.cpp", cflags);
        zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_tables.cpp", cflags);
        zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_draw.cpp", cflags);
        zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_demo.cpp", cflags);

        zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/implot_demo.cpp", cflags);
        zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/implot.cpp", cflags);
        zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/implot_items.cpp", cflags);

        switch (args.options.backend) {
            .glfw_wgpu => {
                zgui_c_cpp.addIncludePath(thisDir() ++ "/../zglfw/libs/glfw/include");
                zgui_c_cpp.addIncludePath(thisDir() ++ "/../zgpu/libs/dawn/include");
                zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/backends/imgui_impl_glfw.cpp", cflags);
                zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/backends/imgui_impl_wgpu.cpp", cflags);
            },
            .sdl2_opengl3 => {
                const include_path_sdl2 = std.fs.path.join(b.allocator, &.{
                    thisDir(),
                    switch (target.getOsTag()) {
                        .windows => "../zsdl/libs/x86_64-windows-gnu/include/SDL2",
                        .linux => "../zsdl/libs/x86_64-linux-gnu/include/SDL2",
                        .macos => "../zsdl/libs/macos/Frameworks/SDL2.framework/Headers",
                        else => unreachable,
                    },
                }) catch unreachable;
                zgui_c_cpp.addIncludePath(include_path_sdl2);
                zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/backends/imgui_impl_sdl2.cpp", cflags);
                zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/backends/imgui_impl_opengl3.cpp", cflags);
            },
            .win32_dx12 => {
                zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/backends/imgui_impl_win32.cpp", cflags);
                zgui_c_cpp.addCSourceFile(thisDir() ++ "/libs/imgui/backends/imgui_impl_dx12.cpp", cflags);
                zgui_c_cpp.linkSystemLibraryName("d3dcompiler_47");
                zgui_c_cpp.linkSystemLibraryName("dwmapi");
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

    pub fn link(zgui_pkg: Package, exe: *std.Build.CompileStep) void {
        exe.linkLibrary(zgui_pkg.zgui_c_cpp);
    }
};

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
