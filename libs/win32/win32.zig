pub const base = @import("windows.zig");
pub const dwrite = @import("dwrite.zig");
pub const dxgi = @import("dxgi.zig");
pub const d3d11 = @import("d3d11.zig");
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
pub const HResultError = base.HResultError || dxgi.Error || d3d12.Error || d3d11.Error || dwrite.Error || xapo.Error;

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
        base.E_FAIL => base.HResultError.E_FAIL,
        base.E_INVALIDARG => base.HResultError.E_INVALIDARG,
        base.E_OUTOFMEMORY => base.HResultError.E_OUTOFMEMORY,
        base.E_NOTIMPL => base.HResultError.E_NOTIMPL,
        base.E_FILE_NOT_FOUND => base.HResultError.E_FILE_NOT_FOUND,
        base.E_NOINTERFACE => base.HResultError.E_NOINTERFACE,
        base.S_FALSE => base.HResultError.S_FALSE,
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
        d3d12.ERROR_ADAPTER_NOT_FOUND => d3d12.Error.ADAPTER_NOT_FOUND,
        d3d12.ERROR_DRIVER_VERSION_MISMATCH => d3d12.Error.DRIVER_VERSION_MISMATCH,
        d3d11.ERROR_FILE_NOT_FOUND => d3d11.Error.FILE_NOT_FOUND,
        d3d11.ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS => d3d11.Error.TOO_MANY_UNIQUE_STATE_OBJECTS,
        d3d11.ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS => d3d11.Error.TOO_MANY_UNIQUE_VIEW_OBJECTS,
        d3d11.ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD => d3d11.Error.DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD,
        dwrite.E_FILEFORMAT => dwrite.Error.E_FILEFORMAT,
        xapo.E_FORMAT_UNSUPPORTED => xapo.Error.E_FORMAT_UNSUPPORTED,
        else => blk: {
            std.debug.print("HRESULT error 0x{x} not recognized treating as E_FAIL.", .{@bitCast(c_ulong, hr)});
            break :blk HResultError.E_FAIL;
        },
    };
}

pub fn errorToHRESULT(err: HResultError) HRESULT {
    return switch (err) {
        base.HResultError.E_FAIL => base.E_FAIL,
        base.HResultError.E_INVALIDARG => base.E_INVALIDARG,
        base.HResultError.E_OUTOFMEMORY => base.E_OUTOFMEMORY,
        base.HResultError.E_NOTIMPL => base.E_NOTIMPL,
        base.HResultError.E_FILE_NOT_FOUND => base.E_FILE_NOT_FOUND,
        base.HResultError.E_NOINTERFACE => base.E_NOINTERFACE,
        base.HResultError.S_FALSE => base.S_FALSE,
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
        d3d12.Error.ADAPTER_NOT_FOUND => d3d12.ERROR_ADAPTER_NOT_FOUND,
        d3d12.Error.DRIVER_VERSION_MISMATCH => d3d12.ERROR_DRIVER_VERSION_MISMATCH,
        d3d11.Error.FILE_NOT_FOUND => d3d11.ERROR_FILE_NOT_FOUND,
        d3d11.Error.TOO_MANY_UNIQUE_STATE_OBJECTS => d3d11.ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS,
        d3d11.Error.TOO_MANY_UNIQUE_VIEW_OBJECTS => d3d11.ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS,
        d3d11.Error.DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD => d3d11.ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD,
        dwrite.Error.E_FILEFORMAT => dwrite.E_FILEFORMAT,
        xapo.Error.E_FORMAT_UNSUPPORTED => xapo.E_FORMAT_UNSUPPORTED,
    };
}
