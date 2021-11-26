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
        return extern struct {
            pub inline fn RegisterForCallbacks(self: *T, cb: *IEngineCallback) HRESULT {
                return self.v.xaudio2.RegisterForCallbacks(self, cb);
            }
            pub inline fn UnregisterForCallbacks(self: *T, cb: *IEngineCallback) void {
                self.v.xaudio2.UnregisterForCallbacks(self, cb);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            RegisterForCallbacks: fn (*T, *IEngineCallback) callconv(WINAPI) HRESULT,
            UnregisterForCallbacks: fn (*T, *IEngineCallback) callconv(WINAPI) void,
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

pub const IEngineCallback = extern struct {
    const Self = @This();
    v: *const extern struct {
        ecb: VTable(Self),
    },
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn OnProcessingPassStart(self: *T) void {
                self.v.ecb.OnProcessingPassStart(self);
            }
            pub inline fn OnProcessingPassEnd(self: *T) void {
                self.v.ecb.OnProcessingPassEnd(self);
            }
            pub inline fn OnCriticalError(self: *T, err: HRESULT) void {
                self.v.ecb.OnCriticalError(self, err);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            OnProcessingPassStart: fn (*T) callconv(WINAPI) void,
            OnProcessingPassEnd: fn (*T) callconv(WINAPI) void,
            OnCriticalError: fn (*T, HRESULT) callconv(WINAPI) void,
        };
    }
};

pub const IVoiceCallback = extern struct {
    const Self = @This();
    v: *const extern struct {
        vcb: VTable(Self),
    },
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn OnVoiceProcessingPassStart(self: *T, bytes_required: UINT32) void {
                self.v.vcb.OnVoiceProcessingPassStart(self, bytes_required);
            }
            pub inline fn OnVoiceProcessingPassEnd(self: *T) void {
                self.v.vcb.OnVoiceProcessingPassEnd(self);
            }
            pub inline fn OnStreamEnd(self: *T) void {
                self.v.vcb.OnStreamEnd(self);
            }
            pub inline fn OnBufferStart(self: *T, context: *c_void) void {
                self.v.vcb.OnBufferStart(self, context);
            }
            pub inline fn OnBufferEnd(self: *T, context: *c_void) void {
                self.v.vcb.OnBufferEnd(self, context);
            }
            pub inline fn OnLoopEnd(self: *T, context: *c_void) void {
                self.v.vcb.OnLoopEnd(self, context);
            }
            pub inline fn OnVoiceError(self: *T, context: *c_void, err: HRESULT) void {
                self.v.vcb.OnVoiceError(self, context, err);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            OnVoiceProcessingPassStart: fn (*T, UINT32) callconv(WINAPI) void,
            OnVoiceProcessingPassEnd: fn (*T) callconv(WINAPI) void,
            OnStreamEnd: fn (*T) callconv(WINAPI) void,
            OnBufferStart: fn (*T, *c_void) callconv(WINAPI) void,
            OnBufferEnd: fn (*T, *c_void) callconv(WINAPI) void,
            OnLoopEnd: fn (*T, *c_void) callconv(WINAPI) void,
            OnVoiceError: fn (*T, *c_void, HRESULT) callconv(WINAPI) void,
        };
    }
};

pub fn create(
    ppv: *?*IXAudio2,
    flags: UINT32, // 0
    processor: UINT32, // 0
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
