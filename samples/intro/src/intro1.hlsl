#define ROOT_SIGNATURE \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)"

[RootSignature(ROOT_SIGNATURE)]
void vsMain(
    float3 position : POSITION,
    out float4 out_position_sv : SV_Position
) {
    out_position_sv = float4(position, 1.0);
}

[RootSignature(ROOT_SIGNATURE)]
void psMain(
    float4 position_window : SV_Position,
    out float4 out_color : SV_Target0
) {
    out_color = float4(0.0, 0.8, 0.0, 1.0);
}
