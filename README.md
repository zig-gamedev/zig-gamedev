[Libraries](#libraries) - [Sample applications](#sample-applications-native-wgpu) - [Vision](#vision) - [Others using zig-gamedev](#others-using-zig-gamedev) - [Progress Reports](https://github.com/zig-gamedev-z/zig-gamedev/wiki/Progress-Reports) - [Roadmap](https://github.com/zig-gamedev/zig-gamedev/wiki/Roadmap)

# zig-gamedev project

We build game development ecosystem for [Zig programming language](https://ziglang.org/), every day since July 2021. Please consider [supporting the project](https://github.com/sponsors/hazeycode). We create:

* Cross-platform and composable [libraries](#libraries)
* Cross-platform [sample applications](#sample-applications-native-wgpu)
* DirectX 12 [sample applications](#sample-applications-directx-12)

Please note that Zig is still in development. Our [main](https://github.com/zig-gamedev/zig-gamedev/tree/main) branch tracks a periodically nominated version of the Zig compiler, this is **0.12.0-dev.2139+e025ad7b4** currently, which can be downloaded using the links below.

| OS/Arch         | Download link               |
| --------------- | --------------------------- |
| Windows x86_64  | [zig-windows-x86_64-0.12.0-dev.2139+e025ad7b4.zip](https://ziglang.org/builds/zig-windows-x86_64-0.12.0-dev.2139+e025ad7b4.zip) |
| Linux x86_64    | [zig-linux-x86_64-0.12.0-dev.2139+e025ad7b4.tar.xz](https://ziglang.org/builds/zig-linux-x86_64-0.12.0-dev.2139+e025ad7b4.tar.xz) |
| macOS x86_64    | [zig-macos-x86_64-0.12.0-dev.2139+e025ad7b4.tar.xz](https://ziglang.org/builds/zig-macos-x86_64-0.12.0-dev.2139+e025ad7b4.tar.xz) |
| macOS aarch64   | [zig-macos-aarch64-0.12.0-dev.2139+e025ad7b4.tar.xz](https://ziglang.org/builds/zig-macos-aarch64-0.12.0-dev.2139+e025ad7b4.tar.xz) |

If you need to use a more recent version of Zig, you may want to use our [unstable](https://github.com/zig-gamedev/zig-gamedev/tree/unstable) branch. But this is not generally recommended.

To get started on Windows/Linux/macOS try out [physically based rendering (wgpu)](https://github.com/zig-gamedev/zig-gamedev/tree/main/samples/physically_based_rendering_wgpu) sample:
```sh
git clone https://github.com/zig-gamedev/zig-gamedev.git
cd zig-gamedev
zig build physically_based_rendering_wgpu-run
```
## Quick start (D3D12)

To use zig-gamedev in your project copy or download zig-gamedev as a submodule, for example:

```sh
git submodule add https://github.com/zig-gamedev/zig-gamedev.git libs/zig-gamedev
```

Currently, we have minimal low-level API which allows you to build the lib once (`package()`) and link it with many executables (`link()`).

Include necessary libraries in `build.zig` like:

```zig
// Fetch the library
const zwin32 = @import("src/deps/zig-gamedev/libs/zwin32/build.zig");

// Build it
const zwin32_pkg = zwin32.package(b, target, optimize, .{});

// Link with your app
zwin32_pkg.link(exe, .{ .d3d12 = true });
```

<details>
<summary>Example build script:</summary>

```zig
const std = @import("std");
const zwin32 = @import("libs/zig-gamedev/libs/zwin32/build.zig");
const common = @import("libs/zig-gamedev/libs/common/build.zig");
const zd3d12 = @import("libs/zig-gamedev/libs/zd3d12/build.zig");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});

    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "example",
        .root_source_file = .{ .path = "src/main.zig" },
        .target = target,
        .optimize = optimize,
    });

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);

    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const zwin32_pkg = zwin32.package(b, target, optimize, .{});
    const zd3d12_pkg = zd3d12.package(b, target, optimize, .{
        .options = .{
            .enable_debug_layer = false,
            .enable_gbv = false,
            .enable_d2d = true,
        },
        .deps = .{ .zwin32 = zwin32_pkg.zwin32 },
    });
    const common_d2d_pkg = common.package(b, target, optimize, .{
        .deps = .{ .zwin32 = zwin32_pkg.zwin32, .zd3d12 = zd3d12_pkg.zd3d12 },
    });

    zwin32_pkg.link(exe, .{ .d3d12 = true });
    zd3d12_pkg.link(exe);
    common_d2d_pkg.link(exe);

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
```
</details>

## Libraries
| Library                       | Latest version | Description                                                                                                                |
|-------------------------------|----------------|----------------------------------------------------------------------------------------------------------------------------|
| **[zphysics](libs/zphysics)** | 0.0.6          | Zig API and C API for [Jolt Physics](https://github.com/jrouwe/JoltPhysics)                                                |
| **[zflecs](libs/zflecs)**     | 0.0.1          | Zig bindings for [flecs](https://github.com/SanderMertens/flecs) ECS                                                       |
| **[zopengl](libs/zopengl)**   | 0.1.3          | OpenGL loader (supports 4.0 Core Profile and ES 2.0 Profile)                                                               |
| **[zsdl](libs/zsdl)**         | 0.0.1          | Bindings for SDL2 (wip)                                                                                                    |
| **[zgpu](libs/zgpu)**         | 0.9.1          | Small helper library built on top of native wgpu implementation ([Dawn](https://github.com/michal-z/dawn-bin))             |
| **[zgui](libs/zgui)**         | 1.89.6         | Easy to use [dear imgui](https://github.com/ocornut/imgui) bindings (includes [ImPlot](https://github.com/epezent/implot)) |
| **[zaudio](libs/zaudio)**     | 0.9.3          | Fully-featured audio library built on top of [miniaudio](https://github.com/mackron/miniaudio)                             |
| **[zmath](libs/zmath)**       | 0.9.6          | SIMD math library for game developers                                                                                      |
| **[zstbi](libs/zstbi)**       | 0.9.3          | Image reading, writing and resizing with [stb](https://github.com/nothings/stb) libraries                                  |
| **[zmesh](libs/zmesh)**       | 0.9.0          | Loading, generating, processing and optimizing triangle meshes                                                             |
| **[ztracy](libs/ztracy)**     | 0.10.0         | Support for CPU profiling with [Tracy](https://github.com/wolfpld/tracy)                                                   |
| **[zpool](libs/zpool)**       | 0.9.0          | Generic pool & handle implementation                                                                                       |
| **[zglfw](libs/zglfw)**       | 0.7.0          | Minimalistic [GLFW](https://github.com/glfw/glfw) bindings with no translate-c dependency                                  |
| **[znoise](libs/znoise)**     | 0.1.0          | Zig bindings for [FastNoiseLite](https://github.com/Auburn/FastNoiseLite)                                                  |
| **[zjobs](libs/zjobs)**       | 0.1.0          | Generic job queue implementation                                                                                           |
| **[zbullet](libs/zbullet)**   | 0.2.0          | Zig bindings and C API for [Bullet physics library](https://github.com/bulletphysics/bullet3)                              |
| **[zwin32](libs/zwin32)**     | 0.9.0          | Zig bindings for Win32 API (d3d12, d3d11, xaudio2, directml, wasapi and more)                                              |
| **[zd3d12](libs/zd3d12)**     | 0.9.0          | Helper library for DirectX 12                                                                                              |
| **[zxaudio2](libs/zxaudio2)** | 0.9.0          | Helper library for XAudio2                                                                                                 |
| **[zpix](libs/zpix)**         | 0.9.0          | Support for GPU profiling with PIX for Windows                                                                             |

## Vision
* Very modular "toolbox of libraries", user can use only the components she needs
* Works on Windows, macOS and Linux
* Has zero dependency except [Zig compiler (master)](https://ziglang.org/download/) and `git` with [Git LFS](https://git-lfs.github.com/) - no Visual Studio, Build Tools, Windows SDK, gcc, dev packages, system headers/libs, cmake, ninja, etc. is needed
* Building is as easy as running `zig build` (see: [Building](#building-sample-applications))
* Libraries are written from scratch in Zig *or* provide Ziggified bindings for carefully selected C/C++ libraries
* Uses native wgpu implementation ([Dawn](https://github.com/michal-z/dawn-bin)) or OpenGL for cross-platform graphics and DirectX 12 for low-level graphics on Windows

## Sample applications (native wgpu)

Some of the sample applications are listed below. More can be found in [samples](samples/) directory.

1. [physically based rendering (wgpu)](samples/physically_based_rendering_wgpu): This sample implements physically-based rendering (PBR) and image-based lighting (IBL) to achive realistic looking rendering results.<br />`zig build physically_based_rendering_wgpu-run`

   <a href="samples/physically_based_rendering_wgpu"><img src="samples/physically_based_rendering_wgpu/screenshot0.jpg" alt="physically based rendering (wgpu)" height="200"></a>

1. [audio experiments (wgpu)](samples/audio_experiments_wgpu): This sample lets the user experiment with audio and observe data that feeds the hardware.<br />`zig build audio_experiments_wgpu-run`

   <a href="samples/audio_experiments_wgpu"><img src="samples/audio_experiments_wgpu/screenshot.png" alt="audio experiments (wgpu)" height="200"></a>

1. [bullet physics test (wgpu)](samples/bullet_physics_test_wgpu): This sample application demonstrates how to use full 3D physics engine in your Zig programs.<br />`zig build bullet_physics_test_wgpu-run`

   <a href="samples/bullet_physics_test_wgpu"><img src="samples/bullet_physics_test_wgpu/screenshot.jpg" alt="bullet physics test (wgpu)" height="200"></a>

1. [procedural mesh (wgpu)](samples/procedural_mesh_wgpu): This sample shows how to efficiently draw several procedurally generated meshes.<br />`zig build procedural_mesh_wgpu-run`

   <a href="samples/procedural_mesh_wgpu"><img src="samples/procedural_mesh_wgpu/screenshot.png" alt="procedural mesh (wgpu)" height="200"></a>

1. [gui test (wgpu)](samples/gui_test_wgpu): This sample shows how to use our [zgui](libs/zgui) library.<br />`zig build gui_test_wgpu-run`

   <a href="samples/gui_test_wgpu"><img src="samples/gui_test_wgpu/screenshot.png" alt="gui test (wgpu)" height="200"></a>

## Sample applications (DirectX 12)

Some of the sample applications are listed below. More can be found in [samples](samples/) directory. They can be built and run on Windows and Linux (Wine + VKD3D-Proton 2.8+):

1. [bindless](samples/bindless): This sample implements physically based shading and image based lighting to achieve realistic looking rendering results. It uses bindless textures and HLSL 6.6 dynamic resources.<br />`zig build bindless-run`

   <a href="samples/bindless"><img src="samples/bindless/screenshot.png" alt="bindless" height="200"></a>

1. [rasterization](samples/rasterization): This sample application shows how GPU rasterizes triangles in slow motion.<br />`zig build rasterization-run`

   <a href="samples/rasterization"><img src="samples/rasterization/screenshot.png" alt="rasterization" height="200"></a>

1. [simple raytracer](samples/simple_raytracer): This sample implements basic hybrid renderer. It uses rasterization to resolve primary rays and raytracing (DXR) for shadow rays.<br />`zig build simple_raytracer-run`

   <a href="samples/simple_raytracer"><img src="samples/simple_raytracer/screenshot.png" alt="simple raytracer" height="200"></a>

1. [mesh shader test](samples/mesh_shader_test): This sample shows how to use DirectX 12 Mesh Shader.<br />`zig build mesh_shader_test-run`

   <a href="samples/mesh_shader_test"><img src="samples/mesh_shader_test/screenshot.png" alt="mesh shader test" height="200"></a>

## Others using zig-gamedev

* [Aftersun](https://github.com/foxnne/aftersun) - Top-down 2D RPG
* [Pixi](https://github.com/foxnne/pixi) - Pixel art editor made with Zig
* [Simulations](https://github.com/ckrowland/simulations) - GPU Accelerated agent-based modeling to visualize and simulate complex systems
* [elvengroin legacy](https://github.com/Srekel/elvengroin-legacy) - TBD
* [jok](https://github.com/jack-ji/jok) - A minimal 2D/3D game framework for Zig

## Building sample applications

To build all sample applications (assuming `zig` is in the PATH and [Git LFS](https://git-lfs.github.com/) is installed):

1. `git clone https://github.com/zig-gamedev/zig-gamedev.git`
1. `cd zig-gamedev`
1. `zig build`

Build artifacts will show up in `zig-out/bin` folder.

`zig build <sample_name>` will build sample application named `<sample_name>`.

`zig build <sample_name>-run` will build and run sample application named `<sample_name>`.

To list all available sample names run `zig build --help` and navigate to `Steps` section.

#### Build options

Options for optimizations:
* `-Doptimize=[Debug|ReleaseFast|ReleaseSafe|ReleaseSmall]` - enable optimizations

Options for Windows applications:
* `-Dzd3d12-enable-debug-layer=[bool]` - Direct3D 12, Direct2D, DXGI debug layers enabled
* `-Dzd3d12-enable-gbv=[bool]` - Direct3D 12 GPU-Based Validation (GBV) enabled
* `-Dzpix-enable=[bool]` - PIX markers and events enabled

## GitHub Sponsors
Thanks to all people who sponsor zig-gamedev project! In particular, these fine folks sponsor zig-gamedev for $25/month or more:
* **[Derek Collison (derekcollison)](https://github.com/derekcollison)**
* [Garett Bass (garettbass)](https://github.com/garettbass)
* [Connor Rowland (ckrowland)](https://github.com/ckrowland)
* Zig Software Foundation (ziglang)
* Joran Dirk Greef (jorangreef)
