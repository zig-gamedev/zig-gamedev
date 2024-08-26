## minimal glfw d3d12

This sample application shows how to draw a triangle using D3D12 API with [zglfw](https://github.com/michal-z/zig-gamedev/tree/main/libs/zglfw) instead of [zwindows](https://github.com/michal-z/zig-gamedev/tree/main/libs/zwindows).

You can build and run it on **Windows** and **Linux**.

To build and run on Windows:

```
zig build minimal_glfw_d3d12-run
```

To build and run on Linux ([Wine](https://www.winehq.org/) with [VKD3D-Proton](https://github.com/HansKristian-Work/vkd3d-proton) is needed):

```
zig build -Dtarget=x86_64-windows-gnu minimal_glfw_d3d12
wine zig-out/bin/minimal_glfw_d3d12.exe
```
