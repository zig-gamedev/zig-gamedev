const std = @import("std");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;

const UniformsGroup = struct {
    group: u32,
    bindings: []const zgpu.BindGroupEntryInfo,
};
const BindGroup = struct {
    group: u32,
    bind_group: zgpu.BindGroupHandle,
};

pub fn Layer(comptime Vertex: type, comptime Instance: type) type {
    return struct {
        const State = @This();

        gctx: *zgpu.GraphicsContext,

        common_uniforms: ?BindGroup,
        common_uniforms_offsets: std.ArrayList(u32),

        vertex_uniforms: ?BindGroup,
        vertex_uniforms_offsets: std.ArrayList(u32),

        fragment_uniforms: ?BindGroup,
        fragment_uniforms_offsets: std.ArrayList(u32),

        pipeline: zgpu.RenderPipelineHandle,

        vertices: std.ArrayList(Vertex),
        vertex_buffer: zgpu.BufferHandle,

        indices: std.ArrayList(u16),
        index_buffer: zgpu.BufferHandle,

        instances: std.ArrayList(Instance),
        instance_buffer: zgpu.BufferHandle,

        pub fn init(
            gctx: *zgpu.GraphicsContext,
            allocator: std.mem.Allocator,
            bind_groups: []const zgpu.BindGroupLayoutHandle,
            common_uniforms: ?UniformsGroup,
            vertex_uniforms: ?UniformsGroup,
            vertex_attributes: []const wgpu.VertexAttribute,
            instance_attributes: []const wgpu.VertexAttribute,
            vertex_shader: [*:0]const u8,
            fragment_uniforms: ?UniformsGroup,
            fragment_shader: [*:0]const u8,
        ) State {
            return .{
                .gctx = gctx,

                .common_uniforms = if (common_uniforms) |cu| .{
                    .group = cu.group,
                    .bind_group = gctx.createBindGroup(bind_groups[cu.group], cu.bindings),
                } else null,
                .common_uniforms_offsets = std.ArrayList(u32).init(allocator),

                .vertex_uniforms = if (vertex_uniforms) |vu| .{
                    .group = vu.group,
                    .bind_group = gctx.createBindGroup(bind_groups[vu.group], vu.bindings),
                } else null,
                .vertex_uniforms_offsets = std.ArrayList(u32).init(allocator),

                .fragment_uniforms = if (fragment_uniforms) |fu| .{
                    .group = fu.group,
                    .bind_group = gctx.createBindGroup(bind_groups[fu.group], fu.bindings),
                } else null,
                .fragment_uniforms_offsets = std.ArrayList(u32).init(allocator),

                .pipeline = createPipeline(gctx, bind_groups, vertex_attributes, instance_attributes, vertex_shader, fragment_shader),

                .vertices = std.ArrayList(Vertex).init(allocator),
                .vertex_buffer = .{},

                .indices = std.ArrayList(u16).init(allocator),
                .index_buffer = .{},

                .instances = std.ArrayList(Instance).init(allocator),
                .instance_buffer = .{},
            };
        }

        pub fn deinit(self: *State) void {
            self.common_uniforms_offsets.deinit();
            self.vertex_uniforms_offsets.deinit();
            self.fragment_uniforms_offsets.deinit();

            self.vertices.deinit();
            self.indices.deinit();
            self.instances.deinit();
        }

        pub fn recreateVertexBuffer(self: *State) void {
            const gctx = self.gctx;

            gctx.destroyResource(self.vertex_buffer);
            const vertex_buffer = gctx.createBuffer(.{
                .usage = .{ .copy_dst = true, .vertex = true },
                .size = ensureFourByteMultiple(self.vertices.items.len * @sizeOf(Vertex)),
            });
            gctx.queue.writeBuffer(gctx.lookupResource(vertex_buffer).?, 0, Vertex, self.vertices.items);
            self.vertex_buffer = vertex_buffer;
        }

        pub fn recreateIndexBuffer(self: *State) void {
            const gctx = self.gctx;

            gctx.destroyResource(self.index_buffer);
            const index_buffer = gctx.createBuffer(.{
                .usage = .{ .copy_dst = true, .index = true },
                .size = ensureFourByteMultiple(self.indices.items.len * @sizeOf(u16)),
            });
            gctx.queue.writeBuffer(gctx.lookupResource(index_buffer).?, 0, u16, self.indices.items);
            self.index_buffer = index_buffer;
        }

        pub fn recreateInstanceBuffer(self: *State) void {
            const gctx = self.gctx;

            gctx.destroyResource(self.instance_buffer);
            const instance_buffer = gctx.createBuffer(.{
                .usage = .{ .copy_dst = true, .vertex = true },
                .size = ensureFourByteMultiple(self.instances.items.len * @sizeOf(Instance)),
            });
            gctx.queue.writeBuffer(gctx.lookupResource(instance_buffer).?, 0, Instance, self.instances.items);
            self.instance_buffer = instance_buffer;
        }

        fn createPipeline(
            gctx: *zgpu.GraphicsContext,
            bind_groups: []const zgpu.BindGroupLayoutHandle,
            vertex_attributes: []const wgpu.VertexAttribute,
            instance_attributes: []const wgpu.VertexAttribute,
            vertex_shader: [*:0]const u8,
            fragment_shader: [*:0]const u8,
        ) zgpu.RenderPipelineHandle {
            const pipeline_layout = gctx.createPipelineLayout(bind_groups);
            defer gctx.releaseResource(pipeline_layout);

            const vs_module = zgpu.createWgslShaderModule(gctx.device, vertex_shader, "vs");
            defer vs_module.release();

            const fs_module = zgpu.createWgslShaderModule(gctx.device, fragment_shader, "fs");
            defer fs_module.release();

            const color_targets = [_]wgpu.ColorTargetState{.{
                .format = zgpu.GraphicsContext.swapchain_format,
            }};

            const vertex_buffers = [_]wgpu.VertexBufferLayout{ .{
                .array_stride = @sizeOf(Vertex),
                .attribute_count = @as(u32, @intCast(vertex_attributes.len)),
                .attributes = vertex_attributes.ptr,
            }, .{
                .array_stride = @sizeOf(Instance),
                .step_mode = .instance,
                .attribute_count = @as(u32, @intCast(instance_attributes.len)),
                .attributes = instance_attributes.ptr,
            } };

            const pipeline_descriptor = wgpu.RenderPipelineDescriptor{
                .vertex = wgpu.VertexState{
                    .module = vs_module,
                    .entry_point = "main",
                    .buffer_count = vertex_buffers.len,
                    .buffers = &vertex_buffers,
                },
                .primitive = wgpu.PrimitiveState{
                    .front_face = .ccw,
                    .cull_mode = .back,
                    .topology = .triangle_strip,
                    .strip_index_format = .uint16,
                },
                .depth_stencil = &wgpu.DepthStencilState{
                    .format = .depth16_unorm,
                    .depth_write_enabled = true,
                    .depth_compare = .less,
                },
                .fragment = &wgpu.FragmentState{
                    .module = fs_module,
                    .entry_point = "main",
                    .target_count = color_targets.len,
                    .targets = &color_targets,
                },
                .multisample = .{
                    .count = 4,
                },
            };
            return gctx.createRenderPipeline(pipeline_layout, pipeline_descriptor);
        }
    };
}

fn ensureFourByteMultiple(size: usize) usize {
    return (size + 3) & ~@as(usize, 3);
}
