const std = @import("std");
const assert = std.debug.assert;
const w32 = @import("w32.zig");
const BYTE = w32.BYTE;
const HRESULT = w32.HRESULT;
const WINAPI = w32.WINAPI;
const UINT32 = w32.UINT32;
const BOOL = w32.BOOL;
const FALSE = w32.FALSE;
const WCHAR = w32.WCHAR;
const GUID = w32.GUID;
const ULONG = w32.ULONG;
const IUnknown = w32.IUnknown;
const wasapi = @import("wasapi.zig");
const WAVEFORMATEX = wasapi.WAVEFORMATEX;

pub const MIN_CHANNELS = 1;
pub const MAX_CHANNELS = 64;

pub const MIN_FRAMERATE = 1000;
pub const MAX_FRAMERATE = 200000;

pub const REGISTRATION_STRING_LENGTH = 256;

pub const FLAGS = packed struct(UINT32) {
    CHANNELS_MUST_MATCH: bool = false,
    FRAMERATE_MUST_MATCH: bool = false,
    BITSPERSAMPLE_MUST_MATCH: bool = false,
    BUFFERCOUNT_MUST_MATCH: bool = false,
    INPLACE_SUPPORTED: bool = false,
    INPLACE_REQUIRED: bool = false,
    __unused: u26 = 0,
};

pub const REGISTRATION_PROPERTIES = extern struct {
    clsid: GUID align(1),
    FriendlyName: [REGISTRATION_STRING_LENGTH]WCHAR align(1),
    CopyrightInfo: [REGISTRATION_STRING_LENGTH]WCHAR align(1),
    MajorVersion: UINT32 align(1),
    MinorVersion: UINT32 align(1),
    Flags: FLAGS align(1),
    MinInputBufferCount: UINT32 align(1),
    MaxInputBufferCount: UINT32 align(1),
    MinOutputBufferCount: UINT32 align(1),
    MaxOutputBufferCount: UINT32 align(1),
};

pub const LOCKFORPROCESS_BUFFER_PARAMETERS = extern struct {
    pFormat: *const WAVEFORMATEX align(1),
    MaxFrameCount: UINT32 align(1),
};

pub const BUFFER_FLAGS = enum(UINT32) {
    SILENT,
    VALID,
};

pub const PROCESS_BUFFER_PARAMETERS = extern struct {
    pBuffer: *anyopaque align(1),
    BufferFlags: BUFFER_FLAGS align(1),
    ValidFrameCount: UINT32 align(1),
};

pub const IID_IXAPO = GUID.parse("{A410B984-9839-4819-A0BE-2856AE6B3ADB}");
pub const IXAPO = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IUnknown.Methods(T);

            pub inline fn GetRegistrationProperties(self: *T, props: **REGISTRATION_PROPERTIES) HRESULT {
                return @ptrCast(*const IXAPO.VTable, self.__v)
                    .GetRegistrationProperties(@ptrCast(*IXAPO, self), props);
            }
            pub inline fn IsInputFormatSupported(
                self: *T,
                output_format: *const WAVEFORMATEX,
                requested_input_format: *const WAVEFORMATEX,
                supported_input_format: ?**WAVEFORMATEX,
            ) HRESULT {
                return @ptrCast(*const IXAPO.VTable, self.__v).IsInputFormatSupported(
                    @ptrCast(*IXAPO, self),
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
                return @ptrCast(*const IXAPO.VTable, self.__v).IsOutputFormatSupported(
                    @ptrCast(*IXAPO, self),
                    input_format,
                    requested_output_format,
                    supported_output_format,
                );
            }
            pub inline fn Initialize(self: *T, data: ?*const anyopaque, data_size: UINT32) HRESULT {
                return @ptrCast(*const IXAPO.VTable, self.__v)
                    .Initialize(@ptrCast(*IXAPO, self), data, data_size);
            }
            pub inline fn Reset(self: *T) void {
                @ptrCast(*const IXAPO.VTable, self.__v).Reset(@ptrCast(*IXAPO, self));
            }
            pub inline fn LockForProcess(
                self: *T,
                num_input_params: UINT32,
                input_params: ?[*]const LOCKFORPROCESS_BUFFER_PARAMETERS,
                num_output_params: UINT32,
                output_params: ?[*]const LOCKFORPROCESS_BUFFER_PARAMETERS,
            ) HRESULT {
                return @ptrCast(*const IXAPO.VTable, self.__v).LockForProcess(
                    @ptrCast(*IXAPO, self),
                    num_input_params,
                    input_params,
                    num_output_params,
                    output_params,
                );
            }
            pub inline fn UnlockForProcess(self: *T) void {
                @ptrCast(*const IXAPO.VTable, self.__v).UnlockForProcess(@ptrCast(*IXAPO, self));
            }
            pub inline fn Process(
                self: *T,
                num_input_params: UINT32,
                input_params: ?[*]const PROCESS_BUFFER_PARAMETERS,
                num_output_params: UINT32,
                output_params: ?[*]PROCESS_BUFFER_PARAMETERS,
                is_enabled: BOOL,
            ) void {
                return @ptrCast(*const IXAPO.VTable, self.__v).Process(
                    @ptrCast(*IXAPO, self),
                    num_input_params,
                    input_params,
                    num_output_params,
                    output_params,
                    is_enabled,
                );
            }
            pub inline fn CalcInputFrames(self: *T, num_output_frames: UINT32) UINT32 {
                return @ptrCast(*const IXAPO.VTable, self.__v)
                    .CalcInputFrames(@ptrCast(*IXAPO, self), num_output_frames);
            }
            pub inline fn CalcOutputFrames(self: *T, num_input_frames: UINT32) UINT32 {
                return @ptrCast(*const IXAPO.VTable, self.__v)
                    .CalcOutputFrames(@ptrCast(*IXAPO, self), num_input_frames);
            }
        };
    }

    pub const VTable = extern struct {
        const T = IXAPO;
        base: IUnknown.VTable,
        GetRegistrationProperties: *const fn (*T, **REGISTRATION_PROPERTIES) callconv(WINAPI) HRESULT,
        IsInputFormatSupported: *const fn (
            *T,
            *const WAVEFORMATEX,
            *const WAVEFORMATEX,
            ?**WAVEFORMATEX,
        ) callconv(WINAPI) HRESULT,
        IsOutputFormatSupported: *const fn (
            *T,
            *const WAVEFORMATEX,
            *const WAVEFORMATEX,
            ?**WAVEFORMATEX,
        ) callconv(WINAPI) HRESULT,
        Initialize: *const fn (*T, ?*const anyopaque, UINT32) callconv(WINAPI) HRESULT,
        Reset: *const fn (*T) callconv(WINAPI) void,
        LockForProcess: *const fn (
            *T,
            UINT32,
            ?[*]const LOCKFORPROCESS_BUFFER_PARAMETERS,
            UINT32,
            ?[*]const LOCKFORPROCESS_BUFFER_PARAMETERS,
        ) callconv(WINAPI) HRESULT,
        UnlockForProcess: *const fn (*T) callconv(WINAPI) void,
        Process: *const fn (
            *T,
            UINT32,
            ?[*]const PROCESS_BUFFER_PARAMETERS,
            UINT32,
            ?[*]PROCESS_BUFFER_PARAMETERS,
            BOOL,
        ) callconv(WINAPI) void,
        CalcInputFrames: *const fn (*T, UINT32) callconv(WINAPI) UINT32,
        CalcOutputFrames: *const fn (*T, UINT32) callconv(WINAPI) UINT32,
    };
};

pub const IXAPOParameters = extern struct {
    __v: *const VTable,

    pub usingnamespace Methods(@This());

    pub fn Methods(comptime T: type) type {
        return extern struct {
            pub usingnamespace IUnknown.Methods(T);

            pub inline fn SetParameters(self: *T, params: *const anyopaque, size: UINT32) void {
                @ptrCast(*const IXAPOParameters.VTable, self.__v)
                    .SetParameters(@ptrCast(*IXAPOParameters, self), params, size);
            }
            pub inline fn GetParameters(self: *T, params: *anyopaque, size: UINT32) void {
                @ptrCast(*const IXAPOParameters.VTable, self.__v)
                    .GetParameters(@ptrCast(*IXAPOParameters, self), params, size);
            }
        };
    }

    const VTable = extern struct {
        base: IUnknown.VTable,
        SetParameters: *const fn (*IXAPOParameters, *const anyopaque, UINT32) callconv(WINAPI) void,
        GetParameters: *const fn (*IXAPOParameters, *anyopaque, UINT32) callconv(WINAPI) void,
    };
};

pub const E_FORMAT_UNSUPPORTED = @bitCast(HRESULT, @as(c_ulong, 0x88970001));

pub const Error = error{
    E_FORMAT_UNSUPPORTED,
};
