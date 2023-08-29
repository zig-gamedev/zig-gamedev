const builtin = @import("builtin");
const std = @import("std");

pub const Options = struct {
    optimize: std.builtin.Mode,
    target: std.zig.CrossTarget,

    zd3d12_enable_debug_layer: bool,
    zd3d12_enable_gbv: bool,

    zpix_enable: bool,
};

pub fn buildWithOptions(b: *std.Build, options: Options) void {
    packagesCrossPlatform(b, options);

    inline for (samples_cross_platform) |sample| {
        install(b, sample, options);
    }

    if (options.target.isWindows() and
        (builtin.target.os.tag == .windows or builtin.target.os.tag == .linux))
    {
        packagesWindowsLinux(b, options);

        inline for (samples_windows_linux) |sample| {
            install(b, sample, options);
        }

        if (builtin.target.os.tag == .windows) {
            packagesWindows(b, options);

            inline for (samples_windows) |sample| {
                install(b, sample, options);
            }
        }
    }
}

const samples_cross_platform = .{
    @import("minimal_glfw_gl/build.zig"),
    @import("minimal_sdl_gl/build.zig"),
    @import("triangle_wgpu/build.zig"),
    @import("procedural_mesh_wgpu/build.zig"),
    @import("textured_quad_wgpu/build.zig"),
    @import("physically_based_rendering_wgpu/build.zig"),
    @import("bullet_physics_test_wgpu/build.zig"),
    @import("audio_experiments_wgpu/build.zig"),
    @import("gui_test_wgpu/build.zig"),
    @import("minimal_zgpu_zgui/build.zig"),
    @import("instanced_pills_wgpu/build.zig"),
    @import("layers_wgpu/build.zig"),
    @import("gamepad_wgpu/build.zig"),
    @import("physics_test_wgpu/build.zig"),
    @import("monolith/build.zig"),
};

const samples_windows_linux = .{
    @import("minimal_d3d12/build.zig"),
    @import("textured_quad/build.zig"),
    @import("triangle/build.zig"),
    @import("mesh_shader_test/build.zig"),
    @import("rasterization/build.zig"),
    @import("bindless/build.zig"),
    @import("simple_raytracer/build.zig"),
};

const samples_windows = .{
    //@import("audio_playback_test/build.zig"),
    //@import("audio_experiments/build.zig"),
    @import("vector_graphics_test/build.zig"),
    //@import("directml_convolution_test/build.zig"),
};

fn install(b: *std.Build, sample: anytype, options: Options) void {
    const exe = sample.build(b, options);

    // TODO: Problems with LTO on Windows.
    exe.want_lto = false;
    if (exe.optimize == .ReleaseFast)
        exe.strip = true;

    const install_step = b.step(sample.name, "Build '" ++ sample.name ++ "' demo");
    install_step.dependOn(&b.addInstallArtifact(exe, .{}).step);

    const run_step = b.step(sample.name ++ "-run", "Run '" ++ sample.name ++ "' demo");
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(install_step);
    run_step.dependOn(&run_cmd.step);

    b.getInstallStep().dependOn(install_step);
}

const zsdl = @import("../libs/zsdl/build.zig");
const zopengl = @import("../libs/zopengl/build.zig");
const zmath = @import("../libs/zmath/build.zig");
const zglfw = @import("../libs/zglfw/build.zig");
const zpool = @import("../libs/zpool/build.zig");
const zjobs = @import("../libs/zjobs/build.zig");
const zmesh = @import("../libs/zmesh/build.zig");
const znoise = @import("../libs/znoise/build.zig");
const zstbi = @import("../libs/zstbi/build.zig");
const zwin32 = @import("../libs/zwin32/build.zig");
const zd3d12 = @import("../libs/zd3d12/build.zig");
const zxaudio2 = @import("../libs/zxaudio2/build.zig");
const zpix = @import("../libs/zpix/build.zig");
const common = @import("../libs/common/build.zig");
const zbullet = @import("../libs/zbullet/build.zig");
const zgui = @import("../libs/zgui/build.zig");
const zgpu = @import("../libs/zgpu/build.zig");
const ztracy = @import("../libs/ztracy/build.zig");
const zphysics = @import("../libs/zphysics/build.zig");
const zaudio = @import("../libs/zaudio/build.zig");
const zflecs = @import("../libs/zflecs/build.zig");

pub var zmath_pkg: zmath.Package = undefined;
pub var znoise_pkg: znoise.Package = undefined;
pub var zopengl_pkg: zopengl.Package = undefined;
pub var zsdl_pkg: zsdl.Package = undefined;
pub var zpool_pkg: zpool.Package = undefined;
pub var zmesh_pkg: zmesh.Package = undefined;
pub var zglfw_pkg: zglfw.Package = undefined;
pub var zstbi_pkg: zstbi.Package = undefined;
pub var zbullet_pkg: zbullet.Package = undefined;
pub var zgui_pkg: zgui.Package = undefined;
pub var zgpu_pkg: zgpu.Package = undefined;
pub var ztracy_pkg: ztracy.Package = undefined;
pub var zphysics_pkg: zphysics.Package = undefined;
pub var zaudio_pkg: zaudio.Package = undefined;
pub var zflecs_pkg: zflecs.Package = undefined;

pub var zwin32_pkg: zwin32.Package = undefined;
pub var zd3d12_pkg: zd3d12.Package = undefined;
pub var zpix_pkg: zpix.Package = undefined;
pub var zxaudio2_pkg: zxaudio2.Package = undefined;
pub var common_pkg: common.Package = undefined;
pub var common_d2d_pkg: common.Package = undefined;
pub var zd3d12_d2d_pkg: zd3d12.Package = undefined;

fn packagesWindowsLinux(b: *std.Build, options: Options) void {
    const target = options.target;
    const optimize = options.optimize;

    zwin32_pkg = zwin32.package(b, target, optimize, .{});
    zd3d12_pkg = zd3d12.package(b, target, optimize, .{
        .options = .{
            .enable_debug_layer = options.zd3d12_enable_debug_layer,
            .enable_gbv = options.zd3d12_enable_gbv,
            .upload_heap_capacity = 32 * 1024 * 1024,
        },
        .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
    });
    zpix_pkg = zpix.package(b, target, optimize, .{
        .options = .{ .enable = options.zpix_enable },
        .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
    });
    common_pkg = common.package(b, target, optimize, .{
        .deps = .{ .zwin32 = zwin32_pkg.zwin32, .zd3d12 = zd3d12_pkg.zd3d12 },
    });
}

fn packagesCrossPlatform(b: *std.Build, options: Options) void {
    const target = options.target;
    const optimize = options.optimize;

    zopengl_pkg = zopengl.package(b, target, optimize, .{});
    zmath_pkg = zmath.package(b, target, optimize, .{});
    zpool_pkg = zpool.package(b, target, optimize, .{});
    zglfw_pkg = zglfw.package(b, target, optimize, .{});
    zsdl_pkg = zsdl.package(b, target, optimize, .{});
    zmesh_pkg = zmesh.package(b, target, optimize, .{});
    znoise_pkg = znoise.package(b, target, optimize, .{});
    zstbi_pkg = zstbi.package(b, target, optimize, .{});
    zbullet_pkg = zbullet.package(b, target, optimize, .{});
    zgui_pkg = zgui.package(b, target, optimize, .{
        .options = .{ .backend = .glfw_wgpu },
    });
    zgpu_pkg = zgpu.package(b, target, optimize, .{
        .options = .{ .uniforms_buffer_size = 4 * 1024 * 1024 },
        .deps = .{ .zpool = zpool_pkg.zpool, .zglfw = zglfw_pkg.zglfw },
    });
    ztracy_pkg = ztracy.package(b, target, optimize, .{
        .options = .{
            .enable_ztracy = !target.isDarwin(), // TODO: ztracy fails to compile on macOS.
            .enable_fibers = !target.isDarwin(),
        },
    });
    zphysics_pkg = zphysics.package(b, target, optimize, .{});
    zaudio_pkg = zaudio.package(b, target, optimize, .{});
    zflecs_pkg = zflecs.package(b, target, optimize, .{});
}

fn packagesWindows(b: *std.Build, options: Options) void {
    const target = options.target;
    const optimize = options.optimize;

    zd3d12_d2d_pkg = zd3d12.package(b, target, optimize, .{
        .options = .{
            .enable_debug_layer = options.zd3d12_enable_debug_layer,
            .enable_gbv = options.zd3d12_enable_gbv,
            .enable_d2d = true,
        },
        .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
    });
    common_d2d_pkg = common.package(b, target, optimize, .{
        .deps = .{ .zwin32 = zwin32_pkg.zwin32, .zd3d12 = zd3d12_d2d_pkg.zd3d12 },
    });
    zxaudio2_pkg = zxaudio2.package(b, target, optimize, .{
        .options = .{ .enable_debug_layer = options.zd3d12_enable_debug_layer },
        .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
    });
}
