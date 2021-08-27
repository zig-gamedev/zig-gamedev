usingnamespace @import("windows.zig");
usingnamespace @import("audiosessiontypes.zig");
usingnamespace @import("mmreg.zig");

pub const IAudioClient = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        audioclient: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Initialize(
                self: *T,
                mode: AUDCLNT_SHAREMODE,
                stream_flags: DWORD,
                buffer_duration: REFERENCE_TIME,
                periodicity: REFERENCE_TIME,
                format: *const WAVEFORMATEX,
                audio_session: ?*?*GUID,
            ) HRESULT {
                return self.v.audioclient.Initialize(
                    self,
                    mode,
                    stream_flags,
                    buffer_duration,
                    periodicity,
                    format,
                    audio_session,
                );
            }
            pub inline fn GetBufferSize(self: *T, size: *UINT32) HRESULT {
                return self.v.audioclient.GetBufferSize(self, size);
            }
            pub inline fn GetStreamLatency(self: *T, latency: *REFERENCE_TIME) HRESULT {
                return self.v.audioclient.GetStreamLatency(self, latency);
            }
            pub inline fn GetCurrentPadding(self: *T, padding: *UINT32) HRESULT {
                return self.v.audioclient.GetCurrentPadding(self, padding);
            }
            pub inline fn IsFormatSupported(
                self: *T,
                mode: AUDCLNT_SHAREMODE,
                format: *const WAVEFORMATEX,
                closest_format: ?*?*WAVEFORMATEX,
            ) HRESULT {
                return self.v.audioclient.IsFormatSupported(self, mode, format, closest_format);
            }
            pub inline fn GetMixFormat(self: *T, format: **WAVEFORMATEX) HRESULT {
                return self.v.audioclient.GetMixFormat(self, format);
            }
            pub inline fn GetDevicePeriod(self: *T, default: ?*REFERENCE_TIME, minimum: ?*REFERENCE_TIME) HRESULT {
                return self.v.audioclient.GetDevicePeriod(self, default, minimum);
            }
            pub inline fn Start(self: *T) HRESULT {
                return self.v.audioclient.Start(self);
            }
            pub inline fn Stop(self: *T) HRESULT {
                return self.v.audioclient.Stop(self);
            }
            pub inline fn Reset(self: *T) HRESULT {
                return self.v.audioclient.Reset(self);
            }
            pub inline fn SetEventHandle(self: *T, handle: HANDLE) HRESULT {
                return self.v.audioclient.SetEventHandle(self, handle);
            }
            pub inline fn GetService(self: *T, guid: *const GUID, iface: *?*c_void) HRESULT {
                return self.v.audioclient.GetService(self, guid, iface);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            Initialize: fn (
                *T,
                AUDCLNT_SHAREMODE,
                DWORD,
                REFERENCE_TIME,
                REFERENCE_TIME,
                *const WAVEFORMATEX,
                ?*?*GUID,
            ) callconv(WINAPI) HRESULT,
            GetBufferSize: fn (*T, *UINT32) callconv(WINAPI) HRESULT,
            GetStreamLatency: fn (*T, *REFERENCE_TIME) callconv(WINAPI) HRESULT,
            GetCurrentPadding: fn (*T, *UINT32) callconv(WINAPI) HRESULT,
            IsFormatSupported: fn (*T, AUDCLNT_SHAREMODE, *const WAVEFORMATEX, ?*?*WAVEFORMATEX) callconv(WINAPI) HRESULT,
            GetMixFormat: fn (*T, **WAVEFORMATEX) callconv(WINAPI) HRESULT,
            GetDevicePeriod: fn (*T, ?*REFERENCE_TIME, ?*REFERENCE_TIME) callconv(WINAPI) HRESULT,
            Start: fn (*T) callconv(WINAPI) HRESULT,
            Stop: fn (*T) callconv(WINAPI) HRESULT,
            Reset: fn (*T) callconv(WINAPI) HRESULT,
            SetEventHandle: fn (*T, HANDLE) callconv(WINAPI) HRESULT,
            GetService: fn (*T, *const GUID, *?*c_void) callconv(WINAPI) HRESULT,
        };
    }
};

pub const IAudioClient2 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        audioclient: IAudioClient.VTable(Self),
        audioclient2: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IAudioClient.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            IsOffloadCapable: *c_void,
            SetClientProperties: *c_void,
            GetBufferSizeLimits: *c_void,
        };
    }
};

// NOTE(mziulek): IAudioClient3 interface is available on Windows 10 and newer.
pub const IAudioClient3 = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        audioclient: IAudioClient.VTable(Self),
        audioclient2: IAudioClient2.VTable(Self),
        audioclient3: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IAudioClient.Methods(Self);
    usingnamespace IAudioClient2.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    pub fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetSharedModeEnginePeriod: *c_void,
            GetCurrentSharedModeEnginePeriod: *c_void,
            InitializeSharedAudioStream: *c_void,
        };
    }
};

pub const IAudioRenderClient = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        renderclient: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetBuffer(self: *T, num_frames_requested: UINT32, data: *?*BYTE) HRESULT {
                return self.v.renderclient.GetBuffer(self, num_frames_requested, data);
            }
            pub inline fn ReleaseBuffer(self: *T, num_frames_written: UINT32, flags: DWORD) HRESULT {
                return self.v.renderclient.ReleaseBuffer(self, num_frames_written, flags);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetBuffer: fn (*T, UINT32, *?*BYTE) callconv(WINAPI) HRESULT,
            ReleaseBuffer: fn (*T, UINT32, DWORD) callconv(WINAPI) HRESULT,
        };
    }
};

// NOTE(mziulek): IAudioClient3 interface is available on Windows 10 and newer.
pub const IID_IAudioClient3 = GUID{
    .Data1 = 0x7ED4EE07,
    .Data2 = 0x8E67,
    .Data3 = 0x4CD4,
    .Data4 = .{ 0x8C, 0x1A, 0x2B, 0x7A, 0x59, 0x87, 0xAD, 0x42 },
};

pub const IID_IAudioRenderClient = GUID{
    .Data1 = 0xF294ACFC,
    .Data2 = 0x3146,
    .Data3 = 0x4483,
    .Data4 = .{ 0xA7, 0xBF, 0xAD, 0xDC, 0xA7, 0xC2, 0x60, 0xE2 },
};
