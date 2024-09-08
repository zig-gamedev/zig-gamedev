[Libraries](#libraries) - [Getting Started](#getting-started) - [Sample applications](#sample-applications-native-wgpu) - [Others using zig-gamedev](#others-using-zig-gamedev)

# [zig-gamedev project](https://github.com/zig-gamedev) monorepo

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

Our [main](https://github.com/zig-gamedev/zig-gamedev/tree/main) branch is currenly tracking Zig **0.13.0-dev.351+64ef45eb0**. Or you can use the **unstable** [zig-0.14.0 branch](https://github.com/zig-gamedev/zig-gamedev/tree/zig-0.14.0).

[zigup](https://github.com/marler8997/zigup) is recommended for managing compiler versions. To switch to the compiler version after branch checkout:
```sh
zigup $(< .zigversion)
```

Alternatively, you can download and install manually using the links below:

| OS/Arch         | Download link               |
| --------------- | --------------------------- |
| Windows x86_64  | [zig-windows-x86_64-0.13.0-dev.351+64ef45eb0.zip](https://ziglang.org/builds/zig-windows-x86_64-0.13.0-dev.351+64ef45eb0.zip) |
| Linux x86_64    | [zig-linux-x86_64-0.13.0-dev.351+64ef45eb0.tar.xz](https://ziglang.org/builds/zig-linux-x86_64-0.13.0-dev.351+64ef45eb0.tar.xz) |
| macOS x86_64    | [zig-macos-x86_64-0.13.0-dev.351+64ef45eb0.tar.xz](https://ziglang.org/builds/zig-macos-x86_64-0.13.0-dev.351+64ef45eb0.tar.xz) |
| macOS aarch64   | [zig-macos-aarch64-0.13.0-dev.351+64ef45eb0.tar.xz](https://ziglang.org/builds/zig-macos-aarch64-0.13.0-dev.351+64ef45eb0.tar.xz) |

### Build and run the [Samples](#sample-applications-native-wgpu)

To get started on Windows/Linux/macOS try out [physically based rendering (wgpu)](https://github.com/zig-gamedev/zig-gamedev/tree/main/samples/physically_based_rendering_wgpu) sample:
```
zig build physically_based_rendering_wgpu-run
```

To get a list of all available build steps:
```
zig build -l
```

## Libraries
Note: Libs are being migrated from [libs/](libs/) folder in this repo to each their own repository under the [zig-gamedev GitHub organisation](https://github.com/zig-gamedev). See [migration tracking PR](https://github.com/zig-gamedev/zig-gamedev/pull/668).

| Library                       | Description                                                                                                                |
|-------------------------------|----------------------------------------------------------------------------------------------------------------------------|
| **[zaudio](libs/zaudio)**     | Cross-platform audio using [miniaudio](https://github.com/mackron/miniaudio)                                                                         |
| **[zbullet](libs/zbullet)**   | Build package, [C API](https://github.com/zig-gamedev/zig-gamedev/tree/main/libs/zbullet/libs/cbullet) and bindings for [Bullet physics](https://github.com/bulletphysics/bullet3)                                                                           |
| **[zd3d12](libs/zd3d12)**     | Helper library for DirectX 12                                                                                 |
| **[zflecs](libs/zflecs)**     | Build package and bindings for [flecs](https://github.com/SanderMertens/flecs) ECS                                                         |
| **[zemscripten](libs/zemscripten)**  | Build package and shims for [Emscripten](https://emscripten.org) emsdk |
| **[zglfw](libs/zglfw)**       | Build package & bindings for [GLFW](https://github.com/glfw/glfw)                                                                          |
| **[zgpu](libs/zgpu)**         | Small helper library built on top of [Dawn](https://github.com/zig-gamedev/dawn) native WebGPU implementation                              |
| **[zgui](libs/zgui)**         | Build package and bindings for [Dear Imgui](https://github.com/ocornut/imgui), [Test engine](https://github.com/ocornut/imgui_test_engine), [ImPlot](https://github.com/epezent/implot), [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) and [imgui-node-editor](https://github.com/thedmd/imgui-node-editor)                       |
| **[zjobs](libs/zjobs)**       | Generic job queue implementation                                                                                                           |
| **[zmath](libs/zmath)**       | SIMD math library for game developers                                                                                                      |
| **[zmesh](libs/zmesh)**       | Loading, generating, processing and optimizing triangle meshes                                                                             |
| **[znoise](libs/znoise)**     | Build package & bindings for [FastNoiseLite](https://github.com/Auburn/FastNoiseLite)                                                      |
| **[zopengl](libs/zopengl)**   | OpenGL loader (supports 4.2 Core Profile and ES 2.0 Profile)                                                                               |
| **[zopenvr](libs/zopenvr)**   | Bindings for [OpenVR](https://github.com/ValveSoftware/openvr)                                                                             |
| **[zphysics](libs/zphysics)** | Build package, [C API](libs/zphysics/libs/JoltC) and bindings for [Jolt Physics](https://github.com/jrouwe/JoltPhysics)                    |
| **[zpix](libs/zpix)**         | Support for GPU profiling with PIX for Windows                                                           |
| **[zpool](libs/zpool)**       | Generic pool & handle implementation                                                                     |
| **[zsdl](libs/zsdl)**         | Bindings for SDL2 and SDL3                                                                               |
| **[zstbi](libs/zstbi)**       | Image reading, writing and resizing with [stb](https://github.com/nothings/stb) libraries                |
| **[ztracy](libs/ztracy)**     | Support for CPU profiling with [Tracy](https://github.com/wolfpld/tracy)                                                                   |
| **[zwindows](libs/zwindows)** | Windows development SDK for Zig game developers        |

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
* [Markets](https://github.com/ckrowland/markets) - Visually simulate markets of basic consumers and producers
* [krateroid](https://github.com/kussakaa/krateroid) - 3D strategy game
* [blokens](https://github.com/btipling/blockens) - Voxel game
* [Delve Framework](https://github.com/Interrupt/delve-framework) - Simple game framework for making games with Lua
* [jok](https://github.com/jack-ji/jok) - A minimal 2D/3D game framework for Zig
* [Aftersun](https://github.com/foxnne/aftersun) - Top-down 2D RPG
* [Pixi](https://github.com/foxnne/pixi) - Pixel art editor made with Zig
