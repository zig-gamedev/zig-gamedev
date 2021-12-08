struct InputVertex {
    float3 position;
    float3 normal;
};

struct Vertex {
    float4 position_sv : SV_Position;
    float3 position : _Position;
    float3 normal : _Normal;
};

struct DrawConst {
    float4x4 object_to_world;
};

struct FrameConst {
    float4x4 world_to_clip;
};

ConstantBuffer<DrawConst> cbv_draw_const : register(b1);
ConstantBuffer<FrameConst> cbv_frame_const : register(b2);

StructuredBuffer<InputVertex> srv_vertices : register(t0);
Buffer<uint> srv_indices : register(t1);

#define ROOT_SIGNATURE_VS \
    "RootConstants(b0, num32BitConstants = 2, visibility = SHADER_VISIBILITY_VERTEX), " \
    "CBV(b1, visibility = SHADER_VISIBILITY_VERTEX), " \
    "CBV(b2, visibility = SHADER_VISIBILITY_VERTEX), " \
    "DescriptorTable(SRV(t0, numDescriptors = 2), visibility = SHADER_VISIBILITY_VERTEX)"

#define ROOT_SIGNATURE_MS \
    "RootConstants(b0, num32BitConstants = 2, visibility = SHADER_VISIBILITY_MESH), " \
    "CBV(b1, visibility = SHADER_VISIBILITY_MESH), " \
    "CBV(b2, visibility = SHADER_VISIBILITY_MESH), " \
    "DescriptorTable(SRV(t0, numDescriptors = 4), visibility = SHADER_VISIBILITY_MESH)"

#if defined(PSO__VERTEX_SHADER)
#define ROOT_SIGNATURE ROOT_SIGNATURE_VS
#elif defined(PSO__MESH_SHADER)
#define ROOT_SIGNATURE ROOT_SIGNATURE_MS
#endif

#if defined(PSO__MESH_SHADER)

#define NUM_THREADS 32
#define MAX_NUM_VERTICES 64
#define MAX_NUM_PRIMITIVES 126

struct RootConst {
    uint meshlet_offset;
    uint vertex_offset;
};

ConstantBuffer<RootConst> cbv_root_const : register(b0);

Buffer<uint> srv_meshlets : register(t2);
Buffer<uint> srv_meshlets_data : register(t3);

[RootSignature(ROOT_SIGNATURE)]
[outputtopology("triangle")]
[numthreads(NUM_THREADS, 1, 1)]
void msMain(
    uint group_index : SV_GroupIndex,
    uint3 group_id : SV_GroupID,
    out vertices Vertex out_vertices[MAX_NUM_VERTICES],
    out indices uint3 out_triangles[MAX_NUM_PRIMITIVES]
) {
    const uint thread_index = group_index;
    const uint meshlet_index = group_id.x + cbv_root_const.meshlet_offset;

    const uint offset_vertices_triangles = srv_meshlets[meshlet_index];
    const uint data_offset = offset_vertices_triangles & 0x3ffff;
    const uint num_vertices = (offset_vertices_triangles >> 18) & 0x7f;
    const uint num_triangles = (offset_vertices_triangles >> 25) & 0x7f;

    const uint vertex_offset = data_offset;
    const uint index_offset = data_offset + num_vertices;

    const float4x4 object_to_world = cbv_draw_const.object_to_world;
    const float4x4 world_to_clip = cbv_frame_const.world_to_clip;

    SetMeshOutputCounts(num_vertices, num_triangles);

    uint i;
    for (i = thread_index; i < num_vertices; i += NUM_THREADS) {
        const uint vertex_index = srv_meshlets_data[vertex_offset + i] + cbv_root_const.vertex_offset;

        float4 position = float4(srv_vertices[vertex_index].position, 1.0);

        position = mul(position, object_to_world);
        out_vertices[i].position = position.xyz;

        position = mul(position, world_to_clip);
        out_vertices[i].position_sv = position;
        out_vertices[i].normal = mul(srv_vertices[vertex_index].normal, (float3x3)object_to_world);
    }

    for (i = thread_index; i < num_triangles; i += NUM_THREADS) {
        const uint prim = srv_meshlets_data[index_offset + i];
        out_triangles[i] = uint3(prim & 0x3ff, (prim >> 10) & 0x3ff, (prim >> 20) & 0x3ff);
    }
}

#elif defined(PSO__VERTEX_SHADER)

struct RootConst {
    uint index_offset;
    uint vertex_offset;
};

ConstantBuffer<RootConst> cbv_root_const : register(b0);

[RootSignature(ROOT_SIGNATURE)]
void vsMain(
    uint vertex_id : SV_VertexID,
    out Vertex out_vertex
) {
    const uint vertex_index = srv_indices[vertex_id + cbv_root_const.index_offset] + cbv_root_const.vertex_offset;

    float4 position = float4(srv_vertices[vertex_index].position, 1.0);

    const float4x4 object_to_world = cbv_draw_const.object_to_world;
    const float4x4 world_to_clip = cbv_frame_const.world_to_clip;

    position = mul(position, object_to_world);
    out_vertex.position = position.xyz;

    position = mul(position, world_to_clip);
    out_vertex.position_sv = position;
    out_vertex.normal = mul(srv_vertices[vertex_index].normal, (float3x3)object_to_world);
}

#endif

[RootSignature(ROOT_SIGNATURE)]
void psMain(
    Vertex vertex,
    out float4 out_color : SV_Target0
) {
    out_color = float4(0.0, 1.0, 0.0, 1.0);
}
