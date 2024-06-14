const std = @import("std");

const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: frame pacing (wgpu)";

const Surface = struct {
    window: *zglfw.Window,
    gctx: *zgpu.GraphicsContext,
    scale_factor: f32,

    const Self = @This();

    fn init(allocator: std.mem.Allocator, monitor_video_mode: ?MonitorVideoMode) !Self {
        var width: i32 = 1280;
        var height: i32 = 720;

        // default windowed vsync on (monitor is null)
        var present_mode: wgpu.PresentMode = .fifo;

        var monitor: ?*zglfw.Monitor = null;
        if (monitor_video_mode) |mvm| {
            monitor = mvm.monitor;
            width = mvm.video_mode.width;
            height = mvm.video_mode.height;
            zglfw.windowHintTyped(.red_bits, mvm.video_mode.red_bits);
            zglfw.windowHintTyped(.green_bits, mvm.video_mode.green_bits);
            zglfw.windowHintTyped(.blue_bits, mvm.video_mode.blue_bits);
            zglfw.windowHintTyped(.refresh_rate, mvm.video_mode.refresh_rate);

            // vsync off
            present_mode = .immediate;
        }
        zglfw.windowHintTyped(.client_api, .no_api);
        const window = try zglfw.Window.create(width, height, window_title, monitor);

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
            .{
                .present_mode = present_mode,
            },
        );

        zgui.init(allocator);
        zgui.plot.init();

        zgui.backend.init(
            window,
            gctx.device,
            @intFromEnum(zgpu.GraphicsContext.swapchain_format),
            @intFromEnum(wgpu.TextureFormat.undef),
        );

        const scale_factor = scale_factor: {
            const scale = window.getContentScale();
            break :scale_factor @max(scale[0], scale[1]);
        };
        {
            _ = zgui.io.addFontFromFile(
                content_dir ++ "Roboto-Medium.ttf",
                std.math.floor(16.0 * scale_factor),
            );

            zgui.getStyle().scaleAllSizes(scale_factor);
        }
        return .{
            .window = window,
            .gctx = gctx,
            .scale_factor = scale_factor,
        };
    }

    fn reinit(self: *Self, allocator: std.mem.Allocator, monitor_video_mode: ?MonitorVideoMode) !void {
        self.deinit(allocator);
        const other = try Self.init(allocator, monitor_video_mode);
        self.window = other.window;
        self.gctx = other.gctx;
    }

    fn deinit(self: *Self, allocator: std.mem.Allocator) void {
        zgui.backend.deinit();
        zgui.plot.deinit();
        zgui.deinit();
        self.gctx.destroy(allocator);
        self.window.destroy();
    }
};

const XyData = std.DoublyLinkedList(struct {
    x: f64,
    y: f64,
});

const XyLine = struct {
    xv: []const f64,
    yv: []const f64,
    const Self = @This();

    fn init(allocator: std.mem.Allocator, xy_data: XyData) !Self {
        var xv = try allocator.alloc(f64, xy_data.len);
        var yv = try allocator.alloc(f64, xy_data.len);
        var current = xy_data.first;
        var i: usize = 0;
        while (current) |node| : ({
            current = node.next;
            i += 1;
        }) {
            xv[i] = node.data.x;
            yv[i] = node.data.y;
        }

        return .{
            .xv = xv,
            .yv = yv,
        };
    }

    fn deinit(self: Self, allocator: std.mem.Allocator) void {
        allocator.free(self.xv);
        allocator.free(self.yv);
    }
};

const MonitorVideoMode = struct {
    monitor: *zglfw.Monitor,
    video_mode: zglfw.VideoMode,
};

pub fn main() !void {
    try zglfw.init();
    defer zglfw.terminate();

    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        try std.posix.chdir(path);
    }

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    var surface = try Surface.init(allocator, null);
    defer surface.deinit(allocator);

    var frame_time_history: XyData = .{};
    defer while (frame_time_history.pop()) |frame_time| {
        allocator.destroy(frame_time);
    };

    const Mode = enum {
        windowed,
        fullscreen,
    };
    var mode: Mode = .windowed;

    const FrameTargetOption = enum {
        unlimited,
        frame_rate,
        frame_time,
    };
    var frame_target_option: FrameTargetOption = .unlimited;

    var frame_rate_target: i32 = 60;
    var frame_time_target: f32 = 16.666;

    var monitor_names = std.ArrayList([:0]const u8).init(allocator);
    defer {
        for (monitor_names.items) |monitor_name| {
            allocator.free(monitor_name);
        }
        monitor_names.deinit();
    }

    var monitor_video_modes = std.ArrayList(MonitorVideoMode).init(allocator);
    defer monitor_video_modes.deinit();

    if (zglfw.Monitor.getAll()) |monitors| {
        for (monitors) |monitor| {
            for (try monitor.getVideoModes()) |video_mode| {
                const bits = video_mode.red_bits + video_mode.green_bits + video_mode.blue_bits;
                try monitor_names.append(try std.fmt.allocPrintZ(allocator, "{s} {}×{} {}-bits {}hz", .{
                    try monitor.getName(),
                    video_mode.width,
                    video_mode.height,
                    bits,
                    video_mode.refresh_rate,
                }));
                try monitor_video_modes.append(.{
                    .monitor = monitor,
                    .video_mode = video_mode,
                });
            }
        }
    }
    var selected_monitor_video_mode: usize = 0;

    var reinit_surface: bool = false;

    var frame_timer = try std.time.Timer.start();
    main: while (!surface.window.shouldClose() and surface.window.getKey(.escape) != .press) {
        if (reinit_surface) {
            switch (mode) {
                .windowed => try surface.reinit(allocator, null),
                .fullscreen => {
                    const monitor_video_mode = monitor_video_modes.items[selected_monitor_video_mode];
                    try surface.reinit(allocator, monitor_video_mode);
                },
            }
            reinit_surface = false;
            continue :main;
        }

        // getCurrentTextureView blocks when vsync is on
        const swapchain_texture_view = surface.gctx.swapchain.getCurrentTextureView();
        defer swapchain_texture_view.release();

        {
            // spin loop for frame limiter
            var target_ns: ?u64 = null;
            if (frame_target_option == .frame_rate) {
                target_ns = @divTrunc(std.time.ns_per_s, @as(u64, @intCast(frame_rate_target)));
            }
            if (frame_target_option == .frame_time) {
                target_ns = @as(u64, @intFromFloat(std.time.ns_per_ms * frame_time_target));
            }
            if (target_ns) |t| {
                while (frame_timer.read() < t) {
                    std.atomic.spinLoopHint();
                }
                frame_timer.reset();
            }
        }

        // poll for input immediately after vsync or frame limiter to reduce input latency
        zglfw.pollEvents();

        {
            defer zgui.end();
            zgui.backend.newFrame(
                surface.gctx.swapchain_descriptor.width,
                surface.gctx.swapchain_descriptor.height,
            );

            zgui.setNextWindowPos(.{ .x = 20.0, .y = 20.0, .cond = .first_use_ever });
            zgui.setNextWindowSize(.{ .w = 600, .h = 500, .cond = .first_use_ever });

            if (zgui.begin("Config", .{})) {
                {
                    {
                        var frame_time_node = try allocator.create(XyData.Node);
                        frame_time_node.data.x = surface.gctx.stats.time;
                        frame_time_node.data.y = surface.gctx.stats.average_cpu_time;
                        frame_time_history.append(frame_time_node);
                        while (frame_time_history.len > 100) {
                            const node = frame_time_history.popFirst() orelse unreachable;
                            allocator.destroy(node);
                        }
                    }

                    zgui.text(
                        "{d:.3} ms/frame ({d:.1} fps)",
                        .{ surface.gctx.stats.average_cpu_time, surface.gctx.stats.fps },
                    );
                    if (zgui.plot.beginPlot("frame times", .{ .h = surface.scale_factor * 100 })) {
                        defer zgui.plot.endPlot();
                        zgui.plot.setupAxis(.x1, .{
                            .flags = .{
                                .no_tick_labels = true,
                                .auto_fit = true,
                            },
                        });
                        zgui.plot.setupAxisLimits(.y1, .{ .min = 0, .max = 50 });
                        zgui.plot.setupLegend(.{}, .{});
                        zgui.plot.setupFinish();

                        const frame_time_line = try XyLine.init(allocator, frame_time_history);
                        defer frame_time_line.deinit(allocator);
                        zgui.plot.plotLine("##frame times data", f64, .{
                            .xv = frame_time_line.xv,
                            .yv = frame_time_line.yv,
                        });
                    }
                }
                {
                    zgui.separatorText("Mode");
                    if (zgui.radioButton("windowed vsync on", .{ .active = mode == .windowed })) {
                        mode = .windowed;
                        reinit_surface = true;
                    }
                    {
                        if (zgui.radioButton("fullscreen vsync off", .{ .active = mode == .fullscreen })) {
                            mode = .fullscreen;
                            reinit_surface = true;
                        }
                        zgui.sameLine(.{});
                        {
                            if (zgui.beginCombo("##monitor", .{ .preview_value = monitor_names.items[selected_monitor_video_mode] })) {
                                defer zgui.endCombo();

                                for (monitor_names.items, 0..) |monitor_name, i| {
                                    if (zgui.selectable(monitor_name, .{ .selected = i == selected_monitor_video_mode })) {
                                        selected_monitor_video_mode = i;
                                        mode = .fullscreen;
                                        reinit_surface = true;
                                    }
                                }
                            }
                        }
                    }
                }
                {
                    zgui.separatorText("Limiter");
                    if (zgui.radioButton("unlimited", .{ .active = frame_target_option == .unlimited })) {
                        frame_target_option = .unlimited;
                    }
                    {
                        if (zgui.radioButton("frame rate", .{ .active = frame_target_option == .frame_rate })) {
                            frame_target_option = .frame_rate;
                        }
                        zgui.sameLine(.{});
                        {
                            zgui.beginDisabled(.{ .disabled = frame_target_option != .frame_rate });
                            defer zgui.endDisabled();

                            _ = zgui.sliderInt("##frame rate", .{ .v = &frame_rate_target, .min = 0, .max = 1000 });
                        }
                    }

                    {
                        if (zgui.radioButton("frame time (ms)", .{ .active = frame_target_option == .frame_time })) {
                            frame_target_option = .frame_time;
                        }
                        zgui.sameLine(.{});
                        {
                            zgui.beginDisabled(.{ .disabled = frame_target_option != .frame_time });
                            defer zgui.endDisabled();

                            _ = zgui.sliderFloat("##frame time", .{ .v = &frame_time_target, .min = 0, .max = 100 });
                        }
                    }
                }
            }
        }

        {
            const commands = commands: {
                const encoder = surface.gctx.device.createCommandEncoder(null);
                defer encoder.release();

                {
                    const pass = zgpu.beginRenderPassSimple(encoder, .load, swapchain_texture_view, null, null, null);
                    defer zgpu.endReleasePass(pass);

                    zgui.backend.draw(pass);
                }

                break :commands encoder.finish(null);
            };
            defer commands.release();

            surface.gctx.submit(&.{commands});
        }

        _ = surface.gctx.present();
    }
}
