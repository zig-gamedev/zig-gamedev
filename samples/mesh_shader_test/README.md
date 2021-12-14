## mesh shader test

This sample application is an introduction to DirectX 12 Mesh Shaders (MS). It draws a large amount of geometry using three techniques.

When drawing **200M triangles on GTX 1660**:
- Mesh Shader (emulating VS, no culling): ~26.8 ms
- Vertex Shader with programmable vertex fetching (no HW index buffer): ~50.2 ms
- Vertex Shader with fixed function vertex fetching: ~24.8 ms

When drawing **200M triangles on RX 6800**:
- Mesh Shader (emulating VS, no culling): ~23.6 ms
- Vertex Shader with programmable vertex fetching (no HW index buffer): ~13.5 ms
- Vertex Shader with fixed function vertex fetching: ~12.4 ms

<br />

![image](screenshot.png)
