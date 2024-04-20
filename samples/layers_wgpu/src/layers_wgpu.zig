const std = @import("std");
const math = std.math;
const zglfw = @import("zglfw");
const zm = @import("zmath");

const Graphics = @import("graphics.zig").State;

const pill = @import("pill.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: Layers (wgpu)";

const DemoState = struct {
    graphics: Graphics,

    hexagons: pill.Pills,
    pills: pill.Pills,

    fn init(allocator: std.mem.Allocator, window: *zglfw.Window) !DemoState {
        var graphics = try Graphics.init(allocator, window);
        graphics.background_color = .{ .r = 0.1, .g = 0.1, .b = 0.1, .a = 1.0 };

        return .{
            .graphics = graphics,
            .hexagons = try pill.init(graphics.gctx, allocator, 3),
            .pills = try pill.init(graphics.gctx, allocator, 12),
        };
    }

    fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
        demo.graphics.deinit(allocator);
        demo.hexagons.deinit();
        demo.pills.deinit();
    }

    fn update(demo: *DemoState, _: std.mem.Allocator) !void {
        {
            demo.hexagons.instances.clearRetainingCapacity();
            try demo.hexagons.instances.append(.{
                .width = 0.9,
                .length = 0.0,
                .angle = 0.0,
                .position = .{ 0.0, 0.0 },
                .depth = 0.1,
                .start_color = .{ 1.0, 0.0, 0.0, 1.0 },
                .end_color = .{ 1.0, 0.0, 0.0, 1.0 },
            });
            demo.hexagons.recreateInstanceBuffer();

            const vertex_uniforms_data = demo.graphics.gctx.uniformsAllocate(zm.Mat, 1);
            const object_to_clip = zm.scaling(demo.graphics.dimension.width / 2, demo.graphics.dimension.height / 2, 1.0);
            vertex_uniforms_data.slice[0] = zm.transpose(object_to_clip);
            demo.hexagons.vertex_uniforms_offsets.clearRetainingCapacity();
            try demo.hexagons.vertex_uniforms_offsets.append(vertex_uniforms_data.offset);
        }
        {
            demo.pills.instances.clearRetainingCapacity();
            try demo.pills.instances.append(.{
                .width = 0.1,
                .length = 1.8,
                .angle = -math.pi / 3.0,
                .position = .{ 0.0, 0.0 },
                .depth = 0.0,
                .start_color = .{ 1.0, 1.0, 1.0, 1.0 },
                .end_color = .{ 1.0, 1.0, 1.0, 1.0 },
            });
            try demo.pills.instances.append(.{
                .width = 0.1,
                .length = 1.8,
                .angle = math.pi / 3.0,
                .position = .{ 0.0, 0.0 },
                .depth = 0.2,
                .start_color = .{ 1.0, 0.0, 1.0, 1.0 },
                .end_color = .{ 1.0, 0.0, 1.0, 1.0 },
            });
            demo.pills.recreateInstanceBuffer();

            const vertex_uniforms_data = demo.graphics.gctx.uniformsAllocate(zm.Mat, 1);
            const object_to_clip = zm.scaling(demo.graphics.dimension.width / 2, demo.graphics.dimension.height / 2, 1.0);
            vertex_uniforms_data.slice[0] = zm.transpose(object_to_clip);
            demo.pills.vertex_uniforms_offsets.clearRetainingCapacity();
            try demo.pills.vertex_uniforms_offsets.append(vertex_uniforms_data.offset);
        }

        demo.graphics.layers.clearRetainingCapacity();
        try demo.graphics.addLayer(demo.pills);
        try demo.graphics.addLayer(demo.hexagons);
    }
};

pub fn main() !void {
    try zglfw.init();
    defer zglfw.terminate();

    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.posix.chdir(path) catch {};
    }

    zglfw.windowHintTyped(.client_api, .no_api);

    const window = try zglfw.Window.create(800, 800, window_title, null);
    defer window.destroy();
    window.setSizeLimits(400, 400, -1, -1);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = try DemoState.init(allocator, window);
    defer demo.deinit(allocator);

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();
        try demo.update(allocator);
        demo.graphics.draw();
    }
}
