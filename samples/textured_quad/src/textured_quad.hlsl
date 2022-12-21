#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT, visibility = SHADER_VISIBILITY_PIXEL)"

[RootSignature(root_signature)]
void vsMain(
    float3 position : POSITION,
    float2 uv : _Texcoords0,
    out float4 out_position : SV_Position,
    out float2 out_uv : _Texcoords0
) {
    out_position = float4(position, 1.0);
    out_uv = uv;
}

Texture2D srv_t0 : register(t0);
SamplerState sam_s0 : register(s0);

[RootSignature(root_signature)]
void psMain(
    float4 position : SV_Position,
    float2 uv : _Texcoords0,
    out float4 out_color : SV_Target0
) {
    out_color = srv_t0.Sample(sam_s0, uv);
}
