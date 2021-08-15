#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)"

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

[RootSignature(root_signature)]
void psMain(
    float4 position : SV_Position,
    float2 uv : _Texcoords0,
    out float4 out_color : SV_Target0
) {
    out_color = float4(1.0, 0.5, 0.0, 1.0);
}
