#define GAMMA 2.2
#define PI 3.1415926

static const float3 g_light_position = float3(0.0, 5.0, 0.0);

struct Vertex {
    float3 position;
    float3 normal;
    float2 texcoords0;
    float4 tangent;
};

#if defined(PSO__RAST_STATIC_MESH)

#define root_signature \
    "RootConstants(b0, num32BitConstants = 2), " \
    "DescriptorTable(SRV(t2, numDescriptors = 3), visibility = SHADER_VISIBILITY_PIXEL), " \
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
    float4x4 object_to_world;
    float3 camera_position;
};
ConstantBuffer<FrameConst> cbv_frame : register(b1);

StructuredBuffer<Vertex> srv_vertex_buffer : register(t0);
Buffer<uint> srv_index_buffer : register(t1);

[RootSignature(root_signature)]
void vsRastStaticMesh(
    uint vertex_id : SV_VertexID,
    out float4 out_position_ndc : SV_Position,
    out float3 out_position : _Position,
    out float3 out_normal : _Normal,
    out float2 out_texcoords0 : _Texcoords0,
    out float4 out_tangent : _Tangent
) {
    const uint vertex_index = srv_index_buffer[vertex_id + cbv_draw_root.index_offset] + cbv_draw_root.vertex_offset;
    const Vertex vertex = srv_vertex_buffer[vertex_index];

    out_position_ndc = mul(float4(vertex.position, 1.0), cbv_frame.object_to_clip);
    out_position = mul(float4(vertex.position, 1.0), cbv_frame.object_to_world).xyz;
    out_normal = vertex.normal;
    out_texcoords0 = vertex.texcoords0;
    out_tangent = vertex.tangent;
}

Texture2D srv_base_color_texture : register(t2);
Texture2D srv_metallic_roughness_texture : register(t3);
Texture2D srv_normal_texture: register(t4);

SamplerState sam_aniso : register(s0);

float3 fresnelSchlick(float cos_theta, float3 f0) {
    return saturate(f0 + (1.0 - f0) * pow(1.0 - cos_theta, 5.0));
}

float distributionGgx(float3 n, float3 h, float roughness) {
    float alpha = roughness * roughness;
    float alpha_sq = alpha * alpha;
    float n_dot_h = dot(n, h);
    float n_dot_h_sq = n_dot_h * n_dot_h;
    float k = n_dot_h_sq * alpha_sq + (1.0 - n_dot_h_sq);
    return alpha_sq / (PI * k * k);
}

float geometrySchlickGgx(float cos_theta, float roughness) {
    float k = (roughness * roughness) * 0.5;
    return cos_theta / (cos_theta * (1.0 - k) + k);
}

// Geometry function returns probability [0.0, 1.0].
float geometrySmith(float n_dot_l, float n_dot_v, float roughness) {
    return saturate(geometrySchlickGgx(n_dot_v, roughness) * geometrySchlickGgx(n_dot_l, roughness));
}

[RootSignature(root_signature)]
void psRastStaticMesh(
    float4 position_ndc : SV_Position,
    float3 position : _Position,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    float4 tangent : _Tangent,
    out float4 out_color : SV_Target0
) {
    float3 n = normalize(srv_normal_texture.Sample(sam_aniso, texcoords0).rgb * 2.0 - 1.0);

    normal = normalize(normal);
    tangent.xyz = normalize(tangent.xyz);
    const float3 bitangent = normalize(cross(normal, tangent.xyz)) * tangent.w;

    n = mul(n, float3x3(tangent.xyz, bitangent, normal));
    n = normalize(mul(n, (float3x3)cbv_frame.object_to_world));

    float3 base_color = pow(srv_base_color_texture.Sample(sam_aniso, texcoords0).rgb, GAMMA);

    float metallic;
    float roughness;
    float ao;
    {
        const float3 rgb = srv_metallic_roughness_texture.Sample(sam_aniso, texcoords0).rgb;
        roughness = rgb.g;
        metallic = rgb.b;
        ao = rgb.r;
    }

    const float3 v = normalize(cbv_frame.camera_position - position);
    const float n_dot_v = saturate(dot(n, v));

    float3 f0 = float3(0.04, 0.04, 0.04);
    f0 = lerp(f0, base_color, metallic);

    float3 lo = 0.0;

    // Light contribution.
    {
        const float3 l_radiance = float3(70.0, 70.0, 50.0);
        const float3 l_vec = g_light_position - position;
        const float3 l = normalize(l_vec);

        float3 h = normalize(l + v);
        float n_dot_l = saturate(dot(n, l));
        float h_dot_v = saturate(dot(h, v));

        float attenuation = max(1.0 / dot(l_vec, l_vec), 0.001);
        float3 radiance = l_radiance * attenuation;

        float3 f = fresnelSchlick(h_dot_v, f0);
        float nd = distributionGgx(n, h, roughness);
        float g = geometrySmith(n_dot_l, n_dot_v, (roughness + 1.0) * 0.5);

        float3 specular = (nd * g * f) / max(4.0 * n_dot_v * n_dot_l, 0.001);

        float3 kd = (1.0 - f) * (1.0 - metallic);

        lo += (kd * (base_color / PI) + specular) * radiance * n_dot_l;
    }

    const float3 ambient = 0.05 * base_color * ao;

    float3 color = ambient + lo;

    color = color / (color + 1.0);
    color = pow(color, 1.0 / GAMMA);

    out_color = float4(color, 1.0);
}

#elif defined(PSO__Z_PRE_PASS)

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

StructuredBuffer<Vertex> srv_vertex_buffer : register(t0);
Buffer<uint> srv_index_buffer : register(t1);

[RootSignature(root_signature)]
void vsZPrePass(
    uint vertex_id : SV_VertexID,
    out float4 out_position_ndc : SV_Position
) {
    const uint vertex_index = srv_index_buffer[vertex_id + cbv_draw_root.index_offset] + cbv_draw_root.vertex_offset;
    const Vertex vertex = srv_vertex_buffer[vertex_index];

    out_position_ndc = mul(float4(vertex.position, 1.0), cbv_frame.object_to_clip);
}

[RootSignature(root_signature)]
void psZPrePass(
    float4 position_ndc : SV_Position
) {
}

#elif defined(PSO__GEN_SHADOW_RAYS)

#define root_signature \
    "RootConstants(b0, num32BitConstants = 2), " \
    "CBV(b1), " \
    "DescriptorTable(SRV(t0, numDescriptors = 2), visibility = SHADER_VISIBILITY_VERTEX), " \
    "DescriptorTable(UAV(u0), visibility = SHADER_VISIBILITY_PIXEL)"

struct DrawRootConst {
    uint vertex_offset;
    uint index_offset;
};
ConstantBuffer<DrawRootConst> cbv_draw_root : register(b0);

struct FrameConst {
    float4x4 object_to_clip;
    float4x4 object_to_world;
};
ConstantBuffer<FrameConst> cbv_frame : register(b1);

StructuredBuffer<Vertex> srv_vertex_buffer : register(t0);
Buffer<uint> srv_index_buffer : register(t1);

RWTexture2D<float3> uav_shadow_rays : register(u0);

[RootSignature(root_signature)]
void vsGenShadowRays(
    uint vertex_id : SV_VertexID,
    out float4 out_position_ndc : SV_Position,
    out float3 out_position : _Position,
    out float3 out_normal : _Normal
) {
    const uint vertex_index = srv_index_buffer[vertex_id + cbv_draw_root.index_offset] + cbv_draw_root.vertex_offset;
    const Vertex vertex = srv_vertex_buffer[vertex_index];

    out_position_ndc = mul(float4(vertex.position, 1.0), cbv_frame.object_to_clip);
    out_position = mul(float4(vertex.position, 1.0), cbv_frame.object_to_world).xyz;
    out_normal = mul(vertex.normal, (float3x3)cbv_frame.object_to_world);
}

[earlydepthstencil]
[RootSignature(root_signature)]
void psGenShadowRays(
    float4 position_screen : SV_Position,
    float3 position : _Position,
    float3 normal : _Normal
) {
    float3 n = normalize(normal);
    float3 ro = position + 0.005 * n;
    uav_shadow_rays[position_screen.xy] = ro;
}

#elif defined(PSO__TRACE_SHADOW_RAYS)

RaytracingShaderConfig g_shader_config = {
	4, // MaxPayloadSizeInBytes
	4, // MaxAttributeSizeInBytes
};

RaytracingPipelineConfig g_pipeline_config = {
    1, // MaxTraceRecursionDepth
};

GlobalRootSignature g_global_signature = {
    "SRV(t0),"
    "DescriptorTable(SRV(t1), UAV(u0)),"
};

RaytracingAccelerationStructure srv_bvh : register(t0);
Texture2D<float3> srv_shadow_rays : register(t1);

RWTexture2D<float> uav_shadow_mask : register(u0);

struct Payload {
    float mask;
};

[shader("raygeneration")]
void generateShadowRay() {
    float3 ro = srv_shadow_rays[DispatchRaysIndex().xy];

    RayDesc ray;
    ray.Origin = ro;
    ray.Direction = g_light_position - ro;
    ray.TMin = 0.0;
    ray.TMax = 100.0;

    Payload payload;
    payload.mask = 1.0;

    TraceRay(
        srv_bvh,
        RAY_FLAG_ACCEPT_FIRST_HIT_AND_END_SEARCH,
        1, // InstanceInclusionMask
        0, // RayContributionToHitGroupIndex
        0, // MultiplierForGeometryContributionToHitGroupIndex
        0, // MissShaderIndex
        ray,
        payload
    );

    uav_shadow_mask[DispatchRaysIndex().xy] = payload.mask;
}

[shader("miss")]
void shadowMiss(inout Payload payload) {
	payload.mask = 0.0;
}

#endif
