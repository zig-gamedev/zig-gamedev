#define ROOT_SIGNATURE \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
    "CBV(b1, visibility = SHADER_VISIBILITY_VERTEX), " \
    "DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(s0, filter = FILTER_ANISOTROPIC, maxAnisotropy = 16, visibility = SHADER_VISIBILITY_PIXEL)"

struct DrawConst {
    float4x4 object_to_world;
};
ConstantBuffer<DrawConst> cbv_draw_const : register(b0);

struct FrameConst {
    float4x4 world_to_clip;
};
ConstantBuffer<FrameConst> cbv_frame_const : register(b1);

Texture2D srv_ao_texture : register(t0);
SamplerState sam_aniso : register(s0);

[RootSignature(ROOT_SIGNATURE)]
void vsMain(
    float3 position : POSITION,
    float3 normal : _Normal,
    float2 texcoord : _Texcoord,
    out float4 out_position_sv : SV_Position,
    out float2 out_texcoord : _Texcoord
) {
    const float4x4 object_to_clip = mul(cbv_draw_const.object_to_world, cbv_frame_const.world_to_clip);
    out_position_sv = mul(float4(position, 1.0), object_to_clip);
    out_texcoord = texcoord;
}

[RootSignature(ROOT_SIGNATURE)]
void psMain(
    float4 position_window : SV_Position,
    float2 texcoord : _Texcoord,
    out float4 out_color : SV_Target0
) {
    out_color = float4(srv_ao_texture.Sample(sam_aniso, texcoord).rgb, 1.0);
}
