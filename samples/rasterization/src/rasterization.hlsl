#define COMPUTE_GROUP_SZIE 32

struct Pixel {
    float2 position;
    float3 color;
};

#if defined(PSO__RECORD_PIXELS)

#define ROOT_SIGNATURE \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED), " \
    "CBV(b0), " \
    "CBV(b1), " \
    "DescriptorTable(UAV(u0), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(s0, filter = FILTER_ANISOTROPIC, maxAnisotropy = 16, visibility = SHADER_VISIBILITY_PIXEL)"

struct DrawConst {
    float4x4 object_to_world;
};
ConstantBuffer<DrawConst> cbv_draw_const : register(b0);

struct FrameConst {
    float4x4 world_to_clip;
    float3 camera_position;
};
ConstantBuffer<FrameConst> cbv_frame_const : register(b1);

RWStructuredBuffer<Pixel> uav_pixels : register(u0);

SamplerState sam_aniso : register(s0);

[RootSignature(ROOT_SIGNATURE)]
void vsRecordPixels(
    float3 position : POSITION,
    float3 normal : _Normal,
    float2 texcoord : _Texcoord,
    float4 tangent : _Tangent,
    out float4 out_position_sv : SV_Position,
    out float3 out_position : _Position,
    out float3 out_normal : _Normal,
    out float2 out_texcoord : _Texcoord,
    out float4 out_tangent : _Tangent
) {
    const float4x4 object_to_clip = mul(cbv_draw_const.object_to_world, cbv_frame_const.world_to_clip);
    out_position_sv = mul(float4(position, 1.0), object_to_clip);
    out_position = mul(float4(position, 1.0), cbv_draw_const.object_to_world).xyz;
    out_normal = normal;
    out_texcoord = texcoord;
    out_tangent = tangent;
}

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

[earlydepthstencil]
[RootSignature(ROOT_SIGNATURE)]
void psRecordPixels(
    float4 position_window : SV_Position,
    float3 position : _Position,
    float3 normal : _Normal,
    float2 texcoord : _Texcoord,
    float4 tangent : _Tangent
) {
    Texture2D srv_ao = ResourceDescriptorHeap[0];
    Texture2D srv_base_color = ResourceDescriptorHeap[1];
    Texture2D srv_metallic_roughness = ResourceDescriptorHeap[2];
    Texture2D srv_normal = ResourceDescriptorHeap[3];

    const float3 v = normalize(cbv_frame_const.camera_position - position);

    const float3 ao = srv_ao.Sample(sam_aniso, texcoord).rgb;
    const float3 base_color = pow(srv_base_color.Sample(sam_aniso, texcoord).rgb, 2.2);

    float3 n = normalize(srv_normal.Sample(sam_aniso, texcoord).rgb * 2.0 - 1.0);
    {
        normal = normalize(normal);
        tangent.xyz = normalize(tangent.xyz);
        const float3 bitangent = normalize(cross(normal, tangent.xyz)) * tangent.w;
        const float3x3 object_to_world = (float3x3)cbv_draw_const.object_to_world;
        n = mul(n, float3x3(tangent.xyz, bitangent, normal));
        n = normalize(mul(n, object_to_world));
    }

    float metallic;
    float roughness;
    {
        const float2 mr = srv_metallic_roughness.Sample(sam_aniso, texcoord).bg;
        metallic = mr.r;
        roughness = mr.g;
    }

    float alpha = roughness * roughness;
    float k = alpha + 1.0;
    k = (k * k) / 8.0;
    float3 f0 = float3(0.04, 0.04, 0.04);
    f0 = lerp(f0, base_color, metallic);

    const float3 light_positions[4] = {
        float3(20.0, 10.0, 20.0),
        float3(-20.0, 10.0, 20.0),
        float3(20.0, 10.0, -20.0),
        float3(-20.0, 10.0, -20.0),
    };
    const float3 light_radiance[4] = {
        8.0 * float3(200.0, 150.0, 200.0),
        8.0 * float3(200.0, 150.0, 200.0),
        8.0 * float3(200.0, 150.0, 200.0),
        8.0 * float3(200.0, 150.0, 200.0),
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

    const uint index = uav_pixels.IncrementCounter();
    uav_pixels[index].position = position_window.xy;
    uav_pixels[index].color = color;
}

#elif defined(PSO__DRAW_PIXELS)

#define ROOT_SIGNATURE \
    "DescriptorTable(SRV(t0), UAV(u0))"

StructuredBuffer<Pixel> srv_pixels : register(t0);
RWTexture2D<float4> uav_pixels : register(u0);

[RootSignature(ROOT_SIGNATURE)]
[numthreads(COMPUTE_GROUP_SZIE, 1, 1)]
void csDrawPixels(
    uint3 dispatch_id : SV_DispatchThreadID
) {
    const Pixel pixel = srv_pixels[dispatch_id.x];
    uav_pixels[uint2(pixel.position)] = float4(pixel.color, 1.0);
}

#elif defined(PSO__CLEAR_PIXELS)

#define ROOT_SIGNATURE \
    "DescriptorTable(UAV(u0))"

RWStructuredBuffer<Pixel> uav_pixels : register(u0);

[RootSignature(ROOT_SIGNATURE)]
[numthreads(COMPUTE_GROUP_SZIE, 1, 1)]
void csClearPixels(
    uint3 dispatch_id : SV_DispatchThreadID
) {
    uav_pixels[dispatch_id.x].position = 0.0;
    uav_pixels[dispatch_id.x].color = 0.0;
}

#elif defined(PSO__DRAW_MESH)

#define ROOT_SIGNATURE \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
    "CBV(b1, visibility = SHADER_VISIBILITY_VERTEX)"

struct DrawConst {
    float4x4 object_to_world;
};
ConstantBuffer<DrawConst> cbv_draw_const : register(b0);

struct FrameConst {
    float4x4 world_to_clip;
};
ConstantBuffer<FrameConst> cbv_frame_const : register(b1);

[RootSignature(ROOT_SIGNATURE)]
void vsDrawMesh(
    float3 position : POSITION,
    out float4 out_position_sv : SV_Position
) {
    const float4x4 object_to_clip = mul(cbv_draw_const.object_to_world, cbv_frame_const.world_to_clip);
    out_position_sv = mul(float4(position, 1.0), object_to_clip);
}

[RootSignature(ROOT_SIGNATURE)]
void psDrawMesh(
    float4 position_window : SV_Position,
    out float4 out_color : SV_Target0
) {
    out_color = 1.0;
}

#endif
