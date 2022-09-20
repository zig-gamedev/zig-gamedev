**Project requires [Zig 0.10.0-dev.4060 (master)](https://ziglang.org/download/) or newer to compile.**
# zig-gamedev project (Windows branch)

To get started try out [physically based rendering](https://github.com/michal-z/zig-gamedev/tree/main/samples/physically_based_rendering) sample:

(`git` with [Git LFS](https://git-lfs.github.com/) extension and [Zig 0.10.0-dev.4060 (master)](https://ziglang.org/download/) or newer is required)
```
git clone https://github.com/michal-z/zig-gamedev.git
cd zig-gamedev
zig build physically_based_rendering-run
```

#### Libraries:

* [zwin32](https://github.com/michal-z/zig-gamedev/blob/main/libs/zwin32) - Zig bindings for Win32 API
* [zd3d12](https://github.com/michal-z/zig-gamedev/blob/main/libs/zd3d12) - helper library for working with DirectX 12
* [zxaudio2](https://github.com/michal-z/zig-gamedev/blob/main/libs/zxaudio2) - helper library for working with XAudio2
* [zpix](https://github.com/michal-z/zig-gamedev/blob/main/libs/zpix) - support for GPU profiling with PIX
* Interop with Direct2D and DirectWrite for high-quality vector graphics and text rendering (optional)

## Sample applications (DirectX 12)

If you are new to DirectX 12 graphics programming I recommend starting with [intro applications](https://github.com/michal-z/zig-gamedev/tree/main/samples/intro).

1. [rasterization](samples/rasterization): This sample application shows how GPU rasterizes triangles in slow motion.

    <a href="samples/rasterization"><img src="samples/rasterization/screenshot.png" alt="rasterization" height="200"></a>

    `zig build rasterization-run`

1. [simple raytracer](samples/simple_raytracer): This sample implements basic hybrid renderer. It uses rasterization to resolve primary rays and raytracing (DXR) for shadow rays.

    <a href="samples/simple_raytracer"><img src="samples/simple_raytracer/screenshot.png" alt="simple raytracer" height="200"></a>

    `zig build simple_raytracer-run`

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

Addidtional options for Windows applications:
* `-Denable-dx-debug=[bool]` - Direct3D 12, Direct2D, DXGI debug layers enabled
* `-Denable-dx-gpu-debug=[bool]` - Direct3D 12 GPU-Based Validation enabled (requires -Denable-dx-debug=true)
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
