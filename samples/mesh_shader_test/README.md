## mesh shader test

This sample application is an introduction to DirectX 12 Mesh Shaders (MS). It draws a large amount of geometry using three techniques:

* Mesh Shader emulating Vertex Shader
* Vertex Shader with programmable vertex fetching
* Vertex Shader with fixed function vertex fetching

When drawing 200M triangles on GTX 1660:
- Mesh Shader emulating Vertex Shader: ~26.8 ms
- Vertex Shader with programmable vertex fetching: ~50.2 ms
- Vertex Shader with fixed function vertex fetching: ~24.8 ms

![image](screenshot.png)
