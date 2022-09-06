const std = @import("std");

pub fn getPkg(dependencies: []const std.build.Pkg) std.build.Pkg {
    return .{
        .name = "zgui",
        .source = .{ .path = thisDir() ++ "/src/main.zig" },
        .dependencies = dependencies,
    };
}

pub fn link(exe: *std.build.LibExeObjStep) void {
    exe.addCSourceFile(thisDir() ++ "/src/zgui.cpp", &.{""});
    exe.addIncludeDir(thisDir() ++ "/libs");

    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_widgets.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_tables.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_draw.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_demo.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_glfw.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_wgpu.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot_demo.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot.cpp", &.{""});
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot_items.cpp", &.{""});
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
