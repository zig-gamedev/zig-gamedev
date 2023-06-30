const std = @import("std");
const w32 = @import("w32.zig");
const IUnknown = w32.IUnknown;
const BYTE = w32.BYTE;
const UINT = w32.UINT;
const UINT32 = w32.UINT32;
const UINT64 = w32.UINT64;
const WINAPI = w32.WINAPI;
const LPCWSTR = w32.LPCWSTR;
const BOOL = w32.BOOL;
const DWORD = w32.DWORD;
const GUID = w32.GUID;
const HRESULT = w32.HRESULT;
const WAVEFORMATEX = @import("wasapi.zig").WAVEFORMATEX;

// NOTE(mziulek):
// xaudio2redist.h uses tight field packing so we need align each field with `align(1)`
// in all non-interface structure definitions.

pub const COMMIT_NOW = 0;
pub const COMMIT_ALL = 0;
pub const INVALID_OPSET = 0xffff_ffff;
pub const NO_LOOP_REGION = 0;
pub const LOOP_INFINITE = 255;
pub const DEFAULT_CHANNELS = 0;
pub const DEFAULT_SAMPLERATE = 0;

pub const MAX_BUFFER_BYTES = 0x8000_0000;
pub const MAX_QUEUED_BUFFERS = 64;
pub const MAX_BUFFERS_SYSTEM = 2;
pub const MAX_AUDIO_CHANNELS = 64;
pub const MIN_SAMPLE_RATE = 1000;
pub const MAX_SAMPLE_RATE = 200000;
pub const MAX_VOLUME_LEVEL = 16777216.0;
pub const MIN_FREQ_RATIO = 1.0 / 1024.0;
pub const MAX_FREQ_RATIO = 1024.0;
pub const DEFAULT_FREQ_RATIO = 2.0;
pub const MAX_FILTER_ONEOVERQ = 1.5;
pub const MAX_FILTER_FREQUENCY = 1.0;
pub const MAX_LOOP_COUNT = 254;
pub const MAX_INSTANCES = 8;

pub const FLAGS = packed struct(UINT32) {
    DEBUG_ENGINE: bool = false,
    VOICE_NOPITCH: bool = false,
    VOICE_NOSRC: bool = false,
    VOICE_USEFILTER: bool = false,
    __unused4: bool = false,
    PLAY_TAILS: bool = false,
    END_OF_STREAM: bool = false,
    SEND_USEFILTER: bool = false,
    VOICE_NOSAMPLESPLAYED: bool = false,
    __unused9: bool = false,
    __unused10: bool = false,
    __unused11: bool = false,
    __unused12: bool = false,
    STOP_ENGINE_WHEN_IDLE: bool = false,
    __unused14: bool = false,
    @"1024_QUANTUM": bool = false,
    NO_VIRTUAL_AUDIO_CLIENT: bool = false,
    __unused: u15 = 0,
};

pub const VOICE_DETAILS = extern struct {
    CreationFlags: FLAGS align(1),
    ActiveFlags: FLAGS align(1),
    InputChannels: UINT32 align(1),
    InputSampleRate: UINT32 align(1),
};

pub const SEND_DESCRIPTOR = extern struct {
    Flags: FLAGS align(1),
    pOutputVoice: *IVoice align(1),
};

pub const VOICE_SENDS = extern struct {
    SendCount: UINT32 align(1),
    pSends: [*]SEND_DESCRIPTOR align(1),
};

pub const EFFECT_DESCRIPTOR = extern struct {
    pEffect: *IUnknown align(1),
    InitialState: BOOL align(1),
    OutputChannels: UINT32 align(1),
};

pub const EFFECT_CHAIN = extern struct {
    EffectCount: UINT32 align(1),
    pEffectDescriptors: [*]EFFECT_DESCRIPTOR align(1),
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

pub const FILTER_PARAMETERS = extern struct {
    Type: FILTER_TYPE align(1),
    Frequency: f32 align(1),
    OneOverQ: f32 align(1),
};

pub const BUFFER = extern struct {
    Flags: FLAGS align(1),
    AudioBytes: UINT32 align(1),
    pAudioData: [*]const BYTE align(1),
    PlayBegin: UINT32 align(1),
    PlayLength: UINT32 align(1),
    LoopBegin: UINT32 align(1),
    LoopLength: UINT32 align(1),
    LoopCount: UINT32 align(1),
    pContext: ?*anyopaque align(1),
};

pub const BUFFER_WMA = extern struct {
    pDecodedPacketCumulativeBytes: *const UINT32 align(1),
    PacketCount: UINT32 align(1),
};

pub const VOICE_STATE = extern struct {
    pCurrentBufferContext: ?*anyopaque align(1),
    BuffersQueued: UINT32 align(1),
    SamplesPlayed: UINT64 align(1),
};

pub const PERFORMANCE_DATA = extern struct {
    AudioCyclesSinceLastQuery: UINT64 align(1),
    TotalCyclesSinceLastQuery: UINT64 align(1),
    MinimumCyclesPerQuantum: UINT32 align(1),
    MaximumCyclesPerQuantum: UINT32 align(1),
    MemoryUsageInBytes: UINT32 align(1),
    CurrentLatencyInSamples: UINT32 align(1),
    GlitchesSinceEngineStarted: UINT32 align(1),
    ActiveSourceVoiceCount: UINT32 align(1),
    TotalSourceVoiceCount: UINT32 align(1),
    ActiveSubmixVoiceCount: UINT32 align(1),
    ActiveResamplerCount: UINT32 align(1),
    ActiveMatrixMixCount: UINT32 align(1),
    ActiveXmaSourceVoices: UINT32 align(1),
    ActiveXmaStreams: UINT32 align(1),
};

pub const LOG_FLAGS = packed struct(UINT32) {
    ERRORS: bool = false,
    WARNINGS: bool = false,
    INFO: bool = false,
    DETAIL: bool = false,
    API_CALLS: bool = false,
    FUNC_CALLS: bool = false,
    TIMING: bool = false,
    LOCKS: bool = false,
    MEMORY: bool = false,
    __unused9: bool = false,
    __unused10: bool = false,
    __unused11: bool = false,
    STREAMING: bool = false,
    __unused: u19 = 0,
};

pub const DEBUG_CONFIGURATION = extern struct {
    TraceMask: LOG_FLAGS align(1),
    BreakMask: LOG_FLAGS align(1),
    LogThreadID: BOOL align(1),
    LogFileline: BOOL align(1),
    LogFunctionName: BOOL align(1),
    LogTiming: BOOL align(1),
};

pub const IXAudio2 = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IUnknown.Methods(T);

            pub inline fn RegisterForCallbacks(self: *T, cb: *IEngineCallback) HRESULT {
                return @as(*const IXAudio2.VTable, @ptrCast(self.__v))
                    .RegisterForCallbacks(@as(*IXAudio2, @ptrCast(self)), cb);
            }
            pub inline fn UnregisterForCallbacks(self: *T, cb: *IEngineCallback) void {
                @as(*const IXAudio2.VTable, @ptrCast(self.__v))
                    .UnregisterForCallbacks(@as(*IXAudio2, @ptrCast(self)), cb);
            }
            pub inline fn CreateSourceVoice(
                self: *T,
                source_voice: *?*ISourceVoice,
                source_format: *const WAVEFORMATEX,
                flags: FLAGS,
                max_frequency_ratio: f32,
                callback: ?*IVoiceCallback,
                send_list: ?*const VOICE_SENDS,
                effect_chain: ?*const EFFECT_CHAIN,
            ) HRESULT {
                return @as(*const IXAudio2.VTable, @ptrCast(self.__v)).CreateSourceVoice(
                    @as(*IXAudio2, @ptrCast(self)),
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
                flags: FLAGS,
                processing_stage: UINT32,
                send_list: ?*const VOICE_SENDS,
                effect_chain: ?*const EFFECT_CHAIN,
            ) HRESULT {
                return @as(*const IXAudio2.VTable, @ptrCast(self.__v)).CreateSubmixVoice(
                    @as(*IXAudio2, @ptrCast(self)),
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
                flags: FLAGS,
                device_id: ?LPCWSTR,
                effect_chain: ?*const EFFECT_CHAIN,
                stream_category: AUDIO_STREAM_CATEGORY,
            ) HRESULT {
                return @as(*const IXAudio2.VTable, @ptrCast(self.__v)).CreateMasteringVoice(
                    @as(*IXAudio2, @ptrCast(self)),
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
                return @as(*const IXAudio2.VTable, @ptrCast(self.__v))
                    .StartEngine(@as(*IXAudio2, @ptrCast(self)));
            }
            pub inline fn StopEngine(self: *T) void {
                @as(*const IXAudio2.VTable, @ptrCast(self.__v)).StopEngine(@as(*IXAudio2, @ptrCast(self)));
            }
            pub inline fn CommitChanges(self: *T, operation_set: UINT32) HRESULT {
                return @as(*const IXAudio2.VTable, @ptrCast(self.__v))
                    .CommitChanges(@as(*IXAudio2, @ptrCast(self)), operation_set);
            }
            pub inline fn GetPerformanceData(self: *T, data: *PERFORMANCE_DATA) void {
                @as(*const IXAudio2.VTable, @ptrCast(self.__v))
                    .GetPerformanceData(@as(*IXAudio2, @ptrCast(self)), data);
            }
            pub inline fn SetDebugConfiguration(
                self: *T,
                config: ?*const DEBUG_CONFIGURATION,
                reserved: ?*anyopaque,
            ) void {
                @as(*const IXAudio2.VTable, @ptrCast(self.__v))
                    .SetDebugConfiguration(@as(*IXAudio2, @ptrCast(self)), config, reserved);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IXAudio2;
        base: IUnknown.VTable,
        RegisterForCallbacks: *const fn (*T, *IEngineCallback) callconv(WINAPI) HRESULT,
        UnregisterForCallbacks: *const fn (*T, *IEngineCallback) callconv(WINAPI) void,
        CreateSourceVoice: *const fn (
            *T,
            *?*ISourceVoice,
            *const WAVEFORMATEX,
            FLAGS,
            f32,
            ?*IVoiceCallback,
            ?*const VOICE_SENDS,
            ?*const EFFECT_CHAIN,
        ) callconv(WINAPI) HRESULT,
        CreateSubmixVoice: *const fn (
            *T,
            *?*ISubmixVoice,
            UINT32,
            UINT32,
            FLAGS,
            UINT32,
            ?*const VOICE_SENDS,
            ?*const EFFECT_CHAIN,
        ) callconv(WINAPI) HRESULT,
        CreateMasteringVoice: *const fn (
            *T,
            *?*IMasteringVoice,
            UINT32,
            UINT32,
            FLAGS,
            ?LPCWSTR,
            ?*const EFFECT_CHAIN,
            AUDIO_STREAM_CATEGORY,
        ) callconv(WINAPI) HRESULT,
        StartEngine: *const fn (*T) callconv(WINAPI) HRESULT,
        StopEngine: *const fn (*T) callconv(WINAPI) void,
        CommitChanges: *const fn (*T, UINT32) callconv(WINAPI) HRESULT,
        GetPerformanceData: *const fn (*T, *PERFORMANCE_DATA) callconv(WINAPI) void,
        SetDebugConfiguration: *const fn (
            *T,
            ?*const DEBUG_CONFIGURATION,
            ?*anyopaque,
        ) callconv(WINAPI) void,
    };
};

pub const IVoice = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetVoiceDetails(self: *T, details: *VOICE_DETAILS) void {
                @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .GetVoiceDetails(@as(*IVoice, @ptrCast(self)), details);
            }
            pub inline fn SetOutputVoices(self: *T, send_list: ?*const VOICE_SENDS) HRESULT {
                return @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .SetOutputVoices(@as(*IVoice, @ptrCast(self)), send_list);
            }
            pub inline fn SetEffectChain(self: *T, effect_chain: ?*const EFFECT_CHAIN) HRESULT {
                return @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .SetEffectChain(@as(*IVoice, @ptrCast(self)), effect_chain);
            }
            pub inline fn EnableEffect(self: *T, effect_index: UINT32, operation_set: UINT32) HRESULT {
                return @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .EnableEffect(@as(*IVoice, @ptrCast(self)), effect_index, operation_set);
            }
            pub inline fn DisableEffect(self: *T, effect_index: UINT32, operation_set: UINT32) HRESULT {
                return @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .DisableEffect(@as(*IVoice, @ptrCast(self)), effect_index, operation_set);
            }
            pub inline fn GetEffectState(self: *T, effect_index: UINT32, enabled: *BOOL) void {
                @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .GetEffectState(@as(*IVoice, @ptrCast(self)), effect_index, enabled);
            }
            pub inline fn SetEffectParameters(
                self: *T,
                effect_index: UINT32,
                params: *const anyopaque,
                params_size: UINT32,
                operation_set: UINT32,
            ) HRESULT {
                return @as(*const IVoice.VTable, @ptrCast(self.__v)).SetEffectParameters(
                    @as(*IVoice, @ptrCast(self)),
                    effect_index,
                    params,
                    params_size,
                    operation_set,
                );
            }
            pub inline fn GetEffectParameters(
                self: *T,
                effect_index: UINT32,
                params: *anyopaque,
                params_size: UINT32,
            ) HRESULT {
                return @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .GetEffectParameters(@as(*IVoice, @ptrCast(self)), effect_index, params, params_size);
            }
            pub inline fn SetFilterParameters(
                self: *T,
                params: *const FILTER_PARAMETERS,
                operation_set: UINT32,
            ) HRESULT {
                return @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .SetFilterParameters(@as(*IVoice, @ptrCast(self)), params, operation_set);
            }
            pub inline fn GetFilterParameters(self: *T, params: *FILTER_PARAMETERS) void {
                @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .GetFilterParameters(@as(*IVoice, @ptrCast(self)), params);
            }
            pub inline fn SetOutputFilterParameters(
                self: *T,
                dst_voice: ?*IVoice,
                params: *const FILTER_PARAMETERS,
                operation_set: UINT32,
            ) HRESULT {
                return @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .SetOutputFilterParameters(@as(*IVoice, @ptrCast(self)), dst_voice, params, operation_set);
            }
            pub inline fn GetOutputFilterParameters(
                self: *T,
                dst_voice: ?*IVoice,
                params: *FILTER_PARAMETERS,
            ) void {
                @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .GetOutputFilterParameters(@as(*IVoice, @ptrCast(self)), dst_voice, params);
            }
            pub inline fn SetVolume(self: *T, volume: f32) HRESULT {
                return @as(*const IVoice.VTable, @ptrCast(self.__v)).SetVolume(@as(*IVoice, @ptrCast(self)), volume);
            }
            pub inline fn GetVolume(self: *T, volume: *f32) void {
                @as(*const IVoice.VTable, @ptrCast(self.__v)).GetVolume(@as(*IVoice, @ptrCast(self)), volume);
            }
            pub inline fn SetChannelVolumes(
                self: *T,
                num_channels: UINT32,
                volumes: [*]const f32,
                operation_set: UINT32,
            ) HRESULT {
                return @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .SetChannelVolumes(@as(*IVoice, @ptrCast(self)), num_channels, volumes, operation_set);
            }
            pub inline fn GetChannelVolumes(self: *T, num_channels: UINT32, volumes: [*]f32) void {
                @as(*const IVoice.VTable, @ptrCast(self.__v))
                    .GetChannelVolumes(@as(*IVoice, @ptrCast(self)), num_channels, volumes);
            }
            pub inline fn DestroyVoice(self: *T) void {
                @as(*const IVoice.VTable, @ptrCast(self.__v)).DestroyVoice(@as(*IVoice, @ptrCast(self)));
            }
        };
    }

    pub const VTable = extern struct {
        const T = IVoice;
        GetVoiceDetails: *const fn (*T, *VOICE_DETAILS) callconv(WINAPI) void,
        SetOutputVoices: *const fn (*T, ?*const VOICE_SENDS) callconv(WINAPI) HRESULT,
        SetEffectChain: *const fn (*T, ?*const EFFECT_CHAIN) callconv(WINAPI) HRESULT,
        EnableEffect: *const fn (*T, UINT32, UINT32) callconv(WINAPI) HRESULT,
        DisableEffect: *const fn (*T, UINT32, UINT32) callconv(WINAPI) HRESULT,
        GetEffectState: *const fn (*T, UINT32, *BOOL) callconv(WINAPI) void,
        SetEffectParameters: *const fn (
            *T,
            UINT32,
            *const anyopaque,
            UINT32,
            UINT32,
        ) callconv(WINAPI) HRESULT,
        GetEffectParameters: *const fn (*T, UINT32, *anyopaque, UINT32) callconv(WINAPI) HRESULT,
        SetFilterParameters: *const fn (
            *T,
            *const FILTER_PARAMETERS,
            UINT32,
        ) callconv(WINAPI) HRESULT,
        GetFilterParameters: *const fn (*T, *FILTER_PARAMETERS) callconv(WINAPI) void,
        SetOutputFilterParameters: *const fn (
            *T,
            ?*IVoice,
            *const FILTER_PARAMETERS,
            UINT32,
        ) callconv(WINAPI) HRESULT,
        GetOutputFilterParameters: *const fn (*T, ?*IVoice, *FILTER_PARAMETERS) callconv(WINAPI) void,
        SetVolume: *const fn (*T, f32) callconv(WINAPI) HRESULT,
        GetVolume: *const fn (*T, *f32) callconv(WINAPI) void,
        SetChannelVolumes: *const fn (*T, UINT32, [*]const f32, UINT32) callconv(WINAPI) HRESULT,
        GetChannelVolumes: *const fn (*T, UINT32, [*]f32) callconv(WINAPI) void,
        SetOutputMatrix: *anyopaque,
        GetOutputMatrix: *anyopaque,
        DestroyVoice: *const fn (*T) callconv(WINAPI) void,
    };
};

pub const ISourceVoice = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IVoice.Methods(T);

            pub inline fn Start(self: *T, flags: FLAGS, operation_set: UINT32) HRESULT {
                return @as(*const ISourceVoice.VTable, @ptrCast(self.__v))
                    .Start(@as(*ISourceVoice, @ptrCast(self)), flags, operation_set);
            }
            pub inline fn Stop(self: *T, flags: FLAGS, operation_set: UINT32) HRESULT {
                return @as(*const ISourceVoice.VTable, @ptrCast(self.__v))
                    .Stop(@as(*ISourceVoice, @ptrCast(self)), flags, operation_set);
            }
            pub inline fn SubmitSourceBuffer(
                self: *T,
                buffer: *const BUFFER,
                wmabuffer: ?*const BUFFER_WMA,
            ) HRESULT {
                return @as(*const ISourceVoice.VTable, @ptrCast(self.__v))
                    .SubmitSourceBuffer(@as(*ISourceVoice, @ptrCast(self)), buffer, wmabuffer);
            }
            pub inline fn FlushSourceBuffers(self: *T) HRESULT {
                return @as(*const ISourceVoice.VTable, @ptrCast(self.__v))
                    .FlushSourceBuffers(@as(*ISourceVoice, @ptrCast(self)));
            }
            pub inline fn Discontinuity(self: *T) HRESULT {
                return @as(*const ISourceVoice.VTable, @ptrCast(self.__v))
                    .Discontinuity(@as(*ISourceVoice, @ptrCast(self)));
            }
            pub inline fn ExitLoop(self: *T, operation_set: UINT32) HRESULT {
                return @as(*const ISourceVoice.VTable, @ptrCast(self.__v))
                    .ExitLoop(@as(*ISourceVoice, @ptrCast(self)), operation_set);
            }
            pub inline fn GetState(self: *T, state: *VOICE_STATE, flags: FLAGS) void {
                @as(*const ISourceVoice.VTable, @ptrCast(self.__v))
                    .GetState(@as(*ISourceVoice, @ptrCast(self)), state, flags);
            }
            pub inline fn SetFrequencyRatio(self: *T, ratio: f32, operation_set: UINT32) HRESULT {
                return @as(*const ISourceVoice.VTable, @ptrCast(self.__v))
                    .SetFrequencyRatio(@as(*ISourceVoice, @ptrCast(self)), ratio, operation_set);
            }
            pub inline fn GetFrequencyRatio(self: *T, ratio: *f32) void {
                @as(*const ISourceVoice.VTable, @ptrCast(self.__v))
                    .GetFrequencyRatio(@as(*ISourceVoice, @ptrCast(self)), ratio);
            }
            pub inline fn SetSourceSampleRate(self: *T, sample_rate: UINT32) HRESULT {
                return @as(*const ISourceVoice.VTable, @ptrCast(self.__v))
                    .SetSourceSampleRate(@as(*ISourceVoice, @ptrCast(self)), sample_rate);
            }
        };
    }

    pub const VTable = extern struct {
        const T = ISourceVoice;
        base: IVoice.VTable,
        Start: *const fn (*T, FLAGS, UINT32) callconv(WINAPI) HRESULT,
        Stop: *const fn (*T, FLAGS, UINT32) callconv(WINAPI) HRESULT,
        SubmitSourceBuffer: *const fn (
            *T,
            *const BUFFER,
            ?*const BUFFER_WMA,
        ) callconv(WINAPI) HRESULT,
        FlushSourceBuffers: *const fn (*T) callconv(WINAPI) HRESULT,
        Discontinuity: *const fn (*T) callconv(WINAPI) HRESULT,
        ExitLoop: *const fn (*T, UINT32) callconv(WINAPI) HRESULT,
        GetState: *const fn (*T, *VOICE_STATE, FLAGS) callconv(WINAPI) void,
        SetFrequencyRatio: *const fn (*T, f32, UINT32) callconv(WINAPI) HRESULT,
        GetFrequencyRatio: *const fn (*T, *f32) callconv(WINAPI) void,
        SetSourceSampleRate: *const fn (*T, UINT32) callconv(WINAPI) HRESULT,
    };
};

pub const ISubmixVoice = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IVoice.Methods(T);
        };
    }

    pub const VTable = extern struct {
        base: IVoice.VTable,
    };
};

pub const IMasteringVoice = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IVoice.Methods(T);

            pub inline fn GetChannelMask(self: *T, channel_mask: *DWORD) HRESULT {
                return @as(*const IMasteringVoice.VTable, @ptrCast(self.__v))
                    .GetChannelMask(@as(*IMasteringVoice, @ptrCast(self)), channel_mask);
            }
        };
    }

    pub const VTable = extern struct {
        base: IVoice.VTable,
        GetChannelMask: *const fn (*IMasteringVoice, *DWORD) callconv(WINAPI) HRESULT,
    };
};

pub const IEngineCallback = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn OnProcessingPassStart(self: *T) void {
                @as(*const IEngineCallback.VTable, @ptrCast(self.__v))
                    .OnProcessingPassStart(@as(*IEngineCallback, @ptrCast(self)));
            }
            pub inline fn OnProcessingPassEnd(self: *T) void {
                @as(*const IEngineCallback.VTable, @ptrCast(self.__v))
                    .OnProcessingPassEnd(@as(*IEngineCallback, @ptrCast(self)));
            }
            pub inline fn OnCriticalError(self: *T, err: HRESULT) void {
                @as(*const IEngineCallback.VTable, @ptrCast(self.__v))
                    .OnCriticalError(@as(*IEngineCallback, @ptrCast(self)), err);
            }
        };
    }

    pub const VTable = extern struct {
        OnProcessingPassStart: *const fn (*IEngineCallback) callconv(WINAPI) void = _onProcessingPassStart,

        OnProcessingPassEnd: *const fn (*IEngineCallback) callconv(WINAPI) void = _onProcessingPassEnd,

        OnCriticalError: *const fn (*IEngineCallback, HRESULT) callconv(WINAPI) void = _onCriticalError,
    };

    // Default implementations
    fn _onProcessingPassStart(_: *IEngineCallback) callconv(WINAPI) void {}
    fn _onProcessingPassEnd(_: *IEngineCallback) callconv(WINAPI) void {}
    fn _onCriticalError(_: *IEngineCallback, _: HRESULT) callconv(WINAPI) void {}
};

pub const IVoiceCallback = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn OnVoiceProcessingPassStart(self: *T, bytes_required: UINT32) void {
                @as(*const IVoiceCallback.VTable, @ptrCast(self.__v))
                    .OnVoiceProcessingPassStart(@as(*IVoiceCallback, @ptrCast(self)), bytes_required);
            }
            pub inline fn OnVoiceProcessingPassEnd(self: *T) void {
                @as(*const IVoiceCallback.VTable, @ptrCast(self.__v))
                    .OnVoiceProcessingPassEnd(@as(*IVoiceCallback, @ptrCast(self)));
            }
            pub inline fn OnStreamEnd(self: *T) void {
                @as(*const IVoiceCallback.VTable, @ptrCast(self.__v))
                    .OnStreamEnd(@as(*IVoiceCallback, @ptrCast(self)));
            }
            pub inline fn OnBufferStart(self: *T, context: ?*anyopaque) void {
                @as(*const IVoiceCallback.VTable, @ptrCast(self.__v))
                    .OnBufferStart(@as(*IVoiceCallback, @ptrCast(self)), context);
            }
            pub inline fn OnBufferEnd(self: *T, context: ?*anyopaque) void {
                @as(*const IVoiceCallback.VTable, @ptrCast(self.__v))
                    .OnBufferEnd(@as(*IVoiceCallback, @ptrCast(self)), context);
            }
            pub inline fn OnLoopEnd(self: *T, context: ?*anyopaque) void {
                @as(*const IVoiceCallback.VTable, @ptrCast(self.__v))
                    .OnLoopEnd(@as(*IVoiceCallback, @ptrCast(self)), context);
            }
            pub inline fn OnVoiceError(self: *T, context: ?*anyopaque, err: HRESULT) void {
                @as(*const IVoiceCallback.VTable, @ptrCast(self.__v))
                    .OnVoiceError(@as(*IVoiceCallback, @ptrCast(self)), context, err);
            }
        };
    }

    pub const VTable = extern struct {
        OnVoiceProcessingPassStart: *const fn (*IVoiceCallback, UINT32) callconv(WINAPI) void =
            _onVoiceProcessingPassStart,

        OnVoiceProcessingPassEnd: *const fn (*IVoiceCallback) callconv(WINAPI) void =
            _onVoiceProcessingPassEnd,

        OnStreamEnd: *const fn (*IVoiceCallback) callconv(WINAPI) void = _onStreamEnd,

        OnBufferStart: *const fn (*IVoiceCallback, ?*anyopaque) callconv(WINAPI) void = _onBufferStart,

        OnBufferEnd: *const fn (*IVoiceCallback, ?*anyopaque) callconv(WINAPI) void = _onBufferEnd,

        OnLoopEnd: *const fn (*IVoiceCallback, ?*anyopaque) callconv(WINAPI) void = _onLoopEnd,

        OnVoiceError: *const fn (*IVoiceCallback, ?*anyopaque, HRESULT) callconv(WINAPI) void = _onVoiceError,
    };

    // Default implementations
    fn _onVoiceProcessingPassStart(_: *IVoiceCallback, _: UINT32) callconv(WINAPI) void {}
    fn _onVoiceProcessingPassEnd(_: *IVoiceCallback) callconv(WINAPI) void {}
    fn _onStreamEnd(_: *IVoiceCallback) callconv(WINAPI) void {}
    fn _onBufferStart(_: *IVoiceCallback, _: ?*anyopaque) callconv(WINAPI) void {}
    fn _onBufferEnd(_: *IVoiceCallback, _: ?*anyopaque) callconv(WINAPI) void {}
    fn _onLoopEnd(_: *IVoiceCallback, _: ?*anyopaque) callconv(WINAPI) void {}
    fn _onVoiceError(_: *IVoiceCallback, _: ?*anyopaque, _: HRESULT) callconv(WINAPI) void {}
};

pub fn create(
    ppv: *?*IXAudio2,
    flags: FLAGS, // .{}
    processor: UINT32, // 0
) HRESULT {
    var xaudio2_dll = w32.GetModuleHandleA("xaudio2_9redist.dll");
    if (xaudio2_dll == null) {
        xaudio2_dll = w32.LoadLibraryA("xaudio2_9redist.dll");
    }

    var xaudio2Create: *const fn (*?*IXAudio2, FLAGS, UINT32) callconv(WINAPI) HRESULT = undefined;
    xaudio2Create = @as(
        @TypeOf(xaudio2Create),
        @ptrCast(w32.GetProcAddress(xaudio2_dll.?, "XAudio2Create").?),
    );

    return xaudio2Create(ppv, flags, processor);
}
