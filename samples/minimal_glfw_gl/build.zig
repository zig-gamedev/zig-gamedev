const std = @import("std");

pub const demo_name = "minimal_glfw_gl";

pub fn build(b: *std.Build, options: anytype) *std.Build.Step.Compile {
    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });
    const exe = b.addExecutable(.{
        .name = demo_name,
        .root_source_file = b.path(b.pathJoin(&.{ src_path, "main.zig" })),
        .target = options.target,
        .optimize = options.optimize,
    });

    const zglfw = b.dependency("zglfw", .{
        .target = options.target,
    });
    exe.root_module.addImport("zglfw", zglfw.module("root"));
    exe.linkLibrary(zglfw.artifact("glfw"));

    const zopengl = b.dependency("zopengl", .{});
    exe.root_module.addImport("zopengl", zopengl.module("root"));

    if (options.target.result.os.tag == .macos) {
        if (b.lazyDependency("system_sdk", .{})) |system_sdk| {
            exe.addLibraryPath(system_sdk.path("macos12/usr/lib"));
            exe.addSystemFrameworkPath(system_sdk.path("macos12/System/Library/Frameworks"));
        }
    } else if (options.target.result.os.tag == .linux) {
        if (b.lazyDependency("system_sdk", .{})) |system_sdk| {
            exe.addLibraryPath(system_sdk.path("linux/lib/x86_64-linux-gnu"));
        }
    }

    return exe;
}

pub fn buildWeb(b: *std.Build, options: anytype) *std.Build.Step {
    const cwd_path = b.pathJoin(&.{ "samples", demo_name });
    const src_path = b.pathJoin(&.{ cwd_path, "src" });

    const zemscripten = @import("zemscripten");

    const zglfw = b.dependency("zglfw", .{
        .target = options.target,
    });
    const zopengl = b.dependency("zopengl", .{
        .target = options.target,
    });

    const wasm = b.addStaticLibrary(.{
        .name = demo_name,
        .root_source_file = b.path(b.pathJoin(&.{ src_path, "main-web.zig" })),
        .target = options.target,
        .optimize = options.optimize,
    });

    wasm.root_module.addImport("zglfw", zglfw.module("root"));

    wasm.root_module.addImport("zopengl", zopengl.module("root"));

    wasm.root_module.addImport("zemscripten", b.dependency("zemscripten", .{}).module("root"));

    const emcc_flags = zemscripten.emccDefaultFlags(b.allocator, .{
        .optimize = options.optimize,
        .fsanitize = true,
    });

    var emcc_settings = zemscripten.emccDefaultSettings(b.allocator, .{
        .optimize = options.optimize,
    });
    emcc_settings.put("ALLOW_MEMORY_GROWTH", "1") catch unreachable;
    emcc_settings.put("USE_GLFW", "3") catch unreachable;
    emcc_settings.put("MIN_WEBGL_VERSION", "2") catch unreachable;
    emcc_settings.put("MAX_WEBGL_VERSION", "2") catch unreachable;
    emcc_settings.put("FULL_ES2", "1") catch unreachable;

    return zemscripten.emccStep(
        b,
        wasm,
        .{
            .optimize = options.optimize,
            .flags = emcc_flags,
            .settings = emcc_settings,
            .install_dir = .{ .custom = "web" },
        },
    );
}
