## minimal

This sample application shows how to draw a triangle using D3D12 API. It has no dependencies except our [zwin32](https://github.com/michal-z/zig-gamedev/tree/main/libs/zwin32) bindings.

You can build and run it on **Windows** and **Linux** (via Wine).

To build and run on Windows:
```
zig build minimal-run
```
To build and run on Linux ([Wine](https://www.winehq.org/) with [VKD3D-Proton](https://github.com/HansKristian-Work/vkd3d-proton) is needed):
```
zig build -Dtarget=x86_64-windows-gnu minimal
wine zig-out/bin/minimal.exe
```
It has been tested on Ubuntu 22.04 with Wine 8.0-rc4 and VK3D-Proton v2.8
