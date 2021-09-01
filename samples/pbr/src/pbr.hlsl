#if defined(PSO_MESH_PBR)

#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
    "DescriptorTable(SRV(t0, numDescriptors = 4), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(s0, filter = FILTER_ANISOTROPIC, maxAnisotropy = 16, visibility = SHADER_VISIBILITY_PIXEL)"

struct Const {
    float4x4 object_to_clip;
    float4x4 object_to_world;
};
ConstantBuffer<Const> cbv_const : register(b0);

[RootSignature(root_signature)]
void vsMeshPbr(
    float3 position : POSITION,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    float4 tangent : _Tangent,
    out float4 out_position_clip : SV_Position,
    out float3 out_position : _Position,
    out float3 out_normal : _Normal,
    out float2 out_texcoords0 : _Texcoords0,
    out float4 out_tangent : _Tangent
) {
    out_position_clip = mul(float4(position, 1.0), cbv_const.object_to_clip);
    out_position = mul(position, (float3x3)cbv_const.object_to_world);
    out_normal = normal;
    out_texcoords0 = texcoords0;
    out_tangent = tangent;
}

Texture2D srv_ao_texture : register(t0);
Texture2D srv_base_color_texture : register(t1);
Texture2D srv_metallic_roughness_texture : register(t2);
Texture2D srv_normal_texture : register(t3);

SamplerState sam_aniso : register(s0);

[RootSignature(root_signature)]
void psMeshPbr(
    float4 position_clip : SV_Position,
    float3 position : _Position,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    float4 tangent : _Tangent,
    out float4 out_color : SV_Target0
) {
    float3 n = normalize(normal);
    float3 color = abs(n) * srv_ao_texture.Sample(sam_aniso, texcoords0).rgb;
    out_color = float4(color, 1.0);
}

#elif defined(PSO_MESH_DEBUG)

#endif
