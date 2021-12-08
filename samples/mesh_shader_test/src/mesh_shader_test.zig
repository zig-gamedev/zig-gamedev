const std = @import("std");
const win32 = @import("win32");
const w = win32.base;
const d2d1 = win32.d2d1;
const d3d12 = win32.d3d12;
const dwrite = win32.dwrite;
const dml = win32.directml;
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

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: mesh shader test";
const window_width = 1920;
const window_height = 1080;

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    mesh_shader_pso: gr.PipelineHandle,

    brush: *d2d1.ISolidColorBrush,
    normal_tfmt: *dwrite.ITextFormat,
};

fn init(gpa_allocator: std.mem.Allocator) DemoState {
    const tracy_zone = tracy.zone(@src(), 1);
    defer tracy_zone.end();

    const window = lib.initWindow(gpa_allocator, window_name, window_width, window_height) catch unreachable;

    var arena_allocator_state = std.heap.ArenaAllocator.init(gpa_allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    _ = pix.loadGpuCapturerLibrary();
    _ = pix.setTargetWindow(window);
    _ = pix.beginCapture(
        pix.CAPTURE_GPU,
        &pix.CaptureParameters{ .gpu_capture_params = .{ .FileName = L("capture.wpix") } },
    );

    var grfx = gr.GraphicsContext.init(window);

    const brush = blk: {
        var brush: ?*d2d1.ISolidColorBrush = null;
        hrPanicOnFail(grfx.d2d.context.CreateSolidColorBrush(
            &.{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
            null,
            &brush,
        ));
        break :blk brush.?;
    };

    const normal_tfmt = blk: {
        var txtfmt: ?*dwrite.ITextFormat = null;
        hrPanicOnFail(grfx.dwrite_factory.CreateTextFormat(
            L("Verdana"),
            null,
            dwrite.FONT_WEIGHT.NORMAL,
            dwrite.FONT_STYLE.NORMAL,
            dwrite.FONT_STRETCH.NORMAL,
            32.0,
            L("en-us"),
            &txtfmt,
        ));
        break :blk txtfmt.?;
    };
    hrPanicOnFail(normal_tfmt.SetTextAlignment(.LEADING));
    hrPanicOnFail(normal_tfmt.SetParagraphAlignment(.NEAR));

    const mesh_shader_pso = blk: {
        var pso_desc = d3d12.MESH_SHADER_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.DSVFormat = .D32_FLOAT;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.SampleDesc = .{ .Count = 1, .Quality = 0 };

        break :blk grfx.createMeshShaderPipeline(
            arena_allocator,
            &pso_desc,
            null,
            "content/shaders/mesh_shader.ms.cso",
            "content/shaders/mesh_shader.ps.cso",
        );
    };

    grfx.beginFrame();

    pix.beginEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist), "GPU init");

    var gui = gr.GuiContext.init(arena_allocator, &grfx, 1);

    _ = pix.endEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, grfx.cmdlist));

    grfx.endFrame();
    grfx.finishGpuCommands();

    _ = pix.endCapture();

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = lib.FrameStats.init(),
        .mesh_shader_pso = mesh_shader_pso,
        .brush = brush,
        .normal_tfmt = normal_tfmt,
    };
}

fn deinit(demo: *DemoState, gpa_allocator: std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    _ = demo.brush.Release();
    _ = demo.normal_tfmt.Release();
    _ = demo.grfx.releasePipeline(demo.mesh_shader_pso);
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit();
    lib.deinitWindow(gpa_allocator);
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
            demo.normal_tfmt,
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

    var gpa_allocator_state = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa_allocator_state.deinit();
        std.debug.assert(leaked == false);
    }
    const gpa_allocator = gpa_allocator_state.allocator();

    var demo = init(gpa_allocator);
    defer deinit(&demo, gpa_allocator);

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
