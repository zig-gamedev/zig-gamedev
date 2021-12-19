const std = @import("std");
const win32 = @import("win32");
const w = win32.base;
const xaudio2 = win32.xaudio2;
const assert = std.debug.assert;
const lib = @import("library.zig");
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

const enable_dx_debug = @import("build_options").enable_dx_debug;

pub const AudioContext = struct {
    device: *xaudio2.IXAudio2,
    master_voice: *xaudio2.IMasteringVoice,

    pub fn init() AudioContext {
        const device = blk: {
            var device: ?*xaudio2.IXAudio2 = null;
            hrPanicOnFail(xaudio2.create(&device, if (enable_dx_debug) xaudio2.DEBUG_ENGINE else 0, 0));
            break :blk device.?;
        };

        if (enable_dx_debug) {
            device.SetDebugConfiguration(&.{
                .TraceMask = xaudio2.LOG_ERRORS | xaudio2.LOG_WARNINGS | xaudio2.LOG_INFO,
                .BreakMask = 0,
                .LogThreadID = w.TRUE,
                .LogFileline = w.FALSE,
                .LogFunctionName = w.FALSE,
                .LogTiming = w.FALSE,
            }, null);
        }

        const master_voice = blk: {
            var voice: ?*xaudio2.IMasteringVoice = null;
            hrPanicOnFail(device.CreateMasteringVoice(
                &voice,
                xaudio2.DEFAULT_CHANNELS,
                xaudio2.DEFAULT_SAMPLERATE,
                0,
                null,
                null,
                .GameEffects,
            ));
            break :blk voice.?;
        };

        return .{
            .device = device,
            .master_voice = master_voice,
        };
    }

    pub fn deinit(audio: *AudioContext) void {
        audio.device.StopEngine();
        audio.master_voice.DestroyVoice();
        _ = audio.device.Release();
        audio.* = undefined;
    }
};
