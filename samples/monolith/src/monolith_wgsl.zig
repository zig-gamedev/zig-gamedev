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
\\      floor_material: vec4<f32>,
\\      monolith_rotation: mat3x3<f32>,
\\      monolith_center: vec3<f32>,
\\      monolith_radius: vec3<f32>,
\\      monolith_inv_radius: vec3<f32>,
\\      camera_position: vec3<f32>,
\\      lights: array<vec3<f32>, 9>,
\\  }
\\  @group(0) @binding(0) var<uniform> frame_uniforms: FrameUniforms;
\\
\\  const pi = 3.1415926535897932384626433832795;
\\  const two_pi = 2.0 * pi;
\\
\\  fn radians(degrees: f32) -> f32 {
\\      return degrees * pi / 180.0;
\\  }
\\
\\  fn rand2(n: vec2<f32>) -> f32 {
\\      return fract(sin(dot(n, vec2<f32>(12.9898, 4.1414))) * 43758.5453);
\\  }
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
\\  struct Box {
\\      center: vec3<f32>,
\\      radius: vec3<f32>,
\\      inv_radius: vec3<f32>,
\\      rotation: mat3x3<f32>,
\\  };
\\  struct Ray {
\\      origin: vec3<f32>,
\\      direction: vec3<f32>,
\\  };
\\  // Alexander Majercik, Cyril Crassin, Peter Shirley, and Morgan McGuire, A Ray-Box Intersection Algorithm and
\\  // Efficient Dynamic Voxel Rendering, Journal of Computer Graphics Techniques (JCGT), vol. 7, no. 3, 66-81, 2018
\\  // This wgsl implementation of the algorithm is customized to fit the assumptions of this use case.
\\  fn intersect_box(box: Box, ray: Ray, distance: ptr<function, f32>, normal: ptr<function, vec3<f32>>) -> bool {
\\      let r = Ray(
\\          (ray.origin - box.center) * box.rotation,
\\          ray.direction * box.rotation,
\\      );
\\      var winding: f32 = 1.0;
\\      let winding_vec = abs(r.origin) * box.inv_radius;
\\      if (max(max(winding_vec.x, winding_vec.y), winding_vec.z) < 1.0) { winding = -1.0; }
\\      var sgn: vec3<f32> = -sign(r.direction);
\\      var plane_dist: vec3<f32> = box.radius * winding * sgn - r.origin;
\\      plane_dist = plane_dist / r.direction;
\\
\\      let test = vec3<bool>(
\\          (plane_dist.x >= 0.0) && (all(abs(r.origin.yz + r.direction.yz * plane_dist.x) < box.radius.yz)),
\\          (plane_dist.y >= 0.0) && (all(abs(r.origin.zx + r.direction.zx * plane_dist.y) < box.radius.zx)),
\\          (plane_dist.z >= 0.0) && (all(abs(r.origin.xy + r.direction.xy * plane_dist.z) < box.radius.xy)),
\\      );
\\
\\      if (test.x) { sgn = vec3<f32>(sgn.x, 0.0, 0.0); }
\\      else if (test.y) { sgn = vec3<f32>(0.0, sgn.y, 0.0); }
\\      else if (test.z) { sgn = vec3<f32>(0.0, 0.0, sgn.z); }
\\      else { sgn = vec3<f32>(0.0, 0.0, 0.0); }
\\
\\      if (sgn.x != 0.0) { *distance = plane_dist.x; }
\\      else if (sgn.y != 0.0) { *distance = plane_dist.y; }
\\      else { *distance = plane_dist.z; }
\\
\\      *normal = box.rotation * sgn;
\\      return (sgn.x != 0.0) || (sgn.y != 0.0) || (sgn.z != 0.0);
\\  }
;
pub const debug = struct {
    pub const vs = common ++
    \\  struct VertexOut {
    \\      @builtin(position) position_clip: vec4<f32>,
    \\      @location(0) position: vec3<f32>,
    \\      @location(1) normal: vec3<f32>,
    \\  }
    \\  @vertex fn main(
    \\      @location(0) position: vec3<f32>,
    \\      @location(1) normal: vec3<f32>,
    \\  ) -> VertexOut {
    \\      var output: VertexOut;
    \\      output.position_clip = vec4(position, 1.0) * draw_uniforms.object_to_world * frame_uniforms.world_to_clip;
    \\      output.position = (vec4(position, 1.0) * draw_uniforms.object_to_world).xyz;
    \\      output.normal = (vec4(normal, 0.0) * draw_uniforms.object_to_world).xyz;
    \\      return output;
    \\  }
    ;
    pub const fs = common ++
    \\  @fragment fn main(
    \\      @location(0) position: vec3<f32>,
    \\      @location(1) normal: vec3<f32>,
    \\  ) -> @location(0) vec4<f32> {
    \\      let v = normalize(frame_uniforms.camera_position - position);
    \\      let n = normalize(normal);
    \\      let facing_cam = saturate(dot(n, v));
    \\
    \\      let curviness = length(fwidth(normal));
    \\      let bubble_mode = step(0.001, curviness);
    \\      let bubble_factor = 2.0 - bubble_mode * facing_cam * 2.0;
    \\      let roughness_mult = 1.0 - 0.75 * bubble_mode;
    \\      let nonphysical_specular_mult = 1.0 + 127.0 * bubble_mode;
    \\
    \\      let base_color = draw_uniforms.basecolor_roughness.xyz;
    \\      var roughness = draw_uniforms.basecolor_roughness.a * roughness_mult;
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
    \\      let radiance = vec3(4.0);
    \\      let l = normalize(vec3(0.3, 1.0, 0.3));
    \\      let h = normalize(l + v);
    \\      let f = fresnelSchlick(saturate(dot(h, v)), f0);
    \\      let ndf = distributionGgx(n, h, alpha);
    \\      let g = geometrySmith(n, v, l, k);
    \\      let numerator = ndf * g * f * nonphysical_specular_mult;
    \\      let denominator = 4.0 * facing_cam * saturate(dot(n, l));
    \\      let specular = numerator / max(denominator, 0.001);
    \\      let ks = f;
    \\      let kd = (vec3(1.0) - ks) * (1.0 - metallic);
    \\      let n_dot_l = saturate(dot(n, l));
    \\      let lighting_result = (kd * base_color / pi + specular) * radiance * n_dot_l;
    \\
    \\      let ambient = vec3(0.03) * base_color;
    \\      var color = ambient + lighting_result;
    \\      color = color / (color + 1.0);
    \\      color = pow(color, vec3(1.0 / 2.2));
    \\
    \\      return vec4(color, 0.4 * bubble_factor);
    \\  }
    ;
};
pub const mesh = struct {
    pub const vs = common ++
    \\  struct VertexOut {
    \\      @builtin(position) position_clip: vec4<f32>,
    \\      @location(0) position: vec3<f32>,
    \\      @location(1) normal: vec3<f32>,
    \\  }
    \\  @vertex fn main(
    \\      @location(0) position: vec3<f32>,
    \\      @location(1) normal: vec3<f32>,
    \\  ) -> VertexOut {
    \\      var output: VertexOut;
    \\      output.position_clip = vec4(position, 1.0) * draw_uniforms.object_to_world * frame_uniforms.world_to_clip;
    \\      output.position = (vec4(position, 1.0) * draw_uniforms.object_to_world).xyz;
    \\      output.normal = (vec4(normal, 0.0) * draw_uniforms.object_to_world).xyz;
    \\      return output;
    \\  }
    ;
    pub const fs = common ++
    \\  @fragment fn main(
    \\      @location(0) position: vec3<f32>,
    \\      @location(1) normal: vec3<f32>,
    \\  ) -> @location(0) vec4<f32> {
    \\      let v = normalize(frame_uniforms.camera_position - position);
    \\      var base_color = draw_uniforms.basecolor_roughness.xyz;
    \\      var roughness = draw_uniforms.basecolor_roughness.a;
    \\
    \\      var n: vec3<f32>;
    \\      if (base_color.x < 0.0) {
    \\          let r_x = rand2(floor(2 * position.xz));
    \\          let r_z = rand2(floor(2 * position.zx));
    \\          let r_vec = vec3(r_x, 0, r_z);
    \\          let r_strength = 0.04;
    \\          n = normalize(normal + r_vec * r_strength);
    \\          roughness += min(0, -dot(r_vec, r_vec) * 0.6 + 0.64);
    \\      } else { n = normalize(normal); }
    \\
    \\      var metallic: f32;
    \\      if (roughness < 0.0) { metallic = 1.0; } else { metallic = 0.0; }
    \\      base_color = abs(base_color);
    \\      roughness = abs(roughness);
    \\
    \\      let alpha = roughness * roughness;
    \\      var k = alpha + 1.0;
    \\      k = (k * k) / 8.0;
    \\      var f0 = vec3(0.04);
    \\      f0 = mix(f0, base_color, metallic);
    \\
    \\      let light_colors = array<vec3<f32>, 9>(
    \\          vec3(1.00, 0.80, 0.26),
    \\          vec3(1.00, 0.75, 0.20),
    \\          vec3(1.00, 0.65, 0.16),
    \\          vec3(0.25, 0.60, 1.00),
    \\          vec3(0.30, 0.50, 1.00),
    \\          vec3(0.35, 0.40, 1.00),
    \\          vec3(1.00, 0.50, 0.40),
    \\          vec3(1.00, 0.70, 0.70),
    \\          vec3(0.70, 1.00, 0.70),
    \\      );
    \\
    \\      let monolith = Box(
    \\          frame_uniforms.monolith_center,
    \\          frame_uniforms.monolith_radius,
    \\          frame_uniforms.monolith_inv_radius,
    \\          frame_uniforms.monolith_rotation,
    \\      );
    \\
    \\      var lo = vec3(0.0);
    \\      for (var light_index: i32 = 0; light_index < 9; light_index = light_index + 1) {
    \\          let lvec = frame_uniforms.lights[light_index] - position;
    \\          let l = normalize(lvec);
    \\          let h = normalize(l + v);
    \\
    \\          let pl_ray = Ray(position, l);
    \\          var pl_hit_dist: f32;
    \\          var pl_hit_norm: vec3<f32>;
    \\          let pl_did_hit: bool = intersect_box(monolith, pl_ray, &pl_hit_dist, &pl_hit_norm);
    \\          if (!pl_did_hit || pl_hit_dist > length(lvec)) {
    \\              let distance_sq = dot(lvec, lvec);
    \\              let attenuation = 1.0 / distance_sq;
    \\              var radiance = 100 * light_colors[light_index] * attenuation;
    \\              let f = fresnelSchlick(saturate(dot(h, v)), f0);
    \\              let ndf = distributionGgx(n, h, alpha);
    \\              let g = geometrySmith(n, v, l, k);
    \\              let numerator = ndf * g * f;
    \\              let n_dot_l = saturate(dot(n, l));
    \\              let denominator = 4.0 * saturate(dot(n, v)) * n_dot_l;
    \\              let specular = numerator / max(denominator, 0.001);
    \\              let ks = f;
    \\              let kd = (vec3(1.0) - ks) * (1.0 - metallic);
    \\              lo += (kd * base_color / pi + specular) * radiance * n_dot_l;
    \\          }
    \\          var mirror_view = vec3<f32>(0, 0, 0);
    \\          if (position.y > 0.0001 && metallic > 0.5) { mirror_view = reflect(-v, n); }
    \\          if (mirror_view.y < 0.0) { // reflection of floor on monolith surface
    \\              let floor_t = -position.y / mirror_view.y;
    \\              let floor_x = position.x + floor_t * mirror_view.x;
    \\              let floor_z = position.z + floor_t * mirror_view.z;
    \\              let floor_pos = vec3<f32>(floor_x, 0.0, floor_z);
    \\              var floor_normal = vec3<f32>(0, 1, 0);
    \\
    \\              let floor_to_light = frame_uniforms.lights[light_index] - floor_pos;
    \\              let fl = normalize(floor_to_light);
    \\              let fl_ray = Ray(floor_pos, fl);
    \\              var fl_hit_dist: f32;
    \\              var fl_hit_norm: vec3<f32>;
    \\              let fl_did_hit: bool = intersect_box(monolith, fl_ray, &fl_hit_dist, &fl_hit_norm);
    \\              if (!fl_did_hit || fl_hit_dist > length(floor_to_light)) {
    \\                  let fv = normalize(position - floor_pos);
    \\                  let fh = normalize(fl + fv);
    \\                  var floor_base_color = frame_uniforms.floor_material.xyz;
    \\                  var floor_roughness = frame_uniforms.floor_material.a;
    \\
    \\                  if (floor_base_color.x < 0.0) {
    \\                      let r_x = rand2(floor(2 * floor_pos.xz));
    \\                      let r_z = rand2(floor(2 * floor_pos.zx));
    \\                      let r_vec = vec3(r_x, 0, r_z);
    \\                      let r_strength = 0.04;
    \\                      floor_normal = normalize(floor_normal + r_vec * r_strength);
    \\                      floor_roughness += min(0, -dot(r_vec, r_vec) * 0.6 + 0.64);
    \\                  }
    \\                  var floor_metallic: f32;
    \\                  if (floor_roughness < 0.0) { floor_metallic = 1.0; } else { floor_metallic = 0.0; }
    \\                  floor_base_color = abs(floor_base_color);
    \\                  floor_roughness = abs(floor_roughness);
    \\                  let floor_alpha = floor_roughness * floor_roughness;
    \\                  var fk = floor_alpha + 1.0;
    \\                  fk = (fk * fk) / 8.0;
    \\                  var ff0 = vec3(0.04);
    \\                  ff0 = mix(ff0, floor_base_color, floor_metallic);
    \\
    \\                  let ff = fresnelSchlick(saturate(dot(fh, fv)), ff0);
    \\                  let fndf = distributionGgx(floor_normal, fh, floor_alpha);
    \\                  let fg = geometrySmith(floor_normal, fv, fl, fk);
    \\                  let floor_numerator = fndf * fg * ff;
    \\                  let floor_n_dot_l = saturate(dot(floor_normal, fl));
    \\                  let floor_denominator = 4.0 * saturate(dot(floor_normal, fv)) * floor_n_dot_l;
    \\                  let floor_specular = floor_numerator / max(floor_denominator, 0.001);
    \\                  let fkd = (vec3(1.0) - ff) * (1.0 - floor_metallic);
    \\
    \\                  let floor_distance_sq = dot(floor_to_light, floor_to_light);
    \\                  let floor_attenuation = 1.0 / floor_distance_sq;
    \\                  let floor_radiance = 4 * light_colors[light_index] * floor_attenuation;
    \\                  lo += (fkd * floor_base_color / pi + floor_specular) * floor_radiance * floor_n_dot_l;
    \\              }
    \\          }
    \\
    \\          // Make light source itself appear as soft orb
    \\          let cl = frame_uniforms.lights[light_index] - frame_uniforms.camera_position;
    \\          let cl_ray = Ray(frame_uniforms.camera_position, -v);
    \\          var cl_hit_dist: f32;
    \\          var cl_hit_norm: vec3<f32>;
    \\          let cl_did_hit: bool = intersect_box(monolith, cl_ray, &cl_hit_dist, &cl_hit_norm);
    \\          if (cl_did_hit && cl_hit_dist < length(cl)) { continue; }
    \\          let negs = (dot(normalize(cl), -v) - 1.0) * dot(cl, cl);
    \\          let peak = 0.175;
    \\          lo += 50 * light_colors[light_index] * max(pow((negs + peak) / peak, 5), 0);
    \\      }
    \\
    \\      var color = lo / (lo + 1.0);
    \\      return vec4(pow(color, vec3(1.0 / 2.2)), 1);
    \\  }
    ;
};
// zig fmt: on
