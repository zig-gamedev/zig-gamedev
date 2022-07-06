const std = @import("std");
const assert = std.debug.assert;

pub const AdapterType = enum(u32) {
    discrete_gpu,
    integrated_gpu,
    cpu,
    unknown,
};

pub const AddressMode = enum(u32) {
    repeat = 0x00000000,
    mirror_repeat = 0x00000001,
    clamp_to_edge = 0x00000002,
};

pub const AlphaMode = enum(u32) {
    premultiplied = 0x00000000,
    unpremultiplied = 0x00000001,
};

pub const BackendType = enum(u32) {
    nul,
    webgpu,
    d3d11,
    d3d12,
    metal,
    vulkan,
    opengl,
    opengles,
};

pub const BlendFactor = enum(u32) {
    zero = 0x00000000,
    one = 0x00000001,
    src = 0x00000002,
    one_minus_src = 0x00000003,
    src_alpha = 0x00000004,
    one_minus_src_alpha = 0x00000005,
    dst = 0x00000006,
    one_minus_dst = 0x00000007,
    dst_alpha = 0x00000008,
    one_minus_dst_alpha = 0x00000009,
    src_alpha_saturated = 0x0000000A,
    constant = 0x0000000B,
    one_minus_constant = 0x0000000C,
};

pub const BlendOperation = enum(u32) {
    add = 0x00000000,
    subtract = 0x00000001,
    reverse_subtract = 0x00000002,
    min = 0x00000003,
    max = 0x00000004,
};

pub const BufferBindingType = enum(u32) {
    @"undefined" = 0x00000000,
    uniform = 0x00000001,
    storage = 0x00000002,
    read_only_storage = 0x00000003,
};

pub const BufferMapAsyncStatus = enum(u32) {
    success = 0x00000000,
    @"error" = 0x00000001,
    unknown = 0x00000002,
    device_lost = 0x00000003,
    destroyed_before_callback = 0x00000004,
    unmapped_before_callback = 0x00000005,
};

pub const CompareFunction = enum(u32) {
    @"undefined" = 0x00000000,
    never = 0x00000001,
    less = 0x00000002,
    less_equal = 0x00000003,
    greater = 0x00000004,
    greater_equal = 0x00000005,
    equal = 0x00000006,
    not_equal = 0x00000007,
    always = 0x00000008,
};

pub const CompilationInfoRequestStatus = enum(u32) {
    success = 0x00000000,
    @"error" = 0x00000001,
    device_lost = 0x00000002,
    unknown = 0x00000003,
};

pub const CompilationMessageType = enum(u32) {
    @"error" = 0x00000000,
    warning = 0x00000001,
    info = 0x00000002,
};

pub const ComputePassTimestampLocation = enum(u32) {
    beginning = 0x00000000,
    end = 0x00000001,
};

pub const CreatePipelineAsyncStatus = enum(u32) {
    success = 0x00000000,
    @"error" = 0x00000001,
    device_lost = 0x00000002,
    device_destroyed = 0x00000003,
    unknown = 0x00000004,
};

pub const CullMode = enum(u32) {
    none = 0x00000000,
    front = 0x00000001,
    back = 0x00000002,
};

pub const DeviceLostReason = enum(u32) {
    @"undefined" = 0x00000000,
    destroyed = 0x00000001,
};

pub const ErrorFilter = enum(u32) {
    validation = 0x00000000,
    out_of_memory = 0x00000001,
};

pub const ErrorType = enum(u32) {
    no_error = 0x00000000,
    validation = 0x00000001,
    out_of_memory = 0x00000002,
    unknown = 0x00000003,
    device_lost = 0x00000004,
};

pub const FeatureName = enum(u32) {
    @"undefined" = 0x00000000,
    depth24_unorm_stencil8 = 0x00000002,
    depth32_float_stencil8 = 0x00000003,
    timestamp_query = 0x00000004,
    pipeline_statistics_query = 0x00000005,
    texture_compression_bc = 0x00000006,
    texture_compression_etc2 = 0x00000007,
    texture_compression_astc = 0x00000008,
    indirect_first_instance = 0x00000009,
    depth_clamping = 0x000003E8,
    dawn_shader_float16 = 0x000003E9,
    dawn_internal_usages = 0x000003EA,
    dawn_multi_planar_formats = 0x000003EB,
    dawn_native = 0x000003EC,
};

pub const FilterMode = enum(u32) {
    nearest = 0x00000000,
    linear = 0x00000001,
};

pub const FrontFace = enum(u32) {
    ccw = 0x00000000,
    cw = 0x00000001,
};

pub const IndexFormat = enum(u32) {
    @"undefined" = 0x00000000,
    uint16 = 0x00000001,
    uint32 = 0x00000002,
};

pub const LoadOp = enum(u32) {
    @"undefined" = 0x00000000,
    clear = 0x00000001,
    load = 0x00000002,
};

pub const LoggingType = enum(u32) {
    verbose = 0x00000000,
    info = 0x00000001,
    warning = 0x00000002,
    @"error" = 0x00000003,
};

pub const PipelineStatisticName = enum(u32) {
    vertex_shader_invocations = 0x00000000,
    clipper_invocations = 0x00000001,
    clipper_primitives_out = 0x00000002,
    fragment_shader_invocations = 0x00000003,
    compute_shader_invocations = 0x00000004,
};

pub const PowerPreference = enum(u32) {
    @"undefined" = 0x00000000,
    low_power = 0x00000001,
    high_performance = 0x00000002,
};

pub const PredefinedColorSpace = enum(u32) {
    @"undefined" = 0x00000000,
    srgb = 0x00000001,
};

pub const PresentMode = enum(u32) {
    immediate = 0x00000000,
    mailbox = 0x00000001,
    fifo = 0x00000002,
};

pub const PrimitiveTopology = enum(u32) {
    point_list = 0x00000000,
    line_list = 0x00000001,
    line_strip = 0x00000002,
    triangle_list = 0x00000003,
    triangle_strip = 0x00000004,
};

pub const QueryType = enum(u32) {
    occlusion = 0x00000000,
    pipeline_statistics = 0x00000001,
    timestamp = 0x00000002,
};

pub const QueueWorkDoneStatus = enum(u32) {
    success = 0x00000000,
    @"error" = 0x00000001,
    unknown = 0x00000002,
    device_lost = 0x00000003,
};

pub const RenderPassTimestampLocation = enum(u32) {
    beginning = 0x00000000,
    end = 0x00000001,
};

pub const RequestAdapterStatus = enum(u32) {
    success = 0x00000000,
    unavailable = 0x00000001,
    @"error" = 0x00000002,
    unknown = 0x00000003,
};

pub const RequestDeviceStatus = enum(u32) {
    success = 0x00000000,
    @"error" = 0x00000001,
    unknown = 0x00000002,
};

pub const SType = enum(u32) {
    invalid = 0x00000000,
    surface_descriptor_from_metal_layer = 0x00000001,
    surface_descriptor_from_windows_hwnd = 0x00000002,
    surface_descriptor_from_xlib_window = 0x00000003,
    surface_descriptor_from_canvas_html_selector = 0x00000004,
    shader_module_spirv_descriptor = 0x00000005,
    shader_module_wgsl_descriptor = 0x00000006,
    surface_descriptor_from_wayland_surface = 0x00000008,
    surface_descriptor_from_android_native_window = 0x00000009,
    surface_descriptor_from_windows_core_window = 0x0000000B,
    external_texture_binding_entry = 0x0000000C,
    external_texture_binding_layout = 0x0000000D,
    surface_descriptor_from_windows_swap_chain_panel = 0x0000000E,
    dawn_texture_internal_usage_descriptor = 0x000003E8,
    primitive_depth_clamping_state = 0x000003E9,
    dawn_toggles_device_descriptor = 0x000003EA,
    dawn_encoder_internal_usage_descriptor = 0x000003EB,
    dawn_instance_descriptor = 0x000003EC,
    dawn_cache_device_descriptor = 0x000003ED,
};

pub const SamplerBindingType = enum(u32) {
    @"undefined" = 0x00000000,
    filtering = 0x00000001,
    non_filtering = 0x00000002,
    comparison = 0x00000003,
};

pub const StencilOperation = enum(u32) {
    keep = 0x00000000,
    zero = 0x00000001,
    replace = 0x00000002,
    invert = 0x00000003,
    increment_lamp = 0x00000004,
    decrement_clamp = 0x00000005,
    increment_wrap = 0x00000006,
    decrement_wrap = 0x00000007,
};

pub const StorageTextureAccess = enum(u32) {
    @"undefined" = 0x00000000,
    write_only = 0x00000001,
};

pub const StoreOp = enum(u32) {
    @"undefined" = 0x00000000,
    store = 0x00000001,
    discard = 0x00000002,
};

pub const TextureAspect = enum(u32) {
    all = 0x00000000,
    stencil_only = 0x00000001,
    depth_only = 0x00000002,
    plane0_only = 0x00000003,
    plane1_only = 0x00000004,
};

pub const TextureComponentType = enum(u32) {
    float = 0x00000000,
    sint = 0x00000001,
    uint = 0x00000002,
    depth_comparison = 0x00000003,
};

pub const TextureDimension = enum(u32) {
    @"1d" = 0x00000000,
    @"2d" = 0x00000001,
    @"3d" = 0x00000002,
};

pub const TextureFormat = enum(u32) {
    @"undefined" = 0x00000000,
    r8_unorm = 0x00000001,
    r8_snorm = 0x00000002,
    r8_uint = 0x00000003,
    r8_sint = 0x00000004,
    r16_uint = 0x00000005,
    r16_sint = 0x00000006,
    r16_float = 0x00000007,
    rg8_unorm = 0x00000008,
    rg8_snorm = 0x00000009,
    rg8_uint = 0x0000000a,
    rg8_sint = 0x0000000b,
    r32_float = 0x0000000c,
    r32_uint = 0x0000000d,
    r32_sint = 0x0000000e,
    rg16_uint = 0x0000000f,
    rg16_sint = 0x00000010,
    rg16_float = 0x00000011,
    rgba8_unorm = 0x00000012,
    rgba8_unorm_srgb = 0x00000013,
    rgba8_snorm = 0x00000014,
    rgba8_uint = 0x00000015,
    rgba8_sint = 0x00000016,
    bgra8_unorm = 0x00000017,
    bgra8_unorm_srgb = 0x00000018,
    rgb10a2_unorm = 0x00000019,
    rg11b10u_float = 0x0000001a,
    rgb9e5u_float = 0x0000001b,
    rg32_float = 0x0000001c,
    rg32_uint = 0x0000001d,
    rg32_sint = 0x0000001e,
    rgba16_uint = 0x0000001f,
    rgba16_sint = 0x00000020,
    rgba16_float = 0x00000021,
    rgba32_float = 0x00000022,
    rgba32_uint = 0x00000023,
    rgba32_sint = 0x00000024,
    stencil8 = 0x00000025,
    depth16_unorm = 0x00000026,
    depth24_plus = 0x00000027,
    depth24_plus_stencil8 = 0x00000028,
    depth24_unorm_stencil8 = 0x00000029,
    depth32_float = 0x0000002a,
    depth32_float_stencil8 = 0x0000002b,
    bc1rgba_unorm = 0x0000002c,
    bc1rgba_unorm_srgb = 0x0000002d,
    bc2rgba_unorm = 0x0000002e,
    bc2rgba_unorm_srgb = 0x0000002f,
    bc3rgba_unorm = 0x00000030,
    bc3rgba_unorm_srgb = 0x00000031,
    bc4r_unorm = 0x00000032,
    bc4r_snorm = 0x00000033,
    bc5rg_unorm = 0x00000034,
    bc5rg_snorm = 0x00000035,
    bc6hrgbu_float = 0x00000036,
    bc6hrgb_float = 0x00000037,
    bc7rgba_unorm = 0x00000038,
    bc7rgba_unorm_srgb = 0x00000039,
    etc2rgb8_unorm = 0x0000003a,
    etc2rgb8_unorm_srgb = 0x0000003b,
    etc2rgb8a1_unorm = 0x0000003c,
    etc2rgb8a1_unorm_srgb = 0x0000003d,
    etc2rgba8_unorm = 0x0000003e,
    etc2rgba8_unorm_srgb = 0x0000003f,
    eacr11_unorm = 0x00000040,
    eacr11_snorm = 0x00000041,
    eacrg11_unorm = 0x00000042,
    eacrg11_snorm = 0x00000043,
    astc4x4_unorm = 0x00000044,
    astc4x4_unorm_srgb = 0x00000045,
    astc5x4_unorm = 0x00000046,
    astc5x4_unorm_srgb = 0x00000047,
    astc5x5_unorm = 0x00000048,
    astc5x5_unorm_srgb = 0x00000049,
    astc6x5_unorm = 0x0000004a,
    astc6x5_unorm_srgb = 0x0000004b,
    astc6x6_unorm = 0x0000004c,
    astc6x6_unorm_srgb = 0x0000004d,
    astc8x5_unorm = 0x0000004e,
    astc8x5_unorm_srgb = 0x0000004f,
    astc8x6_unorm = 0x00000050,
    astc8x6_unorm_srgb = 0x00000051,
    astc8x8_unorm = 0x00000052,
    astc8x8_unorm_srgb = 0x00000053,
    astc10x5_unorm = 0x00000054,
    astc10x5_unorm_srgb = 0x00000055,
    astc10x6_unorm = 0x00000056,
    astc10x6_unorm_srgb = 0x00000057,
    astc10x8_unorm = 0x00000058,
    astc10x8_unorm_srgb = 0x00000059,
    astc10x10_unorm = 0x0000005a,
    astc10x10_unorm_srgb = 0x0000005b,
    astc12x10_unorm = 0x0000005c,
    astc12x10_unorm_srgb = 0x0000005d,
    astc12x12_unorm = 0x0000005e,
    astc12x12_unorm_srgb = 0x0000005f,
    r8bg8biplanar420_unorm = 0x00000060,
};

pub const TextureSampleType = enum(u32) {
    @"undefined" = 0x00000000,
    float = 0x00000001,
    unfilterable_float = 0x00000002,
    depth = 0x00000003,
    sint = 0x00000004,
    uint = 0x00000005,
};

pub const TextureViewDimension = enum(u32) {
    @"undefined" = 0x00000000,
    @"1d" = 0x00000001,
    @"2d" = 0x00000002,
    @"2d_array" = 0x00000003,
    cube = 0x00000004,
    cube_array = 0x00000005,
    @"3d" = 0x00000006,
};

pub const VertexFormat = enum(u32) {
    @"undefined" = 0x00000000,
    uint8x2 = 0x00000001,
    uint8x4 = 0x00000002,
    sint8x2 = 0x00000003,
    sint8x4 = 0x00000004,
    unorm8x2 = 0x00000005,
    unorm8x4 = 0x00000006,
    snorm8x2 = 0x00000007,
    snorm8x4 = 0x00000008,
    uint16x2 = 0x00000009,
    uint16x4 = 0x0000000A,
    sint16x2 = 0x0000000B,
    sint16x4 = 0x0000000C,
    unorm16x2 = 0x0000000D,
    unorm16x4 = 0x0000000E,
    snorm16x2 = 0x0000000F,
    snorm16x4 = 0x00000010,
    float16x2 = 0x00000011,
    float16x4 = 0x00000012,
    float32 = 0x00000013,
    float32x2 = 0x00000014,
    float32x3 = 0x00000015,
    float32x4 = 0x00000016,
    uint32 = 0x00000017,
    uint32x2 = 0x00000018,
    uint32x3 = 0x00000019,
    uint32x4 = 0x0000001A,
    sint32 = 0x0000001B,
    sint32x2 = 0x0000001C,
    sint32x3 = 0x0000001D,
    sint32x4 = 0x0000001E,
};

pub const VertexStepMode = enum(u32) {
    vertex = 0x00000000,
    instance = 0x00000001,
};

pub const BufferUsage = packed struct {
    map_read: bool = false,
    map_write: bool = false,
    copy_src: bool = false,
    copy_dst: bool = false,
    index: bool = false,
    vertex: bool = false,
    uniform: bool = false,
    storage: bool = false,
    indirect: bool = false,
    query_resolve: bool = false,

    _pad0: u6 = 0,
    _pad1: u16 = 0,

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

pub const ColorWriteMask = packed struct {
    red: bool = false,
    green: bool = false,
    blue: bool = false,
    alpha: bool = false,

    _pad0: u12 = 0,
    _pad1: u16 = 0,

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }

    pub const all = ColorWriteMask{ .red = true, .green = true, .blue = true, .alpha = true };
};

pub const MapMode = packed struct {
    read: bool = false,
    write: bool = false,

    _pad0: u14 = 0,
    _pad1: u16 = 0,

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

pub const ShaderStage = packed struct {
    vertex: bool = false,
    fragment: bool = false,
    compute: bool = false,

    _pad0: u13 = 0,
    _pad1: u16 = 0,

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

pub const TextureUsage = packed struct {
    copy_src: bool = false,
    copy_dst: bool = false,
    texture_binding: bool = false,
    storage_binding: bool = false,
    render_attachment: bool = false,
    present: bool = false,

    _pad0: u10 = 0,
    _pad1: u16 = 0,

    comptime {
        assert(@sizeOf(@This()) == @sizeOf(u32) and @bitSizeOf(@This()) == @bitSizeOf(u32));
    }
};

pub const ChainedStruct = extern struct {
    next: ?*const ChainedStruct,
    stype: SType,
};

pub const ChainedStructOut = extern struct {
    next: ?*ChainedStructOut,
    stype: SType,
};

pub const AdapterProperties = extern struct {
    next_in_chain: ?*ChainedStructOut = null,
    vendor_id: u32,
    device_id: u32,
    name: [*:0]const u8,
    driver_description: [*:0]const u8,
    adapter_type: AdapterType,
    backend_type: BackendType,
};

pub const BindGroupEntry = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    binding: u32,
    buffer: Buffer,
    offset: u64,
    size: u64,
    sampler: Sampler,
    textureView: TextureView,
};

pub const BindGroupDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    layout: BindGroupLayout,
    entry_count: u32,
    entries: ?[*]const BindGroupEntry,
};

pub const BufferBindingLayout = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    @"type": BufferBindingType,
    has_dynamic_offset: bool,
    min_binding_size: u64,
};

pub const SamplerBindingLayout = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    @"type": SamplerBindingType,
};

pub const TextureBindingLayout = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    sample_type: TextureSampleType,
    view_dimension: TextureViewDimension,
    multisampled: bool,
};

pub const StorageTextureBindingLayout = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    access: StorageTextureAccess,
    format: TextureFormat,
    view_dimension: TextureViewDimension,
};

pub const BindGroupLayoutEntry = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    binding: u32,
    visibility: ShaderStage,
    buffer: BufferBindingLayout,
    sampler: SamplerBindingLayout,
    texture: TextureBindingLayout,
    storage_texture: StorageTextureBindingLayout,
};

pub const BindGroupLayoutDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    entry_count: u32,
    entries: ?[*]const BindGroupLayoutEntry,
};

pub const BufferDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    usage: BufferUsage,
    size: u64,
    mapped_at_creation: bool,
};

pub const CommandEncoderDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
};

pub const ConstantEntry = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    key: [*:0]const u8,
    value: f64,
};

pub const ProgrammableStageDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    module: ShaderModule,
    entry_point: [*:0]const u8,
    constant_count: u32,
    constants: ?[*]const ConstantEntry,
};

pub const ComputePipelineDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    layout: ?PipelineLayout = null,
    compute: ProgrammableStageDescriptor,
};

pub const ExternalTextureDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    plane0: TextureView,
    plane1: TextureView,
    color_space: PredefinedColorSpace,
};

pub const PipelineLayoutDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    bind_group_layout_count: u32,
    bind_group_layouts: ?[*]const BindGroupLayout,
};

pub const QuerySetDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    @"type": QueryType,
    count: u32,
    pipeline_statistics: ?[*]const PipelineStatisticName,
    pipeline_statistics_count: u32,
};

pub const RenderBundleEncoderDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    color_formats_count: u32,
    color_formats: ?[*]const TextureFormat,
    depth_stencil_format: TextureFormat,
    sample_count: u32,
    depth_read_only: bool,
    stencil_read_only: bool,
};

pub const VertexAttribute = extern struct {
    format: VertexFormat,
    offset: u64,
    shader_location: u32,
};

pub const VertexBufferLayout = extern struct {
    array_stride: u64,
    step_mode: VertexStepMode,
    attribute_count: u32,
    attributes: [*]const VertexAttribute,
};

pub const VertexState = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    module: ShaderModule,
    entry_point: [*:0]const u8,
    constant_count: u32,
    constants: ?[*]const ConstantEntry,
    buffer_count: u32,
    buffers: ?[*]const VertexBufferLayout,
};

pub const BlendComponent = extern struct {
    operation: BlendOperation,
    src_factor: BlendFactor,
    dst_factor: BlendFactor,
};

pub const BlendState = extern struct {
    color: BlendComponent,
    alpha: BlendComponent,
};

pub const ColorTargetState = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    format: TextureFormat,
    blend: ?*const BlendState,
    write_mask: ColorWriteMask,
};

pub const FragmentState = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    module: ShaderModule,
    entry_point: [*:0]const u8,
    constant_count: u32,
    constants: ?[*]const ConstantEntry,
    target_count: u32,
    targets: ?[*]const ColorTargetState,
};

pub const PrimitiveState = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    topology: PrimitiveTopology,
    strip_index_format: IndexFormat,
    front_face: FrontFace,
    cull_mode: CullMode,
};

pub const StencilFaceState = extern struct {
    compare: CompareFunction,
    fail_op: StencilOperation,
    depth_fail_op: StencilOperation,
    pass_op: StencilOperation,
};

pub const DepthStencilState = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    format: TextureFormat,
    depth_write_enabled: bool,
    depth_compare: CompareFunction,
    stencil_front: StencilFaceState,
    stencil_back: StencilFaceState,
    stencil_read_mask: u32,
    stencil_write_mask: u32,
    depth_bias: i32,
    depth_bias_slope_scale: f32,
    depth_bias_clamp: f32,
};

pub const MultisampleState = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    count: u32,
    mask: u32,
    alpha_to_coverage_enabled: bool,
};

pub const RenderPipelineDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    layout: ?PipelineLayout,
    vertex: VertexState,
    primitive: PrimitiveState,
    depth_stencil: ?*const DepthStencilState,
    multisample: MultisampleState,
    fragment: ?*const FragmentState,
};

pub const SamplerDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    address_mode_u: AddressMode,
    address_mode_v: AddressMode,
    address_mode_w: AddressMode,
    mag_filter: FilterMode,
    min_filter: FilterMode,
    lod_min_clamp: f32,
    lod_max_clamp: f32,
    compare: CompareFunction,
    max_anisotropy: u16,
};

pub const ShaderModuleDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
};

pub const SwapChainDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    usage: TextureUsage,
    format: TextureFormat,
    width: u32,
    height: u32,
    present_mode: PresentMode,
    implementation: u64,
};

pub const Extent3D = extern struct {
    width: u32,
    height: u32,
    depth_or_array_layers: u32,
};

pub const TextureDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    usage: TextureUsage,
    dimension: TextureDimension,
    size: Extent3D,
    format: TextureFormat,
    mip_level_count: u32,
    sample_count: u32,
    view_format_count: u32,
    view_formats: ?[*]const TextureFormat,
};

pub const Limits = extern struct {
    max_texture_dimension_1d: u32,
    max_texture_dimension_2d: u32,
    max_texture_dimension_3d: u32,
    max_texture_array_layers: u32,
    max_bind_groups: u32,
    max_dynamic_uniform_buffers_per_pipeline_layout: u32,
    max_dynamic_storage_buffers_per_pipeline_layout: u32,
    max_sampled_textures_per_shader_stage: u32,
    max_samplers_per_shader_stage: u32,
    max_storage_buffers_per_shader_stage: u32,
    max_storage_textures_per_shader_stage: u32,
    max_uniform_buffers_per_shader_stage: u32,
    max_uniform_buffer_binding_size: u64,
    max_storage_buffer_binding_size: u64,
    min_uniform_buffer_offset_alignment: u32,
    min_storage_buffer_offset_alignment: u32,
    max_vertex_buffers: u32,
    max_vertex_attributes: u32,
    max_vertex_buffer_array_stride: u32,
    max_inter_stage_shader_components: u32,
    max_compute_workgroup_storage_size: u32,
    max_compute_invocations_per_workgroup: u32,
    max_compute_workgroup_size_x: u32,
    max_compute_workgroup_size_y: u32,
    max_compute_workgroup_size_z: u32,
    max_compute_workgroups_per_dimension: u32,
};

pub const RequiredLimits = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    limits: Limits,
};

pub const SupportedLimits = extern struct {
    next_in_chain: ?*ChainedStructOut = null,
    limits: Limits,
};

pub const QueueDescription = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
};

pub const DeviceDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    required_features_count: u32,
    required_features: ?[*]const FeatureName,
    required_limits: ?[*]const RequiredLimits,
    default_queue: QueueDescription,
};

pub const SurfaceDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
};

pub const RequestAdapterOptions = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    compatible_surface: ?Surface = null,
    power_preference: PowerPreference,
    force_fallback_adapter: bool = false,
};

pub const Adapter = *align(@sizeOf(usize)) AdapterImpl;
pub const BindGroup = *align(@sizeOf(usize)) BindGroupImpl;
pub const BindGroupLayout = *align(@sizeOf(usize)) BindGroupLayoutImpl;
pub const Buffer = *align(@sizeOf(usize)) BufferImpl;
pub const CommandBuffer = *align(@sizeOf(usize)) CommandBufferImpl;
pub const CommandEncoder = *align(@sizeOf(usize)) CommandEncoderImpl;
pub const ComputePassEncoder = *align(@sizeOf(usize)) ComputePassEncoderImpl;
pub const ComputePipeline = *align(@sizeOf(usize)) ComputePipelineImpl;
pub const Device = *align(@sizeOf(usize)) DeviceImpl;
pub const ExternalTexture = *align(@sizeOf(usize)) ExternalTextureImpl;
pub const Instance = *align(@sizeOf(usize)) InstanceImpl;
pub const PipelineLayout = *align(@sizeOf(usize)) PipelineLayoutImpl;
pub const QuerySet = *align(@sizeOf(usize)) QuerySetImpl;
pub const Queue = *align(@sizeOf(usize)) QueueImpl;
pub const RenderBundle = *align(@sizeOf(usize)) RenderBundleImpl;
pub const RenderBundleEncoder = *align(@sizeOf(usize)) RenderBundleEncoderImpl;
pub const RenderPassEncoder = *align(@sizeOf(usize)) RenderPassEncoderImpl;
pub const RenderPipeline = *align(@sizeOf(usize)) RenderPipelineImpl;
pub const Sampler = *align(@sizeOf(usize)) SamplerImpl;
pub const ShaderModule = *align(@sizeOf(usize)) ShaderModuleImpl;
pub const Surface = *align(@sizeOf(usize)) SurfaceImpl;
pub const SwapChain = *align(@sizeOf(usize)) SwapChainImpl;
pub const Texture = *align(@sizeOf(usize)) TextureImpl;
pub const TextureView = *align(@sizeOf(usize)) TextureViewImpl;

pub const CreateComputePipelineAsyncCallback = fn (
    status: CreatePipelineAsyncStatus,
    pipeline: ComputePipeline,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const CreateRenderPipelineAsyncCallback = fn (
    status: CreatePipelineAsyncStatus,
    pipeline: RenderPipeline,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const ErrorCallback = fn (
    etype: ErrorType,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const LoggingCallback = fn (
    etype: LoggingType,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const DeviceLostCallback = fn (
    reason: DeviceLostReason,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const RequestAdapterCallback = fn (
    status: RequestAdapterStatus,
    adapter: Adapter,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

const AdapterImpl = opaque {
    pub fn createDevice(adapter: Adapter, descriptor: DeviceDescriptor) Device {
        return wgpuAdapterCreateDevice(adapter, &descriptor);
    }
    extern fn wgpuAdapterCreateDevice(adapter: Adapter, descriptor: *const DeviceDescriptor) Device;

    pub fn enumerateFeatures(adapter: Adapter, features: ?[*]FeatureName) usize {
        return wgpuAdapterEnumerateFeatures(adapter, features);
    }
    extern fn wgpuAdapterEnumerateFeatures(adapter: Adapter, features: ?[*]FeatureName) usize;

    // TODO: Add functions.
};

const BindGroupImpl = opaque {
    // TODO: Add functions.
};

const BindGroupLayoutImpl = opaque {
    // TODO: Add functions.
};

const BufferImpl = opaque {
    // TODO: Add functions.
};

const CommandBufferImpl = opaque {
    // TODO: Add functions.
};

const CommandEncoderImpl = opaque {
    // TODO: Add functions.
};

const ComputePassEncoderImpl = opaque {
    // TODO: Add functions.
};

const ComputePipelineImpl = opaque {
    // TODO: Add functions.
};

const DeviceImpl = opaque {
    pub fn createBindGroup(device: Device, descriptor: BindGroupDescriptor) BindGroup {
        return wgpuDeviceCreateBindGroup(device, &descriptor);
    }
    extern fn wgpuDeviceCreateBindGroup(device: Device, descriptor: *const BindGroupDescriptor) BindGroup;

    pub fn createBindGroupLayout(device: Device, descriptor: BindGroupLayoutDescriptor) BindGroupLayout {
        return wgpuDeviceCreateBindGroupLayout(device, &descriptor);
    }
    extern fn wgpuDeviceCreateBindGroupLayout(
        device: Device,
        descriptor: *const BindGroupLayoutDescriptor,
    ) BindGroupLayout;

    pub fn createBuffer(device: Device, descriptor: BufferDescriptor) Buffer {
        return wgpuDeviceCreateBuffer(device, &descriptor);
    }
    extern fn wgpuDeviceCreateBuffer(device: Device, descriptor: *const BufferDescriptor) Buffer;

    pub fn createCommandEncoder(device: Device, descriptor: CommandEncoderDescriptor) CommandEncoder {
        return wgpuDeviceCreateCommandEncoder(device, &descriptor);
    }
    extern fn wgpuDeviceCreateCommandEncoder(
        device: Device,
        descriptor: *const CommandEncoderDescriptor,
    ) CommandEncoder;

    pub fn createComputePipeline(device: Device, descriptor: ComputePipelineDescriptor) ComputePipeline {
        return wgpuDeviceCreateComputePipeline(device, &descriptor);
    }
    extern fn wgpuDeviceCreateComputePipeline(
        device: Device,
        descriptor: *const ComputePipelineDescriptor,
    ) ComputePipeline;

    pub fn createComputePipelineAsync(
        device: Device,
        descriptor: ComputePipelineDescriptor,
        callback: CreateComputePipelineAsyncCallback,
        userdata: ?*anyopaque,
    ) void {
        wgpuDeviceCreateComputePipelineAsync(device, &descriptor, callback, userdata);
    }
    extern fn wgpuDeviceCreateComputePipelineAsync(
        device: Device,
        descriptor: *const ComputePipelineDescriptor,
        callback: CreateComputePipelineAsyncCallback,
        userdata: ?*anyopaque,
    ) void;

    pub fn createErrorBuffer(device: Device) Buffer {
        return wgpuDeviceCreateErrorBuffer(device);
    }
    extern fn wgpuDeviceCreateErrorBuffer(device: Device) Buffer;

    pub fn createExternalTexture(device: Device, descriptor: ExternalTextureDescriptor) ExternalTexture {
        return wgpuDeviceCreateExternalTexture(device, &descriptor);
    }
    extern fn wgpuDeviceCreateExternalTexture(
        device: Device,
        descriptor: *const ExternalTextureDescriptor,
    ) ExternalTexture;

    pub fn createPipelineLayout(device: Device, descriptor: PipelineLayoutDescriptor) PipelineLayout {
        return wgpuDeviceCreatePipelineLayout(device, &descriptor);
    }
    extern fn wgpuDeviceCreatePipelineLayout(
        device: Device,
        descriptor: *const PipelineLayoutDescriptor,
    ) PipelineLayout;

    pub fn createQuerySet(device: Device, descriptor: QuerySetDescriptor) QuerySet {
        return wgpuDeviceCreateQuerySet(device, &descriptor);
    }
    extern fn wgpuDeviceCreateQuerySet(device: Device, descriptor: *const QuerySetDescriptor) QuerySet;

    pub fn createRenderBundleEncoder(device: Device, descriptor: RenderBundleEncoderDescriptor) RenderBundleEncoder {
        return wgpuDeviceCreateRenderBundleEncoder(device, &descriptor);
    }
    extern fn wgpuDeviceCreateRenderBundleEncoder(
        device: Device,
        descriptor: *const RenderBundleEncoderDescriptor,
    ) RenderBundleEncoder;

    pub fn createRenderPipeline(device: Device, descriptor: RenderPipelineDescriptor) RenderPipeline {
        return wgpuDeviceCreateRenderPipeline(device, &descriptor);
    }
    extern fn wgpuDeviceCreateRenderPipeline(
        device: Device,
        descriptor: *const RenderPipelineDescriptor,
    ) RenderPipeline;

    pub fn createRenderPipelineAsync(
        device: Device,
        descriptor: RenderPipelineDescriptor,
        callback: CreateRenderPipelineAsyncCallback,
        userdata: ?*anyopaque,
    ) void {
        wgpuDeviceCreateRenderPipelineAsync(device, &descriptor, callback, userdata);
    }
    extern fn wgpuDeviceCreateRenderPipelineAsync(
        device: Device,
        descriptor: *const RenderPipelineDescriptor,
        callback: CreateRenderPipelineAsyncCallback,
        userdata: ?*anyopaque,
    ) void;

    pub fn createSampler(device: Device, descriptor: SamplerDescriptor) Sampler {
        return wgpuDeviceCreateSampler(device, &descriptor);
    }
    extern fn wgpuDeviceCreateSampler(device: Device, descriptor: *const SamplerDescriptor) Sampler;

    pub fn createShaderModule(device: Device, descriptor: SamplerDescriptor) ShaderModule {
        return wgpuDeviceCreateShaderModule(device, &descriptor);
    }
    extern fn wgpuDeviceCreateShaderModule(device: Device, descriptor: *const SamplerDescriptor) ShaderModule;

    pub fn createSwapChain(device: Device, descriptor: SwapChainDescriptor) SwapChain {
        return wgpuDeviceCreateSwapChain(device, &descriptor);
    }
    extern fn wgpuDeviceCreateSwapChain(device: Device, descriptor: *const SwapChainDescriptor) SwapChain;

    pub fn createTexture(device: Device, descriptor: TextureDescriptor) Texture {
        return wgpuDeviceCreateTexture(device, &descriptor);
    }
    extern fn wgpuDeviceCreateTexture(device: Device, descriptor: *const TextureDescriptor) Texture;

    pub fn destroy(device: Device) void {
        wgpuDeviceDestroy(device);
    }
    extern fn wgpuDeviceDestroy(device: Device) void;

    pub fn enumerateFeatures(device: Device, features: ?[*]FeatureName) usize {
        return wgpuDeviceEnumerateFeatures(device, features);
    }
    extern fn wgpuDeviceEnumerateFeatures(device: Device, features: ?[*]FeatureName) usize;

    pub fn getLimits(device: Device, limits: *SupportedLimits) bool {
        return wgpuDeviceGetLimits(device, limits);
    }
    extern fn wgpuDeviceGetLimits(device: Device, limits: *SupportedLimits) bool;

    pub fn getQueue(device: Device) Queue {
        return wgpuDeviceGetQueue(device);
    }
    extern fn wgpuDeviceGetQueue(device: Device) Queue;

    pub fn hasFeature(device: Device, feature: FeatureName) bool {
        return wgpuDeviceHasFeature(device, feature);
    }
    extern fn wgpuDeviceHasFeature(device: Device, feature: FeatureName) bool;

    pub fn injectError(device: Device, etype: ErrorType, message: ?[*:0]const u8) void {
        wgpuDeviceInjectError(device, etype, message);
    }
    extern fn wgpuDeviceInjectError(device: Device, etype: ErrorType, message: ?[*:0]const u8) void;

    pub fn loseForTesting(device: Device) void {
        wgpuDeviceLoseForTesting(device);
    }
    extern fn wgpuDeviceLoseForTesting(device: Device) void;

    pub fn popErrorScope(device: Device, callback: ErrorCallback, userdata: ?*anyopaque) bool {
        return wgpuDevicePopErrorScope(device, callback, userdata);
    }
    extern fn wgpuDevicePopErrorScope(device: Device, callback: ErrorCallback, userdata: ?*anyopaque) bool;

    pub fn pushErrorScope(device: Device, filter: ErrorFilter) void {
        wgpuDevicePushErrorScope(device, filter);
    }
    extern fn wgpuDevicePushErrorScope(device: Device, filter: ErrorFilter) void;

    pub fn setDeviceLostCallback(device: Device, callback: DeviceLostCallback, userdata: ?*anyopaque) void {
        wgpuDeviceSetDeviceLostCallback(device, callback, userdata);
    }
    extern fn wgpuDeviceSetDeviceLostCallback(
        device: Device,
        callback: DeviceLostCallback,
        userdata: ?*anyopaque,
    ) void;

    pub fn setLabel(device: Device, label: ?[*:0]const u8) void {
        wgpuDeviceSetLabel(device, label);
    }
    extern fn wgpuDeviceSetLabel(device: Device, label: ?[*:0]const u8) void;

    pub fn setLoggingCallback(device: Device, callback: LoggingCallback, userdata: ?*anyopaque) void {
        wgpuDeviceSetLoggingCallback(device, callback, userdata);
    }
    extern fn wgpuDeviceSetLoggingCallback(device: Device, callback: LoggingCallback, userdata: ?*anyopaque) void;

    pub fn setUncapturedErrorCallback(device: Device, callback: ErrorCallback, userdata: ?*anyopaque) void {
        wgpuDeviceSetUncapturedErrorCallback(device, callback, userdata);
    }
    extern fn wgpuDeviceSetUncapturedErrorCallback(
        device: Device,
        callback: ErrorCallback,
        userdata: ?*anyopaque,
    ) void;

    pub fn tick(device: Device) void {
        wgpuDeviceTick(device);
    }
    extern fn wgpuDeviceTick(device: Device) void;

    pub fn reference(device: Device) void {
        wgpuDeviceReference(device);
    }
    extern fn wgpuDeviceReference(device: Device) void;

    pub fn release(device: Device) void {
        wgpuDeviceRelease(device);
    }
    extern fn wgpuDeviceRelease(device: Device) void;
};

const ExternalTextureImpl = opaque {
    // TODO: Add functions.
};

const InstanceImpl = opaque {
    pub fn createSurface(instance: Instance, descriptor: SurfaceDescriptor) Surface {
        return wgpuInstanceCreateSurface(instance, &descriptor);
    }
    extern fn wgpuInstanceCreateSurface(instance: Instance, descriptor: *const SurfaceDescriptor) Surface;

    pub fn requestAdapter(
        instance: Instance,
        options: RequestAdapterOptions,
        callback: RequestAdapterCallback,
        userdata: ?*anyopaque,
    ) void {
        wgpuInstanceRequestAdapter(instance, &options, callback, userdata);
    }
    extern fn wgpuInstanceRequestAdapter(
        instance: Instance,
        options: *const RequestAdapterOptions,
        callback: RequestAdapterCallback,
        userdata: ?*anyopaque,
    ) void;

    pub fn reference(instance: Instance) void {
        wgpuInstanceReference(instance);
    }
    extern fn wgpuInstanceReference(instance: Instance) void;

    pub fn release(instance: Instance) void {
        wgpuInstanceRelease(instance);
    }
    extern fn wgpuInstanceRelease(instance: Instance) void;
};

const PipelineLayoutImpl = opaque {
    // TODO: Add functions.
};

const QuerySetImpl = opaque {
    // TODO: Add functions.
};

const QueueImpl = opaque {
    // TODO: Add functions.
};

const RenderBundleImpl = opaque {
    // TODO: Add functions.
};

const RenderBundleEncoderImpl = opaque {
    // TODO: Add functions.
};

const RenderPassEncoderImpl = opaque {
    // TODO: Add functions.
};

const RenderPipelineImpl = opaque {
    // TODO: Add functions.
};

const SamplerImpl = opaque {
    // TODO: Add functions.
};

const ShaderModuleImpl = opaque {
    // TODO: Add functions.
};

const SurfaceImpl = opaque {
    // TODO: Add functions.
};

const SwapChainImpl = opaque {
    // TODO: Add functions.
};

const TextureImpl = opaque {
    // TODO: Add functions.
};

const TextureViewImpl = opaque {
    // TODO: Add functions.
};
