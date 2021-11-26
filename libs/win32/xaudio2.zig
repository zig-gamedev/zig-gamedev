const std = @import("std");
const windows = @import("windows.zig");
const IUnknown = windows.IUnknown;
const UINT = windows.UINT;
const UINT32 = windows.UINT32;
const WINAPI = windows.WINAPI;
const BOOL = windows.BOOL;
const GUID = windows.GUID;
const HRESULT = windows.HRESULT;
const WAVEFORMATEX = @import("wasapi.zig").WAVEFORMATEX;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

pub const COMMIT_NOW: UINT32 = 0;
pub const COMMIT_ALL: UINT32 = 0;
pub const INVALID_OPSET: UINT32 = 0xffff_ffff;
pub const NO_LOOP_REGION: UINT32 = 0;
pub const LOOP_INFINITE: UINT32 = 255;
pub const DEFAULT_CHANNELS: UINT32 = 0;
pub const DEFAULT_SAMPLERATE: UINT32 = 0;

pub const VOICE_DETAILS = struct {
    CreationFlags: UINT32,
    ActiveFlags: UINT32,
    InputChannels: UINT32,
    InputSampleRate: UINT32,
};

pub const SEND_DESCRIPTOR = struct {
    Flags: UINT32,
    pOutputVoice: *IVoice,
};

pub const VOICE_SENDS = struct {
    SendCount: UINT32,
    pSends: [*]SEND_DESCRIPTOR,
};

pub const EFFECT_DESCRIPTOR = struct {
    pEffect: *IUnknown,
    InitialState: BOOL,
    OutputChannels: UINT32,
};

pub const EFFECT_CHAIN = struct {
    EffectCount: UINT32,
    pEffectDescriptors: [*]EFFECT_DESCRIPTOR,
};

pub const FILTER_TYPE = enum(UINT32) {
    LowPassFilter,
    BandPassFilter,
    HighPassFilter,
    NotchFilter,
    LowPassOnePoleFilter,
    HighPassOnePoleFilter,
};

pub const FILTER_PARAMETERS = struct {
    Type: FILTER_TYPE,
    Frequency: f32,
    OneOverQ: f32,
};

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

pub const IVoice = extern struct {
    const Self = @This();
    v: *const extern struct {
        voice: VTable(Self),
    },
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetVoiceDetails(self: *T, details: *VOICE_DETAILS) void {
                self.v.voice.GetVoiceDetails(self, details);
            }
            pub inline fn SetOutputVoices(self: *T, send_list: ?*const VOICE_SENDS) HRESULT {
                return self.v.voice.SetOutputVoices(self, send_list);
            }
            pub inline fn SetEffectChain(self: *T, effect_chain: ?*const EFFECT_CHAIN) HRESULT {
                return self.v.voice.SetEffectChain(self, effect_chain);
            }
            pub inline fn EnableEffect(self: *T, effect_index: UINT32, operation_set: UINT32) HRESULT {
                return self.v.voice.EnableEffect(self, effect_index, operation_set);
            }
            pub inline fn DisableEffect(self: *T, effect_index: UINT32, operation_set: UINT32) HRESULT {
                return self.v.voice.DisableEffect(self, effect_index, operation_set);
            }
            pub inline fn GetEffectState(self: *T, effect_index: UINT32, enabled: *BOOL) void {
                self.v.voice.GetEffectState(self, effect_index, enabled);
            }
            pub inline fn SetEffectParameters(
                self: *T,
                effect_index: UINT32,
                params: *const c_void,
                params_size: UINT32,
                operation_set: UINT32,
            ) HRESULT {
                return self.v.voice.SetEffectParameters(self, effect_index, params, params_size, operation_set);
            }
            pub inline fn GetEffectParameters(
                self: *T,
                effect_index: UINT32,
                params: *c_void,
                params_size: UINT32,
            ) HRESULT {
                self.v.voice.GetEffectParameters(self, effect_index, params, params_size);
            }
            pub inline fn SetFilterParameters(
                self: *T,
                params: *const FILTER_PARAMETERS,
                operation_set: UINT32,
            ) HRESULT {
                return self.v.voice.SetFilterParameters(self, params, operation_set);
            }
            pub inline fn GetFilterParameters(self: *T, params: *FILTER_PARAMETERS) void {
                self.v.voice.GetFilterParameters(self, params);
            }
            pub inline fn SetOutputFilterParameters(
                self: *T,
                dst_voice: ?*IVoice,
                params: *const FILTER_PARAMETERS,
                operation_set: UINT32,
            ) HRESULT {
                return self.v.voice.SetOutputFilterParameters(self, dst_voice, params, operation_set);
            }
            pub inline fn GetOutputFilterParameters(self: *T, dst_voice: ?*IVoice, params: *FILTER_PARAMETERS) void {
                self.v.voice.GetOutputFilterParameters(self, dst_voice, params);
            }
            pub inline fn SetVolume(self: *T, volume: f32) HRESULT {
                return self.v.voice.SetVolume(self, volume);
            }
            pub inline fn GetVolume(self: *T, volume: *f32) void {
                self.v.voice.GetVolume(self, volume);
            }
            pub inline fn SetChannelVolumes(
                self: *T,
                num_channels: UINT32,
                volumes: [*]const f32,
                operation_set: UINT32,
            ) HRESULT {
                return self.v.voice.SetChannelVolumes(self, num_channels, volumes, operation_set);
            }
            pub inline fn GetChannelVolumes(self: *T, num_channels: UINT32, volumes: [*]f32) void {
                self.v.voice.GetChannelVolumes(self, num_channels, volumes);
            }
            pub inline fn DestroyVoice(self: *T) void {
                self.v.voice.DestroyVoice(self);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetVoiceDetails: fn (*T, *VOICE_DETAILS) callconv(WINAPI) void,
            SetOutputVoices: fn (*T, ?*const VOICE_SENDS) callconv(WINAPI) HRESULT,
            SetEffectChain: fn (*T, ?*const EFFECT_CHAIN) callconv(WINAPI) HRESULT,
            EnableEffect: fn (*T, UINT32, UINT32) callconv(WINAPI) HRESULT,
            DisableEffect: fn (*T, UINT32, UINT32) callconv(WINAPI) HRESULT,
            GetEffectState: fn (*T, UINT32, *BOOL) callconv(WINAPI) void,
            SetEffectParameters: fn (*T, UINT32, *const c_void, UINT32, UINT32) callconv(WINAPI) HRESULT,
            GetEffectParameters: fn (*T, UINT32, *c_void, UINT32) callconv(WINAPI) HRESULT,
            SetFilterParameters: fn (*T, *const FILTER_PARAMETERS, UINT32) callconv(WINAPI) HRESULT,
            GetFilterParameters: fn (*T, *FILTER_PARAMETERS) callconv(WINAPI) void,
            SetOutputFilterParameters: fn (*T, ?*IVoice, *const FILTER_PARAMETERS, UINT32) callconv(WINAPI) HRESULT,
            GetOutputFilterParameters: fn (*T, ?*IVoice, *FILTER_PARAMETERS) callconv(WINAPI) void,
            SetVolume: fn (*T, f32) callconv(WINAPI) HRESULT,
            GetVolume: fn (*T, *f32) callconv(WINAPI) void,
            SetChannelVolumes: fn (*T, UINT32, [*]const f32, UINT32) callconv(WINAPI) HRESULT,
            GetChannelVolumes: fn (*T, UINT32, [*]f32) callconv(WINAPI) void,
            SetOutputMatrix: *c_void,
            GetOutputMatrix: *c_void,
            DestroyVoice: fn (*T) callconv(WINAPI) void,
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
