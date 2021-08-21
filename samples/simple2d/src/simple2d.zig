const builtin = @import("builtin");
const std = @import("std");
const w = @import("win32");
const gr = @import("graphics");
const lib = @import("library");
const c = @import("c");
usingnamespace @import("vectormath");
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const utf8ToUtf16LeStringLiteral = std.unicode.utf8ToUtf16LeStringLiteral;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: simple2d";
const window_width = 1920;
const window_height = 1080;

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    brush: *w.ID2D1SolidColorBrush,
    textformat: *w.IDWriteTextFormat,
    ellipse: *w.ID2D1EllipseGeometry,
    stroke_style: *w.ID2D1StrokeStyle,
    path: *w.ID2D1PathGeometry,
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

    const ellipse = blk: {
        var maybe_ellipse: ?*w.ID2D1EllipseGeometry = null;
        hrPanicOnFail(grfx.d2d.factory.CreateEllipseGeometry(
            &.{ .point = .{ .x = 1210.0, .y = 150.0 }, .radiusX = 50.0, .radiusY = 70.0 },
            &maybe_ellipse,
        ));
        break :blk maybe_ellipse.?;
    };
    const stroke_style = blk: {
        var maybe_stroke_style: ?*w.ID2D1StrokeStyle = null;
        hrPanicOnFail(grfx.d2d.factory.CreateStrokeStyle(
            &.{
                .startCap = .ROUND,
                .endCap = .ROUND,
                .dashCap = .FLAT,
                .lineJoin = .MITER,
                .miterLimit = 0.0,
                .dashStyle = .SOLID,
                .dashOffset = 0.0,
            },
            null,
            0,
            &maybe_stroke_style,
        ));
        break :blk maybe_stroke_style.?;
    };
    const path = blk: {
        var path: *w.ID2D1PathGeometry = undefined;
        hrPanicOnFail(grfx.d2d.factory.CreatePathGeometry(@ptrCast(*?*w.ID2D1PathGeometry, &path)));

        var sink: *w.ID2D1GeometrySink = undefined;
        hrPanicOnFail(path.Open(@ptrCast(*?*w.ID2D1GeometrySink, &sink)));
        defer {
            hrPanicOnFail(sink.Close());
            _ = sink.Release();
        }
        sink.BeginFigure(.{ .x = 500.0, .y = 400.0 }, .FILLED);
        sink.AddLine(.{ .x = 600.0, .y = 300.0 });
        sink.AddLine(.{ .x = 700.0, .y = 400.0 });
        sink.AddLine(.{ .x = 800.0, .y = 300.0 });
        sink.AddBezier(&.{
            .point1 = .{ .x = 850.0, .y = 350.0 },
            .point2 = .{ .x = 850.0, .y = 350.0 },
            .point3 = .{ .x = 900.0, .y = 300.0 },
        });
        sink.AddBezier(&.{
            .point1 = .{ .x = 920.0, .y = 280.0 },
            .point2 = .{ .x = 950.0, .y = 320.0 },
            .point3 = .{ .x = 1000.0, .y = 300.0 },
        });
        sink.EndFigure(.OPEN);
        break :blk path;
    };

    grfx.beginFrame();

    var gui = gr.GuiContext.init(allocator, &grfx);

    grfx.finishGpuCommands();

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = lib.FrameStats.init(),
        .brush = brush,
        .textformat = textformat,
        .ellipse = ellipse,
        .stroke_style = stroke_style,
        .path = path,
    };
}

fn drawShapes(demo: DemoState) void {
    var grfx = &demo.grfx;

    grfx.d2d.context.DrawLine(
        .{ .x = 20.0, .y = 200.0 },
        .{ .x = 120.0, .y = 300.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
        33.0,
        demo.stroke_style,
    );
    grfx.d2d.context.DrawLine(
        .{ .x = 160.0, .y = 300.0 },
        .{ .x = 260.0, .y = 200.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
        33.0,
        demo.stroke_style,
    );
    grfx.d2d.context.DrawLine(
        .{ .x = 300.0, .y = 200.0 },
        .{ .x = 400.0, .y = 300.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
        33.0,
        demo.stroke_style,
    );

    grfx.d2d.context.DrawRectangle(
        &.{ .left = 500.0, .top = 100.0, .right = 600.0, .bottom = 200.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
        5.0,
        null,
    );
    grfx.d2d.context.FillRectangle(
        &.{ .left = 610.0, .top = 100.0, .right = 710.0, .bottom = 200.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
    );
    grfx.d2d.context.FillRoundedRectangle(
        &.{
            .rect = .{ .left = 830.0, .top = 100.0, .right = 930.0, .bottom = 200.0 },
            .radiusX = 20.0,
            .radiusY = 20.0,
        },
        @ptrCast(*w.ID2D1Brush, demo.brush),
    );

    grfx.d2d.context.DrawEllipse(
        &.{ .point = .{ .x = 990.0, .y = 150.0 }, .radiusX = 50.0, .radiusY = 70.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
        5.0,
        null,
    );
    grfx.d2d.context.FillEllipse(
        &.{ .point = .{ .x = 1100.0, .y = 150.0 }, .radiusX = 50.0, .radiusY = 70.0 },
        @ptrCast(*w.ID2D1Brush, demo.brush),
    );
    grfx.d2d.context.DrawGeometry(
        @ptrCast(*w.ID2D1Geometry, demo.ellipse),
        @ptrCast(*w.ID2D1Brush, demo.brush),
        7.0,
        null,
    );
    grfx.d2d.context.SetTransform(&w.D2D1_MATRIX_3X2_F.initTranslation(110.0, 0.0));
    grfx.d2d.context.FillGeometry(@ptrCast(*w.ID2D1Geometry, demo.ellipse), @ptrCast(*w.ID2D1Brush, demo.brush), null);
    grfx.d2d.context.SetTransform(&w.D2D1_MATRIX_3X2_F.initIdentity());

    var i: u32 = 0;
    while (i < 5) : (i += 1) {
        grfx.d2d.context.SetTransform(&w.D2D1_MATRIX_3X2_F.initTranslation(0.0, @intToFloat(f32, i) * 50.0));
        grfx.d2d.context.DrawGeometry(
            @ptrCast(*w.ID2D1Geometry, demo.path),
            @ptrCast(*w.ID2D1Brush, demo.brush),
            15.0,
            null,
        );
        grfx.d2d.context.SetTransform(&w.D2D1_MATRIX_3X2_F.initIdentity());
    }
}

fn deinit(demo: *DemoState, allocator: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    _ = demo.brush.Release();
    _ = demo.textformat.Release();
    _ = demo.ellipse.Release();
    _ = demo.stroke_style.Release();
    _ = demo.path.Release();
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
        &[4]f32{ 0.0, 0.0, 0.0, 0.0 },
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

        drawShapes(demo.*);
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
