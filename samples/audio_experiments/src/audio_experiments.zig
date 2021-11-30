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

fn loadAudioBuffer(gpa: *std.mem.Allocator, audio_file_path: [:0]const u16) std.ArrayList(u8) {
    var config_attribs: *mf.IAttributes = undefined;
    hrPanicOnFail(mf.MFCreateAttributes(&config_attribs, 1));
    defer _ = config_attribs.Release();
    hrPanicOnFail(config_attribs.SetUINT32(&mf.LOW_LATENCY, w.TRUE));

    var source_reader: *mf.ISourceReader = undefined;
    hrPanicOnFail(mf.MFCreateSourceReaderFromURL(audio_file_path, config_attribs, &source_reader));
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
            .TraceMask = xaudio2.LOG_DETAIL | xaudio2.LOG_API_CALLS,
            .BreakMask = 0,
            .LogThreadID = w.TRUE,
            .LogFileline = w.TRUE,
            .LogFunctionName = w.TRUE,
            .LogTiming = w.TRUE,
        }, null);
    }

    hrPanicOnFail(mf.MFStartup(mf.VERSION, 0));
    defer _ = mf.MFShutdown();

    const samples = loadAudioBuffer(gpa, L("content/drum_bass_hard.flac")[0.. :0]);
    defer samples.deinit();

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

    const source_voice = blk: {
        var source_voice: ?*xaudio2.ISourceVoice = null;
        hrPanicOnFail(audio.CreateSourceVoice(&source_voice, &.{
            .wFormatTag = wasapi.WAVE_FORMAT_PCM,
            .nChannels = 1,
            .nSamplesPerSec = sample_rate,
            .nAvgBytesPerSec = 2 * sample_rate,
            .nBlockAlign = 2,
            .wBitsPerSample = 16,
            .cbSize = @sizeOf(wasapi.WAVEFORMATEX),
        }, 0, xaudio2.DEFAULT_FREQ_RATIO, null, null, null));
        break :blk source_voice.?;
    };
    defer source_voice.DestroyVoice();

    hrPanicOnFail(source_voice.SubmitSourceBuffer(&.{
        .Flags = 0,
        .AudioBytes = @intCast(u32, samples.items.len),
        .pAudioData = samples.items.ptr,
        .PlayBegin = 0,
        .PlayLength = 0,
        .LoopBegin = 0,
        .LoopLength = 0,
        .LoopCount = 0,
        .pContext = null,
    }, null));

    hrPanicOnFail(source_voice.Start(0, xaudio2.COMMIT_NOW));

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
    const dt = demo.frame_stats.delta_time;
    demo.frame_stats.update();
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
