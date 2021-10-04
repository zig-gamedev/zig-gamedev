# zig-gamedev project

This repository contains a collection of sample applications, libraries and other tools for game developers using [Zig](https://ziglang.org/) programming language and targeting Windows platform. Sample applications use [DirectX 12](https://docs.microsoft.com/en-us/windows/win32/direct3d12/directx-12-programming-guide) for 3D rendering, [Direct2D and DirectWrite](https://docs.microsoft.com/en-us/windows/win32/direct2d/direct2d-portal) for 2D rendering, [WASAPI](https://docs.microsoft.com/en-us/windows/win32/coreaudio/wasapi) for low-latency audio playback and [DirectML](https://docs.microsoft.com/en-us/windows/ai/directml/dml) for high-performance, GPU-accelerated Machine Learning.

## Building sample applications

The only dependency needed to build sample applications is [latest Zig compiler (master)](https://ziglang.org/download/). Neither Visual Studio nor Windows SDK needs to be installed. This repository is fully standalone.

To build and run sample application (assuming zig.exe is in the PATH):

1. Open terminal window.
1. 'cd' to sample application root directory (for example, `cd samples/simple_raytracer`).
1. Run `zig build run`.
1. Sample application will run and all build artifacts will be copied to `/samples/<sample_name>/zig-out/bin` folder.

## Requirements

This project uses [DirectX 12 Agility SDK](https://devblogs.microsoft.com/directx/gettingstarted-dx12agility/) which allows to always use latest DirectX 12 features regardless of Windows version (this works from Windows 10 November 2019 Update). In particular, following Windows versions are supported:

* Windows 10 May 2021 Update (Build 19043) or newer.
* Windows 10 October 2020 Update (Build 19042.789+).
* Windows 10 May 2020 Update (Build 19041.789+).
* Windows 10 November 2019 Update (Build 18363.1350+).

## Some of the applications

1. [simple raytracer](samples/simple_raytracer): This sample..

    <img src="screenshots/simple_raytracer.png" alt="simple raytracer" height="200">

1. [physically based rendering](samples/physically_based_rendering): This sample..

    <img src="screenshots/physically_based_rendering.png" alt="physically based rendering" height="200">

1. [audio playback test](samples/audio_playback_test): This sample..

    <img src="screenshots/audio_playback_test.png" alt="audio playback test" height="200">

## Command line options

## Libraries
