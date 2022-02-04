#define ROOT_SIGNATURE \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
    "CBV(b1, visibility = SHADER_VISIBILITY_VERTEX)"

struct DrawConst {
    float4x4 object_to_world;
};
ConstantBuffer<DrawConst> cbv_draw_const : register(b0);

struct FrameConst {
    float4x4 world_to_clip;
};
ConstantBuffer<FrameConst> cbv_frame_const : register(b1);

[RootSignature(ROOT_SIGNATURE)]
void vsMain(
    float3 position : POSITION,
    float3 color : _Color,
    out float4 out_position_sv : SV_Position,
    out float3 out_color : _Color
) {
    const float4x4 object_to_clip = mul(cbv_draw_const.object_to_world, cbv_frame_const.world_to_clip);
    out_position_sv = mul(float4(position, 1.0), object_to_clip);
    out_color = color;
}

[RootSignature(ROOT_SIGNATURE)]
void psMain(
    float4 position_window : SV_Position,
    float3 color : _Color,
    out float4 out_color : SV_Target0
) {
    out_color = float4(pow(color, 1.0 / 2.2), 1.0);
}
