struct InputVertex {
    float3 position;
    float3 normal;
};

struct Vertex {
    float4 position_sv : SV_Position;
    float3 position : _Position;
    float3 normal : _Normal;
    float3 color : _Color;
};

struct DrawConst {
    float4x4 object_to_world;
    float4 base_color_roughness;
};

struct FrameConst {
    float4x4 world_to_clip;
    float3 camera_position;
};

ConstantBuffer<DrawConst> cbv_draw_const : register(b1);
ConstantBuffer<FrameConst> cbv_frame_const : register(b2);

StructuredBuffer<InputVertex> srv_vertices : register(t0);
Buffer<uint> srv_indices : register(t1);

#define ROOT_SIGNATURE_VS \
    "RootConstants(b0, num32BitConstants = 2, visibility = SHADER_VISIBILITY_VERTEX), " \
    "CBV(b1), " \
    "CBV(b2), " \
    "DescriptorTable(SRV(t0, numDescriptors = 2), visibility = SHADER_VISIBILITY_VERTEX)"

#define ROOT_SIGNATURE_MS \
    "RootConstants(b0, num32BitConstants = 2, visibility = SHADER_VISIBILITY_MESH), " \
    "CBV(b1), " \
    "CBV(b2), " \
    "DescriptorTable(SRV(t0, numDescriptors = 4), visibility = SHADER_VISIBILITY_MESH)"

#if defined(PSO__VERTEX_SHADER)
#define ROOT_SIGNATURE ROOT_SIGNATURE_VS
#elif defined(PSO__MESH_SHADER)
#define ROOT_SIGNATURE ROOT_SIGNATURE_MS
#endif

#if defined(PSO__MESH_SHADER)

#define NUM_THREADS 32
#define MAX_NUM_VERTICES 64
#define MAX_NUM_PRIMITIVES 128

struct RootConst {
    uint vertex_offset;
    uint meshlet_offset;
};

ConstantBuffer<RootConst> cbv_root_const : register(b0);

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
    out indices uint3 out_triangles[MAX_NUM_PRIMITIVES]
) {
    const uint thread_index = group_index;
    const uint meshlet_index = group_id.x + cbv_root_const.meshlet_offset;

    const uint64_t offset_vertices_triangles = srv_meshlets[meshlet_index];
    const uint data_offset = (uint)offset_vertices_triangles;
    const uint num_vertices = (uint)((offset_vertices_triangles >> 32) & 0xffff);
    const uint num_triangles = (uint)((offset_vertices_triangles >> 48) & 0xffff);

    const uint vertex_offset = data_offset;
    const uint index_offset = data_offset + num_vertices;

    const float4x4 object_to_world = cbv_draw_const.object_to_world;
    const float4x4 world_to_clip = cbv_frame_const.world_to_clip;

    SetMeshOutputCounts(num_vertices, num_triangles);

    const uint hash = computeHash(meshlet_index);
    float3 color = float3(hash & 0xff, (hash >> 8) & 0xff, (hash >> 16) & 0xff) / 255.0;

    uint i;
    for (i = thread_index; i < num_vertices; i += NUM_THREADS) {
        const uint vertex_index = srv_meshlets_data[vertex_offset + i] + cbv_root_const.vertex_offset;

        float4 position = float4(srv_vertices[vertex_index].position, 1.0);

        position = mul(position, object_to_world);
        out_vertices[i].position = position.xyz;

        position = mul(position, world_to_clip);
        out_vertices[i].position_sv = position;
        out_vertices[i].normal = mul(srv_vertices[vertex_index].normal, (float3x3)object_to_world);
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
    out_vertex.color = 1.0;
}

#endif

#define PI 3.1415926

// Trowbridge-Reitz GGX normal distribution function.
float distributionGgx(float3 n, float3 h, float alpha) {
    float alpha_sq = alpha * alpha;
    float n_dot_h = saturate(dot(n, h));
    float k = n_dot_h * n_dot_h * (alpha_sq - 1.0) + 1.0;
    return alpha_sq / (PI * k * k);
}

float geometrySchlickGgx(float x, float k) {
    return x / (x * (1.0 - k) + k);
}

float geometrySmith(float3 n, float3 v, float3 l, float k) {
    float n_dot_v = saturate(dot(n, v));
    float n_dot_l = saturate(dot(n, l));
    return geometrySchlickGgx(n_dot_v, k) * geometrySchlickGgx(n_dot_l, k);
}

float3 fresnelSchlick(float h_dot_v, float3 f0) {
    return f0 + (1.0 - f0) * pow(1.0 - h_dot_v, 5.0);
}

[RootSignature(ROOT_SIGNATURE)]
void psMain(
    Vertex vertex,
    out float4 out_color : SV_Target0
) {
    float3 v = normalize(cbv_frame_const.camera_position - vertex.position);
    float3 n = normalize(vertex.normal);

    float3 base_color = cbv_draw_const.base_color_roughness.rgb;
    float roughness = cbv_draw_const.base_color_roughness.a;
    float metallic = roughness < 0.0 ? 1.0 : 0.0;
    float ao = 1.0;
    roughness = abs(roughness);

    float alpha = roughness * roughness;
    float k = alpha + 1.0;
    k = (k * k) / 8.0;
    float3 f0 = 0.04;
    f0 = lerp(f0, base_color, metallic);

    const float3 light_positions[4] = {
        float3(0.0, 0.0, 15.0),
        float3(0.0, 0.0, -25.0),
        float3(25.0, 25.0, -25.0),
        float3(-25.0, 15.0, -25.0),
    };
    const float3 light_radiance[4] = {
        4.0 * float3(150.0, 75.0, 0.0),
        10.0 * float3(150.0, 150.0, 150.0),
        4.0 * float3(150.0, 75.0, 0.0),
        10.0 * float3(150.0, 150.0, 150.0),
    };

    float3 lo = 0.0;
    for (int light_index = 0; light_index < 4; ++light_index) {
        float3 lvec = light_positions[light_index] - vertex.position;

        float3 l = normalize(lvec);
        float3 h = normalize(l + v);

        float distance_sq = dot(lvec, lvec);
        float attenuation = 1.0 / distance_sq;
        float3 radiance = light_radiance[light_index] * attenuation;

        float3 f = fresnelSchlick(saturate(dot(h, v)), f0);

        float ndf = distributionGgx(n, h, alpha);
        float g = geometrySmith(n, v, l, k);

        float3 numerator = ndf * g * f;
        float denominator = 4.0 * saturate(dot(n, v)) * saturate(dot(n, l));
        float3 specular = numerator / max(denominator, 0.001);

        float3 ks = f;
        float3 kd = 1.0 - ks;
        kd *= 1.0 - metallic;

        float n_dot_l = saturate(dot(n, l));
        lo += (kd * base_color / PI + specular) * radiance * n_dot_l;
    }

    float3 ambient = 0.03 * base_color * ao;
    float3 color = ambient + lo;
    color = color / (color + 1.0);
    color = pow(color, 1.0 / 2.2);

    out_color = float4(color * vertex.color, 1.0);
}
