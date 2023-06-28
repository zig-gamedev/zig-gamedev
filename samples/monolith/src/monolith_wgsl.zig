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
\\      box_rotation: mat3x3<f32>,
\\      box_center: vec3<f32>,
\\      box_radius: vec3<f32>,
\\      box_inv_radius: vec3<f32>,
\\      camera_position: vec3<f32>,
\\  }
\\  @group(0) @binding(0) var<uniform> frame_uniforms: FrameUniforms;
\\
\\  fn radians(degrees: f32) -> f32 {
\\      return degrees * 3.1415926535897932384626433832795 / 180.0;
\\  }
\\
\\  fn rand2(n: vec2<f32>) -> f32 {
\\      return fract(sin(dot(n, vec2<f32>(12.9898, 4.1414))) * 43758.5453);
\\  }
;
pub const line = struct {
    pub const vs = common ++
    \\  struct VertexOut {
    \\      @builtin(position) position_clip: vec4<f32>,
    \\      @location(1) color: vec3<f32>,
    \\  }
    \\  @stage(vertex) fn main(
    \\      @location(0) position: vec3<f32>,
    \\      @location(1) color: vec3<f32>,
    \\      @builtin(vertex_index) vertex_index: u32,
    \\  ) -> VertexOut {
    \\      var output: VertexOut;
    \\      output.position_clip = vec4(position, 1.0) * draw_uniforms.object_to_world * frame_uniforms.world_to_clip;
    \\      output.color = color;
    \\      return output;
    \\  }
    ;
    pub const fs = common ++
    \\  @stage(fragment) fn main(
    \\      @location(1) color: vec3<f32>,
    \\  ) -> @location(0) vec4<f32> {
    \\      return vec4(color, 1.0);
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
    \\  @stage(vertex) fn main(
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
    \\      return output;
    \\  }
    ;
    pub const fs = common ++
    \\  let pi = 3.14159265359;
    \\  let two_pi = 6.28318530718;
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
    \\
    \\  @stage(fragment) fn main(
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
    \\      if (roughness < 0.0) { metallic = 0.98; } else { metallic = 0.; }
    \\      base_color = abs(base_color);
    \\      roughness = abs(roughness);
    \\
    \\      let alpha = roughness * roughness;
    \\      var k = alpha + 1.0;
    \\      k = (k * k) / 8.0;
    \\      var f0 = vec3(0.04);
    \\      f0 = mix(f0, base_color, metallic);
    \\
    \\      var light_positions: array<vec3<f32>, 9>;
    \\      for (var i = 0u; i < 9u; i = i + 1u) {
    \\          let angle: f32 = radians(f32(i) * 40.0);
    \\          light_positions[i] = vec3<f32>(6.0 * cos(angle), 4.0, 6.0 * sin(angle));
    \\      }
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
    \\          frame_uniforms.box_center,
    \\          frame_uniforms.box_radius,
    \\          frame_uniforms.box_inv_radius,
    \\          frame_uniforms.box_rotation,
    \\      );
    \\
    \\      var lo = vec3(0.0);
    \\      for (var light_index: i32 = 0; light_index < 9; light_index = light_index + 1) {
    \\          let lvec = light_positions[light_index] - position;
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
    \\              //var radiance = vec3<f32>(0, 0, 0);
    \\
    \\              if (position.y > 0.0001) { // reflection of floor lighting on monolith surface
    \\                  //let surf_to_mirror =
    \\                  let mirror_cam = vec3<f32>(position.x, -position.y, position.z);
    \\                  let mirror_cam_to_light = light_positions[light_index] - mirror_cam;
    \\                  let t = -mirror_cam.y / (light_positions[light_index].y - mirror_cam.y);
    \\                  let mirror_pos = mirror_cam + t * mirror_cam_to_light;
    \\                  let mirror_normal = vec3<f32>(0, 1, 0);
    \\
    \\                  let mirror_to_light = light_positions[light_index] - mirror_pos;
    \\                  //let ml = normalize(mirror_cam_to_light);
    \\                  let ml = normalize(mirror_to_light);
    \\                  let mv = normalize(position - mirror_pos);
    \\                  let mh = normalize(ml + mv);
    \\                  var mn: vec3<f32>;
    \\                  var mirror_base_color = frame_uniforms.floor_material.xyz;
    \\                  var mirror_roughness = frame_uniforms.floor_material.a;
    \\
    \\                  if (mirror_base_color.x < 0.0) {
    \\                      let r_x = rand2(floor(2 * mirror_pos.xz));
    \\                      let r_z = rand2(floor(2 * mirror_pos.zx));
    \\                      let r_vec = vec3(r_x, 0, r_z);
    \\                      let r_strength = 0.04;
    \\                      mn = normalize(mirror_normal + r_vec * r_strength);
    \\                      mirror_roughness += min(0, -dot(r_vec, r_vec) * 0.6 + 0.64);
    \\                  } else { mn = normalize(mirror_normal); }
    \\                  var mirror_metallic: f32;
    \\                  if (mirror_roughness < 0.0) { mirror_metallic = 1.; } else { mirror_metallic = 0.; }
    \\                  mirror_base_color = abs(mirror_base_color);
    \\                  mirror_roughness = abs(mirror_roughness);
    \\                  let mirror_alpha = mirror_roughness * mirror_roughness;
    \\                  var mk = mirror_alpha + 1.0;
    \\                  mk = (mk * mk) / 8.0;
    \\                  var mf0 = vec3(0.04);
    \\                  mf0 = mix(mf0, mirror_base_color, mirror_metallic);
    \\
    \\                  let mf = fresnelSchlick(saturate(dot(mh, mv)), mf0);
    \\                  let mndf = distributionGgx(mn, mh, mirror_alpha);
    \\                  let mg = geometrySmith(mn, mv, ml, mk);
    \\                  let mirror_numerator = mndf * mg * mf;
    \\                  let mirror_n_dot_l = saturate(dot(mn, ml));
    \\                  let mirror_denominator = 4.0 * saturate(dot(mn, mv)) * mirror_n_dot_l;
    \\                  let mirror_specular = mirror_numerator / max(mirror_denominator, 0.001);
    \\                  let mks = mf;
    \\                  let mkd = (vec3(1.0) - mks) * (1.0 - mirror_metallic);
    \\
    \\                  let mirror_distance_sq = dot(mirror_cam_to_light, mirror_cam_to_light);
    \\                  //let mirror_distance_sq = dot(mirror_to_light, mirror_to_light);
    \\                  let mirror_attenuation = 1.0 / mirror_distance_sq;
    \\                  var mirror_radiance = 100 * light_colors[light_index] * mirror_attenuation;
    \\                  //radiance += (mkd * mirror_base_color / pi + mirror_specular) * mirror_radiance * mirror_n_dot_l;
    \\                  //radiance = (mkd * mirror_base_color / pi + mirror_specular) * mirror_radiance * mirror_n_dot_l;
    \\                  //lo += (mkd * mirror_base_color / pi + mirror_specular) * mirror_radiance * mirror_n_dot_l;
    \\                  let rad2 = (mkd * mirror_base_color / pi + mirror_specular) * mirror_radiance * mirror_n_dot_l;
    \\
    \\                  let l2 = -mv;
    \\                  let h2 = normalize(l2 + v);
    \\                  //let alpha2 = 0.12;
    \\                  //let k2 = alpha2 + 1;
    \\                  //let f02 = mix(f0, base_color + 0.7 * (vec3<f32>(1, 1, 1) - base_color), metallic);
    \\                  let f = fresnelSchlick(saturate(dot(h2, v)), f0);
    \\                  let ndf = distributionGgx(n, h2, alpha);
    \\                  //let ndf = distributionGgx(n, h2, 0.2);
    \\                  let g = geometrySmith(n, v, l2, k);
    \\                  let numerator = ndf * g * f;
    \\                  let n_dot_l = saturate(dot(n, l2));
    \\                  let denominator = 4.0 * saturate(dot(n, v)) * n_dot_l;
    \\                  let specular = numerator / max(denominator, 0.001);
    \\                  let ks = f;
    \\                  let kd = (vec3(1.0) - ks) * (1.0 - metallic);
    \\                  lo += (kd * base_color / pi + specular) * rad2 * n_dot_l;
    \\              }
    \\              //} else {
    \\                  let f = fresnelSchlick(saturate(dot(h, v)), f0);
    \\                  let ndf = distributionGgx(n, h, alpha);
    \\                  let g = geometrySmith(n, v, l, k);
    \\                  let numerator = ndf * g * f;
    \\                  let n_dot_l = saturate(dot(n, l));
    \\                  let denominator = 4.0 * saturate(dot(n, v)) * n_dot_l;
    \\                  let specular = numerator / max(denominator, 0.001);
    \\                  let ks = f;
    \\                  let kd = (vec3(1.0) - ks) * (1.0 - metallic);
    \\                  lo += (kd * base_color / pi + specular) * radiance * n_dot_l;
    \\              //}
    \\          }
    \\
    \\          // Make light source itself appear as soft orb
    \\          let cl = light_positions[light_index] - frame_uniforms.camera_position;
    \\          let cl_ray = Ray(frame_uniforms.camera_position, -v);
    \\          var cl_hit_dist: f32;
    \\          var cl_hit_norm: vec3<f32>;
    \\          let cl_did_hit: bool = intersect_box(monolith, cl_ray, &cl_hit_dist, &cl_hit_norm);
    \\          if (cl_did_hit && cl_hit_dist < length(cl)) { continue; }
    \\          let negs = (dot(normalize(cl), -v) - 1.0) * dot(cl, cl);
    \\          let peak = 0.2;
    \\          lo += 50 * light_colors[light_index] * max(pow((negs + peak) / peak, 5), 0);
    \\      }
    \\
    \\      var color = lo / (lo + 1.0);
    \\      return vec4(pow(color, vec3(1.0 / 2.2)), 1);
    \\  }
    ;
};
// zig fmt: on