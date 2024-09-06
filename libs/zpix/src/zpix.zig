const std = @import("std");
const assert = std.debug.assert;

comptime {
    std.testing.refAllDecls(@This());
}

const windows = std.os.windows;
const HMODULE = windows.HMODULE;
const HRESULT = windows.HRESULT;
const GUID = windows.GUID;
const LPCSTR = windows.LPCSTR;
const LPWSTR = windows.LPWSTR;
const LPCWSTR = windows.LPCWSTR;
const FARPROC = windows.FARPROC;
const HWND = windows.HWND;
const WINAPI = windows.WINAPI;
const BOOL = windows.BOOL;
const DWORD = windows.DWORD;
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
    const module = windows.GetModuleHandleA("WinPixGpuCapturer.dll");
    if (module == null) {
        return null;
    }

    const func = windows.GetProcAddress(module.?, func_name);
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
    const mask0 = (timestamp & EventsTimestampWriteMask) << @as(u6, @intCast(EventsTimestampBitShift));
    const mask1 = (@intFromEnum(event_type) & EventsTypeWriteMask) << @as(u6, @intCast(EventsTypeBitShift));
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
        @as(u6, @intCast(EventsStringAlignmentBitShift));
    const mask1 = (copy_chunk_size & EventsStringCopyChunkSizeWriteMask) <<
        @as(u6, @intCast(EventsStringCopyChunkSizeBitShift));
    const mask2 = (@intFromBool(is_ansi) & EventsStringIsANSIWriteMask) <<
        @as(u6, @intCast(EventsStringIsANSIBitShift));
    const mask3 = (@intFromBool(is_shortcut) & EventsStringIsShortcutWriteMask) <<
        @as(u6, @intCast(EventsStringIsShortcutBitShift));
    return mask0 | mask1 | mask2 | mask3;
}

const PixLibrary = if (enable) struct {
    module: HMODULE,

    pub fn deinit(self: PixLibrary) void {
        windows.FreeLibrary(self.module);
    }
} else struct {
    pub fn deinit(_: PixLibrary) void {}
};

const impl = struct {
    fn loadGpuCapturerLibrary() !PixLibrary {
        const dll_path = dll_path: {
            var buffer: [2048]u8 = undefined;
            var fba = std.heap.FixedBufferAllocator.init(buffer[0..]);
            const allocator = fba.allocator();

            break :dll_path try std.fs.path.joinZ(
                allocator,
                &.{ options.path, "WinPixGpuCapturer.dll" },
            );
        };

        if (windows.LoadLibraryA(dll_path.ptr)) |m| {
            return .{ .module = m };
        } else {
            // unable to reuse same allocator for dll_path due to https://github.com/ziglang/zig/issues/15850
            var buffer: [2048]u8 = undefined;
            var fba = std.heap.FixedBufferAllocator.init(buffer[0..]);
            const allocator = fba.allocator();
            @panic(try std.fmt.allocPrint(allocator, "failed to load library: {s}\n", .{dll_path}));
        }
    }

    fn beginCapture(flags: CAPTURE_FLAGS, params: ?*const CaptureParameters) HRESULT {
        if (flags.GPU) {
            const beginProgrammaticGpuCapture = @as(
                ?*const fn (?*const CaptureParameters) callconv(WINAPI) HRESULT,
                @ptrCast(getFunctionPtr("BeginProgrammaticGpuCapture")),
            );
            if (beginProgrammaticGpuCapture == null) {
                return windows.E_FAIL;
            }
            return beginProgrammaticGpuCapture.?(params);
        } else {
            return windows.E_NOTIMPL;
        }
    }

    fn endCapture() HRESULT {
        const endProgrammaticGpuCapture = @as(
            ?*const fn () callconv(WINAPI) HRESULT,
            @ptrCast(getFunctionPtr("EndProgrammaticGpuCapture")),
        );
        if (endProgrammaticGpuCapture == null) {
            return windows.E_FAIL;
        }
        return endProgrammaticGpuCapture.?();
    }

    fn setTargetWindow(hwnd: HWND) HRESULT {
        const setGlobalTargetWindow = @as(
            ?*const fn (HWND) callconv(WINAPI) void,
            @ptrCast(getFunctionPtr("SetGlobalTargetWindow")),
        );
        if (setGlobalTargetWindow == null) {
            return windows.E_FAIL;
        }
        setGlobalTargetWindow.?(hwnd);
        return windows.S_OK;
    }

    fn gpuCaptureNextFrames(file_name: LPCWSTR, num_frames: UINT32) HRESULT {
        const captureNextFrame = @as(
            ?*const fn (LPCWSTR, UINT32) callconv(WINAPI) HRESULT,
            @ptrCast(getFunctionPtr("CaptureNextFrame")),
        );
        if (captureNextFrame == null) {
            return windows.E_FAIL;
        }
        return captureNextFrame.?(file_name, num_frames);
    }

    fn setMarkerOnContext(comptime Context: type, context: Context, name: []const u8) void {
        comptime {
            const T = @typeInfo(Context).Pointer.child;
            assert(@hasDecl(T, "SetMarker"));
        }
        assert(name.len > 0);
        const num_name_qwords: u32 = (@as(u32, @intCast(name.len + 1)) + 7) / 8;
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
        @memcpy(@as([*]u8, @ptrCast(dest)), name);

        context.SetMarker(D3D12_EVENT_METADATA, @as(*anyopaque, @ptrCast(&buffer)), (3 + num_name_qwords) * 8);
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
        const num_name_qwords: u32 = (@as(u32, @intCast(name.len + 1)) + 7) / 8;
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
        @memcpy(@as([*]u8, @ptrCast(dest)), name);

        context.BeginEvent(D3D12_EVENT_METADATA, @as(*anyopaque, @ptrCast(&buffer)), (3 + num_name_qwords) * 8);
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
    fn loadGpuCapturerLibrary() !PixLibrary {
        return .{};
    }
    fn beginCapture(flags: CAPTURE_FLAGS, params: ?*const CaptureParameters) HRESULT {
        _ = flags;
        _ = params;
        return windows.S_OK;
    }
    fn endCapture() HRESULT {
        return windows.S_OK;
    }
    fn setTargetWindow(hwnd: HWND) HRESULT {
        _ = hwnd;
        return windows.S_OK;
    }
    fn gpuCaptureNextFrames(file_name: LPCWSTR, num_frames: UINT32) HRESULT {
        _ = file_name;
        _ = num_frames;
        return windows.S_OK;
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
