const std = @import("std");

pub fn link(compile_step: *std.Build.Step.Compile, deps: struct {
    zwindows: *std.Build.Module,
    zd3d12: *std.Build.Module,
}) void {
    const b = compile_step.step.owner;
    const target = compile_step.root_module.resolved_target.?;
    const optimize = compile_step.root_module.optimize.?;

    const lib = b.addLibrary(.{
        .name = "common",
        .root_module = b.createModule(.{
            .target = target,
            .optimize = optimize,
            .link_libc = true,
            .link_libcpp = if (target.result.abi != .msvc) true else null,
        }),
    });

    lib.root_module.linkSystemLibrary("imm32", .{});

    lib.root_module.addIncludePath(b.path("libs"));
    lib.root_module.addCSourceFile(
        .{ .file = b.path("samples/common/libs/imgui/imgui.cpp"), .flags = &.{""} },
    );
    lib.root_module.addCSourceFile(
        .{ .file = b.path("samples/common/libs/imgui/imgui_widgets.cpp"), .flags = &.{""} },
    );
    lib.root_module.addCSourceFile(
        .{ .file = b.path("samples/common/libs/imgui/imgui_tables.cpp"), .flags = &.{""} },
    );
    lib.root_module.addCSourceFile(
        .{ .file = b.path("samples/common/libs/imgui/imgui_draw.cpp"), .flags = &.{""} },
    );
    lib.root_module.addCSourceFile(
        .{ .file = b.path("samples/common/libs/imgui/imgui_demo.cpp"), .flags = &.{""} },
    );
    lib.root_module.addCSourceFile(
        .{ .file = b.path("samples/common/libs/imgui/cimgui.cpp"), .flags = &.{""} },
    );

    const zmesh = b.dependency("zmesh", .{});

    lib.root_module.addIncludePath(zmesh.path("libs/cgltf"));
    lib.root_module.addCSourceFile(.{
        .file = zmesh.path("libs/cgltf/cgltf.c"),
        .flags = &.{"-std=c99"},
    });

    lib.root_module.addIncludePath(b.path("samples/common/libs"));
    lib.root_module.addIncludePath(zmesh.path("libs/cgltf"));

    const module = b.createModule(.{
        .root_source_file = b.path("samples/common/src/common.zig"),
        .imports = &.{
            .{ .name = "zwindows", .module = deps.zwindows },
            .{ .name = "zd3d12", .module = deps.zd3d12 },
        },
    });
    module.addIncludePath(b.path("samples/common/libs/imgui"));
    module.addIncludePath(zmesh.path("libs/cgltf"));

    compile_step.root_module.addImport("common", module);

    compile_step.root_module.linkLibrary(lib);
}
