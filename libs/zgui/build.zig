const std = @import("std");

pub const pkg = std.build.Pkg{
    .name = "zgui",
    .source = .{ .path = thisDir() ++ "/src/main.zig" },
};

pub fn link(exe: *std.build.LibExeObjStep) void {
    exe.addIncludePath(thisDir() ++ "/libs");

    const flags = &.{"-fno-sanitize=undefined"};

    exe.addCSourceFile(thisDir() ++ "/src/zgui.cpp", flags);

    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui.cpp", flags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_widgets.cpp", flags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_tables.cpp", flags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_draw.cpp", flags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_demo.cpp", flags);

    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot_demo.cpp", flags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot.cpp", flags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/implot_items.cpp", flags);

    // This is needed for 'glfw/wgpu' rendering backend.
    // You may need to remove/change this if you use different backend.
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_glfw.cpp", flags);
    exe.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_impl_wgpu.cpp", flags);
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
