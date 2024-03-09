[Libraries](#libraries) - [Getting Started](#getting-started) - [Sample applications](#sample-applications-native-wgpu) - [Others using zig-gamedev](#others-using-zig-gamedev)

# zig-gamedev project

We build game development ecosystem for [Zig programming language](https://ziglang.org/), every day since July 2021. Please consider [supporting the project](https://github.com/sponsors/hazeycode). We create:

* Cross-platform and composable [libraries](#libraries)
* Cross-platform [sample applications](#sample-applications-native-wgpu)
* DirectX 12 [sample applications](#sample-applications-directx-12)

### Vision
* Very modular "toolbox of libraries", user can use only the components she needs
* Just [Zig](https://ziglang.org) is required to build on Windows, macOS and Linux - no Visual Studio, Build Tools, Windows SDK, gcc, dev packages, system headers/libs, cmake, ninja, etc. is needed
* Building is as easy as `zig build`
* Libraries are written from scratch in Zig *or* provide Ziggified bindings for carefully selected C/C++ libraries
* Uses native wgpu implementation ([Dawn](https://github.com/michal-z/dawn-bin)) or OpenGL for cross-platform graphics and DirectX 12 for low-level graphics on Windows


## Getting Started

Download the [latest archive](https://github.com/zig-gamedev/zig-gamedev/archive/refs/heads/main.zip) or clone/submodule with Git.

Note: If using Git then you will need [Git LFS](https://git-lfs.github.com/) to be installed.

### Get Zig

Our [main](https://github.com/zig-gamedev/zig-gamedev/tree/main) branch is currenly tracking Zig **0.12.0-dev.2063+804cee3b9** as [nominated by the Mach engine project](https://machengine.org/about/nominated-zig) to maintain compatibilty for users of both projects.

[zigup](https://github.com/marler8997/zigup) is recommended for managing compiler versions. Alternatively, you can download and install manually using the links below:

| OS/Arch         | Download link               |
| --------------- | --------------------------- |
| Windows x86_64  | [zig-windows-x86_64-0.12.0-dev.2063+804cee3b9.zip](https://ziglang.org/builds/zig-windows-x86_64-0.12.0-dev.2063+804cee3b9.zip) |
| Linux x86_64    | [zig-linux-x86_64-0.12.0-dev.2063+804cee3b9.tar.xz](https://ziglang.org/builds/zig-linux-x86_64-0.12.0-dev.2063+804cee3b9.tar.xz) |
| macOS x86_64    | [zig-macos-x86_64-0.12.0-dev.2063+804cee3b9.tar.xz](https://ziglang.org/builds/zig-macos-x86_64-0.12.0-dev.2063+804cee3b9.tar.xz) |
| macOS aarch64   | [zig-macos-aarch64-0.12.0-dev.2063+804cee3b9.tar.xz](https://ziglang.org/builds/zig-macos-aarch64-0.12.0-dev.2063+804cee3b9.tar.xz) |

If you need to use a more recent version of Zig, you can try our [unstable](https://github.com/zig-gamedev/zig-gamedev/tree/unstable) branch. But this is not generally recommended.

### Build and run the [Samples](#sample-applications-native-wgpu)

To get started on Windows/Linux/macOS try out [physically based rendering (wgpu)](https://github.com/zig-gamedev/zig-gamedev/tree/main/samples/physically_based_rendering_wgpu) sample:
```
zig build physically_based_rendering_wgpu-run
```

To get a list of all available build steps:
```
zig build -l
```

### Using the [Libraries](#Libraries)

Option to download packages using Zig Package Manager **coming soon!**

Copy each library to a subdirectory in your project and add them as local package dependencies. For example:

`build.zig.zon`

 ```zig
 .{
     .name = "MyGame",
     .version = "0.0.0",
     .dependencies = .{
         .zglfw = .{ .path = "libs/zglfw" },
         .system_sdk = .{ .path = "libs/system-sdk" },
     },
     .paths = "",
 }
 ```

`build.zig`

 ```zig
 const zglfw = @import("zglfw");

 pub fn build(b: *std.Build) void {
      const zglfw_pkg = zglfw.package(b, target, optimize, .{});

      ...

      zglfw_pkg.link(exe);

      ...
 }
 ```

Refer to each lib's README.md for further usage intructions.


## Libraries
| Library                       | Latest version | Description                                                                                                                |
|-------------------------------|----------------|----------------------------------------------------------------------------------------------------------------------------|
| **[zphysics](libs/zphysics)** | 0.0.6          | Build package, [C API](https://github.com/zig-gamedev/zig-gamedev/tree/main/libs/zphysics/libs/JoltC) and bindings for [Jolt Physics](https://github.com/jrouwe/JoltPhysics)                                                |
| **[zflecs](libs/zflecs)**     | 0.0.1          | Build package and bindings for [flecs](https://github.com/SanderMertens/flecs) ECS                                                       |
| **[zopengl](libs/zopengl)**   | 0.4.3          | OpenGL loader (supports 4.2 Core Profile and ES 2.0 Profile)                                                               |
| **[zsdl](libs/zsdl)**         | 0.0.1          | Bindings for SDL2 and SDL3 (wip)                                                                                                    |
| **[zgpu](libs/zgpu)**         | 0.9.1          | Small helper library built on top of native WebGPU implementation ([Dawn](https://github.com/michal-z/dawn-bin))             |
| **[zgui](libs/zgui)**         | 1.89.6         | Build package and bindings for [Dear Imgui](https://github.com/ocornut/imgui) (includes [ImPlot](https://github.com/epezent/implot)) |
| **[zaudio](libs/zaudio)**     | 0.9.4          | Build package and bindings for [miniaudio](https://github.com/mackron/miniaudio)                             |
| **[zmath](libs/zmath)**       | 0.9.6          | SIMD math library for game developers                                                                                      |
| **[zstbi](libs/zstbi)**       | 0.9.3          | Image reading, writing and resizing with [stb](https://github.com/nothings/stb) libraries                                  |
| **[zmesh](libs/zmesh)**       | 0.9.0          | Loading, generating, processing and optimizing triangle meshes                                                             |
| **[ztracy](libs/ztracy)**     | 0.10.0         | Support for CPU profiling with [Tracy](https://github.com/wolfpld/tracy)                                                   |
| **[zpool](libs/zpool)**       | 0.9.0          | Generic pool & handle implementation                                                                                       |
| **[zglfw](libs/zglfw)**       | 0.8.0          | Build pacakage & bindings for [GLFW](https://github.com/glfw/glfw)                                  |
| **[znoise](libs/znoise)**     | 0.1.0          | Build pacakge & bindings for [FastNoiseLite](https://github.com/Auburn/FastNoiseLite)                                                  |
| **[zjobs](libs/zjobs)**       | 0.1.0          | Generic job queue implementation                                                                                           |
| **[zbullet](libs/zbullet)**   | 0.2.0          | Build package, C API and bindings for [Bullet physics library](https://github.com/bulletphysics/bullet3)                              |
| **[zwin32](libs/zwin32)**     | 0.9.0          | Bindings for Win32 API (d3d12, d3d11, xaudio2, directml, wasapi and more)                                              |
| **[zd3d12](libs/zd3d12)**     | 0.9.0          | Helper library for DirectX 12                                                                                              |
| **[zxaudio2](libs/zxaudio2)** | 0.9.0          | Helper library for XAudio2                                                                                                 |
| **[zpix](libs/zpix)**         | 0.9.0          | Support for GPU profiling with PIX for Windows                                                                             |

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

* [Tides of Revival](https://github.com/Srekel/tides-of-revival) - First-person, open-world, fantasy RPG being developed in the open
* [Simulations](https://github.com/ckrowland/simulations) - GPU Accelerated agent-based modeling to visualize and simulate complex systems
* [krateroid](https://github.com/kussakaa/krateroid) - 3D strategy game
* [blokens](https://github.com/btipling/blockens) - Voxel game
* [Delve Framework](https://github.com/Interrupt/delve-framework) - Simple game framework for making games with Lua
* [jok](https://github.com/jack-ji/jok) - A minimal 2D/3D game framework for Zig
* [Aftersun](https://github.com/foxnne/aftersun) - Top-down 2D RPG
* [Pixi](https://github.com/foxnne/pixi) - Pixel art editor made with Zig
