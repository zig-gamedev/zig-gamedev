const std = @import("std");
const math = std.math;
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const d3d12 = zwin32.d3d12;
const wasapi = zwin32.wasapi;
const mf = zwin32.mf;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;
const zd3d12 = @import("zd3d12");
const common = @import("common");
const c = common.c;
const vm = common.vectormath;
const GuiRenderer = common.GuiRenderer;

const Vec2 = vm.Vec2;

const num_vis_samples = 400;

pub export const D3D12SDKVersion: u32 = 608;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;

const window_name = "zig-gamedev: audio playback test";
const window_width = 1920;
const window_height = 1080;

const AudioContex = struct {
    client: *wasapi.IAudioClient3,
    render_client: *wasapi.IAudioRenderClient,
    buffer_ready_event: w32.HANDLE,
    buffer_size_in_frames: u32,
    thread_handle: ?w32.HANDLE,
    samples: std.ArrayList(i16),
    current_frame_index: u32,
    is_locked: bool,
};

const DemoState = struct {
    gctx: zd3d12.GraphicsContext,
    guir: GuiRenderer,
    frame_stats: common.FrameStats,

    audio: AudioContex,

    lines_pso: zd3d12.PipelineHandle,
    image_pso: zd3d12.PipelineHandle,
    lines_buffer: zd3d12.ResourceHandle,

    image: zd3d12.ResourceHandle,
    image_srv: d3d12.CPU_DESCRIPTOR_HANDLE,
};

fn fillAudioBuffer(audio: *AudioContex) void {
    while (@cmpxchgWeak(bool, &audio.is_locked, false, true, .Acquire, .Monotonic) != null) {}
    defer @atomicStore(bool, &audio.is_locked, false, .Release);

    var buffer_padding_in_frames: w32.UINT = 0;
    hrPanicOnFail(audio.client.GetCurrentPadding(&buffer_padding_in_frames));

    const num_frames = audio.buffer_size_in_frames - buffer_padding_in_frames;

    var ptr: [*]f32 = undefined;
    hrPanicOnFail(audio.render_client.GetBuffer(num_frames, @ptrCast(*?[*]w32.BYTE, &ptr)));

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

fn audioThread(ctx: ?*anyopaque) callconv(.C) w32.DWORD {
    const audio = @ptrCast(*AudioContex, @alignCast(@sizeOf(usize), ctx));

    fillAudioBuffer(audio);
    while (true) {
        _ = w32.WaitForSingleObject(audio.buffer_ready_event, w32.INFINITE);
        fillAudioBuffer(audio);
    }

    return 0;
}

fn init(allocator: std.mem.Allocator) !DemoState {
    const window = try common.initWindow(allocator, window_name, window_width, window_height);

    var gctx = zd3d12.GraphicsContext.init(allocator, window);

    gctx.present_flags = .{};
    gctx.present_interval = 1;

    var arena_allocator_state = std.heap.ArenaAllocator.init(allocator);
    defer arena_allocator_state.deinit();
    const arena_allocator = arena_allocator_state.allocator();

    const audio_device_enumerator = blk: {
        var audio_device_enumerator: *wasapi.IMMDeviceEnumerator = undefined;
        hrPanicOnFail(w32.CoCreateInstance(
            &wasapi.CLSID_MMDeviceEnumerator,
            null,
            w32.CLSCTX_INPROC_SERVER,
            &wasapi.IID_IMMDeviceEnumerator,
            @ptrCast(*?*anyopaque, &audio_device_enumerator),
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
            w32.CLSCTX_INPROC_SERVER,
            null,
            @ptrCast(*?*anyopaque, &audio_client),
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
            @ptrCast(*?*anyopaque, &audio_render_client),
        ));
        break :blk audio_render_client;
    };

    const audio_buffer_ready_event = w32.CreateEventExA(
        null,
        "audio_buffer_ready_event",
        0,
        w32.EVENT_ALL_ACCESS,
    ).?;

    hrPanicOnFail(audio_client.SetEventHandle(audio_buffer_ready_event));

    var audio_buffer_size_in_frames: w32.UINT = 0;
    hrPanicOnFail(audio_client.GetBufferSize(&audio_buffer_size_in_frames));

    const audio_samples = blk: {
        hrPanicOnFail(mf.MFStartup(mf.VERSION, 0));
        defer _ = mf.MFShutdown();

        var config_attribs: *mf.IAttributes = undefined;
        hrPanicOnFail(mf.MFCreateAttributes(&config_attribs, 1));
        defer _ = config_attribs.Release();
        hrPanicOnFail(config_attribs.SetUINT32(&mf.LOW_LATENCY, w32.TRUE));

        var source_reader: *mf.ISourceReader = undefined;
        hrPanicOnFail(mf.MFCreateSourceReaderFromURL(
            L(content_dir ++ "acid_walk.mp3"),
            config_attribs,
            &source_reader,
        ));
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

        var audio_samples = std.ArrayList(i16).init(allocator);
        while (true) {
            var flags: mf.SOURCE_READER_FLAG = .{};
            var sample: ?*mf.ISample = null;
            defer {
                if (sample != null) {
                    _ = sample.?.Release();
                }
            }
            hrPanicOnFail(source_reader.ReadSample(
                mf.SOURCE_READER_FIRST_AUDIO_STREAM,
                .{},
                null,
                &flags,
                null,
                &sample,
            ));
            if (flags.END_OF_STREAM) {
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
        pso_desc.DepthStencilState.DepthEnable = w32.FALSE;
        pso_desc.RasterizerState.AntialiasedLineEnable = w32.TRUE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/lines.vs.cso",
            content_dir ++ "shaders/lines.ps.cso",
        );
    };

    const image_pso = blk: {
        var pso_desc = d3d12.GRAPHICS_PIPELINE_STATE_DESC.initDefault();
        pso_desc.RTVFormats[0] = .R8G8B8A8_UNORM;
        pso_desc.NumRenderTargets = 1;
        pso_desc.BlendState.RenderTarget[0].RenderTargetWriteMask = 0xf;
        pso_desc.PrimitiveTopologyType = .TRIANGLE;
        pso_desc.DepthStencilState.DepthEnable = w32.FALSE;

        break :blk gctx.createGraphicsShaderPipeline(
            arena_allocator,
            &pso_desc,
            content_dir ++ "shaders/image.vs.cso",
            content_dir ++ "shaders/image.ps.cso",
        );
    };

    const lines_buffer = gctx.createCommittedResource(
        .DEFAULT,
        .{},
        &d3d12.RESOURCE_DESC.initBuffer(num_vis_samples * @sizeOf(Vec2)),
        .{ .COPY_DEST = true },
        null,
    ) catch |err| hrPanic(err);

    gctx.beginFrame();

    var guir = GuiRenderer.init(arena_allocator, &gctx, 1, content_dir);

    const image = gctx.createAndUploadTex2dFromFile(
        content_dir ++ "genart_008b.png",
        .{ .num_mip_levels = 1 },
    ) catch |err| hrPanic(err);
    const image_srv = gctx.allocateCpuDescriptors(.CBV_SRV_UAV, 1);
    gctx.device.CreateShaderResourceView(gctx.lookupResource(image).?, null, image_srv);
    gctx.addTransitionBarrier(image, .{ .PIXEL_SHADER_RESOURCE = true });

    gctx.endFrame();
    gctx.finishGpuCommands();

    return DemoState{
        .gctx = gctx,
        .guir = guir,
        .frame_stats = common.FrameStats.init(),
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
        .image_pso = image_pso,
        .lines_buffer = lines_buffer,
        .image = image,
        .image_srv = image_srv,
    };
}

fn deinit(demo: *DemoState, allocator: std.mem.Allocator) void {
    demo.gctx.finishGpuCommands();

    while (@cmpxchgWeak(bool, &demo.audio.is_locked, false, true, .Acquire, .Monotonic) != null) {}
    _ = w32.TerminateThread(demo.audio.thread_handle.?, 0);
    _ = w32.CloseHandle(demo.audio.buffer_ready_event);
    _ = w32.CloseHandle(demo.audio.thread_handle.?);
    hrPanicOnFail(demo.audio.client.Stop());
    _ = demo.audio.render_client.Release();
    _ = demo.audio.client.Release();
    demo.audio.samples.deinit();

    demo.guir.deinit(&demo.gctx);
    demo.gctx.deinit(allocator);
    common.deinitWindow(allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update(demo.gctx.window, window_name);
    common.newImGuiFrame(demo.frame_stats.delta_time);
}

fn draw(demo: *DemoState) void {
    var gctx = &demo.gctx;
    gctx.beginFrame();

    const back_buffer = gctx.getBackBuffer();

    gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
    gctx.addTransitionBarrier(demo.lines_buffer, .{ .COPY_DEST = true });
    gctx.flushResourceBarriers();

    {
        while (@cmpxchgWeak(bool, &demo.audio.is_locked, false, true, .Acquire, .Monotonic) != null) {}
        defer @atomicStore(bool, &demo.audio.is_locked, false, .Release);

        const frame = demo.audio.current_frame_index;

        const upload = gctx.allocateUploadBufferRegion(Vec2, num_vis_samples);
        for (upload.cpu_slice) |_, i| {
            const y = blk: {
                if ((frame + i) * 2 >= demo.audio.samples.items.len) {
                    break :blk 0.0;
                } else {
                    const l = @intToFloat(f32, demo.audio.samples.items[(frame + i) * 2 + 0]) /
                        @intToFloat(f32, 0x7fff);
                    const r = @intToFloat(f32, demo.audio.samples.items[(frame + i) * 2 + 1]) /
                        @intToFloat(f32, 0x7fff);
                    break :blk (l + r) * 0.5;
                }
            };
            const x = -1.0 + 2.0 * @intToFloat(f32, i) / @intToFloat(f32, num_vis_samples - 1);
            upload.cpu_slice[i] = Vec2.init(0.95 * x, y);
        }
        gctx.cmdlist.CopyBufferRegion(
            gctx.lookupResource(demo.lines_buffer).?,
            0,
            upload.buffer,
            upload.buffer_offset,
            upload.cpu_slice.len * @sizeOf(@TypeOf(upload.cpu_slice[0])),
        );
    }

    gctx.addTransitionBarrier(demo.lines_buffer, .{ .VERTEX_AND_CONSTANT_BUFFER = true });
    gctx.flushResourceBarriers();

    gctx.cmdlist.OMSetRenderTargets(
        1,
        &[_]d3d12.CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w32.TRUE,
        null,
    );
    gctx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );

    // Draw background image.
    gctx.setCurrentPipeline(demo.image_pso);
    gctx.cmdlist.IASetPrimitiveTopology(.TRIANGLELIST);
    gctx.cmdlist.SetGraphicsRootDescriptorTable(0, gctx.copyDescriptorsToGpuHeap(1, demo.image_srv));
    gctx.cmdlist.DrawInstanced(3, 1, 0, 0);

    // Draw audio stream samples.
    gctx.setCurrentPipeline(demo.lines_pso);
    gctx.cmdlist.IASetPrimitiveTopology(.LINESTRIP);
    gctx.cmdlist.IASetVertexBuffers(0, 1, &[_]d3d12.VERTEX_BUFFER_VIEW{.{
        .BufferLocation = gctx.lookupResource(demo.lines_buffer).?.GetGPUVirtualAddress(),
        .SizeInBytes = num_vis_samples * @sizeOf(Vec2),
        .StrideInBytes = @sizeOf(Vec2),
    }});
    gctx.cmdlist.DrawInstanced(num_vis_samples, 1, 0, 0);

    demo.guir.draw(gctx);

    gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
    gctx.flushResourceBarriers();

    gctx.endFrame();
}

pub fn main() !void {
    common.init();
    defer common.deinit();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();

    const allocator = gpa.allocator();

    var demo = try init(allocator);
    defer deinit(&demo, allocator);

    demo.audio.thread_handle = w32.CreateThread(
        null,
        0,
        audioThread,
        @ptrCast(*anyopaque, &demo.audio),
        0,
        null,
    ).?;
    hrPanicOnFail(demo.audio.client.Start());
    @atomicStore(bool, &demo.audio.is_locked, false, .Release);

    while (common.handleWindowEvents()) {
        update(&demo);
        draw(&demo);
    }
}
