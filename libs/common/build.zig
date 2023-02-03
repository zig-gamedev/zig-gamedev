const std = @import("std");

pub fn getPkg(dependencies: []const std.Build.Pkg) std.Build.Pkg {
    return .{
        .name = "common",
        .source = .{ .path = thisDir() ++ "/src/common.zig" },
        .dependencies = dependencies,
    };
}

pub fn build(b: *std.Build) void {
    _ = b;
}

pub fn link(exe: *std.Build.CompileStep) void {
    const lib = buildLibrary(exe);
    exe.linkLibrary(lib);
    //exe.addIncludePath(thisDir() ++ "/src/c");
    exe.addIncludePath(thisDir() ++ "/libs/imgui");
    exe.addIncludePath(thisDir() ++ "/../zmesh/libs/cgltf");
    //exe.addIncludePath(thisDir() ++ "/../zstbi/libs/stbi");
}

fn buildLibrary(exe: *std.Build.CompileStep) *std.Build.CompileStep {
    const lib = exe.builder.addStaticLibrary(.{
        .name = "common",
        .root_source_file = .{ .path = thisDir() ++ "/src/common.zig" },
        .target = exe.target,
        .optimize = exe.optimize,
    });

    //lib.addIncludePath(thisDir() ++ "/src/c");

    lib.linkSystemLibraryName("c");
    lib.linkSystemLibraryName("c++");
    lib.linkSystemLibraryName("imm32");

    lib.addIncludePath(thisDir() ++ "/libs");
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_widgets.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_tables.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_draw.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/imgui_demo.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/libs/imgui/cimgui.cpp", &.{""});

    lib.addIncludePath(thisDir() ++ "/../zmesh/libs/cgltf");
    lib.addCSourceFile(thisDir() ++ "/../zmesh/libs/cgltf/cgltf.c", &.{"-std=c99"});

    //lib.addIncludePath(thisDir() ++ "/../zstbi/libs/stbi");
    //lib.addCSourceFile(thisDir() ++ "/../zstbi/libs/stbi/stb_image.c", &.{"-std=c99"});

    return lib;
}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
