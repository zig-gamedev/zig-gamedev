const std = @import("std");
const windows = @import("windows.zig");
const IUnknown = windows.IUnknown;
const BYTE = windows.BYTE;
const UINT = windows.UINT;
const UINT32 = windows.UINT32;
const UINT64 = windows.UINT64;
const WINAPI = windows.WINAPI;
const LPCWSTR = windows.LPCWSTR;
const BOOL = windows.BOOL;
const DWORD = windows.DWORD;
const GUID = windows.GUID;
const HRESULT = windows.HRESULT;
const WAVEFORMATEX = @import("wasapi.zig").WAVEFORMATEX;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

// NOTE(mziulek):
// xaudio2redist.h uses tight field packing so we need to use 'packed struct' instead of 'extern struct' in
// all non-interface structure definitions.

pub const COMMIT_NOW: UINT32 = 0;
pub const COMMIT_ALL: UINT32 = 0;
pub const INVALID_OPSET: UINT32 = 0xffff_ffff;
pub const NO_LOOP_REGION: UINT32 = 0;
pub const LOOP_INFINITE: UINT32 = 255;
pub const DEFAULT_CHANNELS: UINT32 = 0;
pub const DEFAULT_SAMPLERATE: UINT32 = 0;

pub const MAX_BUFFER_BYTES: UINT32 = 0x8000_0000;
pub const MAX_QUEUED_BUFFERS: UINT32 = 64;
pub const MAX_BUFFERS_SYSTEM: UINT32 = 2;
pub const MAX_AUDIO_CHANNELS: UINT32 = 64;
pub const MIN_SAMPLE_RATE: UINT32 = 1000;
pub const MAX_SAMPLE_RATE: UINT32 = 200000;
pub const MAX_VOLUME_LEVEL: f32 = 16777216.0;
pub const MIN_FREQ_RATIO: f32 = 1.0 / 1024.0;
pub const MAX_FREQ_RATIO: f32 = 1024.0;
pub const DEFAULT_FREQ_RATIO: f32 = 2.0;
pub const MAX_FILTER_ONEOVERQ: f32 = 1.5;
pub const MAX_FILTER_FREQUENCY: f32 = 1.0;
pub const MAX_LOOP_COUNT: UINT32 = 254;
pub const MAX_INSTANCES: UINT32 = 8;

pub const DEBUG_ENGINE: UINT32 = 0x0001;
pub const VOICE_NOPITCH: UINT32 = 0x0002;
pub const VOICE_NOSRC: UINT32 = 0x0004;
pub const VOICE_USEFILTER: UINT32 = 0x0008;
pub const PLAY_TAILS: UINT32 = 0x0020;
pub const END_OF_STREAM: UINT32 = 0x0040;
pub const SEND_USEFILTER: UINT32 = 0x0080;
pub const VOICE_NOSAMPLESPLAYED: UINT32 = 0x0100;
pub const STOP_ENGINE_WHEN_IDLE: UINT32 = 0x2000;
pub const NO_VIRTUAL_AUDIO_CLIENT: UINT32 = 0x10000;

pub const VOICE_DETAILS = packed struct {
    CreationFlags: UINT32,
    ActiveFlags: UINT32,
    InputChannels: UINT32,
    InputSampleRate: UINT32,
};

pub const SEND_DESCRIPTOR = packed struct {
    Flags: UINT32,
    pOutputVoice: *IVoice,
};

pub const VOICE_SENDS = packed struct {
    SendCount: UINT32,
    pSends: [*]SEND_DESCRIPTOR,
};

pub const EFFECT_DESCRIPTOR = packed struct {
    pEffect: *IUnknown,
    InitialState: BOOL,
    OutputChannels: UINT32,
};

pub const EFFECT_CHAIN = packed struct {
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

pub const AUDIO_STREAM_CATEGORY = enum(UINT32) {
    Other = 0,
    ForegroundOnlyMedia = 1,
    Communications = 3,
    Alerts = 4,
    SoundEffects = 5,
    GameEffects = 6,
    GameMedia = 7,
    GameChat = 8,
    Speech = 9,
    Movie = 10,
    Media = 11,
};

pub const FILTER_PARAMETERS = packed struct {
    Type: FILTER_TYPE,
    Frequency: f32,
    OneOverQ: f32,
};

pub const BUFFER = packed struct {
    Flags: UINT32,
    AudioBytes: UINT32,
    pAudioData: [*]const BYTE,
    PlayBegin: UINT32,
    PlayLength: UINT32,
    LoopBegin: UINT32,
    LoopLength: UINT32,
    LoopCount: UINT32,
    pContext: ?*anyopaque,
};

pub const BUFFER_WMA = packed struct {
    pDecodedPacketCumulativeBytes: *const UINT32,
    PacketCount: UINT32,
};

pub const VOICE_STATE = packed struct {
    pCurrentBufferContext: ?*anyopaque,
    BuffersQueued: UINT32,
    SamplesPlayed: UINT64,
};

pub const PERFORMANCE_DATA = packed struct {
    AudioCyclesSinceLastQuery: UINT64,
    TotalCyclesSinceLastQuery: UINT64,
    MinimumCyclesPerQuantum: UINT32,
    MaximumCyclesPerQuantum: UINT32,
    MemoryUsageInBytes: UINT32,
    CurrentLatencyInSamples: UINT32,
    GlitchesSinceEngineStarted: UINT32,
    ActiveSourceVoiceCount: UINT32,
    TotalSourceVoiceCount: UINT32,
    ActiveSubmixVoiceCount: UINT32,
    ActiveResamplerCount: UINT32,
    ActiveMatrixMixCount: UINT32,
    ActiveXmaSourceVoices: UINT32,
    ActiveXmaStreams: UINT32,
};

pub const LOG_ERRORS: UINT = 0x0001;
pub const LOG_WARNINGS: UINT = 0x0002;
pub const LOG_INFO: UINT = 0x0004;
pub const LOG_DETAIL: UINT = 0x0008;
pub const LOG_API_CALLS: UINT = 0x0010;
pub const LOG_FUNC_CALLS: UINT = 0x0020;
pub const LOG_TIMING: UINT = 0x0040;
pub const LOG_LOCKS: UINT = 0x0080;
pub const LOG_MEMORY: UINT = 0x0100;
pub const LOG_STREAMING: UINT = 0x1000;

pub const DEBUG_CONFIGURATION = packed struct {
    TraceMask: UINT32,
    BreakMask: UINT32,
    LogThreadID: BOOL,
    LogFileline: BOOL,
    LogFunctionName: BOOL,
    LogTiming: BOOL,
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
            pub inline fn CreateSourceVoice(
                self: *T,
                source_voice: *?*ISourceVoice,
                source_format: *const WAVEFORMATEX,
                flags: UINT32,
                max_frequency_ratio: f32,
                callback: ?*IVoiceCallback,
                send_list: ?*const VOICE_SENDS,
                effect_chain: ?*const EFFECT_CHAIN,
            ) HRESULT {
                return self.v.xaudio2.CreateSourceVoice(
                    self,
                    source_voice,
                    source_format,
                    flags,
                    max_frequency_ratio,
                    callback,
                    send_list,
                    effect_chain,
                );
            }
            pub inline fn CreateSubmixVoice(
                self: *T,
                submix_voice: *?*ISubmixVoice,
                input_channels: UINT32,
                input_sample_rate: UINT32,
                flags: UINT32,
                processing_stage: UINT32,
                send_list: ?*const VOICE_SENDS,
                effect_chain: ?*const EFFECT_CHAIN,
            ) HRESULT {
                return self.v.xaudio2.CreateSubmixVoice(
                    self,
                    submix_voice,
                    input_channels,
                    input_sample_rate,
                    flags,
                    processing_stage,
                    send_list,
                    effect_chain,
                );
            }
            pub inline fn CreateMasteringVoice(
                self: *T,
                mastering_voice: *?*IMasteringVoice,
                input_channels: UINT32,
                input_sample_rate: UINT32,
                flags: UINT32,
                device_id: ?LPCWSTR,
                effect_chain: ?*const EFFECT_CHAIN,
                stream_category: AUDIO_STREAM_CATEGORY,
            ) HRESULT {
                return self.v.xaudio2.CreateMasteringVoice(
                    self,
                    mastering_voice,
                    input_channels,
                    input_sample_rate,
                    flags,
                    device_id,
                    effect_chain,
                    stream_category,
                );
            }
            pub inline fn StartEngine(self: *T) HRESULT {
                return self.v.xaudio2.StartEngine(self);
            }
            pub inline fn StopEngine(self: *T) void {
                self.v.xaudio2.StopEngine(self);
            }
            pub inline fn CommitChanges(self: *T, operation_set: UINT32) HRESULT {
                return self.v.xaudio2.CommitChanges(self, operation_set);
            }
            pub inline fn GetPerformanceData(self: *T, data: *PERFORMANCE_DATA) void {
                self.v.xaudio2.GetPerformanceData(self, data);
            }
            pub inline fn SetDebugConfiguration(
                self: *T,
                config: ?*const DEBUG_CONFIGURATION,
                reserved: ?*anyopaque,
            ) void {
                self.v.xaudio2.SetDebugConfiguration(self, config, reserved);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            RegisterForCallbacks: fn (*T, *IEngineCallback) callconv(WINAPI) HRESULT,
            UnregisterForCallbacks: fn (*T, *IEngineCallback) callconv(WINAPI) void,
            CreateSourceVoice: fn (
                *T,
                *?*ISourceVoice,
                *const WAVEFORMATEX,
                UINT32,
                f32,
                ?*IVoiceCallback,
                ?*const VOICE_SENDS,
                ?*const EFFECT_CHAIN,
            ) callconv(WINAPI) HRESULT,
            CreateSubmixVoice: fn (
                *T,
                *?*ISubmixVoice,
                UINT32,
                UINT32,
                UINT32,
                UINT32,
                ?*const VOICE_SENDS,
                ?*const EFFECT_CHAIN,
            ) callconv(WINAPI) HRESULT,
            CreateMasteringVoice: fn (
                *T,
                *?*IMasteringVoice,
                UINT32,
                UINT32,
                UINT32,
                ?LPCWSTR,
                ?*const EFFECT_CHAIN,
                AUDIO_STREAM_CATEGORY,
            ) callconv(WINAPI) HRESULT,
            StartEngine: fn (*T) callconv(WINAPI) HRESULT,
            StopEngine: fn (*T) callconv(WINAPI) void,
            CommitChanges: fn (*T, UINT32) callconv(WINAPI) HRESULT,
            GetPerformanceData: fn (*T, *PERFORMANCE_DATA) callconv(WINAPI) void,
            SetDebugConfiguration: fn (*T, ?*const DEBUG_CONFIGURATION, ?*anyopaque) callconv(WINAPI) void,
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
                params: *const anyopaque,
                params_size: UINT32,
                operation_set: UINT32,
            ) HRESULT {
                return self.v.voice.SetEffectParameters(self, effect_index, params, params_size, operation_set);
            }
            pub inline fn GetEffectParameters(
                self: *T,
                effect_index: UINT32,
                params: *anyopaque,
                params_size: UINT32,
            ) HRESULT {
                return self.v.voice.GetEffectParameters(self, effect_index, params, params_size);
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
            SetEffectParameters: fn (*T, UINT32, *const anyopaque, UINT32, UINT32) callconv(WINAPI) HRESULT,
            GetEffectParameters: fn (*T, UINT32, *anyopaque, UINT32) callconv(WINAPI) HRESULT,
            SetFilterParameters: fn (*T, *const FILTER_PARAMETERS, UINT32) callconv(WINAPI) HRESULT,
            GetFilterParameters: fn (*T, *FILTER_PARAMETERS) callconv(WINAPI) void,
            SetOutputFilterParameters: fn (*T, ?*IVoice, *const FILTER_PARAMETERS, UINT32) callconv(WINAPI) HRESULT,
            GetOutputFilterParameters: fn (*T, ?*IVoice, *FILTER_PARAMETERS) callconv(WINAPI) void,
            SetVolume: fn (*T, f32) callconv(WINAPI) HRESULT,
            GetVolume: fn (*T, *f32) callconv(WINAPI) void,
            SetChannelVolumes: fn (*T, UINT32, [*]const f32, UINT32) callconv(WINAPI) HRESULT,
            GetChannelVolumes: fn (*T, UINT32, [*]f32) callconv(WINAPI) void,
            SetOutputMatrix: *anyopaque,
            GetOutputMatrix: *anyopaque,
            DestroyVoice: fn (*T) callconv(WINAPI) void,
        };
    }
};

pub const ISourceVoice = extern struct {
    const Self = @This();
    v: *const extern struct {
        voice: IVoice.VTable(Self),
        srcvoice: VTable(Self),
    },
    usingnamespace IVoice.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Start(self: *T, flags: UINT32, operation_set: UINT32) HRESULT {
                return self.v.srcvoice.Start(self, flags, operation_set);
            }
            pub inline fn Stop(self: *T, flags: UINT32, operation_set: UINT32) HRESULT {
                return self.v.srcvoice.Stop(self, flags, operation_set);
            }
            pub inline fn SubmitSourceBuffer(self: *T, buffer: *const BUFFER, wmabuffer: ?*const BUFFER_WMA) HRESULT {
                return self.v.srcvoice.SubmitSourceBuffer(self, buffer, wmabuffer);
            }
            pub inline fn FlushSourceBuffers(self: *T) HRESULT {
                return self.v.srcvoice.FlushSourceBuffers(self);
            }
            pub inline fn Discontinuity(self: *T) HRESULT {
                return self.v.srcvoice.Discontinuity(self);
            }
            pub inline fn ExitLoop(self: *T, operation_set: UINT32) HRESULT {
                return self.v.srcvoice.ExitLoop(self, operation_set);
            }
            pub inline fn GetState(self: *T, state: *VOICE_STATE, flags: UINT32) void {
                self.v.srcvoice.GetState(self, state, flags);
            }
            pub inline fn SetFrequencyRatio(self: *T, ratio: f32, operation_set: UINT32) HRESULT {
                return self.v.srcvoice.SetFrequencyRatio(self, ratio, operation_set);
            }
            pub inline fn GetFrequencyRatio(self: *T, ratio: *f32) void {
                self.v.srcvoice.GetFrequencyRatio(self, ratio);
            }
            pub inline fn SetSourceSampleRate(self: *T, sample_rate: UINT32) HRESULT {
                return self.v.srcvoice.SetSourceSampleRate(self, sample_rate);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            Start: fn (*T, UINT32, UINT32) callconv(WINAPI) HRESULT,
            Stop: fn (*T, UINT32, UINT32) callconv(WINAPI) HRESULT,
            SubmitSourceBuffer: fn (*T, *const BUFFER, ?*const BUFFER_WMA) callconv(WINAPI) HRESULT,
            FlushSourceBuffers: fn (*T) callconv(WINAPI) HRESULT,
            Discontinuity: fn (*T) callconv(WINAPI) HRESULT,
            ExitLoop: fn (*T, UINT32) callconv(WINAPI) HRESULT,
            GetState: fn (*T, *VOICE_STATE, UINT32) callconv(WINAPI) void,
            SetFrequencyRatio: fn (*T, f32, UINT32) callconv(WINAPI) HRESULT,
            GetFrequencyRatio: fn (*T, *f32) callconv(WINAPI) void,
            SetSourceSampleRate: fn (*T, UINT32) callconv(WINAPI) HRESULT,
        };
    }
};

pub const ISubmixVoice = extern struct {
    const Self = @This();
    v: *const extern struct {
        voice: IVoice.VTable(Self),
        submixvoice: VTable(Self),
    },
    usingnamespace IVoice.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {};
    }
};

pub const IMasteringVoice = extern struct {
    const Self = @This();
    v: *const extern struct {
        voice: IVoice.VTable(Self),
        mastervoice: VTable(Self),
    },
    usingnamespace IVoice.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetChannelMask(self: *T, channel_mask: *DWORD) HRESULT {
                return self.v.mastervoice.GetChannelMask(self, channel_mask);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetChannelMask: fn (*T, *DWORD) callconv(WINAPI) HRESULT,
        };
    }
};

pub fn IEngineCallbackVTable(comptime T: type) type {
    return extern struct {
        ecb: extern struct {
            OnProcessingPassStart: fn (*T) callconv(WINAPI) void,
            OnProcessingPassEnd: fn (*T) callconv(WINAPI) void,
            OnCriticalError: fn (*T, HRESULT) callconv(WINAPI) void,
        },
    };
}

pub const IEngineCallback = extern struct {
    v: *const IEngineCallbackVTable(Self),

    const Self = @This();
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
};

pub fn IVoiceCallbackVTable(comptime T: type) type {
    return extern struct {
        vcb: extern struct {
            OnVoiceProcessingPassStart: fn (*T, UINT32) callconv(WINAPI) void,
            OnVoiceProcessingPassEnd: fn (*T) callconv(WINAPI) void,
            OnStreamEnd: fn (*T) callconv(WINAPI) void,
            OnBufferStart: fn (*T, ?*anyopaque) callconv(WINAPI) void,
            OnBufferEnd: fn (*T, ?*anyopaque) callconv(WINAPI) void,
            OnLoopEnd: fn (*T, ?*anyopaque) callconv(WINAPI) void,
            OnVoiceError: fn (*T, ?*anyopaque, HRESULT) callconv(WINAPI) void,
        },
    };
}

pub const IVoiceCallback = extern struct {
    v: *const IVoiceCallbackVTable(Self),

    const Self = @This();
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
            pub inline fn OnBufferStart(self: *T, context: ?*anyopaque) void {
                self.v.vcb.OnBufferStart(self, context);
            }
            pub inline fn OnBufferEnd(self: *T, context: ?*anyopaque) void {
                self.v.vcb.OnBufferEnd(self, context);
            }
            pub inline fn OnLoopEnd(self: *T, context: ?*anyopaque) void {
                self.v.vcb.OnLoopEnd(self, context);
            }
            pub inline fn OnVoiceError(self: *T, context: ?*anyopaque, err: HRESULT) void {
                self.v.vcb.OnVoiceError(self, context, err);
            }
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
        xaudio2_dll = (std.DynLib.openZ("xaudio2_9redist.dll") catch unreachable).dll;
    }

    var XAudio2Create: fn (*?*IXAudio2, UINT32, UINT32) callconv(WINAPI) HRESULT = undefined;
    XAudio2Create = @ptrCast(
        @TypeOf(XAudio2Create),
        windows.kernel32.GetProcAddress(xaudio2_dll.?, "XAudio2Create").?,
    );

    return XAudio2Create(ppv, flags, processor);
}
