#define ROOT_SIGNATURE \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)"

[RootSignature(ROOT_SIGNATURE)]
void vsMain(
    float3 position : POSITION,
    uint vertex_id : SV_VertexID,
    out float4 out_position_sv : SV_Position,
    out float3 out_color : _Color
) {
    // Note, this is not efficient and not robust way of generating vertex colors.
    const float3 colors[3] = {
        float3(1.0, 0.0, 0.0),
        float3(0.0, 1.0, 0.0),
        float3(0.0, 0.0, 1.0),
    };
    out_position_sv = float4(position, 1.0);
    out_color = colors[vertex_id];
}

[RootSignature(ROOT_SIGNATURE)]
void psMain(
    float4 position_window : SV_Position,
    float3 color : _Color,
    out float4 out_color : SV_Target0
) {
    out_color = float4(color, 1.0);
}
