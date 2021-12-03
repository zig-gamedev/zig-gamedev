const std = @import("std");
const win32 = @import("win32");
const w = win32.base;
const d2d1 = win32.d2d1;
const d3d12 = win32.d3d12;
const dwrite = win32.dwrite;
const dml = win32.directml;
const xaudio2 = win32.xaudio2;
const wasapi = win32.wasapi;
const mf = win32.mf;
const common = @import("common");
const gr = common.graphics;
const lib = common.library;
const c = common.c;
const pix = common.pix;
const vm = common.vectormath;
const tracy = common.tracy;
const math = std.math;
const assert = std.debug.assert;
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

const enable_dx_debug = @import("build_options").enable_dx_debug;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: audio experiments";
const window_width = 1920;
const window_height = 1080;

const sample_rate: u32 = 48_000;

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    audio: *xaudio2.IXAudio2,
    master_voice: *xaudio2.IMasteringVoice,

    brush: *d2d1.ISolidColorBrush,
    info_tfmt: *dwrite.ITextFormat,
};

const MusicPlayer = struct {
    critical_section: w.CRITICAL_SECTION,
    gpa: *std.mem.Allocator,
    voice: *xaudio2.ISourceVoice,
    voice_cb: *VoiceCallback,
    music_reader: *mf.ISourceReader,
    music_reader_cb: *SourceReaderCallback,
    music_buffers: [4]?*mf.IMediaBuffer,
    music_buffers_head: u32,
    music_buffers_tail: u32,

    fn create(gpa: *std.mem.Allocator, audio: *xaudio2.IXAudio2) *MusicPlayer {
        const voice_cb = blk: {
            var cb = gpa.create(VoiceCallback) catch unreachable;
            cb.* = VoiceCallback.init();
            break :blk cb;
        };

        const voice = blk: {
            var voice: ?*xaudio2.ISourceVoice = null;
            hrPanicOnFail(audio.CreateSourceVoice(&voice, &.{
                .wFormatTag = wasapi.WAVE_FORMAT_PCM,
                .nChannels = 2,
                .nSamplesPerSec = sample_rate,
                .nAvgBytesPerSec = 4 * sample_rate,
                .nBlockAlign = 4,
                .wBitsPerSample = 16,
                .cbSize = @sizeOf(wasapi.WAVEFORMATEX),
            }, 0, xaudio2.DEFAULT_FREQ_RATIO, @ptrCast(*xaudio2.IVoiceCallback, voice_cb), null, null));
            break :blk voice.?;
        };

        var cs: w.CRITICAL_SECTION = undefined;
        w.kernel32.InitializeCriticalSection(&cs);

        const source_reader_cb = blk: {
            var cb = gpa.create(SourceReaderCallback) catch unreachable;
            cb.* = SourceReaderCallback.init(gpa);
            break :blk cb;
        };

        const source_reader = blk: {
            var attribs: *mf.IAttributes = undefined;
            hrPanicOnFail(mf.MFCreateAttributes(&attribs, 1));
            defer _ = attribs.Release();

            hrPanicOnFail(attribs.SetUnknown(
                &mf.SOURCE_READER_ASYNC_CALLBACK,
                @ptrCast(*w.IUnknown, source_reader_cb),
            ));

            var source_reader: *mf.ISourceReader = undefined;
            hrPanicOnFail(mf.MFCreateSourceReaderFromURL(L("content/acid_walk.mp3"), attribs, &source_reader));

            var media_type: *mf.IMediaType = undefined;
            hrPanicOnFail(source_reader.GetNativeMediaType(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, &media_type));
            defer _ = media_type.Release();

            hrPanicOnFail(media_type.SetGUID(&mf.MT_MAJOR_TYPE, &mf.MediaType_Audio));
            hrPanicOnFail(media_type.SetGUID(&mf.MT_SUBTYPE, &mf.AudioFormat_PCM));
            hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_NUM_CHANNELS, 2));
            hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_SAMPLES_PER_SECOND, sample_rate));
            hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_BITS_PER_SAMPLE, 16));
            hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_BLOCK_ALIGNMENT, 4));
            hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_AVG_BYTES_PER_SECOND, 4 * sample_rate));
            hrPanicOnFail(media_type.SetUINT32(&mf.MT_ALL_SAMPLES_INDEPENDENT, w.TRUE));
            hrPanicOnFail(source_reader.SetCurrentMediaType(mf.SOURCE_READER_FIRST_AUDIO_STREAM, null, media_type));

            break :blk source_reader;
        };

        var player = gpa.create(MusicPlayer) catch unreachable;
        player.* = .{
            .critical_section = cs,
            .gpa = gpa,
            .voice = voice,
            .voice_cb = voice_cb,
            .music_buffers = [_]?*mf.IMediaBuffer{null} ** player.music_buffers.len,
            .music_buffers_head = 0,
            .music_buffers_tail = 0,
            .music_reader = source_reader,
            .music_reader_cb = source_reader_cb,
        };

        voice_cb.player = player;
        source_reader_cb.player = player;

        return player;
    }

    fn destroy(player: *MusicPlayer) void {
        {
            const refcount = player.music_reader.Release();
            assert(refcount == 0);
        }
        {
            const refcount = player.music_reader_cb.Release();
            assert(refcount == 0);
        }
        w.kernel32.DeleteCriticalSection(&player.critical_section);
        player.voice.DestroyVoice();
        player.gpa.destroy(player.voice_cb);
        player.gpa.destroy(player);
    }

    fn onBufferEnd(player: *MusicPlayer) void {
        w.kernel32.EnterCriticalSection(&player.critical_section);
        defer w.kernel32.LeaveCriticalSection(&player.critical_section);

        const buffer = player.music_buffers[player.music_buffers_head].?;
        player.music_buffers_head = (player.music_buffers_head + 1) % @intCast(u32, player.music_buffers.len);

        hrPanicOnFail(buffer.Unlock());
        const refcount = buffer.Release();
        assert(refcount == 0);

        hrPanicOnFail(player.music_reader.ReadSample(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, null, null, null, null));
    }

    fn onReadSample(
        player: *MusicPlayer,
        status: w.HRESULT,
        _: w.DWORD,
        stream_flags: w.DWORD,
        _: w.LONGLONG,
        sample: ?*mf.ISample,
    ) void {
        if (status != w.S_OK or sample == null) {
            return;
        }

        w.kernel32.EnterCriticalSection(&player.critical_section);
        defer w.kernel32.LeaveCriticalSection(&player.critical_section);

        _ = stream_flags;

        var buffer: *mf.IMediaBuffer = undefined;
        hrPanicOnFail(sample.?.ConvertToContiguousBuffer(&buffer));

        var data_ptr: [*]u8 = undefined;
        var data_len: u32 = 0;
        hrPanicOnFail(buffer.Lock(&data_ptr, null, &data_len));

        hrPanicOnFail(player.voice.SubmitSourceBuffer(&.{
            .Flags = 0,
            .AudioBytes = data_len,
            .pAudioData = data_ptr,
            .PlayBegin = 0,
            .PlayLength = 0,
            .LoopBegin = 0,
            .LoopLength = 0,
            .LoopCount = 0,
            .pContext = null,
        }, null));

        player.music_buffers[player.music_buffers_tail] = buffer;
        player.music_buffers_tail = (player.music_buffers_tail + 1) % @intCast(u32, player.music_buffers.len);
    }

    const VoiceCallback = struct {
        vtable: *const xaudio2.IVoiceCallbackVTable(VoiceCallback) = &vtable_voice_cb,
        player: ?*MusicPlayer,

        fn init() VoiceCallback {
            return .{ .player = null };
        }

        fn OnBufferEnd(voice_cb: *VoiceCallback, _: ?*c_void) callconv(w.WINAPI) void {
            voice_cb.player.?.onBufferEnd();
        }

        const vtable_voice_cb = xaudio2.IVoiceCallbackVTable(VoiceCallback){
            .vcb = .{
                .OnVoiceProcessingPassStart = VoiceCallback.OnVoiceProcessingPassStart,
                .OnVoiceProcessingPassEnd = OnVoiceProcessingPassEnd,
                .OnStreamEnd = VoiceCallback.OnStreamEnd,
                .OnBufferStart = VoiceCallback.OnBufferStart,
                .OnBufferEnd = VoiceCallback.OnBufferEnd,
                .OnLoopEnd = VoiceCallback.OnLoopEnd,
                .OnVoiceError = VoiceCallback.OnVoiceError,
            },
        };

        fn OnVoiceProcessingPassStart(_: *VoiceCallback, _: w.UINT32) callconv(w.WINAPI) void {}
        fn OnVoiceProcessingPassEnd(_: *VoiceCallback) callconv(w.WINAPI) void {}
        fn OnStreamEnd(_: *VoiceCallback) callconv(w.WINAPI) void {}
        fn OnBufferStart(_: *VoiceCallback, _: ?*c_void) callconv(w.WINAPI) void {}
        fn OnLoopEnd(_: *VoiceCallback, _: ?*c_void) callconv(w.WINAPI) void {}
        fn OnVoiceError(_: *VoiceCallback, _: ?*c_void, _: w.HRESULT) callconv(w.WINAPI) void {}
    };

    const SourceReaderCallback = struct {
        vtable: *const mf.ISourceReaderCallbackVTable(SourceReaderCallback) = &vtable_source_reader_cb,
        refcount: u32 = 1,
        gpa: *std.mem.Allocator,
        player: ?*MusicPlayer,

        fn init(gpa: *std.mem.Allocator) SourceReaderCallback {
            return .{
                .gpa = gpa,
                .player = null,
            };
        }

        const vtable_source_reader_cb = mf.ISourceReaderCallbackVTable(SourceReaderCallback){
            .unknown = .{
                .QueryInterface = SourceReaderCallback.QueryInterface,
                .AddRef = SourceReaderCallback.AddRef,
                .Release = SourceReaderCallback.Release,
            },
            .cb = .{
                .OnReadSample = SourceReaderCallback.OnReadSample,
                .OnFlush = SourceReaderCallback.OnFlush,
                .OnEvent = SourceReaderCallback.OnEvent,
            },
        };

        fn QueryInterface(
            source_reader_cb: *SourceReaderCallback,
            guid: *const w.GUID,
            outobj: ?*?*c_void,
        ) callconv(w.WINAPI) w.HRESULT {
            assert(outobj != null);

            if (std.mem.eql(u8, std.mem.asBytes(guid), std.mem.asBytes(&w.IID_IUnknown))) {
                outobj.?.* = source_reader_cb;
                _ = source_reader_cb.AddRef();
                return w.S_OK;
            } else if (std.mem.eql(u8, std.mem.asBytes(guid), std.mem.asBytes(&mf.IID_ISourceReaderCallback))) {
                outobj.?.* = source_reader_cb;
                _ = source_reader_cb.AddRef();
                return w.S_OK;
            }

            outobj.?.* = null;
            return w.E_NOINTERFACE;
        }

        fn AddRef(source_reader_cb: *SourceReaderCallback) callconv(w.WINAPI) w.ULONG {
            const prev_refcount = @atomicRmw(u32, &source_reader_cb.refcount, .Add, 1, .Monotonic);
            return prev_refcount + 1;
        }

        fn Release(source_reader_cb: *SourceReaderCallback) callconv(w.WINAPI) w.ULONG {
            const prev_refcount = @atomicRmw(u32, &source_reader_cb.refcount, .Sub, 1, .Monotonic);
            assert(prev_refcount > 0);
            if (prev_refcount == 1) {
                source_reader_cb.gpa.destroy(source_reader_cb);
            }
            return prev_refcount - 1;
        }

        fn OnReadSample(
            source_reader_cb: *SourceReaderCallback,
            status: w.HRESULT,
            stream_index: w.DWORD,
            stream_flags: w.DWORD,
            timestamp: w.LONGLONG,
            sample: ?*mf.ISample,
        ) callconv(w.WINAPI) w.HRESULT {
            source_reader_cb.player.?.onReadSample(status, stream_index, stream_flags, timestamp, sample);
            return w.S_OK;
        }

        fn OnFlush(_: *SourceReaderCallback, _: w.DWORD) callconv(w.WINAPI) w.HRESULT {
            return w.S_OK;
        }

        fn OnEvent(_: *SourceReaderCallback, _: w.DWORD, _: *mf.IMediaEvent) callconv(w.WINAPI) w.HRESULT {
            return w.S_OK;
        }
    };
};

fn loadAudioBuffer(gpa: *std.mem.Allocator, audio_file_path: [:0]const u16) std.ArrayList(u8) {
    const tracy_zone = tracy.zone(@src(), 1);
    defer tracy_zone.end();

    var source_reader: *mf.ISourceReader = undefined;
    hrPanicOnFail(mf.MFCreateSourceReaderFromURL(audio_file_path, null, &source_reader));
    defer _ = source_reader.Release();

    var media_type: *mf.IMediaType = undefined;
    hrPanicOnFail(source_reader.GetNativeMediaType(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, &media_type));
    defer _ = media_type.Release();

    hrPanicOnFail(media_type.SetGUID(&mf.MT_MAJOR_TYPE, &mf.MediaType_Audio));
    hrPanicOnFail(media_type.SetGUID(&mf.MT_SUBTYPE, &mf.AudioFormat_PCM));
    hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_NUM_CHANNELS, 1));
    hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_SAMPLES_PER_SECOND, sample_rate));
    hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_BITS_PER_SAMPLE, 16));
    hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_BLOCK_ALIGNMENT, 2));
    hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_AVG_BYTES_PER_SECOND, 2 * sample_rate));
    hrPanicOnFail(media_type.SetUINT32(&mf.MT_ALL_SAMPLES_INDEPENDENT, w.TRUE));
    hrPanicOnFail(source_reader.SetCurrentMediaType(mf.SOURCE_READER_FIRST_AUDIO_STREAM, null, media_type));

    var audio_samples = std.ArrayList(u8).init(gpa);
    while (true) {
        var flags: w.DWORD = 0;
        var sample: ?*mf.ISample = null;
        defer {
            if (sample) |s| _ = s.Release();
        }
        hrPanicOnFail(source_reader.ReadSample(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, null, &flags, null, &sample));
        if ((flags & mf.SOURCE_READERF_ENDOFSTREAM) != 0) {
            break;
        }

        var buffer: *mf.IMediaBuffer = undefined;
        hrPanicOnFail(sample.?.ConvertToContiguousBuffer(&buffer));
        defer _ = buffer.Release();

        var data_ptr: [*]u8 = undefined;
        var data_len: u32 = 0;
        hrPanicOnFail(buffer.Lock(&data_ptr, null, &data_len));
        audio_samples.appendSlice(data_ptr[0..data_len]) catch unreachable;
        hrPanicOnFail(buffer.Unlock());
    }
    return audio_samples;
}

fn init(gpa: *std.mem.Allocator) DemoState {
    const tracy_zone = tracy.zone(@src(), 1);
    defer tracy_zone.end();

    const audio = blk: {
        var audio: ?*xaudio2.IXAudio2 = null;
        hrPanicOnFail(xaudio2.create(&audio, if (enable_dx_debug) xaudio2.DEBUG_ENGINE else 0, 0));
        break :blk audio.?;
    };

    if (enable_dx_debug) {
        audio.SetDebugConfiguration(&.{
            .TraceMask = xaudio2.LOG_ERRORS | xaudio2.LOG_WARNINGS | xaudio2.LOG_INFO | xaudio2.LOG_API_CALLS,
            .BreakMask = 0,
            .LogThreadID = w.TRUE,
            .LogFileline = w.FALSE,
            .LogFunctionName = w.FALSE,
            .LogTiming = w.FALSE,
        }, null);
    }

    const master_voice = blk: {
        var master_voice: ?*xaudio2.IMasteringVoice = null;
        hrPanicOnFail(audio.CreateMasteringVoice(
            &master_voice,
            xaudio2.DEFAULT_CHANNELS,
            xaudio2.DEFAULT_SAMPLERATE,
            0,
            null,
            null,
            .GameEffects,
        ));
        break :blk master_voice.?;
    };

    hrPanicOnFail(mf.MFStartup(mf.VERSION, 0));
    defer _ = mf.MFShutdown();

    const samples = loadAudioBuffer(gpa, L("content/drum_bass_hard.flac"));
    defer samples.deinit();

    var music = MusicPlayer.create(gpa, audio);
    hrPanicOnFail(music.voice.Start(0, xaudio2.COMMIT_NOW));

    hrPanicOnFail(music.music_reader.ReadSample(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, null, null, null, null));
    hrPanicOnFail(music.music_reader.ReadSample(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, null, null, null, null));
    hrPanicOnFail(music.music_reader.ReadSample(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, null, null, null, null));

    std.log.info("Playing music...", .{});

    w.kernel32.Sleep(3000);
    hrPanicOnFail(music.voice.Stop(0, xaudio2.COMMIT_NOW));

    music.destroy();

    const source_voice0 = blk: {
        var voice: ?*xaudio2.ISourceVoice = null;
        hrPanicOnFail(audio.CreateSourceVoice(&voice, &.{
            .wFormatTag = wasapi.WAVE_FORMAT_PCM,
            .nChannels = 1,
            .nSamplesPerSec = sample_rate,
            .nAvgBytesPerSec = 2 * sample_rate,
            .nBlockAlign = 2,
            .wBitsPerSample = 16,
            .cbSize = @sizeOf(wasapi.WAVEFORMATEX),
        }, 0, xaudio2.DEFAULT_FREQ_RATIO, null, null, null));
        break :blk voice.?;
    };
    defer source_voice0.DestroyVoice();

    const source_voice1 = blk: {
        var voice: ?*xaudio2.ISourceVoice = null;
        hrPanicOnFail(audio.CreateSourceVoice(&voice, &.{
            .wFormatTag = wasapi.WAVE_FORMAT_PCM,
            .nChannels = 1,
            .nSamplesPerSec = sample_rate,
            .nAvgBytesPerSec = 2 * sample_rate,
            .nBlockAlign = 2,
            .wBitsPerSample = 16,
            .cbSize = @sizeOf(wasapi.WAVEFORMATEX),
        }, 0, xaudio2.DEFAULT_FREQ_RATIO, null, null, null));
        break :blk voice.?;
    };
    defer source_voice1.DestroyVoice();

    hrPanicOnFail(source_voice0.SubmitSourceBuffer(&.{
        .Flags = xaudio2.END_OF_STREAM,
        .AudioBytes = @intCast(u32, samples.items.len),
        .pAudioData = samples.items.ptr,
        .PlayBegin = 0,
        .PlayLength = 0,
        .LoopBegin = 0,
        .LoopLength = 0,
        .LoopCount = 0,
        .pContext = null,
    }, null));
    hrPanicOnFail(source_voice0.Start(0, xaudio2.COMMIT_NOW));

    w.kernel32.Sleep(100);

    hrPanicOnFail(source_voice1.SubmitSourceBuffer(&.{
        .Flags = xaudio2.END_OF_STREAM,
        .AudioBytes = @intCast(u32, samples.items.len),
        .pAudioData = samples.items.ptr,
        .PlayBegin = 0,
        .PlayLength = 0,
        .LoopBegin = 0,
        .LoopLength = 0,
        .LoopCount = 0,
        .pContext = null,
    }, null));
    hrPanicOnFail(source_voice1.Start(0, xaudio2.COMMIT_NOW));

    w.kernel32.Sleep(1000);

    const window = lib.initWindow(gpa, window_name, window_width, window_height) catch unreachable;

    var arena_allocator = std.heap.ArenaAllocator.init(gpa);
    defer arena_allocator.deinit();

    _ = pix.loadGpuCapturerLibrary();
    _ = pix.setTargetWindow(window);
    _ = pix.beginCapture(
        pix.CAPTURE_GPU,
        &pix.CaptureParameters{ .gpu_capture_params = .{ .FileName = L("capture.wpix") } },
    );

    var grfx = gr.GraphicsContext.init(window);
    grfx.present_flags = 0;
    grfx.present_interval = 1;

    const brush = blk: {
        var brush: *d2d1.ISolidColorBrush = undefined;
        hrPanicOnFail(grfx.d2d.context.CreateSolidColorBrush(
            &.{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
            null,
            @ptrCast(*?*d2d1.ISolidColorBrush, &brush),
        ));
        break :blk brush;
    };

    const info_tfmt = blk: {
        var info_txtfmt: *dwrite.ITextFormat = undefined;
        hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
            L("Verdana"),
            null,
            dwrite.FONT_WEIGHT.BOLD,
            dwrite.FONT_STYLE.NORMAL,
            dwrite.FONT_STRETCH.NORMAL,
            32.0,
            L("en-us"),
            @ptrCast(*?*dwrite.ITextFormat, &info_txtfmt),
        ));
        break :blk info_txtfmt;
    };
    hrPanicOnFail(info_tfmt.SetTextAlignment(.LEADING));
    hrPanicOnFail(info_tfmt.SetParagraphAlignment(.NEAR));

    //
    // Begin frame to init/upload resources to the GPU.
    //
    grfx.beginFrame();
    grfx.endFrame();
    grfx.beginFrame();

    pix.beginEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist), "GPU init");

    var gui = gr.GuiContext.init(&arena_allocator.allocator, &grfx, 1);

    _ = pix.endEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist));

    grfx.endFrame();
    grfx.finishGpuCommands();

    _ = pix.endCapture();

    return .{
        .grfx = grfx,
        .gui = gui,
        .audio = audio,
        .master_voice = master_voice,
        .frame_stats = lib.FrameStats.init(),
        .brush = brush,
        .info_tfmt = info_tfmt,
    };
}

fn deinit(demo: *DemoState, gpa: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    _ = demo.brush.Release();
    _ = demo.info_tfmt.Release();
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit();
    demo.audio.StopEngine();
    demo.master_voice.DestroyVoice();
    _ = demo.audio.Release();
    lib.deinitWindow(gpa);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();
    const dt = demo.frame_stats.delta_time;
    lib.newImGuiFrame(dt);
}

fn draw(demo: *DemoState) void {
    var grfx = &demo.grfx;
    grfx.beginFrame();

    const back_buffer = grfx.getBackBuffer();
    grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
    grfx.flushResourceBarriers();

    grfx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w.TRUE,
        null,
    );
    grfx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );

    demo.gui.draw(grfx);

    grfx.beginDraw2d();
    {
        const stats = &demo.frame_stats;
        var buffer = [_]u8{0} ** 64;
        const text = std.fmt.bufPrint(
            buffer[0..],
            "FPS: {d:.1}\nCPU time: {d:.3} ms",
            .{ stats.fps, stats.average_cpu_time },
        ) catch unreachable;

        demo.brush.SetColor(&.{ .r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0 });
        lib.drawText(
            grfx.d2d.context,
            text,
            demo.info_tfmt,
            &d2d1.RECT_F{
                .left = 10.0,
                .top = 10.0,
                .right = @intToFloat(f32, grfx.viewport_width),
                .bottom = @intToFloat(f32, grfx.viewport_height),
            },
            @ptrCast(*d2d1.IBrush, demo.brush),
        );
    }
    grfx.endDraw2d();

    grfx.endFrame();
}

pub fn main() !void {
    lib.init();
    defer lib.deinit();

    var gpa_allocator = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa_allocator.deinit();
        std.debug.assert(leaked == false);
    }
    const gpa = &gpa_allocator.allocator;

    var demo = init(gpa);
    defer deinit(&demo, gpa);

    while (true) {
        var message = std.mem.zeroes(w.user32.MSG);
        const has_message = w.user32.peekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) catch false;
        if (has_message) {
            _ = w.user32.translateMessage(&message);
            _ = w.user32.dispatchMessageA(&message);
            if (message.message == w.user32.WM_QUIT) {
                break;
            }
        } else {
            update(&demo);
            draw(&demo);
        }
    }
}
