# zig-gamedev project

This repository contains a collection of sample applications, libraries and other tools for game developers using [Zig](https://ziglang.org/) programming language and targeting Windows platform.

Sample applications use [DirectX 12](https://docs.microsoft.com/en-us/windows/win32/direct3d12/directx-12-programming-guide) for 3D rendering, [Direct2D and DirectWrite](https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-portal) for 2D rendering, [WASAPI](https://docs.microsoft.com/en-us/windows/win32/coreaudio/wasapi) for low-latency audio processing and [DirectML](https://docs.microsoft.com/en-us/windows/ai/directml/dml) for high-performance, GPU-accelerated Machine Learning.

Helper libraries implemented in Zig that this project provides are: graphics, vectormath, tracy, pix. See [Libraries](#libraries) section below for more informations.

Additionally, Zig compiler can build C/C++ code which then can be easily invoked from Zig code. This project takes advantage of this capability to use some great C/C++ libraries.

## Building sample applications

Not counting [Zig compiler](https://ziglang.org/download/) - **this repository is fully standalone** - neither Visual Studio nor Windows SDK needs to be installed to build, modify and re-build this project.

Zig compiler consists of single ~60MB .zip file and needs to be downloaded separately. Latest development build of the compiler must be used (master) you can download prebuilt binaries [here](https://ziglang.org/download/).

To build a sample application (assuming zig.exe is in the PATH):

1. Open terminal window.
1. 'cd' to sample application root directory (for example, `cd samples/simple_raytracer`).
1. Run `zig build` command.
1. Sample application will be build, assets and build artifacts will be copied to `samples/<sample_name>/zig-out/bin` folder.

Behind the scenes `zig build` command performs following steps:

1. `zig cc` builds all C/C++ libraries that application uses (imgui, cgltf).
1. DirectX Shader Compiler (dxc) which can be found in `external/bin/dxc` folder is invoked to build all HLSL shaders.
1. Zig code is compiled.
1. Everything is linked together into single executable.
1. Assets and build artifacts are copied to destination folder.

You can look at [samples/simple_raytracer/build.zig](samples/simple_raytracer/build.zig) file to see how those steps are implemented in Zig.

## Requirements

This project uses [DirectX 12 Agility SDK](https://devblogs.microsoft.com/directx/gettingstarted-dx12agility/) which allows to always use latest DirectX 12 features regardless of Windows version installed (this works from Windows 10 November 2019 Update). In particular, following Windows versions are supported:

* Windows 10 May 2021 Update (Build 19043) or newer.
* Windows 10 October 2020 Update (Build 19042.789+).
* Windows 10 May 2020 Update (Build 19041.789+).
* Windows 10 November 2019 Update (Build 18363.1350+).

## Sample applications

Below you can find a list of more interesting samples implemented in Zig. More can be found in [samples](samples/) directory.

1. [simple raytracer](samples/simple_raytracer): This sample implements basic hybrid renderer. It uses rasterization to resolve primary rays and raytracing for shadow rays. Right Mouse button and W, A, S, D keys can be used to move the camera.

    <img src="screenshots/simple_raytracer.png" alt="simple raytracer" height="200">

1. [physically based rendering](samples/physically_based_rendering): This sample uses physically based shading and image based lighting to achive realistic looking rendering results. Right Mouse button and W, A, S, D keys can be used to move the camera.

    <img src="screenshots/physically_based_rendering.png" alt="physically based rendering" height="200">

1. [audio playback test](samples/audio_playback_test): This sample demonstrates how to decode .mp3 file using Windows Media Foundation and play it back with Windows Audio Session API (WASAPI).

    <img src="screenshots/audio_playback_test.png" alt="audio playback test" height="200">

#### Build options

All sample applications support following build options:

* `-Denable-pix=[true|false]` - builds an executable with PIX markers and events enabled/disabled. Default value is false.
* `-Denable-dx-debug=[true|false]` - builds an executable with Direct3D 12, Direct2D, DXGI, DirectML debug layers enabled/disabled. Default value is false.
* `-Denable-dx-gpu-debug=[true|false]` - builds an executable with Direct3D 12 GPU-Based Validation enabled. Requires -Denable-dx-debug=true. Default value is false.
* `-Dtracy=[path/to/tracy/source]` - builds an executable with [Tracy](https://github.com/wolfpld/tracy) profiler instrumentation enabled.

Examples:<br/>
`zig build -Denable-dx-debug=true`<br/>
`zig build -Dtracy="C:/Development/tools/Tracy/tracy-0.7.8"`<br/>

## Libraries

### [graphics](libs/common/graphics.zig)

Some of the features of graphics library:

* Basic DirectX 12 context management (descriptor heaps, memory heaps, swapchain, CPU and GPU sync, etc.).
* Basic DirectX 12 resource management (handle-based resources and pipelines).
* Basic resource barriers management with simple state-tracking.
* Loading images.
* Fast mipmap generator running on the GPU.
* Interop with Direct2D and DirectWrite.
* Custom integration of [dear imgui](https://github.com/ocornut/imgui) library.

### [vectormath](libs/common/vectormath.zig)

This libarary implements all basic linear algebra operations for Vec2, Vec3, Vec4, Mat4 and Quat.

### [pix](libs/common/pix3.zig)

This is a simple libarary that lets you mark named events on the GPU timeline. Those events can be then anaylzed in PIX. Additionaly, you can programmatically record PIX traces to a file. Note, that this library does not require WinPixEventRuntime.dll to work. Following operations are supported:

* beginCapture, endCapture
* beginEventOnCommandList, endEventOnCommandList, setMarkerOnCommandList
* beginEventOnCommandQueue, endEventOnCommandQueue, setMarkerOnCommandQueue

### [tracy](libs/common/tracy.zig)

This is a simple libarary that lets you mark named events on the CPU timeline. Those events can be then anaylzed in [Tracy](https://github.com/wolfpld/tracy) profiler. Following operation are supported:

* zone, zoneN, zoneNC
* frameMark, frameMarkNamed
