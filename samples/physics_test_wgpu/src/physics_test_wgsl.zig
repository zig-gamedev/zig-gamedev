// zig fmt: off
const common =
\\  struct DrawUniforms {
\\      object_to_world: mat4x4<f32>,
\\      basecolor_roughness: vec4<f32>,
\\  }
\\  @group(1) @binding(0) var<uniform> draw_uniforms: DrawUniforms;
\\
\\  struct FrameUniforms {
\\      world_to_clip: mat4x4<f32>,
\\      camera_position: vec3<f32>,
\\  }
\\  @group(0) @binding(0) var<uniform> frame_uniforms: FrameUniforms;
;
pub const vs = common ++
\\  struct VertexOut {
\\      @builtin(position) position_clip: vec4<f32>,
\\      @location(0) position: vec3<f32>,
\\      @location(1) normal: vec3<f32>,
\\      @location(2) barycentrics: vec3<f32>,
\\  }
\\  @vertex fn main(
\\      @location(0) position: vec3<f32>,
\\      @location(1) normal: vec3<f32>,
\\      @builtin(vertex_index) vertex_index: u32,
\\  ) -> VertexOut {
\\      var output: VertexOut;
\\      output.position_clip = vec4(position, 1.0) * draw_uniforms.object_to_world * frame_uniforms.world_to_clip;
\\      output.position = (vec4(position, 1.0) * draw_uniforms.object_to_world).xyz;
\\      output.normal = normal * mat3x3(
\\          draw_uniforms.object_to_world[0].xyz,
\\          draw_uniforms.object_to_world[1].xyz,
\\          draw_uniforms.object_to_world[2].xyz,
\\      );
\\      let index = vertex_index % 3u;
\\      output.barycentrics = vec3(f32(index == 0u), f32(index == 1u), f32(index == 2u));
\\      return output;
\\  }
;
pub const fs = common ++
\\  const pi = 3.1415926;
\\
\\  fn saturate(x: f32) -> f32 { return clamp(x, 0.0, 1.0); }
\\
\\  // Trowbridge-Reitz GGX normal distribution function.
\\  fn distributionGgx(n: vec3<f32>, h: vec3<f32>, alpha: f32) -> f32 {
\\      let alpha_sq = alpha * alpha;
\\      let n_dot_h = saturate(dot(n, h));
\\      let k = n_dot_h * n_dot_h * (alpha_sq - 1.0) + 1.0;
\\      return alpha_sq / (pi * k * k);
\\  }
\\
\\  fn geometrySchlickGgx(x: f32, k: f32) -> f32 {
\\      return x / (x * (1.0 - k) + k);
\\  }
\\
\\  fn geometrySmith(n: vec3<f32>, v: vec3<f32>, l: vec3<f32>, k: f32) -> f32 {
\\      let n_dot_v = saturate(dot(n, v));
\\      let n_dot_l = saturate(dot(n, l));
\\      return geometrySchlickGgx(n_dot_v, k) * geometrySchlickGgx(n_dot_l, k);
\\  }
\\
\\  fn fresnelSchlick(h_dot_v: f32, f0: vec3<f32>) -> vec3<f32> {
\\      return f0 + (vec3(1.0, 1.0, 1.0) - f0) * pow(1.0 - h_dot_v, 5.0);
\\  }
\\
\\  @fragment fn main(
\\      @location(0) position: vec3<f32>,
\\      @location(1) normal: vec3<f32>,
\\      @location(2) barycentrics: vec3<f32>,
\\  ) -> @location(0) vec4<f32> {
\\      let v = normalize(frame_uniforms.camera_position - position);
\\      let n = normalize(normal);
\\
\\      let base_color = draw_uniforms.basecolor_roughness.xyz;
\\      let ao = 1.0;
\\      var roughness = draw_uniforms.basecolor_roughness.a;
\\      var metallic: f32;
\\      if (roughness < 0.0) { metallic = 1.0; } else { metallic = 0.0; }
\\      roughness = abs(roughness);
\\
\\      let alpha = roughness * roughness;
\\      var k = alpha + 1.0;
\\      k = (k * k) / 8.0;
\\      var f0 = vec3(0.04);
\\      f0 = mix(f0, base_color, metallic);
\\
\\      let light_positions = array<vec3<f32>, 4>(
\\          vec3(25.0, 15.0, 25.0),
\\          vec3(-25.0, 15.0, 25.0),
\\          vec3(25.0, 15.0, -25.0),
\\          vec3(-25.0, 15.0, -25.0),
\\      );
\\      let light_radiance = array<vec3<f32>, 4>(
\\          4.0 * vec3(0.0, 100.0, 250.0),
\\          8.0 * vec3(200.0, 150.0, 250.0),
\\          3.0 * vec3(200.0, 0.0, 0.0),
\\          9.0 * vec3(200.0, 150.0, 0.0),
\\      );
\\
\\      var lo = vec3(0.0);
\\      for (var light_index: i32 = 0; light_index < 4; light_index = light_index + 1) {
\\          let lvec = light_positions[light_index] - position;
\\
\\          let l = normalize(lvec);
\\          let h = normalize(l + v);
\\
\\          let distance_sq = dot(lvec, lvec);
\\          let attenuation = 1.0 / distance_sq;
\\          let radiance = light_radiance[light_index] * attenuation;
\\
\\          let f = fresnelSchlick(saturate(dot(h, v)), f0);
\\
\\          let ndf = distributionGgx(n, h, alpha);
\\          let g = geometrySmith(n, v, l, k);
\\
\\          let numerator = ndf * g * f;
\\          let denominator = 4.0 * saturate(dot(n, v)) * saturate(dot(n, l));
\\          let specular = numerator / max(denominator, 0.001);
\\
\\          let ks = f;
\\          let kd = (vec3(1.0) - ks) * (1.0 - metallic);
\\
\\          let n_dot_l = saturate(dot(n, l));
\\          lo = lo + (kd * base_color / pi + specular) * radiance * n_dot_l;
\\      }
\\
\\      let ambient = vec3(0.03) * base_color * ao;
\\      var color = ambient + lo;
\\      color = color / (color + 1.0);
\\      color = pow(color, vec3(1.0 / 2.2));
\\
\\      // wireframe
\\      var barys = barycentrics;
\\      barys.z = 1.0 - barys.x - barys.y;
\\      let deltas = fwidth(barys);
\\      let smoothing = deltas * 1.0;
\\      let thickness = deltas * 0.25;
\\      barys = smoothstep(thickness, thickness + smoothing, barys);
\\      let min_bary = min(barys.x, min(barys.y, barys.z));
\\      return vec4(min_bary * color, 1.0);
\\  }
// zig fmt: on
;
