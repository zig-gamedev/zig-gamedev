#if defined(PSO__LINES)

#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)"

[RootSignature(root_signature)]
void vsLines(
    float2 position : POSITION,
    out float4 out_position_clip : SV_Position
) {
    out_position_clip = float4(position, 0.0, 1.0);
}

[RootSignature(root_signature)]
void psLines(
    float4 position_window : SV_Position,
    out float4 out_color : SV_Target0
) {
    out_color = float4(1.0, 0.5, 0.0, 1.0);
}

#endif
