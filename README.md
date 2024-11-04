# [zig-gamedev](https://github.com/zig-gamedev) dev repo

The original repo spawned in July 2021 by [Michal Ziulek](https://github.com/michal-z). This is the main development repo for the [zig-gamedev libraries](https://github.com/zig-gamedev#libraries) and [sample applications](#sample-applications-native-wgpu).

Zig is still in development. This repo aims to track zig nightly.

Libraries now live in their own repositories and are included in this repo as git submodules for developer convenience.

### Build and run the [Samples](#sample-applications-native-wgpu)

To get started on Windows/Linux/macOS try out [physically based rendering (wgpu)](https://github.com/zig-gamedev/zig-gamedev/tree/main/samples/physically_based_rendering_wgpu) sample:
```
zig build physically_based_rendering_wgpu-run
```

To get a list of all available build steps:
```
zig build -l
```

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
