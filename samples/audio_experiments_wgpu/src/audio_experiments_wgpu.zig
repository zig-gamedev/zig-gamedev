const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const Mutex = std.Thread.Mutex;
const glfw = @import("glfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = zgpu.zgui;
const zmath = @import("zmath");
const zaudio = @import("zaudio");
const wgsl = @import("audio_experiments_wgsl.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: audio experiments (wgpu)";

const Vertex = extern struct {
    position: [3]f32,
    color: [3]f32,
};

const AudioState = struct {
    const num_sample_sets = 100;
    const samples_per_set = 512;
    const usable_samples_per_set = 480;

    device: zaudio.Device,
    engine: zaudio.Engine,
    mutex: Mutex = .{},
    current_sample_set: u32 = num_sample_sets - 1,
    samples: std.ArrayList(f32),

    fn audioPlaybackCallback(context: ?*anyopaque, outptr: *anyopaque, num_frames: u32) void {
        if (context == null) return;

        const audio = @ptrCast(*AudioState, @alignCast(@alignOf(AudioState), context));

        audio.engine.readPcmFrames(outptr, num_frames, null) catch {};
    }

    fn create(allocator: std.mem.Allocator) !*AudioState {
        const samples = samples: {
            var samples = std.ArrayList(f32).initCapacity(
                allocator,
                num_sample_sets * samples_per_set,
            ) catch unreachable;
            samples.expandToCapacity();
            for (samples.items) |*s| s.* = 0.0;
            break :samples samples;
        };

        const audio = try allocator.create(AudioState);

        const device = device: {
            var config = zaudio.DeviceConfig.init(.playback);
            config.playback_callback = .{
                .context = audio,
                .func = audioPlaybackCallback,
            };
            config.raw.playback.format = @enumToInt(zaudio.Format.float32);
            config.raw.playback.channels = 2;
            config.raw.sampleRate = 48_000;
            config.raw.periodSizeInFrames = 480;
            config.raw.periodSizeInMilliseconds = 10;
            break :device try zaudio.initDevice(allocator, null, &config);
        };

        const engine = engine: {
            var config = zaudio.EngineConfig.init();
            config.raw.pDevice = device.asRaw();
            config.raw.noAutoStart = 1;
            break :engine try zaudio.initEngine(allocator, config);
        };

        audio.* = .{
            .device = device,
            .engine = engine,
            .samples = samples,
        };
        return audio;
    }

    fn destroy(audio: *AudioState, allocator: std.mem.Allocator) void {
        audio.samples.deinit();
        audio.engine.deinit(allocator);
        audio.device.deinit(allocator);
        allocator.destroy(audio);
    }
};

const DemoState = struct {
    gctx: *zgpu.GraphicsContext,
    audio: *AudioState,

    lines_pipe: zgpu.RenderPipelineHandle = .{},

    depth_tex: zgpu.TextureHandle,
    depth_texv: zgpu.TextureViewHandle,

    music: zaudio.Sound,
};

fn create(allocator: std.mem.Allocator, window: glfw.Window) !*DemoState {
    const gctx = try zgpu.GraphicsContext.init(allocator, window);

    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    //const arena = arena_state.allocator();

    const uniform_bgl = gctx.createBindGroupLayout(&.{
        zgpu.bglBuffer(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
    });
    defer gctx.releaseResource(uniform_bgl);

    const depth = createDepthTexture(gctx);

    const audio = try AudioState.create(allocator);
    try audio.engine.start();

    const music = try audio.engine.initSoundFromFile(
        allocator,
        content_dir ++ "Broke For Free - Night Owl.mp3",
        .{ .flags = .{ .stream = true } },
    );
    music.setVolume(1.5);
    try music.start();

    const demo = try allocator.create(DemoState);
    demo.* = .{
        .gctx = gctx,
        .depth_tex = depth.tex,
        .depth_texv = depth.texv,
        .audio = audio,
        .music = music,
    };

    const common_depth_state = wgpu.DepthStencilState{
        .format = .depth32_float,
        .depth_write_enabled = true,
        .depth_compare = .less,
    };
    const pos_color_attribs = [_]wgpu.VertexAttribute{
        .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
        .{ .format = .float32x3, .offset = @offsetOf(Vertex, "color"), .shader_location = 1 },
    };
    zgpu.util.createRenderPipelineSimple(
        allocator,
        gctx,
        &.{ uniform_bgl, uniform_bgl },
        wgsl.lines_vs,
        wgsl.lines_fs,
        @sizeOf(Vertex),
        pos_color_attribs[0..],
        .{ .topology = .line_list },
        zgpu.GraphicsContext.swapchain_format,
        common_depth_state,
        &demo.lines_pipe,
    );

    return demo;
}

fn destroy(allocator: std.mem.Allocator, demo: *DemoState) void {
    demo.music.deinit(allocator);
    demo.gctx.deinit(allocator);
    demo.audio.destroy(allocator);
    allocator.destroy(demo);
}

fn update(demo: *DemoState) !void {
    zgpu.gui.newFrame(demo.gctx.swapchain_descriptor.width, demo.gctx.swapchain_descriptor.height);

    if (zgui.begin("Demo Settings", .{ .flags = .{ .no_move = true, .no_resize = true } })) {
        zgui.bullet();
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Average :");
        zgui.sameLine(.{});
        zgui.text(
            "  {d:.3} ms/frame ({d:.1} fps)",
            .{ demo.gctx.stats.average_cpu_time, demo.gctx.stats.fps },
        );

        zgui.spacing();
        zgui.textUnformatted("Music:");
        const music_is_playing = demo.music.isPlaying();
        if (zgui.button(if (music_is_playing) "Pause" else "Play", .{ .w = 200.0 })) {
            if (music_is_playing) {
                try demo.music.stop();
            } else try demo.music.start();
        }
        zgui.sameLine(.{ .offset_from_start_x = 0.0 });
        if (zgui.button("  Rewind  ", .{})) {
            try demo.music.seekToPcmFrame(0);
        }

        zgui.spacing();
        zgui.textUnformatted("Sounds:");
        if (zgui.button("  Play Sound 1  ", .{})) {
            try demo.audio.engine.playSound(content_dir ++ "drum_bass_hard.flac", null);
        }
        zgui.sameLine(.{});
        if (zgui.button("  Play Sound 2  ", .{})) {
            try demo.audio.engine.playSound(content_dir ++ "tabla_tas1.flac", null);
        }
        zgui.sameLine(.{});
        if (zgui.button("  Play Sound 3  ", .{})) {
            try demo.audio.engine.playSound(content_dir ++ "loop_mika.flac", null);
        }
    }
    zgui.end();
}

fn draw(demo: *DemoState) void {
    const gctx = demo.gctx;
    //const fb_width = gctx.swapchain_descriptor.width;
    //const fb_height = gctx.swapchain_descriptor.height;

    const swapchain_texv = gctx.swapchain.getCurrentTextureView();
    defer swapchain_texv.release();

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        // Gui pass.
        {
            const pass = zgpu.util.beginRenderPassSimple(encoder, .load, swapchain_texv, null, null, null);
            defer zgpu.util.endRelease(pass);
            zgpu.gui.draw(pass);
        }

        break :commands encoder.finish(null);
    };
    defer commands.release();

    gctx.submit(&.{commands});

    if (gctx.present() == .swap_chain_resized) {
        // Release old depth texture.
        gctx.releaseResource(demo.depth_texv);
        gctx.destroyResource(demo.depth_tex);

        // Create a new depth texture to match the new window size.
        const depth = createDepthTexture(gctx);
        demo.depth_tex = depth.tex;
        demo.depth_texv = depth.texv;
    }
}

fn createDepthTexture(gctx: *zgpu.GraphicsContext) struct {
    tex: zgpu.TextureHandle,
    texv: zgpu.TextureViewHandle,
} {
    const tex = gctx.createTexture(.{
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
    const texv = gctx.createTextureView(tex, .{});
    return .{ .tex = tex, .texv = texv };
}

pub fn main() !void {
    try glfw.init(.{});
    defer glfw.terminate();

    zgpu.checkSystem(content_dir) catch {
        // In case of error zgpu.checkSystem() will print error message.
        return;
    };

    const window = try glfw.Window.create(1600, 1000, window_title, null, null, .{
        .client_api = .no_api,
        .cocoa_retina_framebuffer = true,
    });
    defer window.destroy();
    try window.setSizeLimits(.{ .width = 400, .height = 400 }, .{ .width = null, .height = null });

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    const demo = try create(allocator, window);
    defer destroy(allocator, demo);

    zgpu.gui.init(window, demo.gctx.device, content_dir, "Roboto-Medium.ttf", 25.0);
    defer zgpu.gui.deinit();

    while (!window.shouldClose()) {
        try glfw.pollEvents();
        try update(demo);
        draw(demo);
    }
}
