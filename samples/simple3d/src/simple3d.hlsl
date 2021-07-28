#define root_signature "RootFlags(0)"

[RootSignature(root_signature)]
void vsTriangle(
    uint vertex_id : SV_VertexID,
    out float4 out_position : SV_Position
) {
    const float2 positions[] = { float2(-1.0, -1.0), float2(0.0, 1.0), float2(1.0, -1.0) };
    out_position = float4(positions[vertex_id], 0.0, 1.0);
}

[RootSignature(root_signature)]
void psTriangle(
    float4 position : SV_Position,
    out float4 out_color : SV_Target0
) {
    out_color = float4(0.0, 0.5, 0.0, 1.0);
}
