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

test "zaudio.init" {
    const engine = try Engine.init(std.testing.allocator, null);
    defer engine.deinit(std.testing.allocator);
}
