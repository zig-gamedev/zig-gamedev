#if defined(PSO__TEXTURE_TO_BUFFER)

#define root_signature \
    "DescriptorTable(SRV(t0, flags = DESCRIPTORS_VOLATILE), UAV(u0))"

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

#elif defined(PSO__BUFFER_TO_TEXTURE)

#define root_signature \
    "DescriptorTable(SRV(t0), UAV(u0))"

Buffer<float> srv_buffer : register(t0);
RWTexture2D<float4> uav_texture : register(u0);

[RootSignature(root_signature)]
[numthreads(8, 8, 1)]
void csBufferToTexture(
    uint3 dispatch_id : SV_DispatchThreadID
) {
    const uint2 tid = dispatch_id.xy;
    uint w, h;
    uav_texture.GetDimensions(w, h);

    if (dispatch_id.x >= w || dispatch_id.y >= h) return;

    const float luminance = srv_buffer[w * tid.y + tid.x];
    uav_texture[tid] = luminance;
}

#elif defined(PSO__DRAW_TEXTURE)

#define root_signature \
    "DescriptorTable(SRV(t0, flags = DESCRIPTORS_VOLATILE), visibility = SHADER_VISIBILITY_PIXEL), " \
    "StaticSampler(s0, filter = FILTER_MIN_MAG_LINEAR_MIP_POINT, visibility = SHADER_VISIBILITY_PIXEL)"

[RootSignature(root_signature)]
void vsDrawTexture(
    uint vertex_id: SV_VertexID,
    out float4 out_position : SV_Position,
    out float2 out_uv : _Texcoords0
) {
    const float2 positions[3] = { float2(-1.0, -1.0), float2(-1.0, 3.0), float2(3.0, -1.0) };
    const float2 position = positions[vertex_id];

    out_position = float4(position, 0.0, 1.0);
    out_uv = 0.5 + 0.5 * position;
    out_uv = float2(out_uv.x, 1.0 - out_uv.y);
}

Texture2D srv_texture : register(t0);
SamplerState sam_sampler : register(s0);

[RootSignature(root_signature)]
void psDrawTexture(
    float4 position_window : SV_Position,
    float2 uv : _Texcoords0,
    out float4 out_color : SV_Target0
) {
    float3 color = srv_texture.Sample(sam_sampler, uv).rgb;
    out_color = float4(color, 1.0);
}

#endif
