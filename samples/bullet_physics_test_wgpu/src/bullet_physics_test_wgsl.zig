// zig fmt: off
const global =
\\  let gamma: f32 = 2.2;
\\  let pi: f32 = 3.1415926;
\\
\\  fn saturate(x: f32) -> f32 {
\\      return clamp(x, 0.0, 1.0);
\\  }
\\
\\  fn radicalInverseVdc(in_bits: u32) -> f32 {
\\      var bits = (in_bits << 16u) | (in_bits >> 16u);
\\      bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
\\      bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
\\      bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
\\      bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
\\      return f32(bits) * bitcast<f32>(0x2f800000);
\\  }
\\
\\  fn hammersley(idx: u32, n: u32) -> vec2<f32> {
\\      return vec2(f32(idx) / f32(n), radicalInverseVdc(idx));
\\  }
\\
\\  fn importanceSampleGgx(xi: vec2<f32>, roughness: f32, n: vec3<f32>) -> vec3<f32> {
\\      let alpha = roughness * roughness;
\\      let phi = 2.0 * pi * xi.x;
\\      let cos_theta = sqrt((1.0 - xi.y) / (1.0 + (alpha * alpha - 1.0) * xi.y));
\\      let sin_theta = sqrt(1.0 - cos_theta * cos_theta);
\\
\\      var h: vec3<f32>;
\\      h.x = sin_theta * cos(phi);
\\      h.y = sin_theta * sin(phi);
\\      h.z = cos_theta;
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
\\      // Tangent to world space.
\\      return normalize(tangent_x * h.x + tangent_y * h.y + n * h.z);
\\  }
\\
\\  fn geometrySchlickGgx(cos_theta: f32, roughness: f32) -> f32 {
\\      let k = (roughness * roughness) * 0.5;
\\      return cos_theta / (cos_theta * (1.0 - k) + k);
\\  }
\\
\\  fn geometrySmith(n_dot_l: f32, n_dot_v: f32, roughness: f32) -> f32 {
\\      return geometrySchlickGgx(n_dot_v, roughness) * geometrySchlickGgx(n_dot_l, roughness);
\\  }
\\
;
// zig fmt: on
