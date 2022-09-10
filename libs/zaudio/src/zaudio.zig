const std = @import("std");
const assert = std.debug.assert;
const math = std.math;
const c = if (@import("builtin").zig_backend == .stage1)
    @cImport(@cInclude("miniaudio.h"))
else
    @import("cimport_stage2.zig");

// TODO: Get rid of WA_ma_* functions which are workarounds for Zig C ABI issues on aarch64.

pub const SoundFlags = packed struct {
    stream: bool = false,
    decode: bool = false,
    async_load: bool = false,
    wait_init: bool = false,
    no_default_attachment: bool = false,
    no_pitch: bool = false,
    no_spatialization: bool = false,

    _padding: u25 = 0,

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

// TODO: Add all errors.
pub const Error = error{
    GenericError,
    InvalidArgs,
    InvalidOperation,
    OutOfMemory,
};

const Result = enum(i32) {
    success = 0,
    generic_error = -1,
    invalid_args = -2,
    invalid_operation = -3,
    out_of_memory = -4,
};

pub fn maybeError(result: Result) Error!void {
    return switch (result) {
        .success => {},
        .generic_error => Error.GenericError,
        .invalid_args => Error.InvalidArgs,
        .invalid_operation => Error.InvalidOperation,
        .out_of_memory => Error.OutOfMemory,
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

pub const DeviceState = enum(u32) {
    uninitialized = 0,
    stopped = 1,
    started = 2,
    starting = 3,
    stopping = 4,
};

pub const Bool32 = u32;
pub const Channel = u8;

pub const PlaybackCallback = extern struct {
    const CbType = *const fn (context: ?*anyopaque, outptr: *anyopaque, num_frames: u32) void;

    context: ?*anyopaque = null,
    callback: ?CbType = null,
};

pub const CaptureCallback = extern struct {
    const CbType = *const fn (context: ?*anyopaque, inptr: *const anyopaque, num_frames: u32) void;

    context: ?*anyopaque = null,
    callback: ?CbType = null,
};

pub const ResourceManager = *align(@sizeOf(usize)) ResourceManagerImpl;
const ResourceManagerImpl = opaque {
    // TODO: Add methods.
};

pub const Context = *align(@sizeOf(usize)) ContextImpl;
const ContextImpl = opaque {
    // TODO: Add methods.
};

pub const Log = *align(@sizeOf(usize)) LogImpl;
const LogImpl = opaque {
    // TODO: Add methods.
};
//--------------------------------------------------------------------------------------------------
//
// Data Source
//
//--------------------------------------------------------------------------------------------------
pub const DataSource = *align(@sizeOf(usize)) DataSourceImpl;

pub fn createDataSource(config: DataSourceConfig) Error!DataSource {
    var handle: ?DataSource = null;
    try maybeError(zaudioDataSourceCreate(&config, &handle));
    return handle.?;
}
extern fn zaudioDataSourceCreate(config: *const DataSourceConfig, out_handle: ?*?*DataSourceImpl) Result;

pub const DataSourceFlags = packed struct(u32) {
    self_managed_range_and_loop_point: bool = false,
    _padding: u31 = 0,
};

pub const DataSourceVTable = extern struct {
    onRead: ?*const fn (
        ds: DataSource,
        frames_out: ?*anyopaque,
        frame_count: u64,
        frames_read: *u64,
    ) callconv(.C) Result,
    onSeek: ?*const fn (ds: DataSource, frame_index: u64) callconv(.C) Result,
    onGetDataFormat: ?*const fn (
        ds: DataSource,
        format: ?*Format,
        channels: ?*u32,
        sample_rate: ?*u32,
        channel_map: ?[*]Channel,
        channel_map_cap: usize,
    ) callconv(.C) Result,
    onGetCursor: ?*const fn (ds: DataSource, cursor: ?*u64) callconv(.C) Result,
    onGetLength: ?*const fn (ds: DataSource, length: ?*u64) callconv(.C) Result,
    onSetLooping: ?*const fn (ds: DataSource, is_looping: Bool32) callconv(.C) Result,
    flags: DataSourceFlags,
};

pub const DataSourceConfig = extern struct {
    vtable: *const DataSourceVTable,

    pub fn init() DataSourceConfig {
        var config: DataSourceConfig = undefined;
        zaudioDataSourceConfigInit(&config);
        return config;
    }
    extern fn zaudioDataSourceConfigInit(out_config: *DataSourceConfig) void;
};

const DataSourceImpl = opaque {
    pub const destroy = zaudioDataSourceDestroy;
    extern fn zaudioDataSourceDestroy(handle: DataSource) void;

    fn Methods(comptime T: type) type {
        return struct {
            pub fn asDataSource(handle: T) DataSource {
                return @ptrCast(DataSource, handle);
            }
            // TODO: Add missing methods.
        };
    }
};
//--------------------------------------------------------------------------------------------------
//
// Waveform Data Source
//
//--------------------------------------------------------------------------------------------------
pub const WaveformDataSource = *align(@sizeOf(usize)) WaveformDataSourceImpl;

pub fn createWaveformDataSource(config: WaveformConfig) Error!WaveformDataSource {
    var handle: ?WaveformDataSource = null;
    try maybeError(zaudioWaveformCreate(&config, &handle));
    return handle.?;
}
extern fn zaudioWaveformCreate(config: *const WaveformConfig, handle: ?*?*WaveformDataSourceImpl) Result;

pub const WaveformType = enum(u32) {
    sine,
    square,
    triangle,
    sawtooth,
};

pub const WaveformConfig = extern struct {
    format: Format,
    channels: u32,
    sampleRate: u32,
    waveform_type: WaveformType,
    amplitude: f64,
    frequency: f64,

    pub fn init(
        format: Format,
        num_channels: u32,
        sample_rate: u32,
        waveform_type: WaveformType,
        amplitude: f64,
        frequency: f64,
    ) WaveformConfig {
        var config: WaveformConfig = undefined;
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
        waveform_type: WaveformType,
        amplitude: f64,
        frequency: f64,
        out_config: *WaveformConfig,
    ) void;
};

const WaveformDataSourceImpl = opaque {
    pub usingnamespace DataSourceImpl.Methods(WaveformDataSource);

    pub const destroy = zaudioWaveformDestroy;
    extern fn zaudioWaveformDestroy(waveform: WaveformDataSource) void;

    pub fn setAmplitude(waveform: WaveformDataSource, amplitude: f64) Error!void {
        try maybeError(ma_waveform_set_amplitude(waveform, amplitude));
    }
    extern fn ma_waveform_set_amplitude(waveform: WaveformDataSource, amplitude: f64) Result;

    pub fn setFrequency(waveform: WaveformDataSource, frequency: f64) Error!void {
        try maybeError(ma_waveform_set_frequency(waveform, frequency));
    }
    extern fn ma_waveform_set_frequency(waveform: WaveformDataSource, frequency: f64) Result;

    pub fn setType(waveform: WaveformDataSource, waveform_type: WaveformType) Error!void {
        try maybeError(ma_waveform_set_type(waveform, waveform_type));
    }
    extern fn ma_waveform_set_type(waveform: WaveformDataSource, waveform_type: WaveformType) Result;

    pub fn setSampleRate(waveform: WaveformDataSource, sample_rate: u32) Error!void {
        try maybeError(ma_waveform_set_sample_rate(waveform, sample_rate));
    }
    extern fn ma_waveform_set_sample_rate(waveform: WaveformDataSource, sample_rate: u32) Result;
};
//--------------------------------------------------------------------------------------------------
//
// Noise Data Source
//
//--------------------------------------------------------------------------------------------------
pub const NoiseDataSource = *align(@sizeOf(usize)) NoiseDataSourceImpl;

pub fn createNoiseDataSource(config: NoiseConfig) Error!NoiseDataSource {
    var handle: ?NoiseDataSource = null;
    try maybeError(zaudioNoiseCreate(&config, &handle));
    return handle.?;
}
extern fn zaudioNoiseCreate(config: *const NoiseConfig, out_handle: ?*?*NoiseDataSourceImpl) Result;

pub const NoiseType = enum(u32) {
    white,
    pink,
    brownian,
};

pub const NoiseConfig = extern struct {
    format: Format,
    num_channels: u32,
    noise_type: NoiseType,
    seed: i32,
    amplitude: f64,
    duplicate_channels: Bool32,

    pub fn init(
        format: Format,
        num_channels: u32,
        noise_type: NoiseType,
        seed: i32,
        amplitude: f64,
    ) NoiseConfig {
        var config: NoiseConfig = undefined;
        zaudioNoiseConfigInit(
            format,
            num_channels,
            noise_type,
            seed,
            amplitude,
            &config,
        );
        return config;
    }
    extern fn zaudioNoiseConfigInit(
        format: Format,
        num_channels: u32,
        noise_type: NoiseType,
        seed: i32,
        amplitude: f64,
        out_config: *NoiseConfig,
    ) void;
};

const NoiseDataSourceImpl = opaque {
    pub usingnamespace DataSourceImpl.Methods(NoiseDataSource);

    pub const destroy = zaudioNoiseDestroy;
    extern fn zaudioNoiseDestroy(handle: NoiseDataSource) void;

    pub fn setAmplitude(noise: NoiseDataSource, amplitude: f64) Error!void {
        try maybeError(ma_noise_set_amplitude(noise, amplitude));
    }
    extern fn ma_noise_set_amplitude(noise: NoiseDataSource, amplitude: f64) Result;

    pub fn setType(noise: NoiseDataSource, noise_type: NoiseType) Error!void {
        try maybeError(ma_noise_set_type(noise, noise_type));
    }
    extern fn ma_noise_set_type(noise: NoiseDataSource, noise_type: NoiseType) Result;
};
//--------------------------------------------------------------------------------------------------
//
// Node
//
//--------------------------------------------------------------------------------------------------
pub const Node = *align(@sizeOf(usize)) NodeImpl;

pub const NodeState = enum(u32) {
    started,
    stopped,
};

pub const NodeFlags = packed struct(u32) {
    passthrough: bool = false,
    continuous_processing: bool = false,
    allow_null_input: bool = false,
    different_processing_rates: bool = false,
    silent_output: bool = false,
    _padding: u27 = 0,
};

pub const NodeVTable = extern struct {
    onProcess: ?*const fn (
        node: Node,
        frames_in: ?[*][*]const f32,
        frame_count_in: *u32,
        frames_out: [*][*]f32,
        frame_count_out: *u32,
    ) callconv(.C) void,
    onGetRequiredInputFrameCount: ?*const fn (
        node: Node,
        num_output_frames: u32,
        num_input_frames: *u32,
    ) callconv(.C) Result,
    input_bus_count: u8,
    output_bus_count: u8,
    flags: NodeFlags,
};

pub const NodeConfig = extern struct {
    vtable: *const NodeVTable,
    initial_state: NodeState,
    input_bus_count: u32,
    output_bus_count: u32,
    input_channels: [*]const u32,
    output_channels: [*]const u32,

    pub fn init() NodeConfig {
        var config: NodeConfig = undefined;
        zaudioNodeConfigInit(&config);
        return config;
    }
    extern fn zaudioNodeConfigInit(out_config: *NodeConfig) void;
};

const NodeImpl = opaque {
    pub usingnamespace NodeImpl.Methods(Node);

    fn Methods(comptime T: type) type {
        return struct {
            pub fn asNode(node: T) Node {
                return @ptrCast(Node, node);
            }
            pub fn asRawNode(node: T) *c.ma_node {
                return @ptrCast(*c.ma_node, node);
            }

            pub fn getNodeGraph(node: T) NodeGraph {
                return ma_node_get_node_graph(node.asNode());
            }
            extern fn ma_node_get_node_graph(node: Node) NodeGraph;

            pub fn getNumInputBuses(node: T) u32 {
                return ma_node_get_input_bus_count(node.asNode());
            }
            extern fn ma_node_get_input_bus_count(node: Node) u32;

            pub fn getNumOutputBuses(node: T) u32 {
                return ma_node_get_output_bus_count(node.asNode());
            }
            extern fn ma_node_get_output_bus_count(node: Node) u32;

            pub fn getNumInputChannels(node: T, bus_index: u32) u32 {
                return ma_node_get_input_channels(node.asNode(), bus_index);
            }
            extern fn ma_node_get_input_channels(node: Node, bus_index: u32) u32;

            pub fn getNumOutputChannels(node: T, bus_index: u32) u32 {
                return ma_node_get_output_channels(node.asNode(), bus_index);
            }
            extern fn ma_node_get_output_channels(node: Node, bus_index: u32) u32;

            pub fn attachOutputBus(
                node: T,
                output_bus_index: u32,
                other_node: Node,
                other_node_input_bus_index: u32,
            ) Error!void {
                try maybeError(ma_node_attach_output_bus(
                    node.asNode(),
                    output_bus_index,
                    other_node.asNode(),
                    other_node_input_bus_index,
                ));
            }
            extern fn ma_node_attach_output_bus(
                node: Node,
                output_bus_index: u32,
                other_node: Node,
                other_node_input_bus_index: u32,
            ) Result;

            pub fn dettachOutputBus(node: T, output_bus_index: u32) Error!void {
                try maybeError(ma_node_detach_output_bus(node.asNode(), output_bus_index));
            }
            extern fn ma_node_detach_output_bus(node: Node, output_bus_index: u32) Result;

            pub fn dettachAllOutputBuses(node: T) Error!void {
                try maybeError(ma_node_detach_all_output_buses(node.asNode()));
            }
            extern fn ma_node_detach_all_output_buses(node: Node) Result;

            pub fn setOutputBusVolume(node: T, output_bus_index: u32, volume: f32) Error!void {
                try maybeError(ma_node_set_output_bus_volume(node.asNode(), output_bus_index, volume));
            }
            extern fn ma_node_set_output_bus_volume(node: Node, output_bus_index: u32, volume: f32) Result;

            pub fn getOutputBusVolume(node: T, output_bus_index: u32) f32 {
                return ma_node_get_output_bus_volume(node.asNode(), output_bus_index);
            }
            extern fn ma_node_get_output_bus_volume(node: Node, output_bus_index: u32) f32;

            pub fn setState(node: T, state: NodeState) Error!void {
                try maybeError(ma_node_set_state(node.asNode(), state));
            }
            extern fn ma_node_set_state(node: Node, NodeState) Result;

            pub fn getState(node: T) NodeState {
                return ma_node_get_state(node.asNode());
            }
            extern fn ma_node_get_state(node: Node) NodeState;

            pub fn setTime(node: T, local_time: u64) Error!void {
                try maybeError(ma_node_set_time(node.asNode(), local_time));
            }
            extern fn ma_node_set_time(node: Node, local_time: u64) Result;

            pub fn getTime(node: T) u64 {
                return ma_node_get_time(node.asNode());
            }
            extern fn ma_node_get_time(node: Node) u64;
        };
    }
};
//--------------------------------------------------------------------------------------------------
//
// Data Source Node
//
//--------------------------------------------------------------------------------------------------
pub const DataSourceNode = *align(@sizeOf(usize)) DataSourceNodeImpl;

pub const DataSourceNodeConfig = extern struct {
    node_config: NodeConfig,
    data_source: DataSource,

    pub fn init(ds: DataSource) DataSourceNodeConfig {
        var config: DataSourceNodeConfig = undefined;
        zaudioDataSourceNodeConfigInit(ds, &config);
        return config;
    }
    extern fn zaudioDataSourceNodeConfigInit(ds: DataSource, out_config: *DataSourceNodeConfig) void;
};

const DataSourceNodeImpl = opaque {
    pub usingnamespace NodeImpl.Methods(DataSourceNode);

    pub const destroy = zaudioDataSourceNodeDestroy;
    extern fn zaudioDataSourceNodeDestroy(handle: DataSourceNode) void;

    pub fn setLooping(handle: DataSourceNode, is_looping: bool) void {
        try maybeError(ma_data_source_node_set_looping(handle, @boolToInt(is_looping)));
    }
    extern fn ma_data_source_node_set_looping(handle: DataSourceNode, is_looping: Bool32) Result;

    pub fn isLooping(handle: DataSourceNode) bool {
        return if (ma_data_source_node_is_looping(handle) == 0) false else true;
    }
    extern fn ma_data_source_node_is_looping(handle: DataSourceNode) Bool32;
};
//--------------------------------------------------------------------------------------------------
//
// Splitter Node
//
//--------------------------------------------------------------------------------------------------
pub const SplitterNode = *align(@sizeOf(usize)) SplitterNodeImpl;

pub const SplitterNodeConfig = struct {
    node_config: NodeConfig,
    channels: u32,

    pub fn init(channels: u32) SplitterNodeConfig {
        var config: SplitterNodeConfig = undefined;
        zaudioSplitterNodeConfigInit(channels, &config);
        return config;
    }
    extern fn zaudioSplitterNodeConfigInit(channels: u32, out_config: *SplitterNodeConfig) void;
};

const SplitterNodeImpl = opaque {
    pub usingnamespace NodeImpl.Methods(SplitterNode);

    pub const destroy = zaudioSplitterNodeDestroy;
    extern fn zaudioSplitterNodeDestroy(handle: SplitterNode) void;
};
//--------------------------------------------------------------------------------------------------
//
// Biquad Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const BiquadNode = *align(@sizeOf(usize)) BiquadNodeImpl;

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

pub const BiquadNodeConfig = extern struct {
    node_config: NodeConfig,
    biquad: BiquadConfig,

    pub fn init(channels: u32, b0: f32, b1: f32, b2: f32, a0: f32, a1: f32, a2: f32) BiquadNodeConfig {
        var config: BiquadNodeConfig = undefined;
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
    out_config: *BiquadNodeConfig,
) void;

const BiquadNodeImpl = opaque {
    pub usingnamespace NodeImpl.Methods(BiquadNode);

    pub const destroy = zaudioBiquadNodeDestroy;
    extern fn zaudioBiquadNodeDestroy(handle: BiquadNode) void;

    pub fn reconfigure(handle: BiquadNode, config: BiquadNodeConfig) Error!void {
        try maybeError(ma_biquad_node_reinit(&config, handle));
    }
    extern fn ma_biquad_node_reinit(config: *const BiquadConfig, handle: BiquadNode) Result;
};
//--------------------------------------------------------------------------------------------------
//
// Low-Pass Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const LpfNode = *align(@sizeOf(usize)) LpfNodeImpl;

pub const LpfConfig = extern struct {
    format: Format,
    channels: u32,
    sampleRate: u32,
    cutoff_frequency: f64,
    order: u32,
};

pub const LpfNodeConfig = extern struct {
    node_config: NodeConfig,
    lpf: LpfConfig,

    pub fn init(channels: u32, sample_rate: u32, cutoff_frequency: f64, order: u32) LpfNodeConfig {
        var config: LpfNodeConfig = undefined;
        zaudioLpfNodeConfigInit(channels, sample_rate, cutoff_frequency, order, &config);
        return config;
    }
    extern fn zaudioLpfNodeConfigInit(
        channels: u32,
        sample_rate: u32,
        cutoff_frequency: f64,
        order: u32,
        out_config: *LpfNodeConfig,
    ) void;
};

const LpfNodeImpl = opaque {
    pub usingnamespace NodeImpl.Methods(LpfNode);

    pub const destroy = zaudioLpfNodeDestroy;
    extern fn zaudioLpfNodeDestroy(handle: LpfNode) void;

    pub fn reconfigure(handle: LpfNode, config: LpfConfig) Error!void {
        try maybeError(ma_lpf_node_reinit(&config, handle));
    }
    extern fn ma_lpf_node_reinit(config: *const LpfConfig, handle: LpfNode) Result;
};
//--------------------------------------------------------------------------------------------------
//
// High-Pass Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const HpfNode = *align(@sizeOf(usize)) HpfNodeImpl;

pub const HpfConfig = extern struct {
    format: Format,
    channels: u32,
    sample_rate: u32,
    cutoff_frequency: f64,
    order: u32,
};

pub const HpfNodeConfig = extern struct {
    node_config: NodeConfig,
    hpf: HpfConfig,

    pub fn init(channels: u32, sample_rate: u32, cutoff_frequency: f64, order: u32) HpfNodeConfig {
        var config: HpfNodeConfig = undefined;
        zaudioHpfNodeConfigInit(channels, sample_rate, cutoff_frequency, order, &config);
        return config;
    }
    extern fn zaudioHpfNodeConfigInit(
        channels: u32,
        sample_rate: u32,
        cutoff_frequency: f64,
        order: u32,
        out_config: *HpfNodeConfig,
    ) void;
};

const HpfNodeImpl = opaque {
    pub usingnamespace NodeImpl.Methods(HpfNode);

    pub const destroy = zaudioHpfNodeDestroy;
    extern fn zaudioHpfNodeDestroy(handle: HpfNode) void;

    pub fn reconfigure(handle: HpfNode, config: HpfConfig) Error!void {
        try maybeError(ma_hpf_node_reinit(&config, handle));
    }
    extern fn ma_hpf_node_reinit(config: *const HpfConfig, handle: HpfNode) Result;
};
//--------------------------------------------------------------------------------------------------
//
// Notch Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const NotchNode = *align(@sizeOf(usize)) NotchNodeImpl;

pub const NotchConfig = extern struct {
    format: Format,
    channels: u32,
    sample_rate: u32,
    q: f64,
    frequency: f64,
};

pub const NotchNodeConfig = extern struct {
    node_config: NodeConfig,
    notch: NotchConfig,

    pub fn init(channels: u32, sample_rate: u32, q: f64, frequency: f64) NotchNodeConfig {
        var config: NotchNodeConfig = undefined;
        zaudioNotchNodeConfigInit(channels, sample_rate, q, frequency, &config);
        return config;
    }
    extern fn zaudioNotchNodeConfigInit(
        channels: u32,
        sample_rate: u32,
        q: f64,
        frequency: f64,
        out_config: *NotchNodeConfig,
    ) void;
};

const NotchNodeImpl = opaque {
    pub usingnamespace NodeImpl.Methods(NotchNode);

    pub const destroy = zaudioNotchNodeDestroy;
    extern fn zaudioNotchNodeDestroy(handle: NotchNode) void;

    pub fn reconfigure(handle: NotchNode, config: NotchConfig) Error!void {
        try maybeError(ma_notch_node_reinit(&config, handle));
    }
    extern fn ma_notch_node_reinit(config: *const NotchConfig, handle: NotchNode) Result;
};
//--------------------------------------------------------------------------------------------------
//
// Peak Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const PeakNode = *align(@sizeOf(usize)) PeakNodeImpl;

pub const PeakConfig = extern struct {
    format: Format,
    channels: u32,
    sample_rate: u32,
    gain_db: f64,
    q: f64,
    frequency: f64,
};

pub const PeakNodeConfig = extern struct {
    node_config: NodeConfig,
    peak: PeakConfig,

    pub fn init(channels: u32, sample_rate: u32, gain_db: f64, q: f64, frequency: f64) PeakNodeConfig {
        var config: PeakNodeConfig = undefined;
        zaudioPeakNodeConfigInit(channels, sample_rate, gain_db, q, frequency, &config);
        return config;
    }
    extern fn zaudioPeakNodeConfigInit(
        channels: u32,
        sample_rate: u32,
        gain_db: f64,
        q: f64,
        frequency: f64,
        out_config: *PeakNodeConfig,
    ) void;
};

const PeakNodeImpl = opaque {
    pub usingnamespace NodeImpl.Methods(PeakNode);

    pub const destroy = zaudioPeakNodeDestroy;
    extern fn zaudioPeakNodeDestroy(handle: PeakNode) void;

    pub fn reconfigure(handle: PeakNode, config: PeakConfig) Error!void {
        try maybeError(ma_peak_node_reinit(&config, handle));
    }
    extern fn ma_peak_node_reinit(config: *const PeakConfig, handle: PeakNode) Result;
};
//--------------------------------------------------------------------------------------------------
//
// Low Shelf Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const LoshelfNode = *align(@sizeOf(usize)) LoshelfNodeImpl;

pub const LoshelfConfig = extern struct {
    format: Format,
    channels: u32,
    sample_rate: u32,
    gain_db: f64,
    shelf_slope: f64,
    frequency: f64,
};

pub const LoshelfNodeConfig = extern struct {
    node_config: NodeConfig,
    loshelf: LoshelfConfig,

    pub fn init(
        channels: u32,
        sample_rate: u32,
        gain_db: f64,
        shelf_slope: f64,
        frequency: f64,
    ) LoshelfNodeConfig {
        var config: LoshelfNodeConfig = undefined;
        zaudioLoshelfNodeConfigInit(channels, sample_rate, gain_db, shelf_slope, frequency, &config);
        return config;
    }
    extern fn zaudioLoshelfNodeConfigInit(
        channels: u32,
        sample_rate: u32,
        gain_db: f64,
        shelf_slope: f64,
        frequency: f64,
        out_config: *LoshelfNodeConfig,
    ) void;
};

const LoshelfNodeImpl = opaque {
    pub usingnamespace NodeImpl.Methods(LoshelfNode);

    pub const destroy = zaudioLoshelfNodeDestroy;
    extern fn zaudioLoshelfNodeDestroy(handle: LoshelfNode) void;

    pub fn reconfigure(handle: LoshelfNode, config: LoshelfConfig) Error!void {
        try maybeError(ma_loshelf_node_reinit(&config, handle));
    }
    extern fn ma_loshelf_node_reinit(config: *const LoshelfConfig, handle: LoshelfNode) Result;
};
//--------------------------------------------------------------------------------------------------
//
// High Shelf Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const HishelfNode = *align(@sizeOf(usize)) HishelfNodeImpl;

pub const HishelfConfig = extern struct {
    format: Format,
    channels: u32,
    sample_rate: u32,
    gain_db: f64,
    shelf_slope: f64,
    frequency: f64,
};

pub const HishelfNodeConfig = extern struct {
    node_config: NodeConfig,
    hishelf: HishelfConfig,

    pub fn init(
        channels: u32,
        sample_rate: u32,
        gain_db: f64,
        shelf_slope: f64,
        frequency: f64,
    ) HishelfNodeConfig {
        var config: HishelfNodeConfig = undefined;
        zaudioHishelfNodeConfigInit(channels, sample_rate, gain_db, shelf_slope, frequency, &config);
        return config;
    }
    extern fn zaudioHishelfNodeConfigInit(
        channels: u32,
        sample_rate: u32,
        gain_db: f64,
        shelf_slope: f64,
        frequency: f64,
        out_config: *HishelfNodeConfig,
    ) void;
};

const HishelfNodeImpl = opaque {
    pub usingnamespace NodeImpl.Methods(HishelfNode);

    pub const destroy = zaudioHishelfNodeDestroy;
    extern fn zaudioHishelfNodeDestroy(handle: HishelfNode) void;

    pub fn reconfigure(handle: HishelfNode, config: HishelfConfig) Error!void {
        try maybeError(ma_hishelf_node_reinit(&config, handle));
    }
    extern fn ma_hishelf_node_reinit(config: *const HishelfConfig, handle: HishelfNode) Result;
};
//--------------------------------------------------------------------------------------------------
//
// Delay Filter Node
//
//--------------------------------------------------------------------------------------------------
pub const DelayNode = *align(@sizeOf(usize)) DelayNodeImpl;

pub const DelayConfig = extern struct {
    channels: u32,
    sample_rate: u32,
    delay_in_frames: u32,
    delay_start: Bool32,
    wet: f32,
    dry: f32,
    decay: f32,
};

pub const DelayNodeConfig = extern struct {
    node_config: NodeConfig,
    delay: DelayConfig,

    pub fn init(channels: u32, sample_rate: u32, delay_in_frames: u32, decay: f32) DelayNodeConfig {
        var config: DelayNodeConfig = undefined;
        zaudioDelayNodeConfigInit(channels, sample_rate, delay_in_frames, decay, &config);
        return config;
    }
    extern fn zaudioDelayNodeConfigInit(
        channels: u32,
        sample_rate: u32,
        delay_in_frames: u32,
        decay: f32,
        out_config: *DelayNodeConfig,
    ) void;
};

const DelayNodeImpl = opaque {
    pub usingnamespace NodeImpl.Methods(DelayNode);

    pub const destroy = zaudioDelayNodeDestroy;
    extern fn zaudioDelayNodeDestroy(handle: DelayNode) void;

    pub const setWet = ma_delay_node_set_wet;
    extern fn ma_delay_node_set_wet(handle: DelayNode, value: f32) void;

    pub const getWet = ma_delay_node_get_wet;
    extern fn ma_delay_node_get_wet(handle: DelayNode) f32;

    pub const setDry = ma_delay_node_set_dry;
    extern fn ma_delay_node_set_dry(handle: DelayNode, value: f32) void;

    pub const getDry = ma_delay_node_get_dry;
    extern fn ma_delay_node_get_dry(handle: DelayNode) f32;

    pub const setDecay = ma_delay_node_set_decay;
    extern fn ma_delay_node_set_decay(handle: DelayNode, value: f32) void;

    pub const getDecay = ma_delay_node_get_decay;
    extern fn ma_delay_node_get_decay(handle: DelayNode) f32;
};
//--------------------------------------------------------------------------------------------------
//
// Node Graph
//
//--------------------------------------------------------------------------------------------------
pub const NodeGraph = *align(@sizeOf(usize)) NodeGraphImpl;

pub fn createNodeGraph(config: NodeGraphConfig) Error!NodeGraph {
    var handle: ?NodeGraph = null;
    try maybeError(zaudioNodeGraphCreate(&config, &handle));
    return handle.?;
}
extern fn zaudioNodeGraphCreate(config: *const NodeGraphConfig, out_handle: ?*?*NodeGraphImpl) Result;

pub const NodeGraphConfig = extern struct {
    channels: u32,
    node_cache_cap_in_frames: u16,

    pub fn init(channels: u32) NodeGraphConfig {
        var config: NodeGraphConfig = undefined;
        zaudioNodeGraphConfigInit(channels, &config);
        return config;
    }
    extern fn zaudioNodeGraphConfigInit(channels: u32, out_config: *NodeGraphConfig) void;
};

const NodeGraphImpl = opaque {
    pub usingnamespace NodeImpl.Methods(NodeGraph);
    pub usingnamespace NodeGraphImpl.Methods(NodeGraph);

    pub const destroy = zaudioNodeGraphDestroy;
    extern fn zaudioNodeGraphDestroy(handle: NodeGraph) void;

    fn Methods(comptime T: type) type {
        return struct {
            pub fn asNodeGraph(handle: T) NodeGraph {
                return @ptrCast(NodeGraph, handle);
            }

            pub fn createDataSourceNode(node_graph: T, config: DataSourceNodeConfig) Error!DataSourceNode {
                var handle: ?DataSourceNode = null;
                try maybeError(zaudioDataSourceNodeCreate(node_graph.asNodeGraph(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioDataSourceNodeCreate(
                node_graph: NodeGraph,
                config: *const DataSourceNodeConfig,
                out_handle: ?*?*DataSourceNodeImpl,
            ) Result;

            pub fn createBiquadNode(node_graph: T, config: BiquadNodeConfig) Error!BiquadNode {
                var handle: ?BiquadNode = null;
                try maybeError(zaudioBiquadNodeCreate(node_graph.asNodeGraph(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioBiquadNodeCreate(
                node_graph: NodeGraph,
                config: *const BiquadNodeConfig,
                out_handle: ?*?*BiquadNodeImpl,
            ) Result;

            pub fn createLpfNode(node_graph: T, config: LpfNodeConfig) Error!LpfNode {
                var handle: ?LpfNode = null;
                try maybeError(zaudioLpfNodeCreate(node_graph.asNodeGraph(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioLpfNodeCreate(
                node_graph: NodeGraph,
                config: *const LpfNodeConfig,
                out_handle: ?*?*LpfNodeImpl,
            ) Result;

            pub fn createHpfNode(node_graph: T, config: HpfNodeConfig) Error!HpfNode {
                var handle: ?HpfNode = null;
                try maybeError(zaudioHpfNodeCreate(node_graph.asNodeGraph(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioHpfNodeCreate(
                node_graph: NodeGraph,
                config: *const HpfNodeConfig,
                out_handle: ?*?*HpfNodeImpl,
            ) Result;

            pub fn createSplitterNode(node_graph: T, config: SplitterNodeConfig) Error!SplitterNode {
                var handle: ?SplitterNode = null;
                try maybeError(zaudioSplitterNodeCreate(node_graph.asNodeGraph(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioSplitterNodeCreate(
                node_graph: NodeGraph,
                config: *const SplitterNodeConfig,
                out_handle: ?*?*SplitterNodeImpl,
            ) Result;

            pub fn createNotchNode(node_graph: T, config: NotchNodeConfig) Error!NotchNode {
                var handle: ?NotchNode = null;
                try maybeError(zaudioNotchNodeCreate(node_graph.asNodeGraph(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioNotchNodeCreate(
                node_graph: NodeGraph,
                config: *const NotchNodeConfig,
                out_handle: ?*?*NotchNodeImpl,
            ) Result;

            pub fn createPeakNode(node_graph: T, config: PeakNodeConfig) Error!PeakNode {
                var handle: ?PeakNode = null;
                try maybeError(zaudioPeakNodeCreate(node_graph.asNodeGraph(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioPeakNodeCreate(
                node_graph: NodeGraph,
                config: *const PeakNodeConfig,
                out_handle: ?*?*PeakNodeImpl,
            ) Result;

            pub fn createLoshelfNode(node_graph: T, config: LoshelfNodeConfig) Error!LoshelfNode {
                var handle: ?LoshelfNode = null;
                try maybeError(zaudioLoshelfNodeCreate(node_graph.asNodeGraph(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioLoshelfNodeCreate(
                node_graph: NodeGraph,
                config: *const LoshelfNodeConfig,
                out_handle: ?*?*LoshelfNodeImpl,
            ) Result;

            pub fn createHishelfNode(node_graph: T, config: HishelfNodeConfig) Error!HishelfNode {
                var handle: ?HishelfNode = null;
                try maybeError(zaudioHishelfNodeCreate(node_graph.asNodeGraph(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioHishelfNodeCreate(
                node_graph: NodeGraph,
                config: *const HishelfNodeConfig,
                out_handle: ?*?*HishelfNodeImpl,
            ) Result;

            pub fn createDelayNode(node_graph: T, config: DelayNodeConfig) Error!DelayNode {
                var handle: ?DelayNode = null;
                try maybeError(zaudioDelayNodeCreate(node_graph.asNodeGraph(), &config, &handle));
                return handle.?;
            }
            extern fn zaudioDelayNodeCreate(
                node_graph: NodeGraph,
                config: *const DelayNodeConfig,
                out_handle: ?*?*DelayNodeImpl,
            ) Result;

            pub fn getEndpoint(handle: T) Node {
                return ma_node_graph_get_endpoint(handle.asNodeGraph());
            }
            extern fn ma_node_graph_get_endpoint(handle: NodeGraph) Node;

            pub fn getChannels(handle: T) u32 {
                return ma_node_graph_get_channels(handle.asNodeGraph());
            }
            extern fn ma_node_graph_get_channels(handle: NodeGraph) u32;

            pub fn readPcmFrames(
                node_graph: T,
                frames_out: *anyopaque,
                frame_count: u64,
                frames_read: ?*u64,
            ) Error!void {
                try maybeError(ma_node_graph_read_pcm_frames(
                    node_graph.asNodeGraph(),
                    frames_out,
                    frame_count,
                    frames_read,
                ));
            }
            extern fn ma_node_graph_read_pcm_frames(
                handle: NodeGraph,
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
pub const Device = *align(@sizeOf(usize)) DeviceImpl;

pub const DeviceType = enum(u32) {
    playback = 1,
    capture = 2,
    duplex = 3,
    loopback = 4,
};

pub const PerformanceProfile = enum(u32) {
    low_latency,
    conservative,
};

pub const DeviceDataProc = *const fn (
    device: Device,
    output: ?*anyopaque,
    input: ?*const anyopaque,
    frame_count: u32,
) callconv(.C) void;

pub const DeviceNotificationProc = *const fn (
    *const anyopaque, // TODO: `*const ma_device_notification`
) callconv(.C) void;

pub const StopProc = *const fn (device: Device) callconv(.C) void;

pub const ResampleAlgorithm = enum(u32) {
    linear,
    custom,
};

pub const DeviceId = extern union {
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
    announcement,
    emergency,
    safety,
    vehicle_status,
    alarm,
    assistance_accessibility,
    assistance_navigation_guidance,
    assistance_sonification,
    assitant,
    game,
    media,
    notification,
    notification_event,
    notification_ringtone,
    voice_communication,
    voice_communication_signalling,
};

pub const AaudioContentType = enum(u32) {
    default,
    movie,
    music,
    sonification,
    speech,
};

pub const AaudioInputPreset = enum(u32) {
    default,
    generic,
    camcorder,
    unprocessed,
    voice_recognition,
    voice_communication,
    voice_performance,
};

pub const DeviceConfig = extern struct {
    device_type: DeviceType,
    sample_rate: u32,
    period_size_in_frames: u32,
    period_size_in_milliseconds: u32,
    periods: u32,
    performance_profile: PerformanceProfile,
    no_pre_silenced_output_buffer: bool,
    no_clip: bool,
    no_disable_denormals: bool,
    no_fixed_sized_callback: bool,
    data_callback: ?DeviceDataProc,
    notification_callback: ?DeviceNotificationProc,
    stop_callback: ?StopProc,
    user_data: ?*anyopaque,
    resampling: extern struct {
        format: Format,
        channels: u32,
        sample_rate_in: u32,
        sample_rate_out: u32,
        algorithm: ResampleAlgorithm,
        backend_vtable: ?*anyopaque, // TODO: `*ma_resampling_backend_vtable` (custom resamplers)
        backend_user_data: ?*anyopaque,
        linear: extern struct {
            lpf_order: u32,
        },
    },
    playback: extern struct {
        device_id: ?*const DeviceId,
        format: Format,
        channels: u32,
        channel_map: [*]Channel,
        channel_mix_mode: ChannelMixMode,
        share_mode: ShareMode,
    },
    capture: extern struct {
        device_id: ?*const DeviceId,
        format: Format,
        channels: u32,
        channel_map: [*]Channel,
        channel_mix_mode: ChannelMixMode,
        share_mode: ShareMode,
    },
    wasapi: extern struct {
        no_auto_convert_src: bool,
        no_default_quality_src: bool,
        no_auto_stream_routing: bool,
        no_hardware_offloading: bool,
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

    // Our additional data (not present in miniaudio)
    playback_callback: PlaybackCallback,
    capture_callback: CaptureCallback,

    pub fn init(device_type: DeviceType) DeviceConfig {
        var config: DeviceConfig = undefined;
        zaudioDeviceConfigInit(device_type, &config);
        config.playback_callback = .{};
        config.capture_callback = .{};
        return config;
    }
    extern fn zaudioDeviceConfigInit(device_type: DeviceType, out_config: *DeviceConfig) void;
};

pub fn createDevice(allocator: std.mem.Allocator, context: ?Context, config: *DeviceConfig) Error!Device {
    return DeviceImpl.create(allocator, context, config);
}

const DeviceImpl = opaque {
    const InternalState = struct {
        playback_callback: PlaybackCallback = .{},
        capture_callback: CaptureCallback = .{},
    };

    fn internalDataCallback(
        device: Device,
        outptr: ?*anyopaque,
        inptr: ?*const anyopaque,
        num_frames: u32,
    ) callconv(.C) void {
        const user_data = zaudioDeviceGetUserData(device);
        const internal_state = @ptrCast(
            *InternalState,
            @alignCast(@alignOf(InternalState), user_data),
        );

        if (num_frames > 0) {
            // Dispatch playback callback.
            if (outptr != null) if (internal_state.playback_callback.callback) |func| {
                func(internal_state.playback_callback.context, outptr.?, num_frames);
            };

            // Dispatch capture callback.
            if (inptr != null) if (internal_state.capture_callback.callback) |func| {
                func(internal_state.capture_callback.context, inptr.?, num_frames);
            };
        }
    }

    fn create(allocator: std.mem.Allocator, context: ?Context, config: *DeviceConfig) Error!Device {
        // We don't allow setting below fields (we use them internally), please use
        // `config.playback_callback` and/or `config.capture_callback` instead.
        assert(config.data_callback == null);
        assert(config.user_data == null);

        const internal_state = allocator.create(InternalState) catch return error.OutOfMemory;
        errdefer allocator.destroy(internal_state);

        internal_state.* = .{
            .playback_callback = config.playback_callback,
            .capture_callback = config.capture_callback,
        };

        config.user_data = internal_state;

        if (config.playback_callback.callback != null or config.capture_callback.callback != null) {
            config.data_callback = internalDataCallback;
        }

        var handle: ?Device = null;
        try maybeError(zaudioDeviceCreate(if (context) |ctx| ctx else null, config, &handle));
        return handle.?;
    }
    extern fn zaudioDeviceCreate(
        context: ?Context,
        config: *const DeviceConfig,
        out_handle: ?*?*DeviceImpl,
    ) Result;

    pub fn destroy(device: Device, allocator: std.mem.Allocator) void {
        const user_data = zaudioDeviceGetUserData(device);
        allocator.destroy(@ptrCast(
            *InternalState,
            @alignCast(@alignOf(InternalState), user_data),
        ));
        zaudioDeviceDestroy(device);
    }
    extern fn zaudioDeviceDestroy(device: Device) void;
    extern fn zaudioDeviceGetUserData(device: Device) ?*anyopaque;

    pub fn asRaw(device: Device) *c.ma_device {
        return @ptrCast(*c.ma_device, device);
    }

    pub const getContext = ma_device_get_context;
    extern fn ma_device_get_context(device: Device) Context;

    pub const getLog = ma_device_get_log;
    extern fn ma_device_get_log(device: Device) ?Log;

    pub fn start(device: Device) Error!void {
        try maybeError(ma_device_start(device));
    }
    extern fn ma_device_start(device: Device) Result;

    pub fn stop(device: Device) Error!void {
        try maybeError(ma_device_stop(device));
    }
    extern fn ma_device_stop(device: Device) Result;

    pub fn isStarted(device: Device) bool {
        return ma_device_is_started(device) != 0;
    }
    extern fn ma_device_is_started(device: Device) Bool32;

    pub const getState = ma_device_get_state;
    extern fn ma_device_get_state(device: Device) DeviceState;

    pub fn setMasterVolume(device: Device, volume: f32) Error!void {
        try maybeError(ma_device_set_master_volume(device, volume));
    }
    extern fn ma_device_set_master_volume(device: Device, volume: f32) Result;

    pub fn getMasterVolume(device: Device) Error!f32 {
        var volume: f32 = undefined;
        try maybeError(ma_device_get_master_volume(device, &volume));
        return volume;
    }
    extern fn ma_device_get_master_volume(device: Device, volume: *f32) Result;
};
//--------------------------------------------------------------------------------------------------
//
// Engine
//
//--------------------------------------------------------------------------------------------------
pub const EngineConfig = struct {
    raw: c.ma_engine_config,

    pub fn init() EngineConfig {
        return .{ .raw = c.ma_engine_config_init() };
    }
};

pub fn createEngine(allocator: std.mem.Allocator, config: ?EngineConfig) Error!Engine {
    var handle = allocator.create(c.ma_engine) catch return error.OutOfMemory;
    errdefer allocator.destroy(handle);
    try checkResult(c.ma_engine_init(if (config) |conf| &conf.raw else null, handle));
    return @ptrCast(Engine, handle);
}

pub const Engine = *align(@sizeOf(usize)) EngineImpl;
const EngineImpl = opaque {
    pub usingnamespace NodeImpl.Methods(Engine);
    pub usingnamespace NodeGraphImpl.Methods(Engine);

    pub fn destroy(engine: Engine, allocator: std.mem.Allocator) void {
        const raw = engine.asRaw();
        c.ma_engine_uninit(raw);
        allocator.destroy(raw);
    }

    pub fn asRaw(engine: Engine) *c.ma_engine {
        return @ptrCast(*c.ma_engine, engine);
    }

    pub fn createSoundFromFile(
        engine: Engine,
        allocator: std.mem.Allocator,
        filepath: [:0]const u8,
        args: struct {
            flags: SoundFlags = .{},
            sgroup: ?SoundGroup = null,
            done_fence: ?Fence = null,
        },
    ) Error!Sound {
        return SoundImpl.createFromFile(allocator, engine, filepath, args.flags, args.sgroup, args.done_fence);
    }

    pub fn createSoundFromDataSource(
        engine: Engine,
        allocator: std.mem.Allocator,
        data_source: DataSource,
        flags: SoundFlags,
        sgroup: ?SoundGroup,
    ) Error!Sound {
        return SoundImpl.createFromDataSource(allocator, engine, data_source, flags, sgroup);
    }

    pub fn createSound(
        engine: Engine,
        allocator: std.mem.Allocator,
        config: SoundConfig,
    ) Error!Sound {
        return SoundImpl.create(allocator, engine, config);
    }

    pub fn createSoundCopy(
        engine: Engine,
        allocator: std.mem.Allocator,
        existing_sound: Sound,
        flags: SoundFlags,
        sgroup: ?SoundGroup,
    ) Error!Sound {
        return SoundImpl.createCopy(allocator, engine, existing_sound, flags, sgroup);
    }

    pub fn createSoundGroup(
        engine: Engine,
        allocator: std.mem.Allocator,
        flags: SoundFlags,
        parent: ?SoundGroup,
    ) Error!SoundGroup {
        return SoundGroupImpl.create(allocator, engine, flags, parent);
    }

    pub fn getResourceManager(engine: Engine) ResourceManager {
        return @ptrCast(ResourceManager, c.ma_engine_get_resource_manager(engine.asRaw()));
    }

    pub fn getDevice(engine: Engine) ?Device {
        return @ptrCast(?Device, c.ma_engine_get_device(engine.asRaw()));
    }

    pub fn getLog(engine: Engine) ?Log {
        return @ptrCast(?Log, c.ma_engine_get_log(engine.asRaw()));
    }

    pub fn getSampleRate(engine: Engine) u32 {
        return c.ma_engine_get_sample_rate(engine.asRaw());
    }

    pub fn start(engine: Engine) Error!void {
        try checkResult(c.ma_engine_start(engine.asRaw()));
    }
    pub fn stop(engine: Engine) Error!void {
        try checkResult(c.ma_engine_stop(engine.asRaw()));
    }

    pub fn setVolume(engine: Engine, volume: f32) Error!void {
        try checkResult(c.ma_engine_set_volume(engine.asRaw(), volume));
    }

    pub fn setGainDb(engine: Engine, gain_db: f32) Error!void {
        try checkResult(c.ma_engine_set_gain_db(engine.asRaw(), gain_db));
    }

    pub fn getNumListeners(engine: Engine) u32 {
        return c.ma_engine_get_listener_count(engine.asRaw());
    }

    pub fn findClosestListener(engine: Engine, absolute_pos_xyz: [3]f32) u32 {
        return c.ma_engine_find_closest_listener(
            engine.asRaw(),
            absolute_pos_xyz[0],
            absolute_pos_xyz[1],
            absolute_pos_xyz[2],
        );
    }

    pub fn setListenerPosition(engine: Engine, index: u32, v: [3]f32) void {
        c.ma_engine_listener_set_position(engine.asRaw(), index, v[0], v[1], v[2]);
    }
    pub fn getListenerPosition(engine: Engine, index: u32) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_engine_listener_get_position(engine.asRaw(), index, &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_engine_listener_get_position(engine: *c.ma_engine, index: u32, vout: *c.ma_vec3f) void;

    pub fn setListenerDirection(engine: Engine, index: u32, v: [3]f32) void {
        c.ma_engine_listener_set_direction(engine.asRaw(), index, v[0], v[1], v[2]);
    }
    pub fn getListenerDirection(engine: Engine, index: u32) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_engine_listener_get_direction(engine.asRaw(), index, &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_engine_listener_get_direction(engine: *c.ma_engine, index: u32, vout: *c.ma_vec3f) void;

    pub fn setListenerVelocity(engine: Engine, index: u32, v: [3]f32) void {
        c.ma_engine_listener_set_velocity(engine.asRaw(), index, v[0], v[1], v[2]);
    }
    pub fn getListenerVelocity(engine: Engine, index: u32) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_engine_listener_get_velocity(engine.asRaw(), index, &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_engine_listener_get_velocity(engine: *c.ma_engine, index: u32, vout: *c.ma_vec3f) void;

    pub fn setListenerWorldUp(engine: Engine, index: u32, v: [3]f32) void {
        c.ma_engine_listener_set_world_up(engine.asRaw(), index, v[0], v[1], v[2]);
    }
    pub fn getListenerWorldUp(engine: Engine, index: u32) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_engine_listener_get_world_up(engine.asRaw(), index, &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_engine_listener_get_world_up(engine: *c.ma_engine, index: u32, vout: *c.ma_vec3f) void;

    pub fn setListenerEnabled(engine: Engine, index: u32, enabled: bool) void {
        c.ma_engine_listener_set_enabled(engine.asRaw(), index, if (enabled) c.MA_TRUE else c.MA_FALSE);
    }
    pub fn isListenerEnabled(engine: Engine, index: u32) bool {
        return c.ma_engine_listener_is_enabled(engine.asRaw(), index) == c.MA_TRUE;
    }

    pub fn setListenerCone(
        engine: Engine,
        index: u32,
        inner_radians: f32,
        outer_radians: f32,
        outer_gain: f32,
    ) void {
        c.ma_engine_listener_set_cone(engine.asRaw(), index, inner_radians, outer_radians, outer_gain);
    }
    pub fn getListenerCone(
        engine: Engine,
        index: u32,
        inner_radians: ?*f32,
        outer_radians: ?*f32,
        outer_gain: ?*f32,
    ) void {
        c.ma_engine_listener_get_cone(engine.asRaw(), index, inner_radians, outer_radians, outer_gain);
    }

    pub fn playSound(engine: Engine, filepath: [:0]const u8, sgroup: ?SoundGroup) Error!void {
        try checkResult(c.ma_engine_play_sound(
            engine.asRaw(),
            filepath.ptr,
            if (sgroup) |g| g.asRaw() else null,
        ));
    }

    pub fn playSoundEx(
        engine: Engine,
        filepath: [:0]const u8,
        node: ?Node,
        node_input_bus_index: u32,
    ) Error!void {
        try checkResult(c.ma_engine_play_sound_ex(
            engine.asRaw(),
            filepath.ptr,
            if (node) |n| n.asRawNode() else null,
            node_input_bus_index,
        ));
    }
};
//--------------------------------------------------------------------------------------------------
//
// Sound
//
//--------------------------------------------------------------------------------------------------
pub const SoundConfig = struct {
    raw: c.ma_sound_config,

    pub fn init() SoundConfig {
        return .{ .raw = c.ma_sound_config_init() };
    }
};

pub const Sound = *align(@sizeOf(usize)) SoundImpl;
const SoundImpl = opaque {
    pub usingnamespace NodeImpl.Methods(Sound);

    fn createFromFile(
        allocator: std.mem.Allocator,
        engine: Engine,
        filepath: [:0]const u8,
        flags: SoundFlags,
        sgroup: ?SoundGroup,
        done_fence: ?Fence,
    ) Error!Sound {
        var handle = allocator.create(c.ma_sound) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_sound_init_from_file(
            engine.asRaw(),
            filepath.ptr,
            @bitCast(u32, flags),
            if (sgroup) |g| g.asRaw() else null,
            if (done_fence) |f| f.asRaw() else null,
            handle,
        ));

        return @ptrCast(Sound, handle);
    }

    fn createFromDataSource(
        allocator: std.mem.Allocator,
        engine: Engine,
        data_source: DataSource,
        flags: SoundFlags,
        sgroup: ?SoundGroup,
    ) Error!Sound {
        var handle = allocator.create(c.ma_sound) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_sound_init_from_data_source(
            engine.asRaw(),
            data_source.asRaw(),
            @bitCast(u32, flags),
            if (sgroup) |g| g.handle else null,
            handle,
        ));

        return @ptrCast(Sound, handle);
    }

    fn createCopy(
        allocator: std.mem.Allocator,
        engine: Engine,
        existing_sound: Sound,
        flags: SoundFlags,
        sgroup: ?SoundGroup,
    ) Error!Sound {
        var handle = allocator.create(c.ma_sound) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_sound_init_copy(
            engine.asRaw(),
            existing_sound.asRaw(),
            @bitCast(u32, flags),
            if (sgroup) |g| g.asRaw() else null,
            handle,
        ));

        return @ptrCast(Sound, handle);
    }

    fn create(
        allocator: std.mem.Allocator,
        engine: Engine,
        config: SoundConfig,
    ) Error!Sound {
        var handle = allocator.create(c.ma_sound) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_sound_init_ex(engine.asRaw(), &config.raw, handle));

        return @ptrCast(Sound, handle);
    }

    pub fn destroy(sound: Sound, allocator: std.mem.Allocator) void {
        c.ma_sound_uninit(sound.asRaw());
        allocator.destroy(sound.asRaw());
    }

    pub fn asRaw(sound: Sound) *c.ma_sound {
        return @ptrCast(*c.ma_sound, sound);
    }

    pub fn getEngine(sound: Sound) Engine {
        return @ptrCast(Engine, c.ma_sound_get_engine(sound.asRaw()));
    }

    pub fn getDataSource(sound: Sound) ?DataSource {
        return @ptrCast(?DataSource, c.ma_sound_get_data_source(sound.asRaw()));
    }

    pub fn start(sound: Sound) Error!void {
        try checkResult(c.ma_sound_start(sound.asRaw()));
    }
    pub fn stop(sound: Sound) Error!void {
        try checkResult(c.ma_sound_stop(sound.asRaw()));
    }

    pub fn setVolume(sound: Sound, volume: f32) void {
        c.ma_sound_set_volume(sound.asRaw(), volume);
    }
    pub fn getVolume(sound: Sound) f32 {
        return c.ma_sound_get_volume(sound.asRaw());
    }

    pub fn setPan(sound: Sound, pan: f32) void {
        c.ma_sound_set_pan(sound.asRaw(), pan);
    }
    pub fn getPan(sound: Sound) f32 {
        return c.ma_sound_get_pan(sound.asRaw());
    }

    pub fn setPanMode(sound: Sound, pan_mode: PanMode) void {
        c.ma_sound_set_pan_mode(sound.asRaw(), @enumToInt(pan_mode));
    }
    pub fn getPanMode(sound: Sound) PanMode {
        return @intToEnum(PanMode, c.ma_sound_get_pan_mode(sound.asRaw()));
    }

    pub fn setPitch(sound: Sound, pitch: f32) void {
        c.ma_sound_set_pitch(sound.asRaw(), pitch);
    }
    pub fn getPitch(sound: Sound) f32 {
        return c.ma_sound_get_pitch(sound.asRaw());
    }

    pub fn setSpatializationEnabled(sound: Sound, enabled: bool) void {
        c.ma_sound_set_spatialization_enabled(sound.asRaw(), if (enabled) c.MA_TRUE else c.MA_FALSE);
    }
    pub fn isSpatializationEnabled(sound: Sound) bool {
        return c.ma_sound_is_spatialization_enabled(sound.asRaw()) == c.MA_TRUE;
    }

    pub fn setPinnedListenerIndex(sound: Sound, index: u32) void {
        c.ma_sound_set_pinned_listener_index(sound.asRaw(), index);
    }
    pub fn getPinnedListenerIndex(sound: Sound) u32 {
        return c.ma_sound_get_pinned_listener_index(sound.asRaw());
    }
    pub fn getListenerIndex(sound: Sound) u32 {
        return c.ma_sound_get_listener_index(sound.asRaw());
    }

    pub fn getDirectionToListener(sound: Sound) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_get_direction_to_listener(sound.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_get_direction_to_listener(sound: *c.ma_sound, vout: *c.ma_vec3f) void;

    pub fn setPosition(sound: Sound, v: [3]f32) void {
        c.ma_sound_set_position(sound.asRaw(), v[0], v[1], v[2]);
    }
    pub fn getPosition(sound: Sound) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_get_position(sound.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_get_position(sound: *c.ma_sound, vout: *c.ma_vec3f) void;

    pub fn setDirection(sound: Sound, v: [3]f32) void {
        c.ma_sound_set_direction(sound.asRaw(), v[0], v[1], v[2]);
    }
    pub fn getDirection(sound: Sound) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_get_direction(sound.asRaw());
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_get_direction(sound: *c.ma_sound, vout: *c.ma_vec3f) void;

    pub fn setVelocity(sound: Sound, v: [3]f32) void {
        c.ma_sound_set_velocity(sound.asRaw(), v[0], v[1], v[2]);
    }
    pub fn getVelocity(sound: Sound) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_get_velocity(sound.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_get_velocity(sound: *c.ma_sound, vout: *c.ma_vec3f) void;

    pub fn setAttenuationModel(sound: Sound, model: AttenuationModel) void {
        c.ma_sound_set_attenuation_model(sound.asRaw(), @enumToInt(model));
    }
    pub fn getAttenuationModel(sound: Sound) AttenuationModel {
        return @intToEnum(AttenuationModel, c.ma_sound_get_attenuation_model(sound.asRaw()));
    }

    pub fn setPositioning(sound: Sound, pos: Positioning) void {
        c.ma_sound_set_positioning(sound.asRaw(), @enumToInt(pos));
    }
    pub fn getPositioning(sound: Sound) Positioning {
        return @intToEnum(Positioning, c.ma_sound_get_positioning(sound.asRaw()));
    }

    pub fn setRolloff(sound: Sound, rolloff: f32) void {
        c.ma_sound_set_rolloff(sound.asRaw(), rolloff);
    }
    pub fn getRolloff(sound: Sound) f32 {
        return c.ma_sound_get_rolloff(sound.asRaw());
    }

    pub fn setMinGain(sound: Sound, min_gain: f32) void {
        c.ma_sound_set_min_gain(sound.asRaw(), min_gain);
    }
    pub fn getMinGain(sound: Sound) f32 {
        return c.ma_sound_get_min_gain(sound.asRaw());
    }

    pub fn setMaxGain(sound: Sound, max_gain: f32) void {
        c.ma_sound_set_max_gain(sound.asRaw(), max_gain);
    }
    pub fn getMaxGain(sound: Sound) f32 {
        return c.ma_sound_get_max_gain(sound.asRaw());
    }

    pub fn setMinDistance(sound: Sound, min_distance: f32) void {
        c.ma_sound_set_min_distance(sound.asRaw(), min_distance);
    }
    pub fn getMinDistance(sound: Sound) f32 {
        return c.ma_sound_get_min_distance(sound.asRaw());
    }

    pub fn setMaxDistance(sound: Sound, max_distance: f32) void {
        c.ma_sound_set_max_distance(sound.asRaw(), max_distance);
    }
    pub fn getMaxDistance(sound: Sound) f32 {
        return c.ma_sound_get_max_distance(sound.asRaw());
    }

    pub fn setCone(sound: Sound, inner_radians: f32, outer_radians: f32, outer_gain: f32) void {
        c.ma_sound_set_cone(sound.asRaw(), inner_radians, outer_radians, outer_gain);
    }
    pub fn getCone(sound: Sound, inner_radians: ?*f32, outer_radians: ?*f32, outer_gain: ?*f32) void {
        c.ma_sound_get_cone(sound.asRaw(), inner_radians, outer_radians, outer_gain);
    }

    pub fn setDopplerFactor(sound: Sound, factor: f32) void {
        c.ma_sound_set_doppler_factor(sound.asRaw(), factor);
    }
    pub fn getDopplerFactor(sound: Sound) f32 {
        return c.ma_sound_get_doppler_factor(sound.asRaw());
    }

    pub fn setDirectionalAttenuationFactor(sound: Sound, factor: f32) void {
        c.ma_sound_set_directional_attenuation_factor(sound.asRaw(), factor);
    }
    pub fn getDirectionalAttenuationFactor(sound: Sound) f32 {
        return c.ma_sound_get_directional_attenuation_factor(sound.asRaw());
    }

    pub fn setFadePcmFrames(sound: Sound, volume_begin: f32, volume_end: f32, len_in_frames: u64) void {
        c.ma_sound_set_fade_in_pcm_frames(sound.asRaw(), volume_begin, volume_end, len_in_frames);
    }
    pub fn setFadeMilliseconds(sound: Sound, volume_begin: f32, volume_end: f32, len_in_ms: u64) void {
        c.ma_sound_set_fade_in_milliseconds(sound.asRaw(), volume_begin, volume_end, len_in_ms);
    }
    pub fn getCurrentFadeVolume(sound: Sound) f32 {
        return c.ma_sound_get_current_fade_volume(sound.asRaw());
    }

    pub fn setStartTimePcmFrames(sound: Sound, abs_global_time_in_frames: u64) void {
        c.ma_sound_set_start_time_in_pcm_frames(sound.asRaw(), abs_global_time_in_frames);
    }
    pub fn setStartTimeMilliseconds(sound: Sound, abs_global_time_in_ms: u64) void {
        c.ma_sound_set_start_time_in_milliseconds(sound.asRaw(), abs_global_time_in_ms);
    }

    pub fn setStopTimePcmFrames(sound: Sound, abs_global_time_in_frames: u64) void {
        c.ma_sound_set_stop_time_in_pcm_frames(sound.asRaw(), abs_global_time_in_frames);
    }
    pub fn setStopTimeMilliseconds(sound: Sound, abs_global_time_in_ms: u64) void {
        c.ma_sound_set_stop_time_in_milliseconds(sound.asRaw(), abs_global_time_in_ms);
    }

    pub fn isPlaying(sound: Sound) bool {
        return c.ma_sound_is_playing(sound.asRaw()) == c.MA_TRUE;
    }

    pub fn getTimePcmFrames(sound: Sound) u64 {
        return c.ma_sound_get_time_in_pcm_frames(sound.asRaw());
    }

    pub fn setLooping(sound: Sound, looping: bool) void {
        return c.ma_sound_set_looping(sound.asRaw(), if (looping) c.MA_TRUE else c.MA_FALSE);
    }
    pub fn isLooping(sound: Sound) bool {
        return c.ma_sound_is_looping(sound.asRaw()) == c.MA_TRUE;
    }

    pub fn isAtEnd(sound: Sound) bool {
        return c.ma_sound_at_end(sound.asRaw()) == c.MA_TRUE;
    }

    pub fn seekToPcmFrame(sound: Sound, frame: u64) Error!void {
        try checkResult(c.ma_sound_seek_to_pcm_frame(sound.asRaw(), frame));
    }

    pub fn getDataFormat(
        sound: Sound,
        format: ?*Format,
        num_channels: ?*u32,
        sample_rate: ?*u32,
        channel_map: ?[]Channel,
    ) Error!void {
        try checkResult(c.ma_sound_get_data_format(
            sound.asRaw(),
            if (format) |fmt| @ptrCast(*u32, fmt) else null,
            num_channels,
            sample_rate,
            if (channel_map) |chm| chm.ptr else null,
            if (channel_map) |chm| chm.len else 0,
        ));
    }

    pub fn getCursorPcmFrames(sound: Sound) Error!u64 {
        var cursor: u64 = undefined;
        try checkResult(c.ma_sound_get_cursor_in_pcm_frames(sound.asRaw(), &cursor));
        return cursor;
    }

    pub fn getLengthPcmFrames(sound: Sound) Error!u64 {
        var length: u64 = undefined;
        try checkResult(c.ma_sound_get_length_in_pcm_frames(sound.asRaw(), &length));
        return length;
    }

    pub fn getCursorSeconds(sound: Sound) Error!f32 {
        var cursor: f32 = undefined;
        try checkResult(c.ma_sound_get_cursor_in_seconds(sound.asRaw(), &cursor));
        return cursor;
    }

    pub fn getLengthSeconds(sound: Sound) Error!f32 {
        var length: f32 = undefined;
        try checkResult(c.ma_sound_get_length_in_seconds(sound.asRaw(), &length));
        return length;
    }
};
//--------------------------------------------------------------------------------------------------
//
// Sound Group
//
//--------------------------------------------------------------------------------------------------
pub const SoundGroup = *align(@sizeOf(usize)) SoundGroupImpl;
const SoundGroupImpl = opaque {
    pub usingnamespace NodeImpl.Methods(SoundGroup);

    fn create(
        allocator: std.mem.Allocator,
        engine: Engine,
        flags: SoundFlags,
        parent: ?SoundGroup,
    ) Error!SoundGroup {
        var handle = allocator.create(c.ma_sound_group) catch return error.OutOfMemory;
        errdefer allocator.destroy(handle);

        try checkResult(c.ma_sound_group_init(
            engine.asRaw(),
            @bitCast(u32, flags),
            if (parent) |p| p.asRaw() else null,
            handle,
        ));

        return @ptrCast(SoundGroup, handle);
    }

    pub fn destroy(sgroup: SoundGroup, allocator: std.mem.Allocator) void {
        c.ma_sound_group_uninit(sgroup.asRaw());
        allocator.destroy(sgroup.asRaw());
    }

    pub fn asRaw(sound: SoundGroup) *c.ma_sound_group {
        return @ptrCast(*c.ma_sound_group, sound);
    }

    pub fn getEngine(sgroup: SoundGroup) Engine {
        return @ptrCast(Engine, c.ma_sound_group_get_engine(sgroup.asRaw()));
    }

    pub fn start(sgroup: SoundGroup) Error!void {
        try checkResult(c.ma_sound_group_start(sgroup.asRaw()));
    }
    pub fn stop(sgroup: SoundGroup) Error!void {
        try checkResult(c.ma_sound_group_stop(sgroup.asRaw()));
    }

    pub fn setVolume(sgroup: SoundGroup, volume: f32) void {
        c.ma_sound_group_set_volume(sgroup.asRaw(), volume);
    }
    pub fn getVolume(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_volume(sgroup.asRaw());
    }

    pub fn setPan(sgroup: SoundGroup, pan: f32) void {
        c.ma_sound_group_set_pan(sgroup.asRaw(), pan);
    }
    pub fn getPan(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_pan(sgroup.asRaw());
    }

    pub fn setPanMode(sgroup: SoundGroup, pan_mode: PanMode) void {
        c.ma_sound_group_set_pan_mode(sgroup.asRaw(), pan_mode);
    }
    pub fn getPanMode(sgroup: SoundGroup) PanMode {
        return c.ma_sound_group_get_pan_mode(sgroup.asRaw());
    }

    pub fn setPitch(sgroup: SoundGroup, pitch: f32) void {
        c.ma_sound_group_set_pitch(sgroup.asRaw(), pitch);
    }
    pub fn getPitch(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_pitch(sgroup.asRaw());
    }

    pub fn setSpatializationEnabled(sgroup: SoundGroup, enabled: bool) void {
        c.ma_sound_group_set_spatialization_enabled(sgroup.asRaw(), if (enabled) c.MA_TRUE else c.MA_FALSE);
    }
    pub fn isSpatializationEnabled(sgroup: SoundGroup) bool {
        return c.ma_sound_group_is_spatialization_enabled(sgroup.asRaw()) == c.MA_TRUE;
    }

    pub fn setPinnedListenerIndex(sgroup: SoundGroup, index: u32) void {
        c.ma_sound_group_set_pinned_listener_index(sgroup.asRaw(), index);
    }
    pub fn getPinnedListenerIndex(sgroup: SoundGroup) u32 {
        return c.ma_sound_group_get_pinned_listener_index(sgroup.asRaw());
    }
    pub fn getListenerIndex(sgroup: SoundGroup) u32 {
        return c.ma_sound_group_get_listener_index(sgroup.asRaw());
    }

    pub fn getDirectionToListener(sgroup: SoundGroup) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_group_get_direction_to_listener(sgroup.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_group_get_direction_to_listener(sgroup: *c.ma_sound_group, vout: *c.ma_vec3f) void;

    pub fn setPosition(sgroup: SoundGroup, v: [3]f32) void {
        c.ma_sound_group_set_position(sgroup.asRaw(), v[0], v[1], v[2]);
    }
    pub fn getPosition(sgroup: SoundGroup) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_group_get_position(sgroup.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_group_get_position(sgroup: *c.ma_sound_group, vout: *c.ma_vec3f) void;

    pub fn setDirection(sgroup: SoundGroup, v: [3]f32) void {
        c.ma_sound_group_set_direction(sgroup.asRaw(), v[0], v[1], v[2]);
    }
    pub fn getDirection(sgroup: SoundGroup) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_group_get_direction(sgroup.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_group_get_direction(sgroup: *c.ma_sound_group, vout: *c.ma_vec3f) void;

    pub fn setVelocity(sgroup: SoundGroup, v: [3]f32) void {
        c.ma_sound_group_set_velocity(sgroup.asRaw(), v[0], v[1], v[2]);
    }
    pub fn getVelocity(sgroup: SoundGroup) [3]f32 {
        var v: c.ma_vec3f = undefined;
        WA_ma_sound_group_get_velocity(sgroup.asRaw(), &v);
        return .{ v.x, v.y, v.z };
    }
    extern fn WA_ma_sound_group_get_velocity(sgroup: *c.ma_sound_group, vout: *c.ma_vec3f) void;

    pub fn setAttenuationModel(sgroup: SoundGroup, model: AttenuationModel) void {
        c.ma_sound_group_set_attenuation_model(sgroup.asRaw(), model);
    }
    pub fn getAttenuationModel(sgroup: SoundGroup) AttenuationModel {
        return c.ma_sound_group_get_attenuation_model(sgroup.asRaw());
    }

    pub fn setPositioning(sgroup: SoundGroup, pos: Positioning) void {
        c.ma_sound_group_set_positioning(sgroup.asRaw(), pos);
    }
    pub fn getPositioning(sgroup: SoundGroup) Positioning {
        return c.ma_sound_group_get_positioning(sgroup.asRaw());
    }

    pub fn setRolloff(sgroup: SoundGroup, rolloff: f32) void {
        c.ma_sound_group_set_rolloff(sgroup.asRaw(), rolloff);
    }
    pub fn getRolloff(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_rolloff(sgroup.asRaw());
    }

    pub fn setMinGain(sgroup: SoundGroup, min_gain: f32) void {
        c.ma_sound_group_set_min_gain(sgroup.asRaw(), min_gain);
    }
    pub fn getMinGain(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_min_gain(sgroup.asRaw());
    }

    pub fn setMaxGain(sgroup: SoundGroup, max_gain: f32) void {
        c.ma_sound_group_set_max_gain(sgroup.asRaw(), max_gain);
    }
    pub fn getMaxGain(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_max_gain(sgroup.asRaw());
    }

    pub fn setMinDistance(sgroup: SoundGroup, min_distance: f32) void {
        c.ma_sound_group_set_min_distance(sgroup.asRaw(), min_distance);
    }
    pub fn getMinDistance(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_min_distance(sgroup.asRaw());
    }

    pub fn setMaxDistance(sgroup: SoundGroup, max_distance: f32) void {
        c.ma_sound_group_set_max_distance(sgroup.asRaw(), max_distance);
    }
    pub fn getMaxDistance(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_max_distance(sgroup.asRaw());
    }

    pub fn setCone(sgroup: SoundGroup, inner_radians: f32, outer_radians: f32, outer_gain: f32) void {
        c.ma_sound_group_set_cone(sgroup.asRaw(), inner_radians, outer_radians, outer_gain);
    }
    pub fn getCone(sgroup: SoundGroup, inner_radians: ?*f32, outer_radians: ?*f32, outer_gain: ?*f32) void {
        c.ma_sound_group_get_cone(sgroup.asRaw(), inner_radians, outer_radians, outer_gain);
    }

    pub fn setDopplerFactor(sgroup: SoundGroup, factor: f32) void {
        c.ma_sound_group_set_doppler_factor(sgroup.asRaw(), factor);
    }
    pub fn getDopplerFactor(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_doppler_factor(sgroup.asRaw());
    }

    pub fn setDirectionalAttenuationFactor(sgroup: SoundGroup, factor: f32) void {
        c.ma_sound_group_set_directional_attenuation_factor(sgroup.asRaw(), factor);
    }
    pub fn getDirectionalAttenuationFactor(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_directional_attenuation_factor(sgroup.asRaw());
    }

    pub fn setFadePcmFrames(sgroup: SoundGroup, volume_begin: f32, volume_end: f32, len_in_frames: u64) void {
        c.ma_sound_group_set_fade_in_pcm_frames(sgroup.asRaw(), volume_begin, volume_end, len_in_frames);
    }
    pub fn setFadeMilliseconds(sgroup: SoundGroup, volume_begin: f32, volume_end: f32, len_in_ms: u64) void {
        c.ma_sound_group_set_fade_in_milliseconds(sgroup.asRaw(), volume_begin, volume_end, len_in_ms);
    }
    pub fn getCurrentFadeVolume(sgroup: SoundGroup) f32 {
        return c.ma_sound_group_get_current_fade_volume(sgroup.asRaw());
    }

    pub fn setStartTimePcmFrames(sgroup: SoundGroup, abs_global_time_in_frames: u64) void {
        c.ma_sound_group_set_start_time_in_pcm_frames(sgroup.asRaw(), abs_global_time_in_frames);
    }
    pub fn setStartTimeMilliseconds(sgroup: SoundGroup, abs_global_time_in_ms: u64) void {
        c.ma_sound_group_set_start_time_in_milliseconds(sgroup.asRaw(), abs_global_time_in_ms);
    }

    pub fn setStopTimePcmFrames(sgroup: SoundGroup, abs_global_time_in_frames: u64) void {
        c.ma_sound_group_set_stop_time_in_pcm_frames(sgroup.asRaw(), abs_global_time_in_frames);
    }
    pub fn setStopTimeMilliseconds(sgroup: SoundGroup, abs_global_time_in_ms: u64) void {
        c.ma_sound_group_set_stop_time_in_milliseconds(sgroup.asRaw(), abs_global_time_in_ms);
    }

    pub fn isPlaying(sgroup: SoundGroup) bool {
        return c.ma_sound_group_is_playing(sgroup.asRaw()) == c.MA_TRUE;
    }

    pub fn getTimePcmFrames(sgroup: SoundGroup) u64 {
        return c.ma_sound_group_get_time_in_pcm_frames(sgroup.asRaw());
    }
};
//--------------------------------------------------------------------------------------------------
//
// Fence
//
//--------------------------------------------------------------------------------------------------
pub fn createFence(allocator: std.mem.Allocator) Error!Fence {
    var handle = allocator.create(c.ma_fence) catch return error.OutOfMemory;
    errdefer allocator.destroy(handle);
    try checkResult(c.ma_fence_init(handle));
    return @ptrCast(Fence, handle);
}

pub const Fence = *align(@sizeOf(usize)) FenceImpl;
const FenceImpl = opaque {
    pub fn destroy(fence: Fence, allocator: std.mem.Allocator) void {
        const raw = fence.asRaw();
        c.ma_fence_uninit(raw);
        allocator.destroy(raw);
    }

    pub fn asRaw(fence: Fence) *c.ma_fence {
        return @ptrCast(*c.ma_fence, fence);
    }

    pub fn acquire(fence: Fence) Error!void {
        try checkResult(c.ma_fence_acquire(fence.asRaw()));
    }

    pub fn release(fence: Fence) Error!void {
        try checkResult(c.ma_fence_release(fence.asRaw()));
    }

    pub fn wait(fence: Fence) Error!void {
        try checkResult(c.ma_fence_wait(fence.asRaw()));
    }
};
//--------------------------------------------------------------------------------------------------
fn checkResult(result: c.ma_result) Error!void {
    // TODO: Handle all errors.
    if (result != c.MA_SUCCESS)
        return error.GenericError;
}
//--------------------------------------------------------------------------------------------------
//
// Tests
//
//--------------------------------------------------------------------------------------------------
const expect = std.testing.expect;

test "zaudio.engine.basic" {
    const engine = try createEngine(std.testing.allocator, null);
    defer engine.destroy(std.testing.allocator);

    try engine.setTime(engine.getTime());

    std.debug.print("Channels: {}, SampleRate: {}, NumListeners: {}, ClosestListener: {}\n", .{
        engine.getChannels(),
        engine.getSampleRate(),
        engine.getNumListeners(),
        engine.findClosestListener(.{ 0.0, 0.0, 0.0 }),
    });

    try engine.start();
    try engine.stop();
    try engine.start();
    try engine.setVolume(1.0);
    try engine.setGainDb(1.0);

    engine.setListenerEnabled(0, true);
    try expect(engine.isListenerEnabled(0) == true);

    try expect(engine.getDevice() != null);
    _ = engine.getResourceManager();
    _ = engine.getLog();
    _ = engine.getEndpoint();

    const hpf_node = try engine.createHpfNode(HpfNodeConfig.init(
        engine.getChannels(),
        engine.getSampleRate(),
        1000.0,
        2,
    ));
    defer hpf_node.destroy();
    try hpf_node.attachOutputBus(0, engine.getEndpoint(), 0);
}

test "zaudio.soundgroup.basic" {
    const engine = try createEngine(std.testing.allocator, null);
    defer engine.destroy(std.testing.allocator);

    const sgroup = try engine.createSoundGroup(std.testing.allocator, .{}, null);
    defer sgroup.destroy(std.testing.allocator);

    try expect(sgroup.getEngine() == engine);

    try sgroup.start();
    try sgroup.stop();
    try sgroup.start();

    _ = sgroup.getNumInputChannels(0);
    _ = sgroup.getNumOutputChannels(0);
    _ = sgroup.getNumInputBuses();
    _ = sgroup.getNumOutputBuses();

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
    const fence = try createFence(std.testing.allocator);
    defer fence.destroy(std.testing.allocator);

    try fence.acquire();
    try fence.release();
    try fence.wait();
}

test "zaudio.sound.basic" {
    const engine = try createEngine(std.testing.allocator, null);
    defer engine.destroy(std.testing.allocator);

    var config = SoundConfig.init();
    config.raw.channelsIn = 1;
    const sound = try engine.createSound(std.testing.allocator, config);
    defer sound.destroy(std.testing.allocator);

    _ = sound.getNumInputChannels(0);
    _ = sound.getNumOutputChannels(0);
    _ = sound.getNumInputBuses();
    _ = sound.getNumOutputBuses();

    sound.setVolume(0.25);
    try expect(sound.getVolume() == 0.25);

    sound.setPanMode(.pan);
    try expect(sound.getPanMode() == .pan);

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
    var config = DeviceConfig.init(.playback);
    config.playback.format = .float32;
    config.playback.channels = 2;
    config.sample_rate = 48_000;
    const device = try createDevice(std.testing.allocator, null, &config);
    defer device.destroy(std.testing.allocator);
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
    const config = NodeGraphConfig.init(2);
    const node_graph = try createNodeGraph(config);
    defer node_graph.destroy();
    _ = node_graph.getTime();
}
//--------------------------------------------------------------------------------------------------
