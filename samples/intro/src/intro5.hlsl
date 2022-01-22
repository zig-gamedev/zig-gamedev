#define ROOT_SIGNATURE \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX)"

struct FrameConst {
    float4x4 world_to_clip;
};
ConstantBuffer<FrameConst> cbv_frame_const : register(b0);

[RootSignature(ROOT_SIGNATURE)]
void vsMain(
    float3 position : POSITION,
    out float4 out_position_sv : SV_Position
) {
    out_position_sv = mul(float4(position, 1.0), cbv_frame_const.world_to_clip);
}

[RootSignature(ROOT_SIGNATURE)]
void psMain(
    float4 position_window : SV_Position,
    out float4 out_color : SV_Target0
) {
    out_color = float4(0.0, 0.9, 0.0, 1.0);
}
