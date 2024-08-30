const std = @import("std");

pub fn link(compile_step: *std.Build.Step.Compile, deps: struct {
    zwindows: *std.Build.Module,
    zd3d12: *std.Build.Module,
}) void {
    const b = compile_step.step.owner;
    const target = compile_step.root_module.resolved_target.?;
    const optimize = compile_step.root_module.optimize.?;

    const lib = b.addStaticLibrary(.{
        .name = "common",
        .target = target,
        .optimize = optimize,
    });

    lib.linkLibC();
    if (target.result.abi != .msvc)
        lib.linkLibCpp();
    lib.linkSystemLibrary("imm32");

    lib.addIncludePath(b.path("libs"));
    lib.addCSourceFile(
        .{ .file = b.path("samples/common/libs/imgui/imgui.cpp"), .flags = &.{""} },
    );
    lib.addCSourceFile(
        .{ .file = b.path("samples/common/libs/imgui/imgui_widgets.cpp"), .flags = &.{""} },
    );
    lib.addCSourceFile(
        .{ .file = b.path("samples/common/libs/imgui/imgui_tables.cpp"), .flags = &.{""} },
    );
    lib.addCSourceFile(
        .{ .file = b.path("samples/common/libs/imgui/imgui_draw.cpp"), .flags = &.{""} },
    );
    lib.addCSourceFile(
        .{ .file = b.path("samples/common/libs/imgui/imgui_demo.cpp"), .flags = &.{""} },
    );
    lib.addCSourceFile(
        .{ .file = b.path("samples/common/libs/imgui/cimgui.cpp"), .flags = &.{""} },
    );

    lib.addIncludePath(b.path("libs/zmesh/libs/cgltf"));
    lib.addCSourceFile(.{
        .file = b.path("libs/zmesh/libs/cgltf/cgltf.c"),
        .flags = &.{"-std=c99"},
    });

    lib.addIncludePath(b.path("samples/common/libs"));
    lib.addIncludePath(b.path("libs/zmesh/libs/cgltf"));

    const module = b.createModule(.{
        .root_source_file = b.path("samples/common/src/common.zig"),
        .imports = &.{
            .{ .name = "zwindows", .module = deps.zwindows },
            .{ .name = "zd3d12", .module = deps.zd3d12 },
        },
    });
    module.addIncludePath(b.path("samples/common/libs/imgui"));
    module.addIncludePath(b.path("libs/zmesh/libs/cgltf"));

    compile_step.root_module.addImport("common", module);

    compile_step.linkLibrary(lib);
}
