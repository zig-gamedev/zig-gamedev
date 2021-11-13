#if defined(PSO__SIMPLE_ENTITY_WITH_GS)
#define PSO__SIMPLE_ENTITY
#endif

#if defined(PSO__PHYSICS_DEBUG)

#define root_signature \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
    "SRV(t0, visibility = SHADER_VISIBILITY_VERTEX)"

struct Vertex {
    float3 position;
    uint color;
};

struct FrameConst {
    float4x4 world_to_clip;
};

ConstantBuffer<FrameConst> cbv_frame_const : register(b0);
StructuredBuffer<Vertex> srv_vertices : register(t0);

[RootSignature(root_signature)]
void vsPhysicsDebug(
    uint vertex_id : SV_VertexID,
    out float4 out_position_clip : SV_Position,
    out float3 out_color : _Color
) {
    const Vertex v = srv_vertices[vertex_id];

    out_position_clip = mul(float4(v.position, 1.0), cbv_frame_const.world_to_clip);
    out_color = float3(
        (v.color & 0xff) / 255.0,
        ((v.color >> 8) & 0xff) / 255.0,
        ((v.color >> 16) & 0xff) / 255.0
    );
}

[RootSignature(root_signature)]
void psPhysicsDebug(
    float4 position_window : SV_Position,
    float3 color : _Color,
    out float4 out_color : SV_Target0
) {
    out_color = float4(color, 1.0);
}

#elif defined(PSO__SIMPLE_ENTITY)

#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0), " \
    "CBV(b1)"

struct DrawConst {
    float4x4 object_to_world;
    float4 base_color_roughness;
};

struct FrameConst {
    float4x4 world_to_clip;
    float3 camera_position;
};

ConstantBuffer<DrawConst> cbv_draw_const : register(b0);
ConstantBuffer<FrameConst> cbv_frame_const : register(b1);

[RootSignature(root_signature)]
void vsSimpleEntity(
    float3 position : POSITION,
    float3 normal : _Normal,
    out float4 out_position_clip : SV_Position,
    out float3 out_position : _Position,
    out float3 out_normal : _Normal
) {
    const float4x4 object_to_clip = mul(cbv_draw_const.object_to_world, cbv_frame_const.world_to_clip);
    out_position_clip = mul(float4(position, 1.0), object_to_clip);
    out_position = mul(float4(position, 1.0), cbv_draw_const.object_to_world).xyz;
    out_normal = mul(normal, (float3x3)cbv_draw_const.object_to_world);
}

struct GsInput {
    float4 position_clip : SV_Position;
    float3 position : _Position;
    float3 normal : _Normal;
};

struct GsOutput {
    float4 position_clip : SV_Position;
    float3 position : _Position;
    float3 normal : _Normal;
    float3 barycentrics : _Barycentrics;
};

[RootSignature(root_signature)]
[maxvertexcount(3)]
void gsSimpleEntity(
    triangle GsInput input[3],
    inout TriangleStream<GsOutput> triangle_stream
) {
    for (int i = 0; i < 3; ++i) {
        GsOutput output;
        output.position_clip = input[i].position_clip;
        output.position = input[i].position;
        output.normal = input[i].normal;
        if (i == 0) output.barycentrics = float3(1.0, 0.0, 0.0);
        else if (i == 1) output.barycentrics = float3(0.0, 1.0, 0.0);
        else if (i == 2) output.barycentrics = float3(0.0, 0.0, 1.0);
        triangle_stream.Append(output);
    }
}

static const float g_wireframe_smoothing = 1.0;
static const float g_wireframe_thickness = 0.25;

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
    return f0 + (float3(1.0, 1.0, 1.0) - f0) * pow(1.0 - h_dot_v, 5.0);
}

[RootSignature(root_signature)]
void psSimpleEntity(
    float4 position_window : SV_Position,
    float3 position : _Position,
    float3 normal : _Normal,
#if defined(PSO__SIMPLE_ENTITY_WITH_GS)
    float3 barycentrics : _Barycentrics,
#else
    float3 barycentrics : SV_Barycentrics,
#endif
    out float4 out_color : SV_Target0
) {
    float3 v = normalize(cbv_frame_const.camera_position - position);
    float3 n = normalize(normal);

    float3 base_color = cbv_draw_const.base_color_roughness.rgb;
    float roughness = cbv_draw_const.base_color_roughness.a;
    float metallic = 0.0;
    float ao = 1.0;

    float alpha = roughness * roughness;
    float k = alpha + 1.0;
    k = (k * k) / 8.0;
    float3 f0 = float3(0.04, 0.04, 0.04);
    f0 = lerp(f0, base_color, metallic);

    const float3 light_positions[4] = {
        float3(25.0, 15.0, 25.0),
        float3(-25.0, 15.0, 25.0),
        float3(25.0, 15.0, -25.0),
        float3(-25.0, 15.0, -25.0),
    };
    const float3 light_radiance[4] = {
        4.0 * float3(0.0, 100.0, 250.0),
        8.0 * float3(200.0, 150.0, 250.0),
        3.0 * float3(200.0, 0.0, 0.0),
        9.0 * float3(200.0, 150.0, 0.0),
    };

    float3 lo = 0.0;
    for (int light_index = 0; light_index < 4; ++light_index) {
        float3 lvec = light_positions[light_index] - position;

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
        float3 kd = float3(1.0, 1.0, 1.0) - ks;
        kd *= 1.0 - metallic;

        float n_dot_l = saturate(dot(n, l));
        lo += (kd * base_color / PI + specular) * radiance * n_dot_l;
    }

    float3 ambient = float3(0.03, 0.03, 0.03) * base_color * ao;
    float3 color = ambient + lo;
    color = color / (color + 1.0);
    color = pow(color, 1.0 / 2.2);

    // wireframe
    float3 barys = barycentrics;
    barys.z = 1.0 - barys.x - barys.y;
    float3 deltas = fwidth(barys);
    float3 smoothing = deltas * g_wireframe_smoothing;
    float3 thickness = deltas * g_wireframe_thickness;
    barys = smoothstep(thickness, thickness + smoothing, barys);
    float min_bary = min(barys.x, min(barys.y, barys.z));

    out_color = float4(min_bary * color, 1.0);
}

#endif
