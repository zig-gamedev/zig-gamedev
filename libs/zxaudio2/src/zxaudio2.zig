const std = @import("std");
const assert = std.debug.assert;
const L = std.unicode.utf8ToUtf16LeStringLiteral;
const zwin32 = @import("zwin32");
const w32 = zwin32.base;
const xaudio2 = zwin32.xaudio2;
const mf = zwin32.mf;
const wasapi = zwin32.wasapi;
const xapo = zwin32.xapo;
const hrPanic = zwin32.hrPanic;
const hrPanicOnFail = zwin32.hrPanicOnFail;

const WAVEFORMATEX = wasapi.WAVEFORMATEX;

const enable_dx_debug = @import("build_options").enable_dx_debug;

const optimal_voice_format = WAVEFORMATEX{
    .wFormatTag = wasapi.WAVE_FORMAT_PCM,
    .nChannels = 1,
    .nSamplesPerSec = 48_000,
    .nAvgBytesPerSec = 2 * 48_000,
    .nBlockAlign = 2,
    .wBitsPerSample = 16,
    .cbSize = @sizeOf(WAVEFORMATEX),
};

const StopOnBufferEnd_VoiceCallback = struct {
    vtable: *const xaudio2.IVoiceCallbackVTable(Self) = &vtable_instance,

    const Self = @This();

    fn OnBufferEnd(_: *Self, context: ?*anyopaque) callconv(w32.WINAPI) void {
        const voice = @ptrCast(*xaudio2.ISourceVoice, @alignCast(@sizeOf(usize), context));
        hrPanicOnFail(voice.Stop(0, xaudio2.COMMIT_NOW));
    }

    const vtable_instance = xaudio2.IVoiceCallbackVTable(Self){
        .vcb = .{
            .OnVoiceProcessingPassStart = OnVoiceProcessingPassStart,
            .OnVoiceProcessingPassEnd = OnVoiceProcessingPassEnd,
            .OnStreamEnd = OnStreamEnd,
            .OnBufferStart = OnBufferStart,
            .OnBufferEnd = OnBufferEnd,
            .OnLoopEnd = OnLoopEnd,
            .OnVoiceError = OnVoiceError,
        },
    };

    fn OnVoiceProcessingPassStart(_: *Self, _: w32.UINT32) callconv(w32.WINAPI) void {}
    fn OnVoiceProcessingPassEnd(_: *Self) callconv(w32.WINAPI) void {}
    fn OnStreamEnd(_: *Self) callconv(w32.WINAPI) void {}
    fn OnBufferStart(_: *Self, _: ?*anyopaque) callconv(w32.WINAPI) void {}
    fn OnLoopEnd(_: *Self, _: ?*anyopaque) callconv(w32.WINAPI) void {}
    fn OnVoiceError(_: *Self, _: ?*anyopaque, _: w32.HRESULT) callconv(w32.WINAPI) void {}
};
var stop_on_buffer_end_vcb: StopOnBufferEnd_VoiceCallback = .{};

pub const AudioContext = struct {
    allocator: std.mem.Allocator,
    device: *xaudio2.IXAudio2,
    master_voice: *xaudio2.IMasteringVoice,
    source_voices: std.ArrayList(*xaudio2.ISourceVoice),
    sound_pool: SoundPool,

    pub fn init(allocator: std.mem.Allocator) AudioContext {
        const device = blk: {
            var device: ?*xaudio2.IXAudio2 = null;
            hrPanicOnFail(xaudio2.create(&device, if (enable_dx_debug) xaudio2.DEBUG_ENGINE else 0, 0));
            break :blk device.?;
        };

        if (enable_dx_debug) {
            device.SetDebugConfiguration(&.{
                .TraceMask = xaudio2.LOG_ERRORS | xaudio2.LOG_WARNINGS | xaudio2.LOG_INFO,
                .BreakMask = 0,
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
                0,
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
                    0,
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

    pub fn deinit(audio: *AudioContext) void {
        audio.device.StopEngine();
        hrPanicOnFail(mf.MFShutdown());
        audio.sound_pool.deinit(audio.allocator);
        for (audio.source_voices.items) |voice| {
            voice.DestroyVoice();
        }
        audio.source_voices.deinit();
        audio.master_voice.DestroyVoice();
        _ = audio.device.Release();
        audio.* = undefined;
    }

    pub fn getSourceVoice(audio: *AudioContext) *xaudio2.ISourceVoice {
        const idle_voice = blk: {
            for (audio.source_voices.items) |voice| {
                var state: xaudio2.VOICE_STATE = undefined;
                voice.GetState(&state, xaudio2.VOICE_NOSAMPLESPLAYED);
                if (state.BuffersQueued == 0) {
                    break :blk voice;
                }
            }

            var voice: ?*xaudio2.ISourceVoice = null;
            hrPanicOnFail(audio.device.CreateSourceVoice(
                &voice,
                &optimal_voice_format,
                0,
                xaudio2.DEFAULT_FREQ_RATIO,
                @ptrCast(*xaudio2.IVoiceCallback, &stop_on_buffer_end_vcb),
                null,
                null,
            ));
            audio.source_voices.append(voice.?) catch unreachable;
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
            .Flags = xaudio2.END_OF_STREAM,
            .AudioBytes = @intCast(u32, sound.?.data.?.len),
            .pAudioData = sound.?.data.?.ptr,
            .PlayBegin = params.play_begin,
            .PlayLength = params.play_length,
            .LoopBegin = params.loop_begin,
            .LoopLength = params.loop_length,
            .LoopCount = params.loop_count,
            .pContext = voice,
        }, null));

        hrPanicOnFail(voice.Start(0, xaudio2.COMMIT_NOW));
    }

    pub fn loadSound(actx: *AudioContext, sound_file_path: [:0]const u16) SoundHandle {
        const data = loadBufferData(actx.allocator, sound_file_path);
        return actx.sound_pool.addSound(data);
    }
};

pub const Stream = struct {
    critical_section: w32.CRITICAL_SECTION,
    allocator: std.mem.Allocator,
    voice: *xaudio2.ISourceVoice,
    voice_cb: *VoiceCallback,
    reader: *mf.ISourceReader,
    reader_cb: *SourceReaderCallback,

    pub fn create(allocator: std.mem.Allocator, device: *xaudio2.IXAudio2, file_path: [:0]const u16) *Stream {
        const voice_cb = blk: {
            var cb = allocator.create(VoiceCallback) catch unreachable;
            cb.* = VoiceCallback.init();
            break :blk cb;
        };

        var cs: w32.CRITICAL_SECTION = undefined;
        w32.kernel32.InitializeCriticalSection(&cs);

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

            var source_reader: *mf.ISourceReader = undefined;
            hrPanicOnFail(mf.MFCreateSourceReaderFromURL(file_path, attribs, &source_reader));

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
            }, 0, xaudio2.DEFAULT_FREQ_RATIO, @ptrCast(*xaudio2.IVoiceCallback, voice_cb), null, null));
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
        hrPanicOnFail(source_reader.ReadSample(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, null, null, null, null));
        hrPanicOnFail(source_reader.ReadSample(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, null, null, null, null));

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
        w32.kernel32.DeleteCriticalSection(&stream.critical_section);
        stream.voice.DestroyVoice();
        stream.allocator.destroy(stream.voice_cb);
        stream.allocator.destroy(stream);
    }

    pub fn setCurrentPosition(stream: *Stream, position: i64) void {
        w32.kernel32.EnterCriticalSection(&stream.critical_section);
        defer w32.kernel32.LeaveCriticalSection(&stream.critical_section);

        const pos = w32.PROPVARIANT{ .vt = w32.VT_I8, .u = .{ .hVal = position } };
        hrPanicOnFail(stream.reader.SetCurrentPosition(&w32.GUID_NULL, &pos));
        hrPanicOnFail(stream.reader.ReadSample(
            mf.SOURCE_READER_FIRST_AUDIO_STREAM,
            0,
            null,
            null,
            null,
            null,
        ));
    }

    fn onBufferEnd(stream: *Stream, buffer: *mf.IMediaBuffer) void {
        w32.kernel32.EnterCriticalSection(&stream.critical_section);
        defer w32.kernel32.LeaveCriticalSection(&stream.critical_section);

        hrPanicOnFail(buffer.Unlock());
        const refcount = buffer.Release();
        assert(refcount == 0);

        // Request new audio buffer
        hrPanicOnFail(stream.reader.ReadSample(mf.SOURCE_READER_FIRST_AUDIO_STREAM, 0, null, null, null, null));
    }

    fn onReadSample(
        stream: *Stream,
        status: w32.HRESULT,
        _: w32.DWORD,
        stream_flags: w32.DWORD,
        _: w32.LONGLONG,
        sample: ?*mf.ISample,
    ) void {
        w32.kernel32.EnterCriticalSection(&stream.critical_section);
        defer w32.kernel32.LeaveCriticalSection(&stream.critical_section);

        if ((stream_flags & mf.SOURCE_READERF_ENDOFSTREAM) != 0) {
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
            .Flags = 0,
            .AudioBytes = data_len,
            .pAudioData = data_ptr,
            .PlayBegin = 0,
            .PlayLength = 0,
            .LoopBegin = 0,
            .LoopLength = 0,
            .LoopCount = 0,
            .pContext = buffer, // Store pointer to the buffer so that we can release it in onBufferEnd()
        }, null));
    }

    const VoiceCallback = struct {
        vtable: *const xaudio2.IVoiceCallbackVTable(VoiceCallback) = &vtable_voice_cb,
        stream: ?*Stream,

        fn init() VoiceCallback {
            return .{ .stream = null };
        }

        fn OnBufferEnd(voice_cb: *VoiceCallback, context: ?*anyopaque) callconv(w32.WINAPI) void {
            voice_cb.stream.?.onBufferEnd(@ptrCast(*mf.IMediaBuffer, @alignCast(@sizeOf(usize), context)));
        }

        const vtable_voice_cb = xaudio2.IVoiceCallbackVTable(VoiceCallback){
            .vcb = .{
                .OnVoiceProcessingPassStart = VoiceCallback.OnVoiceProcessingPassStart,
                .OnVoiceProcessingPassEnd = VoiceCallback.OnVoiceProcessingPassEnd,
                .OnStreamEnd = VoiceCallback.OnStreamEnd,
                .OnBufferStart = VoiceCallback.OnBufferStart,
                .OnBufferEnd = VoiceCallback.OnBufferEnd,
                .OnLoopEnd = VoiceCallback.OnLoopEnd,
                .OnVoiceError = VoiceCallback.OnVoiceError,
            },
        };

        fn OnVoiceProcessingPassStart(_: *VoiceCallback, _: w32.UINT32) callconv(w32.WINAPI) void {}
        fn OnVoiceProcessingPassEnd(_: *VoiceCallback) callconv(w32.WINAPI) void {}
        fn OnStreamEnd(_: *VoiceCallback) callconv(w32.WINAPI) void {}
        fn OnBufferStart(_: *VoiceCallback, _: ?*anyopaque) callconv(w32.WINAPI) void {}
        fn OnLoopEnd(_: *VoiceCallback, _: ?*anyopaque) callconv(w32.WINAPI) void {}
        fn OnVoiceError(_: *VoiceCallback, _: ?*anyopaque, _: w32.HRESULT) callconv(w32.WINAPI) void {}
    };

    const SourceReaderCallback = struct {
        vtable: *const mf.ISourceReaderCallbackVTable(SourceReaderCallback) = &vtable_source_reader_cb,
        refcount: u32 = 1,
        allocator: std.mem.Allocator,
        stream: ?*Stream,

        fn init(allocator: std.mem.Allocator) SourceReaderCallback {
            return .{
                .allocator = allocator,
                .stream = null,
            };
        }

        const vtable_source_reader_cb = mf.ISourceReaderCallbackVTable(SourceReaderCallback){
            .unknown = .{
                .QueryInterface = SourceReaderCallback.QueryInterface,
                .AddRef = SourceReaderCallback.AddRef,
                .Release = SourceReaderCallback.Release,
            },
            .cb = .{
                .OnReadSample = SourceReaderCallback.OnReadSample,
                .OnFlush = SourceReaderCallback.OnFlush,
                .OnEvent = SourceReaderCallback.OnEvent,
            },
        };

        fn QueryInterface(
            source_reader_cb: *SourceReaderCallback,
            guid: *const w32.GUID,
            outobj: ?*?*anyopaque,
        ) callconv(w32.WINAPI) w32.HRESULT {
            assert(outobj != null);

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

        fn AddRef(source_reader_cb: *SourceReaderCallback) callconv(w32.WINAPI) w32.ULONG {
            const prev_refcount = @atomicRmw(u32, &source_reader_cb.refcount, .Add, 1, .Monotonic);
            return prev_refcount + 1;
        }

        fn Release(source_reader_cb: *SourceReaderCallback) callconv(w32.WINAPI) w32.ULONG {
            const prev_refcount = @atomicRmw(u32, &source_reader_cb.refcount, .Sub, 1, .Monotonic);
            assert(prev_refcount > 0);
            if (prev_refcount == 1) {
                source_reader_cb.allocator.destroy(source_reader_cb);
            }
            return prev_refcount - 1;
        }

        fn OnReadSample(
            source_reader_cb: *SourceReaderCallback,
            status: w32.HRESULT,
            stream_index: w32.DWORD,
            stream_flags: w32.DWORD,
            timestamp: w32.LONGLONG,
            sample: ?*mf.ISample,
        ) callconv(w32.WINAPI) w32.HRESULT {
            source_reader_cb.stream.?.onReadSample(status, stream_index, stream_flags, timestamp, sample);
            return w32.S_OK;
        }

        fn OnFlush(_: *SourceReaderCallback, _: w32.DWORD) callconv(w32.WINAPI) w32.HRESULT {
            return w32.S_OK;
        }

        fn OnEvent(_: *SourceReaderCallback, _: w32.DWORD, _: *mf.IMediaEvent) callconv(w32.WINAPI) w32.HRESULT {
            return w32.S_OK;
        }
    };
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
        var flags: w32.DWORD = 0;
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
        data.appendSlice(data_ptr[0..data_len]) catch unreachable;
        hrPanicOnFail(buffer.Unlock());
    }
    return data.toOwnedSlice();
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

const SimpleAudioProcessor = extern struct {
    v: *const xapo.IXAPOVTable(SimpleAudioProcessor) = &vtable,
    refcount: u32 = 1,
    is_locked: bool = false,
    num_channels: u16 = 0,
    process: *const fn ([]f32, u32, ?*anyopaque) void,
    context: ?*anyopaque,

    const Self = @This();

    const vtable = xapo.IXAPOVTable(SimpleAudioProcessor){
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

    const info = xapo.REGISTRATION_PROPERTIES{
        .clsid = w32.GUID_NULL,
        .FriendlyName = [_]w32.WCHAR{0} ** xapo.REGISTRATION_STRING_LENGTH,
        .CopyrightInfo = [_]w32.WCHAR{0} ** xapo.REGISTRATION_STRING_LENGTH,
        .MajorVersion = 1,
        .MinorVersion = 0,
        .Flags = xapo.FLAG_CHANNELS_MUST_MATCH |
            xapo.FLAG_FRAMERATE_MUST_MATCH |
            xapo.FLAG_BITSPERSAMPLE_MUST_MATCH |
            xapo.FLAG_BUFFERCOUNT_MUST_MATCH |
            xapo.FLAG_INPLACE_SUPPORTED |
            xapo.FLAG_INPLACE_REQUIRED,
        .MinInputBufferCount = 1,
        .MaxInputBufferCount = 1,
        .MinOutputBufferCount = 1,
        .MaxOutputBufferCount = 1,
    };

    fn QueryInterface(
        self: *Self,
        guid: *const w32.GUID,
        outobj: ?*?*anyopaque,
    ) callconv(w32.WINAPI) w32.HRESULT {
        assert(outobj != null);

        if (std.mem.eql(u8, std.mem.asBytes(guid), std.mem.asBytes(&w32.IID_IUnknown))) {
            outobj.?.* = self;
            _ = self.AddRef();
            return w32.S_OK;
        } else if (std.mem.eql(u8, std.mem.asBytes(guid), std.mem.asBytes(&xapo.IID_IXAPO))) {
            outobj.?.* = self;
            _ = self.AddRef();
            return w32.S_OK;
        }

        outobj.?.* = null;
        return w32.E_NOINTERFACE;
    }

    fn AddRef(self: *Self) callconv(w32.WINAPI) w32.ULONG {
        return @atomicRmw(u32, &self.refcount, .Add, 1, .Monotonic) + 1;
    }

    fn Release(self: *Self) callconv(w32.WINAPI) w32.ULONG {
        const prev_refcount = @atomicRmw(u32, &self.refcount, .Sub, 1, .Monotonic);
        if (prev_refcount == 1) {
            w32.ole32.CoTaskMemFree(self);
        }
        return prev_refcount - 1;
    }

    fn GetRegistrationProperties(
        _: *Self,
        props: **xapo.REGISTRATION_PROPERTIES,
    ) callconv(w32.WINAPI) w32.HRESULT {
        const ptr = w32.CoTaskMemAlloc(@sizeOf(xapo.REGISTRATION_PROPERTIES));
        if (ptr != null) {
            props.* = @ptrCast(*xapo.REGISTRATION_PROPERTIES, @alignCast(@sizeOf(usize), ptr.?));
            props.*.* = info;
            return w32.S_OK;
        }
        return w32.E_FAIL;
    }

    fn IsInputFormatSupported(
        _: *Self,
        _: *const WAVEFORMATEX,
        requested_input_format: *const WAVEFORMATEX,
        supported_input_format: ?**WAVEFORMATEX,
    ) callconv(w32.WINAPI) w32.HRESULT {
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

    fn IsOutputFormatSupported(
        _: *Self,
        _: *const WAVEFORMATEX,
        requested_output_format: *const WAVEFORMATEX,
        supported_output_format: ?**WAVEFORMATEX,
    ) callconv(w32.WINAPI) w32.HRESULT {
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

    fn Initialize(_: *Self, data: ?*const anyopaque, data_size: w32.UINT32) callconv(w32.WINAPI) w32.HRESULT {
        _ = data;
        _ = data_size;
        return w32.S_OK;
    }

    fn Reset(_: *Self) callconv(w32.WINAPI) void {}

    fn LockForProcess(
        self: *Self,
        num_input_params: w32.UINT32,
        input_params: ?[*]const xapo.LOCKFORPROCESS_BUFFER_PARAMETERS,
        num_output_params: w32.UINT32,
        output_params: ?[*]const xapo.LOCKFORPROCESS_BUFFER_PARAMETERS,
    ) callconv(w32.WINAPI) w32.HRESULT {
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

    fn UnlockForProcess(self: *Self) callconv(w32.WINAPI) void {
        assert(self.is_locked == true);
        self.num_channels = 0;
        self.is_locked = false;
    }

    fn Process(
        self: *Self,
        num_input_params: w32.UINT32,
        input_params: ?[*]const xapo.PROCESS_BUFFER_PARAMETERS,
        num_output_params: w32.UINT32,
        output_params: ?[*]xapo.PROCESS_BUFFER_PARAMETERS,
        is_enabled: w32.BOOL,
    ) callconv(w32.WINAPI) void {
        assert(self.is_locked and self.num_channels > 0);
        assert(num_input_params == 1 and num_output_params == 1);
        assert(input_params != null and output_params != null);
        assert(input_params.?[0].pBuffer == output_params.?[0].pBuffer);

        if (is_enabled == w32.TRUE) {
            var samples = @ptrCast([*]f32, @alignCast(16, input_params.?[0].pBuffer)); // XAudio2 aligns data to 16.
            const num_samples = input_params.?[0].ValidFrameCount * self.num_channels;

            self.process.*(
                samples[0..num_samples],
                if (input_params.?[0].BufferFlags == .VALID) self.num_channels else 0,
                self.context,
            );
        }

        output_params.?[0].ValidFrameCount = input_params.?[0].ValidFrameCount;
        output_params.?[0].BufferFlags = input_params.?[0].BufferFlags;
    }

    fn CalcInputFrames(_: *Self, num_output_frames: w32.UINT32) callconv(w32.WINAPI) w32.UINT32 {
        return num_output_frames;
    }

    fn CalcOutputFrames(_: *Self, num_input_frames: w32.UINT32) callconv(w32.WINAPI) w32.UINT32 {
        return num_input_frames;
    }
};

pub fn createSimpleProcessor(
    process: *const fn ([]f32, u32, ?*anyopaque) void,
    context: ?*anyopaque,
) *w32.IUnknown {
    const ptr = w32.CoTaskMemAlloc(@sizeOf(SimpleAudioProcessor)).?;
    const comptr = @ptrCast(*SimpleAudioProcessor, @alignCast(@sizeOf(usize), ptr));
    comptr.* = .{
        .process = process,
        .context = context,
    };
    return @ptrCast(*w32.IUnknown, comptr);
}
