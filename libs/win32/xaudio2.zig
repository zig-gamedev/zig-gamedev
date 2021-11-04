const std = @import("std");
const windows = @import("windows.zig");
const IUnknown = windows.IUnknown;
const UINT = windows.UINT;
const UINT32 = windows.UINT32;
const WINAPI = windows.WINAPI;
const GUID = windows.GUID;
const HRESULT = windows.HRESULT;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

pub const IXAudio2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        xaudio2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            RegisterForCallbacks: *c_void,
            UnregisterForCallbacks: *c_void,
            CreateSourceVoice: *c_void,
            CreateSubmixVoice: *c_void,
            CreateMasteringVoice: *c_void,
            StartEngine: *c_void,
            StopEngine: *c_void,
            CommitChanges: *c_void,
            GetPerformanceData: *c_void,
            SetDebugConfiguration: *c_void,
        };
    }
};

pub fn create(
    ppv: *?*IXAudio2,
    flags: UINT32,
    processor: UINT32,
) HRESULT {
    var xaudio2_dll = windows.kernel32.GetModuleHandleW(L("xaudio2_9redist.dll"));
    if (xaudio2_dll == null) {
        xaudio2_dll = (std.DynLib.openZ("d3d12/xaudio2_9redist.dll") catch unreachable).dll;
    }

    var XAudio2Create: fn (*?*IXAudio2, UINT32, UINT32) callconv(WINAPI) HRESULT = undefined;
    XAudio2Create = @ptrCast(
        @TypeOf(XAudio2Create),
        windows.kernel32.GetProcAddress(xaudio2_dll.?, "XAudio2Create").?,
    );

    return XAudio2Create(ppv, flags, processor);
}
