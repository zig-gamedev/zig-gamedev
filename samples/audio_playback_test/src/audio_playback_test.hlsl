#if defined(PSO__LINES)

#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)"

[RootSignature(root_signature)]
void vsLines(
    float2 position : POSITION,
    out float4 out_position_clip : SV_Position,
    out float2 out_position : _Position
) {
    out_position_clip = float4(position, 0.0, 1.0);
    out_position = position;
}

[RootSignature(root_signature)]
void psLines(
    float4 position_window : SV_Position,
    float2 position : _Position,
    out float4 out_color : SV_Target0
) {
    out_color = float4(1.0, 6.0 * abs(position.y), 0.0, 1.0);
}

#elif defined(PSO__IMAGE)

#define root_signature \
    "DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT, visibility = SHADER_VISIBILITY_PIXEL)"

[RootSignature(root_signature)]
void vsImage(
    uint vertex_id: SV_VertexID,
    out float4 out_position : SV_Position,
    out float2 out_uv : _Texcoords0
) {
    const float2 positions[3] = { float2(-1.0, -1.0), float2(-1.0, 3.0), float2(3.0, -1.0) };
    const float2 position = positions[vertex_id];

    out_position = float4(position, 0.0, 1.0);
    out_uv = 0.5 + 0.5 * position;
    out_uv = float2(out_uv.x, 1.0 - out_uv.y);
}

Texture2D srv_t0 : register(t0);
SamplerState sam_s0 : register(s0);

[RootSignature(root_signature)]
void psImage(
    float4 position_window : SV_Position,
    float2 uv : _Texcoords0,
    out float4 out_color : SV_Target0
) {
    float3 color = pow(srv_t0.Sample(sam_s0, uv).rgb, 1.5);
    out_color = float4(color, 1.0);
}

#endif
