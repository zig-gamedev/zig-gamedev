// zig fmt: off
const global =
\\  let gamma: f32 = 2.2;
\\  let pi: f32 = 3.1415926;
\\
;
const mesh_common =
\\  struct MeshUniforms {
\\      object_to_world: mat4x4<f32>,
\\      world_to_clip: mat4x4<f32>,
\\  }
\\  @group(0) @binding(0) var<uniform> uniforms: MeshUniforms;
\\  @group(0) @binding(1) var base_color_tex: texture_2d<f32>;
\\  @group(0) @binding(2) var aniso_sam: sampler;
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
\\      output.position_clip = vec4(position, 1.0) * uniforms.object_to_world * uniforms.world_to_clip;
\\      output.position = (vec4(position, 1.0) * uniforms.object_to_world).xyz;
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
pub const precompute_env_tex_vs =
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
pub const precompute_env_tex_fs =
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
;
pub const precompute_irradiance_tex_vs =
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
pub const precompute_irradiance_tex_fs = global ++
\\  @group(0) @binding(1) var env_tex: texture_cube<f32>;
\\  @group(0) @binding(2) var env_sam: sampler;
\\  @stage(fragment) fn main(
\\      @location(0) position: vec3<f32>,
\\  ) -> @location(0) vec4<f32> {
\\      let n = normalize(position);
\\
\\      // This is Right-Handed coordinate system and works for upper-left UV coordinate systems.
\\      var up_vector: vec3<f32>;
\\      if (abs(n.y) < 0.999) {
\\          up_vector = vec3(0.0, 1.0, 0.0);
\\      } else {
\\          up_vector = vec3(0.0, 0.0, 1.0);
\\      }
\\      let tangent_x = normalize(cross(up_vector, n));
\\      let tangent_y = normalize(cross(n, tangent_x));
\\
\\      var num_samples: i32  = 0;
\\      var irradiance = vec3(0.0);
\\
\\      for (var phi = 0.0; phi < 2.0 * pi; phi = phi + 0.025) {
\\          let sin_phi = sin(phi);
\\          let cos_phi = cos(phi);
\\
\\          for (var theta = 0.0; theta < 0.5 * pi; theta = theta + 0.025) {
\\              let sin_theta = sin(theta);
\\              let cos_theta = cos(theta);
\\
\\              // Point on a hemisphere.
\\              let h = vec3(sin_theta * cos_phi, sin_theta * sin_phi, cos_theta);
\\
\\              // Transform from tangent space to world space.
\\              let sample_vector = tangent_x * h.x + tangent_y * h.y + n * h.z;
\\
\\              let irr = textureSampleLevel(env_tex, env_sam, sample_vector, 0.0).xyz * cos_theta * sin_theta;
\\
\\              irradiance = irradiance + irr;
\\              num_samples = num_samples + 1;
\\          }
\\      }
\\
\\      irradiance = pi * irradiance * vec3(1.0 / f32(num_samples));
\\      return vec4(irradiance, 1.0);
\\  }
;
pub const sample_env_tex_vs =
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
\\      output.position_clip = (vec4(position, 1.0) * uniforms.object_to_clip).xyww;
\\      output.position = position;
\\      return output;
\\  }
;
pub const sample_env_tex_fs = global ++
\\  @group(0) @binding(1) var env_tex: texture_cube<f32>;
\\  @group(0) @binding(2) var env_sam: sampler;
\\  @stage(fragment) fn main(
\\      @location(0) position: vec3<f32>,
\\  ) -> @location(0) vec4<f32> {
\\      var color = textureSample(env_tex, env_sam, position).xyz;
\\      color = color / (color + vec3(1.0));
\\      return vec4(pow(color, vec3(1.0 / gamma)), 1.0);
\\  }
;
// zig fmt: on
