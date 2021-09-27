#define root_signature \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "RootConstants(b0, num32BitConstants = 1, visibility = SHADER_VISIBILITY_PIXEL)"

[RootSignature(root_signature)]
void vsTriangle(
    float3 position : POSITION,
    out float4 out_position : SV_Position
) {
    out_position = float4(position, 1.0);
}

struct Const {
    uint color;
};
ConstantBuffer<Const> cbv_const : register(b0);

[RootSignature(root_signature)]
void psTriangle(
    float4 position : SV_Position,
    out float4 out_color : SV_Target0
) {
    const uint c = cbv_const.color;
    out_color = float4((c & 0xff) / 255.0, ((c >> 8) & 0xff) / 255.0, ((c >> 16) & 0xff) / 255.0, 1.0);
}
