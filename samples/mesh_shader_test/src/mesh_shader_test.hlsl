struct InputVertex {
    float3 position : Position;
    float3 normal : _Normal;
};

struct Vertex {
    float4 position_sv : SV_Position;
    float3 color : _Color;
};

struct DrawConst {
    float4x4 object_to_clip;
};

ConstantBuffer<DrawConst> cbv_draw_const : register(b0);

StructuredBuffer<InputVertex> srv_vertices : register(t0);
Buffer<uint> srv_indices : register(t1);

#define ROOT_SIGNATURE_VS \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
    "RootConstants(b1, num32BitConstants = 2, visibility = SHADER_VISIBILITY_VERTEX), " \
    "DescriptorTable(SRV(t0, numDescriptors = 2), visibility = SHADER_VISIBILITY_VERTEX)"

#define ROOT_SIGNATURE_VS_FIXED \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX)"

#define ROOT_SIGNATURE_MS \
    "CBV(b0, visibility = SHADER_VISIBILITY_MESH), " \
    "RootConstants(b1, num32BitConstants = 2, visibility = SHADER_VISIBILITY_MESH), " \
    "DescriptorTable(SRV(t0, numDescriptors = 4), visibility = SHADER_VISIBILITY_MESH)"

#if defined(PSO__VERTEX_SHADER)
#define ROOT_SIGNATURE ROOT_SIGNATURE_VS
#elif defined(PSO__VERTEX_SHADER_FIXED)
#define ROOT_SIGNATURE ROOT_SIGNATURE_VS_FIXED
#elif defined(PSO__MESH_SHADER)
#define ROOT_SIGNATURE ROOT_SIGNATURE_MS
#endif

#if defined(PSO__MESH_SHADER)

#define NUM_THREADS 32
// Also need to change max_num_meshlet_vertices and max_num_meshlet_triangles in mesh_shader_test.zig
#define MAX_NUM_VERTICES 64
#define MAX_NUM_TRIANGLES 64

struct RootConst {
    uint vertex_offset;
    uint meshlet_offset;
};

ConstantBuffer<RootConst> cbv_root_const : register(b1);

StructuredBuffer<uint64_t> srv_meshlets : register(t2);
Buffer<uint> srv_meshlets_data : register(t3);

uint computeHash(uint a) {
    a = (a + 0x7ed55d16) + (a << 12);
    a = (a ^ 0xc761c23c) ^ (a >> 19);
    a = (a + 0x165667b1) + (a << 5);
    a = (a + 0xd3a2646c) ^ (a << 9);
    a = (a + 0xfd7046c5) + (a << 3);
    a = (a ^ 0xb55a4f09) ^ (a >> 16);
    return a;
}

[RootSignature(ROOT_SIGNATURE)]
[outputtopology("triangle")]
[numthreads(NUM_THREADS, 1, 1)]
void msMain(
    uint group_index : SV_GroupIndex,
    uint3 group_id : SV_GroupID,
    out vertices Vertex out_vertices[MAX_NUM_VERTICES],
    out indices uint3 out_triangles[MAX_NUM_TRIANGLES]
) {
    const uint thread_index = group_index;
    const uint meshlet_index = group_id.x + cbv_root_const.meshlet_offset;

    const uint64_t offset_vertices_triangles = srv_meshlets[meshlet_index];
    const uint data_offset = (uint)offset_vertices_triangles;
    const uint num_vertices = (uint)((offset_vertices_triangles >> 32) & 0xffff);
    const uint num_triangles = (uint)((offset_vertices_triangles >> 48) & 0xffff);

    const uint vertex_offset = data_offset;
    const uint index_offset = data_offset + num_vertices;

    const float4x4 object_to_clip = cbv_draw_const.object_to_clip;

    SetMeshOutputCounts(num_vertices, num_triangles);

    const uint hash = computeHash(meshlet_index);
    const float3 color = float3(hash & 0xff, (hash >> 8) & 0xff, (hash >> 16) & 0xff) / 255.0;

    uint i;
    for (i = thread_index; i < num_vertices; i += NUM_THREADS) {
        const uint vertex_index = srv_meshlets_data[vertex_offset + i] + cbv_root_const.vertex_offset;

        float4 position = float4(srv_vertices[vertex_index].position, 1.0);

        position = mul(position, object_to_clip);

        out_vertices[i].position_sv = position;
        out_vertices[i].color = color;
    }

    for (i = thread_index; i < num_triangles; i += NUM_THREADS) {
        const uint prim = srv_meshlets_data[index_offset + i];
        out_triangles[i] = uint3(prim & 0x3ff, (prim >> 10) & 0x3ff, (prim >> 20) & 0x3ff);
    }
}

#elif defined(PSO__VERTEX_SHADER)

struct RootConst {
    uint vertex_offset;
    uint index_offset;
};

ConstantBuffer<RootConst> cbv_root_const : register(b1);

[RootSignature(ROOT_SIGNATURE)]
void vsMain(
    uint vertex_id : SV_VertexID,
    out Vertex out_vertex
) {
    const uint vertex_index = srv_indices[vertex_id + cbv_root_const.index_offset] + cbv_root_const.vertex_offset;

    float4 position = float4(srv_vertices[vertex_index].position, 1.0);

    const float4x4 object_to_clip = cbv_draw_const.object_to_clip;

    position = mul(position, object_to_clip);

    out_vertex.position_sv = position;

    out_vertex.color = abs(srv_vertices[vertex_index].normal);
}

#elif defined(PSO__VERTEX_SHADER_FIXED)

[RootSignature(ROOT_SIGNATURE)]
void vsMain(
    InputVertex vertex,
    out Vertex out_vertex
) {
    float4 position = float4(vertex.position, 1.0);

    const float4x4 object_to_clip = cbv_draw_const.object_to_clip;

    position = mul(position, object_to_clip);

    out_vertex.position_sv = position;

    out_vertex.color = 1.0 - abs(vertex.normal);
}

#endif

[RootSignature(ROOT_SIGNATURE)]
void psMain(
    float3 barycentrics : SV_Barycentrics,
    Vertex vertex,
    out float4 out_color : SV_Target0
) {
    // wireframe
    float3 barys = barycentrics;
    const float3 deltas = fwidth(barys);
    const float3 smoothing = deltas * 1.0;
    const float3 thickness = deltas * 0.25;
    barys = smoothstep(thickness, thickness + smoothing, barys);
    float min_bary = min(barys.x, min(barys.y, barys.z));

    out_color = float4(min_bary * vertex.color, 1.0);
}
