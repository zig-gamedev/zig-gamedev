#define ROOT_SIGNATURE \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)"

[RootSignature(ROOT_SIGNATURE)]
void vsMain(
    float3 position : POSITION,
    uint color : _Color,
    out float4 out_position_sv : SV_Position,
    out float3 out_color : _Color
) {
    out_position_sv = float4(position, 1.0);
    out_color = float3(
        (color & 0xff) / 255.0,
        ((color >> 8) & 0xff) / 255.0,
        ((color >> 16) & 0xff) / 255.0
    );
}

[RootSignature(ROOT_SIGNATURE)]
void psMain(
    float4 position_window : SV_Position,
    float3 color : _Color,
    out float4 out_color : SV_Target0
) {
    out_color = float4(color, 1.0);
}
