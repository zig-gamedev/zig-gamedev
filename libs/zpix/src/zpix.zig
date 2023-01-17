const std = @import("std");
const assert = std.debug.assert;
const w32 = @import("zwin32").w32;
const HMODULE = w32.HMODULE;
const HRESULT = w32.HRESULT;
const GUID = w32.GUID;
const LPCSTR = w32.LPCSTR;
const LPWSTR = w32.LPWSTR;
const LPCWSTR = w32.LPCWSTR;
const FARPROC = w32.FARPROC;
const HWND = w32.HWND;
const WINAPI = w32.WINAPI;
const BOOL = w32.BOOL;
const DWORD = w32.DWORD;
const UINT32 = u32;

const options = @import("zpix_options");
const enable = if (@hasDecl(options, "enable")) options.enable else false;

pub const CAPTURE_FLAGS = packed struct(UINT32) {
    TIMING: bool = false,
    GPU: bool = false,
    FUNCTION_SUMMARY: bool = false,
    FUNCTION_DETAILS: bool = false,
    CALLGRAPH: bool = false,
    INSTRUCTION_TRACE: bool = false,
    SYSTEM_MONITOR_COUNTERS: bool = false,
    VIDEO: bool = false,
    AUDIO: bool = false,
    __unused: u23 = 0,
};

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

pub const loadGpuCapturerLibrary = if (enable) impl.loadGpuCapturerLibrary else empty.loadGpuCapturerLibrary;
pub const beginCapture = if (enable) impl.beginCapture else empty.beginCapture;
pub const endCapture = if (enable) impl.endCapture else empty.endCapture;
pub const setTargetWindow = if (enable) impl.setTargetWindow else empty.setTargetWindow;
pub const gpuCaptureNextFrames = if (enable) impl.gpuCaptureNextFrames else empty.gpuCaptureNextFrames;

pub const setMarker = if (enable) impl.setMarker else empty.setMarker;
pub const beginEvent = if (enable) impl.beginEvent else empty.beginEvent;
pub const endEvent = if (enable) impl.endEvent else empty.endEvent;

fn getFunctionPtr(func_name: LPCSTR) ?FARPROC {
    const module = w32.GetModuleHandleA("WinPixGpuCapturer.dll");
    if (module == null) {
        return null;
    }

    const func = w32.GetProcAddress(module.?, func_name);
    if (func == null) {
        return null;
    }

    return func;
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
    const mask0 = (alignment & EventsStringAlignmentWriteMask) <<
        @intCast(u6, EventsStringAlignmentBitShift);
    const mask1 = (copy_chunk_size & EventsStringCopyChunkSizeWriteMask) <<
        @intCast(u6, EventsStringCopyChunkSizeBitShift);
    const mask2 = (@boolToInt(is_ansi) & EventsStringIsANSIWriteMask) <<
        @intCast(u6, EventsStringIsANSIBitShift);
    const mask3 = (@boolToInt(is_shortcut) & EventsStringIsShortcutWriteMask) <<
        @intCast(u6, EventsStringIsShortcutBitShift);
    return mask0 | mask1 | mask2 | mask3;
}

const impl = struct {
    fn loadGpuCapturerLibrary() ?HMODULE {
        const module = w32.GetModuleHandleA("WinPixGpuCapturer.dll");
        if (module != null) {
            return module;
        }

        var program_files_path_ptr: LPWSTR = undefined;
        if (w32.SHGetKnownFolderPath(
            &w32.FOLDERID_ProgramFiles,
            w32.KF_FLAG_DEFAULT,
            null,
            &program_files_path_ptr,
        ) != w32.S_OK) {
            return null;
        }
        defer w32.CoTaskMemFree(program_files_path_ptr);

        var alloc_buffer: [2048]u8 = undefined;
        var alloc_state = std.heap.FixedBufferAllocator.init(alloc_buffer[0..]);
        var alloc = alloc_state.allocator();

        const program_files_path = std.unicode.utf16leToUtf8AllocZ(
            alloc,
            std.mem.span(program_files_path_ptr),
        ) catch unreachable;
        const pix_path = std.fs.path.joinZ(
            alloc,
            &[_][]const u8{ program_files_path, "Microsoft PIX" },
        ) catch unreachable;

        const pix_dir = std.fs.openIterableDirAbsoluteZ(pix_path, .{}) catch return null;
        var newest_ver: f64 = 0.0;
        var newest_ver_name: [w32.MAX_PATH:0]u8 = undefined;

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
            alloc,
            &[_][]const u8{ pix_path, std.mem.sliceTo(&newest_ver_name, 0), "WinPixGpuCapturer.dll" },
        ) catch unreachable;

        return if (w32.LoadLibraryA(dll_path.ptr)) |lib| lib else null;
    }

    fn beginCapture(flags: CAPTURE_FLAGS, params: ?*const CaptureParameters) HRESULT {
        if (flags.GPU) {
            const beginProgrammaticGpuCapture = @ptrCast(
                ?*const fn (?*const CaptureParameters) callconv(WINAPI) HRESULT,
                getFunctionPtr("BeginProgrammaticGpuCapture"),
            );
            if (beginProgrammaticGpuCapture == null) {
                return w32.E_FAIL;
            }
            return beginProgrammaticGpuCapture.?(params);
        } else {
            return w32.E_NOTIMPL;
        }
    }

    fn endCapture() HRESULT {
        const endProgrammaticGpuCapture = @ptrCast(
            ?*const fn () callconv(WINAPI) HRESULT,
            getFunctionPtr("EndProgrammaticGpuCapture"),
        );
        if (endProgrammaticGpuCapture == null) {
            return w32.E_FAIL;
        }
        return endProgrammaticGpuCapture.?();
    }

    fn setTargetWindow(hwnd: HWND) HRESULT {
        const setGlobalTargetWindow = @ptrCast(
            ?*const fn (HWND) callconv(WINAPI) void,
            getFunctionPtr("SetGlobalTargetWindow"),
        );
        if (setGlobalTargetWindow == null) {
            return w32.E_FAIL;
        }
        setGlobalTargetWindow.?(hwnd);
        return w32.S_OK;
    }

    fn gpuCaptureNextFrames(file_name: LPCWSTR, num_frames: UINT32) HRESULT {
        const captureNextFrame = @ptrCast(
            ?*const fn (LPCWSTR, UINT32) callconv(WINAPI) HRESULT,
            getFunctionPtr("CaptureNextFrame"),
        );
        if (captureNextFrame == null) {
            return w32.E_FAIL;
        }
        return captureNextFrame.?(file_name, num_frames);
    }

    fn setMarkerOnContext(comptime Context: type, context: Context, name: []const u8) void {
        comptime {
            const T = @typeInfo(Context).Pointer.child;
            assert(@hasDecl(T, "SetMarker"));
        }
        assert(name.len > 0);
        const num_name_qwords: u32 = (@intCast(u32, name.len + 1) + 7) / 8;
        assert(num_name_qwords < (EventsGraphicsRecordSpaceQwords / 2));

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

        context.SetMarker(D3D12_EVENT_METADATA, @ptrCast(*anyopaque, &buffer), (3 + num_name_qwords) * 8);
    }

    fn setMarker(target: anytype, name: []const u8) void {
        setMarkerOnContext(@TypeOf(target), target, name);
    }

    fn beginEventOnContext(comptime Context: type, context: Context, name: []const u8) void {
        comptime {
            const T = @typeInfo(Context).Pointer.child;
            assert(@hasDecl(T, "BeginEvent"));
        }
        assert(name.len > 0);
        const num_name_qwords: u32 = (@intCast(u32, name.len + 1) + 7) / 8;
        assert(num_name_qwords < (EventsGraphicsRecordSpaceQwords / 2));

        var buffer: [EventsGraphicsRecordSpaceQwords]u64 = undefined;
        var dest: [*]u64 = &buffer;
        dest[0] = encodeEventInfo(0, .BeginEvent_NoArgs);
        dest += 1;

        dest[0] = 0xff_ff_00_ff; // Color.
        dest += 1;

        dest[0] = encodeStringInfo(0, 8, true, false);
        dest += 1;

        dest[num_name_qwords - 1] = 0;
        dest[num_name_qwords + 0] = 0;
        @memcpy(@ptrCast([*]u8, dest), name.ptr, name.len);

        context.BeginEvent(D3D12_EVENT_METADATA, @ptrCast(*anyopaque, &buffer), (3 + num_name_qwords) * 8);
    }

    fn beginEvent(target: anytype, name: []const u8) void {
        beginEventOnContext(@TypeOf(target), target, name);
    }

    fn endEventOnContext(comptime Context: type, context: Context) void {
        comptime {
            const T = @typeInfo(Context).Pointer.child;
            assert(@hasDecl(T, "EndEvent"));
        }
        context.EndEvent();
    }

    fn endEvent(target: anytype) void {
        endEventOnContext(@TypeOf(target), target);
    }
};

const empty = struct {
    fn loadGpuCapturerLibrary() ?HMODULE {
        return null;
    }
    fn beginCapture(flags: CAPTURE_FLAGS, params: ?*const CaptureParameters) HRESULT {
        _ = flags;
        _ = params;
        return w32.S_OK;
    }
    fn endCapture() HRESULT {
        return w32.S_OK;
    }
    fn setTargetWindow(hwnd: HWND) HRESULT {
        _ = hwnd;
        return w32.S_OK;
    }
    fn gpuCaptureNextFrames(file_name: LPCWSTR, num_frames: UINT32) HRESULT {
        _ = file_name;
        _ = num_frames;
        return w32.S_OK;
    }

    fn setMarker(target: anytype, name: []const u8) void {
        _ = target;
        _ = name;
    }
    fn beginEvent(target: anytype, name: []const u8) void {
        _ = target;
        _ = name;
    }
    fn endEvent(target: anytype) void {
        _ = target;
    }
};
