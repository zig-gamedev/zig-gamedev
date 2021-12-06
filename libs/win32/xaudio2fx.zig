const std = @import("std");
const windows = @import("windows.zig");
const IUnknown = windows.IUnknown;
const BYTE = windows.BYTE;
const HRESULT = windows.HRESULT;
const WINAPI = windows.WINAPI;
const UINT32 = windows.UINT32;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

pub const VOLUMEMETER_LEVELS = packed struct {
    pPeakLevels: ?[*]f32,
    pRMSLevels: ?[*]f32,
    ChannelCount: UINT32,
};

pub const REVERB_MIN_FRAMERATE: UINT32 = 20000;
pub const REVERB_MAX_FRAMERATE: UINT32 = 48000;

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
