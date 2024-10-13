const std = @import("std");

const demo_name = "sdl2_demo";
const content_dir = demo_name ++ "_content/";

pub fn build(b: *std.Build, options: anytype) *std.Build.Step.Compile {
    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });

    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = b.path(b.pathJoin(&.{ src_path, "main.zig" })),
        .target = options.target,
        .optimize = options.optimize,
    });
    exe.linkLibC();

    const exe_options = b.addOptions();
    exe.root_module.addOptions("build_options", exe_options);
    exe_options.addOption([]const u8, "content_dir", content_dir);

    const zsdl = b.dependency("zsdl", .{});
    exe.root_module.addImport("zsdl2", zsdl.module("zsdl2"));
    exe.root_module.addImport("zsdl2_image", zsdl.module("zsdl2_image"));

    @import("zsdl").prebuilt.addLibraryPathsTo(exe);

    if (@import("zsdl").prebuilt.install_SDL2(b, options.target.result, .bin)) |install_lib_step| {
        exe.step.dependOn(install_lib_step);
    }

    if (@import("zsdl").prebuilt.install_SDL2_image(b, options.target.result, .bin)) |install_lib_step| {
        exe.step.dependOn(install_lib_step);
    }

    @import("zsdl").link_SDL2(exe);
    @import("zsdl").link_SDL2_image(exe);

    const install_content_step = b.addInstallDirectory(.{
        .source_dir = b.path(b.pathJoin(&.{ cwd_path, content_dir })),
        .install_dir = .bin,
        .install_subdir = content_dir,
    });
    exe.step.dependOn(&install_content_step.step);

    return exe;
}

pub fn buildWeb(b: *std.Build, options: anytype) *std.Build.Step {
    const zemscripten = @import("zemscripten");

    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });

    const wasm = b.addStaticLibrary(.{
        .name = demo_name,
        .root_source_file = b.path(b.pathJoin(&.{ src_path, "main-web.zig" })),
        .target = options.target,
        .optimize = options.optimize,
    });

    const zsdl = b.dependency("zsdl", .{});
    wasm.root_module.addImport("zsdl2", zsdl.module("zsdl2"));
    wasm.root_module.addImport("zsdl2_image", zsdl.module("zsdl2_image"));

    wasm.root_module.addImport("zemscripten", b.dependency("zemscripten", .{}).module("root"));

    const emcc_flags = zemscripten.emccDefaultFlags(b.allocator, options.optimize);

    var emcc_settings = zemscripten.emccDefaultSettings(b.allocator, .{
        .optimize = options.optimize,
    });
    emcc_settings.put("ALLOW_MEMORY_GROWTH", "1") catch unreachable;
    emcc_settings.put("USE_SDL", "2") catch unreachable;
    emcc_settings.put("USE_SDL_IMAGE", "2") catch unreachable;
    emcc_settings.put("SDL2_IMAGE_FORMATS", "[\"png\"]") catch unreachable;

    return zemscripten.emccStep(
        b,
        wasm,
        .{
            .optimize = options.optimize,
            .flags = emcc_flags,
            .settings = emcc_settings,
            .embed_paths = &.{
                .{
                    .src_path = "samples/sdl2_demo/sdl2_demo_content/zero.png",
                    .virtual_path = "sdl2_demo_content/zero.png",
                },
            },
            .install_dir = .{ .custom = "web" },
        },
    );
}
