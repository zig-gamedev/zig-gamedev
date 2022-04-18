const std = @import("std");
const zwin32 = @import("../zwin32/build.zig");
const ztracy = @import("../ztracy/build.zig");
const zd3d12 = @import("../zd3d12/build.zig");

pub fn getPkg(b: *std.build.Builder, options_pkg: std.build.Pkg) std.build.Pkg {
    const pkg = std.build.Pkg{
        .name = "common",
        .path = .{ .path = thisDir() ++ "/src/common.zig" },
        .dependencies = &[_]std.build.Pkg{
            zwin32.pkg,
            ztracy.getPkg(b, options_pkg),
            zd3d12.getPkg(b, options_pkg),
            options_pkg,
        },
    };
    return b.dupePkg(pkg);
}

pub fn build(b: *std.build.Builder) void {
    _ = b;
}

pub fn link(exe: *std.build.LibExeObjStep) void {
    const lib = buildLibrary(exe);
    exe.linkLibrary(lib);
    exe.addIncludeDir(thisDir() ++ "/src/c");
    exe.addIncludeDir(thisDir() ++ "/../zgpu/libs/imgui");
}

fn buildLibrary(exe: *std.build.LibExeObjStep) *std.build.LibExeObjStep {
    const lib = exe.builder.addStaticLibrary("common", thisDir() ++ "/src/common.zig");

    lib.setBuildMode(exe.build_mode);
    lib.setTarget(exe.target);
    lib.want_lto = false;
    lib.addIncludeDir(thisDir() ++ "/src/c");
    lib.addIncludeDir(thisDir() ++ "/../zgpu/libs");

    lib.linkSystemLibrary("c");
    lib.linkSystemLibrary("c++");
    lib.linkSystemLibrary("imm32");

    lib.addCSourceFile(thisDir() ++ "/../zgpu/libs/imgui/imgui.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/../zgpu/libs/imgui/imgui_widgets.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/../zgpu/libs/imgui/imgui_tables.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/../zgpu/libs/imgui/imgui_draw.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/../zgpu/libs/imgui/imgui_demo.cpp", &.{""});
    lib.addCSourceFile(thisDir() ++ "/../zgpu/libs/imgui/cimgui.cpp", &.{""});

    lib.addCSourceFile(thisDir() ++ "/src/c/cgltf.c", &.{"-std=c99"});

    lib.addCSourceFile(thisDir() ++ "/src/c/stb_image.c", &.{"-std=c99"});

    return lib;
}

fn thisDir() []const u8 {
    return std.fs.path.dirname(@src().file) orelse ".";
}
