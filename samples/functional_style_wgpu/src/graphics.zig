const std = @import("std");
const math = std.math;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zm = @import("zmath");

const Dimension = struct {
    width: f32,
    height: f32,
};

const Layer = struct {
    vertex_attributes: []const wgpu.VertexAttribute,
    vertex_shader: [*:0]const u8,
    vertex_count: u32,
    vertex_stride: u64,
    vertex_buffer: zgpu.BufferHandle,

    index_buffer: zgpu.BufferHandle,

    instance_count: u32,
    instance_attributes: []const wgpu.VertexAttribute,
    instance_stride: u64,
    instance_buffer: zgpu.BufferHandle,

    fragment_shader: [*:0]const u8,
};

pub const State = struct {
    gctx: *zgpu.GraphicsContext,

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
            .vertex_attributes = layer.vertex_attributes,
            .vertex_shader = layer.vertex_shader,
            .vertex_count = @intCast(u32, layer.vertices.items.len),
            .vertex_stride = layer.vertex_stride,
            .vertex_buffer = layer.vertex_buffer,
            .index_buffer = layer.index_buffer,

            .instance_count = @intCast(u32, layer.instances.items.len),
            .instance_attributes = layer.instance_attributes,
            .instance_stride = layer.instance_stride,
            .instance_buffer = layer.instance_buffer,

            .fragment_shader = layer.fragment_shader,
        });
    }

    pub fn drawLayers(self: *State, back_buffer_view: wgpu.TextureView, depth_view: wgpu.TextureView, encoder: wgpu.CommandEncoder) void {
        const gctx = self.gctx;

        const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
            .view = back_buffer_view,
            .load_op = .clear,
            .store_op = .store,
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
        for (self.layers.items) |layer| {
            const pass = encoder.beginRenderPass(render_pass_info);
            defer {
                pass.end();
                pass.release();
            }

            const vb_info = gctx.lookupResourceInfo(layer.vertex_buffer) orelse continue;
            const itb_info = gctx.lookupResourceInfo(layer.instance_buffer) orelse continue;
            const idb_info = gctx.lookupResourceInfo(layer.index_buffer) orelse continue;

            pass.setVertexBuffer(0, vb_info.gpuobj.?, 0, vb_info.size);
            pass.setVertexBuffer(1, itb_info.gpuobj.?, 0, itb_info.size);

            pass.setIndexBuffer(idb_info.gpuobj.?, .uint16, 0, idb_info.size);

            pass.setPipeline(createPipeline(gctx, layer));

            const object_to_clip = zm.scaling(self.dimension.width / 2, self.dimension.height / 2, 1.0);

            const mem = gctx.uniformsAllocate(zm.Mat, 1);
            mem.slice[0] = zm.transpose(object_to_clip);

            pass.setBindGroup(0, createBindGroup(gctx), &.{mem.offset});
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

fn createBindGroup(gctx: *zgpu.GraphicsContext) wgpu.BindGroup {
    const bind_group_layout = gctx.createBindGroupLayout(&.{
        zgpu.bufferEntry(0, .{ .vertex = true }, .uniform, true, 0),
    });
    defer gctx.releaseResource(bind_group_layout);

    const handle = gctx.createBindGroup(bind_group_layout, &[_]zgpu.BindGroupEntryInfo{.{
        .binding = 0,
        .buffer_handle = gctx.uniforms.buffer,
        .offset = 0,
        .size = @sizeOf(zm.Mat),
    }});
    return gctx.lookupResource(handle).?;
}

fn createPipeline(gctx: *zgpu.GraphicsContext, layer: Layer) wgpu.RenderPipeline {
    const bind_group_layout = gctx.createBindGroupLayout(&.{
        zgpu.bufferEntry(0, .{ .vertex = true }, .uniform, true, 0),
    });
    defer gctx.releaseResource(bind_group_layout);

    const pipeline_layout = gctx.createPipelineLayout(&.{bind_group_layout});
    defer gctx.releaseResource(pipeline_layout);

    const vs_module = zgpu.createWgslShaderModule(gctx.device, layer.vertex_shader, "vs");
    defer vs_module.release();

    const fs_module = zgpu.createWgslShaderModule(gctx.device, layer.fragment_shader, "fs");
    defer fs_module.release();

    const color_targets = [_]wgpu.ColorTargetState{.{
        .format = zgpu.GraphicsContext.swapchain_format,
    }};

    const vertex_buffers = [_]wgpu.VertexBufferLayout{ .{
        .array_stride = layer.vertex_stride,
        .attribute_count = @intCast(u32, layer.vertex_attributes.len),
        .attributes = layer.vertex_attributes.ptr,
    }, .{
        .array_stride = layer.instance_stride,
        .step_mode = .instance,
        .attribute_count = @intCast(u32, layer.instance_attributes.len),
        .attributes = layer.instance_attributes.ptr,
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
    const handle = gctx.createRenderPipeline(pipeline_layout, pipeline_descriptor);
    return gctx.lookupResource(handle).?;
}
