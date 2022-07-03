const std = @import("std");
const assert = std.debug.assert;
const math = std.math;
const c = @cImport(@cInclude("miniaudio.h"));

// TODO: Get rid of WA_ma_* functions which are workarounds for Zig C ABI issues on aarch64.

pub const SoundFlags = packed struct {
    stream: bool = false,
    decode: bool = false,
    @"async": bool = false,
    wait_init: bool = false,
    no_default_attachment: bool = false,
    no_pitch: bool = false,
    no_spatialization: bool = false,

    _pad0: u9 = 0,
    _pad1: u16 = 0,

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

pub const PanMode = enum(c_uint) {
    balance,
    pan,
};

pub const AttenuationModel = enum(c_uint) {
    none,
    inverse,
    linear,
    exponential,
};

pub const Positioning = enum(c_uint) {
    absolute,
    relative,
};

pub const Format = enum(c_uint) {
    unknown,
    @"u8",
    s16,
    s24,
    s32,
    @"f32",
};

pub const DeviceType = enum(c_uint) {
    playback = 1,
    capture = 2,
    duplex = 3,
    loopback = 4,
};

pub const DeviceState = enum(c_uint) {
    uninitialized = 0,
    stopped = 1,
    started = 2,
    starting = 3,
    stopping = 4,
};

pub const Channel = c.ma_channel;

pub const PlaybackDataCallback = struct {
    context: ?*anyopaque = null,
    func: ?fn (context: ?*anyopaque, outptr: *anyopaque, num_frames: u32) void = null,
};

pub const CaptureDataCallback = struct {
    context: ?*anyopaque = null,
    func: ?fn (context: ?*anyopaque, inptr: *const anyopaque, num_frames: u32) void = null,
};

pub const DeviceConfig = struct {
    raw: c.ma_device_config,

    playback_callback: PlaybackDataCallback = .{},
    capture_callback: CaptureDataCallback = .{},

    pub fn init(device_type: DeviceType) DeviceConfig {
        return .{ .raw = c.ma_device_config_init(@bitCast(c_uint, device_type)) };
    }
};

pub const EngineConfig = struct {
    raw: c.ma_engine_config,

    pub fn init() EngineConfig {
        return .{ .raw = c.ma_engine_config_init() };
    }
};

pub const SoundConfig = struct {
    raw: c.ma_sound_config,

    pub fn init() SoundConfig {
        return .{ .raw = c.ma_sound_config_init() };
    }
};

pub const DataSourceRef = *align(@sizeOf(usize)) DataSource;
pub const DataSource = opaque {
    pub fn asRaw(data_source: DataSourceRef) *c.ma_data_source {
        return @ptrCast(*c.ma_data_source, data_source);
    }
    // TODO: Add methods.
};

pub const NodeGraphRef = *align(@sizeOf(usize)) NodeGraph;
pub const NodeGraph = opaque {
    // TODO: Add methods.
};

pub const ResourceManagerRef = *align(@sizeOf(usize)) ResourceManager;
pub const ResourceManager = opaque {
    // TODO: Add methods.
};

pub const ContextRef = *align(@sizeOf(usize)) Context;
pub const Context = opaque {
    pub fn asRaw(context: ContextRef) *c.ma_context {
        return @ptrCast(*c.ma_context, context);
    }
    // TODO: Add methods.
};

pub const DeviceRef = *align(@sizeOf(usize)) Device;
pub const Device = opaque {
    const InternalState = struct {
        playback_callback: PlaybackDataCallback = .{},
        capture_callback: CaptureDataCallback = .{},
    };

    fn internalDataCallback(
        raw_device: ?*c.ma_device,
        outptr: ?*anyopaque,
        inptr: ?*const anyopaque,
        num_frames: u32,
    ) callconv(.C) void {
        assert(raw_device != null);

        const internal_state = @ptrCast(
            *InternalState,
            @alignCast(@alignOf(InternalState), raw_device.?.pUserData),
        );

        if (num_frames > 0) {
            // Dispatch playback callback.
            if (outptr != null) if (internal_state.playback_callback.func) |func| {
                func(internal_state.playback_callback.context, outptr.?, num_frames);
            };

            // Dispatch capture callback.
            if (inptr != null) if (internal_state.capture_callback.func) |func| {
                func(internal_state.capture_callback.context, inptr.?, num_frames);
            };
        }
    }

    pub fn init(allocator: std.mem.Allocator, context: ?ContextRef, config: *DeviceConfig) Error!DeviceRef {
        // We don't allow setting below fields (we use them internally), please use
        // `config.playback_callback` and/or `config.capture_callback` instead.
        assert(config.raw.dataCallback == null);
        assert(config.raw.pUserData == null);

        var handle = allocator.create(c.ma_device) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        const internal_state = allocator.create(InternalState) catch return error.OutOfMemory;
        errdefer allocator.destroy(internal_state);

        internal_state.* = .{
            .playback_callback = config.playback_callback,
            .capture_callback = config.capture_callback,
        };

        config.raw.pUserData = internal_state;

        if (config.playback_callback.func != null or config.capture_callback.func != null) {
            config.raw.dataCallback = internalDataCallback;
        }

        try checkResult(c.ma_device_init(
            if (context) |ctx| ctx.asRaw() else null,
            &config.raw,
            handle,
        ));

        return @ptrCast(DeviceRef, handle);
    }

    pub fn deinit(device: DeviceRef, allocator: std.mem.Allocator) void {
        const raw = device.asRaw();
        allocator.destroy(@ptrCast(
            *InternalState,
            @alignCast(@alignOf(InternalState), raw.pUserData),
        ));
        c.ma_device_uninit(raw);
        allocator.destroy(raw);
    }

    pub fn asRaw(device: DeviceRef) *c.ma_device {
        return @ptrCast(*c.ma_device, device);
    }

    pub fn getContext(device: DeviceRef) ContextRef {
        const raw = c.ma_device_get_context(device.asRaw());
        assert(raw != null);
        return @ptrCast(ContextRef, raw);
    }

    pub fn getLog(device: DeviceRef) ?LogRef {
        const raw = c.ma_device_get_log(device.asRaw());
        if (raw != null) return @ptrCast(LogRef, raw);
        return null;
    }

    pub fn start(device: DeviceRef) Error!void {
        try checkResult(c.ma_device_start(device.asRaw()));
    }

    pub fn stop(device: DeviceRef) Error!void {
        try checkResult(c.ma_device_stop(device.asRaw()));
    }

    pub fn isStarted(device: DeviceRef) bool {
        return c.ma_device_is_started(device.asRaw()) == c.MA_TRUE;
    }

    pub fn getState(device: DeviceRef) DeviceState {
        return @intToEnum(DeviceState, c.ma_device_get_state(device.asRaw()));
    }

    pub fn setMasterVolume(device: DeviceRef, volume: f32) Error!void {
        try checkResult(c.ma_device_set_master_volume(device.asRaw(), volume));
    }
    pub fn getMasterVolume(device: DeviceRef) Error!f32 {
        var volume: f32 = undefined;
        try checkResult(c.ma_device_get_master_volume(device.asRaw(), &volume));
        return volume;
    }
};

pub const LogRef = *align(@sizeOf(usize)) Log;
pub const Log = opaque {
    // TODO: Add methods.
};

pub const NodeRef = *align(@sizeOf(usize)) Node;
pub const Node = opaque {
    pub fn asRaw(node: NodeRef) *c.ma_node {
        return @ptrCast(*c.ma_node, node);
    }
    // TODO: Add methods.
};

pub const EngineRef = *align(@sizeOf(usize)) Engine;
pub const Engine = opaque {
    pub fn init(allocator: std.mem.Allocator, config: ?EngineConfig) Error!EngineRef {
        var handle = allocator.create(c.ma_engine) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_engine_init(if (config) |conf| &conf.raw else null, handle));

        return @ptrCast(EngineRef, handle);
    }

    pub fn deinit(engine: EngineRef, allocator: std.mem.Allocator) void {
        const raw = engine.asRaw();
        c.ma_engine_uninit(raw);
        allocator.destroy(raw);
    }

    pub fn asRaw(engine: EngineRef) *c.ma_engine {
        return @ptrCast(*c.ma_engine, engine);
    }

    pub fn readPcmFrames(engine: EngineRef, outptr: *anyopaque, num_frames: u64, num_frames_read: ?*u64) Error!void {
        try checkResult(c.ma_engine_read_pcm_frames(engine.asRaw(), outptr, num_frames, num_frames_read));
    }

    pub fn getResourceManager(engine: EngineRef) ?ResourceManagerRef {
        const raw = c.ma_engine_get_resource_manager(engine.asRaw());
        if (raw != null) return @ptrCast(ResourceManagerRef, raw);
        return null;
    }

    pub fn getDevice(engine: EngineRef) ?DeviceRef {
        const raw = c.ma_engine_get_device(engine.asRaw());
        if (raw != null) return @ptrCast(DeviceRef, raw);
        return null;
    }

    pub fn getLog(engine: EngineRef) ?LogRef {
        const raw = c.ma_engine_get_log(engine.asRaw());
        if (raw != null) return @ptrCast(LogRef, raw);
        return null;
    }

    pub fn getNodeGraph(engine: EngineRef) NodeGraphRef {
        const raw = c.ma_engine_get_node_graph(engine.asRaw());
        assert(raw != null);
        return @ptrCast(NodeGraphRef, raw);
    }

    pub fn getEndpoint(engine: EngineRef) ?NodeRef {
        const raw = c.ma_engine_get_endpoint(engine.asRaw());
        assert(raw != null);
        return @ptrCast(NodeRef, raw);
    }

    pub fn getTime(engine: EngineRef) u64 {
        return c.ma_engine_get_time(engine.asRaw());
    }
    pub fn setTime(engine: EngineRef, global_time: u64) Error!void {
        try checkResult(c.ma_engine_set_time(engine.asRaw(), global_time));
    }

    pub fn getChannels(engine: EngineRef) u32 {
        return c.ma_engine_get_channels(engine.asRaw());
    }

    pub fn getSampleRate(engine: EngineRef) u32 {
        return c.ma_engine_get_sample_rate(engine.asRaw());
    }

    pub fn start(engine: EngineRef) Error!void {
        try checkResult(c.ma_engine_start(engine.asRaw()));
    }
    pub fn stop(engine: EngineRef) Error!void {
        try checkResult(c.ma_engine_stop(engine.asRaw()));
    }

    pub fn setVolume(engine: EngineRef, volume: f32) Error!void {
        try checkResult(c.ma_engine_set_volume(engine.asRaw(), volume));
    }

    pub fn setGainDb(engine: EngineRef, gain_db: f32) Error!void {
        try checkResult(c.ma_engine_set_gain_db(engine.asRaw(), gain_db));
    }

    pub fn getNumListeners(engine: EngineRef) u32 {
        return c.ma_engine_get_listener_count(engine.asRaw());
    }

    pub fn findClosestListener(engine: EngineRef, absolute_pos_xyz: [3]f32) u32 {
        return c.ma_engine_find_closest_listener(
            engine.asRaw(),
            absolute_pos_xyz[0],
            absolute_pos_xyz[1],
            absolute_pos_xyz[2],
        );
    }

    pub fn setListenerPosition(engine: EngineRef, index: u32, v: [3]f32) void {
        c.ma_engine_listener_set_position(engine.asRaw(), index, v[0], v[1], v[2]);
    }
    pub fn getListenerPosition(engine: EngineRef, index: u32) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_engine_listener_get_position(engine.asRaw(), index, &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_engine_listener_get_position(engine: *c.ma_engine, index: u32, vout: *c.ma_vec3f) void;

    pub fn setListenerDirection(engine: EngineRef, index: u32, v: [3]f32) void {
        c.ma_engine_listener_set_direction(engine.asRaw(), index, v[0], v[1], v[2]);
    }
    pub fn getListenerDirection(engine: EngineRef, index: u32) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_engine_listener_get_direction(engine.asRaw(), index, &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_engine_listener_get_direction(engine: *c.ma_engine, index: u32, vout: *c.ma_vec3f) void;

    pub fn setListenerVelocity(engine: EngineRef, index: u32, v: [3]f32) void {
        c.ma_engine_listener_set_velocity(engine.asRaw(), index, v[0], v[1], v[2]);
    }
    pub fn getListenerVelocity(engine: EngineRef, index: u32) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_engine_listener_get_velocity(engine.asRaw(), index, &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_engine_listener_get_velocity(engine: *c.ma_engine, index: u32, vout: *c.ma_vec3f) void;

    pub fn setListenerWorldUp(engine: EngineRef, index: u32, v: [3]f32) void {
        c.ma_engine_listener_set_world_up(engine.asRaw(), index, v[0], v[1], v[2]);
    }
    pub fn getListenerWorldUp(engine: EngineRef, index: u32) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_engine_listener_get_world_up(engine.asRaw(), index, &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_engine_listener_get_world_up(engine: *c.ma_engine, index: u32, vout: *c.ma_vec3f) void;

    pub fn setListenerEnabled(engine: EngineRef, index: u32, enabled: bool) void {
        c.ma_engine_listener_set_enabled(engine.asRaw(), index, if (enabled) c.MA_TRUE else c.MA_FALSE);
    }
    pub fn isListenerEnabled(engine: EngineRef, index: u32) bool {
        return c.ma_engine_listener_is_enabled(engine.asRaw(), index) == c.MA_TRUE;
    }

    pub fn setListenerCone(
        engine: EngineRef,
        index: u32,
        inner_radians: f32,
        outer_radians: f32,
        outer_gain: f32,
    ) void {
        c.ma_engine_listener_set_cone(engine.asRaw(), index, inner_radians, outer_radians, outer_gain);
    }
    pub fn getListenerCone(
        engine: EngineRef,
        index: u32,
        inner_radians: ?*f32,
        outer_radians: ?*f32,
        outer_gain: ?*f32,
    ) void {
        c.ma_engine_listener_get_cone(engine.asRaw(), index, inner_radians, outer_radians, outer_gain);
    }

    pub fn playSound(engine: EngineRef, filepath: [:0]const u8, sgroup: ?SoundGroupRef) Error!void {
        try checkResult(c.ma_engine_play_sound(
            engine.asRaw(),
            filepath.ptr,
            if (sgroup) |g| g.asRaw() else null,
        ));
    }

    pub fn playSoundEx(
        engine: EngineRef,
        filepath: [:0]const u8,
        node: ?NodeRef,
        node_input_bus_index: u32,
    ) Error!void {
        try checkResult(c.ma_engine_play_sound_ex(
            engine.asRaw(),
            filepath.ptr,
            if (node) |n| n.asRaw() else null,
            node_input_bus_index,
        ));
    }
};

pub const SoundRef = *align(@sizeOf(usize)) Sound;
pub const Sound = opaque {
    pub fn initFile(
        allocator: std.mem.Allocator,
        engine: EngineRef,
        filepath: [:0]const u8,
        args: struct {
            flags: SoundFlags = .{},
            sgroup: ?SoundGroupRef = null,
            done_fence: ?FenceRef = null,
        },
    ) Error!SoundRef {
        var handle = allocator.create(c.ma_sound) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_sound_init_from_file(
            engine.asRaw(),
            filepath.ptr,
            @bitCast(u32, args.flags),
            if (args.sgroup) |g| g.asRaw() else null,
            if (args.done_fence) |f| f.asRaw() else null,
            handle,
        ));

        return @ptrCast(SoundRef, handle);
    }

    pub fn initDataSource(
        allocator: std.mem.Allocator,
        engine: EngineRef,
        data_source: DataSourceRef,
        flags: SoundFlags,
        sgroup: ?SoundGroupRef,
    ) Error!SoundRef {
        var handle = allocator.create(c.ma_sound) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_sound_init_from_data_source(
            engine.asRaw(),
            data_source.asRaw(),
            @bitCast(u32, flags),
            if (sgroup) |g| g.handle else null,
            handle,
        ));

        return @ptrCast(SoundRef, handle);
    }

    pub fn initCopy(
        allocator: std.mem.Allocator,
        engine: EngineRef,
        existing_sound: SoundRef,
        flags: SoundFlags,
        sgroup: ?SoundGroupRef,
    ) Error!SoundRef {
        var handle = allocator.create(c.ma_sound) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_sound_init_copy(
            engine.asRaw(),
            existing_sound.asRaw(),
            @bitCast(u32, flags),
            if (sgroup) |g| g.asRaw() else null,
            handle,
        ));

        return @ptrCast(SoundRef, handle);
    }

    pub fn initConfig(
        allocator: std.mem.Allocator,
        engine: EngineRef,
        config: SoundConfig,
    ) Error!SoundRef {
        var handle = allocator.create(c.ma_sound) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_sound_init_ex(engine.asRaw(), &config.raw, handle));

        return @ptrCast(SoundRef, handle);
    }

    pub fn deinit(sound: SoundRef, allocator: std.mem.Allocator) void {
        c.ma_sound_uninit(sound.asRaw());
        allocator.destroy(sound.asRaw());
    }

    pub fn asRaw(sound: SoundRef) *c.ma_sound {
        return @ptrCast(*c.ma_sound, sound);
    }

    pub fn getEngine(sound: SoundRef) EngineRef {
        return @ptrCast(EngineRef, c.ma_sound_get_engine(sound.asRaw()));
    }

    pub fn getDataSource(sound: SoundRef) ?DataSourceRef {
        const raw = c.ma_sound_get_data_source(sound.asRaw());
        if (raw != null) return @ptrCast(DataSourceRef, raw);
        return null;
    }

    pub fn start(sound: SoundRef) Error!void {
        try checkResult(c.ma_sound_start(sound.asRaw()));
    }
    pub fn stop(sound: SoundRef) Error!void {
        try checkResult(c.ma_sound_stop(sound.asRaw()));
    }

    pub fn setVolume(sound: SoundRef, volume: f32) void {
        c.ma_sound_set_volume(sound.asRaw(), volume);
    }
    pub fn getVolume(sound: SoundRef) f32 {
        return c.ma_sound_get_volume(sound.asRaw());
    }

    pub fn setPan(sound: SoundRef, pan: f32) void {
        c.ma_sound_set_pan(sound.asRaw(), pan);
    }
    pub fn getPan(sound: SoundRef) f32 {
        return c.ma_sound_get_pan(sound.asRaw());
    }

    pub fn setPanMode(sound: SoundRef, pan_mode: PanMode) void {
        c.ma_sound_set_pan_mode(sound.asRaw(), @enumToInt(pan_mode));
    }
    pub fn getPanMode(sound: SoundRef) PanMode {
        return @intToEnum(PanMode, c.ma_sound_get_pan_mode(sound.asRaw()));
    }

    pub fn setPitch(sound: SoundRef, pitch: f32) void {
        c.ma_sound_set_pitch(sound.asRaw(), pitch);
    }
    pub fn getPitch(sound: SoundRef) f32 {
        return c.ma_sound_get_pitch(sound.asRaw());
    }

    pub fn setSpatializationEnabled(sound: SoundRef, enabled: bool) void {
        c.ma_sound_set_spatialization_enabled(sound.asRaw(), if (enabled) c.MA_TRUE else c.MA_FALSE);
    }
    pub fn isSpatializationEnabled(sound: SoundRef) bool {
        return c.ma_sound_is_spatialization_enabled(sound.asRaw()) == c.MA_TRUE;
    }

    pub fn setPinnedListenerIndex(sound: SoundRef, index: u32) void {
        c.ma_sound_set_pinned_listener_index(sound.asRaw(), index);
    }
    pub fn getPinnedListenerIndex(sound: SoundRef) u32 {
        return c.ma_sound_get_pinned_listener_index(sound.asRaw());
    }
    pub fn getListenerIndex(sound: SoundRef) u32 {
        return c.ma_sound_get_listener_index(sound.asRaw());
    }

    pub fn getDirectionToListener(sound: SoundRef) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_get_direction_to_listener(sound.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_get_direction_to_listener(sound: *c.ma_sound, vout: *c.ma_vec3f) void;

    pub fn setPosition(sound: SoundRef, v: [3]f32) void {
        c.ma_sound_set_position(sound.asRaw(), v[0], v[1], v[2]);
    }
    pub fn getPosition(sound: SoundRef) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_get_position(sound.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_get_position(sound: *c.ma_sound, vout: *c.ma_vec3f) void;

    pub fn setDirection(sound: SoundRef, v: [3]f32) void {
        c.ma_sound_set_direction(sound.asRaw(), v[0], v[1], v[2]);
    }
    pub fn getDirection(sound: SoundRef) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_get_direction(sound.asRaw());
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_get_direction(sound: *c.ma_sound, vout: *c.ma_vec3f) void;

    pub fn setVelocity(sound: SoundRef, v: [3]f32) void {
        c.ma_sound_set_velocity(sound.asRaw(), v[0], v[1], v[2]);
    }
    pub fn getVelocity(sound: SoundRef) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_get_velocity(sound.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_get_velocity(sound: *c.ma_sound, vout: *c.ma_vec3f) void;

    pub fn setAttenuationModel(sound: SoundRef, model: AttenuationModel) void {
        c.ma_sound_set_attenuation_model(sound.asRaw(), @enumToInt(model));
    }
    pub fn getAttenuationModel(sound: SoundRef) AttenuationModel {
        return @intToEnum(AttenuationModel, c.ma_sound_get_attenuation_model(sound.asRaw()));
    }

    pub fn setPositioning(sound: SoundRef, pos: Positioning) void {
        c.ma_sound_set_positioning(sound.asRaw(), @enumToInt(pos));
    }
    pub fn getPositioning(sound: SoundRef) Positioning {
        return @intToEnum(Positioning, c.ma_sound_get_positioning(sound.asRaw()));
    }

    pub fn setRolloff(sound: SoundRef, rolloff: f32) void {
        c.ma_sound_set_rolloff(sound.asRaw(), rolloff);
    }
    pub fn getRolloff(sound: SoundRef) f32 {
        return c.ma_sound_get_rolloff(sound.asRaw());
    }

    pub fn setMinGain(sound: SoundRef, min_gain: f32) void {
        c.ma_sound_set_min_gain(sound.asRaw(), min_gain);
    }
    pub fn getMinGain(sound: SoundRef) f32 {
        return c.ma_sound_get_min_gain(sound.asRaw());
    }

    pub fn setMaxGain(sound: SoundRef, max_gain: f32) void {
        c.ma_sound_set_max_gain(sound.asRaw(), max_gain);
    }
    pub fn getMaxGain(sound: SoundRef) f32 {
        return c.ma_sound_get_max_gain(sound.asRaw());
    }

    pub fn setMinDistance(sound: SoundRef, min_distance: f32) void {
        c.ma_sound_set_min_distance(sound.asRaw(), min_distance);
    }
    pub fn getMinDistance(sound: SoundRef) f32 {
        return c.ma_sound_get_min_distance(sound.asRaw());
    }

    pub fn setMaxDistance(sound: SoundRef, max_distance: f32) void {
        c.ma_sound_set_max_distance(sound.asRaw(), max_distance);
    }
    pub fn getMaxDistance(sound: SoundRef) f32 {
        return c.ma_sound_get_max_distance(sound.asRaw());
    }

    pub fn setCone(sound: SoundRef, inner_radians: f32, outer_radians: f32, outer_gain: f32) void {
        c.ma_sound_set_cone(sound.asRaw(), inner_radians, outer_radians, outer_gain);
    }
    pub fn getCone(sound: SoundRef, inner_radians: ?*f32, outer_radians: ?*f32, outer_gain: ?*f32) void {
        c.ma_sound_get_cone(sound.asRaw(), inner_radians, outer_radians, outer_gain);
    }

    pub fn setDopplerFactor(sound: SoundRef, factor: f32) void {
        c.ma_sound_set_doppler_factor(sound.asRaw(), factor);
    }
    pub fn getDopplerFactor(sound: SoundRef) f32 {
        return c.ma_sound_get_doppler_factor(sound.asRaw());
    }

    pub fn setDirectionalAttenuationFactor(sound: SoundRef, factor: f32) void {
        c.ma_sound_set_directional_attenuation_factor(sound.asRaw(), factor);
    }
    pub fn getDirectionalAttenuationFactor(sound: SoundRef) f32 {
        return c.ma_sound_get_directional_attenuation_factor(sound.asRaw());
    }

    pub fn setFadePcmFrames(sound: SoundRef, volume_begin: f32, volume_end: f32, len_in_frames: u64) void {
        c.ma_sound_set_fade_in_pcm_frames(sound.asRaw(), volume_begin, volume_end, len_in_frames);
    }
    pub fn setFadeMilliseconds(sound: SoundRef, volume_begin: f32, volume_end: f32, len_in_ms: u64) void {
        c.ma_sound_set_fade_in_milliseconds(sound.asRaw(), volume_begin, volume_end, len_in_ms);
    }
    pub fn getCurrentFadeVolume(sound: SoundRef) f32 {
        return c.ma_sound_get_current_fade_volume(sound.asRaw());
    }

    pub fn setStartTimePcmFrames(sound: SoundRef, abs_global_time_in_frames: u64) void {
        c.ma_sound_set_start_time_in_pcm_frames(sound.asRaw(), abs_global_time_in_frames);
    }
    pub fn setStartTimeMilliseconds(sound: SoundRef, abs_global_time_in_ms: u64) void {
        c.ma_sound_set_start_time_in_milliseconds(sound.asRaw(), abs_global_time_in_ms);
    }

    pub fn setStopTimePcmFrames(sound: SoundRef, abs_global_time_in_frames: u64) void {
        c.ma_sound_set_stop_time_in_pcm_frames(sound.asRaw(), abs_global_time_in_frames);
    }
    pub fn setStopTimeMilliseconds(sound: SoundRef, abs_global_time_in_ms: u64) void {
        c.ma_sound_set_stop_time_in_milliseconds(sound.asRaw(), abs_global_time_in_ms);
    }

    pub fn isPlaying(sound: SoundRef) bool {
        return c.ma_sound_is_playing(sound.asRaw()) == c.MA_TRUE;
    }

    pub fn getTimePcmFrames(sound: SoundRef) u64 {
        return c.ma_sound_get_time_in_pcm_frames(sound.asRaw());
    }

    pub fn setLooping(sound: SoundRef, looping: bool) void {
        return c.ma_sound_set_looping(sound.asRaw(), if (looping) c.MA_TRUE else c.MA_FALSE);
    }
    pub fn isLooping(sound: SoundRef) bool {
        return c.ma_sound_is_looping(sound.asRaw()) == c.MA_TRUE;
    }

    pub fn isAtEnd(sound: SoundRef) bool {
        return c.ma_sound_at_end(sound.asRaw()) == c.MA_TRUE;
    }

    pub fn seekToPcmFrame(sound: SoundRef, frame: u64) Error!void {
        try checkResult(c.ma_sound_seek_to_pcm_frame(sound.asRaw(), frame));
    }

    pub fn getDataFormat(
        sound: SoundRef,
        format: ?*Format,
        num_channels: ?*u32,
        sample_rate: ?*u32,
        channel_map: ?[]Channel,
    ) Error!void {
        try checkResult(c.ma_sound_get_data_format(
            sound.asRaw(),
            if (format) |fmt| @ptrCast(*u32, fmt) else null,
            num_channels,
            sample_rate,
            if (channel_map) |chm| chm.ptr else null,
            if (channel_map) |chm| chm.len else 0,
        ));
    }

    pub fn getCursorPcmFrames(sound: SoundRef) Error!u64 {
        var cursor: u64 = undefined;
        try checkResult(c.ma_sound_get_cursor_in_pcm_frames(sound.asRaw(), &cursor));
        return cursor;
    }

    pub fn getLengthPcmFrames(sound: SoundRef) Error!u64 {
        var length: u64 = undefined;
        try checkResult(c.ma_sound_get_length_in_pcm_frames(sound.asRaw(), &length));
        return length;
    }

    pub fn getCursorSeconds(sound: SoundRef) Error!f32 {
        var cursor: f32 = undefined;
        try checkResult(c.ma_sound_get_cursor_in_seconds(sound.asRaw(), &cursor));
        return cursor;
    }

    pub fn getLengthSeconds(sound: SoundRef) Error!f32 {
        var length: f32 = undefined;
        try checkResult(c.ma_sound_get_length_in_seconds(sound.asRaw(), &length));
        return length;
    }
};

pub const SoundGroupRef = *align(@sizeOf(usize)) SoundGroup;
pub const SoundGroup = opaque {
    pub fn init(
        allocator: std.mem.Allocator,
        engine: EngineRef,
        flags: SoundFlags,
        parent: ?SoundGroupRef,
    ) Error!SoundGroupRef {
        var handle = allocator.create(c.ma_sound_group) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_sound_group_init(
            engine.asRaw(),
            @bitCast(u32, flags),
            if (parent) |p| p.asRaw() else null,
            handle,
        ));

        return @ptrCast(SoundGroupRef, handle);
    }

    pub fn deinit(sgroup: SoundGroupRef, allocator: std.mem.Allocator) void {
        c.ma_sound_group_uninit(sgroup.asRaw());
        allocator.destroy(sgroup.asRaw());
    }

    pub fn asRaw(sound: SoundGroupRef) *c.ma_sound_group {
        return @ptrCast(*c.ma_sound_group, sound);
    }

    pub fn getEngine(sgroup: SoundGroupRef) EngineRef {
        return @ptrCast(EngineRef, c.ma_sound_group_get_engine(sgroup.asRaw()));
    }

    pub fn start(sgroup: SoundGroupRef) Error!void {
        try checkResult(c.ma_sound_group_start(sgroup.asRaw()));
    }
    pub fn stop(sgroup: SoundGroupRef) Error!void {
        try checkResult(c.ma_sound_group_stop(sgroup.asRaw()));
    }

    pub fn setVolume(sgroup: SoundGroupRef, volume: f32) void {
        c.ma_sound_group_set_volume(sgroup.asRaw(), volume);
    }
    pub fn getVolume(sgroup: SoundGroupRef) f32 {
        return c.ma_sound_group_get_volume(sgroup.asRaw());
    }

    pub fn setPan(sgroup: SoundGroupRef, pan: f32) void {
        c.ma_sound_group_set_pan(sgroup.asRaw(), pan);
    }
    pub fn getPan(sgroup: SoundGroupRef) f32 {
        return c.ma_sound_group_get_pan(sgroup.asRaw());
    }

    pub fn setPanMode(sgroup: SoundGroupRef, pan_mode: PanMode) void {
        c.ma_sound_group_set_pan_mode(sgroup.asRaw(), pan_mode);
    }
    pub fn getPanMode(sgroup: SoundGroupRef) PanMode {
        return c.ma_sound_group_get_pan_mode(sgroup.asRaw());
    }

    pub fn setPitch(sgroup: SoundGroupRef, pitch: f32) void {
        c.ma_sound_group_set_pitch(sgroup.asRaw(), pitch);
    }
    pub fn getPitch(sgroup: SoundGroupRef) f32 {
        return c.ma_sound_group_get_pitch(sgroup.asRaw());
    }

    pub fn setSpatializationEnabled(sgroup: SoundGroupRef, enabled: bool) void {
        c.ma_sound_group_set_spatialization_enabled(sgroup.asRaw(), if (enabled) c.MA_TRUE else c.MA_FALSE);
    }
    pub fn isSpatializationEnabled(sgroup: SoundGroupRef) bool {
        return c.ma_sound_group_is_spatialization_enabled(sgroup.asRaw()) == c.MA_TRUE;
    }

    pub fn setPinnedListenerIndex(sgroup: SoundGroupRef, index: u32) void {
        c.ma_sound_group_set_pinned_listener_index(sgroup.asRaw(), index);
    }
    pub fn getPinnedListenerIndex(sgroup: SoundGroupRef) u32 {
        return c.ma_sound_group_get_pinned_listener_index(sgroup.asRaw());
    }
    pub fn getListenerIndex(sgroup: SoundGroupRef) u32 {
        return c.ma_sound_group_get_listener_index(sgroup.asRaw());
    }

    pub fn getDirectionToListener(sgroup: SoundGroupRef) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_group_get_direction_to_listener(sgroup.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_group_get_direction_to_listener(sgroup: *c.ma_sound_group, vout: *c.ma_vec3f) void;

    pub fn setPosition(sgroup: SoundGroupRef, v: [3]f32) void {
        c.ma_sound_group_set_position(sgroup.asRaw(), v[0], v[1], v[2]);
    }
    pub fn getPosition(sgroup: SoundGroupRef) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_group_get_position(sgroup.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_group_get_position(sgroup: *c.ma_sound_group, vout: *c.ma_vec3f) void;

    pub fn setDirection(sgroup: SoundGroupRef, v: [3]f32) void {
        c.ma_sound_group_set_direction(sgroup.asRaw(), v[0], v[1], v[2]);
    }
    pub fn getDirection(sgroup: SoundGroupRef) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_group_get_direction(sgroup.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_group_get_direction(sgroup: *c.ma_sound_group, vout: *c.ma_vec3f) void;

    pub fn setVelocity(sgroup: SoundGroupRef, v: [3]f32) void {
        c.ma_sound_group_set_velocity(sgroup.asRaw(), v[0], v[1], v[2]);
    }
    pub fn getVelocity(sgroup: SoundGroupRef) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_group_get_velocity(sgroup.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_group_get_velocity(sgroup: *c.ma_sound_group, vout: *c.ma_vec3f) void;

    pub fn setAttenuationModel(sgroup: SoundGroupRef, model: AttenuationModel) void {
        c.ma_sound_group_set_attenuation_model(sgroup.asRaw(), model);
    }
    pub fn getAttenuationModel(sgroup: SoundGroupRef) AttenuationModel {
        return c.ma_sound_group_get_attenuation_model(sgroup.asRaw());
    }

    pub fn setPositioning(sgroup: SoundGroupRef, pos: Positioning) void {
        c.ma_sound_group_set_positioning(sgroup.asRaw(), pos);
    }
    pub fn getPositioning(sgroup: SoundGroupRef) Positioning {
        return c.ma_sound_group_get_positioning(sgroup.asRaw());
    }

    pub fn setRolloff(sgroup: SoundGroupRef, rolloff: f32) void {
        c.ma_sound_group_set_rolloff(sgroup.asRaw(), rolloff);
    }
    pub fn getRolloff(sgroup: SoundGroupRef) f32 {
        return c.ma_sound_group_get_rolloff(sgroup.asRaw());
    }

    pub fn setMinGain(sgroup: SoundGroupRef, min_gain: f32) void {
        c.ma_sound_group_set_min_gain(sgroup.asRaw(), min_gain);
    }
    pub fn getMinGain(sgroup: SoundGroupRef) f32 {
        return c.ma_sound_group_get_min_gain(sgroup.asRaw());
    }

    pub fn setMaxGain(sgroup: SoundGroupRef, max_gain: f32) void {
        c.ma_sound_group_set_max_gain(sgroup.asRaw(), max_gain);
    }
    pub fn getMaxGain(sgroup: SoundGroupRef) f32 {
        return c.ma_sound_group_get_max_gain(sgroup.asRaw());
    }

    pub fn setMinDistance(sgroup: SoundGroupRef, min_distance: f32) void {
        c.ma_sound_group_set_min_distance(sgroup.asRaw(), min_distance);
    }
    pub fn getMinDistance(sgroup: SoundGroupRef) f32 {
        return c.ma_sound_group_get_min_distance(sgroup.asRaw());
    }

    pub fn setMaxDistance(sgroup: SoundGroupRef, max_distance: f32) void {
        c.ma_sound_group_set_max_distance(sgroup.asRaw(), max_distance);
    }
    pub fn getMaxDistance(sgroup: SoundGroupRef) f32 {
        return c.ma_sound_group_get_max_distance(sgroup.asRaw());
    }

    pub fn setCone(sgroup: SoundGroupRef, inner_radians: f32, outer_radians: f32, outer_gain: f32) void {
        c.ma_sound_group_set_cone(sgroup.asRaw(), inner_radians, outer_radians, outer_gain);
    }
    pub fn getCone(sgroup: SoundGroupRef, inner_radians: ?*f32, outer_radians: ?*f32, outer_gain: ?*f32) void {
        c.ma_sound_group_get_cone(sgroup.asRaw(), inner_radians, outer_radians, outer_gain);
    }

    pub fn setDopplerFactor(sgroup: SoundGroupRef, factor: f32) void {
        c.ma_sound_group_set_doppler_factor(sgroup.asRaw(), factor);
    }
    pub fn getDopplerFactor(sgroup: SoundGroupRef) f32 {
        return c.ma_sound_group_get_doppler_factor(sgroup.asRaw());
    }

    pub fn setDirectionalAttenuationFactor(sgroup: SoundGroupRef, factor: f32) void {
        c.ma_sound_group_set_directional_attenuation_factor(sgroup.asRaw(), factor);
    }
    pub fn getDirectionalAttenuationFactor(sgroup: SoundGroupRef) f32 {
        return c.ma_sound_group_get_directional_attenuation_factor(sgroup.asRaw());
    }

    pub fn setFadePcmFrames(sgroup: SoundGroupRef, volume_begin: f32, volume_end: f32, len_in_frames: u64) void {
        c.ma_sound_group_set_fade_in_pcm_frames(sgroup.asRaw(), volume_begin, volume_end, len_in_frames);
    }
    pub fn setFadeMilliseconds(sgroup: SoundGroupRef, volume_begin: f32, volume_end: f32, len_in_ms: u64) void {
        c.ma_sound_group_set_fade_in_milliseconds(sgroup.asRaw(), volume_begin, volume_end, len_in_ms);
    }
    pub fn getCurrentFadeVolume(sgroup: SoundGroupRef) f32 {
        return c.ma_sound_group_get_current_fade_volume(sgroup.asRaw());
    }

    pub fn setStartTimePcmFrames(sgroup: SoundGroupRef, abs_global_time_in_frames: u64) void {
        c.ma_sound_group_set_start_time_in_pcm_frames(sgroup.asRaw(), abs_global_time_in_frames);
    }
    pub fn setStartTimeMilliseconds(sgroup: SoundGroupRef, abs_global_time_in_ms: u64) void {
        c.ma_sound_group_set_start_time_in_milliseconds(sgroup.asRaw(), abs_global_time_in_ms);
    }

    pub fn setStopTimePcmFrames(sgroup: SoundGroupRef, abs_global_time_in_frames: u64) void {
        c.ma_sound_group_set_stop_time_in_pcm_frames(sgroup.asRaw(), abs_global_time_in_frames);
    }
    pub fn setStopTimeMilliseconds(sgroup: SoundGroupRef, abs_global_time_in_ms: u64) void {
        c.ma_sound_group_set_stop_time_in_milliseconds(sgroup.asRaw(), abs_global_time_in_ms);
    }

    pub fn isPlaying(sgroup: SoundGroupRef) bool {
        return c.ma_sound_group_is_playing(sgroup.asRaw()) == c.MA_TRUE;
    }

    pub fn getTimePcmFrames(sgroup: SoundGroupRef) u64 {
        return c.ma_sound_group_get_time_in_pcm_frames(sgroup.asRaw());
    }
};

pub const FenceRef = *align(@sizeOf(usize)) Fence;
pub const Fence = opaque {
    pub fn init(allocator: std.mem.Allocator) Error!FenceRef {
        var handle = allocator.create(c.ma_fence) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_fence_init(handle));

        return @ptrCast(FenceRef, handle);
    }

    pub fn deinit(fence: FenceRef, allocator: std.mem.Allocator) void {
        const raw = fence.asRaw();
        c.ma_fence_uninit(raw);
        allocator.destroy(raw);
    }

    pub fn asRaw(fence: FenceRef) *c.ma_fence {
        return @ptrCast(*c.ma_fence, fence);
    }

    pub fn acquire(fence: FenceRef) Error!void {
        try checkResult(c.ma_fence_acquire(fence.asRaw()));
    }

    pub fn release(fence: FenceRef) Error!void {
        try checkResult(c.ma_fence_release(fence.asRaw()));
    }

    pub fn wait(fence: FenceRef) Error!void {
        try checkResult(c.ma_fence_wait(fence.asRaw()));
    }
};

pub const Error = error{
    GenericError,
    InvalidArgs,
    InvalidOperation,
    OutOfMemory,
};

fn checkResult(result: c.ma_result) Error!void {
    // TODO: Handle all errors.
    if (result != c.MA_SUCCESS)
        return error.GenericError;
}

const expect = std.testing.expect;

test "zaudio.engine.basic" {
    const engine = try Engine.init(std.testing.allocator, null);
    defer engine.deinit(std.testing.allocator);

    try engine.setTime(engine.getTime());

    std.debug.print("Channels: {}, SampleRate: {}, NumListeners: {}, ClosestListener: {}\n", .{
        engine.getChannels(),
        engine.getSampleRate(),
        engine.getNumListeners(),
        engine.findClosestListener(.{ 0.0, 0.0, 0.0 }),
    });

    try engine.start();
    try engine.stop();
    try engine.start();
    try engine.setVolume(1.0);
    try engine.setGainDb(1.0);

    engine.setListenerEnabled(0, true);
    try expect(engine.isListenerEnabled(0) == true);

    try expect(engine.getDevice() != null);
}

test "zaudio.soundgroup.basic" {
    const engine = try Engine.init(std.testing.allocator, null);
    defer engine.deinit(std.testing.allocator);

    const sgroup = try SoundGroup.init(std.testing.allocator, engine, .{}, null);
    defer sgroup.deinit(std.testing.allocator);

    try expect(sgroup.getEngine() == engine);

    try sgroup.start();
    try sgroup.stop();
    try sgroup.start();

    sgroup.setVolume(0.5);
    try expect(sgroup.getVolume() == 0.5);

    sgroup.setPan(0.25);
    try expect(sgroup.getPan() == 0.25);

    const sdir = [3]f32{ 1.0, 2.0, 3.0 };
    sgroup.setDirection(sdir);
    {
        const gdir = sgroup.getDirection();
        expect(sdir[0] == gdir[0] and sdir[1] == gdir[1] and sdir[2] == gdir[2]) catch |err| {
            std.debug.print("Direction is: {any} should be: {any}\n", .{ gdir, sdir });
            return err;
        };
    }
}

test "zaudio.fence.basic" {
    const fence = try Fence.init(std.testing.allocator);
    defer fence.deinit(std.testing.allocator);

    try fence.acquire();
    try fence.release();
    try fence.wait();
}

test "zaudio.sound.basic" {
    const engine = try Engine.init(std.testing.allocator, null);
    defer engine.deinit(std.testing.allocator);

    var config = SoundConfig.init();
    config.raw.channelsIn = 1;
    const sound = try Sound.initConfig(std.testing.allocator, engine, config);
    defer sound.deinit(std.testing.allocator);

    sound.setVolume(0.25);
    try expect(sound.getVolume() == 0.25);

    sound.setPanMode(.pan);
    try expect(sound.getPanMode() == .pan);

    sound.setPitch(0.5);
    try expect(sound.getPitch() == 0.5);

    var format: Format = .unknown;
    var num_channels: u32 = 0;
    var sample_rate: u32 = 0;
    try sound.getDataFormat(&format, &num_channels, &sample_rate, null);
    try expect(num_channels == 1);
    try expect(sample_rate > 0);
    try expect(format != .unknown);
}

test "zaudio.device.basic" {
    var config = DeviceConfig.init(.playback);
    config.raw.playback.format = c.ma_format_f32;
    config.raw.playback.channels = 2;
    config.raw.sampleRate = 48_000;
    const device = try Device.init(std.testing.allocator, null, &config);
    defer device.deinit(std.testing.allocator);
    try device.start();
    try expect(device.getState() == .started or device.getState() == .starting);
    try device.stop();
    try expect(device.getState() == .stopped or device.getState() == .stopping);
    const context = device.getContext();
    _ = context;
    const log = device.getLog();
    _ = log;
    try device.setMasterVolume(0.1);
    try expect((try device.getMasterVolume()) == 0.1);
}
