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

Texture2D<float4> srv_texture : register(t0);
RWBuffer<float> uav_buffer : register(u0);

[RootSignature(root_signature)]
[numthreads(8, 8, 1)]
void csTextureToBuffer(
    uint3 dispatch_id : SV_DispatchThreadID
) {
    const uint2 tid = dispatch_id.xy;
    uint w, h;
    srv_texture.GetDimensions(w, h);

    if (dispatch_id.x >= w || dispatch_id.y >= h) return;

    const float3 c = srv_texture[tid].rgb;
    const float luminance = c.r * 0.3 + c.g * 0.59 + c.b * 0.11;

    uav_buffer[w * tid.y + tid.x] = luminance;
}

#endif
