#if defined(PSO__STATIC_MESH)

#define root_signature \
    "RootConstants(b0, num32BitConstants = 2), " \
    "CBV(b1), " \
    "DescriptorTable(SRV(t0, numDescriptors = 2), visibility = SHADER_VISIBILITY_VERTEX)"

struct DrawRootConst {
    uint vertex_offset;
    uint index_offset;
};
ConstantBuffer<DrawRootConst> cbv_draw_root : register(b0);

struct FrameConst {
    float4x4 object_to_clip;
};
ConstantBuffer<FrameConst> cbv_frame : register(b1);

struct Vertex {
    float3 position;
    float3 normal;
    float2 texcoords0;
    float4 tangent;
};
StructuredBuffer<Vertex> srv_vertex_buffer : register(t0);
Buffer<uint> srv_index_buffer : register(t1);

[RootSignature(root_signature)]
void vsStaticMesh(
    uint vertex_id : SV_VertexID,
    out float3 out_position_ndc : SV_Position
) {
    const uint vertex_index = srv_index_buffer[vertex_id + cbv_draw_root.index_offset] + cbv_draw_root.vertex_offset;
    const Vertex vertex = srv_vertex_buffer[vertex_index];

    out_position_ndc = mul(float4(vertex.position, 1.0), cbv_frame.object_to_clip);
}

[RootSignature(root_signature)]
void psStaticMesh(
    float4 position_ndc : SV_Position,
    out float4 out_color : SV_Target0
) {
    out_color = float4(1.0, 0.0, 0.0, 1.0);
}

#endif
