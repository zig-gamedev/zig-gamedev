const std = @import("std");
const math = std.math;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zm = @import("zmath");

const Layers = @import("layers.zig").State;

pub const Dimension = struct {
    width: f32,
    height: f32,
};

pub const State = struct {
    gctx: *zgpu.GraphicsContext,

    dimension: Dimension,

    layers: Layers,

    depth_texture: zgpu.TextureHandle,
    depth_texture_view: zgpu.TextureViewHandle,

    pub fn init(allocator: std.mem.Allocator, window: zglfw.Window) !State {
        const gctx = try zgpu.GraphicsContext.create(allocator, window);

        // Create a depth texture and its 'view'.
        const depth = createDepthTexture(gctx);

        return .{
            .gctx = gctx,
            .dimension = calculateDimensions(gctx),
            .layers = Layers.init(gctx, allocator),
            .depth_texture = depth.texture,
            .depth_texture_view = depth.view,
        };
    }

    pub fn deinit(self: *State, allocator: std.mem.Allocator) void {
        const gctx = self.gctx;
        self.layers.deinit();
        gctx.destroy(allocator);
    }

    pub fn draw(self: *State) void {
        const gctx = self.gctx;

        const back_buffer_view = gctx.swapchain.getCurrentTextureView();
        defer back_buffer_view.release();

        const depth_view = gctx.lookupResource(self.depth_texture_view) orelse return;
        const commands = commands: {
            const encoder = gctx.device.createCommandEncoder(null);
            defer encoder.release();

            self.layers.draw(self.dimension, back_buffer_view, depth_view, encoder);

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
        .format = .depth32_float,
        .mip_level_count = 1,
        .sample_count = 1,
    });
    const view = gctx.createTextureView(texture, .{});
    return .{ .texture = texture, .view = view };
}
