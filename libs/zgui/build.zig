const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zgui",
    .source = .{ .path = thisDir() ++ "/src/main.zig" },
};

pub fn link(exe: *std.build.LibExeObjStep) void {
    exe.addIncludePath(thisDir() ++ "/libs");
    exe.addCSourceFile(thisDir() ++ "/src/zgui.cpp", &.{""});

    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_widgets.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_tables.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_draw.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_demo.cpp", &.{""});

    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot_demo.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot_items.cpp", &.{""});

    // This is needed for 'glfw_wgpu' rendering backend.
    // You may need to remove/change this is you different backend.
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_glfw.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_wgpu.cpp", &.{""});
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
