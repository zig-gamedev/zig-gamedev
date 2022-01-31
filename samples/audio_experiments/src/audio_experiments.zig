const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const win32 = @import("win32");
const w = win32.base;
const d2d1 = win32.d2d1;
const d3d12 = win32.d3d12;
const dwrite = win32.dwrite;
const dml = win32.directml;
const xaudio2 = win32.xaudio2;
const xaudio2fx = win32.xaudio2fx;
const xapo = win32.xapo;
const wasapi = win32.wasapi;
const mf = win32.mf;
const common = @import("common");
const gfx = common.graphics;
const sfx = common.audio;
const lib = common.library;
const zm = common.zmath;
const c = common.c;
const pix = common.pix;
const tracy = common.tracy;

const WAVEFORMATEX = wasapi.WAVEFORMATEX;
const hrPanic = lib.hrPanic;
const hrPanicOnFail = lib.hrPanicOnFail;
const L = std.unicode.utf8ToUtf16LeStringLiteral;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: audio experiments";
const window_width = 1920;
const window_height = 1080;

const AudioProcessor = extern struct {
    v: *const xapo.IXAPOVTable(AudioProcessor) = &vtable,
    refcount: u32 = 1,

    const Self = @This();

    const vtable = xapo.IXAPOVTable(AudioProcessor){
        .unknown = .{
            .QueryInterface = QueryInterface,
            .AddRef = AddRef,
            .Release = Release,
        },
        .xapo = .{
            .GetRegistrationProperties = GetRegistrationProperties,
            .IsInputFormatSupported = IsInputFormatSupported,
            .IsOutputFormatSupported = IsOutputFormatSupported,
            .Initialize = Initialize,
            .Reset = Reset,
            .LockForProcess = LockForProcess,
            .UnlockForProcess = UnlockForProcess,
            .Process = Process,
            .CalcInputFrames = CalcInputFrames,
            .CalcOutputFrames = CalcOutputFrames,
        },
    };

    fn QueryInterface(
        self: *Self,
        guid: *const w.GUID,
        outobj: ?*?*anyopaque,
    ) callconv(w.WINAPI) w.HRESULT {
        assert(outobj != null);

        if (std.mem.eql(u8, std.mem.asBytes(guid), std.mem.asBytes(&w.IID_IUnknown))) {
            outobj.?.* = self;
            _ = self.AddRef();
            return w.S_OK;
        } else if (std.mem.eql(u8, std.mem.asBytes(guid), std.mem.asBytes(&xapo.IID_IXAPO))) {
            outobj.?.* = self;
            _ = self.AddRef();
            return w.S_OK;
        }

        outobj.?.* = null;
        return w.E_NOINTERFACE;
    }

    fn AddRef(self: *Self) callconv(w.WINAPI) w.ULONG {
        return @atomicRmw(u32, &self.refcount, .Add, 1, .Monotonic) + 1;
    }

    fn Release(self: *Self) callconv(w.WINAPI) w.ULONG {
        const prev_refcount = @atomicRmw(u32, &self.refcount, .Sub, 1, .Monotonic);
        if (prev_refcount == 1) {
            w.ole32.CoTaskMemFree(self);
        }
        return prev_refcount - 1;
    }

    fn GetRegistrationProperties(
        self: *Self,
        props: *?*xapo.REGISTRATION_PROPERTIES,
    ) callconv(w.WINAPI) w.HRESULT {
        _ = self;
        _ = props;
        return w.S_OK;
    }

    fn IsInputFormatSupported(
        self: *Self,
        output_format: *const WAVEFORMATEX,
        requested_input_format: *const WAVEFORMATEX,
        supported_input_format: ?*?*WAVEFORMATEX,
    ) callconv(w.WINAPI) w.HRESULT {
        _ = self;
        _ = output_format;
        _ = requested_input_format;
        _ = supported_input_format;
        return w.S_OK;
    }

    fn IsOutputFormatSupported(
        self: *Self,
        input_format: *const WAVEFORMATEX,
        requested_output_format: *const WAVEFORMATEX,
        supported_output_format: ?*?*WAVEFORMATEX,
    ) callconv(w.WINAPI) w.HRESULT {
        _ = self;
        _ = input_format;
        _ = requested_output_format;
        _ = supported_output_format;
        return w.S_OK;
    }

    fn Initialize(self: *Self, data: ?*const anyopaque, data_size: w.UINT32) callconv(w.WINAPI) w.HRESULT {
        _ = self;
        _ = data;
        _ = data_size;
        return w.S_OK;
    }

    fn Reset(self: *Self) callconv(w.WINAPI) void {
        _ = self;
    }

    fn LockForProcess(
        self: *Self,
        num_input_params: w.UINT32,
        input_params: ?[*]const xapo.LOCKFORPROCESS_BUFFER_PARAMETERS,
        num_output_params: w.UINT32,
        output_params: ?[*]const xapo.LOCKFORPROCESS_BUFFER_PARAMETERS,
    ) callconv(w.WINAPI) w.HRESULT {
        _ = self;
        _ = num_input_params;
        _ = input_params;
        _ = num_output_params;
        _ = output_params;
        return w.S_OK;
    }

    fn UnlockForProcess(self: *Self) callconv(w.WINAPI) void {
        _ = self;
    }

    fn Process(
        self: *Self,
        num_input_params: w.UINT32,
        input_params: ?[*]const xapo.PROCESS_BUFFER_PARAMETERS,
        num_output_params: w.UINT32,
        output_params: ?[*]const xapo.PROCESS_BUFFER_PARAMETERS,
        is_enabled: w.BOOL,
    ) callconv(w.WINAPI) void {
        _ = self;
        _ = num_input_params;
        _ = input_params;
        _ = num_output_params;
        _ = output_params;
        _ = is_enabled;
    }

    fn CalcInputFrames(_: *Self, num_output_frames: w.UINT32) callconv(w.WINAPI) w.UINT32 {
        return num_output_frames;
    }

    fn CalcOutputFrames(_: *Self, num_input_frames: w.UINT32) callconv(w.WINAPI) w.UINT32 {
        return num_input_frames;
    }
};

const DemoState = struct {
    gctx: gfx.GraphicsContext,
    actx: sfx.AudioContext,
    guictx: gfx.GuiContext,
    frame_stats: lib.FrameStats,

    music: *sfx.Stream,
    music_is_playing: bool = true,
    music_has_reverb: bool = false,
    sound1_data: []const u8,
    sound2_data: []const u8,
    sound3_data: []const u8,

    brush: *d2d1.ISolidColorBrush,
    normal_tfmt: *dwrite.ITextFormat,
};

fn init(gpa_allocator: std.mem.Allocator) DemoState {
    const tracy_zone = tracy.zone(@src(), 1);
    defer tracy_zone.end();

    const comptr = @ptrCast(*AudioProcessor, @alignCast(8, w.CoTaskMemAlloc(@sizeOf(AudioProcessor)).?));
    comptr.* = .{};
    defer _ = comptr.Release();

    var actx = sfx.AudioContext.init(gpa_allocator);

    hrPanicOnFail(mf.MFStartup(mf.VERSION, 0));

    const sound1_data = sfx.loadBufferData(gpa_allocator, L("content/drum_bass_hard.flac"));
    const sound2_data = sfx.loadBufferData(gpa_allocator, L("content/tabla_tas1.flac"));
    const sound3_data = sfx.loadBufferData(gpa_allocator, L("content/loop_mika.flac"));

    var music = sfx.Stream.create(gpa_allocator, actx.device, L("content/Broke For Free - Night Owl.mp3"));
    hrPanicOnFail(music.voice.Start(0, xaudio2.COMMIT_NOW));

    {
        var reverb_apo: ?*w.IUnknown = null;
        hrPanicOnFail(xaudio2fx.createReverb(&reverb_apo, 0));
        defer _ = reverb_apo.?.Release();

        var effect_descriptor = [_]xaudio2.EFFECT_DESCRIPTOR{.{
            .pEffect = reverb_apo.?,
            .InitialState = w.FALSE,
            .OutputChannels = 2,
        }};
        const effect_chain = xaudio2.EFFECT_CHAIN{ .EffectCount = 1, .pEffectDescriptors = &effect_descriptor };
        hrPanicOnFail(music.voice.SetEffectChain(&effect_chain));

        hrPanicOnFail(music.voice.SetEffectParameters(
            0,
            &xaudio2fx.REVERB_PARAMETERS.initDefault(),
            @sizeOf(xaudio2fx.REVERB_PARAMETERS),
            xaudio2.COMMIT_NOW,
        ));
    }

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

    var gctx = gfx.GraphicsContext.init(window);
    gctx.present_flags = 0;
    gctx.present_interval = 1;

    const brush = blk: {
        var brush: *d2d1.ISolidColorBrush = undefined;
        hrPanicOnFail(gctx.d2d.context.CreateSolidColorBrush(
            &.{ .r = 1.0, .g = 0.0, .b = 0.0, .a = 0.5 },
            null,
            @ptrCast(*?*d2d1.ISolidColorBrush, &brush),
        ));
        break :blk brush;
    };

    const normal_tfmt = blk: {
        var info_txtfmt: *dwrite.ITextFormat = undefined;
        hrPanicOnFail(gctx.dwrite_factory.CreateTextFormat(
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
    hrPanicOnFail(normal_tfmt.SetTextAlignment(.LEADING));
    hrPanicOnFail(normal_tfmt.SetParagraphAlignment(.NEAR));

    //
    // Begin frame to init/upload resources to the GPU.
    //
    gctx.beginFrame();
    gctx.endFrame();
    gctx.beginFrame();

    pix.beginEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, gctx.cmdlist), "GPU init");

    var guictx = gfx.GuiContext.init(arena_allocator, &gctx, 1);

    _ = pix.endEventOnCommandList(@ptrCast(*d3d12.IGraphicsCommandList, gctx.cmdlist));

    gctx.endFrame();
    gctx.finishGpuCommands();

    _ = pix.endCapture();

    return .{
        .gctx = gctx,
        .actx = actx,
        .guictx = guictx,
        .music = music,
        .sound1_data = sound1_data,
        .sound2_data = sound2_data,
        .sound3_data = sound3_data,
        .frame_stats = lib.FrameStats.init(),
        .brush = brush,
        .normal_tfmt = normal_tfmt,
    };
}

fn deinit(demo: *DemoState, gpa_allocator: std.mem.Allocator) void {
    demo.gctx.finishGpuCommands();
    _ = demo.brush.Release();
    _ = demo.normal_tfmt.Release();
    demo.guictx.deinit(&demo.gctx);
    demo.gctx.deinit();
    demo.music.destroy();
    hrPanicOnFail(mf.MFShutdown());
    gpa_allocator.free(demo.sound1_data);
    gpa_allocator.free(demo.sound2_data);
    gpa_allocator.free(demo.sound3_data);
    demo.actx.deinit();
    lib.deinitWindow(gpa_allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();
    const dt = demo.frame_stats.delta_time;
    lib.newImGuiFrame(dt);

    c.igSetNextWindowPos(
        c.ImVec2{ .x = @intToFloat(f32, demo.gctx.viewport_width) - 600.0 - 20, .y = 20.0 },
        c.ImGuiCond_FirstUseEver,
        c.ImVec2{ .x = 0.0, .y = 0.0 },
    );
    c.igSetNextWindowSize(.{ .x = 600.0, .y = -1 }, c.ImGuiCond_Always);

    _ = c.igBegin(
        "Demo Settings",
        null,
        c.ImGuiWindowFlags_NoMove | c.ImGuiWindowFlags_NoResize | c.ImGuiWindowFlags_NoSavedSettings,
    );

    c.igText("Music:", "");
    if (c.igButton(
        if (demo.music_is_playing) "  Pause  " else "  Play  ",
        .{ .x = 0, .y = 0 },
    )) {
        demo.music_is_playing = !demo.music_is_playing;
        if (demo.music_is_playing) {
            hrPanicOnFail(demo.music.voice.Start(0, xaudio2.COMMIT_NOW));
        } else {
            hrPanicOnFail(demo.music.voice.Stop(0, xaudio2.COMMIT_NOW));
        }
    }
    c.igSameLine(0.0, -1.0);
    if (c.igButton("  Rewind  ", .{ .x = 0, .y = 0 })) {
        demo.music.setCurrentPosition(0);
    }
    c.igSameLine(0.0, -1.0);
    if (c.igButton(
        if (demo.music_has_reverb) "  No Reverb  " else "  Reverb  ",
        .{ .x = 0, .y = 0 },
    )) {
        demo.music_has_reverb = !demo.music_has_reverb;
        if (demo.music_has_reverb) {
            hrPanicOnFail(demo.music.voice.EnableEffect(0, xaudio2.COMMIT_NOW));
        } else {
            hrPanicOnFail(demo.music.voice.DisableEffect(0, xaudio2.COMMIT_NOW));
        }
    }
    c.igSpacing();

    c.igText("Sounds:", "");
    if (c.igButton("  Play Sound 1  ", .{ .x = 0, .y = 0 })) {
        demo.actx.playBuffer(.{ .data = demo.sound1_data });
    }
    c.igSameLine(0.0, -1.0);
    if (c.igButton("  Play Sound 2  ", .{ .x = 0, .y = 0 })) {
        demo.actx.playBuffer(.{ .data = demo.sound2_data });
    }
    c.igSameLine(0.0, -1.0);
    if (c.igButton("  Play Sound 3  ", .{ .x = 0, .y = 0 })) {
        demo.actx.playBuffer(.{ .data = demo.sound3_data });
    }

    c.igEnd();

    //var state: xaudio2.VOICE_STATE = std.mem.zeroes(xaudio2.VOICE_STATE);
    //demo.music.voice.GetState(&state, 0);
    //std.log.info("{}", .{state});
}

fn draw(demo: *DemoState) void {
    var gctx = &demo.gctx;
    gctx.beginFrame();

    const back_buffer = gctx.getBackBuffer();
    gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATE_RENDER_TARGET);
    gctx.flushResourceBarriers();

    gctx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w.TRUE,
        null,
    );
    gctx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );

    demo.guictx.draw(gctx);

    gctx.beginDraw2d();
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
            gctx.d2d.context,
            text,
            demo.normal_tfmt,
            &d2d1.RECT_F{
                .left = 10.0,
                .top = 10.0,
                .right = @intToFloat(f32, gctx.viewport_width),
                .bottom = @intToFloat(f32, gctx.viewport_height),
            },
            @ptrCast(*d2d1.IBrush, demo.brush),
        );
    }
    gctx.endDraw2d();

    gctx.endFrame();
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
