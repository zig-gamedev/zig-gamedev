const std = @import("std");

pub const Package = struct {
    common: *std.Build.Module,
    common_c_cpp: *std.Build.CompileStep,

    pub fn link(pkg: Package, exe: *std.Build.CompileStep) void {
        exe.addModule("common", pkg.common);
        exe.linkLibrary(pkg.common_c_cpp);
        exe.addIncludePath(thisDir() ++ "/libs/imgui");
        exe.addIncludePath(thisDir() ++ "/../zmesh/libs/cgltf");
    }
};

pub fn package(
    b: *std.Build,
    target: std.zig.CrossTarget,
    optimize: std.builtin.Mode,
    args: struct {
        deps: struct {
            zwin32: *std.Build.Module,
            zd3d12: *std.Build.Module,
        },
    },
) Package {
    const lib = b.addStaticLibrary(.{
        .name = "common",
        .target = target,
        .optimize = optimize,
    });

    lib.linkLibC();
    lib.linkLibCpp();
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

    return .{
        .common = b.createModule(.{
            .source_file = .{ .path = thisDir() ++ "/src/common.zig" },
            .dependencies = &.{
                .{ .name = "zwin32", .module = args.deps.zwin32 },
                .{ .name = "zd3d12", .module = args.deps.zd3d12 },
            },
        }),
        .common_c_cpp = lib,
    };
}

pub fn build(_: *std.Build) void {}

inline fn thisDir() []const u8 {
    return comptime std.fs.path.dirname(@src().file) orelse ".";
}
