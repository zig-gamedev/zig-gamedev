const std = @import("std");
const windows = @import("windows.zig");
const IUnknown = windows.IUnknown;
const BYTE = windows.BYTE;
const HRESULT = windows.HRESULT;
const WINAPI = windows.WINAPI;
const UINT32 = windows.UINT32;
const BOOL = windows.BOOL;
const FALSE = windows.FALSE;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

pub const VOLUMEMETER_LEVELS = packed struct {
    pPeakLevels: ?[*]f32,
    pRMSLevels: ?[*]f32,
    ChannelCount: UINT32,
};

pub const REVERB_MIN_FRAMERATE: UINT32 = 20000;
pub const REVERB_MAX_FRAMERATE: UINT32 = 48000;

pub const REVERB_PARAMETERS = packed struct {
    WetDryMix: f32,
    ReflectionsDelay: UINT32,
    ReverbDelay: BYTE,
    RearDelay: BYTE,
    SideDelay: BYTE,
    PositionLeft: BYTE,
    PositionRight: BYTE,
    PositionMatrixLeft: BYTE,
    PositionMatrixRight: BYTE,
    EarlyDiffusion: BYTE,
    LateDiffusion: BYTE,
    LowEQGain: BYTE,
    LowEQCutoff: BYTE,
    HighEQGain: BYTE,
    HighEQCutoff: BYTE,
    RoomFilterFreq: f32,
    RoomFilterMain: f32,
    RoomFilterHF: f32,
    ReflectionsGain: f32,
    ReverbGain: f32,
    DecayTime: f32,
    Density: f32,
    RoomSize: f32,
    DisableLateField: BOOL,

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

pub const REVERB_MIN_WET_DRY_MIX: f32 = 0.0;
pub const REVERB_MIN_REFLECTIONS_DELAY: UINT32 = 0;
pub const REVERB_MIN_REVERB_DELAY: BYTE = 0;
pub const REVERB_MIN_REAR_DELAY: BYTE = 0;
pub const REVERB_MIN_7POINT1_SIDE_DELAY: BYTE = 0;
pub const REVERB_MIN_7POINT1_REAR_DELAY: BYTE = 0;
pub const REVERB_MIN_POSITION: BYTE = 0;
pub const REVERB_MIN_DIFFUSION: BYTE = 0;
pub const REVERB_MIN_LOW_EQ_GAIN: BYTE = 0;
pub const REVERB_MIN_LOW_EQ_CUTOFF: BYTE = 0;
pub const REVERB_MIN_HIGH_EQ_GAIN: BYTE = 0;
pub const REVERB_MIN_HIGH_EQ_CUTOFF: BYTE = 0;
pub const REVERB_MIN_ROOM_FILTER_FREQ: f32 = 20.0;
pub const REVERB_MIN_ROOM_FILTER_MAIN: f32 = -100.0;
pub const REVERB_MIN_ROOM_FILTER_HF: f32 = -100.0;
pub const REVERB_MIN_REFLECTIONS_GAIN: f32 = -100.0;
pub const REVERB_MIN_REVERB_GAIN: f32 = -100.0;
pub const REVERB_MIN_DECAY_TIME: f32 = 0.1;
pub const REVERB_MIN_DENSITY: f32 = 0.0;
pub const REVERB_MIN_ROOM_SIZE: f32 = 0.0;

pub const REVERB_MAX_WET_DRY_MIX: f32 = 100.0;
pub const REVERB_MAX_REFLECTIONS_DELAY: UINT32 = 300;
pub const REVERB_MAX_REVERB_DELAY: BYTE = 85;
pub const REVERB_MAX_REAR_DELAY: BYTE = 5;
pub const REVERB_MAX_7POINT1_SIDE_DELAY: BYTE = 5;
pub const REVERB_MAX_7POINT1_REAR_DELAY: BYTE = 20;
pub const REVERB_MAX_POSITION: BYTE = 30;
pub const REVERB_MAX_DIFFUSION: BYTE = 15;
pub const REVERB_MAX_LOW_EQ_GAIN: BYTE = 12;
pub const REVERB_MAX_LOW_EQ_CUTOFF: BYTE = 9;
pub const REVERB_MAX_HIGH_EQ_GAIN: BYTE = 8;
pub const REVERB_MAX_HIGH_EQ_CUTOFF: BYTE = 14;
pub const REVERB_MAX_ROOM_FILTER_FREQ: f32 = 20000.0;
pub const REVERB_MAX_ROOM_FILTER_MAIN: f32 = 0.0;
pub const REVERB_MAX_ROOM_FILTER_HF: f32 = 0.0;
pub const REVERB_MAX_REFLECTIONS_GAIN: f32 = 20.0;
pub const REVERB_MAX_REVERB_GAIN: f32 = 20.0;
pub const REVERB_MAX_DENSITY: f32 = 100.0;
pub const REVERB_MAX_ROOM_SIZE: f32 = 100.0;

pub const REVERB_DEFAULT_WET_DRY_MIX: f32 = 100.0;
pub const REVERB_DEFAULT_REFLECTIONS_DELAY: UINT32 = 5;
pub const REVERB_DEFAULT_REVERB_DELAY: BYTE = 5;
pub const REVERB_DEFAULT_REAR_DELAY: BYTE = 5;
pub const REVERB_DEFAULT_7POINT1_SIDE_DELAY: BYTE = 5;
pub const REVERB_DEFAULT_7POINT1_REAR_DELAY: BYTE = 20;
pub const REVERB_DEFAULT_POSITION: BYTE = 6;
pub const REVERB_DEFAULT_POSITION_MATRIX: BYTE = 27;
pub const REVERB_DEFAULT_EARLY_DIFFUSION: BYTE = 8;
pub const REVERB_DEFAULT_LATE_DIFFUSION: BYTE = 8;
pub const REVERB_DEFAULT_LOW_EQ_GAIN: BYTE = 8;
pub const REVERB_DEFAULT_LOW_EQ_CUTOFF: BYTE = 4;
pub const REVERB_DEFAULT_HIGH_EQ_GAIN: BYTE = 8;
pub const REVERB_DEFAULT_HIGH_EQ_CUTOFF: BYTE = 4;
pub const REVERB_DEFAULT_ROOM_FILTER_FREQ: f32 = 5000.0;
pub const REVERB_DEFAULT_ROOM_FILTER_MAIN: f32 = 0.0;
pub const REVERB_DEFAULT_ROOM_FILTER_HF: f32 = 0.0;
pub const REVERB_DEFAULT_REFLECTIONS_GAIN: f32 = 0.0;
pub const REVERB_DEFAULT_REVERB_GAIN: f32 = 0.0;
pub const REVERB_DEFAULT_DECAY_TIME: f32 = 1.0;
pub const REVERB_DEFAULT_DENSITY: f32 = 100.0;
pub const REVERB_DEFAULT_ROOM_SIZE: f32 = 100.0;
pub const REVERB_DEFAULT_DISABLE_LATE_FIELD: BOOL = FALSE;

pub fn createVolumeMeter(apo: *?*IUnknown, _: UINT32) HRESULT {
    var xaudio2_dll = windows.kernel32.GetModuleHandleW(L("xaudio2_9redist.dll"));
    if (xaudio2_dll == null) {
        xaudio2_dll = (std.DynLib.openZ("d3d12/xaudio2_9redist.dll") catch unreachable).dll;
    }

    var CreateAudioVolumeMeter: fn (*?*IUnknown) callconv(WINAPI) HRESULT = undefined;
    CreateAudioVolumeMeter = @ptrCast(
        @TypeOf(CreateAudioVolumeMeter),
        windows.kernel32.GetProcAddress(xaudio2_dll.?, "CreateAudioVolumeMeter").?,
    );

    return CreateAudioVolumeMeter(apo);
}

pub fn createReverb(apo: *?*IUnknown, _: UINT32) HRESULT {
    var xaudio2_dll = windows.kernel32.GetModuleHandleW(L("xaudio2_9redist.dll"));
    if (xaudio2_dll == null) {
        xaudio2_dll = (std.DynLib.openZ("d3d12/xaudio2_9redist.dll") catch unreachable).dll;
    }

    var CreateAudioReverb: fn (*?*IUnknown) callconv(WINAPI) HRESULT = undefined;
    CreateAudioReverb = @ptrCast(
        @TypeOf(CreateAudioReverb),
        windows.kernel32.GetProcAddress(xaudio2_dll.?, "CreateAudioReverb").?,
    );

    return CreateAudioReverb(apo);
}
