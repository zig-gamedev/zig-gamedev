#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "RootConstants(b0, num32BitConstants = 1, visibility = SHADER_VISIBILITY_VERTEX), " \
    "DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_VERTEX)"

struct EntityIndex {
    uint index;
};
ConstantBuffer<EntityIndex> cbv_entity_index : register(b0);

struct EntityData {
    float4x4 object_to_clip;
};
StructuredBuffer<EntityData> srv_entity_data : register(t0);

[RootSignature(root_signature)]
void vsTriangle(
    float3 position : POSITION,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    out float4 out_position : SV_Position
) {
    const uint entity_index = cbv_entity_index.index;
    out_position = mul(float4(position, 1.0), srv_entity_data[entity_index].object_to_clip);
}

[RootSignature(root_signature)]
void psTriangle(
    float4 position : SV_Position,
    out float4 out_color : SV_Target0
) {
    out_color = float4(1.0, 0.5, 0.0, 1.0);
}
