#define root_signature "RootFlags(0)"

[RootSignature(root_signature)]
void vsMain(
    uint vertex_id : SV_VertexID,
    out float4 out_position : SV_Position
) {
    const float2 verts[] = { float2(-0.9, -0.9), float2(0.0, 0.9), float2(0.9, -0.9) };
    out_position = float4(verts[vertex_id], 0.0, 1.0);
}

[RootSignature(root_signature)]
void psMain(
    float4 position : SV_Position,
    out float4 out_color : SV_Target0
) {
    out_color = float4(0.75, 0.0, 0.0, 1.0);
}
