const std = @import("std");
const windows = std.os.windows;
const WORD = windows.WORD;
const DWORD = windows.DWORD;
const HANDLE = windows.HANDLE;
const LONG = windows.LONG;
const LPARAM = windows.LPARAM;
const WPARAM = windows.WPARAM;
const GUID = windows.GUID;
const ULONG = windows.ULONG;
const WINAPI = windows.WINAPI;
const BOOL = windows.BOOL;
const LPCSTR = windows.LPCSTR;
const RECT = windows.RECT;
const SHORT = windows.SHORT;
const POINT = windows.POINT;
const SIZE_T = windows.SIZE_T;
const LPVOID = windows.LPVOID;
const LPCWSTR = windows.LPCWSTR;
const LPSECURITY_ATTRIBUTES = *windows.SECURITY_ATTRIBUTES;
const LARGE_INTEGER = windows.LARGE_INTEGER;

pub const PTIMERAPCROUTINE = *anyopaque;

pub const CREATE_WAITABLE_TIMER_MANUAL_RESET = @as(DWORD, 0x00000001);
pub const CREATE_WAITABLE_TIMER_HIGH_RESOLUTION = @as(DWORD, 0x00000002);

pub extern "kernel32" fn CreateWaitableTimerExW(
    ?LPSECURITY_ATTRIBUTES,
    ?LPCWSTR,
    DWORD,
    DWORD,
) callconv(WINAPI) HANDLE;

pub extern "kernel32" fn SetWaitableTimer(
    HANDLE,
    *const LARGE_INTEGER,
    LONG,
    ?PTIMERAPCROUTINE,
    ?LPVOID,
    BOOL,
) callconv(WINAPI) BOOL;
