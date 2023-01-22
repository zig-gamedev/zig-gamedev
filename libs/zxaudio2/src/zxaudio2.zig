const std = @import("std");
const assert = std.debug.assert;
const zwin32 = @import("zwin32");
const w32 = zwin32.w32;
const IUnknown = w32.IUnknown;
const WINAPI = w32.WINAPI;
const UINT32 = w32.UINT32;
const DWORD = w32.DWORD;
const HRESULT = w32.HRESULT;
const LONGLONG = w32.LONGLONG;
const ULONG = w32.ULONG;
const BOOL = w32.BOOL;
const xaudio2 = zwin32.xaudio2;
const mf = zwin32.mf;
const wasapi = zwin32.wasapi;
const xapo = zwin32.xapo;
const hrPanicOnFail = zwin32.hrPanicOnFail;

const WAVEFORMATEX = wasapi.WAVEFORMATEX;

const enable_debug_layer = @import("zxaudio2_options").enable_debug_layer;

const optimal_voice_format = WAVEFORMATEX{
    .wFormatTag = wasapi.WAVE_FORMAT_PCM,
    .nChannels = 1,
    .nSamplesPerSec = 48_000,
    .nAvgBytesPerSec = 2 * 48_000,
    .nBlockAlign = 2,
    .wBitsPerSample = 16,
    .cbSize = @sizeOf(WAVEFORMATEX),
};

const StopOnBufferEndVoiceCallback = struct {
    usingnamespace xaudio2.IVoiceCallback.Methods(@This());
    __v: *const xaudio2.IVoiceCallback.VTable = &vtable,

    const vtable = xaudio2.IVoiceCallback.VTable{
        .OnBufferEnd = onBufferEndImpl,
    };

    fn onBufferEndImpl(_: *xaudio2.IVoiceCallback, context: ?*anyopaque) callconv(WINAPI) void {
        const voice = @ptrCast(*xaudio2.ISourceVoice, @alignCast(@sizeOf(usize), context));
        hrPanicOnFail(voice.Stop(.{}, xaudio2.COMMIT_NOW));
    }
};
var stop_on_buffer_end_vcb: StopOnBufferEndVoiceCallback = .{};

pub const AudioContext = struct {
    allocator: std.mem.Allocator,
    device: *xaudio2.IXAudio2,
    master_voice: *xaudio2.IMasteringVoice,
    source_voices: std.ArrayList(*xaudio2.ISourceVoice),
    sound_pool: SoundPool,

    pub fn init(allocator: std.mem.Allocator) AudioContext {
        const device = blk: {
            var device: ?*xaudio2.IXAudio2 = null;
            hrPanicOnFail(xaudio2.create(&device, .{ .DEBUG_ENGINE = enable_debug_layer }, 0));
            break :blk device.?;
        };

        if (enable_debug_layer) {
            device.SetDebugConfiguration(&.{
                .TraceMask = .{ .ERRORS = true, .WARNINGS = true, .INFO = true },
                .BreakMask = .{},
                .LogThreadID = w32.TRUE,
                .LogFileline = w32.FALSE,
                .LogFunctionName = w32.FALSE,
                .LogTiming = w32.FALSE,
            }, null);
        }

        const master_voice = blk: {
            var voice: ?*xaudio2.IMasteringVoice = null;
            hrPanicOnFail(device.CreateMasteringVoice(
                &voice,
                xaudio2.DEFAULT_CHANNELS,
                xaudio2.DEFAULT_SAMPLERATE,
                .{},
                null,
                null,
                .GameEffects,
            ));
            break :blk voice.?;
        };

        var source_voices = std.ArrayList(*xaudio2.ISourceVoice).init(allocator);
        {
            var i: u32 = 0;
            while (i < 32) : (i += 1) {
                var voice: ?*xaudio2.ISourceVoice = null;
                hrPanicOnFail(device.CreateSourceVoice(
                    &voice,
                    &optimal_voice_format,
                    .{},
                    xaudio2.DEFAULT_FREQ_RATIO,
                    @ptrCast(*xaudio2.IVoiceCallback, &stop_on_buffer_end_vcb),
                    null,
                    null,
                ));
                source_voices.append(voice.?) catch unreachable;
            }
        }

        hrPanicOnFail(mf.MFStartup(mf.VERSION, 0));

        return .{
            .allocator = allocator,
            .device = device,
            .master_voice = master_voice,
            .source_voices = source_voices,
            .sound_pool = SoundPool.init(allocator),
        };
    }

    pub fn deinit(actx: *AudioContext) void {
        actx.device.StopEngine();
        hrPanicOnFail(mf.MFShutdown());
        actx.sound_pool.deinit(actx.allocator);
        for (actx.source_voices.items) |voice| {
            voice.DestroyVoice();
        }
        actx.source_voices.deinit();
        actx.master_voice.DestroyVoice();
        _ = actx.device.Release();
        actx.* = undefined;
    }

    pub fn getSourceVoice(actx: *AudioContext) *xaudio2.ISourceVoice {
        const idle_voice = blk: {
            for (actx.source_voices.items) |voice| {
                var state: xaudio2.VOICE_STATE = undefined;
                voice.GetState(&state, .{ .VOICE_NOSAMPLESPLAYED = true });
                if (state.BuffersQueued == 0) {
                    break :blk voice;
                }
            }

            var voice: ?*xaudio2.ISourceVoice = null;
            hrPanicOnFail(actx.device.CreateSourceVoice(
                &voice,
                &optimal_voice_format,
                .{},
                xaudio2.DEFAULT_FREQ_RATIO,
                @ptrCast(*xaudio2.IVoiceCallback, &stop_on_buffer_end_vcb),
                null,
                null,
            ));
            actx.source_voices.append(voice.?) catch unreachable;
            break :blk voice.?;
        };

        // Reset voice state
        hrPanicOnFail(idle_voice.SetEffectChain(null));
        hrPanicOnFail(idle_voice.SetVolume(1.0));
        hrPanicOnFail(idle_voice.SetSourceSampleRate(optimal_voice_format.nSamplesPerSec));
        hrPanicOnFail(idle_voice.SetChannelVolumes(1, &[1]f32{1.0}, xaudio2.COMMIT_NOW));
        hrPanicOnFail(idle_voice.SetFrequencyRatio(1.0, xaudio2.COMMIT_NOW));

        return idle_voice;
    }

    pub fn playSound(actx: *AudioContext, handle: SoundHandle, params: struct {
        play_begin: u32 = 0,
        play_length: u32 = 0,
        loop_begin: u32 = 0,
        loop_length: u32 = 0,
        loop_count: u32 = 0,
    }) void {
        const sound = actx.sound_pool.lookupSound(handle);
        if (sound == null)
            return;

        const voice = actx.getSourceVoice();

        hrPanicOnFail(voice.SubmitSourceBuffer(&.{
            .Flags = .{ .END_OF_STREAM = true },
            .AudioBytes = @intCast(u32, sound.?.data.?.len),
            .pAudioData = sound.?.data.?.ptr,
            .PlayBegin = params.play_begin,
            .PlayLength = params.play_length,
            .LoopBegin = params.loop_begin,
            .LoopLength = params.loop_length,
            .LoopCount = params.loop_count,
            .pContext = voice,
        }, null));

        hrPanicOnFail(voice.Start(.{}, xaudio2.COMMIT_NOW));
    }

    pub fn loadSound(actx: *AudioContext, relpath: []const u8) SoundHandle {
        var buffer: [std.fs.MAX_PATH_BYTES]u8 = undefined;
        var fba = std.heap.FixedBufferAllocator.init(buffer[0..]);
        const allocator = fba.allocator();

        const abspath = std.fs.path.join(allocator, &.{
            std.fs.selfExeDirPathAlloc(allocator) catch unreachable,
            relpath,
        }) catch unreachable;

        var abspath_w: [std.os.windows.PATH_MAX_WIDE:0]u16 = undefined;
        abspath_w[std.unicode.utf8ToUtf16Le(abspath_w[0..], abspath) catch unreachable] = 0;

        const data = loadBufferData(actx.allocator, abspath_w[0..]);
        return actx.sound_pool.addSound(data);
    }
};

pub const Stream = struct {
    critical_section: w32.CRITICAL_SECTION,
    allocator: std.mem.Allocator,
    voice: *xaudio2.ISourceVoice,
    voice_cb: *StreamVoiceCallback,
    reader: *mf.ISourceReader,
    reader_cb: *SourceReaderCallback,

    pub fn create(allocator: std.mem.Allocator, device: *xaudio2.IXAudio2, relpath: []const u8) *Stream {
        const voice_cb = blk: {
            var cb = allocator.create(StreamVoiceCallback) catch unreachable;
            cb.* = StreamVoiceCallback.init();
            break :blk cb;
        };

        var cs: w32.CRITICAL_SECTION = undefined;
        w32.InitializeCriticalSection(&cs);

        const source_reader_cb = blk: {
            var cb = allocator.create(SourceReaderCallback) catch unreachable;
            cb.* = SourceReaderCallback.init(allocator);
            break :blk cb;
        };

        var sample_rate: u32 = 0;
        const source_reader = blk: {
            var attribs: *mf.IAttributes = undefined;
            hrPanicOnFail(mf.MFCreateAttributes(&attribs, 1));
            defer _ = attribs.Release();

            hrPanicOnFail(attribs.SetUnknown(
                &mf.SOURCE_READER_ASYNC_CALLBACK,
                @ptrCast(*w32.IUnknown, source_reader_cb),
            ));

            var arena_state = std.heap.ArenaAllocator.init(allocator);
            defer arena_state.deinit();
            const arena = arena_state.allocator();

            const abspath = std.fs.path.join(arena, &.{
                std.fs.selfExeDirPathAlloc(arena) catch unreachable,
                relpath,
            }) catch unreachable;

            var abspath_w: [std.os.windows.PATH_MAX_WIDE:0]u16 = undefined;
            abspath_w[std.unicode.utf8ToUtf16Le(abspath_w[0..], abspath) catch unreachable] = 0;

            var source_reader: *mf.ISourceReader = undefined;
            hrPanicOnFail(mf.MFCreateSourceReaderFromURL(&abspath_w, attribs, &source_reader));

            var media_type: *mf.IMediaType = undefined;
            hrPanicOnFail(source_reader.GetNativeMediaType(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, &media_type));
            defer _ = media_type.Release();

            hrPanicOnFail(media_type.GetUINT32(&mf.MT_AUDIO_SAMPLES_PER_SECOND, &sample_rate));

            hrPanicOnFail(media_type.SetGUID(&mf.MT_MAJOR_TYPE, &mf.MediaType_Audio));
            hrPanicOnFail(media_type.SetGUID(&mf.MT_SUBTYPE, &mf.AudioFormat_PCM));
            hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_NUM_CHANNELS, 2));
            hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_SAMPLES_PER_SECOND, sample_rate));
            hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_BITS_PER_SAMPLE, 16));
            hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_BLOCK_ALIGNMENT, 4));
            hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_AVG_BYTES_PER_SECOND, 4 * sample_rate));
            hrPanicOnFail(media_type.SetUINT32(&mf.MT_ALL_SAMPLES_INDEPENDENT, w32.TRUE));
            hrPanicOnFail(source_reader.SetCurrentMediaType(mf.SOURCE_READER_FIRST_AUDIO_STREAM, null, media_type));

            break :blk source_reader;
        };
        assert(sample_rate != 0);

        const voice = blk: {
            var voice: ?*xaudio2.ISourceVoice = null;
            hrPanicOnFail(device.CreateSourceVoice(&voice, &.{
                .wFormatTag = wasapi.WAVE_FORMAT_PCM,
                .nChannels = 2,
                .nSamplesPerSec = sample_rate,
                .nAvgBytesPerSec = 4 * sample_rate,
                .nBlockAlign = 4,
                .wBitsPerSample = 16,
                .cbSize = @sizeOf(wasapi.WAVEFORMATEX),
            }, .{}, xaudio2.DEFAULT_FREQ_RATIO, @ptrCast(*xaudio2.IVoiceCallback, voice_cb), null, null));
            break :blk voice.?;
        };

        var stream = allocator.create(Stream) catch unreachable;
        stream.* = .{
            .critical_section = cs,
            .allocator = allocator,
            .voice = voice,
            .voice_cb = voice_cb,
            .reader = source_reader,
            .reader_cb = source_reader_cb,
        };

        voice_cb.stream = stream;
        source_reader_cb.stream = stream;

        // Start async loading/decoding
        hrPanicOnFail(source_reader.ReadSample(mf.SOURCE_READER_FIRST_AUDIO_STREAM, .{}, null, null, null, null));
        hrPanicOnFail(source_reader.ReadSample(mf.SOURCE_READER_FIRST_AUDIO_STREAM, .{}, null, null, null, null));

        return stream;
    }

    pub fn destroy(stream: *Stream) void {
        {
            const refcount = stream.reader.Release();
            assert(refcount == 0);
        }
        {
            const refcount = stream.reader_cb.Release();
            assert(refcount == 0);
        }
        w32.DeleteCriticalSection(&stream.critical_section);
        stream.voice.DestroyVoice();
        stream.allocator.destroy(stream.voice_cb);
        stream.allocator.destroy(stream);
    }

    pub fn setCurrentPosition(stream: *Stream, position: i64) void {
        w32.EnterCriticalSection(&stream.critical_section);
        defer w32.LeaveCriticalSection(&stream.critical_section);

        const pos = w32.PROPVARIANT{ .vt = w32.VT_I8, .u = .{ .hVal = position } };
        hrPanicOnFail(stream.reader.SetCurrentPosition(&w32.GUID_NULL, &pos));
        hrPanicOnFail(stream.reader.ReadSample(
            mf.SOURCE_READER_FIRST_AUDIO_STREAM,
            .{},
            null,
            null,
            null,
            null,
        ));
    }

    fn endOfStreamChunk(stream: *Stream, buffer: *mf.IMediaBuffer) void {
        w32.EnterCriticalSection(&stream.critical_section);
        defer w32.LeaveCriticalSection(&stream.critical_section);

        hrPanicOnFail(buffer.Unlock());
        const refcount = buffer.Release();
        assert(refcount == 0);

        // Request new audio buffer
        hrPanicOnFail(stream.reader.ReadSample(mf.SOURCE_READER_FIRST_AUDIO_STREAM, .{}, null, null, null, null));
    }

    fn playStreamChunk(
        stream: *Stream,
        status: HRESULT,
        _: DWORD,
        stream_flags: mf.SOURCE_READER_FLAG,
        _: LONGLONG,
        sample: ?*mf.ISample,
    ) void {
        w32.EnterCriticalSection(&stream.critical_section);
        defer w32.LeaveCriticalSection(&stream.critical_section);

        if (stream_flags.END_OF_STREAM) {
            setCurrentPosition(stream, 0);
            return;
        }
        if (status != w32.S_OK or sample == null) {
            return;
        }

        var buffer: *mf.IMediaBuffer = undefined;
        hrPanicOnFail(sample.?.ConvertToContiguousBuffer(&buffer));

        var data_ptr: [*]u8 = undefined;
        var data_len: u32 = 0;
        hrPanicOnFail(buffer.Lock(&data_ptr, null, &data_len));

        // Submit decoded buffer
        hrPanicOnFail(stream.voice.SubmitSourceBuffer(&.{
            .Flags = .{},
            .AudioBytes = data_len,
            .pAudioData = data_ptr,
            .PlayBegin = 0,
            .PlayLength = 0,
            .LoopBegin = 0,
            .LoopLength = 0,
            .LoopCount = 0,
            .pContext = buffer, // Store pointer to the buffer so that we can release it in endOfStreamChunk()
        }, null));
    }
};

const StreamVoiceCallback = struct {
    usingnamespace xaudio2.IVoiceCallback.Methods(@This());
    __v: *const xaudio2.IVoiceCallback.VTable = &vtable,

    stream: ?*Stream,

    const vtable = xaudio2.IVoiceCallback.VTable{
        .OnBufferEnd = onBufferEndImpl,
    };

    fn init() StreamVoiceCallback {
        return .{ .stream = null };
    }

    fn onBufferEndImpl(i_voice_callback: *xaudio2.IVoiceCallback, context: ?*anyopaque) callconv(WINAPI) void {
        const voice_cb = @ptrCast(*StreamVoiceCallback, i_voice_callback);
        voice_cb.stream.?.endOfStreamChunk(@ptrCast(*mf.IMediaBuffer, @alignCast(@sizeOf(usize), context)));
    }
};

const SourceReaderCallback = struct {
    usingnamespace mf.ISourceReaderCallback.Methods(@This());
    __v: *const mf.ISourceReaderCallback.VTable = &vtable,

    refcount: u32 = 1,
    allocator: std.mem.Allocator,
    stream: ?*Stream,

    const vtable = mf.ISourceReaderCallback.VTable{
        .base = .{
            .QueryInterface = queryInterfaceImpl,
            .AddRef = addRefImpl,
            .Release = releaseImpl,
        },
        .OnReadSample = onReadSampleImpl,
    };

    fn init(allocator: std.mem.Allocator) SourceReaderCallback {
        return .{
            .allocator = allocator,
            .stream = null,
        };
    }

    fn queryInterfaceImpl(
        i_unknown: *IUnknown,
        guid: *const w32.GUID,
        outobj: ?*?*anyopaque,
    ) callconv(WINAPI) HRESULT {
        assert(outobj != null);
        const source_reader_cb = @ptrCast(*SourceReaderCallback, i_unknown);

        if (std.mem.eql(u8, std.mem.asBytes(guid), std.mem.asBytes(&w32.IID_IUnknown))) {
            outobj.?.* = source_reader_cb;
            _ = source_reader_cb.AddRef();
            return w32.S_OK;
        } else if (std.mem.eql(u8, std.mem.asBytes(guid), std.mem.asBytes(&mf.IID_ISourceReaderCallback))) {
            outobj.?.* = source_reader_cb;
            _ = source_reader_cb.AddRef();
            return w32.S_OK;
        }

        outobj.?.* = null;
        return w32.E_NOINTERFACE;
    }

    fn addRefImpl(i_unknown: *IUnknown) callconv(WINAPI) ULONG {
        const source_reader_cb = @ptrCast(*SourceReaderCallback, i_unknown);
        const prev_refcount = @atomicRmw(u32, &source_reader_cb.refcount, .Add, 1, .Monotonic);
        return prev_refcount + 1;
    }

    fn releaseImpl(i_unknown: *IUnknown) callconv(WINAPI) ULONG {
        const source_reader_cb = @ptrCast(*SourceReaderCallback, i_unknown);
        const prev_refcount = @atomicRmw(u32, &source_reader_cb.refcount, .Sub, 1, .Monotonic);
        assert(prev_refcount > 0);
        if (prev_refcount == 1) {
            source_reader_cb.allocator.destroy(source_reader_cb);
        }
        return prev_refcount - 1;
    }

    fn onReadSampleImpl(
        i_source_reader_callback: *mf.ISourceReaderCallback,
        status: HRESULT,
        stream_index: DWORD,
        stream_flags: mf.SOURCE_READER_FLAG,
        timestamp: LONGLONG,
        sample: ?*mf.ISample,
    ) callconv(WINAPI) HRESULT {
        const source_reader_cb = @ptrCast(*SourceReaderCallback, i_source_reader_callback);
        source_reader_cb.stream.?.playStreamChunk(status, stream_index, stream_flags, timestamp, sample);
        return w32.S_OK;
    }
};

fn loadBufferData(allocator: std.mem.Allocator, audio_file_path: [:0]const u16) []const u8 {
    var source_reader: *mf.ISourceReader = undefined;
    hrPanicOnFail(mf.MFCreateSourceReaderFromURL(audio_file_path, null, &source_reader));
    defer _ = source_reader.Release();

    var media_type: *mf.IMediaType = undefined;
    hrPanicOnFail(source_reader.GetNativeMediaType(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, &media_type));
    defer _ = media_type.Release();

    hrPanicOnFail(media_type.SetGUID(&mf.MT_MAJOR_TYPE, &mf.MediaType_Audio));
    hrPanicOnFail(media_type.SetGUID(&mf.MT_SUBTYPE, &mf.AudioFormat_PCM));
    hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_NUM_CHANNELS, optimal_voice_format.nChannels));
    hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_SAMPLES_PER_SECOND, optimal_voice_format.nSamplesPerSec));
    hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_BITS_PER_SAMPLE, optimal_voice_format.wBitsPerSample));
    hrPanicOnFail(media_type.SetUINT32(&mf.MT_AUDIO_BLOCK_ALIGNMENT, optimal_voice_format.nBlockAlign));
    hrPanicOnFail(media_type.SetUINT32(
        &mf.MT_AUDIO_AVG_BYTES_PER_SECOND,
        optimal_voice_format.nBlockAlign * optimal_voice_format.nSamplesPerSec,
    ));
    hrPanicOnFail(media_type.SetUINT32(&mf.MT_ALL_SAMPLES_INDEPENDENT, w32.TRUE));
    hrPanicOnFail(source_reader.SetCurrentMediaType(mf.SOURCE_READER_FIRST_AUDIO_STREAM, null, media_type));

    var data = std.ArrayList(u8).init(allocator);
    while (true) {
        var flags: mf.SOURCE_READER_FLAG = .{};
        var sample: ?*mf.ISample = null;
        defer {
            if (sample) |s| _ = s.Release();
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

        var data_ptr: [*]u8 = undefined;
        var data_len: u32 = 0;
        hrPanicOnFail(buffer.Lock(&data_ptr, null, &data_len));
        data.appendSlice(data_ptr[0..data_len]) catch unreachable;
        hrPanicOnFail(buffer.Unlock());
    }
    return data.toOwnedSlice() catch unreachable;
}

pub const SoundHandle = struct {
    index: u16 align(4) = 0,
    generation: u16 = 0,
};

const Sound = struct {
    data: ?[]const u8,
};

const SoundPool = struct {
    const max_num_sounds = 256;

    sounds: []Sound,
    generations: []u16,

    fn init(allocator: std.mem.Allocator) SoundPool {
        return .{
            .sounds = blk: {
                var sounds = allocator.alloc(Sound, max_num_sounds + 1) catch unreachable;
                for (sounds) |*sound| {
                    sound.* = .{
                        .data = null,
                    };
                }
                break :blk sounds;
            },
            .generations = blk: {
                var generations = allocator.alloc(u16, max_num_sounds + 1) catch unreachable;
                for (generations) |*gen|
                    gen.* = 0;
                break :blk generations;
            },
        };
    }

    fn deinit(pool: *SoundPool, allocator: std.mem.Allocator) void {
        for (pool.sounds) |sound| {
            if (sound.data != null)
                allocator.free(sound.data.?);
        }
        allocator.free(pool.sounds);
        allocator.free(pool.generations);
        pool.* = undefined;
    }

    fn addSound(
        pool: SoundPool,
        data: []const u8,
    ) SoundHandle {
        var slot_idx: u32 = 1;
        while (slot_idx <= max_num_sounds) : (slot_idx += 1) {
            if (pool.sounds[slot_idx].data == null)
                break;
        }
        assert(slot_idx <= max_num_sounds);

        pool.sounds[slot_idx] = .{ .data = data };
        return .{
            .index = @intCast(u16, slot_idx),
            .generation = blk: {
                pool.generations[slot_idx] += 1;
                break :blk pool.generations[slot_idx];
            },
        };
    }

    fn destroySound(pool: SoundPool, allocator: std.mem.Allocator, handle: SoundHandle) void {
        var sound = pool.lookupSound(handle);
        if (sound == null)
            return;

        allocator.free(sound.data.?);
        sound.?.* = .{ .data = null };
    }

    fn isSoundValid(pool: SoundPool, handle: SoundHandle) bool {
        return handle.index > 0 and
            handle.index <= max_num_sounds and
            handle.generation > 0 and
            handle.generation == pool.generations[handle.index] and
            pool.sounds[handle.index].data != null;
    }

    fn lookupSound(pool: SoundPool, handle: SoundHandle) ?*Sound {
        if (pool.isSoundValid(handle)) {
            return &pool.sounds[handle.index];
        }
        return null;
    }
};

const SimpleAudioProcessor = struct {
    usingnamespace xapo.IXAPO.Methods(@This());
    __v: *const xapo.IXAPO.VTable = &vtable,

    refcount: u32 = 1,
    is_locked: bool = false,
    num_channels: u16 = 0,
    process: *const fn ([]f32, u32, ?*anyopaque) void,
    context: ?*anyopaque,

    const vtable = xapo.IXAPO.VTable{
        .base = .{
            .QueryInterface = queryInterfaceImpl,
            .AddRef = addRefImpl,
            .Release = releaseImpl,
        },
        .GetRegistrationProperties = getRegistrationPropertiesImpl,
        .IsInputFormatSupported = isInputFormatSupportedImpl,
        .IsOutputFormatSupported = isOutputFormatSupportedImpl,
        .Initialize = initializeImpl,
        .Reset = resetImpl,
        .LockForProcess = lockForProcessImpl,
        .UnlockForProcess = unlockForProcessImpl,
        .Process = processImpl,
        .CalcInputFrames = calcInputFramesImpl,
        .CalcOutputFrames = calcOutputFramesImpl,
    };

    const info = xapo.REGISTRATION_PROPERTIES{
        .clsid = w32.GUID_NULL,
        .FriendlyName = [_]w32.WCHAR{0} ** xapo.REGISTRATION_STRING_LENGTH,
        .CopyrightInfo = [_]w32.WCHAR{0} ** xapo.REGISTRATION_STRING_LENGTH,
        .MajorVersion = 1,
        .MinorVersion = 0,
        .Flags = .{
            .CHANNELS_MUST_MATCH = true,
            .FRAMERATE_MUST_MATCH = true,
            .BITSPERSAMPLE_MUST_MATCH = true,
            .BUFFERCOUNT_MUST_MATCH = true,
            .INPLACE_SUPPORTED = true,
            .INPLACE_REQUIRED = true,
        },
        .MinInputBufferCount = 1,
        .MaxInputBufferCount = 1,
        .MinOutputBufferCount = 1,
        .MaxOutputBufferCount = 1,
    };

    fn queryInterfaceImpl(
        i_unknown: *IUnknown,
        guid: *const w32.GUID,
        outobj: ?*?*anyopaque,
    ) callconv(WINAPI) HRESULT {
        assert(outobj != null);
        const audio_processor = @ptrCast(*SimpleAudioProcessor, i_unknown);

        if (std.mem.eql(u8, std.mem.asBytes(guid), std.mem.asBytes(&w32.IID_IUnknown))) {
            outobj.?.* = audio_processor;
            _ = audio_processor.AddRef();
            return w32.S_OK;
        } else if (std.mem.eql(u8, std.mem.asBytes(guid), std.mem.asBytes(&xapo.IID_IXAPO))) {
            outobj.?.* = audio_processor;
            _ = audio_processor.AddRef();
            return w32.S_OK;
        }

        outobj.?.* = null;
        return w32.E_NOINTERFACE;
    }

    fn addRefImpl(i_unknown: *IUnknown) callconv(WINAPI) ULONG {
        const audio_processor = @ptrCast(*SimpleAudioProcessor, i_unknown);
        return @atomicRmw(u32, &audio_processor.refcount, .Add, 1, .Monotonic) + 1;
    }

    fn releaseImpl(i_unknown: *IUnknown) callconv(WINAPI) ULONG {
        const audio_processor = @ptrCast(*SimpleAudioProcessor, i_unknown);
        const prev_refcount = @atomicRmw(u32, &audio_processor.refcount, .Sub, 1, .Monotonic);
        if (prev_refcount == 1) {
            w32.CoTaskMemFree(audio_processor);
        }
        return prev_refcount - 1;
    }

    fn getRegistrationPropertiesImpl(
        _: *xapo.IXAPO,
        props: **xapo.REGISTRATION_PROPERTIES,
    ) callconv(WINAPI) HRESULT {
        const ptr = w32.CoTaskMemAlloc(@sizeOf(xapo.REGISTRATION_PROPERTIES));
        if (ptr != null) {
            props.* = @ptrCast(*xapo.REGISTRATION_PROPERTIES, @alignCast(@sizeOf(usize), ptr.?));
            props.*.* = info;
            return w32.S_OK;
        }
        return w32.E_FAIL;
    }

    fn isInputFormatSupportedImpl(
        _: *xapo.IXAPO,
        _: *const WAVEFORMATEX,
        requested_input_format: *const WAVEFORMATEX,
        supported_input_format: ?**WAVEFORMATEX,
    ) callconv(WINAPI) HRESULT {
        if (requested_input_format.wFormatTag != wasapi.WAVE_FORMAT_IEEE_FLOAT or
            requested_input_format.nChannels < xapo.MIN_CHANNELS or
            requested_input_format.nChannels > xapo.MAX_CHANNELS or
            requested_input_format.nSamplesPerSec < xapo.MIN_FRAMERATE or
            requested_input_format.nSamplesPerSec > xapo.MAX_FRAMERATE or
            requested_input_format.wBitsPerSample != 32)
        {
            if (supported_input_format != null) {
                supported_input_format.?.*.wFormatTag = wasapi.WAVE_FORMAT_IEEE_FLOAT;
                supported_input_format.?.*.wBitsPerSample = 32;
                supported_input_format.?.*.nChannels = std.math.clamp(
                    requested_input_format.nChannels,
                    @intCast(u16, xapo.MIN_CHANNELS),
                    @intCast(u16, xapo.MAX_CHANNELS),
                );
                supported_input_format.?.*.nSamplesPerSec = std.math.clamp(
                    requested_input_format.nSamplesPerSec,
                    xapo.MIN_FRAMERATE,
                    xapo.MAX_FRAMERATE,
                );
            }
            return xapo.E_FORMAT_UNSUPPORTED;
        }
        return w32.S_OK;
    }

    fn isOutputFormatSupportedImpl(
        _: *xapo.IXAPO,
        _: *const WAVEFORMATEX,
        requested_output_format: *const WAVEFORMATEX,
        supported_output_format: ?**WAVEFORMATEX,
    ) callconv(WINAPI) HRESULT {
        if (requested_output_format.wFormatTag != wasapi.WAVE_FORMAT_IEEE_FLOAT or
            requested_output_format.nChannels < xapo.MIN_CHANNELS or
            requested_output_format.nChannels > xapo.MAX_CHANNELS or
            requested_output_format.nSamplesPerSec < xapo.MIN_FRAMERATE or
            requested_output_format.nSamplesPerSec > xapo.MAX_FRAMERATE or
            requested_output_format.wBitsPerSample != 32)
        {
            if (supported_output_format != null) {
                supported_output_format.?.*.wFormatTag = wasapi.WAVE_FORMAT_IEEE_FLOAT;
                supported_output_format.?.*.wBitsPerSample = 32;
                supported_output_format.?.*.nChannels = std.math.clamp(
                    requested_output_format.nChannels,
                    @intCast(u16, xapo.MIN_CHANNELS),
                    @intCast(u16, xapo.MAX_CHANNELS),
                );
                supported_output_format.?.*.nSamplesPerSec = std.math.clamp(
                    requested_output_format.nSamplesPerSec,
                    xapo.MIN_FRAMERATE,
                    xapo.MAX_FRAMERATE,
                );
            }
            return xapo.E_FORMAT_UNSUPPORTED;
        }
        return w32.S_OK;
    }

    fn initializeImpl(_: *xapo.IXAPO, data: ?*const anyopaque, data_size: UINT32) callconv(WINAPI) HRESULT {
        _ = data;
        _ = data_size;
        return w32.S_OK;
    }

    fn resetImpl(_: *xapo.IXAPO) callconv(WINAPI) void {}

    fn lockForProcessImpl(
        i_xapo: *xapo.IXAPO,
        num_input_params: UINT32,
        input_params: ?[*]const xapo.LOCKFORPROCESS_BUFFER_PARAMETERS,
        num_output_params: UINT32,
        output_params: ?[*]const xapo.LOCKFORPROCESS_BUFFER_PARAMETERS,
    ) callconv(WINAPI) HRESULT {
        const self = @ptrCast(*SimpleAudioProcessor, i_xapo);
        assert(self.is_locked == false);
        assert(num_input_params == 1 and num_output_params == 1);
        assert(input_params != null and output_params != null);
        assert(input_params.?[0].pFormat.wFormatTag == output_params.?[0].pFormat.wFormatTag);
        assert(input_params.?[0].pFormat.nChannels == output_params.?[0].pFormat.nChannels);
        assert(input_params.?[0].pFormat.nSamplesPerSec == output_params.?[0].pFormat.nSamplesPerSec);
        assert(input_params.?[0].pFormat.wBitsPerSample == output_params.?[0].pFormat.wBitsPerSample);
        assert(input_params.?[0].pFormat.wBitsPerSample == 32);

        self.num_channels = input_params.?[0].pFormat.nChannels;
        self.is_locked = true;

        return w32.S_OK;
    }

    fn unlockForProcessImpl(i_xapo: *xapo.IXAPO) callconv(WINAPI) void {
        const self = @ptrCast(*SimpleAudioProcessor, i_xapo);
        assert(self.is_locked == true);
        self.num_channels = 0;
        self.is_locked = false;
    }

    fn processImpl(
        i_xapo: *xapo.IXAPO,
        num_input_params: UINT32,
        input_params: ?[*]const xapo.PROCESS_BUFFER_PARAMETERS,
        num_output_params: UINT32,
        output_params: ?[*]xapo.PROCESS_BUFFER_PARAMETERS,
        is_enabled: BOOL,
    ) callconv(WINAPI) void {
        const self = @ptrCast(*SimpleAudioProcessor, i_xapo);
        assert(self.is_locked and self.num_channels > 0);
        assert(num_input_params == 1 and num_output_params == 1);
        assert(input_params != null and output_params != null);
        assert(input_params.?[0].pBuffer == output_params.?[0].pBuffer);

        if (is_enabled == w32.TRUE) {
            var samples = @ptrCast([*]f32, @alignCast(16, input_params.?[0].pBuffer)); // XAudio2 aligns data to 16.
            const num_samples = input_params.?[0].ValidFrameCount * self.num_channels;

            self.process(
                samples[0..num_samples],
                if (input_params.?[0].BufferFlags == .VALID) self.num_channels else 0,
                self.context,
            );
        }

        output_params.?[0].ValidFrameCount = input_params.?[0].ValidFrameCount;
        output_params.?[0].BufferFlags = input_params.?[0].BufferFlags;
    }

    fn calcInputFramesImpl(_: *xapo.IXAPO, num_output_frames: UINT32) callconv(WINAPI) UINT32 {
        return num_output_frames;
    }

    fn calcOutputFramesImpl(_: *xapo.IXAPO, num_input_frames: UINT32) callconv(WINAPI) UINT32 {
        return num_input_frames;
    }
};

pub fn createSimpleProcessor(
    process: *const fn ([]f32, u32, ?*anyopaque) void,
    context: ?*anyopaque,
) *IUnknown {
    const ptr = w32.CoTaskMemAlloc(@sizeOf(SimpleAudioProcessor)).?;
    const comptr = @ptrCast(*SimpleAudioProcessor, @alignCast(@sizeOf(usize), ptr));
    comptr.* = .{
        .process = process,
        .context = context,
    };
    return @ptrCast(*IUnknown, comptr);
}
