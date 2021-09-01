#if defined(PSO_MESH_PBR)

#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX)"
    //"DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL), " \
    //"StaticSampler(s0, filter = FILTER_ANISOTROPIC, maxAnisotropy = 16, visibility = SHADER_VISIBILITY_PIXEL)"

struct Const {
    float4x4 object_to_clip;
};
ConstantBuffer<Const> cbv_const : register(b0);

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
    out_position = mul(float4(position, 1.0), cbv_const.object_to_clip);
    out_normal = normal;
    out_texcoords0 = texcoords0;
    out_tangent = tangent;
}

Texture2D srv_t0 : register(t0);
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
    float3 color = abs(n);// * srv_t0.Sample(sam_s0, texcoords0).rgb;
    out_color = float4(color, 1.0);
}

#elif defined(PSO_MESH_DEBUG)

#endif
