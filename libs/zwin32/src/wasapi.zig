const windows = @import("windows.zig");
const BYTE = windows.BYTE;
const UINT = windows.UINT;
const UINT32 = windows.UINT32;
const IUnknown = windows.IUnknown;
const WINAPI = windows.WINAPI;
const GUID = windows.GUID;
const HRESULT = windows.HRESULT;
const DWORD = windows.DWORD;
const WORD = windows.WORD;
const PROPVARIANT = windows.PROPVARIANT;
const HANDLE = windows.HANDLE;
const REFERENCE_TIME = windows.REFERENCE_TIME;
const MAKE_HRESULT = windows.MAKE_HRESULT;
const SEVERITY_SUCCESS = windows.SEVERITY_SUCCESS;
const SEVERITY_ERROR = windows.SEVERITY_ERROR;

pub const EDataFlow = enum(UINT) {
    eRender = 0,
    eCapture = 1,
    eAll = 2,
};

pub const ERole = enum(UINT) {
    eConsole = 0,
    eMultimedia = 1,
    eCommunications = 2,
};

pub const IMMDevice = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        device: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Activate(
                self: *T,
                guid: *const GUID,
                clsctx: DWORD,
                params: ?*PROPVARIANT,
                iface: *?*anyopaque,
            ) HRESULT {
                return self.v.device.Activate(self, guid, clsctx, params, iface);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            Activate: fn (*T, *const GUID, DWORD, ?*PROPVARIANT, *?*anyopaque) callconv(WINAPI) HRESULT,
            OpenPropertyStore: *anyopaque,
            GetId: *anyopaque,
            GetState: *anyopaque,
        };
    }
};

pub const IMMDeviceEnumerator = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        devenum: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetDefaultAudioEndpoint(
                self: *T,
                flow: EDataFlow,
                role: ERole,
                endpoint: *?*IMMDevice,
            ) HRESULT {
                return self.v.devenum.GetDefaultAudioEndpoint(self, flow, role, endpoint);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            EnumAudioEndpoints: *anyopaque,
            GetDefaultAudioEndpoint: fn (*T, EDataFlow, ERole, *?*IMMDevice) callconv(WINAPI) HRESULT,
            GetDevice: *anyopaque,
            RegisterEndpointNotificationCallback: *anyopaque,
            UnregisterEndpointNotificationCallback: *anyopaque,
        };
    }
};

pub const CLSID_MMDeviceEnumerator = GUID{
    .Data1 = 0xBCDE0395,
    .Data2 = 0xE52F,
    .Data3 = 0x467C,
    .Data4 = .{ 0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E },
};
pub const IID_IMMDeviceEnumerator = GUID{
    .Data1 = 0xA95664D2,
    .Data2 = 0x9614,
    .Data3 = 0x4F35,
    .Data4 = .{ 0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6 },
};

pub const WAVEFORMATEX = extern struct {
    wFormatTag: WORD,
    nChannels: WORD,
    nSamplesPerSec: DWORD,
    nAvgBytesPerSec: DWORD,
    nBlockAlign: WORD,
    wBitsPerSample: WORD,
    cbSize: WORD,
};

pub const WAVE_FORMAT_PCM: UINT = 1;
pub const WAVE_FORMAT_IEEE_FLOAT: UINT = 0x0003;

pub const AUDCLNT_SHAREMODE = enum(UINT) {
    SHARED = 0,
    EXCLUSIVE = 1,
};

pub const AUDCLNT_STREAMFLAGS_CROSSPROCESS: UINT = 0x00010000;
pub const AUDCLNT_STREAMFLAGS_LOOPBACK: UINT = 0x00020000;
pub const AUDCLNT_STREAMFLAGS_EVENTCALLBACK: UINT = 0x00040000;
pub const AUDCLNT_STREAMFLAGS_NOPERSIST: UINT = 0x00080000;
pub const AUDCLNT_STREAMFLAGS_RATEADJUST: UINT = 0x00100000;
pub const AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY: UINT = 0x08000000;
pub const AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM: UINT = 0x80000000;
pub const AUDCLNT_SESSIONFLAGS_EXPIREWHENUNOWNED: UINT = 0x10000000;
pub const AUDCLNT_SESSIONFLAGS_DISPLAY_HIDE: UINT = 0x20000000;
pub const AUDCLNT_SESSIONFLAGS_DISPLAY_HIDEWHENEXPIRED: UINT = 0x40000000;

pub const AUDCLNT_BUFFERFLAGS_DATA_DISCONTINUITY: UINT = 0x1;
pub const AUDCLNT_BUFFERFLAGS_SILENT: UINT = 0x2;
pub const AUDCLNT_BUFFERFLAGS_TIMESTAMP_ERROR: UINT = 0x4;

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
            pub inline fn GetService(self: *T, guid: *const GUID, iface: *?*anyopaque) HRESULT {
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
            IsFormatSupported: fn (
                *T,
                AUDCLNT_SHAREMODE,
                *const WAVEFORMATEX,
                ?*?*WAVEFORMATEX,
            ) callconv(WINAPI) HRESULT,
            GetMixFormat: fn (*T, **WAVEFORMATEX) callconv(WINAPI) HRESULT,
            GetDevicePeriod: fn (*T, ?*REFERENCE_TIME, ?*REFERENCE_TIME) callconv(WINAPI) HRESULT,
            Start: fn (*T) callconv(WINAPI) HRESULT,
            Stop: fn (*T) callconv(WINAPI) HRESULT,
            Reset: fn (*T) callconv(WINAPI) HRESULT,
            SetEventHandle: fn (*T, HANDLE) callconv(WINAPI) HRESULT,
            GetService: fn (*T, *const GUID, *?*anyopaque) callconv(WINAPI) HRESULT,
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
            IsOffloadCapable: *anyopaque,
            SetClientProperties: *anyopaque,
            GetBufferSizeLimits: *anyopaque,
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
            GetSharedModeEnginePeriod: *anyopaque,
            GetCurrentSharedModeEnginePeriod: *anyopaque,
            InitializeSharedAudioStream: *anyopaque,
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
            pub inline fn GetBuffer(self: *T, num_frames_requested: UINT32, data: ?*?[*]BYTE) HRESULT {
                return self.v.renderclient.GetBuffer(self, num_frames_requested, data);
            }
            pub inline fn ReleaseBuffer(self: *T, num_frames_written: UINT32, flags: DWORD) HRESULT {
                return self.v.renderclient.ReleaseBuffer(self, num_frames_written, flags);
            }
        };
    }

    pub fn VTable(comptime T: type) type {
        return extern struct {
            GetBuffer: fn (*T, UINT32, ?*?[*]BYTE) callconv(WINAPI) HRESULT,
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

pub const FACILITY_AUDCLNT = 0x889;

pub inline fn AUDCLNT_SUCCESS(code: anytype) HRESULT {
    return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_AUDCLNT, code);
}

pub inline fn AUDCLNT_ERR(code: anytype) HRESULT {
    return MAKE_HRESULT(SEVERITY_ERROR, FACILITY_AUDCLNT, code);
}

// success return codes
pub const AUDCLNT_S_BUFFER_EMPTY = AUDCLNT_SUCCESS(0x001);
pub const AUDCLNT_S_THREAD_ALREADY_REGISTERED = AUDCLNT_SUCCESS(0x002);
pub const AUDCLNT_S_POSITION_STALLED = AUDCLNT_SUCCESS(0x003);

// error return codes
pub const AUDCLNT_E_NOT_INITIALIZED = AUDCLNT_ERR(0x001);
pub const AUDCLNT_E_ALREADY_INITIALIZED = AUDCLNT_ERR(0x002);
pub const AUDCLNT_E_WRONG_ENDPOINT_TYPE = AUDCLNT_ERR(0x003);
pub const AUDCLNT_E_DEVICE_INVALIDATED = AUDCLNT_ERR(0x004);
pub const AUDCLNT_E_NOT_STOPPED = AUDCLNT_ERR(0x005);
pub const AUDCLNT_E_BUFFER_TOO_LARGE = AUDCLNT_ERR(0x006);
pub const AUDCLNT_E_OUT_OF_ORDER = AUDCLNT_ERR(0x007);
pub const AUDCLNT_E_UNSUPPORTED_FORMAT = AUDCLNT_ERR(0x008);
pub const AUDCLNT_E_INVALID_SIZE = AUDCLNT_ERR(0x009);
pub const AUDCLNT_E_DEVICE_IN_USE = AUDCLNT_ERR(0x00a);
pub const AUDCLNT_E_BUFFER_OPERATION_PENDING = AUDCLNT_ERR(0x00b);
pub const AUDCLNT_E_THREAD_NOT_REGISTERED = AUDCLNT_ERR(0x00c);
pub const AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED = AUDCLNT_ERR(0x00e);
pub const AUDCLNT_E_ENDPOINT_CREATE_FAILED = AUDCLNT_ERR(0x00f);
pub const AUDCLNT_E_SERVICE_NOT_RUNNING = AUDCLNT_ERR(0x010);
pub const AUDCLNT_E_EVENTHANDLE_NOT_EXPECTED = AUDCLNT_ERR(0x011);
pub const AUDCLNT_E_EXCLUSIVE_MODE_ONLY = AUDCLNT_ERR(0x012);
pub const AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL = AUDCLNT_ERR(0x013);
pub const AUDCLNT_E_EVENTHANDLE_NOT_SET = AUDCLNT_ERR(0x014);
pub const AUDCLNT_E_INCORRECT_BUFFER_SIZE = AUDCLNT_ERR(0x015);
pub const AUDCLNT_E_BUFFER_SIZE_ERROR = AUDCLNT_ERR(0x016);
pub const AUDCLNT_E_CPUUSAGE_EXCEEDED = AUDCLNT_ERR(0x017);
pub const AUDCLNT_E_BUFFER_ERROR = AUDCLNT_ERR(0x018);
pub const AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED = AUDCLNT_ERR(0x019);
pub const AUDCLNT_E_INVALID_DEVICE_PERIOD = AUDCLNT_ERR(0x020);

// error set corresponding to the above return codes
pub const Error = error{
    NOT_INITIALIZED,
    ALREADY_INITIALIZED,
    WRONG_ENDPOINT_TYPE,
    DEVICE_INVALIDATED,
    NOT_STOPPED,
    BUFFER_TOO_LARGE,
    OUT_OF_ORDER,
    UNSUPPORTED_FORMAT,
    INVALID_SIZE,
    DEVICE_IN_USE,
    BUFFER_OPERATION_PENDING,
    THREAD_NOT_REGISTERED,
    EXCLUSIVE_MODE_NOT_ALLOWED,
    ENDPOINT_CREATE_FAILED,
    SERVICE_NOT_RUNNING,
    EVENTHANDLE_NOT_EXPECTED,
    EXCLUSIVE_MODE_ONLY,
    BUFDURATION_PERIOD_NOT_EQUAL,
    EVENTHANDLE_NOT_SET,
    INCORRECT_BUFFER_SIZE,
    BUFFER_SIZE_ERROR,
    CPUUSAGE_EXCEEDED,
    BUFFER_ERROR,
    BUFFER_SIZE_NOT_ALIGNED,
    INVALID_DEVICE_PERIOD,
};
