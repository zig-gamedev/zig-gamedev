const std = @import("std");
const win32 = @import("win32");
const w = win32.base;
const d2d1 = win32.d2d1;
const d3d12 = win32.d3d12;
const dwrite = win32.dwrite;
const wasapi = win32.wasapi;
const common = @import("common");
const gr = common.graphics;
const lib = common.library;
const c = common.c;
const math = std.math;
const assert = std.debug.assert;
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: wasapi test";
const window_width = 1920;
const window_height = 1080;

const AudioContex = struct {
    client: *wasapi.IAudioClient3,
    render_client: *wasapi.IAudioRenderClient,
    buffer_ready_event: w.HANDLE,
    buffer_size_in_frames: u32,
    thread_handle: ?w.HANDLE,
};

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    brush: *d2d1.ISolidColorBrush,
    textformat: *dwrite.ITextFormat,

    audio: AudioContex,
};

fn fillAudioBuffer(audio: AudioContex) void {
    var buffer_padding_in_frames: w.UINT = 0;
    hrPanicOnFail(audio.client.GetCurrentPadding(&buffer_padding_in_frames));

    const num_frames = audio.buffer_size_in_frames - buffer_padding_in_frames;
    var ptr: [*]f32 = undefined;

    hrPanicOnFail(audio.render_client.GetBuffer(num_frames, @ptrCast(*?*w.BYTE, &ptr)));
    var i: u32 = 0;
    while (i < num_frames) : (i += 1) {
        const frac = @intToFloat(f32, i) / @intToFloat(f32, num_frames);
        ptr[i * 2 + 0] = 0.25 * math.sin(2.0 * math.pi * 440.0 * frac);
        ptr[i * 2 + 1] = 0.25 * math.sin(2.0 * math.pi * 440.0 * frac);
    }
    hrPanicOnFail(audio.render_client.ReleaseBuffer(num_frames, 0));
}

fn audioThread(ctx: ?*c_void) callconv(.C) w.DWORD {
    const audio = @ptrCast(*AudioContex, @alignCast(8, ctx));

    fillAudioBuffer(audio.*);
    while (true) {
        w.WaitForSingleObject(audio.buffer_ready_event, w.INFINITE) catch unreachable;
        fillAudioBuffer(audio.*);
    }
    return 0;
}

fn init(allocator: *std.mem.Allocator) DemoState {
    const window = lib.initWindow(allocator, window_name, window_width, window_height) catch unreachable;
    var grfx = gr.GraphicsContext.init(window);

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

    grfx.beginFrame();

    var gui = gr.GuiContext.init(allocator, &grfx);

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
        },
    };
}

fn deinit(demo: *DemoState, allocator: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    hrPanicOnFail(demo.audio.client.Stop());
    w.CloseHandle(demo.audio.buffer_ready_event);
    _ = demo.audio.render_client.Release();
    _ = demo.audio.client.Release();
    _ = demo.brush.Release();
    _ = demo.textformat.Release();
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit(allocator);
    lib.deinitWindow(allocator);
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

        demo.brush.SetColor(&d2d1.COLOR_F{ .r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0 });
        lib.DrawText(
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
    // WIC requires below call (when we pass COINIT_MULTITHREADED '_ = wic_factory.Release()' crashes on exit).
    _ = w.ole32.CoInitializeEx(null, @enumToInt(w.COINIT_APARTMENTTHREADED));
    defer w.ole32.CoUninitialize();

    _ = w.SetProcessDPIAware();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa.deinit();
        std.debug.assert(leaked == false);
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
    w.kernel32.Sleep(1);
    hrPanicOnFail(demo.audio.client.Start());

    while (true) {
        var message = std.mem.zeroes(w.user32.MSG);
        const has_message = w.user32.peekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) catch unreachable;
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
