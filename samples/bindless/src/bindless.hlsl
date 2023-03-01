#define PI 3.1415926

#define GAMMA 2.2

float radicalInverseVdc(uint bits) {
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return (float)bits * 2.3283064365386963e-10; // / 0x100000000
}

float2 hammersley(uint idx, uint n) {
    return float2(idx / (float)n, radicalInverseVdc(idx));
}

float3 importanceSampleGgx(float2 xi, float roughness, float3 n) {
    const float alpha = roughness * roughness;
    const float phi = 2.0 * PI * xi.x;
    const float cos_theta = sqrt((1.0 - xi.y) / (1.0 + (alpha * alpha - 1.0) * xi.y));
    const float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    float3 h;
    h.x = sin_theta * cos(phi);
    h.y = sin_theta * sin(phi);
    h.z = cos_theta;

    const float3 up_vector = abs(n.y) < 0.999 ? float3(0.0, 1.0, 0.0) : float3(0.0, 0.0, 1.0);
    const float3 tangent_x = normalize(cross(up_vector, n));
    const float3 tangent_y = cross(n, tangent_x);

    // Tangent to world space.
    return normalize(tangent_x * h.x + tangent_y * h.y + n * h.z);
}

float geometrySchlickGgx(float cos_theta, float roughness) {
    const float k = (roughness * roughness) * 0.5;
    return cos_theta / (cos_theta * (1.0 - k) + k);
}

float geometrySmith(float n_dot_l, float n_dot_v, float roughness) {
    return geometrySchlickGgx(n_dot_v, roughness) * geometrySchlickGgx(n_dot_l, roughness);
}

#if defined(PSO__MESH_PBR)

#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED), " \
    "CBV(b0), " \
    "CBV(b1), " \
    "DescriptorTable(SRV(t0, numDescriptors = 3), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(s0, filter = FILTER_ANISOTROPIC, maxAnisotropy = 16, visibility = SHADER_VISIBILITY_PIXEL)"

struct SceneConst {
    float4x4 world_to_clip;
    float3 camera_position;
    int draw_mode;
};

struct DrawConst {
    float4x4 object_to_world;
    uint base_color_index;
    uint ao_index;
    uint metallic_roughness_index;
    uint normal_index;
};

ConstantBuffer<SceneConst> scene_const : register(b0);
ConstantBuffer<DrawConst> draw_const : register(b1);

[RootSignature(root_signature)]
void vsMeshPbr(
    float3 position : POSITION,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    float4 tangent : _Tangent,
    out float4 out_position_clip : SV_Position,
    out float3 out_position : _Position,
    out float3 out_normal : _Normal,
    out float2 out_texcoords0 : _Texcoords0,
    out float4 out_tangent : _Tangent
) {
    const float4x4 object_to_clip = mul(draw_const.object_to_world, scene_const.world_to_clip);
    out_position_clip = mul(float4(position, 1.0), object_to_clip);
    out_position = mul(position, (float3x3)draw_const.object_to_world);
    out_normal = normal;
    out_tangent = tangent;
    out_texcoords0 = texcoords0;
}

TextureCube srv_irradiance_texture : register(t0);
TextureCube srv_prefiltered_env_texture : register(t1);
Texture2D srv_brdf_integration_texture : register(t2);

SamplerState sam_aniso : register(s0);

float3 fresnelSchlickRoughness(float cos_theta, float3 f0, float roughness) {
    return f0 + (max(1.0 - roughness, f0) - f0) * pow(1.0 - cos_theta, 5.0);
}

[RootSignature(root_signature)]
void psMeshPbr(
    float4 position_clip : SV_Position,
    float3 position : _Position,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    float4 tangent : _Tangent,
    out float4 out_color : SV_Target0
) {
    Texture2D srv_ao_texture = ResourceDescriptorHeap[draw_const.ao_index];
    Texture2D srv_base_color_texture = ResourceDescriptorHeap[draw_const.base_color_index];
    Texture2D srv_metallic_roughness_texture = ResourceDescriptorHeap[draw_const.metallic_roughness_index];
    Texture2D srv_normal_texture = ResourceDescriptorHeap[draw_const.normal_index];

    if (scene_const.draw_mode == 1) {
        out_color = srv_ao_texture.Sample(sam_aniso, texcoords0).r;
        return;
    } else if (scene_const.draw_mode == 2) {
        out_color = srv_base_color_texture.Sample(sam_aniso, texcoords0);
        return;
    } else if (scene_const.draw_mode == 3) {
        out_color = srv_metallic_roughness_texture.Sample(sam_aniso, texcoords0).b;
        return;
    } else if (scene_const.draw_mode == 4) {
        out_color = srv_metallic_roughness_texture.Sample(sam_aniso, texcoords0).g;
        return;
    } else if (scene_const.draw_mode == 5) {
        float3 n = float3(srv_normal_texture.Sample(sam_aniso, texcoords0).rg, 0.0);
        n.z = sqrt(1.0 - saturate(dot(n.xy, n.xy)));
        out_color = float4(n, 1.0);
        return;
    }

    float3 n = float3(srv_normal_texture.Sample(sam_aniso, texcoords0).rg, 0.0);
    n.z = sqrt(1.0 - saturate(dot(n.xy, n.xy)));
    n = normalize(2.0 * n - 1.0);

    normal = normalize(normal);
    tangent.xyz = normalize(tangent.xyz);
    const float3 bitangent = normalize(cross(normal, tangent.xyz)) * tangent.w;

    const float3x3 object_to_world = (float3x3)draw_const.object_to_world;

    n = mul(n, float3x3(tangent.xyz, bitangent, normal));
    n = normalize(mul(n, object_to_world));

    float metallic;
    float roughness;
    {
        const float2 mr = srv_metallic_roughness_texture.Sample(sam_aniso, texcoords0).bg;
        metallic = mr.r;
        roughness = mr.g;
    }
    const float3 base_color = srv_base_color_texture.Sample(sam_aniso, texcoords0).rgb;
    const float ao = srv_ao_texture.Sample(sam_aniso, texcoords0).r;

    const float3 v = normalize(scene_const.camera_position - position);
    const float n_dot_v = saturate(dot(n, v));

    float3 f0 = float3(0.04, 0.04, 0.04);
    f0 = lerp(f0, base_color, metallic);

    const float3 r = reflect(-v, n);
    const float3 f = fresnelSchlickRoughness(n_dot_v, f0, roughness);

    const float3 kd = (1.0 - f) * (1.0 - metallic);

    const float3 irradiance = srv_irradiance_texture.SampleLevel(sam_aniso, n, 0.0).rgb;
    const float3 prefiltered_color = srv_prefiltered_env_texture.SampleLevel(
        sam_aniso,
        r,
        roughness * 5.0 // roughness * (num_mip_levels - 1.0)
    ).rgb;
    const float2 env_brdf = srv_brdf_integration_texture.SampleLevel(
        sam_aniso,
        float2(min(n_dot_v, 0.999), roughness),
        0.0
    ).rg;

    const float3 diffuse = irradiance * base_color;
    const float3 specular = prefiltered_color * (f * env_brdf.x + env_brdf.y);
    const float3 ambient = (kd * diffuse + specular) * ao;

    float3 color = ambient;
    color = color / (color + 1.0);

    out_color = float4(pow(color, 1.0 / GAMMA), 1.0);
}

#elif defined(PSO__GENERATE_ENV_TEXTURE)

#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
    "DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(s0, visibility = SHADER_VISIBILITY_PIXEL)"

struct Const {
    float4x4 object_to_clip;
};
ConstantBuffer<Const> cbv_const : register(b0);

Texture2D srv_equirect_texture : register(t0);
SamplerState sam_s0 : register(s0);

[RootSignature(root_signature)]
void vsGenerateEnvTexture(
    float3 position : POSITION,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    float4 tangent : _Tangent,
    out float4 out_position_clip : SV_Position,
    out float3 out_position : _Position
) {
    out_position_clip = mul(float4(position, 1.0), cbv_const.object_to_clip);
    out_position = position; // Position in object space.
}

float2 sampleSphericalMap(float3 v) {
    float2 uv = float2(atan2(v.z, v.x), asin(v.y));
    uv *= float2(0.1591, 0.3183);
    uv += 0.5;
    return uv;
}

[RootSignature(root_signature)]
void psGenerateEnvTexture(
    float4 position_ndc : SV_Position,
    float3 position : _Position,
    out float4 out_color : SV_Target0
) {
    const float2 uv = sampleSphericalMap(normalize(position));
    float3 color = srv_equirect_texture.SampleLevel(sam_s0, uv, 0).rgb;
    out_color = float4(color, 1.0);
}

#elif defined(PSO__SAMPLE_ENV_TEXTURE)

#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
    "DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(" \
    "   s0, " \
    "   filter = FILTER_MIN_MAG_MIP_LINEAR, " \
    "   visibility = SHADER_VISIBILITY_PIXEL, " \
    "   addressU = TEXTURE_ADDRESS_CLAMP, " \
    "   addressV = TEXTURE_ADDRESS_CLAMP, " \
    "   addressW = TEXTURE_ADDRESS_CLAMP" \
    ")"

struct Const {
    float4x4 object_to_clip;
};
ConstantBuffer<Const> cbv_const : register(b0);

TextureCube srv_env_texture : register(t0);
SamplerState sam_s0 : register(s0);

[RootSignature(root_signature)]
void vsSampleEnvTexture(
    float3 position : POSITION,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    float4 tangent : _Tangent,
    out float4 out_position_clip : SV_Position,
    out float3 out_uvw : _Uvw
) {
    out_position_clip = mul(float4(position, 1.0), cbv_const.object_to_clip).xyww;
    out_uvw = position;
}

[RootSignature(root_signature)]
void psSampleEnvTexture(
    float4 position_clip : SV_Position,
    float3 uvw : _Uvw,
    out float4 out_color : SV_Target0
) {
    float3 env_color = srv_env_texture.Sample(sam_s0, uvw).rgb;
    env_color = env_color / (env_color + 1.0);
    out_color = float4(pow(env_color, 1.0 / GAMMA), 1.0);
}

#elif defined(PSO__GENERATE_IRRADIANCE_TEXTURE)

#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
    "DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(" \
    "   s0, " \
    "   filter = FILTER_MIN_MAG_MIP_LINEAR, " \
    "   visibility = SHADER_VISIBILITY_PIXEL, " \
    "   addressU = TEXTURE_ADDRESS_CLAMP, " \
    "   addressV = TEXTURE_ADDRESS_CLAMP, " \
    "   addressW = TEXTURE_ADDRESS_CLAMP" \
    ")"

struct Const {
    float4x4 object_to_clip;
};
ConstantBuffer<Const> cbv_const : register(b0);

TextureCube srv_env_texture : register(t0);
SamplerState sam_s0 : register(s0);

[RootSignature(root_signature)]
void vsGenerateIrradianceTexture(
    float3 position : POSITION,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    float4 tangent : _Tangent,
    out float4 out_position_clip : SV_Position,
    out float3 out_position : _Position
) {
    out_position_clip = mul(float4(position, 1.0), cbv_const.object_to_clip);
    out_position = position;
}

[RootSignature(root_signature)]
void psGenerateIrradianceTexture(
    float4 position_clip : SV_Position,
    float3 position : _Position,
    out float4 out_color : SV_Target0
) {
    const float3 n = normalize(position);

    // This is Right-Handed coordinate system and works for upper-left UV coordinate systems.
    const float3 up_vector = abs(n.y) < 0.999 ? float3(0.0, 1.0, 0.0) : float3(0.0, 0.0, 1.0);
    const float3 tangent_x = normalize(cross(up_vector, n));
    const float3 tangent_y = normalize(cross(n, tangent_x));

    uint num_samples = 0;
    float3 irradiance = 0.0;

    for (float phi = 0.0; phi < (2.0 * PI); phi += 0.025) {
        for (float theta = 0.0; theta < (0.5 * PI); theta += 0.025) {
            // Point on a hemisphere.
            const float3 h = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));

            // Transform from tangent space to world space.
            const float3 sample_vector = tangent_x * h.x + tangent_y * h.y + n * h.z;

            irradiance += srv_env_texture.SampleLevel(sam_s0, sample_vector, 0).rgb *
                cos(theta) * sin(theta);

            num_samples++;
        }
    }

    irradiance = PI * irradiance * (1.0 / num_samples);
    out_color = float4(irradiance, 1.0);
}

#elif defined(PSO__GENERATE_PREFILTERED_ENV_TEXTURE)

#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
    "RootConstants(b1, num32BitConstants = 1, visibility = SHADER_VISIBILITY_PIXEL), " \
    "DescriptorTable(SRV(t0), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(" \
    "   s0, " \
    "   filter = FILTER_MIN_MAG_MIP_LINEAR, " \
    "   visibility = SHADER_VISIBILITY_PIXEL, " \
    "   addressU = TEXTURE_ADDRESS_CLAMP, " \
    "   addressV = TEXTURE_ADDRESS_CLAMP, " \
    "   addressW = TEXTURE_ADDRESS_CLAMP" \
    ")"

struct Const {
    float4x4 object_to_clip;
};
ConstantBuffer<Const> cbv_const : register(b0);

struct RootConst {
    float roughness;
};
ConstantBuffer<RootConst> cbv_root_const : register(b1);

TextureCube srv_env_texture : register(t0);
SamplerState sam_s0 : register(s0);

[RootSignature(root_signature)]
void vsGeneratePrefilteredEnvTexture(
    float3 position : POSITION,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    float4 tangent : _Tangent,
    out float4 out_position_clip : SV_Position,
    out float3 out_position : _Position
) {
    out_position_clip = mul(float4(position, 1.0), cbv_const.object_to_clip);
    out_position = position;
}

[RootSignature(root_signature)]
void psGeneratePrefilteredEnvTexture(
    float4 position_clip : SV_Position,
    float3 position : _Position,
    out float4 out_color : SV_Target0
) {
    const float roughness = cbv_root_const.roughness;
    const float3 n = normalize(position);
    const float3 r = n;
    const float3 v = r;

    float3 prefiltered_color = 0.0;
    float total_weight = 0.0;
    const uint num_samples = 1024;

    for (uint sample_idx = 0; sample_idx < num_samples; ++sample_idx) {
        const float2 xi = hammersley(sample_idx, num_samples);
        const float3 h = importanceSampleGgx(xi, roughness, n);
        const float3 l = normalize(2.0 * dot(v, h) * h - v);
        const float n_dot_l = saturate(dot(n, l));
        if (n_dot_l > 0.0) {
            prefiltered_color += srv_env_texture.SampleLevel(sam_s0, l, 0).rgb * n_dot_l;
            total_weight += n_dot_l;
        }
    }
    out_color = float4(prefiltered_color / max(total_weight, 0.001), 1.0);
}

#elif defined(PSO__GENERATE_BRDF_INTEGRATION_TEXTURE)

#define root_signature \
    "DescriptorTable(UAV(u0))"

RWTexture2D<float4> uav_brdf_integration_texture : register(u0);

float2 integrate(float roughness, float n_dot_v) {
    float3 v;
    v.x = 0.0;
    v.y = n_dot_v; // cos
    v.z = sqrt(1.0 - n_dot_v * n_dot_v); // sin

    const float3 n = float3(0.0, 1.0, 0.0);

    float a = 0.0;
    float b = 0.0;
    const uint num_samples = 1024;

    for (uint sample_idx = 0; sample_idx < num_samples; ++sample_idx) {
        const float2 xi = hammersley(sample_idx, num_samples);
        const float3 h = importanceSampleGgx(xi, roughness, n);
        const float3 l = normalize(2.0 * dot(v, h) * h - v);

        const float n_dot_l = saturate(l.y);
        const float n_dot_h = saturate(h.y);
        const float v_dot_h = saturate(dot(v, h));

        if (n_dot_l > 0.0) {
            const float g = geometrySmith(n_dot_l, n_dot_v, roughness);
            const float g_vis = g * v_dot_h / (n_dot_h * n_dot_v);
            const float fc = pow(1.0 - v_dot_h, 5.0);
            a += (1.0 - fc) * g_vis;
            b += fc * g_vis;
        }
    }
    return float2(a, b) / num_samples;
}

[RootSignature(root_signature)]
[numthreads(8, 8, 1)]
void csGenerateBrdfIntegrationTexture(uint3 dispatch_id : SV_DispatchThreadID) {
    float width, height;
    uav_brdf_integration_texture.GetDimensions(width, height);

    const float roughness = (dispatch_id.y + 1) / height;
    const float n_dot_v = (dispatch_id.x + 1) / width;
    const float2 result = integrate(roughness, n_dot_v);

    uav_brdf_integration_texture[dispatch_id.xy] = float4(result, 0.0, 1.0);
}

#endif
