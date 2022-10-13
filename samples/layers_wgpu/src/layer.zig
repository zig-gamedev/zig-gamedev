const std = @import("std");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zm = @import("zmath");

pub fn Layer(comptime Vertex: type, comptime Instance: type) type {
    return struct {
        const State = @This();

        gctx: *zgpu.GraphicsContext,

        bind_group: zgpu.BindGroupHandle,
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
            vertex_attributes: []const wgpu.VertexAttribute,
            instance_attributes: []const wgpu.VertexAttribute,
            vertex_shader: [*:0]const u8,
            fragment_shader: [*:0]const u8,
        ) State {
            return .{
                .gctx = gctx,

                .bind_group = createBindGroup(gctx),
                .pipeline = createPipeline(gctx, vertex_attributes, instance_attributes, vertex_shader, fragment_shader),

                .vertices = std.ArrayList(Vertex).init(allocator),
                .vertex_buffer = .{},

                .indices = std.ArrayList(u16).init(allocator),
                .index_buffer = .{},

                .instances = std.ArrayList(Instance).init(allocator),
                .instance_buffer = .{},
            };
        }

        pub fn deinit(self: *State) void {
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

        fn createBindGroup(gctx: *zgpu.GraphicsContext) zgpu.BindGroupHandle {
            const bind_group_layout = gctx.createBindGroupLayout(&.{
                zgpu.bufferEntry(0, .{ .vertex = true }, .uniform, true, 0),
            });
            defer gctx.releaseResource(bind_group_layout);

            return gctx.createBindGroup(bind_group_layout, &[_]zgpu.BindGroupEntryInfo{.{
                .binding = 0,
                .buffer_handle = gctx.uniforms.buffer,
                .offset = 0,
                .size = @sizeOf(zm.Mat),
            }});
        }

        fn createPipeline(
            gctx: *zgpu.GraphicsContext,
            vertex_attributes: []const wgpu.VertexAttribute,
            instance_attributes: []const wgpu.VertexAttribute,
            vertex_shader: [*:0]const u8,
            fragment_shader: [*:0]const u8,
        ) zgpu.RenderPipelineHandle {
            const bind_group_layout = gctx.createBindGroupLayout(&.{
                zgpu.bufferEntry(0, .{ .vertex = true }, .uniform, true, 0),
            });
            defer gctx.releaseResource(bind_group_layout);

            const pipeline_layout = gctx.createPipelineLayout(&.{bind_group_layout});
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
                .attribute_count = @intCast(u32, vertex_attributes.len),
                .attributes = vertex_attributes.ptr,
            }, .{
                .array_stride = @sizeOf(Instance),
                .step_mode = .instance,
                .attribute_count = @intCast(u32, instance_attributes.len),
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
            };
            return gctx.createRenderPipeline(pipeline_layout, pipeline_descriptor);
        }
    };
}

fn ensureFourByteMultiple(size: usize) usize {
    return (size + 3) & ~@as(usize, 3);
}
