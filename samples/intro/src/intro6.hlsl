#if defined(PSO__SIMPLE)

#define ROOT_SIGNATURE \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, visibility = SHADER_VISIBILITY_VERTEX), " /* index 0 */ \
    "CBV(b1, visibility = SHADER_VISIBILITY_VERTEX)"   /* index 1 */

struct DrawConst {
    float4x4 object_to_world;
};
ConstantBuffer<DrawConst> cbv_draw_const : register(b0);

struct FrameConst {
    float4x4 world_to_clip;
};
ConstantBuffer<FrameConst> cbv_frame_const : register(b1);

[RootSignature(ROOT_SIGNATURE)]
void vsMain(
    float3 position : POSITION,
    float3 normal : _Normal,
    out float4 out_position_sv : SV_Position,
    out float3 out_normal : _Normal
) {
    const float4x4 object_to_clip = mul(cbv_draw_const.object_to_world, cbv_frame_const.world_to_clip);
    out_position_sv = mul(float4(position, 1.0), object_to_clip);
    out_normal = normal; // object-space normal
}

[RootSignature(ROOT_SIGNATURE)]
void psMain(
    float4 position_window : SV_Position,
    float3 normal : _Normal,
    out float4 out_color : SV_Target0
) {
    out_color = float4(abs(normal), 1.0);
}

#elif defined(PSO__PHYSICS_DEBUG)

#define ROOT_SIGNATURE \
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

[RootSignature(ROOT_SIGNATURE)]
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

[RootSignature(ROOT_SIGNATURE)]
void psPhysicsDebug(
    float4 position_window : SV_Position,
    float3 color : _Color,
    out float4 out_color : SV_Target0
) {
    out_color = float4(color, 1.0);
}

#endif
