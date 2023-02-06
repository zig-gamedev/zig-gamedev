const std = @import("std");

pub const Backend = enum {
    no_backend,
    glfw_wgpu,
    win32_dx12,
};

pub const Options = struct {
    backend: Backend,
};

pub const Package = struct {
    module: *std.Build.Module,
    options: Options,
    options_module: *std.Build.Module,
};

pub fn package(
    b: *std.Build,
    args: struct {
        options: Options,
    },
) Package {
    const step = b.addOptions();
    step.addOption(Backend, "backend", args.options.backend);

    const options_module = step.createModule();

    const module = b.createModule(.{
        .source_file = .{ .path = thisDir() ++ "/src/main.zig" },
        .dependencies = &.{
            .{ .name = "zgui_options", .module = options_module },
        },
    });

    return .{
        .module = module,
        .options = args.options,
        .options_module = options_module,
    };
}

pub fn build(_: *std.Build) void {}

pub fn link(exe: *std.Build.CompileStep, options: Options) void {
    exe.addIncludePath(thisDir() ++ "/libs");
    exe.addIncludePath(thisDir() ++ "/libs/imgui");

    exe.linkSystemLibraryName("c");
    exe.linkSystemLibraryName("c++");

    const cflags = &.{"-fno-sanitize=undefined"};

    exe.addCSourceFile(thisDir() ++ "/src/zgui.cpp", cflags);

    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_widgets.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_tables.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_draw.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_demo.cpp", cflags);

    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot_demo.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot_items.cpp", cflags);

    switch (options.backend) {
        .glfw_wgpu => {
            exe.addCSourceFile(thisDir() ++ "/libs/imgui/backends/imgui_impl_glfw.cpp", cflags);
            exe.addCSourceFile(thisDir() ++ "/libs/imgui/backends/imgui_impl_wgpu.cpp", cflags);
        },
        .win32_dx12 => {
            exe.addCSourceFile(thisDir() ++ "/libs/imgui/backends/imgui_impl_win32.cpp", cflags);
            exe.addCSourceFile(thisDir() ++ "/libs/imgui/backends/imgui_impl_dx12.cpp", cflags);
            exe.linkSystemLibraryName("d3dcompiler_47");
            exe.linkSystemLibraryName("dwmapi");
        },
        .no_backend => {},
    }
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
