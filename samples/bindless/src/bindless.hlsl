#define PI 3.1415926

#define GAMMA 2.2

struct SceneConst {
    float4x4 world_to_clip;
    float3 camera_position;
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

Texture2D Textures[] : register(t0, space0);
SamplerState sam_aniso : register(s0);

float3 fresnelSchlickRoughness(float cos_theta, float3 f0, float roughness) {
    return f0 + (max(1.0 - roughness, f0) - f0) * pow(1.0 - cos_theta, 5.0);
}

struct InputVS {
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 tangent : TANGENT;
};

struct InputPS {
    float4 position_clip : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 tangent : TANGENT;
    float3 position_world : POSITION;
};

void BindlessVS(InputVS input, out InputPS output) {
    const float4x4 object_to_clip = mul(draw_const.object_to_world, scene_const.world_to_clip);
    output.position_clip = mul(float4(input.position, 1.0), object_to_clip);
    output.position_world = mul(input.position, (float3x3)draw_const.object_to_world);
    output.normal = input.normal;
    output.tangent = input.tangent;
    output.uv = input.uv;
}

void BindlessPS(in InputPS input, out float4 out_color : SV_Target0) {
    Texture2D srv_ao_texture = Textures[draw_const.ao_index];
    Texture2D srv_base_color_texture = Textures[draw_const.base_color_index];
    Texture2D srv_metallic_roughness_texture = Textures[draw_const.metallic_roughness_index];
    Texture2D srv_normal_texture = Textures[draw_const.normal_index];

    float3 n = normalize(srv_normal_texture.Sample(sam_aniso, input.uv).rgb * 2.0 - 1.0);

    input.normal = normalize(input.normal);
    input.tangent.xyz = normalize(input.tangent.xyz);
    const float3 bitangent = normalize(cross(input.normal, input.tangent.xyz)) * input.tangent.w;

    const float3x3 object_to_world = (float3x3)draw_const.object_to_world;

    n = mul(n, float3x3(input.tangent.xyz, bitangent, input.normal));
    n = normalize(mul(n, object_to_world));

    float metallic;
    float roughness;
    {
        const float2 mr = srv_metallic_roughness_texture.Sample(sam_aniso, input.uv).bg;
        metallic = mr.r;
        roughness = mr.g;
    }
    const float3 base_color = pow(srv_base_color_texture.Sample(sam_aniso, input.uv).rgb, GAMMA);
    const float ao = srv_ao_texture.Sample(sam_aniso, input.uv).r;
 
    const float3 v = normalize(scene_const.camera_position - input.position_world);
    const float n_dot_v = saturate(dot(n, v));

    float3 f0 = float3(0.04, 0.04, 0.04);
    f0 = lerp(f0, base_color, metallic);

    const float3 r = reflect(-v, n);
    const float3 f = fresnelSchlickRoughness(n_dot_v, f0, roughness);

    const float3 kd = (1.0 - f) * (1.0 - metallic);

    // TODO
    const float3 irradiance = 1;
    const float3 specular = 0;
    const float3 diffuse = irradiance * base_color;

    const float3 ambient = (kd * diffuse + specular) * ao;

    float3 color = ambient;
    color = color / (color + 1.0);

    out_color = float4(pow(color, 1.0 / GAMMA), 1.0);
}