const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const Mutex = std.Thread.Mutex;
const glfw = @import("glfw");
const zgpu = @import("zgpu");
const wgpu = zgpu.wgpu;
const zgui = zgpu.zgui;
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
        config: zaudio.LpfNodeConfig,
        node: zaudio.LpfNode,
    },
    hpf: struct {
        config: zaudio.HpfNodeConfig,
        node: zaudio.HpfNode,
    },
    notch: struct {
        config: zaudio.NotchNodeConfig,
        node: zaudio.NotchNode,
    },
    peak: struct {
        config: zaudio.PeakNodeConfig,
        node: zaudio.PeakNode,
    },
    loshelf: struct {
        config: zaudio.LoshelfNodeConfig,
        node: zaudio.LoshelfNode,
    },
    hishelf: struct {
        config: zaudio.HishelfNodeConfig,
        node: zaudio.HishelfNode,
    },

    fn getCurrentNode(filter: AudioFilter) zaudio.Node {
        return switch (filter.current_type) {
            .lpf => filter.lpf.node.asNode(),
            .hpf => filter.hpf.node.asNode(),
            .notch => filter.notch.node.asNode(),
            .peak => filter.peak.node.asNode(),
            .loshelf => filter.loshelf.node.asNode(),
            .hishelf => filter.hishelf.node.asNode(),
        };
    }

    fn destroy(filter: AudioFilter, allocator: std.mem.Allocator) void {
        filter.lpf.node.destroy(allocator);
        filter.hpf.node.destroy(allocator);
        filter.notch.node.destroy(allocator);
        filter.peak.node.destroy(allocator);
        filter.loshelf.node.destroy(allocator);
        filter.hishelf.node.destroy(allocator);
    }
};

const AudioState = struct {
    const num_sets = 100;
    const samples_per_set = 512;
    const usable_samples_per_set = 480;

    device: zaudio.Device,
    engine: zaudio.Engine,
    mutex: Mutex = .{},
    current_set: u32 = num_sets - 1,
    samples: std.ArrayList(f32),

    fn audioCallback(context: ?*anyopaque, outptr: *anyopaque, num_frames: u32) void {
        if (context == null) return;

        const audio = @ptrCast(*AudioState, @alignCast(@alignOf(AudioState), context));

        audio.engine.readPcmFrames(outptr, num_frames, null) catch {};

        audio.mutex.lock();
        defer audio.mutex.unlock();

        audio.current_set = (audio.current_set + 1) % num_sets;

        const num_channels = 2;
        const base_index = samples_per_set * audio.current_set;
        const frames = @ptrCast([*]f32, @alignCast(@sizeOf(f32), outptr));

        var i: u32 = 0;
        while (i < math.min(num_frames, usable_samples_per_set)) : (i += 1) {
            audio.samples.items[base_index + i] = frames[i * num_channels];
        }
    }

    fn create(allocator: std.mem.Allocator) !*AudioState {
        const samples = samples: {
            var samples = std.ArrayList(f32).initCapacity(
                allocator,
                num_sets * samples_per_set,
            ) catch unreachable;
            samples.expandToCapacity();
            std.mem.set(f32, samples.items, 0.0);
            break :samples samples;
        };

        const audio = try allocator.create(AudioState);

        const device = device: {
            var config = zaudio.DeviceConfig.init(.playback);
            config.playback_callback = .{
                .context = audio,
                .callback = audioCallback,
            };
            config.raw.playback.format = @enumToInt(zaudio.Format.float32);
            config.raw.playback.channels = 2;
            config.raw.sampleRate = 48_000;
            config.raw.periodSizeInFrames = 480;
            config.raw.periodSizeInMilliseconds = 10;
            break :device try zaudio.createDevice(allocator, null, &config);
        };

        const engine = engine: {
            var config = zaudio.EngineConfig.init();
            config.raw.pDevice = device.asRaw();
            config.raw.noAutoStart = 1;
            break :engine try zaudio.createEngine(allocator, config);
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
        audio.engine.destroy(allocator);
        audio.device.destroy(allocator);
        allocator.destroy(audio);
    }
};

const DemoState = struct {
    allocator: std.mem.Allocator,
    gctx: *zgpu.GraphicsContext,
    audio: *AudioState,

    lines_pipe: zgpu.RenderPipelineHandle = .{},

    uniform_bg: zgpu.BindGroupHandle,

    depth_tex: zgpu.TextureHandle,
    depth_texv: zgpu.TextureViewHandle,

    music: zaudio.Sound,
    sounds: std.ArrayList(zaudio.Sound),
    audio_filter: AudioFilter,

    waveform_config: zaudio.WaveformConfig,
    waveform_data_source: zaudio.WaveformDataSource,
    waveform_node: zaudio.DataSourceNode,

    noise_config: zaudio.NoiseConfig,
    noise_data_source: zaudio.NoiseDataSource,
    noise_node: zaudio.DataSourceNode,

    camera: struct {
        position: [3]f32 = .{ -10.0, 15.0, -10.0 },
        forward: [3]f32 = .{ 0.0, 0.0, 1.0 },
        pitch: f32 = 0.15 * math.pi,
        yaw: f32 = 0.25 * math.pi,
    } = .{},
    mouse: struct {
        cursor: glfw.Window.CursorPos = .{ .xpos = 0.0, .ypos = 0.0 },
    } = .{},
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

    const uniform_bg = gctx.createBindGroup(uniform_bgl, &[_]zgpu.BindGroupEntryInfo{.{
        .binding = 0,
        .buffer_handle = gctx.uniforms.buffer,
        .offset = 0,
        .size = safe_uniform_size,
    }});

    const depth = createDepthTexture(gctx);

    const audio = try AudioState.create(allocator);
    try audio.engine.start();

    const sounds = sounds: {
        var sounds = std.ArrayList(zaudio.Sound).init(allocator);
        try sounds.append(try audio.engine.createSoundFromFile(
            allocator,
            content_dir ++ "drum_bass_hard.flac",
            .{},
        ));
        try sounds.append(try audio.engine.createSoundFromFile(
            allocator,
            content_dir ++ "tabla_tas1.flac",
            .{},
        ));
        try sounds.append(try audio.engine.createSoundFromFile(
            allocator,
            content_dir ++ "loop_mika.flac",
            .{},
        ));
        break :sounds sounds;
    };

    const music = try audio.engine.createSoundFromFile(
        allocator,
        content_dir ++ "Broke For Free - Night Owl.mp3",
        .{ .flags = .{ .stream = true } },
    );
    music.setVolume(1.5);
    try music.start();

    const audio_filter = audio_filter: {
        const lpf_config = zaudio.LpfNodeConfig.init(
            audio.engine.getNumChannels(),
            audio.engine.getSampleRate(),
            min_filter_fequency,
            filter_order,
        );
        const hpf_config = zaudio.HpfNodeConfig.init(
            audio.engine.getNumChannels(),
            audio.engine.getSampleRate(),
            min_filter_fequency,
            filter_order,
        );
        const notch_config = zaudio.NotchNodeConfig.init(
            audio.engine.getNumChannels(),
            audio.engine.getSampleRate(),
            min_filter_q,
            min_filter_fequency,
        );
        const peak_config = zaudio.PeakNodeConfig.init(
            audio.engine.getNumChannels(),
            audio.engine.getSampleRate(),
            min_filter_gain,
            min_filter_q,
            min_filter_fequency,
        );
        const loshelf_config = zaudio.LoshelfNodeConfig.init(
            audio.engine.getNumChannels(),
            audio.engine.getSampleRate(),
            min_filter_gain,
            min_filter_q,
            min_filter_fequency,
        );
        const hishelf_config = zaudio.HishelfNodeConfig.init(
            audio.engine.getNumChannels(),
            audio.engine.getSampleRate(),
            min_filter_gain,
            min_filter_q,
            min_filter_fequency,
        );

        const audio_filter = AudioFilter{
            .lpf = .{
                .config = lpf_config,
                .node = try audio.engine.createLpfNode(allocator, lpf_config),
            },
            .hpf = .{
                .config = hpf_config,
                .node = try audio.engine.createHpfNode(allocator, hpf_config),
            },
            .notch = .{
                .config = notch_config,
                .node = try audio.engine.createNotchNode(allocator, notch_config),
            },
            .peak = .{
                .config = peak_config,
                .node = try audio.engine.createPeakNode(allocator, peak_config),
            },
            .loshelf = .{
                .config = loshelf_config,
                .node = try audio.engine.createLoshelfNode(allocator, loshelf_config),
            },
            .hishelf = .{
                .config = hishelf_config,
                .node = try audio.engine.createHishelfNode(allocator, hishelf_config),
            },
        };

        try audio_filter.lpf.node.attachOutputBus(0, audio.engine.getEndpoint(), 0);
        try audio_filter.hpf.node.attachOutputBus(0, audio.engine.getEndpoint(), 0);
        try audio_filter.notch.node.attachOutputBus(0, audio.engine.getEndpoint(), 0);
        try audio_filter.peak.node.attachOutputBus(0, audio.engine.getEndpoint(), 0);
        try audio_filter.loshelf.node.attachOutputBus(0, audio.engine.getEndpoint(), 0);
        try audio_filter.hishelf.node.attachOutputBus(0, audio.engine.getEndpoint(), 0);

        break :audio_filter audio_filter;
    };

    // Waveform generator
    const waveform_config = zaudio.WaveformConfig.init(
        .float32,
        audio.engine.getNumChannels(),
        audio.engine.getSampleRate(),
        .sine,
        0.5,
        440.0,
    );
    const waveform_data_source = try zaudio.createWaveformDataSource(allocator, waveform_config);
    const waveform_node = try audio.engine.createDataSourceNode(
        allocator,
        zaudio.DataSourceNodeConfig.init(waveform_data_source.asDataSource()),
    );
    try waveform_node.setState(.stopped);

    // Noise generator
    const noise_config = zaudio.NoiseConfig.init(
        .float32,
        audio.engine.getNumChannels(),
        .pink,
        123,
        0.5,
    );
    const noise_data_source = try zaudio.createNoiseDataSource(allocator, noise_config);
    const noise_node = try audio.engine.createDataSourceNode(
        allocator,
        zaudio.DataSourceNodeConfig.init(noise_data_source.asDataSource()),
    );
    try noise_node.setState(.stopped);

    const demo = try allocator.create(DemoState);
    demo.* = .{
        .allocator = allocator,
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
    zgpu.util.createRenderPipelineSimple(
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
    demo.audio_filter.destroy(allocator);
    demo.waveform_data_source.destroy(allocator);
    demo.waveform_node.destroy(allocator);
    demo.noise_data_source.destroy(allocator);
    demo.noise_node.destroy(allocator);
    demo.music.destroy(allocator);
    for (demo.sounds.items) |sound| sound.destroy(allocator);
    demo.sounds.deinit();
    demo.audio.destroy(allocator);
    demo.gctx.deinit(allocator);
    allocator.destroy(demo);
}

fn updateAudioGraph(demo: DemoState) !void {
    const node = node: {
        if (demo.audio_filter.is_enabled == false)
            break :node demo.audio.engine.getEndpoint();
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
    zgpu.gui.newFrame(demo.gctx.swapchain_descriptor.width, demo.gctx.swapchain_descriptor.height);

    const win_offset: f32 = 10.0;
    const win_width: f32 = 450.0;
    var win_y: f32 = 10.0;
    zgui.setNextWindowPos(.{ .x = win_offset, .y = win_y });
    zgui.setNextWindowSize(.{ .w = win_width, .h = -1.0 });

    if (zgui.begin(
        "Info",
        .{ .flags = .{ .no_move = true, .no_resize = true, .no_collapse = true } },
    )) {
        zgui.bullet();
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "Average :");
        zgui.sameLine(.{});
        zgui.text(
            " {d:.3} ms/frame ({d:.1} fps)",
            .{ demo.gctx.stats.average_cpu_time, demo.gctx.stats.fps },
        );

        zgui.bullet();
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "RMB + drag :");
        zgui.sameLine(.{});
        zgui.textUnformatted(" rotate camera");

        zgui.bullet();
        zgui.textUnformattedColored(.{ 0, 0.8, 0, 1 }, "W, A, S, D :");
        zgui.sameLine(.{});
        zgui.textUnformatted(" move camera");
    }
    win_y += zgui.getWindowHeight() + win_offset;
    zgui.end();

    zgui.setNextWindowPos(.{ .x = win_offset, .y = win_y });
    zgui.setNextWindowSize(.{ .w = win_width, .h = -1.0 });

    if (zgui.begin(
        "Data Sources",
        .{ .flags = .{ .no_move = true, .no_resize = true, .no_collapse = true } },
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

            var enabled = demo.waveform_node.getState() == .started;
            if (zgui.checkbox("Enabled", .{ .v = &enabled })) {
                if (enabled) {
                    try demo.waveform_node.setState(.started);
                } else try demo.waveform_node.setState(.stopped);
            }

            const selected_item = demo.waveform_config.raw.type;
            const names = [_][:0]const u8{ "Sine", "Square", "Triangle", "Sawtooth" };
            if (zgui.beginCombo("Type", .{ .preview_value = names[selected_item] })) {
                for (names) |name, index| {
                    if (zgui.selectable(name, .{ .selected = (selected_item == index) }) and
                        selected_item != index)
                    {
                        demo.waveform_config.raw.type = @intCast(u32, index);
                        try demo.waveform_data_source.setType(@intToEnum(zaudio.WaveformType, index));
                    }
                }
                zgui.endCombo();
            }

            if (zgui.sliderScalar("Frequency", f64, .{
                .v = &demo.waveform_config.raw.frequency,
                .min = 20.0,
                .max = 1000.0,
                .cfmt = "%.1f Hz",
            })) {
                try demo.waveform_data_source.setFrequency(demo.waveform_config.raw.frequency);
            }
            if (zgui.sliderScalar("Amplitude", f64, .{
                .v = &demo.waveform_config.raw.amplitude,
                .min = 0.05,
                .max = 0.75,
                .cfmt = "%.3f",
            })) {
                try demo.waveform_data_source.setAmplitude(demo.waveform_config.raw.amplitude);
            }
        }

        // Noise generator
        {
            zgui.pushIntId(1);
            defer zgui.popId();
            zgui.spacing();
            zgui.separator();
            zgui.textUnformatted("Noise Generator:");

            var enabled = demo.noise_node.getState() == .started;
            if (zgui.checkbox("Enabled", .{ .v = &enabled })) {
                if (enabled) {
                    try demo.noise_node.setState(.started);
                } else try demo.noise_node.setState(.stopped);
            }

            const selected_item = demo.noise_config.raw.type;
            const names = [_][:0]const u8{ "White", "Pink" };
            if (zgui.beginCombo("Type", .{ .preview_value = names[selected_item] })) {
                for (names) |name, index| {
                    if (zgui.selectable(name, .{ .selected = (selected_item == index) }) and
                        selected_item != index)
                    {
                        demo.noise_config.raw.type = @intCast(u32, index);
                        try demo.noise_data_source.setType(@intToEnum(zaudio.NoiseType, index));
                    }
                }
                zgui.endCombo();
            }

            if (zgui.sliderScalar("Amplitude", f64, .{
                .v = &demo.noise_config.raw.amplitude,
                .min = 0.05,
                .max = 0.75,
                .cfmt = "%.3f",
            })) {
                try demo.noise_data_source.setAmplitude(demo.noise_config.raw.amplitude);
            }
        }
    }
    win_y += zgui.getWindowHeight() + win_offset;
    zgui.end();

    zgui.setNextWindowPos(.{ .x = win_offset, .y = win_y });
    zgui.setNextWindowSize(.{ .w = win_width, .h = -1.0 });

    if (zgui.begin("Audio Filter", .{
        .flags = .{ .no_move = true, .no_resize = true, .no_collapse = true },
    })) {
        if (zgui.checkbox("Enabled", .{ .v = &demo.audio_filter.is_enabled })) {
            try updateAudioGraph(demo.*);
        }

        const selected_item = @enumToInt(demo.audio_filter.current_type);
        if (zgui.beginCombo("Type", .{ .preview_value = AudioFilterType.names[selected_item] })) {
            for (AudioFilterType.names) |name, index| {
                if (zgui.selectable(name, .{ .selected = (selected_item == index) }) and
                    selected_item != index)
                {
                    demo.audio_filter.current_type = @intToEnum(AudioFilterType, index);
                    try updateAudioGraph(demo.*);
                }
            }
            zgui.endCombo();
        }

        switch (demo.audio_filter.current_type) {
            .lpf => {
                const config = &demo.audio_filter.lpf.config;
                if (zgui.sliderScalar("Cutoff", f64, .{
                    .v = &config.raw.lpf.cutoffFrequency,
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
                    .v = &config.raw.hpf.cutoffFrequency,
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
                    .v = &config.raw.notch.frequency,
                    .min = min_filter_fequency,
                    .max = max_filter_fequency,
                    .cfmt = "%.1f Hz",
                })) has_changed = true;
                if (zgui.sliderScalar("Q", f64, .{
                    .v = &config.raw.notch.q,
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
                    .v = &config.raw.peak.gainDB,
                    .min = min_filter_gain,
                    .max = max_filter_gain,
                    .cfmt = "%.1f dB",
                })) has_changed = true;
                if (zgui.sliderScalar("Frequency", f64, .{
                    .v = &config.raw.peak.frequency,
                    .min = min_filter_fequency,
                    .max = max_filter_fequency,
                    .cfmt = "%.1f Hz",
                })) has_changed = true;
                if (zgui.sliderScalar("Q", f64, .{
                    .v = &config.raw.peak.q,
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
                    .v = &config.raw.loshelf.gainDB,
                    .min = min_filter_gain,
                    .max = max_filter_gain,
                    .cfmt = "%.1f dB",
                })) has_changed = true;
                if (zgui.sliderScalar("Frequency", f64, .{
                    .v = &config.raw.loshelf.frequency,
                    .min = min_filter_fequency,
                    .max = max_filter_fequency,
                    .cfmt = "%.1f Hz",
                })) has_changed = true;
                if (zgui.sliderScalar("Slope", f64, .{
                    .v = &config.raw.loshelf.shelfSlope,
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
                    .v = &config.raw.hishelf.gainDB,
                    .min = min_filter_gain,
                    .max = max_filter_gain,
                    .cfmt = "%.1f dB",
                })) has_changed = true;
                if (zgui.sliderScalar("Frequency", f64, .{
                    .v = &config.raw.hishelf.frequency,
                    .min = min_filter_fequency,
                    .max = max_filter_fequency,
                    .cfmt = "%.1f Hz",
                })) has_changed = true;
                if (zgui.sliderScalar("Slope", f64, .{
                    .v = &config.raw.hishelf.shelfSlope,
                    .min = min_filter_q,
                    .max = max_filter_q,
                    .cfmt = "%.3f",
                })) has_changed = true;
                if (has_changed) try demo.audio_filter.hishelf.node.reconfigure(config.*);
            },
        }
    }
    zgui.end();

    const window = demo.gctx.window;

    // Handle camera rotation with mouse.
    {
        const cursor = window.getCursorPos() catch unreachable;
        const delta_x = @floatCast(f32, cursor.xpos - demo.mouse.cursor.xpos);
        const delta_y = @floatCast(f32, cursor.ypos - demo.mouse.cursor.ypos);
        demo.mouse.cursor.xpos = cursor.xpos;
        demo.mouse.cursor.ypos = cursor.ypos;

        if (window.getMouseButton(.right) == .press) {
            demo.camera.pitch += 0.0025 * delta_y;
            demo.camera.yaw += 0.0025 * delta_x;
            demo.camera.pitch = math.min(demo.camera.pitch, 0.48 * math.pi);
            demo.camera.pitch = math.max(demo.camera.pitch, -0.48 * math.pi);
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

        const right = speed * delta_time * zm.normalize3(zm.cross3(zm.f32x4(0.0, 1.0, 0.0, 0.0), forward));
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
        @intToFloat(f32, fb_width) / @intToFloat(f32, fb_height),
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
    defer vertex_buffer.destroy();
    {
        const mem = vertex_buffer.getMappedRange(
            Vertex,
            0,
            AudioState.num_sets * AudioState.usable_samples_per_set,
        ).?;

        demo.audio.mutex.lock();
        defer demo.audio.mutex.unlock();

        const rcp_num_sets = 1.0 / @intToFloat(f32, AudioState.num_sets - 1);
        var set: u32 = 0;
        while (set < AudioState.num_sets) : (set += 1) {
            const z = (demo.audio.current_set + set) % AudioState.num_sets;
            const f = if (set == 0) 1.0 else rcp_num_sets * @intToFloat(f32, set - 1);

            var x: u32 = 0;
            while (x < AudioState.usable_samples_per_set) : (x += 1) {
                const sample = demo.audio.samples.items[x + z * AudioState.samples_per_set];

                const color = zm.vecToArr3(zm.lerp(
                    zm.f32x4(0.2, 1.0, 0.0, 0.0),
                    zm.f32x4(1.0, 0.0, 0.0, 0.0),
                    1.2 * @sqrt(f) * @fabs(sample),
                ));

                mem[x + set * AudioState.usable_samples_per_set] = Vertex{
                    .position = [_]f32{
                        0.1 * @intToFloat(f32, x),
                        f * f * f * 10.0 * sample,
                        0.5 * @intToFloat(f32, z),
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

            const pass = zgpu.util.beginRenderPassSimple(encoder, .clear, swapchain_texv, null, depth_texv, 1.0);
            defer zgpu.util.endRelease(pass);

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
                pass.draw(AudioState.usable_samples_per_set, 1, i * AudioState.usable_samples_per_set, 0);
            }
        }

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
