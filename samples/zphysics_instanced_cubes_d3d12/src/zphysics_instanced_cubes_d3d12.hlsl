#define root_signature "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),CBV(b0)"

cbuffer SceneConstantBuffer : register(b0) {
    row_major matrix mvp;
};

struct Pixel {
  float4 color : COLOR;
  float4 position : SV_POSITION;
};

struct CubeVertex {
    float3 position: CUBE_MESH_POSITION;
    float3 normal: CUBE_MESH_NORMAL;
    row_major matrix transform: CUBE_TRANSFORM;
    float3 color: CUBE_COLOR;
};

[RootSignature(root_signature)]
Pixel cubeVs(
    CubeVertex vertex
) {
    Pixel pixel;

    float4 position = mul(float4(vertex.position, 1.0), vertex.transform);
    pixel.position = mul(position, mvp);


    float4 normal_end = mul(float4(vertex.position + vertex.normal, 1.0), vertex.transform);
    float4 normal = normal_end - position;
    float3 light_direction = float3(0.0, 1.0, 0.0);
    float3 ambient = 0.5;
    pixel.color = saturate(float4(vertex.color * dot(normal.xyz, light_direction) + vertex.color * ambient, 1.0));

    return pixel;
}

[RootSignature(root_signature)]
float4 ps(
    Pixel pixel
): SV_Target {
    return pixel.color;
}
