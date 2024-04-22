const std = @import("std");
const math = std.math;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");

const content_dir = @import("build_options").content_dir;

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

    color_texture: zgpu.TextureHandle,
    color_texture_view: zgpu.TextureViewHandle,

    depth_texture: zgpu.TextureHandle,
    depth_texture_view: zgpu.TextureViewHandle,

    pub fn init(allocator: std.mem.Allocator, window: *zglfw.Window) !State {
        const gctx = try zgpu.GraphicsContext.create(
            allocator,
            .{
                .window = window,
                .fn_getTime = @ptrCast(&zglfw.getTime),
                .fn_getFramebufferSize = @ptrCast(&zglfw.Window.getFramebufferSize),
                .fn_getWin32Window = @ptrCast(&zglfw.getWin32Window),
                .fn_getX11Display = @ptrCast(&zglfw.getX11Display),
                .fn_getX11Window = @ptrCast(&zglfw.getX11Window),
                .fn_getWaylandDisplay = @ptrCast(&zglfw.getWaylandDisplay),
                .fn_getWaylandSurface = @ptrCast(&zglfw.getWaylandWindow),
                .fn_getCocoaWindow = @ptrCast(&zglfw.getCocoaWindow),
            },
            .{},
        );
        errdefer gctx.destroy(allocator);

        zgui.init(allocator);
        const scale_factor = scale_factor: {
            const scale = window.getContentScale();
            break :scale_factor @max(scale[0], scale[1]);
        };
        _ = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", math.floor(16.0 * scale_factor));

        // This needs to be called *after* adding your custom fonts.
        zgui.backend.init(
            window,
            gctx.device,
            @intFromEnum(zgpu.GraphicsContext.swapchain_format),
            @intFromEnum(wgpu.TextureFormat.undef),
        );

        // Create a color/depth texture and its 'view'.
        const color = createColorTexture(gctx);
        const depth = createDepthTexture(gctx);

        return .{
            .gctx = gctx,

            .background_color = .{ .r = 0.0, .g = 0.0, .b = 0.0, .a = 1.0 },
            .dimension = calculateDimensions(gctx),
            .layers = std.ArrayList(Layer).init(allocator),

            .color_texture = color.texture,
            .color_texture_view = color.view,

            .depth_texture = depth.texture,
            .depth_texture_view = depth.view,
        };
    }

    pub fn deinit(self: *State, allocator: std.mem.Allocator) void {
        zgui.backend.deinit();
        zgui.deinit();
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

            .vertex_count = @as(u32, @intCast(layer.vertices.items.len)),
            .vertex_buffer = layer.vertex_buffer,
            .index_buffer = layer.index_buffer,

            .instance_count = @as(u32, @intCast(layer.instances.items.len)),
            .instance_buffer = layer.instance_buffer,
        });
    }

    fn drawLayers(
        self: *State,
        back_buffer_view: wgpu.TextureView,
        color_view: wgpu.TextureView,
        depth_view: wgpu.TextureView,
        encoder: wgpu.CommandEncoder,
    ) void {
        const gctx = self.gctx;

        const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
            .view = color_view,
            .resolve_target = back_buffer_view,
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

        zgui.backend.newFrame(
            gctx.swapchain_descriptor.width,
            gctx.swapchain_descriptor.height,
        );
        const draw_list = zgui.getBackgroundDrawList();
        draw_list.addText(
            .{ 10, 10 },
            0xff_ff_ff_ff,
            "{d:.3} ms/frame ({d:.1} fps)",
            .{ gctx.stats.average_cpu_time, gctx.stats.fps },
        );

        const back_buffer_view = gctx.swapchain.getCurrentTextureView();
        defer back_buffer_view.release();

        const color_view = gctx.lookupResource(self.color_texture_view) orelse return;
        const depth_view = gctx.lookupResource(self.depth_texture_view) orelse return;
        const commands = commands: {
            const encoder = gctx.device.createCommandEncoder(null);
            defer encoder.release();
            {
                const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
                    .view = color_view,
                    .resolve_target = back_buffer_view,
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
            self.drawLayers(back_buffer_view, color_view, depth_view, encoder);

            {
                const color_attachments = [_]wgpu.RenderPassColorAttachment{.{
                    .view = back_buffer_view,
                    .load_op = .load,
                    .store_op = .store,
                }};
                const render_pass_info = wgpu.RenderPassDescriptor{
                    .color_attachment_count = color_attachments.len,
                    .color_attachments = &color_attachments,
                };
                const pass = encoder.beginRenderPass(render_pass_info);
                defer {
                    pass.end();
                    pass.release();
                }
                zgui.backend.draw(pass);
            }

            break :commands encoder.finish(null);
        };
        defer commands.release();

        gctx.submit(&.{commands});
        if (gctx.present() == .swap_chain_resized) {
            self.dimension = calculateDimensions(gctx);

            // Release old color/depth texture.
            gctx.releaseResource(self.color_texture_view);
            gctx.destroyResource(self.color_texture);
            gctx.releaseResource(self.depth_texture_view);
            gctx.destroyResource(self.depth_texture);

            // Create a new color/depth texture to match the new window size.
            const color = createColorTexture(gctx);
            self.color_texture = color.texture;
            self.color_texture_view = color.view;
            const depth = createDepthTexture(gctx);
            self.depth_texture = depth.texture;
            self.depth_texture_view = depth.view;
        }
    }
};

fn calculateDimensions(gctx: *zgpu.GraphicsContext) Dimension {
    const width = @as(f32, @floatFromInt(gctx.swapchain_descriptor.width));
    const height = @as(f32, @floatFromInt(gctx.swapchain_descriptor.height));
    const delta = math.sign(
        @as(i32, @bitCast(gctx.swapchain_descriptor.width)) - @as(i32, @bitCast(gctx.swapchain_descriptor.height)),
    );
    return switch (delta) {
        -1 => .{ .width = 2.0, .height = 2 * width / height },
        0 => .{ .width = 2.0, .height = 2.0 },
        1 => .{ .width = 2 * height / width, .height = 2.0 },
        else => unreachable,
    };
}

fn createColorTexture(gctx: *zgpu.GraphicsContext) struct {
    texture: zgpu.TextureHandle,
    view: zgpu.TextureViewHandle,
} {
    const texture = gctx.createTexture(.{
        .usage = .{ .render_attachment = true },
        .dimension = .tdim_2d,
        .size = .{
            .width = gctx.swapchain_descriptor.width,
            .height = gctx.swapchain_descriptor.height,
        },
        .format = gctx.swapchain_descriptor.format,
        .sample_count = 4,
    });
    const view = gctx.createTextureView(texture, .{});
    return .{ .texture = texture, .view = view };
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
        .sample_count = 4,
    });
    const view = gctx.createTextureView(texture, .{});
    return .{ .texture = texture, .view = view };
}
