const std = @import("std");
const win32 = @import("win32");
const w = win32.base;
const d2d1 = win32.d2d1;
const d3d12 = win32.d3d12;
const dwrite = win32.dwrite;
const wasapi = win32.wasapi;
const mf = win32.mf;
const common = @import("common");
const gr = common.graphics;
const lib = common.library;
const c = common.c;
const vm = common.vectormath;
const math = std.math;
const assert = std.debug.assert;
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const Vec2 = vm.Vec2;

const num_vis_samples = 200;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: audio playback test";
const window_width = 1920;
const window_height = 1080;

const AudioContex = struct {
    client: *wasapi.IAudioClient3,
    render_client: *wasapi.IAudioRenderClient,
    buffer_ready_event: w.HANDLE,
    buffer_size_in_frames: u32,
    thread_handle: ?w.HANDLE,
    samples: std.ArrayList(i16),
    current_frame_index: u32,
    is_locked: bool,
};

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    brush: *d2d1.ISolidColorBrush,
    textformat: *dwrite.ITextFormat,

    audio: AudioContex,

    lines_pso: gr.PipelineHandle,
    lines_buffer: gr.ResourceHandle,
};

fn fillAudioBuffer(audio: *AudioContex) void {
    while (@cmpxchgWeak(bool, &audio.is_locked, false, true, .Acquire, .Monotonic) != null) {}
    defer @atomicStore(bool, &audio.is_locked, false, .Release);

    var buffer_padding_in_frames: w.UINT = 0;
    hrPanicOnFail(audio.client.GetCurrentPadding(&buffer_padding_in_frames));

    const num_frames = audio.buffer_size_in_frames - buffer_padding_in_frames;

    var ptr: [*]f32 = undefined;
    hrPanicOnFail(audio.render_client.GetBuffer(num_frames, @ptrCast(*?[*]w.BYTE, &ptr)));

    var i: u32 = 0;
    while (i < num_frames) : (i += 1) {
        const frame = audio.current_frame_index;
        ptr[i * 2 + 0] = @intToFloat(f32, audio.samples.items[frame * 2 + 0]) / @intToFloat(f32, 0x7fff);
        ptr[i * 2 + 1] = @intToFloat(f32, audio.samples.items[frame * 2 + 1]) / @intToFloat(f32, 0x7fff);

        audio.current_frame_index += 1;
        if (audio.current_frame_index * 2 >= audio.samples.items.len) {
            audio.current_frame_index = 0;
        }
    }
    hrPanicOnFail(audio.render_client.ReleaseBuffer(num_frames, 0));
}

fn audioThread(ctx: ?*c_void) callconv(.C) w.DWORD {
    const audio = @ptrCast(*AudioContex, @alignCast(8, ctx));

    fillAudioBuffer(audio);
    while (true) {
        w.WaitForSingleObject(audio.buffer_ready_event, w.INFINITE) catch return 0;
        fillAudioBuffer(audio);
    }

    return 0;
}

fn init(gpa: *std.mem.Allocator) DemoState {
    const window = lib.initWindow(gpa, window_name, window_width, window_height) catch unreachable;
    var grfx = gr.GraphicsContext.init(window);
    grfx.present_flags = 0;
    grfx.present_interval = 1;

    var arena_allocator = std.heap.ArenaAllocator.init(gpa);
    defer arena_allocator.deinit();

    const brush = blk: {
        var maybe_brush: ?*d2d1.ISolidColorBrush = null;
        hrPanicOnFail(grfx.d2d.context.CreateSolidColorBrush(
            &d2d1.COLOR_F{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
            null,
            &maybe_brush,
        ));
        break :blk maybe_brush.?;
    };

    const textformat = blk: {
        var maybe_textformat: ?*dwrite.ITextFormat = null;
        hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
            L("Verdana"),
            null,
            dwrite.FONT_WEIGHT.NORMAL,
            dwrite.FONT_STYLE.NORMAL,
            dwrite.FONT_STRETCH.NORMAL,
            32.0,
            L("en-us"),
            &maybe_textformat,
        ));
        break :blk maybe_textformat.?;
    };
    hrPanicOnFail(textformat.SetTextAlignment(.LEADING));
    hrPanicOnFail(textformat.SetParagraphAlignment(.NEAR));

    const audio_device_enumerator = blk: {
        var audio_device_enumerator: *wasapi.IMMDeviceEnumerator = undefined;
        hrPanicOnFail(w.CoCreateInstance(
            &wasapi.CLSID_MMDeviceEnumerator,
            null,
            w.CLSCTX_INPROC_SERVER,
            &wasapi.IID_IMMDeviceEnumerator,
            @ptrCast(*?*c_void, &audio_device_enumerator),
        ));
        break :blk audio_device_enumerator;
    };
    defer _ = audio_device_enumerator.Release();

    const audio_device = blk: {
        var audio_device: *wasapi.IMMDevice = undefined;
        hrPanicOnFail(audio_device_enumerator.GetDefaultAudioEndpoint(
            .eRender,
            .eConsole,
            @ptrCast(*?*wasapi.IMMDevice, &audio_device),
        ));
        break :blk audio_device;
    };
    defer _ = audio_device.Release();

    const audio_client = blk: {
        var audio_client: *wasapi.IAudioClient3 = undefined;
        hrPanicOnFail(audio_device.Activate(
            &wasapi.IID_IAudioClient3,
            w.CLSCTX_INPROC_SERVER,
            null,
            @ptrCast(*?*c_void, &audio_client),
        ));
        break :blk audio_client;
    };

    // Initialize audio client interafce.
    {
        var closest_format: ?*wasapi.WAVEFORMATEX = null;
        const wanted_format = wasapi.WAVEFORMATEX{
            .wFormatTag = wasapi.WAVE_FORMAT_IEEE_FLOAT,
            .nChannels = 2,
            .nSamplesPerSec = 48_000,
            .nAvgBytesPerSec = 48_000 * 8,
            .nBlockAlign = 8,
            .wBitsPerSample = 32,
            .cbSize = 0,
        };
        hrPanicOnFail(audio_client.IsFormatSupported(.SHARED, &wanted_format, &closest_format));
        assert(closest_format == null);

        hrPanicOnFail(audio_client.Initialize(
            .SHARED,
            wasapi.AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
            0,
            0,
            &wanted_format,
            null,
        ));
    }

    const audio_render_client = blk: {
        var audio_render_client: *wasapi.IAudioRenderClient = undefined;
        hrPanicOnFail(audio_client.GetService(
            &wasapi.IID_IAudioRenderClient,
            @ptrCast(*?*c_void, &audio_render_client),
        ));
        break :blk audio_render_client;
    };

    const audio_buffer_ready_event = w.CreateEventEx(
        null,
        "audio_buffer_ready_event",
        0,
        w.EVENT_ALL_ACCESS,
    ) catch unreachable;

    hrPanicOnFail(audio_client.SetEventHandle(audio_buffer_ready_event));

    var audio_buffer_size_in_frames: w.UINT = 0;
    hrPanicOnFail(audio_client.GetBufferSize(&audio_buffer_size_in_frames));

    const audio_samples = blk: {
        hrPanicOnFail(mf.MFStartup(mf.VERSION, 0));
        defer _ = mf.MFShutdown();

        var config_attribs: *mf.IAttributes = undefined;
        hrPanicOnFail(mf.MFCreateAttributes(&config_attribs, 1));
        defer _ = config_attribs.Release();
        hrPanicOnFail(config_attribs.SetUINT32(&mf.LOW_LATENCY, w.TRUE));

        var source_reader: *mf.ISourceReader = undefined;
        hrPanicOnFail(mf.MFCreateSourceReaderFromURL(L("content/acid_walk.mp3"), config_attribs, &source_reader));
        defer _ = source_reader.Release();

        var media_type: *mf.IMediaType = undefined;
        hrPanicOnFail(source_reader.GetNativeMediaType(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, &media_type));
        defer _ = media_type.Release();

        hrPanicOnFail(media_type.SetGUID(&mf.MT_MAJOR_TYPE, &mf.MediaType_Audio));
        hrPanicOnFail(media_type.SetGUID(&mf.MT_SUBTYPE, &mf.AudioFormat_PCM));
        hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_NUM_CHANNELS, 2));
        hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_BITS_PER_SAMPLE, 16));
        hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_SAMPLES_PER_SECOND, 48_000));
        hrPanicOnFail(source_reader.SetCurrentMediaType(mf.SOURCE_READER_FIRST_AUDIO_STREAM, null, media_type));

        var audio_samples = std.ArrayList(i16).init(gpa);
        while (true) {
            var flags: w.DWORD = 0;
            var sample: ?*mf.ISample = null;
            defer {
                if (sample != null) {
                    _ = sample.?.Release();
                }
            }
            hrPanicOnFail(source_reader.ReadSample(
                mf.SOURCE_READER_FIRST_AUDIO_STREAM,
                0,
                null,
                &flags,
                null,
                &sample,
            ));
            if ((flags & mf.SOURCE_READERF_ENDOFSTREAM) != 0) {
                break;
            }

            var buffer: *mf.IMediaBuffer = undefined;
            hrPanicOnFail(sample.?.ConvertToContiguousBuffer(&buffer));
            defer _ = buffer.Release();

            var data_ptr: [*]i16 = undefined;
            var data_len: u32 = 0;
            hrPanicOnFail(buffer.Lock(@ptrCast(*[*]u8, &data_ptr), null, &data_len));
            const data = data_ptr[0..@divExact(data_len, 2)];

            for (data) |s| {
                audio_samples.append(s) catch unreachable;
            }
            hrPanicOnFail(buffer.Unlock());
        }
        break :blk audio_samples;
    };

    const lines_pso = blk: {
        const input_layout_desc = [_]d3d12.INPUT_ELEMENT_DESC{
            d3d12.INPUT_ELEMENT_DESC.init("POSITION", 0, .R32G32_FLOAT, 0, 0, .PER_VERTEX_DATA, 0),
        };
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.InputLayout = .{
            .pInputElementDescs = &input_layout_desc,
            .NumElements = input_layout_desc.len,
        };
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .LINE;

        break :blk grfx.createGraphicsShaderPipeline(
            &arena_allocator.allocator,
            &pso_desc,
            "content/shaders/lines.vs.cso",
            "content/shaders/lines.ps.cso",
        );
    };

    const lines_buffer = grfx.createCommittedResource(
        .DEFAULT,
        d3d12.HEAP_FLAG_NONE,
        &d3d12.RESOURCE_DESC.initBuffer(num_vis_samples * @sizeOf(Vec2)),
        d3d12.RESOURCE_STATE_COPY_DEST,
        null,
    ) catch |err| hrPanic(err);

    grfx.beginFrame();

    var gui = gr.GuiContext.init(&arena_allocator.allocator, &grfx);

    grfx.finishGpuCommands();

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = lib.FrameStats.init(),
        .brush = brush,
        .textformat = textformat,
        .audio = .{
            .client = audio_client,
            .render_client = audio_render_client,
            .buffer_ready_event = audio_buffer_ready_event,
            .buffer_size_in_frames = audio_buffer_size_in_frames,
            .thread_handle = null,
            .samples = audio_samples,
            .current_frame_index = 0,
            .is_locked = true,
        },
        .lines_pso = lines_pso,
        .lines_buffer = lines_buffer,
    };
}

fn deinit(demo: *DemoState, gpa: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    _ = demo.grfx.releasePipeline(demo.lines_pso);
    _ = demo.grfx.releaseResource(demo.lines_buffer);

    while (@cmpxchgWeak(bool, &demo.audio.is_locked, false, true, .Acquire, .Monotonic) != null) {}
    _ = w.TerminateThread(demo.audio.thread_handle.?, 0);
    w.CloseHandle(demo.audio.buffer_ready_event);
    w.CloseHandle(demo.audio.thread_handle.?);
    hrPanicOnFail(demo.audio.client.Stop());
    _ = demo.audio.render_client.Release();
    _ = demo.audio.client.Release();
    demo.audio.samples.deinit();

    _ = demo.brush.Release();
    _ = demo.textformat.Release();
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit();
    lib.deinitWindow(gpa);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();

    lib.newImGuiFrame(demo.frame_stats.delta_time);

    c.igShowDemoWindow(null);
}

fn draw(demo: *DemoState) void {
    var grfx = &demo.grfx;
    grfx.beginFrame();

    const back_buffer = grfx.getBackBuffer();

    grfx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
    grfx.addTransitionBarrier(demo.lines_buffer, d3d12.RESOURCE_STATE_COPY_DEST);
    grfx.flushResourceBarriers();

    {
        while (@cmpxchgWeak(bool, &demo.audio.is_locked, false, true, .Acquire, .Monotonic) != null) {}
        defer @atomicStore(bool, &demo.audio.is_locked, false, .Release);

        const frame = demo.audio.current_frame_index;

        const upload = grfx.allocateUploadBufferRegion(Vec2, num_vis_samples);
        for (upload.cpu_slice) |_, i| {
            const y = if ((frame + i) * 2 >= demo.audio.samples.items.len)
                0.0
            else
                @intToFloat(f32, demo.audio.samples.items[(frame + i) * 2]) / @intToFloat(f32, 0x7fff);

            const x = -1.0 + 2.0 * @intToFloat(f32, i) / @intToFloat(f32, num_vis_samples - 1);
            upload.cpu_slice[i] = Vec2.init(x, y);
        }
        grfx.cmdlist.CopyBufferRegion(
            grfx.getResource(demo.lines_buffer),
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
    }

    grfx.addTransitionBarrier(demo.lines_buffer, d3d12.RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
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

    grfx.setCurrentPipeline(demo.lines_pso);
    grfx.cmdlist.IASetPrimitiveTopology(.LINESTRIP);
    grfx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
        .BufferLocation = grfx.getResource(demo.lines_buffer).GetGPUVirtualAddress(),
        .SizeInBytes = num_vis_samples * @sizeOf(Vec2),
        .StrideInBytes = @sizeOf(Vec2),
    }});
    grfx.cmdlist.DrawInstanced(num_vis_samples, 1, 0, 0);

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

        demo.brush.SetColor(&d2d1.COLOR_F{ .r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0 });
        lib.drawText(
            grfx.d2d.context,
            text,
            demo.textformat,
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

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa.deinit();
        assert(leaked == false);
    }
    const allocator = &gpa.allocator;

    var demo = init(allocator);
    defer deinit(&demo, allocator);

    demo.audio.thread_handle = w.kernel32.CreateThread(
        null,
        0,
        audioThread,
        @ptrCast(*c_void, &demo.audio),
        0,
        null,
    ).?;
    hrPanicOnFail(demo.audio.client.Start());
    @atomicStore(bool, &demo.audio.is_locked, false, .Release);

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
