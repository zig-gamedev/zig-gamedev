const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zgui",
    .source = .{ .path = thisDir() ++ "/src/main.zig" },
};

const cflags = &.{"-fno-sanitize=undefined"};

pub fn link(exe: *std.build.LibExeObjStep) void {
    linkNoBackend(exe);

    // This is needed for 'glfw/wgpu' rendering backend.
    // You may need to remove/change this if you use different backend.
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_glfw.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_wgpu.cpp", cflags);
}

pub fn linkNoBackend(exe: *std.build.LibExeObjStep) void {
    exe.addIncludePath(thisDir() ++ "/libs");

    exe.linkSystemLibraryName("c");
    exe.linkSystemLibraryName("c++");

    exe.addCSourceFile(thisDir() ++ "/src/zgui.cpp", cflags);

    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_widgets.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_tables.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_draw.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_demo.cpp", cflags);

    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot_demo.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot.cpp", cflags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot_items.cpp", cflags);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
