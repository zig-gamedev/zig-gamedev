const std = @import("std");
const c = @cImport(@cInclude("miniaudio.h"));

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

test "zaudio.engine.basic" {
    const engine = try Engine.init(std.testing.allocator, null);
    defer engine.deinit(std.testing.allocator);

    var frames: [2]f32 = undefined;
    try engine.readPcmFrames(f32, frames[0..], null);

    try engine.setTime(engine.getTime());

    std.debug.print("Channels: {}, SampleRate: {}\n", .{ engine.getChannels(), engine.getSampleRate() });

    try engine.start();
    try engine.stop();
    try engine.start();
    try engine.setVolume(1.0);
    try engine.setGainDb(1.0);
}
