# zig-gamedev project

This repository contains a collection of sample applications and libraries written in **[Zig programming language](https://ziglang.org/)** and using **[DirectX 12 API](https://docs.microsoft.com/en-us/windows/win32/direct3d12/what-is-directx-12-)**. Prebuilt binaries for all sample applications can be found in [Releases](https://github.com/michal-z/zig-gamedev/releases). Project is under active development please see [Roadmap](https://github.com/michal-z/zig-gamedev/wiki/Roadmap) for the details.

Some features:

* Zero dependency except [Zig compiler](https://ziglang.org/download/) (fully standalone; no Visual Studio, no Windows SDK needed)
* Building is as easy as running `zig build` (see: [Building](#building-sample-applications))
* Helper libraries for working with DirectX 12 and real-time graphics (see: [graphics](#graphics), [vectormath](#vectormath))
* Interop with Direct2D and DirectWrite for high-quality vector graphics and text rendering
* Support for GPU profiling with PIX (markers and events API, see: [pix](#pix))
* Support for CPU profiling with [Tracy](https://github.com/wolfpld/tracy) (zones API, see: [tracy](#tracy))
* Uses some great C/C++ libraries which are seamlessly built by `zig cc` compiler (see: [external/src](external/src))

## Sample applications

Some of the sample applications are listed below. More can be found in [samples](samples/) directory.

1. [simple raytracer](samples/simple_raytracer): This sample implements basic hybrid renderer. It uses rasterization to resolve primary rays and raytracing (DXR) for shadow rays.

    <img src="samples/simple_raytracer/screenshot.png" alt="simple raytracer" height="200">

1. [physically based rendering](samples/physically_based_rendering): This sample implements physically based shading and image based lighting to achive realistic looking rendering results.

    <img src="samples/physically_based_rendering/screenshot.png" alt="physically based rendering" height="200">

1. [audio playback test](samples/audio_playback_test): This sample demonstrates how to decode .mp3 file using Microsoft Media Foundation and play it back using Windows Audio Session API (WASAPI).

    <img src="samples/audio_playback_test/screenshot.png" alt="audio playback test" height="200">

1. [vector graphics test](samples/vector_graphics_test): This sample demonstrates how to use Direct3D 12 <-> Direct2D interop to render vector graphics and high-quality text.

    <img src="samples/vector_graphics_test/screenshot.png" alt="vector graphics test" height="200">

1. [DirectML convolution test](samples/directml_convolution_test): This sample demonstrates how to perform GPU-accelerated convolution operation using DirectML.

    <img src="samples/directml_convolution_test/screenshot.png" alt="directml convolution test" height="200">

## Libraries

### [graphics](libs/common/graphics.zig)

Some features of graphics library:

* Basic DirectX 12 context management (descriptor heaps, memory heaps, swapchain, CPU and GPU sync, etc.)
* Basic DirectX 12 resource management (handle-based resources and pipelines)
* Basic resource barriers management with simple state-tracking
* Fast image loading using WIC (Windows Imaging Component)
* Helpers for uploading data to the GPU
* Fast mipmap generator running on the GPU
* Interop with Direct2D and DirectWrite for high-quality vector graphics and text rendering
* Custom integration of [dear imgui](https://github.com/ocornut/imgui) library

### [vectormath](libs/common/vectormath.zig)

This libarary implements all basic linear algebra operations for Vec2, Vec3, Vec4, Mat4 and Quat.

### [pix](libs/common/pix3.zig)

This is a simple libarary that lets you mark named events on the GPU timeline. Those events can be then anaylzed in PIX. Additionaly, you can programmatically record PIX traces to a file. Note, that this library does not require WinPixEventRuntime.dll to work. Following operations are supported:

* beginCapture, endCapture
* beginEventOnCommandList, endEventOnCommandList, setMarkerOnCommandList
* beginEventOnCommandQueue, endEventOnCommandQueue, setMarkerOnCommandQueue

### [tracy](libs/common/tracy.zig)

This is a simple libarary that lets you mark named events (zones) on the CPU timeline. Zones can be then anaylzed in [Tracy](https://github.com/wolfpld/tracy) profiler. Following operation are supported:

* zone, zoneN, zoneNC
* frameMark, frameMarkNamed

## Building sample applications

As mentioned above, the only dependency needed to build this project is [Zig compiler](https://ziglang.org/download/), neither Visual Studio nor Windows SDK has to be installed.

Zig compiler consists of a single ~60MB .zip file that needs to be downloaded separately. Latest development build of the compiler must be used (master) you can download prebuilt binaries [here](https://ziglang.org/download/).

To build a sample application (assuming zig.exe is in the PATH):

1. Open terminal window.
1. 'cd' to sample application root directory (for example, `cd samples/simple_raytracer`).
1. Run `zig build` command.
1. Sample application will be build, assets and build artifacts will be copied to `samples/<sample_name>/zig-out/bin` directory.

Behind the scenes `zig build` command performs following steps:

1. `zig cc` builds all C/C++ libraries that application uses (imgui, cgltf).
1. DirectX Shader Compiler (which can be found in [external/bin/dxc](external/bin/dxc) directory) is invoked to build all HLSL shaders.
1. Zig code is compiled.
1. Everything is linked together into single executable.
1. Assets and build artifacts are copied to destination directory.

You can look at [samples/simple_raytracer/build.zig](samples/simple_raytracer/build.zig) file to see how those steps are implemented in Zig build script.

#### Build options

All sample applications support following build options:

* `-Drelease-safe=[bool]` - Optimizations on and safety on
* `-Drelease-fast=[bool]` - Optimizations on and safety off
* `-Denable-pix=[bool]` - PIX markers and events enabled
* `-Denable-dx-debug=[bool]` - Direct3D 12, Direct2D, DXGI debug layers enabled
* `-Denable-dx-gpu-debug=[bool]` - Direct3D 12 GPU-Based Validation enabled (requires -Denable-dx-debug=true)
* `-Dtracy=[path/to/tracy/source]` - [Tracy](https://github.com/wolfpld/tracy) profiler zones enabled

Examples:<br />
`zig build -Denable-dx-debug=true -Drelease-fast=true`<br />
`zig build -Dtracy="C:/Development/tools/Tracy/tracy-0.7.8"`<br />

To build and **run** an application you can use:<br />
`zig build run` <- Builds and runs debug build.<br />
`zig build run -Drelease-fast=true -Denable-dx-debug=true` <- Builds and runs release build with DirectX debug layers enabled.<br />

## Requirements

This project uses [DirectX 12 Agility SDK](https://devblogs.microsoft.com/directx/gettingstarted-dx12agility/) which allows to always use latest DirectX 12 features regardless of Windows version installed (this works from Windows 10 November 2019 Update). In particular, following Windows versions are supported:

* Windows 10 May 2021 Update (Build 19043) or newer
* Windows 10 October 2020 Update (Build 19042.789+)
* Windows 10 May 2020 Update (Build 19041.789+)
* Windows 10 November 2019 Update (Build 18363.1350+)
