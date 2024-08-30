comptime {
    std.testing.refAllDeclsRecursive(@This());
}

pub const windows = @import("bindings/windows.zig");
pub const dwrite = @import("bindings/dwrite.zig");
pub const dxgi = @import("bindings/dxgi.zig");
pub const d3d11 = @import("bindings/d3d11.zig");
pub const d3d11d = @import("bindings/d3d11sdklayers.zig");
pub const d3d12 = @import("bindings/d3d12.zig");
pub const d3d12d = @import("bindings/d3d12sdklayers.zig");
pub const d3d = @import("bindings/d3dcommon.zig");
pub const d2d1 = @import("bindings/d2d1.zig");
pub const d3d11on12 = @import("bindings/d3d11on12.zig");
pub const wic = @import("bindings/wincodec.zig");
pub const wasapi = @import("bindings/wasapi.zig");
pub const directml = @import("bindings/directml.zig");
pub const mf = @import("bindings/mf.zig");
pub const xaudio2 = @import("bindings/xaudio2.zig");
pub const xaudio2fx = @import("bindings/xaudio2fx.zig");
pub const xapo = @import("bindings/xapo.zig");
pub const xinput = @import("bindings/xinput.zig");
pub const dds_loader = @import("bindings/dds_loader.zig");
pub const d3dcompiler = @import("bindings/d3dcompiler.zig");

const std = @import("std");
const panic = std.debug.panic;
const assert = std.debug.assert;

const WINAPI = std.os.windows.WINAPI;
const S_OK = std.os.windows.S_OK;
const S_FALSE = std.os.windows.S_FALSE;
const E_NOTIMPL = std.os.windows.E_NOTIMPL;
const E_NOINTERFACE = std.os.windows.E_NOINTERFACE;
const E_POINTER = std.os.windows.E_POINTER;
const E_ABORT = std.os.windows.E_ABORT;
const E_FAIL = std.os.windows.E_FAIL;
const E_UNEXPECTED = std.os.windows.E_UNEXPECTED;
const E_ACCESSDENIED = std.os.windows.E_ACCESSDENIED;
const E_HANDLE = std.os.windows.E_HANDLE;
const E_OUTOFMEMORY = std.os.windows.E_OUTOFMEMORY;
const E_INVALIDARG = std.os.windows.E_INVALIDARG;
const GENERIC_READ = std.os.windows.GENERIC_READ;
const GENERIC_WRITE = std.os.windows.GENERIC_WRITE;
const GENERIC_EXECUTE = std.os.windows.GENERIC_EXECUTE;
const GENERIC_ALL = std.os.windows.GENERIC_ALL;
const EVENT_ALL_ACCESS = std.os.windows.EVENT_ALL_ACCESS;
const TRUE = std.os.windows.TRUE;
const FALSE = std.os.windows.FALSE;
const BOOL = std.os.windows.BOOL;
const BOOLEAN = std.os.windows.BOOLEAN;
const BYTE = std.os.windows.BYTE;
const CHAR = std.os.windows.CHAR;
const UCHAR = std.os.windows.UCHAR;
const WCHAR = std.os.windows.WCHAR;
const FLOAT = std.os.windows.FLOAT;
const HCRYPTPROV = std.os.windows.HCRYPTPROV;
const ATOM = std.os.windows.ATOM;
const WPARAM = std.os.windows.WPARAM;
const LPARAM = std.os.windows.LPARAM;
const LRESULT = std.os.windows.LRESULT;
const HRESULT = std.os.windows.HRESULT;
const HBRUSH = std.os.windows.HBRUSH;
const HCURSOR = std.os.windows.HCURSOR;
const HICON = std.os.windows.HICON;
const HINSTANCE = std.os.windows.HINSTANCE;
const HMENU = std.os.windows.HMENU;
const HMODULE = std.os.windows.HMODULE;
const HWND = std.os.windows.HWND;
const HDC = std.os.windows.HDC;
const HGLRC = std.os.windows.HGLRC;
const FARPROC = std.os.windows.FARPROC;
const INT = std.os.windows.INT;
const SIZE_T = std.os.windows.SIZE_T;
const UINT = std.os.windows.UINT;
const USHORT = std.os.windows.USHORT;
const SHORT = std.os.windows.SHORT;
const ULONG = std.os.windows.ULONG;
const LONG = std.os.windows.LONG;
const WORD = std.os.windows.WORD;
const DWORD = std.os.windows.DWORD;
const ULONGLONG = std.os.windows.ULONGLONG;
const LONGLONG = std.os.windows.LONGLONG;
const LARGE_INTEGER = std.os.windows.LARGE_INTEGER;
const ULARGE_INTEGER = std.os.windows.ULARGE_INTEGER;
const LPCSTR = std.os.windows.LPCSTR;
const LPCVOID = std.os.windows.LPCVOID;
const LPSTR = std.os.windows.LPSTR;
const LPVOID = std.os.windows.LPVOID;
const LPWSTR = std.os.windows.LPWSTR;
const LPCWSTR = std.os.windows.LPCSWTR;
const PVOID = std.os.windows.PVOID;
const PWSTR = std.os.windows.PWSTR;
const PCWSTR = std.os.windows.PCWSTR;
const HANDLE = std.os.windows.HANDLE;
const GUID = std.os.windows.GUID;
const NTSTATUS = std.os.windows.NTSTATUS;
const CRITICAL_SECTION = std.os.windows.CRITICAL_SECTION;
const SECURITY_ATTRIBUTES = std.os.windows.SECURITY_ATTRIBUTES;
const RECT = std.os.windows.RECT;
const POINT = std.os.windows.POINT;

/// https://docs.microsoft.com/en-us/windows/win32/com/com-error-codes-10
///
/// [DEPRECATED]: Use proc specific errors as in std.os.windows
pub const HResultError =
    windows.MiscError || windows.Error || dxgi.Error || d3d12.Error || d3d11.Error ||
    wasapi.Error || dwrite.Error || xapo.Error || xinput.Error;

pub fn hrPanic(err: HResultError) noreturn {
    panic(
        "HRESULT error detected (0x{x}, {}).",
        .{ @as(c_ulong, @bitCast(errorToHRESULT(err))), err },
    );
}

pub inline fn hrPanicOnFail(hr: HRESULT) void {
    if (hr != S_OK) {
        hrPanic(hrToError(hr));
    }
}

/// [DEPRECATED]: Use proc specific errors as in std.os.windows
pub inline fn hrErrorOnFail(hr: HRESULT) HResultError!void {
    if (hr != S_OK) {
        return hrToError(hr);
    }
}

/// [DEPRECATED]: Use proc specific errors as in std.os.windows
pub fn hrToError(hr: HRESULT) HResultError {
    assert(hr != S_OK);
    return switch (hr) {
        //
        windows.E_UNEXPECTED => windows.Error.UNEXPECTED,
        windows.E_NOTIMPL => windows.Error.NOTIMPL,
        windows.E_OUTOFMEMORY => windows.Error.OUTOFMEMORY,
        windows.E_INVALIDARG => windows.Error.INVALIDARG,
        windows.E_POINTER => windows.Error.POINTER,
        windows.E_HANDLE => windows.Error.HANDLE,
        windows.E_ABORT => windows.Error.ABORT,
        windows.E_FAIL => windows.Error.FAIL,
        windows.E_ACCESSDENIED => windows.Error.ACCESSDENIED,
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
        xinput.ERROR_EMPTY => xinput.Error.EMPTY,
        xinput.ERROR_DEVICE_NOT_CONNECTED => xinput.Error.DEVICE_NOT_CONNECTED,
        //
        windows.E_FILE_NOT_FOUND => windows.MiscError.E_FILE_NOT_FOUND,
        windows.S_FALSE => windows.MiscError.S_FALSE,
        // treat unknown error return codes as E_FAIL
        else => blk: {
            std.log.debug("HRESULT error 0x{x} not recognized treating as E_FAIL.", .{@as(c_ulong, @bitCast(hr))});
            break :blk windows.Error.FAIL;
        },
    };
}

pub fn errorToHRESULT(err: HResultError) HRESULT {
    return switch (err) {
        windows.Error.UNEXPECTED => E_UNEXPECTED,
        windows.Error.NOTIMPL => E_NOTIMPL,
        windows.Error.OUTOFMEMORY => E_OUTOFMEMORY,
        windows.Error.INVALIDARG => E_INVALIDARG,
        windows.Error.POINTER => E_POINTER,
        windows.Error.HANDLE => E_HANDLE,
        windows.Error.ABORT => E_ABORT,
        windows.Error.FAIL => E_FAIL,
        windows.Error.ACCESSDENIED => E_ACCESSDENIED,
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
        xinput.Error.EMPTY => xinput.ERROR_EMPTY,
        xinput.Error.DEVICE_NOT_CONNECTED => xinput.ERROR_DEVICE_NOT_CONNECTED,
        //
        windows.MiscError.E_FILE_NOT_FOUND => windows.E_FILE_NOT_FOUND,
        windows.MiscError.S_FALSE => windows.S_FALSE,
    };
}
