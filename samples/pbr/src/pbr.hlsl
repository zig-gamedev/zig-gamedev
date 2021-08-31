#if defined(MESH_PBR)

#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "RootConstants(b0, num32BitConstants = 1, visibility = SHADER_VISIBILITY_VERTEX), " \
    "DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_VERTEX), " \
    "DescriptorTable(SRV(t1), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(s0, filter = FILTER_ANISOTROPIC, maxAnisotropy = 16, visibility = SHADER_VISIBILITY_PIXEL)"

struct EntityIndex {
    uint index;
};
ConstantBuffer<EntityIndex> cbv_entity_index : register(b0);

struct EntityData {
    float4x4 object_to_clip;
};
StructuredBuffer<EntityData> srv_entity_data : register(t0);

[RootSignature(root_signature)]
void vsMeshPbr(
    float3 position : POSITION,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    float4 tangent : _Tangent,
    out float4 out_position : SV_Position,
    out float3 out_normal : _Normal,
    out float2 out_texcoords0 : _Texcoords0,
    out float4 out_tangent : _Tangent
) {
    const uint entity_index = cbv_entity_index.index;
    out_position = mul(float4(position, 1.0), srv_entity_data[entity_index].object_to_clip);
    out_normal = normal;
    out_texcoords0 = texcoords0;
    out_tangent = tangent;
}

Texture2D srv_t1 : register(t1);
SamplerState sam_s0 : register(s0);

[RootSignature(root_signature)]
void psMeshPbr(
    float4 position : SV_Position,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    float4 tangent : _Tangent,
    out float4 out_color : SV_Target0
) {
    float3 n = normalize(normal);
    float3 color = 2.0 * abs(n) * srv_t1.Sample(sam_s0, texcoords0).rgb;
    out_color = float4(color, 1.0);
}

#elif defined(MESH_DEBUG)

#endif
