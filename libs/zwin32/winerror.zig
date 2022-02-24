const base = @import("windows.zig");
const HRESULT = base.HRESULT;
const LONG = base.LONG;

pub const S_FALSE = @bitCast(HRESULT, @as(c_ulong, 0x00000001));

pub const Error = error{
    UNEXPECTED,
    NOTIMPL,
    OUTOFMEMORY,
    INVALIDARG,
    POINTER,
    HANDLE,
    ABORT,
    FAIL,
    ACCESSDENIED,
};

pub const SEVERITY_SUCCESS = 0;
pub const SEVERITY_ERROR = 1;

pub fn MAKE_HRESULT(severity: LONG, facility: LONG, value: LONG) HRESULT {
    return @as(HRESULT, (severity << 31) | (facility << 16) | value);
}
