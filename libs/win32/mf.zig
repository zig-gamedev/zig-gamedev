const windows = @import("windows.zig");
const IUnknown = windows.IUnknown;
const UINT = windows.UINT;
const UINT32 = windows.UINT32;
const HRESULT = windows.HRESULT;
const ULONG = windows.ULONG;
const DWORD = windows.DWORD;
const WINAPI = windows.WINAPI;
const GUID = windows.GUID;
const LPCWSTR = windows.LPCWSTR;

// 0x0002 for Windows 7+
pub const SDK_VERSION: UINT = 0x0002;
pub const API_VERSION: UINT = 0x0070;
pub const VERSION: UINT = (SDK_VERSION << 16 | API_VERSION);

pub const SOURCE_READER_INVALID_STREAM_INDEX: DWORD = 0xffffffff;
pub const SOURCE_READER_ALL_STREAMS: DWORD = 0xfffffffe;
pub const SOURCE_READER_ANY_STREAM: DWORD = 0xfffffffe;
pub const SOURCE_READER_FIRST_AUDIO_STREAM: DWORD = 0xfffffffd;
pub const SOURCE_READER_FIRST_VIDEO_STREAM: DWORD = 0xfffffffc;
pub const SOURCE_READER_MEDIASOURCE: DWORD = 0xffffffff;

pub extern "mfplat" fn MFStartup(version: ULONG, flags: DWORD) callconv(WINAPI) HRESULT;
pub extern "mfplat" fn MFShutdown() callconv(WINAPI) HRESULT;
pub extern "mfplat" fn MFCreateAttributes(attribs: **IAttributes, init_size: UINT32) callconv(WINAPI) HRESULT;

pub extern "mfreadwrite" fn MFCreateSourceReaderFromURL(
    url: LPCWSTR,
    attribs: ?*IAttributes,
    reader: **ISourceReader,
) callconv(WINAPI) HRESULT;

pub const IAttributes = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        attribs: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetGUID(self: *T, key: *const GUID, value: *GUID) HRESULT {
                return self.v.attribs.GetGUID(self, key, value);
            }
            pub inline fn SetUINT32(self: *T, guid: *const GUID, value: UINT32) HRESULT {
                return self.v.attribs.SetUINT32(self, guid, value);
            }
            pub inline fn SetGUID(self: *T, key: *const GUID, value: *const GUID) HRESULT {
                return self.v.attribs.SetGUID(self, key, value);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetItem: *c_void,
            GetItemType: *c_void,
            CompareItem: *c_void,
            Compare: *c_void,
            GetUINT32: *c_void,
            GetUINT64: *c_void,
            GetDouble: *c_void,
            GetGUID: fn (*T, *const GUID, *GUID) callconv(WINAPI) HRESULT,
            GetStringLength: *c_void,
            GetString: *c_void,
            GetAllocatedString: *c_void,
            GetBlobSize: *c_void,
            GetBlob: *c_void,
            GetAllocatedBlob: *c_void,
            GetUnknown: *c_void,
            SetItem: *c_void,
            DeleteItem: *c_void,
            DeleteAllItems: *c_void,
            SetUINT32: fn (*T, *const GUID, UINT32) callconv(WINAPI) HRESULT,
            SetUINT64: *c_void,
            SetDouble: *c_void,
            SetGUID: fn (*T, *const GUID, *const GUID) callconv(WINAPI) HRESULT,
            SetString: *c_void,
            SetBlob: *c_void,
            SetUnknown: *c_void,
            LockStore: *c_void,
            UnlockStore: *c_void,
            GetCount: *c_void,
            GetItemByIndex: *c_void,
            CopyAllItems: *c_void,
        };
    }
};

pub const ISourceReader = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        reader: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetNativeMediaType(
                self: *T,
                stream_index: DWORD,
                media_type_index: DWORD,
                media_type: **IMediaType,
            ) HRESULT {
                return self.v.reader.GetNativeMediaType(self, stream_index, media_type_index, media_type);
            }
            pub inline fn SetCurrentMediaType(
                self: *T,
                stream_index: DWORD,
                reserved: ?*DWORD,
                media_type: *IMediaType,
            ) HRESULT {
                return self.v.reader.SetCurrentMediaType(self, stream_index, reserved, media_type);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetStreamSelection: *c_void,
            SetStreamSelection: *c_void,
            GetNativeMediaType: fn (*T, DWORD, DWORD, **IMediaType) callconv(WINAPI) HRESULT,
            GetCurrentMediaType: *c_void,
            SetCurrentMediaType: fn (*T, DWORD, ?*DWORD, *IMediaType) callconv(WINAPI) HRESULT,
            SetCurrentPosition: *c_void,
            ReadSample: *c_void,
            Flush: *c_void,
            GetServiceForStream: *c_void,
            GetPresentationAttribute: *c_void,
        };
    }
};

pub const IMediaType = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        attribs: IAttributes.VTable(Self),
        mediatype: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IAttributes.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        _ = T;
        return extern struct {};
    }

    fn VTable(comptime T: type) type {
        _ = T;
        return extern struct {
            GetMajorType: *c_void,
            IsCompressedFormat: *c_void,
            IsEqual: *c_void,
            GetRepresentation: *c_void,
            FreeRepresentation: *c_void,
        };
    }
};

pub const LOW_LATENCY = GUID.parse("{9C27891A-ED7A-40e1-88E8-B22727A024EE}"); // {UINT32 (BOOL)}

pub const MT_MAJOR_TYPE = GUID.parse("{48eba18e-f8c9-4687-bf11-0a74c9f96a8f}"); // {GUID}
pub const MT_SUBTYPE = GUID.parse("{f7e34c9a-42e8-4714-b74b-cb29d72c35e5}"); // {GUID}

pub const AudioFormat_Base = GUID.parse("{00000000-0000-0010-8000-00aa00389b71}");
pub const AudioFormat_PCM = GUID.parse("{00000001-0000-0010-8000-00aa00389b71}");
pub const AudioFormat_Float = GUID.parse("{00000003-0000-0010-8000-00aa00389b71}");

pub const MediaType_Audio = GUID.parse("{73647561-0000-0010-8000-00AA00389B71}");
