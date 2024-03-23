const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const Mutex = std.Thread.Mutex;
const zglfw = @import("zglfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = @import("zgui");
const zm = @import("zmath");
const zaudio = @import("zaudio");
const wgsl = @import("audio_experiments_wgsl.zig");

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: audio experiments (wgpu)";

const safe_uniform_size = 256;
const min_filter_fequency: f32 = 20.0;
const max_filter_fequency: f32 = 500.0;
const min_filter_q: f32 = 0.02;
const max_filter_q: f32 = 1.0;
const min_filter_gain: f32 = -20.0;
const max_filter_gain: f32 = 20.0;
const filter_order: u32 = 4;

const Vertex = extern struct {
    position: [3]f32,
    color: [3]f32,
};

const FrameUniforms = struct {
    world_to_clip: zm.Mat,
};

const DrawUniforms = struct {
    object_to_world: zm.Mat,
};

const AudioFilterType = enum {
    lpf,
    hpf,
    notch,
    peak,
    loshelf,
    hishelf,

    const names = [_][:0]const u8{
        "Low-Pass Filter",
        "High-Pass Filter",
        "Notch Filter",
        "Peak Filter",
        "Low Shelf Filter",
        "High Shelf Filter",
    };
};

const AudioFilter = struct {
    current_type: AudioFilterType = .lpf,
    is_enabled: bool = false,

    lpf: struct {
        config: zaudio.LpfConfig,
        node: *zaudio.LpfNode,
    },
    hpf: struct {
        config: zaudio.HpfConfig,
        node: *zaudio.HpfNode,
    },
    notch: struct {
        config: zaudio.NotchConfig,
        node: *zaudio.NotchNode,
    },
    peak: struct {
        config: zaudio.PeakConfig,
        node: *zaudio.PeakNode,
    },
    loshelf: struct {
        config: zaudio.LoshelfConfig,
        node: *zaudio.LoshelfNode,
    },
    hishelf: struct {
        config: zaudio.HishelfConfig,
        node: *zaudio.HishelfNode,
    },

    fn getCurrentNode(filter: AudioFilter) *zaudio.Node {
        return switch (filter.current_type) {
            .lpf => filter.lpf.node.asNodeMut(),
            .hpf => filter.hpf.node.asNodeMut(),
            .notch => filter.notch.node.asNodeMut(),
            .peak => filter.peak.node.asNodeMut(),
            .loshelf => filter.loshelf.node.asNodeMut(),
            .hishelf => filter.hishelf.node.asNodeMut(),
        };
    }

    fn destroy(filter: AudioFilter) void {
        filter.lpf.node.destroy();
        filter.hpf.node.destroy();
        filter.notch.node.destroy();
        filter.peak.node.destroy();
        filter.loshelf.node.destroy();
        filter.hishelf.node.destroy();
    }
};

const AudioState = struct {
    const num_sets = 100;
    const samples_per_set = 512;
    const usable_samples_per_set = 480;

    device: *zaudio.Device,
    engine: *zaudio.Engine,
    mutex: Mutex = .{},
    current_set: u32 = num_sets - 1,
    samples: std.ArrayList(f32),

    fn audioCallback(
        device: *zaudio.Device,
        output: ?*anyopaque,
        _: ?*const anyopaque,
        num_frames: u32,
    ) callconv(.C) void {
        const audio = @as(*AudioState, @ptrCast(@alignCast(device.getUserData())));

        audio.engine.readPcmFrames(output.?, num_frames, null) catch {};

        audio.mutex.lock();
        defer audio.mutex.unlock();

        audio.current_set = (audio.current_set + 1) % num_sets;

        const num_channels = 2;
        const base_index = samples_per_set * audio.current_set;
        const frames = @as([*]f32, @ptrCast(@alignCast(output)));

        var i: u32 = 0;
        while (i < @min(num_frames, usable_samples_per_set)) : (i += 1) {
            audio.samples.items[base_index + i] = frames[i * num_channels];
        }
    }

    fn create(allocator: std.mem.Allocator) !*AudioState {
        const samples = samples: {
            var samples = try std.ArrayList(f32).initCapacity(
                allocator,
                num_sets * samples_per_set,
            );
            samples.expandToCapacity();
            @memset(samples.items, 0.0);
            break :samples samples;
        };

        const audio = try allocator.create(AudioState);

        const device = device: {
            var config = zaudio.Device.Config.init(.playback);
            config.data_callback = audioCallback;
            config.user_data = audio;
            config.sample_rate = 48_000;
            config.period_size_in_frames = 480;
            config.period_size_in_milliseconds = 10;
            config.playback.format = .float32;
            config.playback.channels = 2;
            break :device try zaudio.Device.create(null, config);
        };

        const engine = engine: {
            var config = zaudio.Engine.Config.init();
            config.device = device;
            config.no_auto_start = .true32;
            break :engine try zaudio.Engine.create(config);
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
        audio.engine.destroy();
        audio.device.destroy();
        allocator.destroy(audio);
    }
};

const DemoState = struct {
    allocator: std.mem.Allocator,

    window: *zglfw.Window,
    gctx: *zgpu.GraphicsContext,
    audio: *AudioState,

    lines_pipe: zgpu.RenderPipelineHandle = .{},

    uniform_bg: zgpu.BindGroupHandle,

    depth_tex: zgpu.TextureHandle,
    depth_texv: zgpu.TextureViewHandle,

    music: *zaudio.Sound,
    sounds: std.ArrayList(*zaudio.Sound),
    audio_filter: AudioFilter,

    waveform_config: zaudio.Waveform.Config,
    waveform_data_source: *zaudio.Waveform,
    waveform_node: *zaudio.DataSourceNode,

    noise_config: zaudio.Noise.Config,
    noise_data_source: *zaudio.Noise,
    noise_node: *zaudio.DataSourceNode,

    camera: struct {
        position: [3]f32 = .{ -10.0, 15.0, -10.0 },
        forward: [3]f32 = .{ 0.0, 0.0, 1.0 },
        pitch: f32 = 0.15 * math.pi,
        yaw: f32 = 0.25 * math.pi,
    } = .{},
    mouse: struct {
        cursor_pos: [2]f64 = .{ 0, 0 },
    } = .{},
};

fn create(allocator: std.mem.Allocator, window: *zglfw.Window) !*DemoState {
    const gctx = try zgpu.GraphicsContext.create(
        allocator,
        .{
            .window = window,
            .fn_getTime = @ptrCast(&zglfw.getTime),
            .fn_getFramebufferSize = @ptrCast(&zglfw.Window.getFramebufferSize),
            .fn_getWin32Window = @ptrCast(&zglfw.getWin32Window),
            .fn_getX11Display = @ptrCast(&zglfw.getX11Display),
            .fn_getX11Window = @ptrCast(&zglfw.getX11Window),
            .fn_getCocoaWindow = @ptrCast(&zglfw.getCocoaWindow),
        },
        .{},
    );
    errdefer gctx.destroy(allocator);

    var arena_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_state.deinit();
    //const arena = arena_state.allocator();

    const uniform_bgl = gctx.createBindGroupLayout(&.{
        zgpu.bufferEntry(0, .{ .vertex = true, .fragment = true }, .uniform, true, 0),
    });
    defer gctx.releaseResource(uniform_bgl);

    const uniform_bg = gctx.createBindGroup(uniform_bgl, &[_]zgpu.BindGroupEntryInfo{.{
        .binding = 0,
        .buffer_handle = gctx.uniforms.buffer,
        .offset = 0,
        .size = safe_uniform_size,
    }});

    const depth = createDepthTexture(gctx);

    zaudio.init(allocator);

    const audio = try AudioState.create(allocator);
    try audio.engine.start();

    const sounds = sounds: {
        var sounds = std.ArrayList(*zaudio.Sound).init(allocator);
        try sounds.append(try audio.engine.createSoundFromFile(content_dir ++ "drum_bass_hard.flac", .{}));
        try sounds.append(try audio.engine.createSoundFromFile(content_dir ++ "tabla_tas1.flac", .{}));
        try sounds.append(try audio.engine.createSoundFromFile(content_dir ++ "loop_mika.flac", .{}));
        break :sounds sounds;
    };

    const music = try audio.engine.createSoundFromFile(
        content_dir ++ "Broke For Free - Night Owl.mp3",
        .{ .flags = .{ .stream = true } },
    );
    music.setVolume(1.5);
    try music.start();

    const audio_filter = audio_filter: {
        const lpf_config = zaudio.LpfNode.Config.init(
            audio.engine.getChannels(),
            audio.engine.getSampleRate(),
            min_filter_fequency,
            filter_order,
        );
        const hpf_config = zaudio.HpfNode.Config.init(
            audio.engine.getChannels(),
            audio.engine.getSampleRate(),
            min_filter_fequency,
            filter_order,
        );
        const notch_config = zaudio.NotchNode.Config.init(
            audio.engine.getChannels(),
            audio.engine.getSampleRate(),
            min_filter_q,
            min_filter_fequency,
        );
        const peak_config = zaudio.PeakNode.Config.init(
            audio.engine.getChannels(),
            audio.engine.getSampleRate(),
            min_filter_gain,
            min_filter_q,
            min_filter_fequency,
        );
        const loshelf_config = zaudio.LoshelfNode.Config.init(
            audio.engine.getChannels(),
            audio.engine.getSampleRate(),
            min_filter_gain,
            min_filter_q,
            min_filter_fequency,
        );
        const hishelf_config = zaudio.HishelfNode.Config.init(
            audio.engine.getChannels(),
            audio.engine.getSampleRate(),
            min_filter_gain,
            min_filter_q,
            min_filter_fequency,
        );

        const audio_filter = AudioFilter{
            .lpf = .{
                .config = lpf_config.lpf,
                .node = try audio.engine.createLpfNode(lpf_config),
            },
            .hpf = .{
                .config = hpf_config.hpf,
                .node = try audio.engine.createHpfNode(hpf_config),
            },
            .notch = .{
                .config = notch_config.notch,
                .node = try audio.engine.createNotchNode(notch_config),
            },
            .peak = .{
                .config = peak_config.peak,
                .node = try audio.engine.createPeakNode(peak_config),
            },
            .loshelf = .{
                .config = loshelf_config.loshelf,
                .node = try audio.engine.createLoshelfNode(loshelf_config),
            },
            .hishelf = .{
                .config = hishelf_config.hishelf,
                .node = try audio.engine.createHishelfNode(hishelf_config),
            },
        };

        try audio_filter.lpf.node.attachOutputBus(0, audio.engine.getEndpointMut(), 0);
        try audio_filter.hpf.node.attachOutputBus(0, audio.engine.getEndpointMut(), 0);
        try audio_filter.notch.node.attachOutputBus(0, audio.engine.getEndpointMut(), 0);
        try audio_filter.peak.node.attachOutputBus(0, audio.engine.getEndpointMut(), 0);
        try audio_filter.loshelf.node.attachOutputBus(0, audio.engine.getEndpointMut(), 0);
        try audio_filter.hishelf.node.attachOutputBus(0, audio.engine.getEndpointMut(), 0);

        break :audio_filter audio_filter;
    };

    // Waveform generator
    const waveform_config = zaudio.Waveform.Config.init(
        .float32,
        audio.engine.getChannels(),
        audio.engine.getSampleRate(),
        .sine,
        0.5,
        440.0,
    );
    const waveform_data_source = try zaudio.Waveform.create(waveform_config);
    const waveform_node = try audio.engine.createDataSourceNode(
        zaudio.DataSourceNode.Config.init(waveform_data_source.asDataSourceMut()),
    );
    try waveform_node.setState(.stopped);

    // Noise generator
    const noise_config = zaudio.Noise.Config.init(
        .float32,
        audio.engine.getChannels(),
        .pink,
        123,
        0.25,
    );
    const noise_data_source = try zaudio.Noise.create(noise_config);
    const noise_node = try audio.engine.createDataSourceNode(
        zaudio.DataSourceNode.Config.init(noise_data_source.asDataSourceMut()),
    );
    try noise_node.setState(.stopped);

    const demo = try allocator.create(DemoState);
    demo.* = .{
        .allocator = allocator,
        .window = window,
        .gctx = gctx,
        .uniform_bg = uniform_bg,
        .depth_tex = depth.tex,
        .depth_texv = depth.texv,
        .audio = audio,
        .music = music,
        .sounds = sounds,
        .audio_filter = audio_filter,
        .waveform_config = waveform_config,
        .waveform_data_source = waveform_data_source,
        .waveform_node = waveform_node,
        .noise_config = noise_config,
        .noise_data_source = noise_data_source,
        .noise_node = noise_node,
    };

    try updateAudioGraph(demo.*);

    const common_depth_state = wgpu.DepthStencilState{
        .format = .depth32_float,
        .depth_write_enabled = true,
        .depth_compare = .less,
    };
    const pos_color_attribs = [_]wgpu.VertexAttribute{
        .{ .format = .float32x3, .offset = 0, .shader_location = 0 },
        .{ .format = .float32x3, .offset = @offsetOf(Vertex, "color"), .shader_location = 1 },
    };
    zgpu.createRenderPipelineSimple(
        allocator,
        gctx,
        &.{ uniform_bgl, uniform_bgl },
        wgsl.lines_vs,
        wgsl.lines_fs,
        @sizeOf(Vertex),
        pos_color_attribs[0..],
        .{ .topology = .line_strip },
        zgpu.GraphicsContext.swapchain_format,
        common_depth_state,
        &demo.lines_pipe,
    );

    return demo;
}

fn destroy(allocator: std.mem.Allocator, demo: *DemoState) void {
    demo.audio.engine.stop() catch unreachable;
    demo.audio_filter.is_enabled = false;
    updateAudioGraph(demo.*) catch unreachable;
    demo.audio_filter.destroy();
    demo.waveform_data_source.destroy();
    demo.waveform_node.destroy();
    demo.noise_data_source.destroy();
    demo.noise_node.destroy();
    demo.music.destroy();
    for (demo.sounds.items) |sound| sound.destroy();
    demo.sounds.deinit();
    demo.audio.destroy(allocator);
    zaudio.deinit();
    demo.gctx.destroy(allocator);
    allocator.destroy(demo);
}

fn updateAudioGraph(demo: DemoState) !void {
    const node = node: {
        if (demo.audio_filter.is_enabled == false)
            break :node demo.audio.engine.getEndpointMut();
        break :node demo.audio_filter.getCurrentNode();
    };

    try demo.music.attachOutputBus(0, node, 0);
    try demo.waveform_node.attachOutputBus(0, node, 0);
    try demo.noise_node.attachOutputBus(0, node, 0);
    for (demo.sounds.items) |sound| {
        try sound.attachOutputBus(0, node, 0);
    }
}

fn update(demo: *DemoState) !void {
    zgui.backend.newFrame(
        demo.gctx.swapchain_descriptor.width,
        demo.gctx.swapchain_descriptor.height,
    );

    const win_offset: f32 = 10.0;
    var win_y: f32 = 10.0;
    var win_width: f32 = 0.0;
    zgui.setNextWindowPos(.{ .x = win_offset, .y = win_y });
    zgui.setNextWindowSize(.{ .w = -1.0, .h = -1.0 });

    if (zgui.begin(
        "Info",
        .{ .flags = .{ .no_move = true, .no_resize = true } },
    )) {
        zgui.bullet();
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Average :");
        zgui.sameLine(.{});
        zgui.text(
            "{d:.3} ms/frame ({d:.1} fps)",
            .{ demo.gctx.stats.average_cpu_time, demo.gctx.stats.fps },
        );

        zgui.bullet();
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "RMB + drag :");
        zgui.sameLine(.{});
        zgui.textUnformatted("rotate camera");

        zgui.bullet();
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "W, A, S, D :");
        zgui.sameLine(.{});
        zgui.textUnformatted("move camera");
    }
    win_width = zgui.getWindowWidth();
    win_y += zgui.getWindowHeight() + win_offset;
    zgui.end();

    zgui.setNextWindowPos(.{ .x = win_offset, .y = win_y });
    zgui.setNextWindowSize(.{ .w = win_width, .h = -1.0 });

    if (zgui.begin(
        "Data Sources",
        .{ .flags = .{ .no_move = true, .no_resize = true } },
    )) {
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
        zgui.separator();
        zgui.textUnformatted("Sounds:");
        if (zgui.button("Sound 1", .{})) {
            try demo.sounds.items[0].start();
        }
        zgui.sameLine(.{});
        if (zgui.button("Sound 2", .{})) {
            try demo.sounds.items[1].start();
        }
        zgui.sameLine(.{});
        if (zgui.button("Sound 3", .{})) {
            try demo.sounds.items[2].start();
        }

        // Waveform generator
        {
            zgui.pushIntId(0);
            defer zgui.popId();
            zgui.spacing();
            zgui.separator();
            zgui.textUnformatted("Waveform Generator:");

            var is_enabled = demo.waveform_node.getState() == .started;
            if (zgui.checkbox("Enabled", .{ .v = &is_enabled })) {
                if (is_enabled) {
                    try demo.waveform_node.setState(.started);
                } else try demo.waveform_node.setState(.stopped);
            }
            if (!is_enabled) zgui.beginDisabled(.{});
            defer if (!is_enabled) zgui.endDisabled();

            const selected_item = @intFromEnum(demo.waveform_config.waveform_type);
            const names = [_][:0]const u8{ "Sine", "Square", "Triangle", "Sawtooth" };
            if (zgui.beginCombo("Type", .{ .preview_value = names[selected_item] })) {
                for (names, 0..) |name, index| {
                    if (zgui.selectable(name, .{ .selected = (selected_item == index) }) and
                        selected_item != index)
                    {
                        demo.waveform_config.waveform_type = @as(zaudio.Waveform.Type, @enumFromInt(index));
                        try demo.waveform_data_source.setType(demo.waveform_config.waveform_type);
                    }
                }
                zgui.endCombo();
            }

            if (zgui.sliderScalar("Frequency", f64, .{
                .v = &demo.waveform_config.frequency,
                .min = 20.0,
                .max = 1000.0,
                .cfmt = "%.1f Hz",
            })) {
                try demo.waveform_data_source.setFrequency(demo.waveform_config.frequency);
            }
            if (zgui.sliderScalar("Amplitude", f64, .{
                .v = &demo.waveform_config.amplitude,
                .min = 0.05,
                .max = 0.75,
                .cfmt = "%.3f",
            })) {
                try demo.waveform_data_source.setAmplitude(demo.waveform_config.amplitude);
            }
        }

        // Noise generator
        {
            zgui.pushIntId(1);
            defer zgui.popId();
            zgui.spacing();
            zgui.separator();
            zgui.textUnformatted("Noise Generator:");

            var is_enabled = demo.noise_node.getState() == .started;
            if (zgui.checkbox("Enabled", .{ .v = &is_enabled })) {
                if (is_enabled) {
                    try demo.noise_node.setState(.started);
                } else try demo.noise_node.setState(.stopped);
            }
            if (!is_enabled) zgui.beginDisabled(.{});
            defer if (!is_enabled) zgui.endDisabled();

            const selected_item = @intFromEnum(demo.noise_config.noise_type);
            const names = [_][:0]const u8{ "White", "Pink" };
            if (zgui.beginCombo("Type", .{ .preview_value = names[selected_item] })) {
                for (names, 0..) |name, index| {
                    if (zgui.selectable(name, .{ .selected = (selected_item == index) }) and
                        selected_item != index)
                    {
                        demo.noise_config.noise_type = @as(zaudio.Noise.Type, @enumFromInt(index));
                        try demo.noise_data_source.setType(demo.noise_config.noise_type);
                    }
                }
                zgui.endCombo();
            }

            if (zgui.sliderScalar("Amplitude", f64, .{
                .v = &demo.noise_config.amplitude,
                .min = 0.05,
                .max = 0.75,
                .cfmt = "%.3f",
            })) {
                try demo.noise_data_source.setAmplitude(demo.noise_config.amplitude);
            }
        }
    }
    win_y += zgui.getWindowHeight() + win_offset;
    zgui.end();

    zgui.setNextWindowPos(.{ .x = win_offset, .y = win_y });
    zgui.setNextWindowSize(.{ .w = win_width, .h = -1.0 });

    if (zgui.begin("Audio Filter", .{
        .flags = .{ .no_move = true, .no_resize = true },
    })) {
        if (zgui.checkbox("Enabled", .{ .v = &demo.audio_filter.is_enabled })) {
            try updateAudioGraph(demo.*);
        }
        if (!demo.audio_filter.is_enabled) zgui.beginDisabled(.{});
        defer if (!demo.audio_filter.is_enabled) zgui.endDisabled();

        const selected_item = @intFromEnum(demo.audio_filter.current_type);
        if (zgui.beginCombo("Type", .{ .preview_value = AudioFilterType.names[selected_item] })) {
            for (AudioFilterType.names, 0..) |name, index| {
                if (zgui.selectable(name, .{ .selected = (selected_item == index) }) and
                    selected_item != index)
                {
                    demo.audio_filter.current_type = @as(AudioFilterType, @enumFromInt(index));
                    try updateAudioGraph(demo.*);
                }
            }
            zgui.endCombo();
        }

        switch (demo.audio_filter.current_type) {
            .lpf => {
                const config = &demo.audio_filter.lpf.config;
                if (zgui.sliderScalar("Cutoff", f64, .{
                    .v = &config.cutoff_frequency,
                    .min = min_filter_fequency,
                    .max = max_filter_fequency,
                    .cfmt = "%.1f Hz",
                })) {
                    try demo.audio_filter.lpf.node.reconfigure(config.*);
                }
            },
            .hpf => {
                const config = &demo.audio_filter.hpf.config;
                if (zgui.sliderScalar("Cutoff", f64, .{
                    .v = &config.cutoff_frequency,
                    .min = min_filter_fequency,
                    .max = max_filter_fequency,
                    .cfmt = "%.1f Hz",
                })) {
                    try demo.audio_filter.hpf.node.reconfigure(config.*);
                }
            },
            .notch => {
                const config = &demo.audio_filter.notch.config;
                var has_changed = false;
                if (zgui.sliderScalar("Frequency", f64, .{
                    .v = &config.frequency,
                    .min = min_filter_fequency,
                    .max = max_filter_fequency,
                    .cfmt = "%.1f Hz",
                })) has_changed = true;
                if (zgui.sliderScalar("Q", f64, .{
                    .v = &config.q,
                    .min = min_filter_q,
                    .max = max_filter_q,
                    .cfmt = "%.3f",
                })) has_changed = true;
                if (has_changed) try demo.audio_filter.notch.node.reconfigure(config.*);
            },
            .peak => {
                const config = &demo.audio_filter.peak.config;
                var has_changed = false;
                if (zgui.sliderScalar("Gain", f64, .{
                    .v = &config.gain_db,
                    .min = min_filter_gain,
                    .max = max_filter_gain,
                    .cfmt = "%.1f dB",
                })) has_changed = true;
                if (zgui.sliderScalar("Frequency", f64, .{
                    .v = &config.frequency,
                    .min = min_filter_fequency,
                    .max = max_filter_fequency,
                    .cfmt = "%.1f Hz",
                })) has_changed = true;
                if (zgui.sliderScalar("Q", f64, .{
                    .v = &config.q,
                    .min = min_filter_q,
                    .max = max_filter_q,
                    .cfmt = "%.3f",
                })) has_changed = true;
                if (has_changed) try demo.audio_filter.peak.node.reconfigure(config.*);
            },
            .loshelf => {
                const config = &demo.audio_filter.loshelf.config;
                var has_changed = false;
                if (zgui.sliderScalar("Gain", f64, .{
                    .v = &config.gain_db,
                    .min = min_filter_gain,
                    .max = max_filter_gain,
                    .cfmt = "%.1f dB",
                })) has_changed = true;
                if (zgui.sliderScalar("Frequency", f64, .{
                    .v = &config.frequency,
                    .min = min_filter_fequency,
                    .max = max_filter_fequency,
                    .cfmt = "%.1f Hz",
                })) has_changed = true;
                if (zgui.sliderScalar("Slope", f64, .{
                    .v = &config.shelf_slope,
                    .min = min_filter_q,
                    .max = max_filter_q,
                    .cfmt = "%.3f",
                })) has_changed = true;
                if (has_changed) try demo.audio_filter.loshelf.node.reconfigure(config.*);
            },
            .hishelf => {
                const config = &demo.audio_filter.hishelf.config;
                var has_changed = false;
                if (zgui.sliderScalar("Gain", f64, .{
                    .v = &config.gain_db,
                    .min = min_filter_gain,
                    .max = max_filter_gain,
                    .cfmt = "%.1f dB",
                })) has_changed = true;
                if (zgui.sliderScalar("Frequency", f64, .{
                    .v = &config.frequency,
                    .min = min_filter_fequency,
                    .max = max_filter_fequency,
                    .cfmt = "%.1f Hz",
                })) has_changed = true;
                if (zgui.sliderScalar("Slope", f64, .{
                    .v = &config.shelf_slope,
                    .min = min_filter_q,
                    .max = max_filter_q,
                    .cfmt = "%.3f",
                })) has_changed = true;
                if (has_changed) try demo.audio_filter.hishelf.node.reconfigure(config.*);
            },
        }
    }
    zgui.end();

    const window = demo.window;

    // Handle camera rotation with mouse.
    {
        const cursor_pos = window.getCursorPos();
        const delta_x = @as(f32, @floatCast(cursor_pos[0] - demo.mouse.cursor_pos[0]));
        const delta_y = @as(f32, @floatCast(cursor_pos[1] - demo.mouse.cursor_pos[1]));
        demo.mouse.cursor_pos = cursor_pos;

        if (window.getMouseButton(.right) == .press) {
            demo.camera.pitch += 0.0025 * delta_y;
            demo.camera.yaw += 0.0025 * delta_x;
            demo.camera.pitch = @min(demo.camera.pitch, 0.48 * math.pi);
            demo.camera.pitch = @max(demo.camera.pitch, -0.48 * math.pi);
            demo.camera.yaw = zm.modAngle(demo.camera.yaw);
        }
    }

    // Handle camera movement with 'WASD' keys.
    {
        const speed = zm.f32x4s(10.0);
        const delta_time = zm.f32x4s(demo.gctx.stats.delta_time);
        const transform = zm.mul(zm.rotationX(demo.camera.pitch), zm.rotationY(demo.camera.yaw));
        var forward = zm.normalize3(zm.mul(zm.f32x4(0.0, 0.0, 1.0, 0.0), transform));

        zm.storeArr3(&demo.camera.forward, forward);

        const right = speed * delta_time *
            zm.normalize3(zm.cross3(zm.f32x4(0.0, 1.0, 0.0, 0.0), forward));
        forward = speed * delta_time * forward;

        var cam_pos = zm.loadArr3(demo.camera.position);

        if (window.getKey(.w) == .press) {
            cam_pos += forward;
        } else if (window.getKey(.s) == .press) {
            cam_pos -= forward;
        }
        if (window.getKey(.d) == .press) {
            cam_pos += right;
        } else if (window.getKey(.a) == .press) {
            cam_pos -= right;
        }

        zm.storeArr3(&demo.camera.position, cam_pos);
    }
}

fn draw(demo: *DemoState) void {
    const gctx = demo.gctx;
    const fb_width = gctx.swapchain_descriptor.width;
    const fb_height = gctx.swapchain_descriptor.height;

    const swapchain_texv = gctx.swapchain.getCurrentTextureView();
    defer swapchain_texv.release();

    const cam_world_to_view = zm.lookToLh(
        zm.loadArr3(demo.camera.position),
        zm.loadArr3(demo.camera.forward),
        zm.f32x4(0.0, 1.0, 0.0, 0.0),
    );
    const cam_view_to_clip = zm.perspectiveFovLh(
        0.25 * math.pi,
        @as(f32, @floatFromInt(fb_width)) / @as(f32, @floatFromInt(fb_height)),
        0.01,
        200.0,
    );
    const cam_world_to_clip = zm.mul(cam_world_to_view, cam_view_to_clip);

    const vertex_buffer_handle = gctx.createBuffer(.{
        .usage = .{ .vertex = true },
        .size = AudioState.num_sets * AudioState.usable_samples_per_set * @sizeOf(Vertex),
        .mapped_at_creation = true,
    });
    const vertex_buffer = gctx.lookupResource(vertex_buffer_handle).?;
    defer gctx.destroyResource(vertex_buffer_handle);
    {
        const mem = vertex_buffer.getMappedRange(
            Vertex,
            0,
            AudioState.num_sets * AudioState.usable_samples_per_set,
        ).?;

        demo.audio.mutex.lock();
        defer demo.audio.mutex.unlock();

        const rcp_num_sets = 1.0 / @as(f32, @floatFromInt(AudioState.num_sets - 1));
        var set: u32 = 0;
        while (set < AudioState.num_sets) : (set += 1) {
            const z = (demo.audio.current_set + set) % AudioState.num_sets;
            const f = if (set == 0) 1.0 else rcp_num_sets * @as(f32, @floatFromInt(set - 1));

            var x: u32 = 0;
            while (x < AudioState.usable_samples_per_set) : (x += 1) {
                const sample = demo.audio.samples.items[x + z * AudioState.samples_per_set];

                const color = zm.vecToArr3(zm.lerp(
                    zm.f32x4(0.2, 1.0, 0.0, 0.0),
                    zm.f32x4(1.0, 0.0, 0.0, 0.0),
                    1.2 * @sqrt(f) * @abs(sample),
                ));

                mem[x + set * AudioState.usable_samples_per_set] = Vertex{
                    .position = [_]f32{
                        0.1 * @as(f32, @floatFromInt(x)),
                        f * f * f * 10.0 * sample,
                        0.5 * @as(f32, @floatFromInt(z)),
                    },
                    .color = color,
                };
            }
        }
    }
    vertex_buffer.unmap();

    const commands = commands: {
        const encoder = gctx.device.createCommandEncoder(null);
        defer encoder.release();

        pass: {
            const lines_pipe = gctx.lookupResource(demo.lines_pipe) orelse break :pass;
            const depth_texv = gctx.lookupResource(demo.depth_texv) orelse break :pass;
            const uniform_bg = gctx.lookupResource(demo.uniform_bg) orelse break :pass;

            const pass = zgpu.beginRenderPassSimple(encoder, .clear, swapchain_texv, null, depth_texv, 1.0);
            defer zgpu.endReleasePass(pass);

            pass.setPipeline(lines_pipe);
            pass.setVertexBuffer(
                0,
                vertex_buffer,
                0,
                AudioState.num_sets * AudioState.usable_samples_per_set * @sizeOf(Vertex),
            );
            {
                const mem = gctx.uniformsAllocate(FrameUniforms, 1);
                mem.slice[0] = .{ .world_to_clip = zm.transpose(cam_world_to_clip) };
                pass.setBindGroup(0, uniform_bg, &.{mem.offset});
            }
            {
                const mem = gctx.uniformsAllocate(DrawUniforms, 1);
                mem.slice[0] = .{ .object_to_world = zm.identity() };
                pass.setBindGroup(1, uniform_bg, &.{mem.offset});
            }

            var i: u32 = 0;
            while (i < AudioState.num_sets) : (i += 1) {
                pass.draw(
                    AudioState.usable_samples_per_set,
                    1,
                    i * AudioState.usable_samples_per_set,
                    0,
                );
            }
        }

        // Gui pass.
        {
            const pass = zgpu.beginRenderPassSimple(encoder, .load, swapchain_texv, null, null, null);
            defer zgpu.endReleasePass(pass);
            zgui.backend.draw(pass);
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
        .format = wgpu.TextureFormat.depth32_float,
        .mip_level_count = 1,
        .sample_count = 1,
    });
    const texv = gctx.createTextureView(tex, .{});
    return .{ .tex = tex, .texv = texv };
}

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

    const window = try zglfw.Window.create(1600, 1000, window_title, null);
    defer window.destroy();
    window.setSizeLimits(400, 400, -1, -1);

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    const demo = try create(allocator, window);
    defer destroy(allocator, demo);

    const scale_factor = scale_factor: {
        const scale = window.getContentScale();
        break :scale_factor @max(scale[0], scale[1]);
    };

    zgui.init(allocator);
    defer zgui.deinit();

    _ = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", math.floor(16.0 * scale_factor));

    zgui.backend.init(
        window,
        demo.gctx.device,
        @intFromEnum(zgpu.GraphicsContext.swapchain_format),
        @intFromEnum(wgpu.TextureFormat.undef),
    );
    defer zgui.backend.deinit();

    zgui.getStyle().scaleAllSizes(scale_factor);

    while (!window.shouldClose() and window.getKey(.escape) != .press) {
        zglfw.pollEvents();
        try update(demo);
        draw(demo);
    }
}
