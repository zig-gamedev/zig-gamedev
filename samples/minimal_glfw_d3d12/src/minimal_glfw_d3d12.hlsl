#define root_signature "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),CBV(b0)"

[RootSignature(root_signature)]
void vsMain(
    float2 position : POSITION,
    out float4 out_position : SV_Position
) {
    out_position = float4(position, 0.0, 1.0);
}

struct Input {
    float2 mouse_position;
};
ConstantBuffer<Input> input : register(b0);

[RootSignature(root_signature)]
void psMain(
    float4 position : SV_Position,
    out float4 out_color : SV_Target0
) {
    if (input.mouse_position.x < position.x) {
        if (input.mouse_position.y < position.y) {
            out_color = float4(1.0, 0.0, 0.0, 1.0);
        } else {
            out_color = float4(0.0, 1.0, 0.0, 1.0);
        }
    } else {
        if (input.mouse_position.y < position.y) {
            out_color = float4(0.0, 0.0, 1.0, 1.0);
        } else {
            out_color = float4(1.0, 1.0, 1.0, 1.0);
        }
    }
}
