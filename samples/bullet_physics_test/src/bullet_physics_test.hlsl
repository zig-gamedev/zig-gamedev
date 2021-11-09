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
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " \
    "CBV(b1, visibility = SHADER_VISIBILITY_VERTEX)"

struct Vertex {
    float3 position;
    float3 normal;
};

struct DrawConst {
    float4x4 object_to_world;
};

struct FrameConst {
    float4x4 world_to_clip;
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
    out_normal = normal;//mul(normal, (float3x3)cbv_draw_const.object_to_world);
}

[RootSignature(root_signature)]
void psSimpleEntity(
    float4 position_window : SV_Position,
    float3 position : _Position,
    float3 normal : _Normal,
    out float4 out_color : SV_Target0
) {
    float3 color = abs(normalize(normal));
    out_color = float4(color, 1.0);
}

#endif
