pub const base = @import("windows.zig");
pub const dwrite = @import("dwrite.zig");
pub const dxgi = @import("dxgi.zig");
pub const d3d11 = @import("d3d11.zig");
pub const d3d11d = @import("d3d11sdklayers.zig");
pub const d3d12 = @import("d3d12.zig");
pub const d3d12d = @import("d3d12sdklayers.zig");
pub const d3d = @import("d3dcommon.zig");
pub const d2d1 = @import("d2d1.zig");
pub const d3d11on12 = @import("d3d11on12.zig");
pub const wic = @import("wincodec.zig");
pub const wasapi = @import("wasapi.zig");
pub const directml = @import("directml.zig");
pub const mf = @import("mf.zig");
pub const xaudio2 = @import("xaudio2.zig");
pub const xaudio2fx = @import("xaudio2fx.zig");
pub const xapo = @import("xapo.zig");

/// Disclaimer: You should probably precompile your shaders with dxc and not use d3dcompiler!
pub const d3dcompiler = @import("d3dcompiler.zig");

const HRESULT = base.HRESULT;
const S_OK = base.S_OK;

const std = @import("std");
const panic = std.debug.panic;
const assert = std.debug.assert;

// TODO: Handle more error codes from https://docs.microsoft.com/en-us/windows/win32/com/com-error-codes-10
pub const HResultError =
    base.Error || dxgi.Error || d3d12.Error || d3d11.Error || wasapi.Error || dwrite.Error || xapo.Error || base.MiscError;

pub fn hrPanic(err: HResultError) noreturn {
    panic(
        "HRESULT error detected (0x{x}, {}).",
        .{ @bitCast(c_ulong, errorToHRESULT(err)), err },
    );
}

pub inline fn hrPanicOnFail(hr: HRESULT) void {
    if (hr != S_OK) {
        hrPanic(hrToError(hr));
    }
}

pub inline fn hrErrorOnFail(hr: HRESULT) HResultError!void {
    if (hr != S_OK) {
        return hrToError(hr);
    }
}

pub fn hrToError(hr: HRESULT) HResultError {
    assert(hr != S_OK);
    return switch (hr) {
        //
        base.E_UNEXPECTED => base.Error.UNEXPECTED,
        base.E_NOTIMPL => base.Error.NOTIMPL,
        base.E_OUTOFMEMORY => base.Error.OUTOFMEMORY,
        base.E_INVALIDARG => base.Error.INVALIDARG,
        base.E_POINTER => base.Error.POINTER,
        base.E_HANDLE => base.Error.HANDLE,
        base.E_ABORT => base.Error.ABORT,
        base.E_FAIL => base.Error.FAIL,
        base.E_ACCESSDENIED => base.Error.ACCESSDENIED,
        //
        dxgi.ERROR_ACCESS_DENIED => dxgi.Error.ACCESS_DENIED,
        dxgi.ERROR_ACCESS_LOST => dxgi.Error.ACCESS_LOST,
        dxgi.ERROR_ALREADY_EXISTS => dxgi.Error.ALREADY_EXISTS,
        dxgi.ERROR_CANNOT_PROTECT_CONTENT => dxgi.Error.CANNOT_PROTECT_CONTENT,
        dxgi.ERROR_DEVICE_HUNG => dxgi.Error.DEVICE_HUNG,
        dxgi.ERROR_DEVICE_REMOVED => dxgi.Error.DEVICE_REMOVED,
        dxgi.ERROR_DEVICE_RESET => dxgi.Error.DEVICE_RESET,
        dxgi.ERROR_DRIVER_INTERNAL_ERROR => dxgi.Error.DRIVER_INTERNAL_ERROR,
        dxgi.ERROR_FRAME_STATISTICS_DISJOINT => dxgi.Error.FRAME_STATISTICS_DISJOINT,
        dxgi.ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE => dxgi.Error.GRAPHICS_VIDPN_SOURCE_IN_USE,
        dxgi.ERROR_INVALID_CALL => dxgi.Error.INVALID_CALL,
        dxgi.ERROR_MORE_DATA => dxgi.Error.MORE_DATA,
        dxgi.ERROR_NAME_ALREADY_EXISTS => dxgi.Error.NAME_ALREADY_EXISTS,
        dxgi.ERROR_NONEXCLUSIVE => dxgi.Error.NONEXCLUSIVE,
        dxgi.ERROR_NOT_CURRENTLY_AVAILABLE => dxgi.Error.NOT_CURRENTLY_AVAILABLE,
        dxgi.ERROR_NOT_FOUND => dxgi.Error.NOT_FOUND,
        dxgi.ERROR_REMOTE_CLIENT_DISCONNECTED => dxgi.Error.REMOTE_CLIENT_DISCONNECTED,
        dxgi.ERROR_REMOTE_OUTOFMEMORY => dxgi.Error.REMOTE_OUTOFMEMORY,
        dxgi.ERROR_RESTRICT_TO_OUTPUT_STALE => dxgi.Error.RESTRICT_TO_OUTPUT_STALE,
        dxgi.ERROR_SDK_COMPONENT_MISSING => dxgi.Error.SDK_COMPONENT_MISSING,
        dxgi.ERROR_SESSION_DISCONNECTED => dxgi.Error.SESSION_DISCONNECTED,
        dxgi.ERROR_UNSUPPORTED => dxgi.Error.UNSUPPORTED,
        dxgi.ERROR_WAIT_TIMEOUT => dxgi.Error.WAIT_TIMEOUT,
        dxgi.ERROR_WAS_STILL_DRAWING => dxgi.Error.WAS_STILL_DRAWING,
        //
        d3d12.ERROR_ADAPTER_NOT_FOUND => d3d12.Error.ADAPTER_NOT_FOUND,
        d3d12.ERROR_DRIVER_VERSION_MISMATCH => d3d12.Error.DRIVER_VERSION_MISMATCH,
        //
        d3d11.ERROR_FILE_NOT_FOUND => d3d11.Error.FILE_NOT_FOUND,
        d3d11.ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS => d3d11.Error.TOO_MANY_UNIQUE_STATE_OBJECTS,
        d3d11.ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS => d3d11.Error.TOO_MANY_UNIQUE_VIEW_OBJECTS,
        d3d11.ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD => d3d11.Error.DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD,
        //
        wasapi.AUDCLNT_E_NOT_INITIALIZED => wasapi.Error.NOT_INITIALIZED,
        wasapi.AUDCLNT_E_ALREADY_INITIALIZED => wasapi.Error.ALREADY_INITIALIZED,
        wasapi.AUDCLNT_E_WRONG_ENDPOINT_TYPE => wasapi.Error.WRONG_ENDPOINT_TYPE,
        wasapi.AUDCLNT_E_DEVICE_INVALIDATED => wasapi.Error.DEVICE_INVALIDATED,
        wasapi.AUDCLNT_E_NOT_STOPPED => wasapi.Error.NOT_STOPPED,
        wasapi.AUDCLNT_E_BUFFER_TOO_LARGE => wasapi.Error.BUFFER_TOO_LARGE,
        wasapi.AUDCLNT_E_OUT_OF_ORDER => wasapi.Error.OUT_OF_ORDER,
        wasapi.AUDCLNT_E_UNSUPPORTED_FORMAT => wasapi.Error.UNSUPPORTED_FORMAT,
        wasapi.AUDCLNT_E_INVALID_SIZE => wasapi.Error.INVALID_SIZE,
        wasapi.AUDCLNT_E_DEVICE_IN_USE => wasapi.Error.DEVICE_IN_USE,
        wasapi.AUDCLNT_E_BUFFER_OPERATION_PENDING => wasapi.Error.BUFFER_OPERATION_PENDING,
        wasapi.AUDCLNT_E_THREAD_NOT_REGISTERED => wasapi.Error.THREAD_NOT_REGISTERED,
        wasapi.AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED => wasapi.Error.EXCLUSIVE_MODE_NOT_ALLOWED,
        wasapi.AUDCLNT_E_ENDPOINT_CREATE_FAILED => wasapi.Error.ENDPOINT_CREATE_FAILED,
        wasapi.AUDCLNT_E_SERVICE_NOT_RUNNING => wasapi.Error.SERVICE_NOT_RUNNING,
        wasapi.AUDCLNT_E_EVENTHANDLE_NOT_EXPECTED => wasapi.Error.EVENTHANDLE_NOT_EXPECTED,
        wasapi.AUDCLNT_E_EXCLUSIVE_MODE_ONLY => wasapi.Error.EXCLUSIVE_MODE_ONLY,
        wasapi.AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL => wasapi.Error.BUFDURATION_PERIOD_NOT_EQUAL,
        wasapi.AUDCLNT_E_EVENTHANDLE_NOT_SET => wasapi.Error.EVENTHANDLE_NOT_SET,
        wasapi.AUDCLNT_E_INCORRECT_BUFFER_SIZE => wasapi.Error.INCORRECT_BUFFER_SIZE,
        wasapi.AUDCLNT_E_BUFFER_SIZE_ERROR => wasapi.Error.BUFFER_SIZE_ERROR,
        wasapi.AUDCLNT_E_CPUUSAGE_EXCEEDED => wasapi.Error.CPUUSAGE_EXCEEDED,
        wasapi.AUDCLNT_E_BUFFER_ERROR => wasapi.Error.BUFFER_ERROR,
        wasapi.AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED => wasapi.Error.BUFFER_SIZE_NOT_ALIGNED,
        wasapi.AUDCLNT_E_INVALID_DEVICE_PERIOD => wasapi.Error.INVALID_DEVICE_PERIOD,
        //
        dwrite.E_FILEFORMAT => dwrite.Error.E_FILEFORMAT,
        //
        xapo.E_FORMAT_UNSUPPORTED => xapo.Error.E_FORMAT_UNSUPPORTED,
        //
        base.E_FILE_NOT_FOUND => base.MiscError.E_FILE_NOT_FOUND,
        base.S_FALSE => base.MiscError.S_FALSE,
        // treat unknown error return codes as E_FAIL
        else => blk: {
            std.debug.print("HRESULT error 0x{x} not recognized treating as E_FAIL.", .{@bitCast(c_ulong, hr)});
            break :blk base.Error.FAIL;
        },
    };
}

pub fn errorToHRESULT(err: HResultError) HRESULT {
    return switch (err) {
        base.Error.UNEXPECTED => base.E_UNEXPECTED,
        base.Error.NOTIMPL => base.E_NOTIMPL,
        base.Error.OUTOFMEMORY => base.E_OUTOFMEMORY,
        base.Error.INVALIDARG => base.E_INVALIDARG,
        base.Error.POINTER => base.E_POINTER,
        base.Error.HANDLE => base.E_HANDLE,
        base.Error.ABORT => base.E_ABORT,
        base.Error.FAIL => base.E_FAIL,
        base.Error.ACCESSDENIED => base.E_ACCESSDENIED,
        //
        dxgi.Error.ACCESS_DENIED => dxgi.ERROR_ACCESS_DENIED,
        dxgi.Error.ACCESS_LOST => dxgi.ERROR_ACCESS_LOST,
        dxgi.Error.ALREADY_EXISTS => dxgi.ERROR_ALREADY_EXISTS,
        dxgi.Error.CANNOT_PROTECT_CONTENT => dxgi.ERROR_CANNOT_PROTECT_CONTENT,
        dxgi.Error.DEVICE_HUNG => dxgi.ERROR_DEVICE_HUNG,
        dxgi.Error.DEVICE_REMOVED => dxgi.ERROR_DEVICE_REMOVED,
        dxgi.Error.DEVICE_RESET => dxgi.ERROR_DEVICE_RESET,
        dxgi.Error.DRIVER_INTERNAL_ERROR => dxgi.ERROR_DRIVER_INTERNAL_ERROR,
        dxgi.Error.FRAME_STATISTICS_DISJOINT => dxgi.ERROR_FRAME_STATISTICS_DISJOINT,
        dxgi.Error.GRAPHICS_VIDPN_SOURCE_IN_USE => dxgi.ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE,
        dxgi.Error.INVALID_CALL => dxgi.ERROR_INVALID_CALL,
        dxgi.Error.MORE_DATA => dxgi.ERROR_MORE_DATA,
        dxgi.Error.NAME_ALREADY_EXISTS => dxgi.ERROR_NAME_ALREADY_EXISTS,
        dxgi.Error.NONEXCLUSIVE => dxgi.ERROR_NONEXCLUSIVE,
        dxgi.Error.NOT_CURRENTLY_AVAILABLE => dxgi.ERROR_NOT_CURRENTLY_AVAILABLE,
        dxgi.Error.NOT_FOUND => dxgi.ERROR_NOT_FOUND,
        dxgi.Error.REMOTE_CLIENT_DISCONNECTED => dxgi.ERROR_REMOTE_CLIENT_DISCONNECTED,
        dxgi.Error.REMOTE_OUTOFMEMORY => dxgi.ERROR_REMOTE_OUTOFMEMORY,
        dxgi.Error.RESTRICT_TO_OUTPUT_STALE => dxgi.ERROR_RESTRICT_TO_OUTPUT_STALE,
        dxgi.Error.SDK_COMPONENT_MISSING => dxgi.ERROR_SDK_COMPONENT_MISSING,
        dxgi.Error.SESSION_DISCONNECTED => dxgi.ERROR_SESSION_DISCONNECTED,
        dxgi.Error.UNSUPPORTED => dxgi.ERROR_UNSUPPORTED,
        dxgi.Error.WAIT_TIMEOUT => dxgi.ERROR_WAIT_TIMEOUT,
        dxgi.Error.WAS_STILL_DRAWING => dxgi.ERROR_WAS_STILL_DRAWING,
        //
        d3d12.Error.ADAPTER_NOT_FOUND => d3d12.ERROR_ADAPTER_NOT_FOUND,
        d3d12.Error.DRIVER_VERSION_MISMATCH => d3d12.ERROR_DRIVER_VERSION_MISMATCH,
        d3d11.Error.FILE_NOT_FOUND => d3d11.ERROR_FILE_NOT_FOUND,
        d3d11.Error.TOO_MANY_UNIQUE_STATE_OBJECTS => d3d11.ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS,
        d3d11.Error.TOO_MANY_UNIQUE_VIEW_OBJECTS => d3d11.ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS,
        d3d11.Error.DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD => d3d11.ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD,
        //
        wasapi.Error.NOT_INITIALIZED => wasapi.AUDCLNT_E_NOT_INITIALIZED,
        wasapi.Error.ALREADY_INITIALIZED => wasapi.AUDCLNT_E_ALREADY_INITIALIZED,
        wasapi.Error.WRONG_ENDPOINT_TYPE => wasapi.AUDCLNT_E_WRONG_ENDPOINT_TYPE,
        wasapi.Error.DEVICE_INVALIDATED => wasapi.AUDCLNT_E_DEVICE_INVALIDATED,
        wasapi.Error.NOT_STOPPED => wasapi.AUDCLNT_E_NOT_STOPPED,
        wasapi.Error.BUFFER_TOO_LARGE => wasapi.AUDCLNT_E_BUFFER_TOO_LARGE,
        wasapi.Error.OUT_OF_ORDER => wasapi.AUDCLNT_E_OUT_OF_ORDER,
        wasapi.Error.UNSUPPORTED_FORMAT => wasapi.AUDCLNT_E_UNSUPPORTED_FORMAT,
        wasapi.Error.INVALID_SIZE => wasapi.AUDCLNT_E_INVALID_SIZE,
        wasapi.Error.DEVICE_IN_USE => wasapi.AUDCLNT_E_DEVICE_IN_USE,
        wasapi.Error.BUFFER_OPERATION_PENDING => wasapi.AUDCLNT_E_BUFFER_OPERATION_PENDING,
        wasapi.Error.THREAD_NOT_REGISTERED => wasapi.AUDCLNT_E_THREAD_NOT_REGISTERED,
        wasapi.Error.EXCLUSIVE_MODE_NOT_ALLOWED => wasapi.AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED,
        wasapi.Error.ENDPOINT_CREATE_FAILED => wasapi.AUDCLNT_E_ENDPOINT_CREATE_FAILED,
        wasapi.Error.SERVICE_NOT_RUNNING => wasapi.AUDCLNT_E_SERVICE_NOT_RUNNING,
        wasapi.Error.EVENTHANDLE_NOT_EXPECTED => wasapi.AUDCLNT_E_EVENTHANDLE_NOT_EXPECTED,
        wasapi.Error.EXCLUSIVE_MODE_ONLY => wasapi.AUDCLNT_E_EXCLUSIVE_MODE_ONLY,
        wasapi.Error.BUFDURATION_PERIOD_NOT_EQUAL => wasapi.AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL,
        wasapi.Error.EVENTHANDLE_NOT_SET => wasapi.AUDCLNT_E_EVENTHANDLE_NOT_SET,
        wasapi.Error.INCORRECT_BUFFER_SIZE => wasapi.AUDCLNT_E_INCORRECT_BUFFER_SIZE,
        wasapi.Error.BUFFER_SIZE_ERROR => wasapi.AUDCLNT_E_BUFFER_SIZE_ERROR,
        wasapi.Error.CPUUSAGE_EXCEEDED => wasapi.AUDCLNT_E_CPUUSAGE_EXCEEDED,
        wasapi.Error.BUFFER_ERROR => wasapi.AUDCLNT_E_BUFFER_ERROR,
        wasapi.Error.BUFFER_SIZE_NOT_ALIGNED => wasapi.AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED,
        wasapi.Error.INVALID_DEVICE_PERIOD => wasapi.AUDCLNT_E_INVALID_DEVICE_PERIOD,
        //
        dwrite.Error.E_FILEFORMAT => dwrite.E_FILEFORMAT,
        //
        xapo.Error.E_FORMAT_UNSUPPORTED => xapo.E_FORMAT_UNSUPPORTED,
        //
        base.MiscError.E_FILE_NOT_FOUND => base.E_FILE_NOT_FOUND,
        base.MiscError.S_FALSE => base.S_FALSE,
    };
}
