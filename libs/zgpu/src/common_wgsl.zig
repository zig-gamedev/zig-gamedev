const std = @import("std");

pub fn csGenerateMipmaps(allocator: std.mem.Allocator, format: []const u8) [:0]const u8 {
    const s0 = std.fmt.allocPrint(
        allocator,
        \\  @group(0) @binding(2) var dst_mipmap1: texture_storage_2d<{s}, write>;
        \\  @group(0) @binding(3) var dst_mipmap2: texture_storage_2d<{s}, write>;
        \\  @group(0) @binding(4) var dst_mipmap3: texture_storage_2d<{s}, write>;
        \\  @group(0) @binding(5) var dst_mipmap4: texture_storage_2d<{s}, write>;
    ,
        .{ format, format, format, format },
    ) catch unreachable;
    defer allocator.free(s0);
    return std.mem.joinZ(allocator, "\n\n", &.{ s0, cs_generate_mipmaps }) catch unreachable;
}

// zig fmt: off
const cs_generate_mipmaps =
\\  struct Uniforms {
\\      src_mip_level: i32,
\\      num_mip_levels: u32,
\\  }
\\  @group(0) @binding(0) var<uniform> uniforms: Uniforms;
\\  @group(0) @binding(1) var src_image: texture_2d<f32>;
\\
\\  var<workgroup> red: array<f32, 64>;
\\  var<workgroup> green: array<f32, 64>;
\\  var<workgroup> blue: array<f32, 64>;
\\  var<workgroup> alpha: array<f32, 64>;
\\
\\  fn storeColor(index: u32, color: vec4<f32>) {
\\      red[index] = color.x;
\\      green[index] = color.y;
\\      blue[index] = color.z;
\\      alpha[index] = color.w;
\\  }
\\
\\  fn loadColor(index: u32) -> vec4<f32> {
\\      return vec4(red[index], green[index], blue[index], alpha[index]);
\\  }
\\
\\  @compute @workgroup_size(8, 8, 1)
\\  fn main(
\\      @builtin(global_invocation_id) global_invocation_id: vec3<u32>,
\\      @builtin(local_invocation_index) local_invocation_index : u32,
\\  ) {
\\      let x = i32(global_invocation_id.x * 2u);
\\      let y = i32(global_invocation_id.y * 2u);
\\
\\      var s00 = textureLoad(src_image, vec2(x, y), uniforms.src_mip_level);
\\      var s10 = textureLoad(src_image, vec2(x + 1, y), uniforms.src_mip_level);
\\      var s01 = textureLoad(src_image, vec2(x, y + 1), uniforms.src_mip_level);
\\      var s11 = textureLoad(src_image, vec2(x + 1, y + 1), uniforms.src_mip_level);
\\      s00 = 0.25 * (s00 + s01 + s10 + s11);
\\
\\      textureStore(dst_mipmap1, vec2<i32>(global_invocation_id.xy), s00);
\\      storeColor(local_invocation_index, s00);
\\      if (uniforms.num_mip_levels == 1u) {
\\          return;
\\      }
\\      workgroupBarrier();
\\
\\      if ((local_invocation_index & 0x9u) == 0u) {
\\          s10 = loadColor(local_invocation_index + 1u);
\\          s01 = loadColor(local_invocation_index + 8u);
\\          s11 = loadColor(local_invocation_index + 9u);
\\          s00 = 0.25 * (s00 + s01 + s10 + s11);
\\          textureStore(dst_mipmap2, vec2<i32>(global_invocation_id.xy / 2u), s00);
\\          storeColor(local_invocation_index, s00);
\\      }
\\      if (uniforms.num_mip_levels == 2u) {
\\          return;
\\      }
\\      workgroupBarrier();
\\
\\      if ((local_invocation_index & 0x1Bu) == 0u) {
\\          s10 = loadColor(local_invocation_index + 2u);
\\          s01 = loadColor(local_invocation_index + 16u);
\\          s11 = loadColor(local_invocation_index + 18u);
\\          s00 = 0.25 * (s00 + s01 + s10 + s11);
\\          textureStore(dst_mipmap3, vec2<i32>(global_invocation_id.xy / 4u), s00);
\\          storeColor(local_invocation_index, s00);
\\      }
\\      if (uniforms.num_mip_levels == 3u) {
\\          return;
\\      }
\\      workgroupBarrier();
\\
\\      if (local_invocation_index == 0u) {
\\          s10 = loadColor(local_invocation_index + 4u);
\\          s01 = loadColor(local_invocation_index + 32u);
\\          s11 = loadColor(local_invocation_index + 36u);
\\          s00 = 0.25 * (s00 + s01 + s10 + s11);
\\          textureStore(dst_mipmap4, vec2<i32>(global_invocation_id.xy / 8u), s00);
\\          storeColor(local_invocation_index, s00);
\\      }
\\  }
;
// zig fmt: on
