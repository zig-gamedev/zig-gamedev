const std = @import("std");
const math = std.math;
const zglfw = @import("zglfw");

const Graphics = @import("graphics.zig").State;

const pill = @import("pill.zig");
const vertex_generator = @import("vertex_generator.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: Layers (wgpu)";

const DemoState = struct {
    graphics: Graphics,

    hexagons: pill.Pills,
    pills: pill.Pills,

    fn init(allocator: std.mem.Allocator, window: zglfw.Window) !DemoState {
        const graphics = try Graphics.init(allocator, window);

        var hexagons = pill.init(graphics.gctx, allocator);
        {
            const hexagonSegments: u16 = 3;
            hexagons.vertices.clearRetainingCapacity();
            try vertex_generator.generateVertices(hexagonSegments, &hexagons.vertices);
            hexagons.recreateVertexBuffer();

            hexagons.indices.clearRetainingCapacity();
            try vertex_generator.generateIndices(hexagonSegments, &hexagons.indices);
            hexagons.recreateIndexBuffer();
        }

        var pills = pill.init(graphics.gctx, allocator);
        {
            const pillSegments: u16 = 10;
            pills.vertices.clearRetainingCapacity();
            try vertex_generator.generateVertices(pillSegments, &pills.vertices);
            pills.recreateVertexBuffer();

            pills.indices.clearRetainingCapacity();
            try vertex_generator.generateIndices(pillSegments, &pills.indices);
            pills.recreateIndexBuffer();
        }
        return .{
            .graphics = graphics,
            .hexagons = hexagons,
            .pills = pills,
        };
    }

    fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
        demo.graphics.deinit(allocator);
        demo.hexagons.deinit();
        demo.pills.deinit();
    }

    fn update(demo: *DemoState, _: std.mem.Allocator) !void {
        demo.hexagons.instances.clearRetainingCapacity();
        try demo.hexagons.instances.append(.{
            .width = 0.4,
            .length = 0.0,
            .angle = 0.0,
            .position = .{ 0.0, 0.0 },
            .depth = 0.1,
            .start_color = .{ 1.0, 0.0, 0.0, 1.0 },
            .end_color = .{ 1.0, 0.0, 0.0, 1.0 },
        });
        demo.hexagons.recreateInstanceBuffer();

        demo.pills.instances.clearRetainingCapacity();
        try demo.pills.instances.append(.{
            .width = 0.05,
            .length = 0.8,
            .angle = -math.pi / 3.0,
            .position = .{ 0.0, 0.0 },
            .depth = 0.0,
            .start_color = .{ 1.0, 1.0, 1.0, 1.0 },
            .end_color = .{ 1.0, 1.0, 1.0, 1.0 },
        });
        try demo.pills.instances.append(.{
            .width = 0.05,
            .length = 0.8,
            .angle = math.pi / 3.0,
            .position = .{ 0.0, 0.0 },
            .depth = 0.2,
            .start_color = .{ 1.0, 1.0, 1.0, 1.0 },
            .end_color = .{ 1.0, 1.0, 1.0, 1.0 },
        });
        demo.pills.recreateInstanceBuffer();

        demo.graphics.layers.clearRetainingCapacity();
        try demo.graphics.addLayer(demo.pills);
        try demo.graphics.addLayer(demo.hexagons);
    }
};

pub fn main() !void {
    zglfw.init() catch {
        std.log.err("Failed to initialize GLFW library.", .{});
        return;
    };
    defer zglfw.terminate();

    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        std.os.chdir(path) catch {};
    }

    zglfw.defaultWindowHints();
    zglfw.windowHint(.cocoa_retina_framebuffer, 1);
    zglfw.windowHint(.client_api, 0);
    const window = zglfw.createWindow(1600, 1000, window_title, null, null) catch {
        std.log.err("Failed to create demo window.", .{});
        return;
    };
    defer window.destroy();
    window.setSizeLimits(400, 400, -1, -1);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = DemoState.init(allocator, window) catch {
        std.log.err("Failed to initialize the demo.", .{});
        return;
    };
    defer demo.deinit(allocator);

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();
        try demo.update(allocator);
        demo.graphics.draw();
    }
}
