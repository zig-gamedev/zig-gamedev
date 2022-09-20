**Project requires [Zig 0.10.0-dev.4060 (master)](https://ziglang.org/download/) or newer to compile.**
# zig-gamedev project

This repository contains a collection of [sample applications](#cross-platfrom-winlinmac-sample-applications-native-wgpu) and **cross-platform, composable libraries** written in **[Zig programming language](https://ziglang.org/)**. Currently, it provides a solution for: 3D graphics, multi-threaded physics, SIMD math, audio, GUI, image loading, noise generation and profiling.

The goal of the project is to build a **toolbox of libraries** for Zig game developers. A lot of effort is being put to make the whole package consistent and let the developer use only the components she needs. Project is being developed by contributors and by **one full-time developer**.

If you are interested, please see [Monthly Progress Reports](https://github.com/michal-z/zig-gamedev/wiki/Progress-Reports) and our [Roadmap](https://github.com/michal-z/zig-gamedev/wiki/Roadmap).

To get started on Windows/Linux/Mac try out [physically based rendering (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/physically_based_rendering_wgpu) sample:

(`git` with [Git LFS](https://git-lfs.github.com/) extension and [Zig 0.10.0-dev.4060 (master)](https://ziglang.org/download/) or newer is required)
```
git clone https://github.com/michal-z/zig-gamedev.git
cd zig-gamedev
zig build physically_based_rendering_wgpu-run
```
#### Cross-platfrom (Win/Lin/Mac) libraries:
* [zgpu](https://github.com/michal-z/zig-gamedev/tree/main/libs/zgpu) - cross-platform graphics layer built on top of native wgpu API (Dawn)
* [zgui](https://github.com/michal-z/zig-gamedev/tree/main/libs/zgui) - easy to use [dear imgui](https://github.com/ocornut/imgui) bindings
* [zaudio](https://github.com/michal-z/zig-gamedev/tree/main/libs/zaudio) - Cross-platform audio built on top of [miniaudio](https://github.com/mackron/miniaudio) library
* [zmath](https://github.com/michal-z/zig-gamedev/blob/main/libs/zmath) - SIMD math library for game developers
* [zstbi](https://github.com/michal-z/zig-gamedev/blob/main/libs/zstbi) - image loading with [stbi](https://github.com/nothings/stb)
* [zglfw](https://github.com/michal-z/zig-gamedev/blob/main/libs/zglfw) - minimalistic [GLFW](https://github.com/glfw/glfw) bindings with no translate-c dependency
* [zbullet](https://github.com/michal-z/zig-gamedev/blob/main/libs/zbullet) - Zig bindings and C API for [Bullet physics library](https://github.com/bulletphysics/bullet3)
* [zmesh](https://github.com/michal-z/zig-gamedev/blob/main/libs/zmesh) - loading, generating, processing and optimizing triangle meshes
* [znoise](https://github.com/michal-z/zig-gamedev/blob/main/libs/znoise) - Zig bindings for [FastNoiseLite](https://github.com/Auburn/FastNoiseLite)
* [ztracy](https://github.com/michal-z/zig-gamedev/blob/main/libs/ztracy) - support for CPU profiling with [Tracy](https://github.com/wolfpld/tracy)
* [zpool](https://github.com/michal-z/zig-gamedev/blob/main/libs/zpool) - generic pool & handle implementation
* [znetwork](https://github.com/michal-z/zig-gamedev/blob/main/libs/znetwork) - small abstraction layer around TCP & UDP (vendored from [here](https://github.com/MasterQ32/zig-network))

#### Project vision:
* Works on Windows, Linux and macOS
* Has zero dependency except [Zig compiler (master)](https://ziglang.org/download/) and `git` with [Git LFS](https://git-lfs.github.com/) - no Visual Studio, Build Tools, Windows SDK, gcc, dev packages, system headers/libs, cmake, ninja, etc. is needed
* Building is as easy as running `zig build` (see: [Building](#building-sample-applications))
* Libraries are written from scratch in Zig *or* provide Ziggified bindings to carefully selected C/C++ libraries
* Uses native version of wgpu API (binaries from [dawn-bin](https://github.com/michal-z/dawn-bin)) for cross-platfrom graphics and DirectX 12 for low-level graphics on Windows

*I work on this project full-time and try to make a living from donations. If you like it, please consider [supporting me](https://github.com/sponsors/michal-z). Thanks!*

## Cross-platfrom (Win/Lin/Mac) sample applications (native wgpu)

Some of the sample applications are listed below. More can be found in [samples](samples/) directory.

1. [physically based rendering (wgpu)](samples/physically_based_rendering_wgpu): This sample implements physically-based rendering (PBR) and image-based lighting (IBL) to achive realistic looking rendering results.

    <a href="samples/physically_based_rendering_wgpu"><img src="samples/physically_based_rendering_wgpu/screenshot0.jpg" alt="physically based rendering (wgpu)" height="200"></a>

    `zig build physically_based_rendering_wgpu-run`

1. [audio experiments (wgpu)](samples/audio_experiments_wgpu): This sample lets the user to experiment with audio and observe data that feeds the hardware!

    <a href="samples/audio_experiments_wgpu"><img src="samples/audio_experiments_wgpu/screenshot.png" alt="audio experiments (wgpu)" height="200"></a>

    `zig build audio_experiments_wgpu-run`

1. [bullet physics test (wgpu)](samples/bullet_physics_test_wgpu): This sample application demonstrates how to use full 3D physics engine in your Zig programs.

    <a href="samples/bullet_physics_test_wgpu"><img src="samples/bullet_physics_test_wgpu/screenshot.jpg" alt="bullet physics test (wgpu)" height="200"></a>

    `zig build bullet_physics_test_wgpu-run`

1. [procedural mesh (wgpu)](samples/procedural_mesh_wgpu): This sample shows how to efficiently draw several procedurally generated meshes.

    <a href="samples/procedural_mesh_wgpu"><img src="samples/procedural_mesh_wgpu/screenshot.png" alt="procedural mesh wgpu (wgpu)" height="200"></a>

    `zig build procedural_mesh_wgpu-run`

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
