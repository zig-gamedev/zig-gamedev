# zig-gamedev project

Building gamedev ecosystem for [Zig programming language](https://ziglang.org/), everyday, full-time since July 2021. If you can, please [help to sustain](https://github.com/sponsors/michal-z) the project. We create:

* Sample [applications](#sample-applications)
* Cross-platform and composable [libraries](#libraries)
* Mini-games (coming soon)

To get started on Windows/Linux/Mac try out [physically based rendering (wgpu)](https://github.com/michal-z/zig-gamedev/tree/main/samples/physically_based_rendering_wgpu) sample:
```
git clone https://github.com/michal-z/zig-gamedev.git
cd zig-gamedev
zig build physically_based_rendering_wgpu-run
```

If you are interested, please see [Monthly Progress Reports](https://github.com/michal-z/zig-gamedev/wiki/Progress-Reports) and our [Roadmap](https://github.com/michal-z/zig-gamedev/wiki/Roadmap).

## Libraries
Library | Latest version | Description
------- | --------- | ---------------
**[zgpu](libs/zgpu)** | 0.9 | Small helper library built on top of native wgpu implementation ([Dawn](https://github.com/michal-z/dawn-bin))
**[zgui](libs/zgui)** | 0.9 | Easy to use [dear imgui](https://github.com/ocornut/imgui) bindings (includes [ImPlot](https://github.com/epezent/implot))
**[zaudio](libs/zaudio)** | 0.9 | Full-featured audio library built on top of [miniaudio](https://github.com/mackron/miniaudio)
**[zmath](libs/zmath)** | 0.3 | SIMD math library for game developers
**[zstbi](libs/zstbi)** | 0.2 | Image loading with [stbi](https://github.com/nothings/stb)
**[zglfw](libs/zglfw)** | 0.1 | Minimalistic [GLFW](https://github.com/glfw/glfw) bindings with no translate-c dependency
**[zbullet](libs/zbullet)** | 0.2 | Zig bindings and C API for [Bullet physics library](https://github.com/bulletphysics/bullet3)
**[zmesh](libs/zmesh)** | 0.2 | Loading, generating, processing and optimizing triangle meshes
**[znoise](libs/znoise)** | 0.1 | Zig bindings for [FastNoiseLite](https://github.com/Auburn/FastNoiseLite)
**[ztracy](libs/ztracy)** | 0.9 | Support for CPU profiling with [Tracy](https://github.com/wolfpld/tracy)
**[zpool](libs/zpool)** | 0.9 | Generic pool & handle implementation
**[zjobs](libs/zjobs)** | 0.1 | Generic job queue implementation

## Project vision
* Very modular "toolbox of libraries", user can use only the components she needs
* Works on Windows 10+ (DirectX 12), macOS 12+ (Metal) and Linux (Vulkan)
* Has zero dependency except [Zig compiler (master)](https://ziglang.org/download/) and `git` with [Git LFS](https://git-lfs.github.com/) - no Visual Studio, Build Tools, Windows SDK, gcc, dev packages, system headers/libs, cmake, ninja, etc. is needed
* Building is as easy as running `zig build` (see: [Building](#building-sample-applications))
* Libraries are written from scratch in Zig *or* provide Ziggified bindings for carefully selected C/C++ libraries
* Uses native wgpu implementation ([Dawn](https://github.com/michal-z/dawn-bin)) for cross-platfrom graphics and DirectX 12 for low-level graphics on Windows ([windows branch](https://github.com/michal-z/zig-gamedev/tree/windows))

## Sample applications

Some of the sample applications are listed below. More can be found in [samples](samples/) directory.

1. [physically based rendering (wgpu)](samples/physically_based_rendering_wgpu): This sample implements physically-based rendering (PBR) and image-based lighting (IBL) to achive realistic looking rendering results.<br />Build and run with: `zig build physically_based_rendering_wgpu-run`

    <a href="samples/physically_based_rendering_wgpu"><img src="samples/physically_based_rendering_wgpu/screenshot0.jpg" alt="physically based rendering (wgpu)" height="200"></a>

1. [audio experiments (wgpu)](samples/audio_experiments_wgpu): This sample lets the user to experiment with audio and observe data that feeds the hardware.<br />Build and run with: `zig build audio_experiments_wgpu-run`

    <a href="samples/audio_experiments_wgpu"><img src="samples/audio_experiments_wgpu/screenshot.png" alt="audio experiments (wgpu)" height="200"></a>

1. [bullet physics test (wgpu)](samples/bullet_physics_test_wgpu): This sample application demonstrates how to use full 3D physics engine in your Zig programs.<br />Build and run with: `zig build bullet_physics_test_wgpu-run`

    <a href="samples/bullet_physics_test_wgpu"><img src="samples/bullet_physics_test_wgpu/screenshot.jpg" alt="bullet physics test (wgpu)" height="200"></a>

1. [procedural mesh (wgpu)](samples/procedural_mesh_wgpu): This sample shows how to efficiently draw several procedurally generated meshes.<br />Build and run with: `zig build procedural_mesh_wgpu-run`

    <a href="samples/procedural_mesh_wgpu"><img src="samples/procedural_mesh_wgpu/screenshot.png" alt="procedural mesh (wgpu)" height="200"></a>

1. [gui test (wgpu)](samples/gui_test_wgpu): This sample shows how to use our [zgui](libs/zgui) library.<br />Build and run with: `zig build gui_test_wgpu-run`

    <a href="samples/gui_test_wgpu"><img src="samples/gui_test_wgpu/screenshot.png" alt="gui test (wgpu)" height="200"></a>

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
