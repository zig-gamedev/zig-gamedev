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
    _ = c.igCreateContext(null);

    const window = lib.initWindow(window_name, window_width, window_height) catch unreachable;
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
    c.igDestroyContext(null);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();

    gr.GuiContext.update(demo.frame_stats.delta_time);

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
        if (w.user32.PeekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) > 0) {
            _ = w.user32.DispatchMessageA(&message);
            if (message.message == w.user32.WM_QUIT)
                break;
        } else {
            update(&demo);
            draw(&demo);
        }
    }
}
