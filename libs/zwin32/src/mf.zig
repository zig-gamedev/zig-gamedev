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
const BYTE = windows.BYTE;
const LONGLONG = windows.LONGLONG;
const PROPVARIANT = windows.PROPVARIANT;

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

pub const SOURCE_READERF_ERROR: DWORD = 0x1;
pub const SOURCE_READERF_ENDOFSTREAM: DWORD = 0x2;
pub const SOURCE_READERF_NEWSTREAM: DWORD = 0x4;
pub const SOURCE_READERF_NATIVEMEDIATYPECHANGED: DWORD = 0x10;
pub const SOURCE_READERF_CURRENTMEDIATYPECHANGED: DWORD = 0x20;
pub const SOURCE_READERF_STREAMTICK: DWORD = 0x100;
pub const SOURCE_READERF_ALLEFFECTSREMOVED: DWORD = 0x200;

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
            pub inline fn GetUINT32(self: *T, guid: *const GUID, value: *UINT32) HRESULT {
                return self.v.attribs.GetUINT32(self, guid, value);
            }
            pub inline fn GetGUID(self: *T, key: *const GUID, value: *GUID) HRESULT {
                return self.v.attribs.GetGUID(self, key, value);
            }
            pub inline fn SetUINT32(self: *T, guid: *const GUID, value: UINT32) HRESULT {
                return self.v.attribs.SetUINT32(self, guid, value);
            }
            pub inline fn SetGUID(self: *T, key: *const GUID, value: *const GUID) HRESULT {
                return self.v.attribs.SetGUID(self, key, value);
            }
            pub inline fn SetUnknown(self: *T, guid: *const GUID, unknown: ?*IUnknown) HRESULT {
                return self.v.attribs.SetUnknown(self, guid, unknown);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetItem: *anyopaque,
            GetItemType: *anyopaque,
            CompareItem: *anyopaque,
            Compare: *anyopaque,
            GetUINT32: fn (*T, *const GUID, *UINT32) callconv(WINAPI) HRESULT,
            GetUINT64: *anyopaque,
            GetDouble: *anyopaque,
            GetGUID: fn (*T, *const GUID, *GUID) callconv(WINAPI) HRESULT,
            GetStringLength: *anyopaque,
            GetString: *anyopaque,
            GetAllocatedString: *anyopaque,
            GetBlobSize: *anyopaque,
            GetBlob: *anyopaque,
            GetAllocatedBlob: *anyopaque,
            GetUnknown: *anyopaque,
            SetItem: *anyopaque,
            DeleteItem: *anyopaque,
            DeleteAllItems: *anyopaque,
            SetUINT32: fn (*T, *const GUID, UINT32) callconv(WINAPI) HRESULT,
            SetUINT64: *anyopaque,
            SetDouble: *anyopaque,
            SetGUID: fn (*T, *const GUID, *const GUID) callconv(WINAPI) HRESULT,
            SetString: *anyopaque,
            SetBlob: *anyopaque,
            SetUnknown: fn (*T, *const GUID, ?*IUnknown) callconv(WINAPI) HRESULT,
            LockStore: *anyopaque,
            UnlockStore: *anyopaque,
            GetCount: *anyopaque,
            GetItemByIndex: *anyopaque,
            CopyAllItems: *anyopaque,
        };
    }
};

pub const IMediaEvent = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        attribs: IAttributes.VTable(Self),
        event: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IAttributes.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetType(self: *T, met: *MediaEventType) HRESULT {
                return self.v.event.GetType(self, met);
            }
            pub inline fn GetExtendedType(self: *T, ex_met: *GUID) HRESULT {
                return self.v.event.GetExtendedType(self, ex_met);
            }
            pub inline fn GetStatus(self: *T, status: *HRESULT) HRESULT {
                return self.v.event.GetStatus(self, status);
            }
            pub inline fn GetValue(self: *T, value: *PROPVARIANT) HRESULT {
                return self.v.event.GetValue(self, value);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetType: fn (*T, *MediaEventType) callconv(WINAPI) HRESULT,
            GetExtendedType: fn (*T, *GUID) callconv(WINAPI) HRESULT,
            GetStatus: fn (*T, *HRESULT) callconv(WINAPI) HRESULT,
            GetValue: fn (*T, *PROPVARIANT) callconv(WINAPI) HRESULT,
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
            pub inline fn GetCurrentMediaType(
                self: *T,
                stream_index: DWORD,
                media_type: **IMediaType,
            ) HRESULT {
                return self.v.reader.GetCurrentMediaType(self, stream_index, media_type);
            }
            pub inline fn SetCurrentMediaType(
                self: *T,
                stream_index: DWORD,
                reserved: ?*DWORD,
                media_type: *IMediaType,
            ) HRESULT {
                return self.v.reader.SetCurrentMediaType(self, stream_index, reserved, media_type);
            }
            pub inline fn ReadSample(
                self: *T,
                stream_index: DWORD,
                control_flags: DWORD,
                actual_stream_index: ?*DWORD,
                stream_flags: ?*DWORD,
                timestamp: ?*LONGLONG,
                sample: ?*?*ISample,
            ) HRESULT {
                return self.v.reader.ReadSample(
                    self,
                    stream_index,
                    control_flags,
                    actual_stream_index,
                    stream_flags,
                    timestamp,
                    sample,
                );
            }
            pub inline fn SetCurrentPosition(self: *T, guid: *const GUID, prop: *const PROPVARIANT) HRESULT {
                return self.v.reader.SetCurrentPosition(self, guid, prop);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetStreamSelection: *anyopaque,
            SetStreamSelection: *anyopaque,
            GetNativeMediaType: fn (*T, DWORD, DWORD, **IMediaType) callconv(WINAPI) HRESULT,
            GetCurrentMediaType: fn (*T, DWORD, **IMediaType) callconv(WINAPI) HRESULT,
            SetCurrentMediaType: fn (*T, DWORD, ?*DWORD, *IMediaType) callconv(WINAPI) HRESULT,
            SetCurrentPosition: fn (*T, *const GUID, *const PROPVARIANT) callconv(WINAPI) HRESULT,
            ReadSample: fn (*T, DWORD, DWORD, ?*DWORD, ?*DWORD, ?*LONGLONG, ?*?*ISample) callconv(WINAPI) HRESULT,
            Flush: *anyopaque,
            GetServiceForStream: *anyopaque,
            GetPresentationAttribute: *anyopaque,
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
            GetMajorType: *anyopaque,
            IsCompressedFormat: *anyopaque,
            IsEqual: *anyopaque,
            GetRepresentation: *anyopaque,
            FreeRepresentation: *anyopaque,
        };
    }
};

pub const ISample = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        attribs: IAttributes.VTable(Self),
        sample: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace IAttributes.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn ConvertToContiguousBuffer(self: *T, buffer: **IMediaBuffer) HRESULT {
                return self.v.sample.ConvertToContiguousBuffer(self, buffer);
            }
            pub inline fn GetBufferByIndex(self: *T, index: DWORD, buffer: **IMediaBuffer) HRESULT {
                return self.v.sample.GetBufferByIndex(self, index, buffer);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            GetSampleFlags: *anyopaque,
            SetSampleFlags: *anyopaque,
            GetSampleTime: *anyopaque,
            SetSampleTime: *anyopaque,
            GetSampleDuration: *anyopaque,
            SetSampleDuration: *anyopaque,
            GetBufferCount: *anyopaque,
            GetBufferByIndex: fn (*T, DWORD, **IMediaBuffer) callconv(WINAPI) HRESULT,
            ConvertToContiguousBuffer: fn (*T, **IMediaBuffer) callconv(WINAPI) HRESULT,
            AddBuffer: *anyopaque,
            RemoveBufferByIndex: *anyopaque,
            RemoveAllBuffers: *anyopaque,
            GetTotalLength: *anyopaque,
            CopyToBuffer: *anyopaque,
        };
    }
};

pub const IMediaBuffer = extern struct {
    const Self = @This();
    v: *const extern struct {
        unknown: IUnknown.VTable(Self),
        buffer: VTable(Self),
    },
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn Lock(self: *T, ptr: *[*]BYTE, max_len: ?*DWORD, current_len: ?*DWORD) HRESULT {
                return self.v.buffer.Lock(self, ptr, max_len, current_len);
            }
            pub inline fn Unlock(self: *T) HRESULT {
                return self.v.buffer.Unlock(self);
            }
            pub inline fn GetCurrentLength(self: *T, length: *DWORD) HRESULT {
                return self.v.buffer.GetCurrentLength(self, length);
            }
        };
    }

    fn VTable(comptime T: type) type {
        return extern struct {
            Lock: fn (*T, *[*]BYTE, ?*DWORD, ?*DWORD) callconv(WINAPI) HRESULT,
            Unlock: fn (*T) callconv(WINAPI) HRESULT,
            GetCurrentLength: fn (*T, *DWORD) callconv(WINAPI) HRESULT,
            SetCurrentLength: *anyopaque,
            GetMaxLength: *anyopaque,
        };
    }
};

pub const MediaEventType = DWORD;

pub fn ISourceReaderCallbackVTable(comptime T: type) type {
    return extern struct {
        unknown: IUnknown.VTable(T),
        cb: extern struct {
            OnReadSample: fn (*T, HRESULT, DWORD, DWORD, LONGLONG, ?*ISample) callconv(WINAPI) HRESULT,
            OnFlush: fn (*T, DWORD) callconv(WINAPI) HRESULT,
            OnEvent: fn (*T, DWORD, *IMediaEvent) callconv(WINAPI) HRESULT,
        },
    };
}

pub const IID_ISourceReaderCallback = GUID.parse("{deec8d99-fa1d-4d82-84c2-2c8969944867}");
pub const ISourceReaderCallback = extern struct {
    v: *const ISourceReaderCallbackVTable(Self),

    const Self = @This();
    usingnamespace IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn OnReadSample(
                self: *T,
                status: HRESULT,
                stream_index: DWORD,
                stream_flags: DWORD,
                timestamp: LONGLONG,
                sample: ?*ISample,
            ) HRESULT {
                return self.v.cb.OnReadSample(self, status, stream_index, stream_flags, timestamp, sample);
            }
            pub inline fn OnFlush(self: *T, stream_index: DWORD) HRESULT {
                return self.v.cb.OnFlush(self, stream_index);
            }
            pub inline fn OnEvent(self: *T, stream_index: DWORD, event: *IMediaEvent) HRESULT {
                return self.v.cb.OnEvent(self, stream_index, event);
            }
        };
    }
};

pub const LOW_LATENCY = GUID.parse("{9C27891A-ED7A-40e1-88E8-B22727A024EE}"); // {UINT32 (BOOL)}
pub const SOURCE_READER_ASYNC_CALLBACK = GUID.parse("{1e3dbeac-bb43-4c35-b507-cd644464c965}"); // {*IUnknown}

pub const MT_MAJOR_TYPE = GUID.parse("{48eba18e-f8c9-4687-bf11-0a74c9f96a8f}"); // {GUID}
pub const MT_SUBTYPE = GUID.parse("{f7e34c9a-42e8-4714-b74b-cb29d72c35e5}"); // {GUID}

pub const MT_AUDIO_BITS_PER_SAMPLE = GUID.parse("{f2deb57f-40fa-4764-aa33-ed4f2d1ff669}"); // {UINT32}
pub const MT_AUDIO_SAMPLES_PER_SECOND = GUID.parse("{5faeeae7-0290-4c31-9e8a-c534f68d9dba}"); // {UINT32}
pub const MT_AUDIO_NUM_CHANNELS = GUID.parse("{37e48bf5-645e-4c5b-89de-ada9e29b696a}"); // {UINT32}
pub const MT_AUDIO_BLOCK_ALIGNMENT = GUID.parse("{322de230-9eeb-43bd-ab7a-ff412251541d}"); // {UINT32}
pub const MT_AUDIO_AVG_BYTES_PER_SECOND = GUID.parse("{1aab75c8-cfef-451c-ab95-ac034b8e1731}"); // {UINT32}
pub const MT_ALL_SAMPLES_INDEPENDENT = GUID.parse("{c9173739-5e56-461c-b713-46fb995cb95f}"); // {UINT32 (BOOL)}

pub const AudioFormat_Base = GUID.parse("{00000000-0000-0010-8000-00aa00389b71}");
pub const AudioFormat_PCM = GUID.parse("{00000001-0000-0010-8000-00aa00389b71}");
pub const AudioFormat_Float = GUID.parse("{00000003-0000-0010-8000-00aa00389b71}");

pub const MediaType_Audio = GUID.parse("{73647561-0000-0010-8000-00aa00389b71}");
