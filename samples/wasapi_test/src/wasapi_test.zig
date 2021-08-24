const builtin = @import("builtin");
const std = @import("std");
const w = @import("win32");
const gr = @import("graphics");
const lib = @import("library");
const c = @import("c");
usingnamespace @import("vectormath");
const assert = std.debug.assert;
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const utf8ToUtf16LeStringLiteral = std.unicode.utf8ToUtf16LeStringLiteral;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: wasapi test";
const window_width = 1920;
const window_height = 1080;

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    brush: *w.ID2D1SolidColorBrush,
    textformat: *w.IDWriteTextFormat,
};

fn init(allocator: *std.mem.Allocator) DemoState {
    const window = lib.initWindow(allocator, window_name, window_width, window_height) catch unreachable;
    var grfx = gr.GraphicsContext.init(window);

    const brush = blk: {
        var maybe_brush: ?*w.ID2D1SolidColorBrush = null;
        hrPanicOnFail(grfx.d2d.context.CreateSolidColorBrush(
            &w.D2D1_COLOR_F{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
            null,
            &maybe_brush,
        ));
        break :blk maybe_brush.?;
    };
    const textformat = blk: {
        var maybe_textformat: ?*w.IDWriteTextFormat = null;
        hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
            utf8ToUtf16LeStringLiteral("Verdana"),
            null,
            w.DWRITE_FONT_WEIGHT.NORMAL,
            w.DWRITE_FONT_STYLE.NORMAL,
            w.DWRITE_FONT_STRETCH.NORMAL,
            32.0,
            utf8ToUtf16LeStringLiteral("en-us"),
            &maybe_textformat,
        ));
        break :blk maybe_textformat.?;
    };
    hrPanicOnFail(textformat.SetTextAlignment(.LEADING));
    hrPanicOnFail(textformat.SetParagraphAlignment(.NEAR));

    const audio_device_enumerator = blk: {
        var audio_device_enumerator: *w.IMMDeviceEnumerator = undefined;
        hrPanicOnFail(w.CoCreateInstance(
            &w.CLSID_MMDeviceEnumerator,
            null,
            w.CLSCTX_INPROC_SERVER,
            &w.IID_IMMDeviceEnumerator,
            @ptrCast(*?*c_void, &audio_device_enumerator),
        ));
        break :blk audio_device_enumerator;
    };
    defer _ = audio_device_enumerator.Release();

    const audio_device = blk: {
        var audio_device: *w.IMMDevice = undefined;
        hrPanicOnFail(audio_device_enumerator.GetDefaultAudioEndpoint(
            .eRender,
            .eConsole,
            @ptrCast(*?*w.IMMDevice, &audio_device),
        ));
        break :blk audio_device;
    };
    defer _ = audio_device.Release();

    const audio_client = blk: {
        var audio_client: *w.IAudioClient3 = undefined;
        hrPanicOnFail(audio_device.Activate(
            &w.IID_IAudioClient3,
            w.CLSCTX_INPROC_SERVER,
            null,
            @ptrCast(*?*c_void, &audio_client),
        ));
        break :blk audio_client;
    };
    defer _ = audio_client.Release();

    // Initialize audio client interafce.
    {
        var closest_format: ?*w.WAVEFORMATEX = null;
        const wanted_format = w.WAVEFORMATEX{
            .wFormatTag = w.WAVE_FORMAT_IEEE_FLOAT,
            .nChannels = 2,
            .nSamplesPerSec = 48_000,
            .nAvgBytesPerSec = 48_000 * 8,
            .nBlockAlign = 8,
            .wBitsPerSample = 32,
            .cbSize = 0,
        };
        hrPanicOnFail(audio_client.IsFormatSupported(.SHARED, &wanted_format, &closest_format));
        assert(closest_format == null);

        var default_period: w.REFERENCE_TIME = 0;
        hrPanicOnFail(audio_client.GetDevicePeriod(&default_period, null));

        hrPanicOnFail(audio_client.Initialize(
            .SHARED,
            w.AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
            default_period,
            0,
            &wanted_format,
            null,
        ));
    }

    grfx.beginFrame();

    var gui = gr.GuiContext.init(allocator, &grfx);

    grfx.finishGpuCommands();

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = lib.FrameStats.init(),
        .brush = brush,
        .textformat = textformat,
    };
}

fn deinit(demo: *DemoState, allocator: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    _ = demo.brush.Release();
    _ = demo.textformat.Release();
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit(allocator);
    lib.deinitWindow(allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();

    lib.updateWindow(demo.frame_stats.delta_time);

    c.igShowDemoWindow(null);
}

fn draw(demo: *DemoState) void {
    var grfx = &demo.grfx;
    grfx.beginFrame();

    const back_buffer = grfx.getBackBuffer();

    grfx.addTransitionBarrier(back_buffer.resource_handle, w.D3D12_RESOURCE_STATE_RENDER_TARGET);
    grfx.flushResourceBarriers();

    grfx.cmdlist.OMSetRenderTargets(
        1,
        &[_]w.D3D12_CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
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

        demo.brush.SetColor(&w.D2D1_COLOR_F{ .r = 1.0, .g = 1.0, .b = 1.0, .a = 1.0 });
        grfx.d2d.context.DrawTextSimple(
            text,
            demo.textformat,
            &w.D2D1_RECT_F{
                .left = 10.0,
                .top = 10.0,
                .right = @intToFloat(f32, grfx.viewport_width),
                .bottom = @intToFloat(f32, grfx.viewport_height),
            },
            @ptrCast(*w.ID2D1Brush, demo.brush),
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
