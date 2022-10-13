const std = @import("std");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;

pub fn Layer(comptime Vertex: type, comptime Instance: type) type {
    return struct {
        const State = @This();

        gctx: *zgpu.GraphicsContext,

        vertices: std.ArrayList(Vertex),
        vertex_attributes: []const wgpu.VertexAttribute,
        vertex_stride: u32,
        vertex_buffer: zgpu.BufferHandle,
        vertex_shader: [*:0]const u8,

        indices: std.ArrayList(u16),
        index_buffer: zgpu.BufferHandle,

        instances: std.ArrayList(Instance),
        instance_attributes: []const wgpu.VertexAttribute,
        instance_stride: u32,
        instance_buffer: zgpu.BufferHandle,

        fragment_shader: [*:0]const u8,

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

                .vertices = std.ArrayList(Vertex).init(allocator),
                .vertex_attributes = vertex_attributes,
                .vertex_stride = @sizeOf(Vertex),
                .vertex_buffer = .{},
                .vertex_shader = vertex_shader,

                .indices = std.ArrayList(u16).init(allocator),
                .index_buffer = .{},

                .instances = std.ArrayList(Instance).init(allocator),
                .instance_attributes = instance_attributes,
                .instance_stride = @sizeOf(Instance),
                .instance_buffer = .{},

                .fragment_shader = fragment_shader,
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
    };
}

fn ensureFourByteMultiple(size: usize) usize {
    return (size + 3) & ~@as(usize, 3);
}
