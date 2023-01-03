pub const version = @import("std").SemanticVersion{ .major = 0, .minor = 9, .patch = 3 };
const std = @import("std");
const assert = std.debug.assert;
//--------------------------------------------------------------------------------------------------
//
// Misc
//
//--------------------------------------------------------------------------------------------------
pub fn init(allocator: std.mem.Allocator) void {
    assert(mem_allocator == null);
    mem_allocator = allocator;
    mem_allocations = std.AutoHashMap(usize, usize).init(allocator);
    mem_allocations.?.ensureTotalCapacity(16) catch @panic("zaudio: out of memory");

    zaudioMallocPtr = zaudioMalloc;
    zaudioReallocPtr = zaudioRealloc;
    zaudioFreePtr = zaudioFree;

    zaudioMemInit();
}
extern fn zaudioMemInit() callconv(.C) void;

pub fn deinit() void {
    assert(mem_allocator != null);
    assert(mem_allocations.?.count() == 0);
    mem_allocations.?.deinit();
    mem_allocations = null;
    mem_allocator = null;
}

pub const Error = error{
    GenericError,
    InvalidArgs,
    InvalidOperation,
    OutOfMemory,
    OutOfRange,
    AccessDenied,
    DoesNotExist,
    AlreadyExists,
    TooManyOpenFiles,
    InvalidFile,
    TooBig,
    PathTooLong,
    NameTooLong,
    NotDirectory,
    IsDirectory,
    DirectoryNotEmpty,
    AtEnd,
    NoSpace,
    Busy,
    IoError,
    Interrupt,
    Unavailable,
    AlreadyInUse,
    BadAddress,
    BadSeek,
    BadPipe,
    Deadlock,
    TooManyLinks,
    NotImplemented,
    NoMessage,
    BadMessage,
    NoDataAvailable,
    InvalidData,
    Timeout,
    NoNetwork,
    NotUnique,
    NotSocket,
    NoAddress,
    BadProtocol,
    ProtocolUnavailable,
    ProtocolNotSupported,
    ProtocolFamilyNotSupported,
    AddressFamilyNotSupported,
    SocketNotSupported,
    ConnectionReset,
    AlreadyConnected,
    NotConnected,
    ConnectionRefused,
    NoHost,
    InProgress,
    Cancelled,
    MemoryAlreadyMapped,

    FormatNotSupported,
    DeviceTypeNotSupported,
    ShareModeNotSupported,
    NoBackend,
    NoDevice,
    ApiNotFound,
    InvalidDeviceConfig,
    Loop,

    DeviceNotInitialized,
    DeviceAlreadyInitialized,
    DeviceNotStarted,
    DeviceNotStopped,

    FailedToInitBackend,
    FailedToOpenBackendDevice,
    FailedToStartBackendDevice,
    FailedToStopBackendDevice,
};

pub const Result = enum(i32) {
    success = 0,
    generic_error = -1,
    invalid_args = -2,
    invalid_operation = -3,
    out_of_memory = -4,
    out_of_range = -5,
    access_denied = -6,
    does_not_exist = -7,
    already_exists = -8,
    too_many_open_files = -9,
    invalid_file = -10,
    too_big = -11,
    path_too_long = -12,
    name_too_long = -13,
    not_directory = -14,
    is_directory = -15,
    directory_not_empty = -16,
    at_end = -17,
    no_space = -18,
    busy = -19,
    io_error = -20,
    interrupt = -21,
    unavailable = -22,
    already_in_use = -23,
    bad_address = -24,
    bad_seek = -25,
    bad_pipe = -26,
    deadlock = -27,
    too_many_links = -28,
    not_implemented = -29,
    no_message = -30,
    bad_message = -31,
    no_data_available = -32,
    invalid_data = -33,
    timeout = -34,
    no_network = -35,
    not_unique = -36,
    not_socket = -37,
    no_address = -38,
    bad_protocol = -39,
    protocol_unavailable = -40,
    protocol_not_supported = -41,
    protocol_family_not_supported = -42,
    address_family_not_supported = -43,
    socket_not_supported = -44,
    connection_reset = -45,
    already_connected = -46,
    not_connected = -47,
    connection_refused = -48,
    no_host = -49,
    in_progress = -50,
    cancelled = -51,
    memory_already_mapped = -52,

    format_not_supported = -100,
    device_type_not_supported = -101,
    share_mode_not_supported = -102,
    no_backend = -103,
    no_device = -104,
    api_not_found = -105,
    invalid_device_config = -106,
    loop = -107,

    device_not_initialized = -200,
    device_already_initialized = -201,
    device_not_started = -202,
    device_not_stopped = -203,

    failed_to_init_backend = -300,
    failed_to_open_backend_device = -301,
    failed_to_start_backend_device = -302,
    failed_to_stop_backend_device = -303,
};

pub fn maybeError(result: Result) Error!void {
    return switch (result) {
        .success => {},
        .generic_error => Error.GenericError,
        .invalid_args => Error.InvalidArgs,
        .invalid_operation => Error.InvalidOperation,
        .out_of_memory => Error.OutOfMemory,
        .out_of_range => error.OutOfRange,
        .access_denied => error.AccessDenied,
        .does_not_exist => error.DoesNotExist,
        .already_exists => error.AlreadyExists,
        .too_many_open_files => error.TooManyOpenFiles,
        .invalid_file => error.InvalidFile,
        .too_big => error.TooBig,
        .path_too_long => error.PathTooLong,
        .name_too_long => error.NameTooLong,
        .not_directory => error.NotDirectory,
        .is_directory => error.IsDirectory,
        .directory_not_empty => error.DirectoryNotEmpty,
        .at_end => error.AtEnd,
        .no_space => error.NoSpace,
        .busy => error.Busy,
        .io_error => error.IoError,
        .interrupt => error.Interrupt,
        .unavailable => error.Unavailable,
        .already_in_use => error.AlreadyInUse,
        .bad_address => error.BadAddress,
        .bad_seek => error.BadSeek,
        .bad_pipe => error.BadPipe,
        .deadlock => error.Deadlock,
        .too_many_links => error.TooManyLinks,
        .not_implemented => error.NotImplemented,
        .no_message => error.NoMessage,
        .bad_message => error.BadMessage,
        .no_data_available => error.NoDataAvailable,
        .invalid_data => error.InvalidData,
        .timeout => error.Timeout,
        .no_network => error.NoNetwork,
        .not_unique => error.NotUnique,
        .not_socket => error.NotSocket,
        .no_address => error.NoAddress,
        .bad_protocol => error.BadProtocol,
        .protocol_unavailable => error.ProtocolUnavailable,
        .protocol_not_supported => error.ProtocolNotSupported,
        .protocol_family_not_supported => error.ProtocolFamilyNotSupported,
        .address_family_not_supported => error.AddressFamilyNotSupported,
        .socket_not_supported => error.SocketNotSupported,
        .connection_reset => error.ConnectionReset,
        .already_connected => error.AlreadyConnected,
        .not_connected => error.NotConnected,
        .connection_refused => error.ConnectionRefused,
        .no_host => error.NoHost,
        .in_progress => error.InProgress,
        .cancelled => error.Cancelled,
        .memory_already_mapped => error.MemoryAlreadyMapped,

        .format_not_supported => error.FormatNotSupported,
        .device_type_not_supported => error.DeviceTypeNotSupported,
        .share_mode_not_supported => error.ShareModeNotSupported,
        .no_backend => error.NoBackend,
        .no_device => error.NoDevice,
        .api_not_found => error.ApiNotFound,
        .invalid_device_config => error.InvalidDeviceConfig,
        .loop => error.Loop,

        .device_not_initialized => error.DeviceNotInitialized,
        .device_already_initialized => error.DeviceAlreadyInitialized,
        .device_not_started => error.DeviceNotStarted,
        .device_not_stopped => error.DeviceNotStopped,

        .failed_to_init_backend => error.FailedToInitBackend,
        .failed_to_open_backend_device => error.FailedToOpenBackendDevice,
        .failed_to_start_backend_device => error.FailedToStartBackendDevice,
        .failed_to_stop_backend_device => error.FailedToStopBackendDevice,
    };
}

pub const PanMode = enum(u32) {
    balance,
    pan,
};

pub const AttenuationModel = enum(u32) {
    none,
    inverse,
    linear,
    exponential,
};

pub const Positioning = enum(u32) {
    absolute,
    relative,
};

pub const Format = enum(u32) {
    unknown,
    unsigned8,
    signed16,
    signed24,
    signed32,
    float32,
};

pub const PerformanceProfile = enum(u32) {
    low_latency,
    conservative,
};

pub const ResampleAlgorithm = enum(u32) {
    linear,
    custom,
};

pub const ChannelMixMode = enum(u32) {
    rectangular,
    simple,
    custom_weights,

    pub const default: ChannelMixMode = .rectangular;
};

pub const ShareMode = enum(u32) {
    shared,
    exclusive,
};

pub const WasapiUsage = enum(u32) {
    default,
    games,
    pro_audio,
};

pub const OpenslStreamType = enum(u32) {
    default,
    voice,
    system,
    ring,
    media,
    alarm,
    notification,
};

pub const OpenslRecordingPreset = enum(u32) {
    default,
    generic,
    camcorder,
    recognition,
    voice_communication,
    voice_unprocessed,
};

pub const AaudioUsage = enum(u32) {
    default,
    media,
    voice_communication,
    voice_communication_signalling,
    alarm,
    notification,
    notification_ringtone,
    notification_event,
    assistance_accessibility,
    assistance_navigation_guidance,
    assistance_sonification,
    game,
    assitant,
    emergency,
    safety,
    vehicle_status,
    announcement,
};

pub const AaudioContentType = enum(u32) {
    default,
    speech,
    music,
    movie,
    sonification,
};

pub const AaudioInputPreset = enum(u32) {
    default,
    generic,
    camcorder,
    voice_recognition,
    voice_communication,
    unprocessed,
    voice_performance,
};

pub const MonoExpansionMode = enum(u32) {
    duplicate,
    average,
    stereo_only,

    pub const default: MonoExpansionMode = .duplicate;
};

pub const AllocationCallbacks = extern struct {
    user_data: ?*anyopaque,

    onMalloc: ?*const fn (len: usize, user_data: ?*anyopaque) callconv(.C) ?*anyopaque,

    onRealloc: ?*const fn (
        ptr: ?*anyopaque,
        len: usize,
        user_data: ?*anyopaque,
    ) callconv(.C) ?*anyopaque,

    onFree: ?*const fn (ptr: ?*anyopaque, user_data: ?*anyopaque) callconv(.C) void,
};

pub const Bool32 = enum(u32) {
    false32,
    true32,
};

pub const Channel = u8;

pub const ResourceManager = opaque {
    // TODO: Add methods.
};

pub const Vfs = opaque {
    // TODO: Add methods.
};

pub const Context = opaque {
    // TODO: Add methods.
};

pub const Log = opaque {
    // TODO: Add methods.
};
//--------------------------------------------------------------------------------------------------
//
// DataSource
//
//--------------------------------------------------------------------------------------------------
pub const DataSource = opaque {
    pub usingnamespace Methods(@This());

    pub const destroy = zaudioDataSourceDestroy;
    extern fn zaudioDataSourceDestroy(handle: *DataSource) void;

    fn Methods(comptime T: type) type {
        return struct {
            pub fn asDataSource(handle: *const T) *const DataSource {
                return @ptrCast(*const DataSource, handle);
            }
            pub fn asDataSourceMut(handle: *T) *DataSource {
                return @ptrCast(*DataSource, handle);
            }

            // TODO: Add missing methods.
        };
    }

    pub const Config = extern struct {
        vtable: *VTable,

        pub fn init() Config {
            var config: Config = undefined;
            zaudioDataSourceConfigInit(&config);
            return config;
        }
        extern fn zaudioDataSourceConfigInit(out_config: *Config) void;
    };

    pub fn create(config: Config) Error!*DataSource {
        var handle: ?*DataSource = null;
        try maybeError(zaudioDataSourceCreate(&config, &handle));
        return handle.?;
    }
    extern fn zaudioDataSourceCreate(config: *const Config, out_handle: ?*?*DataSource) Result;

    pub const Flags = packed struct(u32) {
        self_managed_range_and_loop_point: bool = false,
        _padding: u31 = 0,
    };

    pub const VTable = extern struct {
        onRead: ?*const fn (
            ds: *DataSource,
            frames_out: ?*anyopaque,
            frame_count: u64,
            frames_read: *u64,
        ) callconv(.C) Result,

        onSeek: ?*const fn (ds: *DataSource, frame_index: u64) callconv(.C) Result,

        onGetDataFormat: ?*const fn (
            ds: *DataSource,
            format: ?*Format,
            channels: ?*u32,
            sample_rate: ?*u32,
            channel_map: ?[*]Channel,
            channel_map_cap: usize,
        ) callconv(.C) Result,

        onGetCursor: ?*const fn (ds: *DataSource, cursor: ?*u64) callconv(.C) Result,

        onGetLength: ?*const fn (ds: *DataSource, length: ?*u64) callconv(.C) Result,

        onSetLooping: ?*const fn (ds: *DataSource, is_looping: Bool32) callconv(.C) Result,

        flags: Flags,
    };
};
//--------------------------------------------------------------------------------------------------
//
// Waveform (-> DataSource)
//
//--------------------------------------------------------------------------------------------------
pub const Waveform = opaque {
    pub usingnamespace DataSource.Methods(@This());

    pub const destroy = zaudioWaveformDestroy;
    extern fn zaudioWaveformDestroy(waveform: *Waveform) void;

    pub fn setAmplitude(waveform: *Waveform, amplitude: f64) Error!void {
        try maybeError(ma_waveform_set_amplitude(waveform, amplitude));
    }
    extern fn ma_waveform_set_amplitude(waveform: *Waveform, amplitude: f64) Result;

    pub fn setFrequency(waveform: *Waveform, frequency: f64) Error!void {
        try maybeError(ma_waveform_set_frequency(waveform, frequency));
    }
    extern fn ma_waveform_set_frequency(waveform: *Waveform, frequency: f64) Result;

    pub fn setType(waveform: *Waveform, waveform_type: Type) Error!void {
        try maybeError(ma_waveform_set_type(waveform, waveform_type));
    }
    extern fn ma_waveform_set_type(waveform: *Waveform, waveform_type: Type) Result;

    pub fn setSampleRate(waveform: *Waveform, sample_rate: u32) Error!void {
        try maybeError(ma_waveform_set_sample_rate(waveform, sample_rate));
    }
    extern fn ma_waveform_set_sample_rate(waveform: *Waveform, sample_rate: u32) Result;

    pub fn create(config: Config) Error!*Waveform {
        var handle: ?*Waveform = null;
        try maybeError(zaudioWaveformCreate(&config, &handle));
        return handle.?;
    }
    extern fn zaudioWaveformCreate(config: *const Config, handle: ?*?*Waveform) Result;

    pub const Type = enum(u32) {
        sine,
        square,
        triangle,
        sawtooth,
    };

    pub const Config = extern struct {
        format: Format,
        channels: u32,
        sampleRate: u32,
        waveform_type: Type,
        amplitude: f64,
        frequency: f64,

        pub fn init(
            format: Format,
            num_channels: u32,
            sample_rate: u32,
            waveform_type: Type,
            amplitude: f64,
            frequency: f64,
        ) Config {
            var config: Config = undefined;
            zaudioWaveformConfigInit(
                format,
                num_channels,
                sample_rate,
                waveform_type,
                amplitude,
                frequency,
                &config,
            );
            return config;
        }
        extern fn zaudioWaveformConfigInit(
            format: Format,
            channels: u32,
            sampleRate: u32,
            waveform_type: Type,
            amplitude: f64,
            frequency: f64,
            out_config: *Config,
        ) void;
    };
};
//--------------------------------------------------------------------------------------------------
//
// Noise (-> DataSource)
//
//--------------------------------------------------------------------------------------------------
pub const Noise = opaque {
    pub usingnamespace DataSource.Methods(@This());

    pub const destroy = zaudioNoiseDestroy;
    extern fn zaudioNoiseDestroy(handle: *Noise) void;

    pub fn setAmplitude(noise: *Noise, amplitude: f64) Error!void {
        try maybeError(ma_noise_set_amplitude(noise, amplitude));
    }
    extern fn ma_noise_set_amplitude(noise: *Noise, amplitude: f64) Result;

    pub fn setType(noise: *Noise, noise_type: Type) Error!void {
        try maybeError(ma_noise_set_type(noise, noise_type));
    }
    extern fn ma_noise_set_type(noise: *Noise, noise_type: Type) Result;

    pub fn create(config: Config) Error!*Noise {
        var handle: ?*Noise = null;
        try maybeError(zaudioNoiseCreate(&config, &handle));
        return handle.?;
    }
    extern fn zaudioNoiseCreate(config: *const Config, out_handle: ?*?*Noise) Result;

    pub const Type = enum(u32) {
        white,
        pink,
        brownian,
    };

    pub const Config = extern struct {
        format: Format,
        num_channels: u32,
        noise_type: Type,
        seed: i32,
        amplitude: f64,
        duplicate_channels: Bool32,

        pub fn init(
            format: Format,
            num_channels: u32,
            noise_type: Type,
            seed: i32,
            amplitude: f64,
        ) Config {
            var config: Config = undefined;
            zaudioNoiseConfigInit(format, num_channels, noise_type, seed, amplitude, &config);
            return config;
        }
        extern fn zaudioNoiseConfigInit(
            format: Format,
            num_channels: u32,
            noise_type: Type,
            seed: i32,
            amplitude: f64,
            out_config: *Config,
        ) void;
    };
};
//--------------------------------------------------------------------------------------------------
//
// AudioBuffer
//
//--------------------------------------------------------------------------------------------------
pub const AudioBuffer = opaque {
    pub const Config = extern struct {
        format: Format,
        channels: u32,
        sample_rate: u32,
        size_in_frames: u64,
        data: ?*const anyopaque,
        allocation_callbacks: AllocationCallbacks,

        extern fn zaudioAudioBufferConfigInit(
            format: Format,
            channels: u32,
            size_in_frames: u64,
            data: ?*const anyopaque,
            out_config: *Config,
        ) void;

        pub fn init(
            format: Format,
            channels: u32,
            size_in_frames: u64,
            data: ?*const anyopaque,
        ) Config {
            var config: Config = undefined;
            zaudioAudioBufferConfigInit(format, channels, size_in_frames, data, &config);
            return config;
        }
    };

    pub fn create(config: Config) Error!*AudioBuffer {
        var handle: ?*AudioBuffer = null;
        try maybeError(zaudioAudioBufferCreate(&config, &handle));
        return handle.?;
    }
    extern fn zaudioAudioBufferCreate(config: *const Config, out_handle: ?*?*AudioBuffer) Result;

    pub const destroy = zaudioAudioBufferDestroy;
    extern fn zaudioAudioBufferDestroy(handle: *AudioBuffer) void;

    pub fn asDataSource(audio_buffer: *const AudioBuffer) *const DataSource {
        return @ptrCast(*const DataSource, audio_buffer);
    }
    pub fn asDataSourceMut(audio_buffer: *AudioBuffer) *DataSource {
        return @ptrCast(*DataSource, audio_buffer);
    }
};
//--------------------------------------------------------------------------------------------------
//
// Node
//
//--------------------------------------------------------------------------------------------------
pub const Node = opaque {
    pub usingnamespace Methods(@This());

    pub const State = enum(u32) {
        started,
        stopped,
    };

    pub const Flags = packed struct(u32) {
        passthrough: bool = false,
        continuous_processing: bool = false,
        allow_null_input: bool = false,
        different_processing_rates: bool = false,
        silent_output: bool = false,
        _padding: u27 = 0,
    };

    pub const VTable = extern struct {
        onProcess: ?*const fn (
            node: *Node,
            frames_in: ?*[*]const f32,
            frame_count_in: ?*u32,
            frames_out: *[*]f32,
            frame_count_out: *u32,
        ) callconv(.C) void,

        onGetRequiredInputFrameCount: ?*const fn (
            node: *Node,
            output_frame_count: u32,
            input_frame_count: *u32,
        ) callconv(.C) Result,

        input_bus_count: u8,
        output_bus_count: u8,
        flags: Flags,
    };

    pub const Config = extern struct {
        vtable: *VTable,
        initial_state: State,
        input_bus_count: u32,
        output_bus_count: u32,
        input_channels: [*]const u32,
        output_channels: [*]const u32,

        pub fn init() Config {
            var config: Config = undefined;
            zaudioNodeConfigInit(&config);
            return config;
        }
        extern fn zaudioNodeConfigInit(out_config: *Config) void;
    };

    fn Methods(comptime T: type) type {
        return struct {
            pub fn asNode(node: *const T) *const Node {
                return @ptrCast(*const Node, node);
            }
            pub fn asNodeMut(node: *T) *Node {
                return @ptrCast(*Node, node);
            }

            pub fn getNodeGraph(node: *const T) *const NodeGraph {
                return ma_node_get_node_graph(@intToPtr(*Node, @ptrToInt(node.asNode())));
            }
            pub fn getNodeGraphMut(node: *T) *NodeGraph {
                return ma_node_get_node_graph(node.asNodeMut());
            }
            extern fn ma_node_get_node_graph(node: *Node) *NodeGraph;

            pub fn getInputBusCount(node: *const T) u32 {
                return ma_node_get_input_bus_count(node.asNode());
            }
            extern fn ma_node_get_input_bus_count(node: *const Node) u32;

            pub fn getOutputBusCount(node: *const T) u32 {
                return ma_node_get_output_bus_count(node.asNode());
            }
            extern fn ma_node_get_output_bus_count(node: *const Node) u32;

            pub fn getInputChannels(node: *const T, bus_index: u32) u32 {
                return ma_node_get_input_channels(node.asNode(), bus_index);
            }
            extern fn ma_node_get_input_channels(node: *const Node, bus_index: u32) u32;

            pub fn getOutputChannels(node: *const T, bus_index: u32) u32 {
                return ma_node_get_output_channels(node.asNode(), bus_index);
            }
            extern fn ma_node_get_output_channels(node: *const Node, bus_index: u32) u32;

            pub fn attachOutputBus(
                node: *T,
                output_bus_index: u32,
                other_node: *Node,
                other_node_input_bus_index: u32,
            ) Error!void {
                try maybeError(ma_node_attach_output_bus(
                    node.asNodeMut(),
                    output_bus_index,
                    other_node.asNodeMut(),
                    other_node_input_bus_index,
                ));
            }
            extern fn ma_node_attach_output_bus(
                node: *Node,
                output_bus_index: u32,
                other_node: *Node,
                other_node_input_bus_index: u32,
            ) Result;

            pub fn dettachOutputBus(node: *T, output_bus_index: u32) Error!void {
                try maybeError(ma_node_detach_output_bus(node.asNodeMut(), output_bus_index));
            }
            extern fn ma_node_detach_output_bus(node: *Node, output_bus_index: u32) Result;

            pub fn dettachAllOutputBuses(node: *T) Error!void {
                try maybeError(ma_node_detach_all_output_buses(node.asNodeMut()));
            }
            extern fn ma_node_detach_all_output_buses(node: *Node) Result;

            pub fn setOutputBusVolume(node: *T, output_bus_index: u32, volume: f32) Error!void {
                try maybeError(ma_node_set_output_bus_volume(node.asNodeMut(), output_bus_index, volume));
            }
            extern fn ma_node_set_output_bus_volume(node: *Node, output_bus_index: u32, volume: f32) Result;

            pub fn getOutputBusVolume(node: *const T, output_bus_index: u32) f32 {
                return ma_node_get_output_bus_volume(node.asNode(), output_bus_index);
            }
            extern fn ma_node_get_output_bus_volume(node: *const Node, output_bus_index: u32) f32;

            pub fn setState(node: *T, state: State) Error!void {
                try maybeError(ma_node_set_state(node.asNodeMut(), state));
            }
            extern fn ma_node_set_state(node: *Node, state: State) Result;

            pub fn getState(node: *const T) State {
                return ma_node_get_state(node.asNode());
            }
            extern fn ma_node_get_state(node: *const Node) State;

            pub fn setTime(node: *T, local_time: u64) Error!void {
                try maybeError(ma_node_set_time(node.asNodeMut(), local_time));
            }
            extern fn ma_node_set_time(node: *Node, local_time: u64) Result;

            pub fn getTime(node: *const T) u64 {
                return ma_node_get_time(node.asNode());
            }
            extern fn ma_node_get_time(node: *const Node) u64;
        };
    }
};
//--------------------------------------------------------------------------------------------------
//
// DataSourceNode (-> Node)
//
//--------------------------------------------------------------------------------------------------
pub const DataSourceNode = opaque {
    pub usingnamespace Node.Methods(@This());

    pub const destroy = zaudioDataSourceNodeDestroy;
    extern fn zaudioDataSourceNodeDestroy(handle: *DataSourceNode) void;

    pub fn setLooping(handle: *DataSourceNode, is_looping: bool) void {
        try maybeError(ma_data_source_node_set_looping(handle, @boolToInt(is_looping)));
    }
    extern fn ma_data_source_node_set_looping(handle: *DataSourceNode, is_looping: Bool32) Result;

    pub fn isLooping(handle: *const DataSourceNode) bool {
        return ma_data_source_node_is_looping(handle) != .false32;
    }
    extern fn ma_data_source_node_is_looping(handle: *const DataSourceNode) Bool32;

    pub const Config = extern struct {
        node_config: Node.Config,
        data_source: *DataSource,

        pub fn init(ds: *DataSource) Config {
            var config: Config = undefined;
            zaudioDataSourceNodeConfigInit(ds, &config);
            return config;
        }
        extern fn zaudioDataSourceNodeConfigInit(ds: *DataSource, out_config: *Config) void;
    };
};
//--------------------------------------------------------------------------------------------------
//
// SplitterNode (-> Node)
//
//--------------------------------------------------------------------------------------------------
pub const SplitterNode = opaque {
    pub usingnamespace Node.Methods(@This());

    pub const destroy = zaudioSplitterNodeDestroy;
    extern fn zaudioSplitterNodeDestroy(handle: *SplitterNode) void;

    pub const Config = extern struct {
        node_config: Node.Config,
        channels: u32,
        output_bus_count: u32,

        pub fn init(channels: u32) Config {
            var config: Config = undefined;
            zaudioSplitterNodeConfigInit(channels, &config);
            return config;
        }
        extern fn zaudioSplitterNodeConfigInit(channels: u32, out_config: *Config) void;
    };
};
//--------------------------------------------------------------------------------------------------
//
// BiquadNode (-> Node) - Biquad Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const BiquadConfig = extern struct {
    format: Format,
    channels: u32,
    b0: f64,
    b1: f64,
    b2: f64,
    a0: f64,
    a1: f64,
    a2: f64,
};

pub const BiquadNode = opaque {
    pub usingnamespace Node.Methods(@This());

    pub const destroy = zaudioBiquadNodeDestroy;
    extern fn zaudioBiquadNodeDestroy(handle: *BiquadNode) void;

    pub fn reconfigure(handle: *BiquadNode, config: BiquadConfig) Error!void {
        try maybeError(ma_biquad_node_reinit(&config, handle));
    }
    extern fn ma_biquad_node_reinit(config: *const BiquadConfig, handle: *BiquadNode) Result;

    pub const Config = extern struct {
        node_config: Node.Config,
        biquad: BiquadConfig,

        pub fn init(channels: u32, b0: f32, b1: f32, b2: f32, a0: f32, a1: f32, a2: f32) Config {
            var config: Config = undefined;
            zaudioBiquadNodeConfigInit(channels, b0, b1, b2, a0, a1, a2, &config);
            return config;
        }
    };
    extern fn zaudioBiquadNodeConfigInit(
        channels: u32,
        b0: f32,
        b1: f32,
        b2: f32,
        a0: f32,
        a1: f32,
        a2: f32,
        out_config: *Config,
    ) void;
};
//--------------------------------------------------------------------------------------------------
//
// LpfNode (-> Node) - Low-Pass Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const LpfConfig = extern struct {
    format: Format,
    channels: u32,
    sampleRate: u32,
    cutoff_frequency: f64,
    order: u32,
};

pub const LpfNode = opaque {
    pub usingnamespace Node.Methods(@This());

    pub const destroy = zaudioLpfNodeDestroy;
    extern fn zaudioLpfNodeDestroy(handle: *LpfNode) void;

    pub fn reconfigure(handle: *LpfNode, config: LpfConfig) Error!void {
        try maybeError(ma_lpf_node_reinit(&config, handle));
    }
    extern fn ma_lpf_node_reinit(config: *const LpfConfig, handle: *LpfNode) Result;

    pub const Config = extern struct {
        node_config: Node.Config,
        lpf: LpfConfig,

        pub fn init(channels: u32, sample_rate: u32, cutoff_frequency: f64, order: u32) Config {
            var config: Config = undefined;
            zaudioLpfNodeConfigInit(channels, sample_rate, cutoff_frequency, order, &config);
            return config;
        }
        extern fn zaudioLpfNodeConfigInit(
            channels: u32,
            sample_rate: u32,
            cutoff_frequency: f64,
            order: u32,
            out_config: *Config,
        ) void;
    };
};
//--------------------------------------------------------------------------------------------------
//
// HpfNode (-> Node) - High-Pass Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const HpfConfig = extern struct {
    format: Format,
    channels: u32,
    sample_rate: u32,
    cutoff_frequency: f64,
    order: u32,
};

pub const HpfNode = opaque {
    pub usingnamespace Node.Methods(@This());

    pub const destroy = zaudioHpfNodeDestroy;
    extern fn zaudioHpfNodeDestroy(handle: *HpfNode) void;

    pub fn reconfigure(handle: *HpfNode, config: HpfConfig) Error!void {
        try maybeError(ma_hpf_node_reinit(&config, handle));
    }
    extern fn ma_hpf_node_reinit(config: *const HpfConfig, handle: *HpfNode) Result;

    pub const Config = extern struct {
        node_config: Node.Config,
        hpf: HpfConfig,

        pub fn init(channels: u32, sample_rate: u32, cutoff_frequency: f64, order: u32) Config {
            var config: Config = undefined;
            zaudioHpfNodeConfigInit(channels, sample_rate, cutoff_frequency, order, &config);
            return config;
        }
        extern fn zaudioHpfNodeConfigInit(
            channels: u32,
            sample_rate: u32,
            cutoff_frequency: f64,
            order: u32,
            out_config: *Config,
        ) void;
    };
};
//--------------------------------------------------------------------------------------------------
//
// NotchNode (-> Node) - Notch Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const NotchConfig = extern struct {
    format: Format,
    channels: u32,
    sample_rate: u32,
    q: f64,
    frequency: f64,
};

pub const NotchNode = opaque {
    pub usingnamespace Node.Methods(@This());

    pub const destroy = zaudioNotchNodeDestroy;
    extern fn zaudioNotchNodeDestroy(handle: *NotchNode) void;

    pub fn reconfigure(handle: *NotchNode, config: NotchConfig) Error!void {
        try maybeError(ma_notch_node_reinit(&config, handle));
    }
    extern fn ma_notch_node_reinit(config: *const NotchConfig, handle: *NotchNode) Result;

    pub const Config = extern struct {
        node_config: Node.Config,
        notch: NotchConfig,

        pub fn init(channels: u32, sample_rate: u32, q: f64, frequency: f64) Config {
            var config: Config = undefined;
            zaudioNotchNodeConfigInit(channels, sample_rate, q, frequency, &config);
            return config;
        }
        extern fn zaudioNotchNodeConfigInit(
            channels: u32,
            sample_rate: u32,
            q: f64,
            frequency: f64,
            out_config: *Config,
        ) void;
    };
};
//--------------------------------------------------------------------------------------------------
//
// PeakNode (-> Node) - Peak Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const PeakConfig = extern struct {
    format: Format,
    channels: u32,
    sample_rate: u32,
    gain_db: f64,
    q: f64,
    frequency: f64,
};

pub const PeakNode = opaque {
    pub usingnamespace Node.Methods(@This());

    pub const destroy = zaudioPeakNodeDestroy;
    extern fn zaudioPeakNodeDestroy(handle: *PeakNode) void;

    pub fn reconfigure(handle: *PeakNode, config: PeakConfig) Error!void {
        try maybeError(ma_peak_node_reinit(&config, handle));
    }
    extern fn ma_peak_node_reinit(config: *const PeakConfig, handle: *PeakNode) Result;

    pub const Config = extern struct {
        node_config: Node.Config,
        peak: PeakConfig,

        pub fn init(channels: u32, sample_rate: u32, gain_db: f64, q: f64, frequency: f64) Config {
            var config: Config = undefined;
            zaudioPeakNodeConfigInit(channels, sample_rate, gain_db, q, frequency, &config);
            return config;
        }
        extern fn zaudioPeakNodeConfigInit(
            channels: u32,
            sample_rate: u32,
            gain_db: f64,
            q: f64,
            frequency: f64,
            out_config: *Config,
        ) void;
    };
};
//--------------------------------------------------------------------------------------------------
//
// LoshelfNode (-> Node) - Low Shelf Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const LoshelfConfig = extern struct {
    format: Format,
    channels: u32,
    sample_rate: u32,
    gain_db: f64,
    shelf_slope: f64,
    frequency: f64,
};

pub const LoshelfNode = opaque {
    pub usingnamespace Node.Methods(@This());

    pub const destroy = zaudioLoshelfNodeDestroy;
    extern fn zaudioLoshelfNodeDestroy(handle: *LoshelfNode) void;

    pub fn reconfigure(handle: *LoshelfNode, config: LoshelfConfig) Error!void {
        try maybeError(ma_loshelf_node_reinit(&config, handle));
    }
    extern fn ma_loshelf_node_reinit(config: *const LoshelfConfig, handle: *LoshelfNode) Result;

    pub const Config = extern struct {
        node_config: Node.Config,
        loshelf: LoshelfConfig,

        pub fn init(
            channels: u32,
            sample_rate: u32,
            gain_db: f64,
            shelf_slope: f64,
            frequency: f64,
        ) Config {
            var config: Config = undefined;
            zaudioLoshelfNodeConfigInit(channels, sample_rate, gain_db, shelf_slope, frequency, &config);
            return config;
        }
        extern fn zaudioLoshelfNodeConfigInit(
            channels: u32,
            sample_rate: u32,
            gain_db: f64,
            shelf_slope: f64,
            frequency: f64,
            out_config: *Config,
        ) void;
    };
};
//--------------------------------------------------------------------------------------------------
//
// HishelfNode (-> Node) - High Shelf Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const HishelfConfig = extern struct {
    format: Format,
    channels: u32,
    sample_rate: u32,
    gain_db: f64,
    shelf_slope: f64,
    frequency: f64,
};

pub const HishelfNode = opaque {
    pub usingnamespace Node.Methods(@This());

    pub const destroy = zaudioHishelfNodeDestroy;
    extern fn zaudioHishelfNodeDestroy(handle: *HishelfNode) void;

    pub fn reconfigure(handle: *HishelfNode, config: HishelfConfig) Error!void {
        try maybeError(ma_hishelf_node_reinit(&config, handle));
    }
    extern fn ma_hishelf_node_reinit(config: *const HishelfConfig, handle: *HishelfNode) Result;

    pub const Config = extern struct {
        node_config: Node.Config,
        hishelf: HishelfConfig,

        pub fn init(
            channels: u32,
            sample_rate: u32,
            gain_db: f64,
            shelf_slope: f64,
            frequency: f64,
        ) Config {
            var config: Config = undefined;
            zaudioHishelfNodeConfigInit(channels, sample_rate, gain_db, shelf_slope, frequency, &config);
            return config;
        }
        extern fn zaudioHishelfNodeConfigInit(
            channels: u32,
            sample_rate: u32,
            gain_db: f64,
            shelf_slope: f64,
            frequency: f64,
            out_config: *Config,
        ) void;
    };
};
//--------------------------------------------------------------------------------------------------
//
// DelayFilterNode (-> Node)
//
//--------------------------------------------------------------------------------------------------
pub const DelayConfig = extern struct {
    channels: u32,
    sample_rate: u32,
    delay_in_frames: u32,
    delay_start: Bool32,
    wet: f32,
    dry: f32,
    decay: f32,
};

pub const DelayNode = opaque {
    pub usingnamespace Node.Methods(@This());

    pub const destroy = zaudioDelayNodeDestroy;
    extern fn zaudioDelayNodeDestroy(handle: *DelayNode) void;

    pub const setWet = ma_delay_node_set_wet;
    extern fn ma_delay_node_set_wet(handle: *DelayNode, value: f32) void;

    pub const getWet = ma_delay_node_get_wet;
    extern fn ma_delay_node_get_wet(handle: *const DelayNode) f32;

    pub const setDry = ma_delay_node_set_dry;
    extern fn ma_delay_node_set_dry(handle: *DelayNode, value: f32) void;

    pub const getDry = ma_delay_node_get_dry;
    extern fn ma_delay_node_get_dry(handle: *const DelayNode) f32;

    pub const setDecay = ma_delay_node_set_decay;
    extern fn ma_delay_node_set_decay(handle: *DelayNode, value: f32) void;

    pub const getDecay = ma_delay_node_get_decay;
    extern fn ma_delay_node_get_decay(handle: *const DelayNode) f32;

    pub const Config = extern struct {
        node_config: Node.Config,
        delay: DelayConfig,

        pub fn init(channels: u32, sample_rate: u32, delay_in_frames: u32, decay: f32) Config {
            var config: Config = undefined;
            zaudioDelayNodeConfigInit(channels, sample_rate, delay_in_frames, decay, &config);
            return config;
        }
        extern fn zaudioDelayNodeConfigInit(
            channels: u32,
            sample_rate: u32,
            delay_in_frames: u32,
            decay: f32,
            out_config: *Config,
        ) void;
    };
};
//--------------------------------------------------------------------------------------------------
//
// NodeGraph (-> Node)
//
//--------------------------------------------------------------------------------------------------
pub const NodeGraph = opaque {
    pub usingnamespace Methods(@This());

    pub const Config = extern struct {
        channels: u32,
        node_cache_cap_in_frames: u16,

        pub fn init(channels: u32) Config {
            var config: Config = undefined;
            zaudioNodeGraphConfigInit(channels, &config);
            return config;
        }
        extern fn zaudioNodeGraphConfigInit(channels: u32, out_config: *Config) void;
    };

    pub fn create(config: Config) Error!*NodeGraph {
        var handle: ?*NodeGraph = null;
        try maybeError(zaudioNodeGraphCreate(&config, &handle));
        return handle.?;
    }
    extern fn zaudioNodeGraphCreate(config: *const Config, out_handle: ?*?*NodeGraph) Result;

    pub const destroy = zaudioNodeGraphDestroy;
    extern fn zaudioNodeGraphDestroy(handle: *NodeGraph) void;

    fn Methods(comptime T: type) type {
        return struct {
            pub usingnamespace Node.Methods(T);

            pub fn asNodeGraph(handle: *const T) *const NodeGraph {
                return @ptrCast(*const NodeGraph, handle);
            }
            pub fn asNodeGraphMut(handle: *T) *NodeGraph {
                return @ptrCast(*NodeGraph, handle);
            }

            pub fn createDataSourceNode(node_graph: *T, config: DataSourceNode.Config) Error!*DataSourceNode {
                var handle: ?*DataSourceNode = null;
                try maybeError(zaudioDataSourceNodeCreate(node_graph.asNodeGraphMut(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioDataSourceNodeCreate(
                node_graph: *NodeGraph,
                config: *const DataSourceNode.Config,
                out_handle: ?*?*DataSourceNode,
            ) Result;

            pub fn createBiquadNode(node_graph: *T, config: BiquadNode.NodeConfig) Error!*BiquadNode {
                var handle: ?*BiquadNode = null;
                try maybeError(zaudioBiquadNodeCreate(node_graph.asNodeGraphMut(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioBiquadNodeCreate(
                node_graph: *NodeGraph,
                config: *const BiquadNode.NodeConfig,
                out_handle: ?*?*BiquadNode,
            ) Result;

            pub fn createLpfNode(node_graph: *T, config: LpfNode.Config) Error!*LpfNode {
                var handle: ?*LpfNode = null;
                try maybeError(zaudioLpfNodeCreate(node_graph.asNodeGraphMut(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioLpfNodeCreate(
                node_graph: *NodeGraph,
                config: *const LpfNode.Config,
                out_handle: ?*?*LpfNode,
            ) Result;

            pub fn createHpfNode(node_graph: *T, config: HpfNode.Config) Error!*HpfNode {
                var handle: ?*HpfNode = null;
                try maybeError(zaudioHpfNodeCreate(node_graph.asNodeGraphMut(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioHpfNodeCreate(
                node_graph: *NodeGraph,
                config: *const HpfNode.Config,
                out_handle: ?*?*HpfNode,
            ) Result;

            pub fn createSplitterNode(node_graph: *T, config: SplitterNode.Config) Error!*SplitterNode {
                var handle: ?*SplitterNode = null;
                try maybeError(zaudioSplitterNodeCreate(node_graph.asNodeGraphMut(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioSplitterNodeCreate(
                node_graph: *NodeGraph,
                config: *const SplitterNode.Config,
                out_handle: ?*?*SplitterNode,
            ) Result;

            pub fn createNotchNode(node_graph: *T, config: NotchNode.Config) Error!*NotchNode {
                var handle: ?*NotchNode = null;
                try maybeError(zaudioNotchNodeCreate(node_graph.asNodeGraphMut(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioNotchNodeCreate(
                node_graph: *NodeGraph,
                config: *const NotchNode.Config,
                out_handle: ?*?*NotchNode,
            ) Result;

            pub fn createPeakNode(node_graph: *T, config: PeakNode.Config) Error!*PeakNode {
                var handle: ?*PeakNode = null;
                try maybeError(zaudioPeakNodeCreate(node_graph.asNodeGraphMut(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioPeakNodeCreate(
                node_graph: *NodeGraph,
                config: *const PeakNode.Config,
                out_handle: ?*?*PeakNode,
            ) Result;

            pub fn createLoshelfNode(node_graph: *T, config: LoshelfNode.Config) Error!*LoshelfNode {
                var handle: ?*LoshelfNode = null;
                try maybeError(zaudioLoshelfNodeCreate(node_graph.asNodeGraphMut(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioLoshelfNodeCreate(
                node_graph: *NodeGraph,
                config: *const LoshelfNode.Config,
                out_handle: ?*?*LoshelfNode,
            ) Result;

            pub fn createHishelfNode(node_graph: *T, config: HishelfNode.Config) Error!*HishelfNode {
                var handle: ?*HishelfNode = null;
                try maybeError(zaudioHishelfNodeCreate(node_graph.asNodeGraphMut(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioHishelfNodeCreate(
                node_graph: *NodeGraph,
                config: *const HishelfNode.Config,
                out_handle: ?*?*HishelfNode,
            ) Result;

            pub fn createDelayNode(node_graph: *T, config: DelayNode.Config) Error!*DelayNode {
                var handle: ?*DelayNode = null;
                try maybeError(zaudioDelayNodeCreate(node_graph.asNodeGraphMut(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioDelayNodeCreate(
                node_graph: *NodeGraph,
                config: *const DelayNode.Config,
                out_handle: ?*?*DelayNode,
            ) Result;

            pub fn getEndpoint(handle: *const T) *const Node {
                return ma_node_graph_get_endpoint(@intToPtr(*NodeGraph, @ptrToInt(handle.asNodeGraph())));
            }
            pub fn getEndpointMut(handle: *T) *Node {
                return ma_node_graph_get_endpoint(handle.asNodeGraphMut());
            }
            extern fn ma_node_graph_get_endpoint(handle: *NodeGraph) *Node;

            pub fn getChannels(handle: *const T) u32 {
                return ma_node_graph_get_channels(handle.asNodeGraph());
            }
            extern fn ma_node_graph_get_channels(handle: *const NodeGraph) u32;

            pub fn readPcmFrames(
                node_graph: *T,
                frames_out: *anyopaque,
                frame_count: u64,
                frames_read: ?*u64,
            ) Error!void {
                try maybeError(ma_node_graph_read_pcm_frames(
                    node_graph.asNodeGraphMut(),
                    frames_out,
                    frame_count,
                    frames_read,
                ));
            }
            extern fn ma_node_graph_read_pcm_frames(
                handle: *NodeGraph,
                frames_out: *anyopaque,
                frame_count: u64,
                frames_read: ?*u64,
            ) Result;
        };
    }
};
//--------------------------------------------------------------------------------------------------
//
// Device
//
//--------------------------------------------------------------------------------------------------
pub const Device = opaque {
    pub fn create(context: ?*Context, config: Config) Error!*Device {
        var handle: ?*Device = null;
        try maybeError(zaudioDeviceCreate(context, &config, &handle));
        return handle.?;
    }
    extern fn zaudioDeviceCreate(
        context: ?*Context,
        config: *const Config,
        out_handle: ?*?*Device,
    ) Result;

    pub const destroy = zaudioDeviceDestroy;
    extern fn zaudioDeviceDestroy(device: *Device) void;

    pub const getUserData = zaudioDeviceGetUserData;
    extern fn zaudioDeviceGetUserData(device: *const Device) ?*anyopaque;

    pub fn getContext(device: *const Device) *const Context {
        return ma_device_get_context(@intToPtr(*Device, @ptrToInt(device)));
    }
    pub fn getContextMut(device: *Device) *Context {
        return ma_device_get_context(device);
    }
    extern fn ma_device_get_context(device: *Device) *Context;

    pub fn getLog(device: *const Device) ?*const Log {
        return ma_device_get_log(@intToPtr(*Device, @ptrToInt(device)));
    }
    pub fn getLogMut(device: *Device) ?*Log {
        return ma_device_get_log(device);
    }
    extern fn ma_device_get_log(device: *Device) ?*Log;

    pub fn start(device: *Device) Error!void {
        try maybeError(ma_device_start(device));
    }
    extern fn ma_device_start(device: *Device) Result;

    pub fn stop(device: *Device) Error!void {
        try maybeError(ma_device_stop(device));
    }
    extern fn ma_device_stop(device: *Device) Result;

    pub fn isStarted(device: *const Device) bool {
        return ma_device_is_started(device) != .false32;
    }
    extern fn ma_device_is_started(device: *const Device) Bool32;

    pub const getState = ma_device_get_state;
    extern fn ma_device_get_state(device: *const Device) State;

    pub fn setMasterVolume(device: *Device, volume: f32) Error!void {
        try maybeError(ma_device_set_master_volume(device, volume));
    }
    extern fn ma_device_set_master_volume(device: *Device, volume: f32) Result;

    pub fn getMasterVolume(device: *const Device) Error!f32 {
        var volume: f32 = 0.0;
        try maybeError(ma_device_get_master_volume(device, &volume));
        return volume;
    }
    extern fn ma_device_get_master_volume(device: *const Device, volume: *f32) Result;

    pub const Type = enum(u32) {
        playback = 1,
        capture = 2,
        duplex = 3,
        loopback = 4,
    };

    pub const State = enum(u32) {
        uninitialized = 0,
        stopped = 1,
        started = 2,
        starting = 3,
        stopping = 4,
    };

    pub const Config = extern struct {
        device_type: Type,
        sample_rate: u32,
        period_size_in_frames: u32,
        period_size_in_milliseconds: u32,
        periods: u32,
        performance_profile: PerformanceProfile,
        no_pre_silenced_output_buffer: bool,
        no_clip: bool,
        no_disable_denormals: bool,
        no_fixed_sized_callback: bool,
        data_callback: ?DataProc,
        notification_callback: ?NotificationProc,
        stop_callback: ?StopProc,
        user_data: ?*anyopaque,
        resampling: extern struct {
            format: Format,
            channels: u32,
            sample_rate_in: u32,
            sample_rate_out: u32,
            algorithm: ResampleAlgorithm,
            backend_vtable: ?*anyopaque, // TODO: Should be `*ma_resampling_backend_vtable` (custom resamplers).
            backend_user_data: ?*anyopaque,
            linear: extern struct {
                lpf_order: u32,
            },
        },
        playback: extern struct {
            device_id: ?*const Id,
            format: Format,
            channels: u32,
            channel_map: [*]Channel,
            channel_mix_mode: ChannelMixMode,
            calculate_lfe_from_spatial_channels: Bool32,
            share_mode: ShareMode,
        },
        capture: extern struct {
            device_id: ?*const Id,
            format: Format,
            channels: u32,
            channel_map: [*]Channel,
            channel_mix_mode: ChannelMixMode,
            calculate_lfe_from_spatial_channels: Bool32,
            share_mode: ShareMode,
        },
        wasapi: extern struct {
            usage: WasapiUsage,
            no_auto_convert_src: bool,
            no_default_quality_src: bool,
            no_auto_stream_routing: bool,
            no_hardware_offloading: bool,
            loopback_process_id: u32,
            loopback_process_exclude: bool,
        },
        alsa: extern struct {
            no_mmap: Bool32,
            no_auto_format: Bool32,
            no_auto_channels: Bool32,
            no_auto_resample: Bool32,
        },
        pulse: extern struct {
            stream_name_playback: [*:0]const u8,
            stream_name_capture: [*:0]const u8,
        },
        coreaudio: extern struct {
            allow_nominal_sample_rate_change: Bool32,
        },
        opensl: extern struct {
            stream_type: OpenslStreamType,
            recording_preset: OpenslRecordingPreset,
        },
        aaudio: extern struct {
            usage: AaudioUsage,
            content_type: AaudioContentType,
            input_preset: AaudioInputPreset,
            no_auto_start_after_reroute: Bool32,
        },

        pub fn init(device_type: Type) Config {
            var config: Config = undefined;
            zaudioDeviceConfigInit(device_type, &config);
            return config;
        }
        extern fn zaudioDeviceConfigInit(device_type: Type, out_config: *Config) void;
    };

    pub const DataProc = *const fn (
        device: *Device,
        output: ?*anyopaque,
        input: ?*const anyopaque,
        frame_count: u32,
    ) callconv(.C) void;

    pub const NotificationProc = *const fn (
        *const anyopaque, // TODO: Should be `*const ma_device_notification`.
    ) callconv(.C) void;

    pub const StopProc = *const fn (device: *Device) callconv(.C) void;

    pub const Id = extern union {
        wasapi: [64]i32,
        dsound: [16]u8,
        winmm: u32,
        alsa: [256]u8,
        pulse: [256]u8,
        jack: i32,
        coreaudio: [256]u8,
        sndio: [256]u8,
        audio4: [256]u8,
        oss: [64]u8,
        aaudio: i32,
        opensl: u32,
        webaudio: [32]u8,
        custom: extern union {
            i: i32,
            s: [256]u8,
            p: ?*anyopaque,
        },
        nullbackend: i32,
    };
};
//--------------------------------------------------------------------------------------------------
//
// Engine (-> NodeGraph -> Node)
//
//--------------------------------------------------------------------------------------------------
pub const Engine = opaque {
    pub usingnamespace NodeGraph.Methods(@This());

    pub const Config = extern struct {
        resource_manager: ?*ResourceManager,
        context: ?*Context,
        device: ?*Device,
        playback_device_id: ?*Device.Id,
        notification_callback: ?Device.NotificationProc,
        log: ?*Log,
        listener_count: u32,
        channels: u32,
        sample_rate: u32,
        period_size_in_frames: u32,
        period_size_in_milliseconds: u32,
        gain_smooth_time_in_frames: u32,
        gain_smooth_time_in_milliseconds: u32,
        allocation_callbacks: AllocationCallbacks,
        no_auto_start: Bool32,
        no_device: Bool32,
        mono_expansion_mode: MonoExpansionMode,
        resource_manager_vfs: ?*Vfs,

        pub fn init() Config {
            var config: Config = undefined;
            zaudioEngineConfigInit(&config);
            return config;
        }
        extern fn zaudioEngineConfigInit(out_config: *Config) void;
    };

    pub fn create(config: ?Config) Error!*Engine {
        var handle: ?*Engine = null;
        try maybeError(zaudioEngineCreate(if (config) |conf| &conf else null, &handle));
        return handle.?;
    }
    extern fn zaudioEngineCreate(config: ?*const Config, out_handle: ?*?*Engine) Result;

    pub const destroy = zaudioEngineDestroy;
    extern fn zaudioEngineDestroy(handle: *Engine) void;

    pub fn createSoundFromFile(
        engine: *Engine,
        file_path: [:0]const u8,
        args: struct {
            flags: Sound.Flags = .{},
            sgroup: ?*SoundGroup = null,
            done_fence: ?*Fence = null,
        },
    ) Error!*Sound {
        return Sound.createFromFile(engine, file_path, args.flags, args.sgroup, args.done_fence);
    }

    pub fn createSoundFromDataSource(
        engine: *Engine,
        data_source: *DataSource,
        flags: Sound.Flags,
        sgroup: ?*SoundGroup,
    ) Error!*Sound {
        return Sound.createFromDataSource(engine, data_source, flags, sgroup);
    }

    pub fn createSound(engine: *Engine, config: Sound.Config) Error!*Sound {
        return Sound.create(engine, config);
    }

    pub fn createSoundCopy(
        engine: *Engine,
        existing_sound: *Sound,
        flags: Sound.Flags,
        sgroup: ?*SoundGroup,
    ) Error!*Sound {
        return Sound.createCopy(engine, existing_sound, flags, sgroup);
    }

    pub fn createSoundGroup(engine: *Engine, flags: Sound.Flags, parent: ?*SoundGroup) Error!*SoundGroup {
        return SoundGroup.create(engine, flags, parent);
    }

    pub fn getResourceManager(engine: *const Engine) *const ResourceManager {
        return ma_engine_get_resource_manager(@intToPtr(*Engine, @ptrToInt(engine)));
    }
    pub fn getResourceManagerMut(engine: *Engine) *ResourceManager {
        return ma_engine_get_resource_manager(engine);
    }
    extern fn ma_engine_get_resource_manager(engine: *Engine) *ResourceManager;

    pub fn getDevice(engine: *const Engine) ?*const Device {
        return ma_engine_get_device(@intToPtr(*Engine, @ptrToInt(engine)));
    }
    pub fn getDeviceMut(engine: *Engine) ?*Device {
        return ma_engine_get_device(engine);
    }
    extern fn ma_engine_get_device(engine: *Engine) ?*Device;

    pub fn getLog(engine: *const Engine) ?*const Log {
        return ma_engine_get_log(@intToPtr(*Engine, @ptrToInt(engine)));
    }
    pub fn getLogMut(engine: *Engine) ?*Log {
        return ma_engine_get_log(engine);
    }
    extern fn ma_engine_get_log(engine: *Engine) ?*Log;

    pub const getSampleRate = ma_engine_get_sample_rate;
    extern fn ma_engine_get_sample_rate(engine: *const Engine) u32;

    pub fn start(engine: *Engine) Error!void {
        try maybeError(ma_engine_start(engine));
    }
    extern fn ma_engine_start(engine: *Engine) Result;

    pub fn stop(engine: *Engine) Error!void {
        try maybeError(ma_engine_stop(engine));
    }
    extern fn ma_engine_stop(engine: *Engine) Result;

    pub fn setVolume(engine: *Engine, volume: f32) Error!void {
        try maybeError(ma_engine_set_volume(engine, volume));
    }
    extern fn ma_engine_set_volume(engine: *Engine, volume: f32) Result;

    pub fn setGainDb(engine: *Engine, gain_db: f32) Error!void {
        try maybeError(ma_engine_set_gain_db(engine, gain_db));
    }
    extern fn ma_engine_set_gain_db(engine: *Engine, gain_db: f32) Result;

    pub const getListenerCount = ma_engine_get_listener_count;
    extern fn ma_engine_get_listener_count(engine: *const Engine) u32;

    pub fn findClosestListener(engine: *const Engine, absolute_pos_xyz: [3]f32) u32 {
        return ma_engine_find_closest_listener(
            engine,
            absolute_pos_xyz[0],
            absolute_pos_xyz[1],
            absolute_pos_xyz[2],
        );
    }
    extern fn ma_engine_find_closest_listener(engine: *const Engine, x: f32, y: f32, z: f32) u32;

    pub fn setListenerPosition(engine: *Engine, index: u32, v: [3]f32) void {
        ma_engine_listener_set_position(engine, index, v[0], v[1], v[2]);
    }
    extern fn ma_engine_listener_set_position(engine: *Engine, index: u32, x: f32, y: f32, z: f32) void;

    pub fn getListenerPosition(engine: *const Engine, index: u32) [3]f32 {
        var v: [3]f32 = undefined;
        WA_ma_engine_listener_get_position(engine, index, &v);
        return v;
    }
    extern fn WA_ma_engine_listener_get_position(engine: *const Engine, index: u32, vout: *[3]f32) void;

    pub fn setListenerDirection(engine: *Engine, index: u32, v: [3]f32) void {
        ma_engine_listener_set_direction(engine, index, v[0], v[1], v[2]);
    }
    extern fn ma_engine_listener_set_direction(engine: *Engine, index: u32, x: f32, y: f32, z: f32) void;

    pub fn getListenerDirection(engine: *const Engine, index: u32) [3]f32 {
        var v: [3]f32 = undefined;
        WA_ma_engine_listener_get_direction(engine, index, &v);
        return v;
    }
    extern fn WA_ma_engine_listener_get_direction(engine: *const Engine, index: u32, vout: *[3]f32) void;

    pub fn setListenerVelocity(engine: *Engine, index: u32, v: [3]f32) void {
        ma_engine_listener_set_velocity(engine, index, v[0], v[1], v[2]);
    }
    extern fn ma_engine_listener_set_velocity(engine: *Engine, index: u32, x: f32, y: f32, z: f32) void;

    pub fn getListenerVelocity(engine: *const Engine, index: u32) [3]f32 {
        var v: [3]f32 = undefined;
        WA_ma_engine_listener_get_velocity(engine, index, &v);
        return v;
    }
    extern fn WA_ma_engine_listener_get_velocity(engine: *const Engine, index: u32, vout: *[3]f32) void;

    pub fn setListenerWorldUp(engine: *Engine, index: u32, v: [3]f32) void {
        ma_engine_listener_set_world_up(engine, index, v[0], v[1], v[2]);
    }
    extern fn ma_engine_listener_set_world_up(engine: *Engine, index: u32, x: f32, y: f32, z: f32) void;

    pub fn getListenerWorldUp(engine: *const Engine, index: u32) [3]f32 {
        var v: [3]f32 = undefined;
        WA_ma_engine_listener_get_world_up(engine, index, &v);
        return v;
    }
    extern fn WA_ma_engine_listener_get_world_up(engine: *const Engine, index: u32, vout: *[3]f32) void;

    pub fn setListenerEnabled(engine: *Engine, index: u32, enabled: bool) void {
        ma_engine_listener_set_enabled(engine, index, if (enabled) .true32 else .false32);
    }
    extern fn ma_engine_listener_set_enabled(engine: *Engine, index: u32, is_enabled: Bool32) void;

    pub fn isListenerEnabled(engine: *const Engine, index: u32) bool {
        return ma_engine_listener_is_enabled(engine, index) != .false32;
    }
    extern fn ma_engine_listener_is_enabled(engine: *const Engine, index: u32) Bool32;

    pub const setListenerCone = ma_engine_listener_set_cone;
    extern fn ma_engine_listener_set_cone(
        engine: *Engine,
        index: u32,
        inner_radians: f32,
        outer_radians: f32,
        outer_gain: f32,
    ) void;

    pub const getListenerCone = ma_engine_listener_get_cone;
    extern fn ma_engine_listener_get_cone(
        engine: *const Engine,
        index: u32,
        inner_radians: ?*f32,
        outer_radians: ?*f32,
        outer_gain: ?*f32,
    ) void;

    pub fn playSound(engine: *Engine, filepath: [:0]const u8, sgroup: ?*SoundGroup) Error!void {
        try maybeError(ma_engine_play_sound(engine, filepath.ptr, sgroup));
    }
    extern fn ma_engine_play_sound(engine: *Engine, filepath: [*:0]const u8, sgroup: ?*SoundGroup) Result;

    pub fn playSoundEx(
        engine: *Engine,
        filepath: [:0]const u8,
        node: ?*Node,
        node_input_bus_index: u32,
    ) Error!void {
        try maybeError(ma_engine_play_sound_ex(engine, filepath.ptr, node, node_input_bus_index));
    }
    extern fn ma_engine_play_sound_ex(
        engine: *Engine,
        filepath: [*:0]const u8,
        node: ?*Node,
        node_input_bus_index: u32,
    ) Result;
};
//--------------------------------------------------------------------------------------------------
//
// Sound (-> Node)
//
//--------------------------------------------------------------------------------------------------
pub const Sound = opaque {
    pub usingnamespace Node.Methods(@This());

    fn createFromFile(
        engine: *Engine,
        file_path: [:0]const u8,
        flags: Flags,
        sgroup: ?*SoundGroup,
        done_fence: ?*Fence,
    ) Error!*Sound {
        var handle: ?*Sound = null;
        try maybeError(zaudioSoundCreateFromFile(engine, file_path.ptr, flags, sgroup, done_fence, &handle));
        return handle.?;
    }
    extern fn zaudioSoundCreateFromFile(
        engine: *Engine,
        file_path: [*:0]const u8,
        flags: Flags,
        sgroup: ?*SoundGroup,
        done_fence: ?*Fence,
        out_handle: ?*?*Sound,
    ) Result;

    fn createFromDataSource(
        engine: *Engine,
        data_source: *DataSource,
        flags: Flags,
        sgroup: ?*SoundGroup,
    ) Error!*Sound {
        var handle: ?*Sound = null;
        try maybeError(zaudioSoundCreateFromDataSource(engine, data_source, flags, sgroup, &handle));
        return handle.?;
    }
    extern fn zaudioSoundCreateFromDataSource(
        engine: *Engine,
        data_source: *DataSource,
        flags: Flags,
        sgroup: ?*SoundGroup,
        out_handle: ?*?*Sound,
    ) Result;

    fn createCopy(engine: *Engine, existing_sound: *Sound, flags: Flags, sgroup: ?*SoundGroup) Error!*Sound {
        var handle: ?*Sound = null;
        try maybeError(zaudioSoundCreateCopy(engine, existing_sound, flags, sgroup, &handle));
        return handle.?;
    }
    extern fn zaudioSoundCreateCopy(
        engine: *Engine,
        existing_sound: *Sound,
        flags: Flags,
        sgroup: ?*SoundGroup,
        out_handle: ?*?*Sound,
    ) Result;

    fn create(engine: *Engine, config: Sound.Config) Error!*Sound {
        var handle: ?*Sound = null;
        try maybeError(zaudioSoundCreate(engine, &config, &handle));
        return handle.?;
    }
    extern fn zaudioSoundCreate(engine: *Engine, config: *const Sound.Config, out_handle: ?*?*Sound) Result;

    pub const destroy = zaudioSoundDestroy;
    extern fn zaudioSoundDestroy(sound: *Sound) void;

    pub const getDataSource = ma_sound_get_data_source;
    extern fn ma_sound_get_data_source(sound: *const Sound) ?*DataSource;

    pub const getEngine = ma_sound_get_engine;
    extern fn ma_sound_get_engine(sound: *const Sound) *Engine;

    pub fn start(sound: *Sound) Error!void {
        try maybeError(ma_sound_start(sound));
    }
    extern fn ma_sound_start(sound: *Sound) Result;

    pub fn stop(sound: *Sound) Error!void {
        try maybeError(ma_sound_stop(sound));
    }
    extern fn ma_sound_stop(sound: *Sound) Result;

    pub const setVolume = ma_sound_set_volume;
    extern fn ma_sound_set_volume(sound: *Sound, volume: f32) void;

    pub const getVolume = ma_sound_get_volume;
    extern fn ma_sound_get_volume(sound: *const Sound) f32;

    pub const setPan = ma_sound_set_pan;
    extern fn ma_sound_set_pan(sound: *Sound, pan: f32) void;

    pub const getPan = ma_sound_get_pan;
    extern fn ma_sound_get_pan(sound: *const Sound) f32;

    pub const setPanMode = ma_sound_set_pan_mode;
    extern fn ma_sound_set_pan_mode(sound: *Sound, pan_mode: PanMode) void;

    pub const getPanMode = ma_sound_get_pan_mode;
    extern fn ma_sound_get_pan_mode(sound: *const Sound) PanMode;

    pub const setPitch = ma_sound_set_pitch;
    extern fn ma_sound_set_pitch(sound: *Sound, pitch: f32) void;

    pub const getPitch = ma_sound_get_pitch;
    extern fn ma_sound_get_pitch(sound: *const Sound) f32;

    pub fn setSpatializationEnabled(sound: *Sound, enabled: bool) void {
        ma_sound_set_spatialization_enabled(sound, @boolToInt(enabled));
    }
    extern fn ma_sound_set_spatialization_enabled(sound: *Sound, enabled: Bool32) void;

    pub fn isSpatializationEnabled(sound: *const Sound) bool {
        return ma_sound_is_spatialization_enabled(sound) != .false32;
    }
    extern fn ma_sound_is_spatialization_enabled(sound: *const Sound) Bool32;

    pub const setPinnedListenerIndex = ma_sound_set_pinned_listener_index;
    extern fn ma_sound_set_pinned_listener_index(sound: *Sound, index: u32) void;

    pub const getPinnedListenerIndex = ma_sound_get_pinned_listener_index;
    extern fn ma_sound_get_pinned_listener_index(sound: *const Sound) u32;

    pub const getListenerIndex = ma_sound_get_listener_index;
    extern fn ma_sound_get_listener_index(sound: *const Sound) u32;

    pub fn getDirectionToListener(sound: *const Sound) [3]f32 {
        var v: [3]f32 = undefined;
        WA_ma_sound_get_direction_to_listener(sound, &v);
        return v;
    }
    extern fn WA_ma_sound_get_direction_to_listener(sound: *const Sound, vout: *[3]f32) void;

    pub fn setPosition(sound: *Sound, v: [3]f32) void {
        ma_sound_set_position(sound, v[0], v[1], v[2]);
    }
    extern fn ma_sound_set_position(sound: *Sound, x: f32, y: f32, z: f32) void;

    pub fn getPosition(sound: *const Sound) [3]f32 {
        var v: [3]f32 = undefined;
        WA_ma_sound_get_position(sound, &v);
        return v;
    }
    extern fn WA_ma_sound_get_position(sound: *const Sound, vout: *[3]f32) void;

    pub fn setDirection(sound: *Sound, v: [3]f32) void {
        ma_sound_set_direction(sound, v[0], v[1], v[2]);
    }
    extern fn ma_sound_set_direction(sound: *Sound, x: f32, y: f32, z: f32) void;

    pub fn getDirection(sound: *const Sound) [3]f32 {
        var v: [3]f32 = undefined;
        WA_ma_sound_get_direction(sound, &v);
        return v;
    }
    extern fn WA_ma_sound_get_direction(sound: *const Sound, vout: *[3]f32) void;

    pub fn setVelocity(sound: *Sound, v: [3]f32) void {
        ma_sound_set_velocity(sound, v[0], v[1], v[2]);
    }
    extern fn ma_sound_set_velocity(sound: *Sound, x: f32, y: f32, z: f32) void;

    pub fn getVelocity(sound: *const Sound) [3]f32 {
        var v: [3]f32 = undefined;
        WA_ma_sound_get_velocity(sound, &v);
        return v;
    }
    extern fn WA_ma_sound_get_velocity(sound: *const Sound, vout: *[3]f32) void;

    pub const setAttenuationModel = ma_sound_set_attenuation_model;
    extern fn ma_sound_set_attenuation_model(sound: *Sound, model: AttenuationModel) void;

    pub const getAttenuationModel = ma_sound_get_attenuation_model;
    extern fn ma_sound_get_attenuation_model(sound: *const Sound) AttenuationModel;

    pub const setPositioning = ma_sound_set_positioning;
    extern fn ma_sound_set_positioning(sound: *Sound, pos: Positioning) void;

    pub const getPositioning = ma_sound_get_positioning;
    extern fn ma_sound_get_positioning(sound: *const Sound) Positioning;

    pub const setRolloff = ma_sound_set_rolloff;
    extern fn ma_sound_set_rolloff(sound: *Sound, rolloff: f32) void;

    pub const getRolloff = ma_sound_get_rolloff;
    extern fn ma_sound_get_rolloff(sound: *const Sound) f32;

    pub const setMinGain = ma_sound_set_min_gain;
    extern fn ma_sound_set_min_gain(sound: *Sound, min_gain: f32) void;

    pub const getMinGain = ma_sound_get_min_gain;
    extern fn ma_sound_get_min_gain(sound: *const Sound) f32;

    pub const setMaxGain = ma_sound_set_max_gain;
    extern fn ma_sound_set_max_gain(sound: *Sound, max_gain: f32) void;

    pub const getMaxGain = ma_sound_get_max_gain;
    extern fn ma_sound_get_max_gain(sound: *const Sound) f32;

    pub const setMinDistance = ma_sound_set_min_distance;
    extern fn ma_sound_set_min_distance(sound: *Sound, min_distance: f32) void;

    pub const getMinDistance = ma_sound_get_min_distance;
    extern fn ma_sound_get_min_distance(sound: *const Sound) f32;

    pub const setMaxDistance = ma_sound_set_max_distance;
    extern fn ma_sound_set_max_distance(sound: *Sound, max_distance: f32) void;

    pub const getMaxDistance = ma_sound_get_max_distance;
    extern fn ma_sound_get_max_distance(sound: *const Sound) f32;

    pub const setCone = ma_sound_set_cone;
    extern fn ma_sound_set_cone(sound: *Sound, inner_radians: f32, outer_radians: f32, outer_gain: f32) void;

    pub const getCone = ma_sound_get_cone;
    extern fn ma_sound_get_cone(
        sound: *const Sound,
        inner_radians: ?*f32,
        outer_radians: ?*f32,
        outer_gain: ?*f32,
    ) void;

    pub const setDopplerFactor = ma_sound_set_doppler_factor;
    extern fn ma_sound_set_doppler_factor(sound: *Sound, factor: f32) void;

    pub const getDopplerFactor = ma_sound_get_doppler_factor;
    extern fn ma_sound_get_doppler_factor(sound: *const Sound) f32;

    pub const setDirectionalAttenuationFactor = ma_sound_set_directional_attenuation_factor;
    extern fn ma_sound_set_directional_attenuation_factor(sound: *Sound, factor: f32) void;

    pub const getDirectionalAttenuationFactor = ma_sound_get_directional_attenuation_factor;
    extern fn ma_sound_get_directional_attenuation_factor(sound: *const Sound) f32;

    pub const setFadeInPcmFrames = ma_sound_set_fade_in_pcm_frames;
    extern fn ma_sound_set_fade_in_pcm_frames(
        sound: *Sound,
        volume_begin: f32,
        volume_end: f32,
        len_in_frames: u64,
    ) void;

    pub const setFadeInMilliseconds = ma_sound_set_fade_in_milliseconds;
    extern fn ma_sound_set_fade_in_milliseconds(
        sound: *Sound,
        volume_begin: f32,
        volume_end: f32,
        len_in_ms: u64,
    ) void;

    pub const getCurrentFadeVolume = ma_sound_get_current_fade_volume;
    extern fn ma_sound_get_current_fade_volume(sound: *const Sound) f32;

    pub const setStartTimeInPcmFrames = ma_sound_set_start_time_in_pcm_frames;
    extern fn ma_sound_set_start_time_in_pcm_frames(sound: *Sound, abs_global_time_in_frames: u64) void;

    pub const setStartTimeInMilliseconds = ma_sound_set_start_time_in_milliseconds;
    extern fn ma_sound_set_start_time_in_milliseconds(sound: *Sound, abs_global_time_in_ms: u64) void;

    pub const setStopTimeInPcmFrames = ma_sound_set_stop_time_in_pcm_frames;
    extern fn ma_sound_set_stop_time_in_pcm_frames(sound: *Sound, abs_global_time_in_frames: u64) void;

    pub const setStopTimeInMilliseconds = ma_sound_set_stop_time_in_milliseconds;
    extern fn ma_sound_set_stop_time_in_milliseconds(sound: *Sound, abs_global_time_in_ms: u64) void;

    pub fn isPlaying(sound: *const Sound) bool {
        return ma_sound_is_playing(sound) != .false32;
    }
    extern fn ma_sound_is_playing(sound: *const Sound) Bool32;

    pub const getTimeInPcmFrames = ma_sound_get_time_in_pcm_frames;
    extern fn ma_sound_get_time_in_pcm_frames(sound: *const Sound) u64;

    pub fn setLooping(sound: *Sound, looping: bool) void {
        ma_sound_set_looping(sound, if (looping) .true32 else .false32);
    }
    extern fn ma_sound_set_looping(sound: *Sound, looping: Bool32) void;

    pub fn isLooping(sound: *const Sound) bool {
        return ma_sound_is_looping(sound) != .false32;
    }
    extern fn ma_sound_is_looping(sound: *const Sound) Bool32;

    pub fn isAtEnd(sound: *const Sound) bool {
        return ma_sound_at_end(sound) != .false32;
    }
    extern fn ma_sound_at_end(sound: *const Sound) Bool32;

    pub fn seekToPcmFrame(sound: *Sound, frame_index: u64) Error!void {
        try maybeError(ma_sound_seek_to_pcm_frame(sound, frame_index));
    }
    extern fn ma_sound_seek_to_pcm_frame(sound: *Sound, frame_index: u64) Result;

    pub fn getDataFormat(
        sound: *const Sound,
        format: ?*Format,
        channels: ?*u32,
        sample_rate: ?*u32,
        channel_map: ?[]Channel,
    ) Error!void {
        try maybeError(ma_sound_get_data_format(
            sound,
            format,
            channels,
            sample_rate,
            if (channel_map) |chm| chm.ptr else null,
            if (channel_map) |chm| chm.len else 0,
        ));
    }
    extern fn ma_sound_get_data_format(
        sound: *const Sound,
        format: ?*Format,
        channels: ?*u32,
        sample_rate: ?*u32,
        channel_map: ?[*]Channel,
        channel_map_cap: usize,
    ) Result;

    pub fn getCursorInPcmFrames(sound: *const Sound) Error!u64 {
        var cursor: u64 = 0;
        try maybeError(ma_sound_get_cursor_in_pcm_frames(sound, &cursor));
        return cursor;
    }
    extern fn ma_sound_get_cursor_in_pcm_frames(sound: *const Sound, cursor: *u64) Result;

    pub fn getLengthInPcmFrames(sound: *const Sound) Error!u64 {
        var length: u64 = 0;
        try maybeError(ma_sound_get_length_in_pcm_frames(sound, &length));
        return length;
    }
    extern fn ma_sound_get_length_in_pcm_frames(sound: *const Sound, length: *u64) Result;

    pub fn getCursorInSeconds(sound: *const Sound) Error!f32 {
        var cursor: f32 = 0.0;
        try maybeError(ma_sound_get_cursor_in_seconds(sound, &cursor));
        return cursor;
    }
    extern fn ma_sound_get_cursor_in_seconds(sound: *const Sound, cursor: *f32) Result;

    pub fn getLengthInSeconds(sound: *const Sound) Error!f32 {
        var length: f32 = 0.0;
        try maybeError(ma_sound_get_length_in_seconds(sound, &length));
        return length;
    }
    extern fn ma_sound_get_length_in_seconds(sound: *const Sound, length: *f32) Result;

    pub const Flags = packed struct(u32) {
        stream: bool = false,
        decode: bool = false,
        async_load: bool = false,
        wait_init: bool = false,
        no_default_attachment: bool = false,
        no_pitch: bool = false,
        no_spatialization: bool = false,
        _padding: u25 = 0,
    };

    pub const Config = extern struct {
        file_path: ?[*:0]const u8,
        file_path_w: ?[*:0]const i32,
        data_source: ?*DataSource,
        initial_attachment: ?*Node,
        initial_attachment_input_bus_index: u32,
        channels_in: u32,
        channels_out: u32,
        mono_expansion_mode: MonoExpansionMode,
        flags: Flags,
        initial_seek_point_in_pcm_frames: u64,
        range_beg_in_pcm_frames: u64,
        range_end_in_pcm_Frames: u64,
        loop_point_beg_in_pcm_frames: u64,
        loop_point_end_in_pcm_frames: u64,
        is_looping: Bool32,
        done_fence: ?*Fence,

        pub fn init() Config {
            var config: Config = undefined;
            zaudioSoundConfigInit(&config);
            return config;
        }
        extern fn zaudioSoundConfigInit(out_config: *Config) void;
    };
};
//--------------------------------------------------------------------------------------------------
//
// SoundGroup (-> Node)
//
//--------------------------------------------------------------------------------------------------
pub const SoundGroup = opaque {
    pub usingnamespace Node.Methods(@This());

    pub const Config = Sound.Config;

    fn create(engine: *Engine, flags: Sound.Flags, parent: ?*SoundGroup) Error!*SoundGroup {
        var handle: ?*SoundGroup = null;
        try maybeError(zaudioSoundGroupCreate(engine, flags, parent, &handle));
        return handle.?;
    }
    extern fn zaudioSoundGroupCreate(
        engine: *Engine,
        flags: Sound.Flags,
        parent: ?*SoundGroup,
        out_handle: ?*?*SoundGroup,
    ) Result;

    pub const destroy = zaudioSoundGroupDestroy;
    extern fn zaudioSoundGroupDestroy(handle: *SoundGroup) void;

    pub const getEngine = ma_sound_group_get_engine;
    extern fn ma_sound_group_get_engine(sound: *const SoundGroup) *Engine;

    pub fn start(sound: *SoundGroup) Error!void {
        try maybeError(ma_sound_group_start(sound));
    }
    extern fn ma_sound_group_start(sound: *SoundGroup) Result;

    pub fn stop(sound: *SoundGroup) Error!void {
        try maybeError(ma_sound_group_stop(sound));
    }
    extern fn ma_sound_group_stop(sound: *SoundGroup) Result;

    pub const setVolume = ma_sound_group_set_volume;
    extern fn ma_sound_group_set_volume(sound: *SoundGroup, volume: f32) void;

    pub const getVolume = ma_sound_group_get_volume;
    extern fn ma_sound_group_get_volume(sound: *const SoundGroup) f32;

    pub const setPan = ma_sound_group_set_pan;
    extern fn ma_sound_group_set_pan(sound: *SoundGroup, pan: f32) void;

    pub const getPan = ma_sound_group_get_pan;
    extern fn ma_sound_group_get_pan(sound: *const SoundGroup) f32;

    pub const setPanMode = ma_sound_group_set_pan_mode;
    extern fn ma_sound_group_set_pan_mode(sound: *SoundGroup, pan_mode: PanMode) void;

    pub const getPanMode = ma_sound_group_get_pan_mode;
    extern fn ma_sound_group_get_pan_mode(sound: *const SoundGroup) PanMode;

    pub const setPitch = ma_sound_group_set_pitch;
    extern fn ma_sound_group_set_pitch(sound: *SoundGroup, pitch: f32) void;

    pub const getPitch = ma_sound_group_get_pitch;
    extern fn ma_sound_group_get_pitch(sound: *const SoundGroup) f32;

    pub fn setSpatializationEnabled(sound: *SoundGroup, enabled: bool) void {
        ma_sound_group_set_spatialization_enabled(sound, @boolToInt(enabled));
    }
    extern fn ma_sound_group_set_spatialization_enabled(sound: *SoundGroup, enabled: Bool32) void;

    pub fn isSpatializationEnabled(sound: *const SoundGroup) bool {
        return ma_sound_group_is_spatialization_enabled(sound) != .false32;
    }
    extern fn ma_sound_group_is_spatialization_enabled(sound: *const SoundGroup) Bool32;

    pub const setPinnedListenerIndex = ma_sound_group_set_pinned_listener_index;
    extern fn ma_sound_group_set_pinned_listener_index(sound: *SoundGroup, index: u32) void;

    pub const getPinnedListenerIndex = ma_sound_group_get_pinned_listener_index;
    extern fn ma_sound_group_get_pinned_listener_index(sound: *const SoundGroup) u32;

    pub const getListenerIndex = ma_sound_group_get_listener_index;
    extern fn ma_sound_group_get_listener_index(sound: *const SoundGroup) u32;

    pub fn getDirectionToListener(sound: *const SoundGroup) [3]f32 {
        var v: [3]f32 = undefined;
        WA_ma_sound_group_get_direction_to_listener(sound, &v);
        return v;
    }
    extern fn WA_ma_sound_group_get_direction_to_listener(sound: *const SoundGroup, vout: *[3]f32) void;

    pub fn setPosition(sound: *SoundGroup, v: [3]f32) void {
        ma_sound_group_set_position(sound, v[0], v[1], v[2]);
    }
    extern fn ma_sound_group_set_position(sound: *const SoundGroup, x: f32, y: f32, z: f32) void;

    pub fn getPosition(sound: *const SoundGroup) [3]f32 {
        var v: [3]f32 = undefined;
        WA_ma_sound_group_get_position(sound, &v);
        return v;
    }
    extern fn WA_ma_sound_group_get_position(sound: *const SoundGroup, vout: *[3]f32) void;

    pub fn setDirection(sound: *SoundGroup, v: [3]f32) void {
        ma_sound_group_set_direction(sound, v[0], v[1], v[2]);
    }
    extern fn ma_sound_group_set_direction(sound: *SoundGroup, x: f32, y: f32, z: f32) void;

    pub fn getDirection(sound: *const SoundGroup) [3]f32 {
        var v: [3]f32 = undefined;
        WA_ma_sound_group_get_direction(sound, &v);
        return v;
    }
    extern fn WA_ma_sound_group_get_direction(sound: *const SoundGroup, vout: *[3]f32) void;

    pub fn setVelocity(sound: *SoundGroup, v: [3]f32) void {
        ma_sound_group_set_velocity(sound, v[0], v[1], v[2]);
    }
    extern fn ma_sound_group_set_velocity(sound: *SoundGroup, x: f32, y: f32, z: f32) void;

    pub fn getVelocity(sound: *const SoundGroup) [3]f32 {
        var v: [3]f32 = undefined;
        WA_ma_sound_group_get_velocity(sound, &v);
        return v;
    }
    extern fn WA_ma_sound_group_get_velocity(sound: *const SoundGroup, vout: *[3]f32) void;

    pub const setAttenuationModel = ma_sound_group_set_attenuation_model;
    extern fn ma_sound_group_set_attenuation_model(sound: *SoundGroup, model: AttenuationModel) void;

    pub const getAttenuationModel = ma_sound_group_get_attenuation_model;
    extern fn ma_sound_group_get_attenuation_model(sound: *const SoundGroup) AttenuationModel;

    pub const setPositioning = ma_sound_group_set_positioning;
    extern fn ma_sound_group_set_positioning(sound: *SoundGroup, pos: Positioning) void;

    pub const getPositioning = ma_sound_group_get_positioning;
    extern fn ma_sound_group_get_positioning(sound: *const SoundGroup) Positioning;

    pub const setRolloff = ma_sound_group_set_rolloff;
    extern fn ma_sound_group_set_rolloff(sound: *SoundGroup, rolloff: f32) void;

    pub const getRolloff = ma_sound_group_get_rolloff;
    extern fn ma_sound_group_get_rolloff(sound: *const SoundGroup) f32;

    pub const setMinGain = ma_sound_group_set_min_gain;
    extern fn ma_sound_group_set_min_gain(sound: *SoundGroup, min_gain: f32) void;

    pub const getMinGain = ma_sound_group_get_min_gain;
    extern fn ma_sound_group_get_min_gain(sound: *const SoundGroup) f32;

    pub const setMaxGain = ma_sound_group_set_max_gain;
    extern fn ma_sound_group_set_max_gain(sound: *SoundGroup, max_gain: f32) void;

    pub const getMaxGain = ma_sound_group_get_max_gain;
    extern fn ma_sound_group_get_max_gain(sound: *const SoundGroup) f32;

    pub const setMinDistance = ma_sound_group_set_min_distance;
    extern fn ma_sound_group_set_min_distance(sound: *SoundGroup, min_distance: f32) void;

    pub const getMinDistance = ma_sound_group_get_min_distance;
    extern fn ma_sound_group_get_min_distance(sound: *const SoundGroup) f32;

    pub const setMaxDistance = ma_sound_group_set_max_distance;
    extern fn ma_sound_group_set_max_distance(sound: *SoundGroup, max_distance: f32) void;

    pub const getMaxDistance = ma_sound_group_get_max_distance;
    extern fn ma_sound_group_get_max_distance(sound: *const SoundGroup) f32;

    pub const setCone = ma_sound_group_set_cone;
    extern fn ma_sound_group_set_cone(
        sound: *SoundGroup,
        inner_radians: f32,
        outer_radians: f32,
        outer_gain: f32,
    ) void;

    pub const getCone = ma_sound_group_get_cone;
    extern fn ma_sound_group_get_cone(
        sound: *const SoundGroup,
        inner_radians: ?*f32,
        outer_radians: ?*f32,
        outer_gain: ?*f32,
    ) void;

    pub const setDopplerFactor = ma_sound_group_set_doppler_factor;
    extern fn ma_sound_group_set_doppler_factor(sound: *SoundGroup, factor: f32) void;

    pub const getDopplerFactor = ma_sound_group_get_doppler_factor;
    extern fn ma_sound_group_get_doppler_factor(sound: *const SoundGroup) f32;

    pub const setDirectionalAttenuationFactor = ma_sound_group_set_directional_attenuation_factor;
    extern fn ma_sound_group_set_directional_attenuation_factor(sound: *SoundGroup, factor: f32) void;

    pub const getDirectionalAttenuationFactor = ma_sound_group_get_directional_attenuation_factor;
    extern fn ma_sound_group_get_directional_attenuation_factor(sound: *const SoundGroup) f32;

    pub const setFadeInPcmFrames = ma_sound_group_set_fade_in_pcm_frames;
    extern fn ma_sound_group_set_fade_in_pcm_frames(
        sound: *SoundGroup,
        volume_begin: f32,
        volume_end: f32,
        len_in_frames: u64,
    ) void;

    pub const setFadeInMilliseconds = ma_sound_group_set_fade_in_milliseconds;
    extern fn ma_sound_group_set_fade_in_milliseconds(
        sound: *SoundGroup,
        volume_begin: f32,
        volume_end: f32,
        len_in_ms: u64,
    ) void;

    pub const getCurrentFadeVolume = ma_sound_group_get_current_fade_volume;
    extern fn ma_sound_group_get_current_fade_volume(sound: *const SoundGroup) f32;

    pub const setStartTimeInPcmFrames = ma_sound_group_set_start_time_in_pcm_frames;
    extern fn ma_sound_group_set_start_time_in_pcm_frames(sound: *SoundGroup, abs_global_time_in_frames: u64) void;

    pub const setStartTimeInMilliseconds = ma_sound_group_set_start_time_in_milliseconds;
    extern fn ma_sound_group_set_start_time_in_milliseconds(sound: *SoundGroup, abs_global_time_in_ms: u64) void;

    pub const setStopTimeInPcmFrames = ma_sound_group_set_stop_time_in_pcm_frames;
    extern fn ma_sound_group_set_stop_time_in_pcm_frames(sound: *SoundGroup, abs_global_time_in_frames: u64) void;

    pub const setStopTimeInMilliseconds = ma_sound_group_set_stop_time_in_milliseconds;
    extern fn ma_sound_group_set_stop_time_in_milliseconds(sound: *SoundGroup, abs_global_time_in_ms: u64) void;

    pub fn isPlaying(sound: *const SoundGroup) bool {
        return ma_sound_group_is_playing(sound) != .false32;
    }
    extern fn ma_sound_group_is_playing(sound: *const SoundGroup) Bool32;

    pub const getTimeInPcmFrames = ma_sound_group_get_time_in_pcm_frames;
    extern fn ma_sound_group_get_time_in_pcm_frames(sound: *const SoundGroup) u64;
};
//--------------------------------------------------------------------------------------------------
//
// Fence
//
//--------------------------------------------------------------------------------------------------
pub const Fence = opaque {
    pub fn create() Error!*Fence {
        var handle: ?*Fence = null;
        try maybeError(zaudioFenceCreate(&handle));
        return handle.?;
    }
    extern fn zaudioFenceCreate(out_handle: ?*?*Fence) Result;

    pub const destroy = zaudioFenceDestroy;
    extern fn zaudioFenceDestroy(handle: *Fence) void;

    pub fn acquire(fence: *Fence) Error!void {
        try maybeError(ma_fence_acquire(fence));
    }
    extern fn ma_fence_acquire(fence: *Fence) Result;

    pub fn release(fence: *Fence) Error!void {
        try maybeError(ma_fence_release(fence));
    }
    extern fn ma_fence_release(fence: *Fence) Result;

    pub fn wait(fence: *Fence) Error!void {
        try maybeError(ma_fence_wait(fence));
    }
    extern fn ma_fence_wait(fence: *Fence) Result;
};
//--------------------------------------------------------------------------------------------------
//
// Memory
//
//--------------------------------------------------------------------------------------------------
var mem_allocator: ?std.mem.Allocator = null;
var mem_allocations: ?std.AutoHashMap(usize, usize) = null;
var mem_mutex: std.Thread.Mutex = .{};
const mem_alignment = 16;

extern var zaudioMallocPtr: ?*const fn (size: usize, _: ?*anyopaque) callconv(.C) ?*anyopaque;

fn zaudioMalloc(size: usize, _: ?*anyopaque) callconv(.C) ?*anyopaque {
    mem_mutex.lock();
    defer mem_mutex.unlock();

    const mem = mem_allocator.?.alignedAlloc(
        u8,
        mem_alignment,
        size,
    ) catch @panic("zaudio: out of memory");

    mem_allocations.?.put(@ptrToInt(mem.ptr), size) catch @panic("zaudio: out of memory");

    return mem.ptr;
}

extern var zaudioReallocPtr: ?*const fn (ptr: ?*anyopaque, size: usize, _: ?*anyopaque) callconv(.C) ?*anyopaque;

fn zaudioRealloc(ptr: ?*anyopaque, size: usize, _: ?*anyopaque) callconv(.C) ?*anyopaque {
    mem_mutex.lock();
    defer mem_mutex.unlock();

    const old_size = if (ptr != null) mem_allocations.?.get(@ptrToInt(ptr.?)).? else 0;
    const old_mem = if (old_size > 0)
        @ptrCast([*]align(mem_alignment) u8, @alignCast(mem_alignment, ptr))[0..old_size]
    else
        @as([*]align(mem_alignment) u8, undefined)[0..0];

    const new_mem = mem_allocator.?.realloc(old_mem, size) catch @panic("zaudio: out of memory");

    if (ptr != null) {
        const removed = mem_allocations.?.remove(@ptrToInt(ptr.?));
        std.debug.assert(removed);
    }

    mem_allocations.?.put(@ptrToInt(new_mem.ptr), size) catch @panic("zaudio: out of memory");

    return new_mem.ptr;
}

extern var zaudioFreePtr: ?*const fn (maybe_ptr: ?*anyopaque, _: ?*anyopaque) callconv(.C) void;

fn zaudioFree(maybe_ptr: ?*anyopaque, _: ?*anyopaque) callconv(.C) void {
    if (maybe_ptr) |ptr| {
        mem_mutex.lock();
        defer mem_mutex.unlock();

        const size = mem_allocations.?.fetchRemove(@ptrToInt(ptr)).?.value;
        const mem = @ptrCast([*]align(mem_alignment) u8, @alignCast(mem_alignment, ptr))[0..size];
        mem_allocator.?.free(mem);
    }
}
//--------------------------------------------------------------------------------------------------
//
// Tests
//
//--------------------------------------------------------------------------------------------------
const expect = std.testing.expect;

test "zaudio.engine.basic" {
    init(std.testing.allocator);
    defer deinit();

    const engine = try Engine.create(null);
    defer engine.destroy();

    try engine.setTime(engine.getTime());

    _ = engine.getChannels();
    _ = engine.getSampleRate();
    _ = engine.getListenerCount();
    _ = engine.findClosestListener(.{ 0.0, 0.0, 0.0 });

    try engine.start();
    try engine.stop();
    try engine.start();
    try engine.setVolume(1.0);
    try engine.setGainDb(1.0);

    engine.setListenerEnabled(0, true);
    try expect(engine.isListenerEnabled(0) == true);

    engine.setListenerPosition(0, .{ 1.0, 2.0, 3.0 });
    {
        const pos = engine.getListenerPosition(0);
        try expect(pos[0] == 1.0 and pos[1] == 2.0 and pos[2] == 3.0);
    }

    try expect(engine.getDevice() != null);
    _ = engine.getResourceManager();
    _ = engine.getResourceManagerMut();
    _ = engine.getLog();
    _ = engine.getEndpoint();

    const hpf_node = try engine.createHpfNode(HpfNode.Config.init(
        engine.getChannels(),
        engine.getSampleRate(),
        1000.0,
        2,
    ));
    defer hpf_node.destroy();
    try hpf_node.attachOutputBus(0, engine.getEndpointMut(), 0);
}

test "zaudio.soundgroup.basic" {
    init(std.testing.allocator);
    defer deinit();

    const engine = try Engine.create(null);
    defer engine.destroy();

    const sgroup = try engine.createSoundGroup(.{}, null);
    defer sgroup.destroy();

    try expect(sgroup.getEngine() == engine);

    try sgroup.start();
    try sgroup.stop();
    try sgroup.start();

    _ = sgroup.getInputChannels(0);
    _ = sgroup.getOutputChannels(0);
    _ = sgroup.getInputBusCount();
    _ = sgroup.getOutputBusCount();

    sgroup.setVolume(0.5);
    try expect(sgroup.getVolume() == 0.5);

    sgroup.setPan(0.25);
    try expect(sgroup.getPan() == 0.25);

    const sdir = [3]f32{ 1.0, 2.0, 3.0 };
    sgroup.setDirection(sdir);
    {
        const gdir = sgroup.getDirection();
        expect(sdir[0] == gdir[0] and sdir[1] == gdir[1] and sdir[2] == gdir[2]) catch |err| {
            std.debug.print("Direction is: {any} should be: {any}\n", .{ gdir, sdir });
            return err;
        };
    }
}

test "zaudio.fence.basic" {
    init(std.testing.allocator);
    defer deinit();

    const fence = try Fence.create();
    defer fence.destroy();

    try fence.acquire();
    try fence.release();
    try fence.wait();
}

test "zaudio.sound.basic" {
    init(std.testing.allocator);
    defer deinit();

    const engine = try Engine.create(null);
    defer engine.destroy();

    var config = Sound.Config.init();
    config.channels_in = 1;
    const sound = try engine.createSound(config);
    defer sound.destroy();

    _ = sound.getInputChannels(0);
    _ = sound.getOutputChannels(0);
    _ = sound.getInputBusCount();
    _ = sound.getOutputBusCount();

    // Cloning only works for data buffers (not streams) that are loaded from the resource manager.
    _ = engine.createSoundCopy(sound, .{}, null) catch |err| try expect(err == Error.InvalidOperation);

    sound.setVolume(0.25);
    try expect(sound.getVolume() == 0.25);

    sound.setPanMode(.pan);
    try expect(sound.getPanMode() == .pan);

    sound.setLooping(false);
    try expect(sound.isLooping() == false);

    sound.setPitch(0.5);
    try expect(sound.getPitch() == 0.5);

    try expect(sound.getNodeGraph() == engine.asNodeGraph());

    var format: Format = .unknown;
    var num_channels: u32 = 0;
    var sample_rate: u32 = 0;
    try sound.getDataFormat(&format, &num_channels, &sample_rate, null);
    try expect(num_channels == 1);
    try expect(sample_rate > 0);
    try expect(format != .unknown);
}

test "zaudio.device.basic" {
    init(std.testing.allocator);
    defer deinit();

    var config = Device.Config.init(.playback);
    config.playback.format = .float32;
    config.playback.channels = 2;
    config.sample_rate = 48_000;
    const device = try Device.create(null, config);
    defer device.destroy();
    try device.start();
    try expect(device.getState() == .started or device.getState() == .starting);
    try device.stop();
    try expect(device.getState() == .stopped or device.getState() == .stopping);
    const context = device.getContext();
    _ = context;
    const log = device.getLog();
    _ = log;
    try device.setMasterVolume(0.1);
    try expect((try device.getMasterVolume()) == 0.1);
}

test "zaudio.node_graph.basic" {
    init(std.testing.allocator);
    defer deinit();

    const config = NodeGraph.Config.init(2);
    const node_graph = try NodeGraph.create(config);
    defer node_graph.destroy();
    _ = node_graph.getTime();
}

test "zaudio.audio_buffer" {
    init(std.testing.allocator);
    defer deinit();

    var samples = try std.ArrayList(f32).initCapacity(std.testing.allocator, 1000);
    defer samples.deinit();

    var prng = std.rand.DefaultPrng.init(0);
    const rand = prng.random();

    samples.expandToCapacity();
    for (samples.items) |*sample| {
        sample.* = -1.0 + 2.0 * rand.float(f32);
    }

    const audio_buffer = try AudioBuffer.create(
        AudioBuffer.Config.init(.float32, 1, samples.items.len, samples.items.ptr),
    );
    defer audio_buffer.destroy();

    const engine = try Engine.create(null);
    defer engine.destroy();

    const sound = try engine.createSoundFromDataSource(audio_buffer.asDataSourceMut(), .{}, null);
    defer sound.destroy();

    sound.setLooping(true);
    try sound.start();

    std.time.sleep(1e8);
}

test {
    std.testing.refAllDecls(@This());
}
//--------------------------------------------------------------------------------------------------
