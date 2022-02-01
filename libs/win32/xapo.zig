const std = @import("std");
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const w = @import("windows.zig");
const BYTE = w.BYTE;
const HRESULT = w.HRESULT;
const WINAPI = w.WINAPI;
const UINT32 = w.UINT32;
const BOOL = w.BOOL;
const FALSE = w.FALSE;
const WCHAR = w.WCHAR;
const GUID = w.GUID;
const ULONG = w.ULONG;
const wasapi = @import("wasapi.zig");
const WAVEFORMATEX = wasapi.WAVEFORMATEX;

pub const MIN_CHANNELS: UINT32 = 1;
pub const MAX_CHANNELS: UINT32 = 64;

pub const MIN_FRAMERATE: UINT32 = 1000;
pub const MAX_FRAMERATE: UINT32 = 200000;

pub const REGISTRATION_STRING_LENGTH: UINT32 = 256;

pub const FLAG_CHANNELS_MUST_MATCH: UINT32 = 0x00000001;
pub const FLAG_FRAMERATE_MUST_MATCH: UINT32 = 0x00000002;
pub const FLAG_BITSPERSAMPLE_MUST_MATCH: UINT32 = 0x00000004;
pub const FLAG_BUFFERCOUNT_MUST_MATCH: UINT32 = 0x00000008;
pub const FLAG_INPLACE_REQUIRED: UINT32 = 0x00000020;
pub const FLAG_INPLACE_SUPPORTED: UINT32 = 0x00000010;

pub const REGISTRATION_PROPERTIES = packed struct {
    clsid: GUID,
    FriendlyName: [REGISTRATION_STRING_LENGTH]WCHAR,
    CopyrightInfo: [REGISTRATION_STRING_LENGTH]WCHAR,
    MajorVersion: UINT32,
    MinorVersion: UINT32,
    Flags: UINT32,
    MinInputBufferCount: UINT32,
    MaxInputBufferCount: UINT32,
    MinOutputBufferCount: UINT32,
    MaxOutputBufferCount: UINT32,
};

pub const LOCKFORPROCESS_BUFFER_PARAMETERS = packed struct {
    pFormat: *const WAVEFORMATEX,
    MaxFrameCount: UINT32,
};

pub const BUFFER_FLAGS = enum(UINT32) {
    SILENT,
    VALID,
};

pub const PROCESS_BUFFER_PARAMETERS = packed struct {
    pBuffer: *anyopaque,
    BufferFlags: BUFFER_FLAGS,
    ValidFrameCount: UINT32,
};

pub fn IXAPOVTable(comptime T: type) type {
    return extern struct {
        unknown: w.IUnknown.VTable(T),
        xapo: extern struct {
            GetRegistrationProperties: fn (*T, **REGISTRATION_PROPERTIES) callconv(WINAPI) HRESULT,
            IsInputFormatSupported: fn (
                *T,
                *const WAVEFORMATEX,
                *const WAVEFORMATEX,
                ?**WAVEFORMATEX,
            ) callconv(WINAPI) HRESULT,
            IsOutputFormatSupported: fn (
                *T,
                *const WAVEFORMATEX,
                *const WAVEFORMATEX,
                ?**WAVEFORMATEX,
            ) callconv(WINAPI) HRESULT,
            Initialize: fn (*T, ?*const anyopaque, UINT32) callconv(WINAPI) HRESULT,
            Reset: fn (*T) callconv(WINAPI) void,
            LockForProcess: fn (
                *T,
                UINT32,
                ?[*]const LOCKFORPROCESS_BUFFER_PARAMETERS,
                UINT32,
                ?[*]const LOCKFORPROCESS_BUFFER_PARAMETERS,
            ) callconv(WINAPI) HRESULT,
            UnlockForProcess: fn (*T) callconv(WINAPI) void,
            Process: fn (
                *T,
                UINT32,
                ?[*]const PROCESS_BUFFER_PARAMETERS,
                UINT32,
                ?[*]PROCESS_BUFFER_PARAMETERS,
                BOOL,
            ) callconv(WINAPI) void,
            CalcInputFrames: fn (*T, UINT32) callconv(WINAPI) UINT32,
            CalcOutputFrames: fn (*T, UINT32) callconv(WINAPI) UINT32,
        },
    };
}

pub const IID_IXAPO = GUID.parse("{A410B984-9839-4819-A0BE-2856AE6B3ADB}");
pub const IXAPO = extern struct {
    v: *const IXAPOVTable(Self),

    const Self = @This();
    usingnamespace w.IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn GetRegistrationProperties(self: *T, props: **REGISTRATION_PROPERTIES) HRESULT {
                return self.v.xapo.GetRegistrationProperties(self, props);
            }
            pub inline fn IsInputFormatSupported(
                self: *T,
                output_format: *const WAVEFORMATEX,
                requested_input_format: *const WAVEFORMATEX,
                supported_input_format: ?**WAVEFORMATEX,
            ) HRESULT {
                return self.v.xapo.IsInputFormatSupported(
                    self,
                    output_format,
                    requested_input_format,
                    supported_input_format,
                );
            }
            pub inline fn IsOutputFormatSupported(
                self: *T,
                input_format: *const WAVEFORMATEX,
                requested_output_format: *const WAVEFORMATEX,
                supported_output_format: ?**WAVEFORMATEX,
            ) HRESULT {
                return self.v.xapo.IsOutputFormatSupported(
                    self,
                    input_format,
                    requested_output_format,
                    supported_output_format,
                );
            }
            pub inline fn Initialize(self: *T, data: ?*const anyopaque, data_size: UINT32) HRESULT {
                return self.v.xapo.Initialize(self, data, data_size);
            }
            pub inline fn Reset(self: *T) void {
                self.v.xapo.Reset(self);
            }
            pub inline fn LockForProcess(
                self: *T,
                num_input_params: UINT32,
                input_params: ?[*]const LOCKFORPROCESS_BUFFER_PARAMETERS,
                num_output_params: UINT32,
                output_params: ?[*]const LOCKFORPROCESS_BUFFER_PARAMETERS,
            ) HRESULT {
                return self.v.xapo.LockForProcess(
                    self,
                    num_input_params,
                    input_params,
                    num_output_params,
                    output_params,
                );
            }
            pub inline fn UnlockForProcess(self: *T) void {
                self.v.xapo.UnlockForProcess(self);
            }
            pub inline fn Process(
                self: *T,
                num_input_params: UINT32,
                input_params: ?[*]const PROCESS_BUFFER_PARAMETERS,
                num_output_params: UINT32,
                output_params: ?[*]PROCESS_BUFFER_PARAMETERS,
                is_enabled: BOOL,
            ) void {
                return self.v.xapo.Process(
                    self,
                    num_input_params,
                    input_params,
                    num_output_params,
                    output_params,
                    is_enabled,
                );
            }
            pub inline fn CalcInputFrames(self: *T, num_output_frames: UINT32) UINT32 {
                return self.v.xapo.CalcInputFrames(self, num_output_frames);
            }
            pub inline fn CalcOutputFrames(self: *T, num_input_frames: UINT32) UINT32 {
                return self.v.xapo.CalcOutputFrames(self, num_input_frames);
            }
        };
    }
};

pub fn IXAPOParametersVTable(comptime T: type) type {
    return extern struct {
        unknown: w.IUnknown.VTable(T),
        params: extern struct {
            SetParameters: fn (*T, *const anyopaque, UINT32) callconv(WINAPI) void,
            GetParameters: fn (*T, *anyopaque, UINT32) callconv(WINAPI) void,
        },
    };
}

pub const IXAPOParameters = extern struct {
    v: *const IXAPOParametersVTable(Self),

    const Self = @This();
    usingnamespace w.IUnknown.Methods(Self);
    usingnamespace Methods(Self);

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub inline fn SetParameters(self: *T, params: *const anyopaque, size: UINT32) void {
                self.v.params.SetParameters(self, params, size);
            }
            pub inline fn GetParameters(self: *T, params: *anyopaque, size: UINT32) void {
                self.v.params.GetParameters(self, params, size);
            }
        };
    }
};

const SimpleAudioProcessor = extern struct {
    v: *const IXAPOVTable(SimpleAudioProcessor) = &vtable,
    refcount: u32 = 1,
    process: *const fn ([]f32, ?*anyopaque) void,
    context: ?*anyopaque,

    const Self = @This();

    const vtable = IXAPOVTable(SimpleAudioProcessor){
        .unknown = .{
            .QueryInterface = QueryInterface,
            .AddRef = AddRef,
            .Release = Release,
        },
        .xapo = .{
            .GetRegistrationProperties = GetRegistrationProperties,
            .IsInputFormatSupported = IsInputFormatSupported,
            .IsOutputFormatSupported = IsOutputFormatSupported,
            .Initialize = Initialize,
            .Reset = Reset,
            .LockForProcess = LockForProcess,
            .UnlockForProcess = UnlockForProcess,
            .Process = Process,
            .CalcInputFrames = CalcInputFrames,
            .CalcOutputFrames = CalcOutputFrames,
        },
    };

    const info = REGISTRATION_PROPERTIES{
        .clsid = w.GUID_NULL,
        .FriendlyName = [_]WCHAR{0} ** REGISTRATION_STRING_LENGTH,
        .CopyrightInfo = [_]WCHAR{0} ** REGISTRATION_STRING_LENGTH,
        .MajorVersion = 1,
        .MinorVersion = 0,
        .Flags = FLAG_CHANNELS_MUST_MATCH |
            FLAG_FRAMERATE_MUST_MATCH |
            FLAG_BITSPERSAMPLE_MUST_MATCH |
            FLAG_BUFFERCOUNT_MUST_MATCH |
            FLAG_INPLACE_SUPPORTED |
            FLAG_INPLACE_REQUIRED,
        .MinInputBufferCount = 1,
        .MaxInputBufferCount = 1,
        .MinOutputBufferCount = 1,
        .MaxOutputBufferCount = 1,
    };

    fn QueryInterface(
        self: *Self,
        guid: *const GUID,
        outobj: ?*?*anyopaque,
    ) callconv(WINAPI) HRESULT {
        assert(outobj != null);

        if (std.mem.eql(u8, std.mem.asBytes(guid), std.mem.asBytes(&w.IID_IUnknown))) {
            outobj.?.* = self;
            _ = self.AddRef();
            return w.S_OK;
        } else if (std.mem.eql(u8, std.mem.asBytes(guid), std.mem.asBytes(&IID_IXAPO))) {
            outobj.?.* = self;
            _ = self.AddRef();
            return w.S_OK;
        }

        outobj.?.* = null;
        return w.E_NOINTERFACE;
    }

    fn AddRef(self: *Self) callconv(WINAPI) ULONG {
        return @atomicRmw(u32, &self.refcount, .Add, 1, .Monotonic) + 1;
    }

    fn Release(self: *Self) callconv(WINAPI) ULONG {
        const prev_refcount = @atomicRmw(u32, &self.refcount, .Sub, 1, .Monotonic);
        if (prev_refcount == 1) {
            w.ole32.CoTaskMemFree(self);
        }
        return prev_refcount - 1;
    }

    fn GetRegistrationProperties(
        _: *Self,
        props: **REGISTRATION_PROPERTIES,
    ) callconv(WINAPI) HRESULT {
        const ptr = w.CoTaskMemAlloc(@sizeOf(REGISTRATION_PROPERTIES));
        if (ptr != null) {
            props.* = @ptrCast(*REGISTRATION_PROPERTIES, @alignCast(8, ptr.?));
            props.*.* = info;
            return w.S_OK;
        }
        return w.E_FAIL;
    }

    fn IsInputFormatSupported(
        _: *Self,
        _: *const WAVEFORMATEX,
        requested_input_format: *const WAVEFORMATEX,
        supported_input_format: ?**WAVEFORMATEX,
    ) callconv(WINAPI) HRESULT {
        if (requested_input_format.wFormatTag != wasapi.WAVE_FORMAT_IEEE_FLOAT or
            requested_input_format.nChannels != 2 or
            requested_input_format.nSamplesPerSec < MIN_FRAMERATE or
            requested_input_format.nSamplesPerSec > MAX_FRAMERATE or
            requested_input_format.wBitsPerSample != 32)
        {
            if (supported_input_format != null) {
                supported_input_format.?.*.wFormatTag = wasapi.WAVE_FORMAT_IEEE_FLOAT;
                supported_input_format.?.*.nChannels = 2;
                supported_input_format.?.*.nSamplesPerSec = 48_000;
                supported_input_format.?.*.wBitsPerSample = 32;
            }
            return w.XAPO_E_FORMAT_UNSUPPORTED;
        }
        return w.S_OK;
    }

    fn IsOutputFormatSupported(
        _: *Self,
        _: *const WAVEFORMATEX,
        requested_output_format: *const WAVEFORMATEX,
        supported_output_format: ?**WAVEFORMATEX,
    ) callconv(WINAPI) HRESULT {
        if (requested_output_format.wFormatTag != wasapi.WAVE_FORMAT_IEEE_FLOAT or
            requested_output_format.nChannels != 2 or
            requested_output_format.nSamplesPerSec < MIN_FRAMERATE or
            requested_output_format.nSamplesPerSec > MAX_FRAMERATE or
            requested_output_format.wBitsPerSample != 32)
        {
            if (supported_output_format != null) {
                supported_output_format.?.*.wFormatTag = wasapi.WAVE_FORMAT_IEEE_FLOAT;
                supported_output_format.?.*.nChannels = 2;
                supported_output_format.?.*.nSamplesPerSec = 48_000;
                supported_output_format.?.*.wBitsPerSample = 32;
            }
            return w.XAPO_E_FORMAT_UNSUPPORTED;
        }
        return w.S_OK;
    }

    fn Initialize(_: *Self, data: ?*const anyopaque, data_size: UINT32) callconv(WINAPI) w.HRESULT {
        _ = data;
        _ = data_size;
        return w.S_OK;
    }

    fn Reset(_: *Self) callconv(WINAPI) void {}

    fn LockForProcess(
        _: *Self,
        num_input_params: UINT32,
        input_params: ?[*]const LOCKFORPROCESS_BUFFER_PARAMETERS,
        num_output_params: UINT32,
        output_params: ?[*]const LOCKFORPROCESS_BUFFER_PARAMETERS,
    ) callconv(WINAPI) HRESULT {
        assert(num_input_params == 1 and num_output_params == 1);
        assert(input_params != null and output_params != null);
        assert(input_params.?[0].pFormat.wFormatTag == output_params.?[0].pFormat.wFormatTag);
        assert(input_params.?[0].pFormat.nChannels == output_params.?[0].pFormat.nChannels);
        assert(input_params.?[0].pFormat.nSamplesPerSec == output_params.?[0].pFormat.nSamplesPerSec);
        assert(input_params.?[0].pFormat.wBitsPerSample == output_params.?[0].pFormat.wBitsPerSample);
        assert(input_params.?[0].pFormat.nChannels == 2);
        assert(input_params.?[0].pFormat.wBitsPerSample == 32);
        return w.S_OK;
    }

    fn UnlockForProcess(_: *Self) callconv(WINAPI) void {}

    fn Process(
        self: *Self,
        num_input_params: UINT32,
        input_params: ?[*]const PROCESS_BUFFER_PARAMETERS,
        num_output_params: UINT32,
        output_params: ?[*]PROCESS_BUFFER_PARAMETERS,
        is_enabled: BOOL,
    ) callconv(WINAPI) void {
        assert(num_input_params == 1 and num_output_params == 1);
        assert(input_params != null and output_params != null);
        assert(input_params.?[0].pBuffer == output_params.?[0].pBuffer);

        if (input_params.?[0].BufferFlags == .VALID and is_enabled == w.TRUE) {
            var samples = @ptrCast([*]f32, @alignCast(16, input_params.?[0].pBuffer)); // XAudio2 aligns data to 16.
            const num_samples = input_params.?[0].ValidFrameCount * 2; // We support 2 channels only.

            self.process.*(samples[0..num_samples], self.context);
        }

        output_params.?[0].ValidFrameCount = input_params.?[0].ValidFrameCount;
        output_params.?[0].BufferFlags = input_params.?[0].BufferFlags;
    }

    fn CalcInputFrames(_: *Self, num_output_frames: UINT32) callconv(WINAPI) UINT32 {
        return num_output_frames;
    }

    fn CalcOutputFrames(_: *Self, num_input_frames: UINT32) callconv(WINAPI) UINT32 {
        return num_input_frames;
    }
};

pub fn createSimpleProcessor(
    process: *const fn ([]f32, ?*anyopaque) void,
    context: ?*anyopaque,
) *w.IUnknown {
    const ptr = w.CoTaskMemAlloc(@sizeOf(SimpleAudioProcessor)).?;
    const comptr = @ptrCast(*SimpleAudioProcessor, @alignCast(8, ptr));
    comptr.* = .{
        .process = process,
        .context = context,
    };
    return @ptrCast(*w.IUnknown, comptr);
}
