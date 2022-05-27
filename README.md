# zig-gamedev project

This repository contains a collection of [sample applications](#cross-platfrom-sample-applications-native-webgpu) and **cross-platform, standalone, composable libraries** written in **[Zig programming language](https://ziglang.org/)**.
Project is under active development, see [Roadmap](https://github.com/michal-z/zig-gamedev/wiki/Roadmap) and [Progress Reports](https://github.com/michal-z/zig-gamedev/wiki/Progress-Reports) for the details.

To get started on Windows/Linux/Mac try out [physically based rendering (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/physically_based_rendering_wgpu) sample:

(`git` with [Git LFS](https://git-lfs.github.com/) extension and [Zig 0.10.0-dev.2412 (master)](https://ziglang.org/download/) or newer is required)
```
git clone https://github.com/michal-z/zig-gamedev.git
cd zig-gamedev
zig build physically_based_rendering_wgpu-run
```
#### Cross-platfrom (Win/Lin/Mac) libraries:
* [zgpu](https://github.com/michal-z/zig-gamedev/tree/main/libs/zgpu) - Cross-platform graphics layer built on top of native WebGPU API (Dawn)
* [zmath](https://github.com/michal-z/zig-gamedev/blob/main/libs/zmath) - SIMD math library for game developers
* [zbullet](https://github.com/michal-z/zig-gamedev/blob/main/libs/zbullet) - Zig bindings and C API for [Bullet physics library](https://github.com/bulletphysics/bullet3)
* [zmesh](https://github.com/michal-z/zig-gamedev/blob/main/libs/zmesh) - loading, generating, processing and optimizing triangle meshes
* [znoise](https://github.com/michal-z/zig-gamedev/blob/main/libs/znoise) - Zig bindings for [FastNoiseLite](https://github.com/Auburn/FastNoiseLite)
* [znetwork](https://github.com/michal-z/zig-gamedev/blob/main/libs/znetwork) - Zig bindings for [ENet](https://github.com/lsalzman/enet)
* [ztracy](https://github.com/michal-z/zig-gamedev/blob/main/libs/ztracy) - support for CPU profiling with [Tracy](https://github.com/wolfpld/tracy)
* [zpool](https://github.com/michal-z/zig-gamedev/blob/main/libs/zpool) - generic pool & handle implementation

#### Windows libraries:
* [zwin32](https://github.com/michal-z/zig-gamedev/blob/main/libs/zwin32) - Zig bindings for Win32 API
* [zd3d12](https://github.com/michal-z/zig-gamedev/blob/main/libs/zd3d12) - helper library for working with DirectX 12
* [zxaudio2](https://github.com/michal-z/zig-gamedev/blob/main/libs/zxaudio2) - helper library for working with XAudio2
* [zpix](https://github.com/michal-z/zig-gamedev/blob/main/libs/zpix) - support for GPU profiling with PIX
* Interop with Direct2D and DirectWrite for high-quality vector graphics and text rendering (optional)

#### Project vision:
* Works on Windows, Linux and MacOS
* Has zero dependency except [Zig compiler (master)](https://ziglang.org/download/), `git` with [Git LFS](https://git-lfs.github.com/) and `curl` - no Visual Studio, Build Tools, Windows SDK, gcc, dev packages, system headers/libs, cmake, ninja, etc. is needed
* Building is as easy as running `zig build` (see: [Building](#building-sample-applications))
* Libraries are written from scratch in Zig *or* provide Ziggified bindings to carefully selected C/C++ libraries
* Uses native version of WebGPU API ([mach/gpu](https://github.com/hexops/mach/tree/main/gpu)) for cross-platfrom graphics and DirectX 12 for low-level graphics on Windows

*I work on this project full-time and try to make a living from donations. If you like it, please consider [supporting me](https://github.com/sponsors/michal-z). Thanks!*

## Cross-platfrom (Win/Lin/Mac) sample applications (native WebGPU)

Some of the sample applications are listed below. More can be found in [samples](samples/) directory.

1. [physically based rendering (wgpu)](samples/physically_based_rendering_wgpu): This sample implements physically-based rendering (PBR) and image-based lighting (IBL) to achive realistic looking rendering results.

    <a href="samples/physically_based_rendering_wgpu"><img src="samples/physically_based_rendering_wgpu/screenshot0.jpg" alt="physically based rendering (wgpu)" height="200"></a>

    `zig build physically_based_rendering_wgpu-run`

1. [procedural mesh (wgpu)](samples/procedural_mesh_wgpu): This sample shows how to efficiently draw several procedurally generated meshes.

    <a href="samples/procedural_mesh_wgpu"><img src="samples/procedural_mesh_wgpu/screenshot.png" alt="procedural mesh wgpu (wgpu)" height="200"></a>

    `zig build procedural_mesh_wgpu-run`

## Windows sample applications (DirectX 12)

If you are new to DirectX 12 graphics programming I recommend starting with [intro applications](https://github.com/michal-z/zig-gamedev/tree/main/samples/intro).

1. [rasterization](samples/rasterization): This sample application shows how GPU rasterizes triangles in slow motion.

    <a href="samples/rasterization"><img src="samples/rasterization/screenshot.png" alt="rasterization" height="200"></a>

    `zig build rasterization-run`

1. [simple raytracer](samples/simple_raytracer): This sample implements basic hybrid renderer. It uses rasterization to resolve primary rays and raytracing (DXR) for shadow rays.

    <a href="samples/simple_raytracer"><img src="samples/simple_raytracer/screenshot.png" alt="simple raytracer" height="200"></a>

    `zig build simple_raytracer-run`

1. [virtual physics lab](samples/bullet_physics_test): This sample application demonstrates how to use full 3D physics engine in your Zig programs.

    <a href="samples/bullet_physics_test"><img src="samples/bullet_physics_test/screenshot1.png" alt="virtual physics lab" height="200"></a>

    `zig build bullet_physics_test-run`

1. [mesh shader test](samples/mesh_shader_test): This sample shows how to use DirectX 12 Mesh Shader.

    <a href="samples/mesh_shader_test"><img src="samples/mesh_shader_test/screenshot.png" alt="mesh shader test" height="200"></a>

    `zig build mesh_shader_test-run`

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
* `-Dzgpu-dawn-from-source=[bool]` - Build Dawn (WebGPU implementation) from source

Addidtional options for Windows applications:
* `-Denable-dx-debug=[bool]` - Direct3D 12, Direct2D, DXGI debug layers enabled
* `-Denable-dx-gpu-debug=[bool]` - Direct3D 12 GPU-Based Validation enabled (requires -Denable-dx-debug=true)
* `-Dzpix-enable=[bool]` - PIX markers and events enabled

## GitHub Sponsors
Thanks to all people who sponsor zig-gamedev project! In particular, these fine folks sponsor zig-gamedev for $25/month or more:
* **[Derek Collison (derekcollison)](https://github.com/derekcollison)**
* [mzet (mzet-)](https://github.com/mzet-)
* [Garett Bass (garettbass)](https://github.com/garettbass)
* Zig Software Foundation (ziglang)
* Ian (LinuXY)
* Simon A. Nielsen Knights (tauoverpi)
* shintales (shintales)
* Chris Heyes (hazeycode)
* Joran Dirk Greef (jorangreef)
