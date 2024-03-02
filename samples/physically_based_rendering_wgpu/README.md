## physically based rendering (wgpu)

This sample implements physically-based rendering (PBR) and image-based lighting (IBL) to achive realistic looking rendering results. Works on Windows, Linux and MacOS. For more details please see below.

![image](screenshot0.jpg)
![image](screenshot1.jpg)
![image](screenshot2.jpg)

Indirect lighting is precomputed and stored every time user changes current HDRI (skybox).

Precomputation phase consists of few steps:
* Generation of environment cube texture from spherical equirectangular environment texture (render to cube texture)
* Generation of irradiance cube texture (render to cube texture)
* Generation of prefiltered environment cube texture (render to cube texture)
* Generation of BRDF integration 2D texture (compute shader)

During normal rendering we:
* Sample ambient occlusion texture, base color texture, metallic-roughness texture and normal texture (those textures describe the "material" of the object)
* Compute direct lighting using camera position and results from the previous step
* Compute indirect lighting by sampling irradiance cube texture, filtered environment cube texture and BRDF integration 2D texture (we also need camera position, surface normal and surface roughness for correct texture lookups)
* Combine direct and indirect lighting
* Perform very simple tone-mapping
* Perform gamma correction

This demo exercises some features from our libraries, in particular:
* Async shader compilation (zgpu)
* Mipmap generation on the GPU (zgpu)
* GPU resource management (zgpu)
* Image loading (zstbi)
* Mesh loading (zmesh.io)
* Simple GUI (zgui)
* 3D math (zmath)
