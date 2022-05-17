// zig fmt: off
const mesh_common =
\\  struct DrawUniforms {
\\      object_to_world: mat4x4<f32>,
\\  }
\\  @group(1) @binding(0) var<uniform> draw_uniforms: DrawUniforms;
\\  @group(1) @binding(1) var base_color_tex: texture_2d<f32>;
\\  @group(1) @binding(2) var aniso_sam: sampler;
\\
\\  struct FrameUniforms {
\\      world_to_clip: mat4x4<f32>,
\\  }
\\  @group(0) @binding(0) var<uniform> frame_uniforms: FrameUniforms;
;
pub const mesh_vs = mesh_common ++
\\  struct VertexOut {
\\      @builtin(position) position_clip: vec4<f32>,
\\      @location(0) position: vec3<f32>,
\\      @location(1) normal: vec3<f32>,
\\      @location(2) texcoord: vec2<f32>,
\\      @location(3) tangent: vec4<f32>,
\\  }
\\  @stage(vertex) fn main(
\\      @location(0) position: vec3<f32>,
\\      @location(1) normal: vec3<f32>,
\\      @location(2) texcoord: vec2<f32>,
\\      @location(3) tangent: vec4<f32>,
\\  ) -> VertexOut {
\\      var output: VertexOut;
\\      output.position_clip = vec4(position, 1.0) * draw_uniforms.object_to_world * frame_uniforms.world_to_clip;
\\      output.position = (vec4(position, 1.0) * draw_uniforms.object_to_world).xyz;
\\      output.normal = normal;
\\      output.texcoord = texcoord;
\\      output.tangent = tangent;
\\      return output;
\\  }
;
pub const mesh_fs = mesh_common ++
\\  @stage(fragment) fn main(
\\      @location(0) position: vec3<f32>,
\\      @location(1) normal: vec3<f32>,
\\      @location(2) texcoord: vec2<f32>,
\\      @location(3) tangent: vec4<f32>,
\\  ) -> @location(0) vec4<f32> {
\\      let color = textureSample(base_color_tex, aniso_sam, texcoord).xyz;
\\      return vec4(color, 1.0);
\\  }
;
pub const generate_env_tex_vs =
\\  struct Uniforms {
\\      object_to_clip: mat4x4<f32>,
\\  }
\\  @group(0) @binding(0) var<uniform> uniforms: Uniforms;
\\  struct VertexOut {
\\      @builtin(position) position_clip: vec4<f32>,
\\      @location(0) position: vec3<f32>,
\\  }
\\  @stage(vertex) fn main(
\\      @location(0) position: vec3<f32>,
\\  ) -> VertexOut {
\\      var output: VertexOut;
\\      output.position_clip = vec4(position, 1.0) * uniforms.object_to_clip;
\\      output.position = position;
\\      return output;
\\  }
;
pub const generate_env_tex_fs =
\\  @group(0) @binding(1) var equirect_tex: texture_2d<f32>;
\\  @group(0) @binding(2) var equirect_sam: sampler;
\\  fn sampleSphericalMap(v: vec3<f32>) -> vec2<f32> {
\\      var uv = vec2(atan2(v.z, v.x), asin(v.y));
\\      uv = uv * vec2(0.1591, 0.3183);
\\      uv = uv + vec2(0.5);
\\      return uv;
\\  }
\\  @stage(fragment) fn main(
\\      @location(0) position: vec3<f32>,
\\  ) -> @location(0) vec4<f32> {
\\      let uv = sampleSphericalMap(normalize(position));
\\      let color = textureSampleLevel(equirect_tex, equirect_sam, uv, 0.0).xyz;
\\      return vec4(color, 1.0);
\\  }
// zig fmt: on
;
