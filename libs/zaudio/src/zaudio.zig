const std = @import("std");
const assert = std.debug.assert;
const c = @cImport(@cInclude("miniaudio.h"));

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

pub const PanMode = enum(c_int) {
    balance,
    pan,
};

pub const AttenuationModel = enum(c_int) {
    none,
    inverse,
    linear,
    exponential,
};

pub const Positioning = enum(c_int) {
    absolute,
    relative,
};

pub const EngineConfig = c.ma_engine_config;

pub const Engine = struct {
    handle: *c.ma_engine,

    pub fn init(allocator: std.mem.Allocator, config: ?*const EngineConfig) Error!Engine {
        var handle = allocator.create(c.ma_engine) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_engine_init(config, handle));

        return Engine{ .handle = handle };
    }

    pub fn deinit(engine: Engine, allocator: std.mem.Allocator) void {
        c.ma_engine_uninit(engine.handle);
        allocator.destroy(engine.handle);
    }

    pub fn readPcmFrames(engine: Engine, comptime T: type, frames: []T, frames_read: ?*u64) Error!void {
        try checkResult(c.ma_engine_read_pcm_frames(engine.handle, frames.ptr, frames.len, frames_read));
    }

    pub fn getTime(engine: Engine) u64 {
        return c.ma_engine_get_time(engine.handle);
    }
    pub fn setTime(engine: Engine, global_time: u64) Error!void {
        try checkResult(c.ma_engine_set_time(engine.handle, global_time));
    }

    pub fn getChannels(engine: Engine) u32 {
        return c.ma_engine_get_channels(engine.handle);
    }

    pub fn getSampleRate(engine: Engine) u32 {
        return c.ma_engine_get_sample_rate(engine.handle);
    }

    pub fn start(engine: Engine) Error!void {
        try checkResult(c.ma_engine_start(engine.handle));
    }
    pub fn stop(engine: Engine) Error!void {
        try checkResult(c.ma_engine_stop(engine.handle));
    }

    pub fn setVolume(engine: Engine, volume: f32) Error!void {
        try checkResult(c.ma_engine_set_volume(engine.handle, volume));
    }

    pub fn setGainDb(engine: Engine, gain_db: f32) Error!void {
        try checkResult(c.ma_engine_set_gain_db(engine.handle, gain_db));
    }

    pub fn getNumListeners(engine: Engine) u32 {
        return c.ma_engine_get_listener_count(engine.handle);
    }

    pub fn findClosestListener(engine: Engine, absolute_pos_xyz: [3]f32) u32 {
        return c.ma_engine_find_closest_listener(
            engine.handle,
            absolute_pos_xyz[0],
            absolute_pos_xyz[1],
            absolute_pos_xyz[2],
        );
    }

    pub fn setListenerPosition(engine: Engine, index: u32, v: [3]f32) void {
        c.ma_engine_listener_set_position(engine.handle, index, v[0], v[1], v[2]);
    }
    pub fn getListenerPosition(engine: Engine, index: u32) [3]f32 {
        const v = c.ma_engine_listener_get_position(engine.handle, index);
        return .{ v.x, v.y, v.z };
    }

    pub fn setListenerDirection(engine: Engine, index: u32, v: [3]f32) void {
        c.ma_engine_listener_set_direction(engine.handle, index, v[0], v[1], v[2]);
    }
    pub fn getListenerDirection(engine: Engine, index: u32) [3]f32 {
        const v = c.ma_engine_listener_get_direction(engine.handle, index);
        return .{ v.x, v.y, v.z };
    }

    pub fn setListenerVelocity(engine: Engine, index: u32, v: [3]f32) void {
        c.ma_engine_listener_set_velocity(engine.handle, index, v[0], v[1], v[2]);
    }
    pub fn getListenerVelocity(engine: Engine, index: u32) [3]f32 {
        const v = c.ma_engine_listener_get_velocity(engine.handle, index);
        return .{ v.x, v.y, v.z };
    }

    pub fn setListenerWorldUp(engine: Engine, index: u32, v: [3]f32) void {
        c.ma_engine_listener_set_world_up(engine.handle, index, v[0], v[1], v[2]);
    }
    pub fn getListenerWorldUp(engine: Engine, index: u32) [3]f32 {
        const v = c.ma_engine_listener_get_world_up(engine.handle, index);
        return .{ v.x, v.y, v.z };
    }

    pub fn setListenerEnabled(engine: Engine, index: u32, enabled: bool) void {
        c.ma_engine_listener_set_enabled(engine.handle, index, if (enabled) c.MA_TRUE else c.MA_FALSE);
    }
    pub fn isListenerEnabled(engine: Engine, index: u32) bool {
        return c.ma_engine_listener_is_enabled(engine.handle, index) == c.MA_TRUE;
    }

    pub fn setListenerCone(
        engine: Engine,
        index: u32,
        inner_radians: f32,
        outer_radians: f32,
        outer_gain: f32,
    ) void {
        c.ma_engine_listener_set_cone(engine.handle, index, inner_radians, outer_radians, outer_gain);
    }
    pub fn getListenerCone(
        engine: Engine,
        index: u32,
        inner_radians: ?*f32,
        outer_radians: ?*f32,
        outer_gain: ?*f32,
    ) void {
        c.ma_engine_listener_get_cone(engine.handle, index, inner_radians, outer_radians, outer_gain);
    }
};

pub const SoundGroup = struct {
    handle: *c.ma_sound_group,

    pub fn init(
        allocator: std.mem.Allocator,
        engine: Engine,
        flags: SoundFlags,
        parent: ?SoundGroup,
    ) Error!SoundGroup {
        var handle = allocator.create(c.ma_sound_group) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_sound_group_init(
            engine.handle,
            @bitCast(u32, flags),
            if (parent) |p| p.handle else null,
            handle,
        ));

        return SoundGroup{ .handle = handle };
    }

    pub fn deinit(sgroup: SoundGroup, allocator: std.mem.Allocator) void {
        c.ma_sound_group_uninit(sgroup.handle);
        allocator.destroy(sgroup.handle);
    }

    pub fn getEngine(sgroup: SoundGroup) Engine {
        return Engine{ .handle = c.ma_sound_group_get_engine(sgroup.handle) };
    }

    pub fn start(sgroup: SoundGroup) Error!void {
        try checkResult(c.ma_sound_group_start(sgroup.handle));
    }
    pub fn stop(sgroup: SoundGroup) Error!void {
        try checkResult(c.ma_sound_group_stop(sgroup.handle));
    }

    pub fn setVolume(sgroup: SoundGroup, volume: f32) void {
        c.ma_sound_group_set_volume(sgroup.handle, volume);
    }
    pub fn getVolume(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_volume(sgroup.handle);
    }

    pub fn setPan(sgroup: SoundGroup, pan: f32) void {
        c.ma_sound_group_set_pan(sgroup.handle, pan);
    }
    pub fn getPan(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_pan(sgroup.handle);
    }

    pub fn setPanMode(sgroup: SoundGroup, pan_mode: PanMode) void {
        c.ma_sound_group_set_pan_mode(sgroup.handle, pan_mode);
    }
    pub fn getPanMode(sgroup: SoundGroup) PanMode {
        return c.ma_sound_group_get_pan_mode(sgroup.handle);
    }

    pub fn setPitch(sgroup: SoundGroup, pitch: f32) void {
        c.ma_sound_group_set_pitch(sgroup.handle, pitch);
    }
    pub fn getPitch(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_pitch(sgroup.handle);
    }

    pub fn setSpatializationEnabled(sgroup: SoundGroup, enabled: bool) void {
        c.ma_sound_group_set_spatialization_enabled(sgroup.handle, if (enabled) c.MA_TRUE else c.MA_FALSE);
    }
    pub fn isSpatializationEnabled(sgroup: SoundGroup) bool {
        return c.ma_sound_group_is_spatialization_enabled(sgroup.handle) == c.MA_TRUE;
    }

    pub fn setPinnedListenerIndex(sgroup: SoundGroup, index: u32) void {
        c.ma_sound_group_set_pinned_listener_index(sgroup.handle, index);
    }
    pub fn getPinnedListenerIndex(sgroup: SoundGroup) u32 {
        return c.ma_sound_group_get_pinned_listener_index(sgroup.handle);
    }
    pub fn getListenerIndex(sgroup: SoundGroup) u32 {
        return c.ma_sound_group_get_listener_index(sgroup.handle);
    }

    pub fn getDirectionToListener(sgroup: SoundGroup) [3]f32 {
        const v = c.ma_sound_group_get_direction_to_listener(sgroup.handle);
        return .{ v.x, v.y, v.z };
    }

    pub fn setPosition(sgroup: SoundGroup, v: [3]f32) void {
        c.ma_sound_group_set_position(sgroup.handle, v[0], v[1], v[2]);
    }
    pub fn getPosition(sgroup: SoundGroup) [3]f32 {
        const v = c.ma_sound_group_get_position(sgroup.handle);
        return .{ v.x, v.y, v.z };
    }

    pub fn setDirection(sgroup: SoundGroup, v: [3]f32) void {
        c.ma_sound_group_set_direction(sgroup.handle, v[0], v[1], v[2]);
    }
    pub fn getDirection(sgroup: SoundGroup) [3]f32 {
        const v = c.ma_sound_group_get_direction(sgroup.handle);
        return .{ v.x, v.y, v.z };
    }

    pub fn setVelocity(sgroup: SoundGroup, v: [3]f32) void {
        c.ma_sound_group_set_velocity(sgroup.handle, v[0], v[1], v[2]);
    }
    pub fn getVelocity(sgroup: SoundGroup) [3]f32 {
        const v = c.ma_sound_group_get_velocity(sgroup.handle);
        return .{ v.x, v.y, v.z };
    }

    pub fn setAttenuationModel(sgroup: SoundGroup, model: AttenuationModel) void {
        c.ma_sound_group_set_attenuation_model(sgroup.handle, model);
    }
    pub fn getAttenuationModel(sgroup: SoundGroup) AttenuationModel {
        return c.ma_sound_group_get_attenuation_model(sgroup.handle);
    }

    pub fn setPositioning(sgroup: SoundGroup, pos: Positioning) void {
        c.ma_sound_group_set_positioning(sgroup.handle, pos);
    }
    pub fn getPositioning(sgroup: SoundGroup) Positioning {
        return c.ma_sound_group_get_positioning(sgroup.handle);
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

    var frames: [2]f32 = undefined;
    try engine.readPcmFrames(f32, frames[0..], null);

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
}

test "zaudio.soundgroup.basic" {
    const engine = try Engine.init(std.testing.allocator, null);
    defer engine.deinit(std.testing.allocator);

    const sgroup = try SoundGroup.init(std.testing.allocator, engine, .{}, null);
    defer sgroup.deinit(std.testing.allocator);

    try expect(sgroup.getEngine().handle == engine.handle);

    try sgroup.start();
    try sgroup.stop();
    try sgroup.start();

    sgroup.setVolume(0.5);
    try expect(sgroup.getVolume() == 0.5);

    sgroup.setPan(0.25);
    try expect(sgroup.getPan() == 0.25);

    sgroup.setDirection(.{ 1.0, 2.0, 3.0 });
    {
        const dir = sgroup.getDirection();
        try expect(dir[0] == 1.0 and dir[1] == 2.0 and dir[2] == 3.0);
    }
}
