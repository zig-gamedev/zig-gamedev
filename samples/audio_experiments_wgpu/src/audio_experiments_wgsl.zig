// zig fmt: off
pub const lines_vs =
\\  struct DrawUniforms {
\\      object_to_world: mat4x4<f32>,
\\  }
\\  @group(1) @binding(0) var<uniform> draw_uniforms: DrawUniforms;
\\
\\  struct FrameUniforms {
\\      world_to_clip: mat4x4<f32>,
\\  }
\\  @group(0) @binding(0) var<uniform> frame_uniforms: FrameUniforms;
\\
\\  struct VertexOut {
\\      @builtin(position) position_clip: vec4<f32>,
\\      @location(0) color: vec3<f32>,
\\  }
\\  @vertex fn main(
\\      @location(0) position: vec3<f32>,
\\      @location(1) color: vec3<f32>,
\\  ) -> VertexOut {
\\      var output: VertexOut;
\\      output.position_clip = vec4(position, 1.0) * draw_uniforms.object_to_world * frame_uniforms.world_to_clip;
\\      output.color = color;
\\      return output;
\\  }
;
pub const lines_fs =
\\  @fragment fn main(
\\      @location(0) color: vec3<f32>,
\\  ) -> @location(0) vec4<f32> {
\\      return vec4(color, 1.0);
\\  }
;
// zig fmt: on
