const std = @import("std");
const math = std.math;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;

const Dimension = struct {
    width: f32,
    height: f32,
};

const BindGroup = struct {
    group: u32,
    bind_group: zgpu.BindGroupHandle,
    offsets: []const u32,
};

const Layer = struct {
    common_uniforms: ?BindGroup,
    vertex_uniforms: ?BindGroup,
    fragment_uniforms: ?BindGroup,

    pipeline: zgpu.RenderPipelineHandle,

    vertex_count: u32,
    vertex_buffer: zgpu.BufferHandle,

    index_buffer: zgpu.BufferHandle,

    instance_count: u32,
    instance_buffer: zgpu.BufferHandle,
};

pub const State = struct {
    gctx: *zgpu.GraphicsContext,

    background_color: wgpu.Color,
    dimension: Dimension,
    layers: std.ArrayList(Layer),

    depth_texture: zgpu.TextureHandle,
    depth_texture_view: zgpu.TextureViewHandle,

    pub fn init(allocator: std.mem.Allocator, window: zglfw.Window) !State {
        const gctx = try zgpu.GraphicsContext.create(allocator, window);

        // Create a depth texture and its 'view'.
        const depth = createDepthTexture(gctx);

        return .{
            .gctx = gctx,

            .background_color = .{ .r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0 },
            .dimension = calculateDimensions(gctx),
            .layers = std.ArrayList(Layer).init(allocator),

            .depth_texture = depth.texture,
            .depth_texture_view = depth.view,
        };
    }

    pub fn deinit(self: *State, allocator: std.mem.Allocator) void {
        const gctx = self.gctx;
        self.layers.deinit();
        gctx.destroy(allocator);
    }

    pub fn addLayer(self: *State, layer: anytype) !void {
        try self.layers.append(.{
            .common_uniforms = if (layer.common_uniforms) |cu| .{
                .group = cu.group,
                .bind_group = cu.bind_group,
                .offsets = layer.common_uniforms_offsets.items,
            } else null,
            .vertex_uniforms = if (layer.vertex_uniforms) |vu| .{
                .group = vu.group,
                .bind_group = vu.bind_group,
                .offsets = layer.vertex_uniforms_offsets.items,
            } else null,
            .fragment_uniforms = if (layer.fragment_uniforms) |fu| .{
                .group = fu.group,
                .bind_group = fu.bind_group,
                .offsets = layer.fragment_uniforms_offsets.items,
            } else null,
            .pipeline = layer.pipeline,

            .vertex_count = @intCast(u32, layer.vertices.items.len),
            .vertex_buffer = layer.vertex_buffer,
            .index_buffer = layer.index_buffer,

            .instance_count = @intCast(u32, layer.instances.items.len),
            .instance_buffer = layer.instance_buffer,
        });
    }

    fn drawLayers(self: *State, back_buffer_view: wgpu.TextureView, depth_view: wgpu.TextureView, encoder: wgpu.CommandEncoder) void {
        const gctx = self.gctx;

        const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
            .view = back_buffer_view,
            .load_op = .load,
            .store_op = .store,
        }};
        const depth_attachment = wgpu.RenderPassDepthStencilAttachment{
            .view = depth_view,
            .depth_load_op = .load,
            .depth_store_op = .store,
            .depth_clear_value = 1.0,
        };
        const render_pass_info = wgpu.RenderPassDescriptor{
            .color_attachment_count = color_attachments.len,
            .color_attachments = &color_attachments,
            .depth_stencil_attachment = &depth_attachment,
        };
        for (self.layers.items) |layer| {
            const pass = encoder.beginRenderPass(render_pass_info);
            defer {
                pass.end();
                pass.release();
            }

            const pipeline = gctx.lookupResource(layer.pipeline) orelse continue;
            const vb_info = gctx.lookupResourceInfo(layer.vertex_buffer) orelse continue;
            const itb_info = gctx.lookupResourceInfo(layer.instance_buffer) orelse continue;
            const idb_info = gctx.lookupResourceInfo(layer.index_buffer) orelse continue;

            pass.setVertexBuffer(0, vb_info.gpuobj.?, 0, vb_info.size);
            pass.setVertexBuffer(1, itb_info.gpuobj.?, 0, itb_info.size);

            pass.setIndexBuffer(idb_info.gpuobj.?, .uint16, 0, idb_info.size);

            pass.setPipeline(pipeline);

            if (layer.common_uniforms) |cu| {
                const bind_group = gctx.lookupResource(cu.bind_group) orelse continue;
                pass.setBindGroup(cu.group, bind_group, cu.offsets);
            }
            if (layer.vertex_uniforms) |vu| {
                const bind_group = gctx.lookupResource(vu.bind_group) orelse continue;
                pass.setBindGroup(vu.group, bind_group, vu.offsets);
            }
            if (layer.fragment_uniforms) |fu| {
                const bind_group = gctx.lookupResource(fu.bind_group) orelse continue;
                pass.setBindGroup(fu.group, bind_group, fu.offsets);
            }

            pass.drawIndexed(layer.vertex_count, layer.instance_count, 0, 0, 0);
        }
    }

    pub fn draw(self: *State) void {
        const gctx = self.gctx;

        const back_buffer_view = gctx.swapchain.getCurrentTextureView();
        defer back_buffer_view.release();

        const depth_view = gctx.lookupResource(self.depth_texture_view) orelse return;
        const commands = commands: {
            const encoder = gctx.device.createCommandEncoder(null);
            defer encoder.release();
            {
                const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
                    .view = back_buffer_view,
                    .load_op = .clear,
                    .store_op = .store,
                    .clear_value = self.background_color,
                }};
                const depth_attachment = wgpu.RenderPassDepthStencilAttachment{
                    .view = depth_view,
                    .depth_load_op = .clear,
                    .depth_store_op = .store,
                    .depth_clear_value = 1.0,
                };
                const render_pass_info = wgpu.RenderPassDescriptor{
                    .color_attachment_count = color_attachments.len,
                    .color_attachments = &color_attachments,
                    .depth_stencil_attachment = &depth_attachment,
                };
                const pass = encoder.beginRenderPass(render_pass_info);
                defer {
                    pass.end();
                    pass.release();
                }
            }
            self.drawLayers(back_buffer_view, depth_view, encoder);

            break :commands encoder.finish(null);
        };
        defer commands.release();

        gctx.submit(&.{commands});
        if (gctx.present() == .swap_chain_resized) {
            self.dimension = calculateDimensions(gctx);

            // Release old depth texture.
            gctx.releaseResource(self.depth_texture_view);
            gctx.destroyResource(self.depth_texture);

            // Create a new depth texture to match the new window size.
            const depth = createDepthTexture(gctx);
            self.depth_texture = depth.texture;
            self.depth_texture_view = depth.view;
        }
    }
};

fn calculateDimensions(gctx: *zgpu.GraphicsContext) Dimension {
    const width = @intToFloat(f32, gctx.swapchain_descriptor.width);
    const height = @intToFloat(f32, gctx.swapchain_descriptor.height);
    const delta = math.sign(
        @bitCast(i32, gctx.swapchain_descriptor.width) - @bitCast(i32, gctx.swapchain_descriptor.height),
    );
    return switch (delta) {
        -1 => .{ .width = 2.0, .height = 2 * width / height },
        0 => .{ .width = 2.0, .height = 2.0 },
        1 => .{ .width = 2 * height / width, .height = 2.0 },
        else => unreachable,
    };
}

fn createDepthTexture(gctx: *zgpu.GraphicsContext) struct {
    texture: zgpu.TextureHandle,
    view: zgpu.TextureViewHandle,
} {
    const texture = gctx.createTexture(.{
        .usage = .{ .render_attachment = true },
        .dimension = .tdim_2d,
        .size = .{
            .width = gctx.swapchain_descriptor.width,
            .height = gctx.swapchain_descriptor.height,
            .depth_or_array_layers = 1,
        },
        .format = .depth16_unorm,
        .mip_level_count = 1,
        .sample_count = 1,
    });
    const view = gctx.createTextureView(texture, .{});
    return .{ .texture = texture, .view = view };
}
