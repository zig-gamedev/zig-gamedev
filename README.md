[Libraries](#libraries) - [Sample applications](#sample-applications-native-wgpu) - [Vision](#vision) - [Others using zig-gamedev](#others-using-zig-gamedev) - [Monthly reports](https://github.com/michal-z/zig-gamedev/wiki/Progress-Reports) - [Roadmap](https://github.com/michal-z/zig-gamedev/wiki/Roadmap)

# zig-gamedev project

We build game development ecosystem for [Zig programming language](https://ziglang.org/), everyday since July 2021. Please consider [supporting the project](https://github.com/sponsors/michal-z). We create:

* Cross-platform and composable [libraries](#libraries)
* Cross-platform [sample applications](#sample-applications-native-wgpu)
* DirectX 12 [sample applications](#sample-applications-directx-12) and [intro applications](samples/intro)
* Mini-games (in planning)

To get started on Windows/Linux/macOS try out [physically based rendering (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/physically_based_rendering_wgpu) sample:
```
git clone https://github.com/michal-z/zig-gamedev.git
cd zig-gamedev
zig build physically_based_rendering_wgpu-run
```

## Libraries
Library | Latest version | Description
------- | --------- | ---------------
**[zgpu](libs/zgpu)** | 0.9.0 | Small helper library built on top of native wgpu implementation ([Dawn](https://github.com/michal-z/dawn-bin))
**[zgui](libs/zgui)** | 0.9.5 | Easy to use [dear imgui](https://github.com/ocornut/imgui) bindings (includes [ImPlot](https://github.com/epezent/implot))
**[zaudio](libs/zaudio)** | 0.9.3 | Fully-featured audio library built on top of [miniaudio](https://github.com/mackron/miniaudio)
**[zmath](libs/zmath)** | 0.9.5 | SIMD math library for game developers
**[zstbi](libs/zstbi)** | 0.9.2 | Image reading, writing and resizing with [stb](https://github.com/nothings/stb) libraries
**[zmesh](libs/zmesh)** | 0.9.0 | Loading, generating, processing and optimizing triangle meshes
**[ztracy](libs/ztracy)** | 0.9.0 | Support for CPU profiling with [Tracy](https://github.com/wolfpld/tracy)
**[zpool](libs/zpool)** | 0.9.0 | Generic pool & handle implementation
**[zglfw](libs/zglfw)** | 0.5.2 | Minimalistic [GLFW](https://github.com/glfw/glfw) bindings with no translate-c dependency
**[znoise](libs/znoise)** | 0.1.0 | Zig bindings for [FastNoiseLite](https://github.com/Auburn/FastNoiseLite)
**[zjobs](libs/zjobs)** | 0.1.0 | Generic job queue implementation
**[zbullet](libs/zbullet)** | 0.2.0 | Zig bindings and C API for [Bullet physics library](https://github.com/bulletphysics/bullet3)
**[zwin32](libs/zwin32)** | 0.9.0 | Zig bindings for Win32 API (d3d12, d3d11, xaudio2, directml, wasapi and more)
**[zd3d12](libs/zd3d12)** | 0.9.0 | Helper library for DirectX 12
**[zxaudio2](libs/zxaudio2)** | 0.9.0 | Helper library for XAudio2
**[zpix](libs/zpix)** | 0.9.0 | Support for GPU profiling with PIX for Windows

## Vision
* Very modular "toolbox of libraries", user can use only the components she needs
* Works on Windows 10+ (DirectX 12), macOS 12+ (Metal) and Linux (Vulkan)
* Has zero dependency except [Zig compiler (master)](https://ziglang.org/download/) and `git` with [Git LFS](https://git-lfs.github.com/) - no Visual Studio, Build Tools, Windows SDK, gcc, dev packages, system headers/libs, cmake, ninja, etc. is needed
* Building is as easy as running `zig build` (see: [Building](#building-sample-applications))
* Libraries are written from scratch in Zig *or* provide Ziggified bindings for carefully selected C/C++ libraries
* Uses native wgpu implementation ([Dawn](https://github.com/michal-z/dawn-bin)) for cross-platfrom graphics and DirectX 12 for low-level graphics on Windows

## Sample applications (native wgpu)

Some of the sample applications are listed below. More can be found in [samples](samples/) directory.

1. [physically based rendering (wgpu)](samples/physically_based_rendering_wgpu): This sample implements physically-based rendering (PBR) and image-based lighting (IBL) to achive realistic looking rendering results.<br />`zig build physically_based_rendering_wgpu-run`

    <a href="samples/physically_based_rendering_wgpu"><img src="samples/physically_based_rendering_wgpu/screenshot0.jpg" alt="physically based rendering (wgpu)" height="200"></a>

1. [audio experiments (wgpu)](samples/audio_experiments_wgpu): This sample lets the user to experiment with audio and observe data that feeds the hardware.<br />`zig build audio_experiments_wgpu-run`

    <a href="samples/audio_experiments_wgpu"><img src="samples/audio_experiments_wgpu/screenshot.png" alt="audio experiments (wgpu)" height="200"></a>

1. [bullet physics test (wgpu)](samples/bullet_physics_test_wgpu): This sample application demonstrates how to use full 3D physics engine in your Zig programs.<br />`zig build bullet_physics_test_wgpu-run`

    <a href="samples/bullet_physics_test_wgpu"><img src="samples/bullet_physics_test_wgpu/screenshot.jpg" alt="bullet physics test (wgpu)" height="200"></a>

1. [procedural mesh (wgpu)](samples/procedural_mesh_wgpu): This sample shows how to efficiently draw several procedurally generated meshes.<br />`zig build procedural_mesh_wgpu-run`

    <a href="samples/procedural_mesh_wgpu"><img src="samples/procedural_mesh_wgpu/screenshot.png" alt="procedural mesh (wgpu)" height="200"></a>

1. [gui test (wgpu)](samples/gui_test_wgpu): This sample shows how to use our [zgui](libs/zgui) library.<br />`zig build gui_test_wgpu-run`

    <a href="samples/gui_test_wgpu"><img src="samples/gui_test_wgpu/screenshot.png" alt="gui test (wgpu)" height="200"></a>
    
## Sample applications (DirectX 12)

Some of the sample applications are listed below. More can be found in [samples](samples/) directory.

1. [bindless](samples/bindless): This sample implements physically based shading and image based lighting to achive realistic looking rendering results. It uses bindless textures and HLSL 6.6 dynamic resources.<br />`zig build bindless-run`

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
* [Wrinkles](https://github.com/meshula/wrinkles) - Wrinkles Zig demonstrator
* [jok](https://github.com/jack-ji/jok) - A minimal 2D/3D game framework for Zig

## Building sample applications

To build all sample applications (assuming `zig` is in the PATH and [Git LFS](https://git-lfs.github.com/) is installed):

1. `git clone https://github.com/michal-z/zig-gamedev.git`
1. `cd zig-gamedev`
1. `zig build`

Build artifacts will show up in `zig-out/bin` folder.

`zig build <sample_name>` will build sample application named `<sample_name>`.

`zig build <sample_name>-run` will build and run sample application named `<sample_name>`.

To list all available sample names run `zig build --help` and navigate to `Steps` section.

#### Build options

All sample applications support the following build options:

* `-Drelease-safe=[bool]` - Optimizations on and safety on
* `-Drelease-fast=[bool]` - Optimizations on and safety off
* `-Dztracy-enable=[bool]` - [Tracy](https://github.com/wolfpld/tracy) profiler zones enabled

Addidtional options for Windows applications:
* `-Dzd3d12-enable-debug-layer=[bool]` - Direct3D 12, Direct2D, DXGI debug layers enabled
* `-Dzd3d12-enable-gbv=[bool]` - Direct3D 12 GPU-Based Validation (GBV) enabled
* `-Dzpix-enable=[bool]` - PIX markers and events enabled

## GitHub Sponsors
Thanks to all people who sponsor zig-gamedev project! In particular, these fine folks sponsor zig-gamedev for $25/month or more:
* **[Derek Collison (derekcollison)](https://github.com/derekcollison)**
* **[mzet (mzet-)](https://github.com/mzet-)**
* [Garett Bass (garettbass)](https://github.com/garettbass)
* [Connor Rowland (ckrowland)](https://github.com/ckrowland)
* Zig Software Foundation (ziglang)
* Ian (LinuXY)
* Simon A. Nielsen Knights (tauoverpi)
* shintales (shintales)
* Joran Dirk Greef (jorangreef)
