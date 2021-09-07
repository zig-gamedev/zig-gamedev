const std = @import("std");
const win32 = @import("win32");
const windows = std.os.windows;
const d3d12 = win32.d3d12;
const kernel32 = windows.kernel32;
const ole32 = windows.ole32;
const shell32 = windows.shell32;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const HMODULE = windows.HMODULE;
const HRESULT = windows.HRESULT;
const GUID = windows.GUID;
const LPCSTR = windows.LPCSTR;
const LPWSTR = windows.LPWSTR;
const LPCWSTR = windows.LPCWSTR;
const FARPROC = windows.FARPROC;
const HWND = windows.HWND;
const WINAPI = windows.WINAPI;
const UINT32 = u32;
const BOOL = windows.BOOL;
const DWORD = windows.DWORD;

// TODO(mziulek): Disable PIX usage when needed. How?
const USE_PIX: bool = true;

pub const CAPTURE_TIMING = (1 << 0);
pub const CAPTURE_GPU = (1 << 1);
pub const CAPTURE_FUNCTION_SUMMARY = (1 << 2);
pub const CAPTURE_FUNCTION_DETAILS = (1 << 3);
pub const CAPTURE_CALLGRAPH = (1 << 4);
pub const CAPTURE_INSTRUCTION_TRACE = (1 << 5);
pub const CAPTURE_SYSTEM_MONITOR_COUNTERS = (1 << 6);
pub const CAPTURE_VIDEO = (1 << 7);
pub const CAPTURE_AUDIO = (1 << 8);
pub const CAPTURE_RESERVED = (1 << 15);

pub const CaptureStorage = enum(u32) {
    Memory = 0,
};

pub const GpuCaptureParameters = extern struct {
    FileName: LPCWSTR,
};

pub const TimingCaptureParameters = extern struct {
    FileName: LPCWSTR,
    MaximumToolingMemorySizeMb: UINT32,
    CaptureStorage: CaptureStorage,

    CaptureGpuTiming: BOOL,

    CaptureCallstacks: BOOL,
    CaptureCpuSamples: BOOL,
    CpuSamplesPerSecond: UINT32,

    CaptureFileIO: BOOL,

    CaptureVirtualAllocEvents: BOOL,
    CaptureHeapAllocEvents: BOOL,
    CaptureXMemEvents: BOOL, // Xbox only
    CapturePixMemEvents: BOOL, // Xbox only
};

pub const CaptureParameters = extern union {
    gpu_capture_params: GpuCaptureParameters,
    timing_capture_params: TimingCaptureParameters,
};

pub fn loadLatestWinPixGpuCapturerLibrary() ?HMODULE {
    const module = kernel32.GetModuleHandleW(L("WinPixGpuCapturer.dll"));
    if (module != null) {
        return module;
    }

    const FOLDERID_ProgramFiles = GUID.parse("{905e63b6-c1bf-494e-b29c-65b732d3d21a}");
    var program_files_path_ptr: LPWSTR = undefined;
    if (shell32.SHGetKnownFolderPath(
        &FOLDERID_ProgramFiles,
        windows.KF_FLAG_DEFAULT,
        null,
        &program_files_path_ptr,
    ) != windows.S_OK) {
        return null;
    }
    defer ole32.CoTaskMemFree(program_files_path_ptr);

    var alloc_buffer: [2048]u8 = undefined;
    var alloc = std.heap.FixedBufferAllocator.init(alloc_buffer[0..]);

    const program_files_path = std.unicode.utf16leToUtf8AllocZ(
        &alloc.allocator,
        std.mem.spanZ(program_files_path_ptr),
    ) catch unreachable;
    const pix_path = std.fs.path.joinZ(
        &alloc.allocator,
        &[_][]const u8{ program_files_path, "Microsoft PIX" },
    ) catch unreachable;

    const pix_dir = std.fs.openDirAbsoluteZ(pix_path, .{ .iterate = true }) catch return null;
    var newest_ver: f64 = 0.0;
    var newest_ver_name: [windows.MAX_PATH:0]u8 = undefined;

    var pix_dir_it = pix_dir.iterate();
    while (pix_dir_it.next() catch return null) |entry| {
        if (entry.kind == .Directory) {
            const ver = std.fmt.parseFloat(f64, entry.name) catch continue;
            if (ver > newest_ver) {
                newest_ver = ver;
                std.mem.copy(u8, newest_ver_name[0..], entry.name);
                newest_ver_name[entry.name.len] = 0;
            }
        }
    }
    if (newest_ver == 0.0) {
        return null;
    }

    const dll_path = std.fs.path.joinZ(
        &alloc.allocator,
        &[_][]const u8{ pix_path, std.mem.spanZ(&newest_ver_name), "WinPixGpuCapturer.dll" },
    ) catch unreachable;

    const lib = std.DynLib.openZ(dll_path.ptr) catch return null;
    return lib.dll;
}

fn getFunctionPtr(func_name: LPCSTR) ?FARPROC {
    const module = kernel32.GetModuleHandleW(L("WinPixGpuCapturer.dll"));
    if (module == null) {
        return null;
    }

    const func = kernel32.GetProcAddress(module.?, func_name);
    if (func == null) {
        return null;
    }

    return func;
}

pub fn beginCapture(flags: DWORD, params: ?*const CaptureParameters) callconv(WINAPI) HRESULT {
    if (flags == CAPTURE_GPU) {
        const beginProgrammaticGpuCapture = @ptrCast(
            ?fn (?*const CaptureParameters) callconv(WINAPI) HRESULT,
            getFunctionPtr("BeginProgrammaticGpuCapture"),
        );
        if (beginProgrammaticGpuCapture == null) {
            return windows.E_FAIL;
        }
        return beginProgrammaticGpuCapture.?(params);
    } else {
        return windows.E_NOTIMPL;
    }
}

pub fn endCapture() callconv(WINAPI) HRESULT {
    const endProgrammaticGpuCapture = @ptrCast(
        ?fn () callconv(WINAPI) HRESULT,
        getFunctionPtr("EndProgrammaticGpuCapture"),
    );
    if (endProgrammaticGpuCapture == null) {
        return windows.E_FAIL;
    }
    return endProgrammaticGpuCapture.?();
}

pub fn setTargetWindow(hwnd: HWND) HRESULT {
    const setGlobalTargetWindow = @ptrCast(?fn (HWND) callconv(WINAPI) void, getFunctionPtr("SetGlobalTargetWindow"));
    if (setGlobalTargetWindow == null) {
        return windows.E_FAIL;
    }
    setGlobalTargetWindow.?(hwnd);
    return windows.S_OK;
}

pub fn gpuCaptureNextFrames(file_name: LPCWSTR, num_frames: UINT32) HRESULT {
    const captureNextFrame = @ptrCast(?fn (LPCWSTR, UINT32) callconv(WINAPI) HRESULT, getFunctionPtr("CaptureNextFrame"));
    if (captureNextFrame == null) {
        return windows.E_FAIL;
    }
    return captureNextFrame.?(file_name, num_frames);
}

const EventType = enum(u64) {
    EndEvent = 0x000,
    BeginEvent_VarArgs = 0x001,
    BeginEvent_NoArgs = 0x002,
    SetMarker_VarArgs = 0x007,
    SetMarker_NoArgs = 0x008,

    EndEvent_OnContext = 0x010,
    BeginEvent_OnContext_VarArgs = 0x011,
    BeginEvent_OnContext_NoArgs = 0x012,
    SetMarker_OnContext_VarArgs = 0x017,
    SetMarker_OnContext_NoArgs = 0x018,
};

const EventsGraphicsRecordSpaceQwords: u64 = 64;
const EventsReservedTailSpaceQwords: u64 = 2;

const EventsTimestampWriteMask: u64 = 0x00000FFFFFFFFFFF;
const EventsTimestampBitShift: u64 = 20;

const EventsTypeWriteMask: u64 = 0x00000000000003FF;
const EventsTypeBitShift: u64 = 10;

const WINPIX_EVENT_PIX3BLOB_VERSION: u32 = 2;
const D3D12_EVENT_METADATA = WINPIX_EVENT_PIX3BLOB_VERSION;

fn encodeEventInfo(timestamp: u64, event_type: EventType) u64 {
    const mask0 = (timestamp & EventsTimestampWriteMask) << @intCast(u6, EventsTimestampBitShift);
    const mask1 = (@enumToInt(event_type) & EventsTypeWriteMask) << @intCast(u6, EventsTypeBitShift);
    return mask0 | mask1;
}

const EventsStringAlignmentWriteMask: u64 = 0x000000000000000F;
const EventsStringAlignmentBitShift: u64 = 60;

const EventsStringCopyChunkSizeWriteMask: u64 = 0x000000000000001F;
const EventsStringCopyChunkSizeBitShift: u64 = 55;

const EventsStringIsANSIWriteMask: u64 = 0x0000000000000001;
const EventsStringIsANSIBitShift: u64 = 54;

const EventsStringIsShortcutWriteMask: u64 = 0x0000000000000001;
const EventsStringIsShortcutBitShift: u64 = 53;

fn encodeStringInfo(alignment: u64, copy_chunk_size: u64, is_ansi: bool, is_shortcut: bool) u64 {
    const mask0 = (alignment & EventsStringAlignmentWriteMask) << @intCast(u6, EventsStringAlignmentBitShift);
    const mask1 = (copy_chunk_size & EventsStringCopyChunkSizeWriteMask) << @intCast(u6, EventsStringCopyChunkSizeBitShift);
    const mask2 = (@boolToInt(is_ansi) & EventsStringIsANSIWriteMask) << @intCast(u6, EventsStringIsANSIBitShift);
    const mask3 = (@boolToInt(is_shortcut) & EventsStringIsShortcutWriteMask) << @intCast(u6, EventsStringIsShortcutBitShift);
    return mask0 | mask1 | mask2 | mask3;
}

pub fn setMarkerOnCommandList(cmdlist: *d3d12.IGraphicsCommandList, name: []const u8) void {
    std.debug.assert(name.len > 0);
    const num_name_qwords: u32 = (@intCast(u32, name.len) + 7) / 8;
    std.debug.assert(num_name_qwords < (EventsGraphicsRecordSpaceQwords / 2));

    var buffer: [EventsGraphicsRecordSpaceQwords]u64 = undefined;
    var dest: [*]u64 = &buffer;
    dest.* = encodeEventInfo(0, .SetMarker_NoArgs);
    dest += 1;

    dest.* = 0xff_ff_00_ff; // Color.
    dest += 1;

    dest.* = encodeStringInfo(0, 8, true, false);
    dest += 1;

    dest[num_name_qwords - 1] = 0;
    dest[num_name_qwords + 0] = 0;
    @memcpy(@ptrCast([*]u8, dest), name.ptr, name.len);

    cmdlist.SetMarker(D3D12_EVENT_METADATA, @ptrCast(*c_void, &buffer), (3 + num_name_qwords) * 8);
}

pub fn beginEventOnCommandList(cmdlist: *d3d12.IGraphicsCommandList, name: []const u8) void {
    std.debug.assert(name.len > 0);
    const num_name_qwords: u32 = (@intCast(u32, name.len) + 7) / 8;
    std.debug.assert(num_name_qwords < (EventsGraphicsRecordSpaceQwords / 2));

    var buffer: [EventsGraphicsRecordSpaceQwords]u64 = undefined;
    var dest: [*]u64 = &buffer;
    dest.* = encodeEventInfo(0, .BeginEvent_NoArgs);
    dest += 1;

    dest.* = 0xff_ff_00_ff; // Color.
    dest += 1;

    dest.* = encodeStringInfo(0, 8, true, false);
    dest += 1;

    dest[num_name_qwords - 1] = 0;
    dest[num_name_qwords + 0] = 0;
    @memcpy(@ptrCast([*]u8, dest), name.ptr, name.len);

    cmdlist.BeginEvent(D3D12_EVENT_METADATA, @ptrCast(*c_void, &buffer), (3 + num_name_qwords) * 8);
}

pub inline fn endEventOnCommandList(cmdlist: *d3d12.IGraphicsCommandList) void {
    cmdlist.EndEvent();
}
