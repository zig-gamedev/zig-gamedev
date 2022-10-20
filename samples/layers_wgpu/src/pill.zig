const std = @import("std");
const math = std.math;
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zm = @import("zmath");

const vertex_generator = @import("vertex_generator.zig");
const Layer = @import("layer.zig").Layer;

// zig fmt: off
const vertex_shader =
\\  @group(0) @binding(0) var<uniform> object_to_clip: mat4x4<f32>;
\\
\\  struct Vertex {
\\      @location(0) position: vec2<f32>,
\\      @location(1) side: f32,
\\  }
\\
\\  struct Instance {
\\      @location(9) width: f32,
\\      @location(10) length: f32,
\\      @location(11) angle: f32,
\\      @location(12) position: vec3<f32>,
\\      @location(13) depth: f32,
\\      @location(14) start_color: vec4<f32>,
\\      @location(15) end_color: vec4<f32>,
\\  }
\\
\\  struct Fragment {
\\      @builtin(position) position: vec4<f32>,
\\      @location(0) color: vec4<f32>,
\\  }
\\
\\  @vertex fn main(vertex: Vertex, instance: Instance) -> Fragment {
\\      // WebGPU mat4x4 are column vectors
\\      var width_mat: mat4x4<f32> = mat4x4(
\\          instance.width, 0.0, 0.0, 0.0,
\\          0.0, instance.width, 0.0, 0.0,
\\          0.0, 0.0, 1.0, 0.0,
\\          0.0, 0.0, 0.0, 1.0,
\\      );
\\      var length_mat: mat4x4<f32> = mat4x4(
\\          1.0, 0.0, 0.0, vertex.side * instance.length / 2.0,
\\          0.0, 1.0, 0.0, 0.0,
\\          0.0, 0.0, 1.0, 0.0,
\\          0.0, 0.0, 0.0, 1.0,
\\      );
\\      var angle_mat: mat4x4<f32> = mat4x4(
\\          cos(instance.angle), -sin(instance.angle), 0.0, 0.0,
\\          sin(instance.angle), cos(instance.angle), 0.0, 0.0,
\\          0.0, 0.0, 1.0, 0.0,
\\          0.0, 0.0, 0.0, 1.0,
\\      );
\\      var position_mat: mat4x4<f32> = mat4x4(
\\          1.0, 0.0, 0.0, instance.position.x,
\\          0.0, 1.0, 0.0, instance.position.y,
\\          0.0, 0.0, 1.0, instance.depth,
\\          0.0, 0.0, 0.0, 1.0,
\\      );
\\      var fragment: Fragment;
\\      fragment.position = vec4(vertex.position, 0.0, 1.0) * width_mat * length_mat * angle_mat *
\\          position_mat * object_to_clip;
\\      fragment.color = select(instance.end_color, instance.start_color, vertex.side == -1);
\\      return fragment;
\\  }
;
const fragment_shader =
\\  struct Fragment {
\\      @location(0) color: vec4<f32>,
\\  }
\\  struct Screen {
\\      @location(0) color: vec4<f32>,
\\  }
\\
\\  @fragment fn main(fragment: Fragment) -> Screen {
\\      var screen: Screen;
\\      screen.color = fragment.color;
\\      return screen;
\\  }
// zig fmt: on
;

const vertex_attributes = [_]wgpu.VertexAttribute{ .{
    .format = .float32x2,
    .offset = @offsetOf(Vertex, "position"),
    .shader_location = 0,
}, .{
    .format = .float32,
    .offset = @offsetOf(Vertex, "side"),
    .shader_location = 1,
} };

pub const Vertex = struct {
    position: [2]f32,
    side: f32,
};

const instance_attributes = [_]wgpu.VertexAttribute{ .{
    .format = .float32,
    .offset = @offsetOf(Instance, "width"),
    .shader_location = 9,
}, .{
    .format = .float32,
    .offset = @offsetOf(Instance, "length"),
    .shader_location = 10,
}, .{
    .format = .float32,
    .offset = @offsetOf(Instance, "angle"),
    .shader_location = 11,
}, .{
    .format = .float32x2,
    .offset = @offsetOf(Instance, "position"),
    .shader_location = 12,
}, .{
    .format = .float32,
    .offset = @offsetOf(Instance, "depth"),
    .shader_location = 13,
}, .{
    .format = .float32x4,
    .offset = @offsetOf(Instance, "start_color"),
    .shader_location = 14,
}, .{
    .format = .float32x4,
    .offset = @offsetOf(Instance, "end_color"),
    .shader_location = 15,
} };

pub const Instance = struct {
    width: f32,
    length: f32,
    angle: f32,
    position: [2]f32,
    depth: f32,
    start_color: [4]f32,
    end_color: [4]f32,
};

pub const Pills = Layer(Vertex, Instance);

pub fn init(gctx: *zgpu.GraphicsContext, allocator: std.mem.Allocator, segments: u16) !Pills {
    const bind_groups = [_]zgpu.BindGroupLayoutHandle{
        gctx.createBindGroupLayout(&.{
            zgpu.bufferEntry(0, .{ .vertex = true }, .uniform, true, 0),
        }),
    };
    defer for (bind_groups) |bind_group| gctx.releaseResource(bind_group);
    const vertex_bindings = [_]zgpu.BindGroupEntryInfo{.{
        .binding = 0,
        .buffer_handle = gctx.uniforms.buffer,
        .offset = 0,
        .size = @sizeOf(zm.Mat),
    }};
    var self = Pills.init(
        gctx,
        allocator,

        &bind_groups,
        null,
        .{
            .group = 0,
            .bindings = &vertex_bindings,
        },
        &vertex_attributes,
        &instance_attributes,
        vertex_shader,

        null,
        fragment_shader,
    );

    try vertex_generator.generateVertices(segments, &self.vertices);
    self.recreateVertexBuffer();

    try vertex_generator.generateIndices(segments, &self.indices);
    self.recreateIndexBuffer();

    return self;
}
