const std = @import("std");

pub const InitFlags = packed struct(u32) {
    timer: bool = false,
    __unused1: bool = false,
    __unused2: bool = false,
    __unused3: bool = false,
    audio: bool = false,
    video: bool = false,
    __unused6: bool = false,
    __unused7: bool = false,
    __unused8: bool = false,
    joystick: bool = false,
    __unused10: bool = false,
    __unused11: bool = false,
    haptic: bool = false,
    game_controller: bool = false,
    events: bool = false,
    sensor: bool = false,
    __unused16: bool = false,
    __unused17: bool = false,
    __unused18: bool = false,
    __unused19: bool = false,
    no_parachute: bool = false,
    __unused: u11 = 0,

    pub const everything = InitFlags{
        .timer = true,
        .audio = true,
        .video = true,
        .events = true,
        .joystick = true,
        .haptic = true,
        .game_controller = true,
        .sensor = true,
    };
};

extern fn SDL_Init(flags: InitFlags) i32;
pub fn init(flags: InitFlags) Error!void {
    if (SDL_Init(flags) < 0)
        return makeError();
}

extern fn SDL_Quit() void;
/// `pub fn quit() void`
pub const quit = SDL_Quit;

extern fn SDL_GetError() ?[*:0]const u8;
/// `pub fn getError() ?[*:0]const u8`
pub const getError = SDL_GetError;

pub const Error = error{SdlError};

pub fn makeError() error{SdlError} {
    if (getError()) |str| {
        std.log.debug("SDL2: {s}", .{str});
    }
    return error.SdlError;
}
