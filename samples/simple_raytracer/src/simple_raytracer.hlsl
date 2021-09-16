#define GAMMA 2.2

#if defined(PSO__RAST_STATIC_MESH)

#define root_signature \
    "RootConstants(b0, num32BitConstants = 2), " \
    "DescriptorTable(SRV(t2, numDescriptors = 1), visibility = SHADER_VISIBILITY_PIXEL), " \
    "CBV(b1), " \
    "DescriptorTable(SRV(t0, numDescriptors = 2), visibility = SHADER_VISIBILITY_VERTEX), " \
    "StaticSampler(s0, filter = FILTER_ANISOTROPIC, maxAnisotropy = 16, visibility = SHADER_VISIBILITY_PIXEL)"

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
void vsRastStaticMesh(
    uint vertex_id : SV_VertexID,
    out float4 out_position_ndc : SV_Position,
    out float2 out_texcoords0 : _Texcoords0
) {
    const uint vertex_index = srv_index_buffer[vertex_id + cbv_draw_root.index_offset] + cbv_draw_root.vertex_offset;
    const Vertex vertex = srv_vertex_buffer[vertex_index];

    out_position_ndc = mul(float4(0.008 * vertex.position, 1.0), cbv_frame.object_to_clip);
    out_texcoords0 = vertex.texcoords0;
}

Texture2D srv_base_color_texture : register(t2);
SamplerState sam_aniso : register(s0);

[RootSignature(root_signature)]
void psRastStaticMesh(
    float4 position_ndc : SV_Position,
    float2 texcoords0 : _Texcoords0,
    out float4 out_color : SV_Target0
) {
    float3 color = srv_base_color_texture.Sample(sam_aniso, texcoords0).rgb;
    out_color = float4(color, 1.0);
}

#endif
