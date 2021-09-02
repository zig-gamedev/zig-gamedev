#if defined(PSO__MESH_PBR)

#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
    "DescriptorTable(SRV(t0, numDescriptors = 4), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(s0, filter = FILTER_ANISOTROPIC, maxAnisotropy = 16, visibility = SHADER_VISIBILITY_PIXEL)"

struct Const {
    float4x4 object_to_clip;
    float4x4 object_to_world;
};
ConstantBuffer<Const> cbv_const : register(b0);

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
    out_position_clip = mul(float4(position, 1.0), cbv_const.object_to_clip);
    out_position = mul(position, (float3x3)cbv_const.object_to_world);
    out_normal = normal;
    out_texcoords0 = texcoords0;
    out_tangent = tangent;
}

Texture2D srv_ao_texture : register(t0);
Texture2D srv_base_color_texture : register(t1);
Texture2D srv_metallic_roughness_texture : register(t2);
Texture2D srv_normal_texture : register(t3);

SamplerState sam_aniso : register(s0);

[RootSignature(root_signature)]
void psMeshPbr(
    float4 position_clip : SV_Position,
    float3 position : _Position,
    float3 normal : _Normal,
    float2 texcoords0 : _Texcoords0,
    float4 tangent : _Tangent,
    out float4 out_color : SV_Target0
) {
    float3 n = normalize(normal);
    float3 color = abs(n) * srv_ao_texture.Sample(sam_aniso, texcoords0).rgb;
    out_color = float4(color, 1.0);
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
    out_color = srv_equirect_texture.SampleLevel(sam_s0, uv, 0);
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
    out_color = float4(env_color, 1.0);
}

#endif
