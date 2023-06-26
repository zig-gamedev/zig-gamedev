const std = @import("std");
const w32 = @import("w32.zig");
const IUnknown = w32.IUnknown;
const BYTE = w32.BYTE;
const HRESULT = w32.HRESULT;
const WINAPI = w32.WINAPI;
const UINT32 = w32.UINT32;
const BOOL = w32.BOOL;
const FALSE = w32.FALSE;

pub const VOLUMEMETER_LEVELS = extern struct {
    pPeakLevels: ?[*]f32 align(1),
    pRMSLevels: ?[*]f32 align(1),
    ChannelCount: UINT32 align(1),
};

pub const REVERB_MIN_FRAMERATE = 20000;
pub const REVERB_MAX_FRAMERATE = 48000;

pub const REVERB_PARAMETERS = extern struct {
    WetDryMix: f32 align(1),
    ReflectionsDelay: UINT32 align(1),
    ReverbDelay: BYTE align(1),
    RearDelay: BYTE align(1),
    SideDelay: BYTE align(1),
    PositionLeft: BYTE align(1),
    PositionRight: BYTE align(1),
    PositionMatrixLeft: BYTE align(1),
    PositionMatrixRight: BYTE align(1),
    EarlyDiffusion: BYTE align(1),
    LateDiffusion: BYTE align(1),
    LowEQGain: BYTE align(1),
    LowEQCutoff: BYTE align(1),
    HighEQGain: BYTE align(1),
    HighEQCutoff: BYTE align(1),
    RoomFilterFreq: f32 align(1),
    RoomFilterMain: f32 align(1),
    RoomFilterHF: f32 align(1),
    ReflectionsGain: f32 align(1),
    ReverbGain: f32 align(1),
    DecayTime: f32 align(1),
    Density: f32 align(1),
    RoomSize: f32 align(1),
    DisableLateField: BOOL align(1),

    pub fn initDefault() REVERB_PARAMETERS {
        return .{
            .WetDryMix = REVERB_DEFAULT_WET_DRY_MIX,
            .ReflectionsDelay = REVERB_DEFAULT_REFLECTIONS_DELAY,
            .ReverbDelay = REVERB_DEFAULT_REVERB_DELAY,
            .RearDelay = REVERB_DEFAULT_REAR_DELAY,
            .SideDelay = REVERB_DEFAULT_REAR_DELAY,
            .PositionLeft = REVERB_DEFAULT_POSITION,
            .PositionRight = REVERB_DEFAULT_POSITION,
            .PositionMatrixLeft = REVERB_DEFAULT_POSITION_MATRIX,
            .PositionMatrixRight = REVERB_DEFAULT_POSITION_MATRIX,
            .EarlyDiffusion = REVERB_DEFAULT_EARLY_DIFFUSION,
            .LateDiffusion = REVERB_DEFAULT_LATE_DIFFUSION,
            .LowEQGain = REVERB_DEFAULT_LOW_EQ_GAIN,
            .LowEQCutoff = REVERB_DEFAULT_LOW_EQ_CUTOFF,
            .HighEQGain = REVERB_DEFAULT_HIGH_EQ_CUTOFF,
            .HighEQCutoff = REVERB_DEFAULT_HIGH_EQ_GAIN,
            .RoomFilterFreq = REVERB_DEFAULT_ROOM_FILTER_FREQ,
            .RoomFilterMain = REVERB_DEFAULT_ROOM_FILTER_MAIN,
            .RoomFilterHF = REVERB_DEFAULT_ROOM_FILTER_HF,
            .ReflectionsGain = REVERB_DEFAULT_REFLECTIONS_GAIN,
            .ReverbGain = REVERB_DEFAULT_REVERB_GAIN,
            .DecayTime = REVERB_DEFAULT_DECAY_TIME,
            .Density = REVERB_DEFAULT_DENSITY,
            .RoomSize = REVERB_DEFAULT_ROOM_SIZE,
            .DisableLateField = REVERB_DEFAULT_DISABLE_LATE_FIELD,
        };
    }
};

pub const REVERB_MIN_WET_DRY_MIX = 0.0;
pub const REVERB_MIN_REFLECTIONS_DELAY = 0;
pub const REVERB_MIN_REVERB_DELAY = 0;
pub const REVERB_MIN_REAR_DELAY = 0;
pub const REVERB_MIN_7POINT1_SIDE_DELAY = 0;
pub const REVERB_MIN_7POINT1_REAR_DELAY = 0;
pub const REVERB_MIN_POSITION = 0;
pub const REVERB_MIN_DIFFUSION = 0;
pub const REVERB_MIN_LOW_EQ_GAIN = 0;
pub const REVERB_MIN_LOW_EQ_CUTOFF = 0;
pub const REVERB_MIN_HIGH_EQ_GAIN = 0;
pub const REVERB_MIN_HIGH_EQ_CUTOFF = 0;
pub const REVERB_MIN_ROOM_FILTER_FREQ = 20.0;
pub const REVERB_MIN_ROOM_FILTER_MAIN = -100.0;
pub const REVERB_MIN_ROOM_FILTER_HF = -100.0;
pub const REVERB_MIN_REFLECTIONS_GAIN = -100.0;
pub const REVERB_MIN_REVERB_GAIN = -100.0;
pub const REVERB_MIN_DECAY_TIME = 0.1;
pub const REVERB_MIN_DENSITY = 0.0;
pub const REVERB_MIN_ROOM_SIZE = 0.0;

pub const REVERB_MAX_WET_DRY_MIX = 100.0;
pub const REVERB_MAX_REFLECTIONS_DELAY = 300;
pub const REVERB_MAX_REVERB_DELAY = 85;
pub const REVERB_MAX_REAR_DELAY = 5;
pub const REVERB_MAX_7POINT1_SIDE_DELAY = 5;
pub const REVERB_MAX_7POINT1_REAR_DELAY = 20;
pub const REVERB_MAX_POSITION = 30;
pub const REVERB_MAX_DIFFUSION = 15;
pub const REVERB_MAX_LOW_EQ_GAIN = 12;
pub const REVERB_MAX_LOW_EQ_CUTOFF = 9;
pub const REVERB_MAX_HIGH_EQ_GAIN = 8;
pub const REVERB_MAX_HIGH_EQ_CUTOFF = 14;
pub const REVERB_MAX_ROOM_FILTER_FREQ = 20000.0;
pub const REVERB_MAX_ROOM_FILTER_MAIN = 0.0;
pub const REVERB_MAX_ROOM_FILTER_HF = 0.0;
pub const REVERB_MAX_REFLECTIONS_GAIN = 20.0;
pub const REVERB_MAX_REVERB_GAIN = 20.0;
pub const REVERB_MAX_DENSITY = 100.0;
pub const REVERB_MAX_ROOM_SIZE = 100.0;

pub const REVERB_DEFAULT_WET_DRY_MIX = 100.0;
pub const REVERB_DEFAULT_REFLECTIONS_DELAY = 5;
pub const REVERB_DEFAULT_REVERB_DELAY = 5;
pub const REVERB_DEFAULT_REAR_DELAY = 5;
pub const REVERB_DEFAULT_7POINT1_SIDE_DELAY = 5;
pub const REVERB_DEFAULT_7POINT1_REAR_DELAY = 20;
pub const REVERB_DEFAULT_POSITION = 6;
pub const REVERB_DEFAULT_POSITION_MATRIX = 27;
pub const REVERB_DEFAULT_EARLY_DIFFUSION = 8;
pub const REVERB_DEFAULT_LATE_DIFFUSION = 8;
pub const REVERB_DEFAULT_LOW_EQ_GAIN = 8;
pub const REVERB_DEFAULT_LOW_EQ_CUTOFF = 4;
pub const REVERB_DEFAULT_HIGH_EQ_GAIN = 8;
pub const REVERB_DEFAULT_HIGH_EQ_CUTOFF = 4;
pub const REVERB_DEFAULT_ROOM_FILTER_FREQ = 5000.0;
pub const REVERB_DEFAULT_ROOM_FILTER_MAIN = 0.0;
pub const REVERB_DEFAULT_ROOM_FILTER_HF = 0.0;
pub const REVERB_DEFAULT_REFLECTIONS_GAIN = 0.0;
pub const REVERB_DEFAULT_REVERB_GAIN = 0.0;
pub const REVERB_DEFAULT_DECAY_TIME = 1.0;
pub const REVERB_DEFAULT_DENSITY = 100.0;
pub const REVERB_DEFAULT_ROOM_SIZE = 100.0;
pub const REVERB_DEFAULT_DISABLE_LATE_FIELD = FALSE;

pub fn createVolumeMeter(apo: *?*IUnknown, _: UINT32) HRESULT {
    var xaudio2_dll = w32.GetModuleHandleA("xaudio2_9redist.dll");
    if (xaudio2_dll == null) {
        xaudio2_dll = w32.LoadLibraryA("d3d12/xaudio2_9redist.dll");
    }

    var createAudioVolumeMeter: *const fn (*?*IUnknown) callconv(WINAPI) HRESULT = undefined;
    createAudioVolumeMeter = @as(
        @TypeOf(createAudioVolumeMeter),
        @ptrCast(w32.GetProcAddress(xaudio2_dll.?, "CreateAudioVolumeMeter").?),
    );

    return createAudioVolumeMeter(apo);
}

pub fn createReverb(apo: *?*IUnknown, _: UINT32) HRESULT {
    var xaudio2_dll = w32.GetModuleHandleA("xaudio2_9redist.dll");
    if (xaudio2_dll == null) {
        xaudio2_dll = w32.LoadLibraryA("d3d12/xaudio2_9redist.dll");
    }

    var createAudioReverb: *const fn (*?*IUnknown) callconv(WINAPI) HRESULT = undefined;
    createAudioReverb = @as(
        @TypeOf(createAudioReverb),
        @ptrCast(w32.GetProcAddress(xaudio2_dll.?, "CreateAudioReverb").?),
    );

    return createAudioReverb(apo);
}
