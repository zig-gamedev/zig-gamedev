const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zm = @import("zmath");

const Graphics = @import("graphics.zig").State;

const pill = @import("pill.zig");
const vertex_generator = @import("vertex_generator.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: functional style (wgpu)";

const Pills = pill.State;
const Vertex = pill.Vertex;
const Instance = pill.Instance;

const DemoState = struct {
    graphics: Graphics,

    pills: Pills,

    fn init(allocator: std.mem.Allocator, window: zglfw.Window) !DemoState {
        const graphics = try Graphics.init(allocator, window);
        return .{
            .graphics = graphics,
            .pills = Pills.init(graphics.gctx, allocator),
        };
    }

    fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
        demo.graphics.deinit(allocator);
        demo.pills.deinit();
    }

    fn update(demo: *DemoState, _: std.mem.Allocator) !void {
        const segments: u16 = 7;
        demo.pills.element.vertices.clearRetainingCapacity();
        try vertex_generator.generateVertices(segments, &demo.pills.element.vertices);
        demo.pills.element.recreateVertexBuffer();

        demo.pills.element.indices.clearRetainingCapacity();
        try vertex_generator.generateIndices(segments, &demo.pills.element.indices);
        demo.pills.element.recreateIndexBuffer();

        demo.pills.element.instances.clearRetainingCapacity();
        const length: f32 = 0.5;
        const width: f32 = 0.1;
        const angle: f32 = math.pi / 3.0;
        const position: [2]f32 = .{ 0.5, -0.25 };
        const start_color: [4]f32 = .{ 1.0, 0.0, 0.0, 1.0 };
        const end_color: [4]f32 = .{ 0.0, 0.0, 1.0, 1.0 };
        try demo.pills.element.instances.append(.{
            .width = width,
            .length = length,
            .angle = angle,
            .position = position,
            .start_color = start_color,
            .end_color = end_color,
        });
        demo.pills.element.recreateInstanceBuffer();

        demo.graphics.layers.layers.clearRetainingCapacity();
        try demo.graphics.layers.layers.append(demo.pills.element.getLayer());
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
