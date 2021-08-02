#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)"

[RootSignature(root_signature)]
void vsTriangle(
    float3 position : POSITION,
    out float4 out_position : SV_Position
) {
    out_position = float4(position, 1.0);
}

[RootSignature(root_signature)]
void psTriangle(
    float4 position : SV_Position,
    out float4 out_color : SV_Target0
) {
    out_color = float4(1.0, 0.5, 0.0, 1.0);
}
