const std = @import("std");

test "extern struct ABI compatibility" {
    @setEvalBranchQuota(10_000);
    const wgpu = @cImport(@cInclude("dawn/webgpu.h"));
    inline for (comptime std.meta.declarations(@This())) |decl| {
        const ZigType = @field(@This(), decl.name);
        if (@TypeOf(ZigType) != type) {
            continue;
        }
        if (comptime std.meta.activeTag(@typeInfo(ZigType)) == .Struct and
            @typeInfo(ZigType).Struct.layout == .@"extern")
        {
            const wgpu_name = "WGPU" ++ decl.name;
            const CType = @field(wgpu, wgpu_name);
            std.testing.expectEqual(@sizeOf(CType), @sizeOf(ZigType)) catch |err| {
                std.log.err("@sizeOf({s}) != @sizeOf({s})", .{ wgpu_name, decl.name });
                return err;
            };
            comptime var i: usize = 0;
            inline for (comptime std.meta.fieldNames(CType)) |c_field_name| {
                std.testing.expectEqual(
                    @offsetOf(CType, c_field_name),
                    @offsetOf(ZigType, std.meta.fieldNames(ZigType)[i]),
                ) catch |err| {
                    std.log.err(
                        "@offsetOf({s}, {s}) != @offsetOf({s}, {s})",
                        .{ wgpu_name, c_field_name, decl.name, std.meta.fieldNames(ZigType)[i] },
                    );
                    return err;
                };
                i += 1;
            }
        }
    }
}

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
    opaq = 0x00000002,
};

pub const BackendType = enum(u32) {
    undef,
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
    undef = 0x00000000,
    uniform = 0x00000001,
    storage = 0x00000002,
    read_only_storage = 0x00000003,
};

pub const BufferMapAsyncStatus = enum(u32) {
    success = 0x00000000,
    validation_error = 0x00000001,
    unknown = 0x00000002,
    device_lost = 0x00000003,
    destroyed_before_callback = 0x00000004,
    unmapped_before_callback = 0x00000005,
    mappingAlreadyPending = 0x00000006,
    offset_out_of_range = 0x00000007,
    size_out_of_range = 0x00000008,
};

pub const BufferMapState = enum(u32) {
    unmapped = 0x00000000,
    pending = 0x00000001,
    mapped = 0x00000002,
};

pub const CompareFunction = enum(u32) {
    undef = 0x00000000,
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
    err = 0x00000001,
    device_lost = 0x00000002,
    unknown = 0x00000003,
};

pub const CompilationMessageType = enum(u32) {
    err = 0x00000000,
    warning = 0x00000001,
    info = 0x00000002,
};

pub const ComputePassTimestampLocation = enum(u32) {
    beginning = 0x00000000,
    end = 0x00000001,
};

pub const CreatePipelineAsyncStatus = enum(u32) {
    success = 0x00000000,
    validation_error = 0x00000001,
    internal_error = 0x00000002,
    device_lost = 0x00000003,
    device_destroyed = 0x00000004,
    unknown = 0x00000005,
};

pub const ExternalTextureRotation = enum(u32) {
    rotate_0_degrees = 0x00000000,
    rotate_90_degrees = 0x00000001,
    rotate_180_degrees = 0x00000002,
    rotate_270_degrees = 0x00000003,
};

pub const CullMode = enum(u32) {
    none = 0x00000000,
    front = 0x00000001,
    back = 0x00000002,
};

pub const DeviceLostReason = enum(u32) {
    undef = 0x00000000,
    destroyed = 0x00000001,
};

pub const ErrorFilter = enum(u32) {
    validation = 0x00000000,
    out_of_memory = 0x00000001,
    internal = 0x00000002,
};

pub const ErrorType = enum(u32) {
    no_error = 0x00000000,
    validation = 0x00000001,
    out_of_memory = 0x00000002,
    internal = 0x00000003,
    unknown = 0x00000004,
    device_lost = 0x00000005,
};

pub const FeatureName = enum(u32) {
    undef = 0x00000000,
    depth_clip_control = 0x00000001,
    depth32_float_stencil8 = 0x00000002,
    timestamp_query = 0x00000003,
    pipeline_statistics_query = 0x00000004,
    texture_compression_bc = 0x00000005,
    texture_compression_etc2 = 0x00000006,
    texture_compression_astc = 0x00000007,
    indirect_first_instance = 0x00000008,
    shader_f16 = 0x00000009,
    rg11_b10_ufloat_renderable = 0x0000000A,
    bgra8_unorm_storage = 0x0000000B,
    float32_filterable = 0x0000000C,
    depth_clamping = 0x000003E8,
    dawn_shader_float16 = 0x000003E9,
    dawn_internal_usages = 0x000003EA,
    dawn_multi_planar_formats = 0x000003EB,
    dawn_native = 0x000003EC,
    chromium_experimental_dp4a = 0x000003ED,
    timestamp_query_inside_passes = 0x000003EE,
    implicit_device_synchronization = 0x000003EF,
    surface_capabilities = 0x000003F0,
    transient_attachments = 0x000003F1,
    msaa_render_to_single_sampled = 0x000003F2,
};

pub const FilterMode = enum(u32) {
    nearest = 0x00000000,
    linear = 0x00000001,
};

pub const MipmapFilterMode = enum(u32) {
    nearest = 0x00000000,
    linear = 0x00000001,
};

pub const FrontFace = enum(u32) {
    ccw = 0x00000000,
    cw = 0x00000001,
};

pub const IndexFormat = enum(u32) {
    undef = 0x00000000,
    uint16 = 0x00000001,
    uint32 = 0x00000002,
};

pub const LoadOp = enum(u32) {
    undef = 0x00000000,
    clear = 0x00000001,
    load = 0x00000002,
};

pub const LoggingType = enum(u32) {
    verbose = 0x00000000,
    info = 0x00000001,
    warning = 0x00000002,
    err = 0x00000003,
};

pub const PipelineStatisticName = enum(u32) {
    vertex_shader_invocations = 0x00000000,
    clipper_invocations = 0x00000001,
    clipper_primitives_out = 0x00000002,
    fragment_shader_invocations = 0x00000003,
    compute_shader_invocations = 0x00000004,
};

pub const PowerPreference = enum(u32) {
    undef = 0x00000000,
    low_power = 0x00000001,
    high_performance = 0x00000002,
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
    err = 0x00000001,
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
    err = 0x00000002,
    unknown = 0x00000003,
};

pub const RequestDeviceStatus = enum(u32) {
    success = 0x00000000,
    err = 0x00000001,
    unknown = 0x00000002,
};

pub const SurfaceDescriptorFromMetalLayer = extern struct {
    chain: ChainedStruct,
    layer: *anyopaque,
};

pub const SurfaceDescriptorFromWaylandSurface = extern struct {
    chain: ChainedStruct,
    display: *anyopaque,
    surface: *anyopaque,
};

pub const SurfaceDescriptorFromWindowsHWND = extern struct {
    chain: ChainedStruct,
    hinstance: *anyopaque,
    hwnd: *anyopaque,
};

pub const SurfaceDescriptorFromXlibWindow = extern struct {
    chain: ChainedStruct,
    display: *anyopaque,
    window: u32,
};

pub const SurfaceDescriptorFromWindowsCoreWindow = extern struct {
    chain: ChainedStruct,
    core_window: *anyopaque,
};

pub const SurfaceDescriptorFromWindowsSwapChainPanel = extern struct {
    chain: ChainedStruct,
    swap_chain_panel: *anyopaque,
};

pub const SurfaceDescriptorFromCanvasHTMLSelector = extern struct {
    chain: ChainedStruct,
    selector: [*:0]const u8,
};

pub const StructType = enum(u32) {
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
    dawn_encoder_internal_usage_descriptor = 0x000003EB,
    dawn_instance_descriptor = 0x000003EC,
    dawn_cache_device_descriptor = 0x000003ED,
    dawn_adapter_properties_power_preference = 0x000003EE,
    dawn_buffer_descriptor_error_info_from_wire_client = 0x000003EF,
    dawn_toggles_descriptor = 0x000003F0,
    dawn_shader_module_spirv_options_descriptor = 0x000003F1,
    request_adapter_options_luid = 0x000003F2,
    request_adapter_options_get_gl_proc = 0x000003F3,
    dawn_multisample_state_render_to_single_sampled = 0x000003F4,
    dawn_render_pass_color_attachment_render_to_single_sampled = 0x000003F5,
};

pub const SamplerBindingType = enum(u32) {
    undef = 0x00000000,
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
    undef = 0x00000000,
    write_only = 0x00000001,
};

pub const StoreOp = enum(u32) {
    undef = 0x00000000,
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

pub const TextureDimension = enum(u32) {
    tdim_1d = 0x00000000,
    tdim_2d = 0x00000001,
    tdim_3d = 0x00000002,
};

pub const TextureFormat = enum(u32) {
    undef = 0x00000000,
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
    rgb10_a2_unorm = 0x00000019,
    rg11_b10_ufloat = 0x0000001a,
    rgb9_e5_ufloat = 0x0000001b,
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
    depth32_float = 0x00000029,
    depth32_float_stencil8 = 0x0000002a,
    bc1_rgba_unorm = 0x0000002b,
    bc1_rgba_unorm_srgb = 0x0000002c,
    bc2_rgba_unorm = 0x0000002d,
    bc2_rgba_unorm_srgb = 0x0000002e,
    bc3_rgba_unorm = 0x0000002f,
    bc3_rgba_unorm_srgb = 0x00000030,
    bc4_runorm = 0x00000031,
    bc4_rsnorm = 0x00000032,
    bc5_rg_unorm = 0x00000033,
    bc5_rg_snorm = 0x00000034,
    bc6_hrgb_ufloat = 0x00000035,
    bc6_hrgb_float = 0x00000036,
    bc7_rgba_unorm = 0x00000037,
    bc7_rgba_unorm_srgb = 0x00000038,
    etc2_rgb8_unorm = 0x00000039,
    etc2_rgb8_unorm_srgb = 0x0000003a,
    etc2_rgb8_a1_unorm = 0x0000003b,
    etc2_rgb8_a1_unorm_srgb = 0x0000003c,
    etc2_rgba8_unorm = 0x0000003d,
    etc2_rgba8_unorm_srgb = 0x0000003e,
    eacr11_unorm = 0x0000003f,
    eacr11_snorm = 0x00000040,
    eacrg11_unorm = 0x00000041,
    eacrg11_snorm = 0x00000042,
    astc4x4_unorm = 0x00000043,
    astc4x4_unorm_srgb = 0x00000044,
    astc5x4_unorm = 0x00000045,
    astc5x4_unorm_srgb = 0x00000046,
    astc5x5_unorm = 0x00000047,
    astc5x5_unorm_srgb = 0x00000048,
    astc6x5_unorm = 0x00000049,
    astc6x5_unorm_srgb = 0x0000004a,
    astc6x6_unorm = 0x0000004b,
    astc6x6_unorm_srgb = 0x0000004c,
    astc8x5_unorm = 0x0000004d,
    astc8x5_unorm_srgb = 0x0000004e,
    astc8x6_unorm = 0x0000004f,
    astc8x6_unorm_srgb = 0x00000050,
    astc8x8_unorm = 0x00000051,
    astc8x8_unorm_srgb = 0x00000052,
    astc10x5_unorm = 0x00000053,
    astc10x5_unorm_srgb = 0x00000054,
    astc10x6_unorm = 0x00000055,
    astc10x6_unorm_srgb = 0x00000056,
    astc10x8_unorm = 0x00000057,
    astc10x8_unorm_srgb = 0x00000058,
    astc10x10_unorm = 0x00000059,
    astc10x10_unorm_srgb = 0x0000005a,
    astc12x10_unorm = 0x0000005b,
    astc12x10_unorm_srgb = 0x0000005c,
    astc12x12_unorm = 0x0000005d,
    astc12x12_unorm_srgb = 0x0000005e,
    r8_bg8_biplanar420_unorm = 0x0000005f,
};

pub const TextureSampleType = enum(u32) {
    undef = 0x00000000,
    float = 0x00000001,
    unfilterable_float = 0x00000002,
    depth = 0x00000003,
    sint = 0x00000004,
    uint = 0x00000005,
};

pub const TextureViewDimension = enum(u32) {
    undef = 0x00000000,
    tvdim_1d = 0x00000001,
    tvdim_2d = 0x00000002,
    tvdim_2d_array = 0x00000003,
    tvdim_cube = 0x00000004,
    tvdim_cube_array = 0x00000005,
    tvdim_3d = 0x00000006,
};

pub const VertexFormat = enum(u32) {
    undef = 0x00000000,
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
    vertex_buffer_not_used = 0x00000002,
};

pub const BufferUsage = packed struct(u32) {
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
    _padding: u22 = 0,
};

pub const ColorWriteMask = packed struct(u32) {
    red: bool = false,
    green: bool = false,
    blue: bool = false,
    alpha: bool = false,
    _padding: u28 = 0,

    pub const all = ColorWriteMask{ .red = true, .green = true, .blue = true, .alpha = true };
};

pub const MapMode = packed struct(u32) {
    read: bool = false,
    write: bool = false,
    _padding: u30 = 0,
};

pub const ShaderStage = packed struct(u32) {
    vertex: bool = false,
    fragment: bool = false,
    compute: bool = false,
    _padding: u29 = 0,
};

pub const TextureUsage = packed struct(u32) {
    copy_src: bool = false,
    copy_dst: bool = false,
    texture_binding: bool = false,
    storage_binding: bool = false,
    render_attachment: bool = false,
    transient_attachment: bool = false,
    _padding: u26 = 0,
};

pub const ChainedStruct = extern struct {
    next: ?*const ChainedStruct,
    struct_type: StructType,
};

pub const ChainedStructOut = extern struct {
    next: ?*ChainedStructOut,
    struct_type: StructType,
};

pub const AdapterProperties = extern struct {
    next_in_chain: ?*ChainedStructOut = null,
    vendor_id: u32,
    vendor_name: [*:0]const u8,
    architecture: [*:0]const u8,
    device_id: u32,
    name: [*:0]const u8,
    driver_description: [*:0]const u8,
    adapter_type: AdapterType,
    backend_type: BackendType,
    compatibility_mode: bool,
};

pub const BindGroupEntry = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    binding: u32,
    buffer: ?Buffer = null,
    offset: u64 = 0,
    size: u64,
    sampler: ?Sampler = null,
    texture_view: ?TextureView = null,
};

pub const BindGroupDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    layout: BindGroupLayout,
    entry_count: usize,
    entries: ?[*]const BindGroupEntry,
};

pub const BufferBindingLayout = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    binding_type: BufferBindingType = .uniform,
    has_dynamic_offset: bool = false,
    min_binding_size: u64 = 0,
};

pub const SamplerBindingLayout = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    binding_type: SamplerBindingType = .filtering,
};

pub const TextureBindingLayout = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    sample_type: TextureSampleType = .float,
    view_dimension: TextureViewDimension = .tvdim_2d,
    multisampled: bool = false,
};

pub const StorageTextureBindingLayout = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    access: StorageTextureAccess = .write_only,
    format: TextureFormat,
    view_dimension: TextureViewDimension = .tvdim_2d,
};

pub const BindGroupLayoutEntry = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    binding: u32,
    visibility: ShaderStage,
    buffer: BufferBindingLayout = .{ .binding_type = .undef },
    sampler: SamplerBindingLayout = .{ .binding_type = .undef },
    texture: TextureBindingLayout = .{ .sample_type = .undef },
    storage_texture: StorageTextureBindingLayout = .{ .access = .undef, .format = .undef },
};

pub const BindGroupLayoutDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    entry_count: usize,
    entries: ?[*]const BindGroupLayoutEntry,
};

pub const BufferDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    usage: BufferUsage,
    size: u64,
    mapped_at_creation: bool = false,
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
    constant_count: usize = 0,
    constants: ?[*]const ConstantEntry = null,
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
    plane1: ?TextureView = null,
    visible_origin: Origin2D,
    visible_size: Extent2D,
    do_yuv_to_rgb_conversion_only: bool,
    yuv_to_rgb_conversion_matrix: ?[*]const f32,
    src_transfer_function_parameters: [*]const f32,
    dst_transfer_function_parameters: [*]const f32,
    gamut_conversion_matrix: [*]const f32,
    flip_y: bool,
    rotation: ExternalTextureRotation,
};

pub const PipelineLayoutDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    bind_group_layout_count: usize,
    bind_group_layouts: ?[*]const BindGroupLayout,
};

pub const QuerySetDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    query_type: QueryType,
    count: u32,
    pipeline_statistics: ?[*]const PipelineStatisticName,
    pipeline_statistics_count: usize,
};

pub const RenderBundleEncoderDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    color_formats_count: usize,
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
    step_mode: VertexStepMode = .vertex,
    attribute_count: usize,
    attributes: [*]const VertexAttribute,
};

pub const VertexState = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    module: ShaderModule,
    entry_point: [*:0]const u8,
    constant_count: usize = 0,
    constants: ?[*]const ConstantEntry = null,
    buffer_count: usize = 0,
    buffers: ?[*]const VertexBufferLayout = null,
};

pub const BlendComponent = extern struct {
    operation: BlendOperation = .add,
    src_factor: BlendFactor = .one,
    dst_factor: BlendFactor = .zero,
};

pub const BlendState = extern struct {
    color: BlendComponent,
    alpha: BlendComponent,
};

pub const ColorTargetState = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    format: TextureFormat,
    blend: ?*const BlendState = null,
    write_mask: ColorWriteMask = ColorWriteMask.all,
};

pub const FragmentState = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    module: ShaderModule,
    entry_point: [*:0]const u8,
    constant_count: usize = 0,
    constants: ?[*]const ConstantEntry = null,
    target_count: usize = 0,
    targets: ?[*]const ColorTargetState = null,
};

pub const PrimitiveState = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    topology: PrimitiveTopology = .triangle_list,
    strip_index_format: IndexFormat = .undef,
    front_face: FrontFace = .ccw,
    cull_mode: CullMode = .none,
};

pub const StencilFaceState = extern struct {
    compare: CompareFunction = .always,
    fail_op: StencilOperation = .keep,
    depth_fail_op: StencilOperation = .keep,
    pass_op: StencilOperation = .keep,
};

pub const DepthStencilState = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    format: TextureFormat,
    depth_write_enabled: bool = false,
    depth_compare: CompareFunction = .always,
    stencil_front: StencilFaceState = .{},
    stencil_back: StencilFaceState = .{},
    stencil_read_mask: u32 = 0xffff_ffff,
    stencil_write_mask: u32 = 0xffff_ffff,
    depth_bias: i32 = 0,
    depth_bias_slope_scale: f32 = 0.0,
    depth_bias_clamp: f32 = 0.0,
};

pub const MultisampleState = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    count: u32 = 1,
    mask: u32 = 0xffff_ffff,
    alpha_to_coverage_enabled: bool = false,
};

pub const RenderPipelineDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    layout: ?PipelineLayout = null,
    vertex: VertexState,
    primitive: PrimitiveState = .{},
    depth_stencil: ?*const DepthStencilState = null,
    multisample: MultisampleState = .{},
    fragment: ?*const FragmentState = null,
};

pub const SamplerDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    address_mode_u: AddressMode = .clamp_to_edge,
    address_mode_v: AddressMode = .clamp_to_edge,
    address_mode_w: AddressMode = .clamp_to_edge,
    mag_filter: FilterMode = .nearest,
    min_filter: FilterMode = .nearest,
    mipmap_filter: MipmapFilterMode = .nearest,
    lod_min_clamp: f32 = 0.0,
    lod_max_clamp: f32 = 32.0,
    compare: CompareFunction = .undef,
    max_anisotropy: u16 = 1,
};

pub const ShaderModuleDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
};

pub const ShaderModuleWGSLDescriptor = extern struct {
    chain: ChainedStruct,
    code: [*:0]const u8,
};

pub const SwapChainDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    usage: TextureUsage,
    format: TextureFormat,
    width: u32,
    height: u32,
    present_mode: PresentMode,
};

pub const Extent2D = extern struct {
    width: u32,
    height: u32 = 1,
};

pub const Extent3D = extern struct {
    width: u32,
    height: u32 = 1,
    depth_or_array_layers: u32 = 1,
};

pub const TextureDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    usage: TextureUsage,
    dimension: TextureDimension = .tdim_2d,
    size: Extent3D,
    format: TextureFormat,
    mip_level_count: u32 = 1,
    sample_count: u32 = 1,
    view_format_count: usize = 0,
    view_formats: ?[*]const TextureFormat = null,
};

pub const Limits = extern struct {
    const u32_undefined: u32 = 0xFFFFFFFF;
    const u64_undefined: u64 = 0xFFFFFFFFFFFFFFFF;

    max_texture_dimension_1d: u32 = u32_undefined,
    max_texture_dimension_2d: u32 = u32_undefined,
    max_texture_dimension_3d: u32 = u32_undefined,
    max_texture_array_layers: u32 = u32_undefined,
    max_bind_groups: u32 = u32_undefined,
    max_bind_groups_plus_vertex_buffers: u32 = u32_undefined,
    max_bindings_per_bind_group: u32 = u32_undefined,
    max_dynamic_uniform_buffers_per_pipeline_layout: u32 = u32_undefined,
    max_dynamic_storage_buffers_per_pipeline_layout: u32 = u32_undefined,
    max_sampled_textures_per_shader_stage: u32 = u32_undefined,
    max_samplers_per_shader_stage: u32 = u32_undefined,
    max_storage_buffers_per_shader_stage: u32 = u32_undefined,
    max_storage_textures_per_shader_stage: u32 = u32_undefined,
    max_uniform_buffers_per_shader_stage: u32 = u32_undefined,
    max_uniform_buffer_binding_size: u64 = u64_undefined,
    max_storage_buffer_binding_size: u64 = u64_undefined,
    min_uniform_buffer_offset_alignment: u32 = u32_undefined,
    min_storage_buffer_offset_alignment: u32 = u32_undefined,
    max_vertex_buffers: u32 = u32_undefined,
    max_buffer_size: u64 = u64_undefined,
    max_vertex_attributes: u32 = u32_undefined,
    max_vertex_buffer_array_stride: u32 = u32_undefined,
    max_inter_stage_shader_components: u32 = u32_undefined,
    max_inter_stage_shader_variables: u32 = u32_undefined,
    max_color_attachments: u32 = u32_undefined,
    max_color_attachment_bytes_per_sample: u32 = u32_undefined,
    max_compute_workgroup_storage_size: u32 = u32_undefined,
    max_compute_invocations_per_workgroup: u32 = u32_undefined,
    max_compute_workgroup_size_x: u32 = u32_undefined,
    max_compute_workgroup_size_y: u32 = u32_undefined,
    max_compute_workgroup_size_z: u32 = u32_undefined,
    max_compute_workgroups_per_dimension: u32 = u32_undefined,
};

pub const RequiredLimits = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    limits: Limits = .{},
};

pub const SupportedLimits = extern struct {
    next_in_chain: ?*ChainedStructOut = null,
    limits: Limits = .{},
};

pub const QueueDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
};

// Can be chained in InstanceDescriptor
// Can be chained in RequestAdapterOptions
// Can be chained in DeviceDescriptor
pub const DawnTogglesDescriptor = extern struct {
    chain: ChainedStruct,
    enabled_toggles_count: usize = 0,
    enabled_toggles: ?[*]const [*:0]const u8 = null,
    disabled_toggles_count: usize = 0,
    disabled_toggles: ?[*]const [*:0]const u8 = null,
};

pub const DawnAdapterPropertiesPowerPreference = extern struct {
    chain: ChainedStructOut,
    power_preference: PowerPreference,
};

pub const DeviceDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    required_features_count: usize = 0,
    required_features: ?[*]const FeatureName = null,
    required_limits: ?[*]const RequiredLimits = null,
    default_queue: QueueDescriptor = .{},
    device_lost_callback: ?DeviceLostCallback = null,
    device_lost_user_data: ?*anyopaque = null,
};

pub const SurfaceDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
};

pub const RequestAdapterOptions = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    compatible_surface: ?Surface = null,
    power_preference: PowerPreference,
    backend_type: BackendType = .undef,
    force_fallback_adapter: bool = false,
    compatibility_mode: bool = false,
};

pub const ComputePassTimestampWrite = extern struct {
    query_set: QuerySet,
    query_index: u32,
    location: ComputePassTimestampLocation,
};

pub const RenderPassTimestampWrite = extern struct {
    query_set: QuerySet,
    query_index: u32,
    location: RenderPassTimestampLocation,
};

pub const ComputePassDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    timestamp_write_count: usize,
    timestamp_writes: ?[*]const ComputePassTimestampWrite,
};

pub const Color = extern struct {
    r: f64,
    g: f64,
    b: f64,
    a: f64,
};

pub const RenderPassColorAttachment = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    view: ?TextureView,
    resolve_target: ?TextureView = null,
    load_op: LoadOp,
    store_op: StoreOp,
    clear_value: Color = .{ .r = 0.0, .g = 0.0, .b = 0.0, .a = 0.0 },
};

pub const RenderPassDepthStencilAttachment = extern struct {
    view: TextureView,
    depth_load_op: LoadOp = .undef,
    depth_store_op: StoreOp = .undef,
    depth_clear_value: f32 = 0.0,
    depth_read_only: bool = false,
    stencil_load_op: LoadOp = .undef,
    stencil_store_op: StoreOp = .undef,
    stencil_clear_value: u32 = 0,
    stencil_read_only: bool = false,
};

pub const RenderPassDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    color_attachment_count: usize,
    color_attachments: ?[*]const RenderPassColorAttachment,
    depth_stencil_attachment: ?*const RenderPassDepthStencilAttachment = null,
    occlusion_query_set: ?QuerySet = null,
    timestamp_write_count: usize = 0,
    timestamp_writes: ?[*]const RenderPassTimestampWrite = null,
};

pub const TextureDataLayout = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    offset: u64 = 0,
    bytes_per_row: u32,
    rows_per_image: u32,
};

pub const Origin2D = extern struct {
    x: u32 = 0,
    y: u32 = 0,
};

pub const Origin3D = extern struct {
    x: u32 = 0,
    y: u32 = 0,
    z: u32 = 0,
};

pub const ImageCopyBuffer = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    layout: TextureDataLayout,
    buffer: Buffer,
};

pub const ImageCopyTexture = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    texture: Texture,
    mip_level: u32 = 0,
    origin: Origin3D = .{},
    aspect: TextureAspect = .all,
};

pub const ImageCopyExternalTexture = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    external_texture: ExternalTexture,
    origin: Origin3D,
    natural_size: Extent2D,
};

pub const CommandBufferDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
};

pub const CopyTextureForBrowserOptions = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    flip_y: bool,
    needs_color_space_conversion: bool,
    src_alpha_mode: AlphaMode,
    src_transfer_function_parameters: ?[*]const f32,
    conversion_matrix: ?[*]const f32,
    dst_transfer_function_parameters: ?[*]const f32,
    dst_alpha_mode: AlphaMode,
    internal_usage: bool,
};

pub const TextureViewDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
    format: TextureFormat = .undef,
    dimension: TextureViewDimension = .undef,
    base_mip_level: u32 = 0,
    mip_level_count: u32 = 0xffff_ffff,
    base_array_layer: u32 = 0,
    array_layer_count: u32 = 0xffff_ffff,
    aspect: TextureAspect = .all,
};

pub const CompilationMessage = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    message: ?[*:0]const u8 = null,
    message_type: CompilationMessageType,
    line_num: u64,
    line_pos: u64,
    offset: u64,
    length: u64,
    utf16_line_pos: u64,
    utf16_offset: u64,
    utf16_length: u64,
};

pub const CompilationInfo = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    message_count: usize,
    messages: ?[*]const CompilationMessage,
};

pub const RenderBundleDescriptor = extern struct {
    next_in_chain: ?*const ChainedStruct = null,
    label: ?[*:0]const u8 = null,
};

pub const CreateComputePipelineAsyncCallback = *const fn (
    status: CreatePipelineAsyncStatus,
    pipeline: ComputePipeline,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const CreateRenderPipelineAsyncCallback = *const fn (
    status: CreatePipelineAsyncStatus,
    pipeline: RenderPipeline,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const ErrorCallback = *const fn (
    err_type: ErrorType,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const LoggingCallback = *const fn (
    log_type: LoggingType,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const DeviceLostCallback = *const fn (
    reason: DeviceLostReason,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const RequestAdapterCallback = *const fn (
    status: RequestAdapterStatus,
    adapter: Adapter,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const RequestDeviceCallback = *const fn (
    status: RequestDeviceStatus,
    device: Device,
    message: ?[*:0]const u8,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const BufferMapCallback = *const fn (
    status: BufferMapAsyncStatus,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const QueueWorkDoneCallback = *const fn (
    status: QueueWorkDoneStatus,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const CompilationInfoCallback = *const fn (
    status: CompilationInfoRequestStatus,
    info: *const CompilationInfo,
    userdata: ?*anyopaque,
) callconv(.C) void;

pub const Adapter = *opaque {
    pub fn createDevice(adapter: Adapter, descriptor: DeviceDescriptor) Device {
        return wgpuAdapterCreateDevice(adapter, &descriptor);
    }
    extern fn wgpuAdapterCreateDevice(adapter: Adapter, descriptor: *const DeviceDescriptor) Device;

    pub fn enumerateFeatures(adapter: Adapter, features: ?[*]FeatureName) usize {
        return wgpuAdapterEnumerateFeatures(adapter, features);
    }
    extern fn wgpuAdapterEnumerateFeatures(adapter: Adapter, features: ?[*]FeatureName) usize;

    pub fn getLimits(adapter: Adapter, limits: *SupportedLimits) bool {
        return wgpuAdapterGetLimits(adapter, limits);
    }
    extern fn wgpuAdapterGetLimits(adapter: Adapter, limits: *SupportedLimits) bool;

    pub fn getProperties(adapter: Adapter, properties: *AdapterProperties) void {
        wgpuAdapterGetProperties(adapter, properties);
    }
    extern fn wgpuAdapterGetProperties(adapter: Adapter, properties: *AdapterProperties) void;

    pub fn hasFeature(adapter: Adapter, feature: FeatureName) bool {
        return wgpuAdapterHasFeature(adapter, feature);
    }
    extern fn wgpuAdapterHasFeature(adapter: Adapter, feature: FeatureName) bool;

    pub fn requestDevice(
        adapter: Adapter,
        descriptor: DeviceDescriptor,
        callback: RequestDeviceCallback,
        userdata: ?*anyopaque,
    ) void {
        wgpuAdapterRequestDevice(adapter, &descriptor, callback, userdata);
    }
    extern fn wgpuAdapterRequestDevice(
        adapter: Adapter,
        descriptor: *const DeviceDescriptor,
        callback: RequestDeviceCallback,
        userdata: ?*anyopaque,
    ) void;

    pub fn reference(adapter: Adapter) void {
        wgpuAdapterReference(adapter);
    }
    extern fn wgpuAdapterReference(adapter: Adapter) void;

    pub fn release(adapter: Adapter) void {
        wgpuAdapterRelease(adapter);
    }
    extern fn wgpuAdapterRelease(adapter: Adapter) void;
};

pub const BindGroup = *opaque {
    pub fn setLabel(bind_group: BindGroup, label: ?[*:0]const u8) void {
        wgpuBindGroupSetLabel(bind_group, label);
    }
    extern fn wgpuBindGroupSetLabel(bind_group: BindGroup, label: ?[*:0]const u8) void;

    pub fn reference(bind_group: BindGroup) void {
        wgpuBindGroupReference(bind_group);
    }
    extern fn wgpuBindGroupReference(bind_group: BindGroup) void;

    pub fn release(bind_group: BindGroup) void {
        wgpuBindGroupRelease(bind_group);
    }
    extern fn wgpuBindGroupRelease(bind_group: BindGroup) void;
};

pub const BindGroupLayout = *opaque {
    pub fn setLabel(bind_group_layout: BindGroupLayout, label: ?[*:0]const u8) void {
        wgpuBindGroupLayoutSetLabel(bind_group_layout, label);
    }
    extern fn wgpuBindGroupLayoutSetLabel(bind_group_layout: BindGroupLayout, label: ?[*:0]const u8) void;

    pub fn reference(bind_group_layout: BindGroupLayout) void {
        wgpuBindGroupLayoutReference(bind_group_layout);
    }
    extern fn wgpuBindGroupLayoutReference(bind_group_layout: BindGroupLayout) void;

    pub fn release(bind_group_layout: BindGroupLayout) void {
        wgpuBindGroupLayoutRelease(bind_group_layout);
    }
    extern fn wgpuBindGroupLayoutRelease(bind_group_layout: BindGroupLayout) void;
};

pub const Buffer = *opaque {
    pub fn destroy(buffer: Buffer) void {
        wgpuBufferDestroy(buffer);
    }
    extern fn wgpuBufferDestroy(buffer: Buffer) void;

    // `offset` has to be a multiple of 8 (otherwise `null` will be returned).
    // `@sizeOf(T) * len` has to be a multiple of 4 (otherwise `null` will be returned).
    pub fn getConstMappedRange(buffer: Buffer, comptime T: type, offset: usize, len: usize) ?[]const T {
        if (len == 0) return null;
        const ptr = wgpuBufferGetConstMappedRange(buffer, offset, @sizeOf(T) * len);
        if (ptr == null) return null;
        return @as([*]const T, @ptrCast(@alignCast(ptr)))[0..len];
    }
    extern fn wgpuBufferGetConstMappedRange(buffer: Buffer, offset: usize, size: usize) ?*const anyopaque;

    // `offset` has to be a multiple of 8 (otherwise `null` will be returned).
    // `@sizeOf(T) * len` has to be a multiple of 4 (otherwise `null` will be returned).
    pub fn getMappedRange(buffer: Buffer, comptime T: type, offset: usize, len: usize) ?[]T {
        if (len == 0) return null;
        const ptr = wgpuBufferGetMappedRange(buffer, offset, @sizeOf(T) * len);
        if (ptr == null) return null;
        return @as([*]T, @ptrCast(@alignCast(ptr)))[0..len];
    }
    extern fn wgpuBufferGetMappedRange(buffer: Buffer, offset: usize, size: usize) ?*anyopaque;

    // `offset` has to be a multiple of 8 (Dawn's validation layer will warn).
    // `size` has to be a multiple of 4 (Dawn's validation layer will warn).
    // `size == 0` will map entire range (from 'offset' to the end of the buffer).
    pub fn mapAsync(
        buffer: Buffer,
        mode: MapMode,
        offset: usize,
        size: usize,
        callback: BufferMapCallback,
        userdata: ?*anyopaque,
    ) void {
        wgpuBufferMapAsync(buffer, mode, offset, size, callback, userdata);
    }
    extern fn wgpuBufferMapAsync(
        buffer: Buffer,
        mode: MapMode,
        offset: usize,
        size: usize,
        callback: BufferMapCallback,
        userdata: ?*anyopaque,
    ) void;

    pub fn setLabel(buffer: Buffer, label: ?[*:0]const u8) void {
        wgpuBufferSetLabel(buffer, label);
    }
    extern fn wgpuBufferSetLabel(buffer: Buffer, label: ?[*:0]const u8) void;

    pub fn unmap(buffer: Buffer) void {
        wgpuBufferUnmap(buffer);
    }
    extern fn wgpuBufferUnmap(buffer: Buffer) void;

    pub fn reference(buffer: Buffer) void {
        wgpuBufferReference(buffer);
    }
    extern fn wgpuBufferReference(buffer: Buffer) void;

    pub fn release(buffer: Buffer) void {
        wgpuBufferRelease(buffer);
    }
    extern fn wgpuBufferRelease(buffer: Buffer) void;
};

pub const CommandBuffer = *opaque {
    pub fn setLabel(command_buffer: CommandBuffer, label: ?[*:0]const u8) void {
        wgpuCommandBufferSetLabel(command_buffer, label);
    }
    extern fn wgpuCommandBufferSetLabel(command_buffer: CommandBuffer, label: ?[*:0]const u8) void;

    pub fn reference(command_buffer: CommandBuffer) void {
        wgpuCommandBufferReference(command_buffer);
    }
    extern fn wgpuCommandBufferReference(command_buffer: CommandBuffer) void;

    pub fn release(command_buffer: CommandBuffer) void {
        wgpuCommandBufferRelease(command_buffer);
    }
    extern fn wgpuCommandBufferRelease(command_buffer: CommandBuffer) void;
};

pub const CommandEncoder = *opaque {
    pub fn beginComputePass(
        command_encoder: CommandEncoder,
        descriptor: ?ComputePassDescriptor,
    ) ComputePassEncoder {
        return wgpuCommandEncoderBeginComputePass(command_encoder, if (descriptor) |d| &d else null);
    }
    extern fn wgpuCommandEncoderBeginComputePass(
        command_encoder: CommandEncoder,
        descriptor: ?*const ComputePassDescriptor,
    ) ComputePassEncoder;

    pub fn beginRenderPass(
        command_encoder: CommandEncoder,
        descriptor: RenderPassDescriptor,
    ) RenderPassEncoder {
        return wgpuCommandEncoderBeginRenderPass(command_encoder, &descriptor);
    }
    extern fn wgpuCommandEncoderBeginRenderPass(
        command_encoder: CommandEncoder,
        descriptor: *const RenderPassDescriptor,
    ) RenderPassEncoder;

    pub fn clearBuffer(command_encoder: CommandEncoder, buffer: Buffer, offset: usize, size: usize) void {
        wgpuCommandEncoderClearBuffer(command_encoder, buffer, offset, size);
    }
    extern fn wgpuCommandEncoderClearBuffer(
        command_encoder: CommandEncoder,
        buffer: Buffer,
        offset: usize,
        size: usize,
    ) void;

    pub fn copyBufferToBuffer(
        command_encoder: CommandEncoder,
        source: Buffer,
        source_offset: usize,
        destination: Buffer,
        destination_offset: usize,
        size: usize,
    ) void {
        wgpuCommandEncoderCopyBufferToBuffer(
            command_encoder,
            source,
            source_offset,
            destination,
            destination_offset,
            size,
        );
    }
    extern fn wgpuCommandEncoderCopyBufferToBuffer(
        command_encoder: CommandEncoder,
        source: Buffer,
        source_offset: usize,
        destination: Buffer,
        destination_offset: usize,
        size: usize,
    ) void;

    pub fn copyBufferToTexture(
        command_encoder: CommandEncoder,
        source: ImageCopyBuffer,
        destination: ImageCopyTexture,
        copy_size: Extent3D,
    ) void {
        wgpuCommandEncoderCopyBufferToTexture(command_encoder, &source, &destination, &copy_size);
    }
    extern fn wgpuCommandEncoderCopyBufferToTexture(
        command_encoder: CommandEncoder,
        source: *const ImageCopyBuffer,
        destination: *const ImageCopyTexture,
        copy_size: *const Extent3D,
    ) void;

    pub fn copyTextureToBuffer(
        command_encoder: CommandEncoder,
        source: ImageCopyTexture,
        destination: ImageCopyBuffer,
        copy_size: Extent3D,
    ) void {
        wgpuCommandEncoderCopyTextureToBuffer(command_encoder, &source, &destination, &copy_size);
    }
    extern fn wgpuCommandEncoderCopyTextureToBuffer(
        command_encoder: CommandEncoder,
        source: *const ImageCopyTexture,
        destination: *const ImageCopyBuffer,
        copy_size: *const Extent3D,
    ) void;

    pub fn copyTextureToTexture(
        command_encoder: CommandEncoder,
        source: ImageCopyTexture,
        destination: ImageCopyTexture,
        copy_size: Extent3D,
    ) void {
        wgpuCommandEncoderCopyTextureToTexture(command_encoder, &source, &destination, &copy_size);
    }
    extern fn wgpuCommandEncoderCopyTextureToTexture(
        command_encoder: CommandEncoder,
        source: *const ImageCopyTexture,
        destination: *const ImageCopyTexture,
        copy_size: *const Extent3D,
    ) void;

    pub fn finish(command_encoder: CommandEncoder, descriptor: ?CommandBufferDescriptor) CommandBuffer {
        return wgpuCommandEncoderFinish(command_encoder, if (descriptor) |d| &d else null);
    }
    extern fn wgpuCommandEncoderFinish(
        command_encoder: CommandEncoder,
        descriptor: ?*const CommandBufferDescriptor,
    ) CommandBuffer;

    pub fn injectValidationError(command_encoder: CommandEncoder, message: [*:0]const u8) void {
        wgpuCommandEncoderInjectValidationError(command_encoder, message);
    }
    extern fn wgpuCommandEncoderInjectValidationError(command_encoder: CommandEncoder, message: [*:0]const u8) void;

    pub fn insertDebugMarker(command_encoder: CommandEncoder, marker_label: [*:0]const u8) void {
        wgpuCommandEncoderInsertDebugMarker(command_encoder, marker_label);
    }
    extern fn wgpuCommandEncoderInsertDebugMarker(command_encoder: CommandEncoder, marker_label: [*:0]const u8) void;

    pub fn popDebugGroup(command_encoder: CommandEncoder) void {
        wgpuCommandEncoderPopDebugGroup(command_encoder);
    }
    extern fn wgpuCommandEncoderPopDebugGroup(command_encoder: CommandEncoder) void;

    pub fn pushDebugGroup(command_encoder: CommandEncoder, group_label: [*:0]const u8) void {
        wgpuCommandEncoderPushDebugGroup(command_encoder, group_label);
    }
    extern fn wgpuCommandEncoderPushDebugGroup(command_encoder: CommandEncoder, group_label: [*:0]const u8) void;

    pub fn resolveQuerySet(
        command_encoder: CommandEncoder,
        query_set: QuerySet,
        first_query: u32,
        query_count: u32,
        destination: Buffer,
        destination_offset: u64,
    ) void {
        wgpuCommandEncoderResolveQuerySet(
            command_encoder,
            query_set,
            first_query,
            query_count,
            destination,
            destination_offset,
        );
    }
    extern fn wgpuCommandEncoderResolveQuerySet(
        command_encoder: CommandEncoder,
        query_set: QuerySet,
        first_query: u32,
        query_count: u32,
        destination: Buffer,
        destination_offset: u64,
    ) void;

    pub fn setLabel(command_encoder: CommandEncoder, label: ?[*:0]const u8) void {
        wgpuCommandEncoderSetLabel(command_encoder, label);
    }
    extern fn wgpuCommandEncoderSetLabel(command_encoder: CommandEncoder, label: ?[*:0]const u8) void;

    pub fn writeBuffer(
        command_encoder: CommandEncoder,
        buffer: Buffer,
        buffer_offset: u64,
        comptime T: type,
        data: []const T,
    ) void {
        wgpuCommandEncoderWriteBuffer(
            command_encoder,
            buffer,
            buffer_offset,
            @as([*]const u8, @ptrCast(data.ptr)),
            @as(u64, @intCast(data.len)) * @sizeOf(T),
        );
    }
    extern fn wgpuCommandEncoderWriteBuffer(
        command_encoder: CommandEncoder,
        buffer: Buffer,
        buffer_offset: u64,
        data: [*]const u8,
        size: u64,
    ) void;

    pub fn writeTimestamp(command_encoder: CommandEncoder, query_set: QuerySet, query_index: u32) void {
        wgpuCommandEncoderWriteTimestamp(command_encoder, query_set, query_index);
    }
    extern fn wgpuCommandEncoderWriteTimestamp(
        command_encoder: CommandEncoder,
        query_set: QuerySet,
        query_index: u32,
    ) void;

    pub fn reference(command_encoder: CommandEncoder) void {
        wgpuCommandEncoderReference(command_encoder);
    }
    extern fn wgpuCommandEncoderReference(command_encoder: CommandEncoder) void;

    pub fn release(command_encoder: CommandEncoder) void {
        wgpuCommandEncoderRelease(command_encoder);
    }
    extern fn wgpuCommandEncoderRelease(command_encoder: CommandEncoder) void;
};

pub const ComputePassEncoder = *opaque {
    pub fn dispatchWorkgroups(
        compute_pass_encoder: ComputePassEncoder,
        workgroup_count_x: u32,
        workgroup_count_y: u32,
        workgroup_count_z: u32,
    ) void {
        wgpuComputePassEncoderDispatchWorkgroups(
            compute_pass_encoder,
            workgroup_count_x,
            workgroup_count_y,
            workgroup_count_z,
        );
    }
    extern fn wgpuComputePassEncoderDispatchWorkgroups(
        compute_pass_encoder: ComputePassEncoder,
        workgroup_count_x: u32,
        workgroup_count_y: u32,
        workgroup_count_z: u32,
    ) void;

    pub fn dispatchWorkgroupsIndirect(
        compute_pass_encoder: ComputePassEncoder,
        indirect_buffer: Buffer,
        indirect_offset: u64,
    ) void {
        wgpuComputePassEncoderDispatchWorkgroupsIndirect(compute_pass_encoder, indirect_buffer, indirect_offset);
    }
    extern fn wgpuComputePassEncoderDispatchWorkgroupsIndirect(
        compute_pass_encoder: ComputePassEncoder,
        indirect_buffer: Buffer,
        indirect_offset: u64,
    ) void;

    pub fn end(compute_pass_encoder: ComputePassEncoder) void {
        wgpuComputePassEncoderEnd(compute_pass_encoder);
    }
    extern fn wgpuComputePassEncoderEnd(compute_pass_encoder: ComputePassEncoder) void;

    pub fn insertDebugMarker(compute_pass_encoder: ComputePassEncoder, marker_label: [*:0]const u8) void {
        wgpuComputePassEncoderInsertDebugMarker(compute_pass_encoder, marker_label);
    }
    extern fn wgpuComputePassEncoderInsertDebugMarker(
        compute_pass_encoder: ComputePassEncoder,
        marker_label: [*:0]const u8,
    ) void;

    pub fn popDebugGroup(compute_pass_encoder: ComputePassEncoder) void {
        wgpuComputePassEncoderPopDebugGroup(compute_pass_encoder);
    }
    extern fn wgpuComputePassEncoderPopDebugGroup(compute_pass_encoder: ComputePassEncoder) void;

    pub fn pushDebugGroup(compute_pass_encoder: ComputePassEncoder, group_label: [*:0]const u8) void {
        wgpuComputePassEncoderPushDebugGroup(compute_pass_encoder, group_label);
    }
    extern fn wgpuComputePassEncoderPushDebugGroup(
        compute_pass_encoder: ComputePassEncoder,
        group_label: [*:0]const u8,
    ) void;

    pub fn setBindGroup(
        compute_pass_encoder: ComputePassEncoder,
        group_index: u32,
        bind_group: BindGroup,
        dynamic_offsets: ?[]const u32,
    ) void {
        wgpuComputePassEncoderSetBindGroup(
            compute_pass_encoder,
            group_index,
            bind_group,
            if (dynamic_offsets) |dynoff| @as(u32, @intCast(dynoff.len)) else 0,
            if (dynamic_offsets) |dynoff| dynoff.ptr else null,
        );
    }
    extern fn wgpuComputePassEncoderSetBindGroup(
        compute_pass_encoder: ComputePassEncoder,
        group_index: u32,
        bind_group: BindGroup,
        dynamic_offset_count: u32,
        dynamic_offsets: ?[*]const u32,
    ) void;

    pub fn setLabel(compute_pass_encoder: ComputePassEncoder, label: ?[*:0]const u8) void {
        wgpuComputePassEncoderSetLabel(compute_pass_encoder, label);
    }
    extern fn wgpuComputePassEncoderSetLabel(compute_pass_encoder: ComputePassEncoder, label: ?[*:0]const u8) void;

    pub fn setPipeline(compute_pass_encoder: ComputePassEncoder, pipeline: ComputePipeline) void {
        wgpuComputePassEncoderSetPipeline(compute_pass_encoder, pipeline);
    }
    extern fn wgpuComputePassEncoderSetPipeline(
        compute_pass_encoder: ComputePassEncoder,
        pipeline: ComputePipeline,
    ) void;

    pub fn writeTimestamp(
        compute_pass_encoder: ComputePassEncoder,
        query_set: QuerySet,
        query_index: u32,
    ) void {
        wgpuComputePassEncoderWriteTimestamp(compute_pass_encoder, query_set, query_index);
    }
    extern fn wgpuComputePassEncoderWriteTimestamp(
        compute_pass_encoder: ComputePassEncoder,
        query_set: QuerySet,
        query_index: u32,
    ) void;

    pub fn reference(compute_pass_encoder: ComputePassEncoder) void {
        wgpuComputePassEncoderReference(compute_pass_encoder);
    }
    extern fn wgpuComputePassEncoderReference(compute_pass_encoder: ComputePassEncoder) void;

    pub fn release(compute_pass_encoder: ComputePassEncoder) void {
        wgpuComputePassEncoderRelease(compute_pass_encoder);
    }
    extern fn wgpuComputePassEncoderRelease(compute_pass_encoder: ComputePassEncoder) void;
};

pub const ComputePipeline = *opaque {
    pub fn getBindGroupLayout(compute_pipeline: ComputePipeline, group_index: u32) BindGroupLayout {
        return wgpuComputePipelineGetBindGroupLayout(compute_pipeline, group_index);
    }
    extern fn wgpuComputePipelineGetBindGroupLayout(
        compute_pipeline: ComputePipeline,
        group_index: u32,
    ) BindGroupLayout;

    pub fn setLabel(compute_pipeline: ComputePipeline, label: ?[*:0]const u8) void {
        wgpuComputePipelineSetLabel(compute_pipeline, label);
    }
    extern fn wgpuComputePipelineSetLabel(compute_pipeline: ComputePipeline, label: ?[*:0]const u8) void;

    pub fn reference(compute_pipeline: ComputePipeline) void {
        wgpuComputePipelineReference(compute_pipeline);
    }
    extern fn wgpuComputePipelineReference(compute_pipeline: ComputePipeline) void;

    pub fn release(compute_pipeline: ComputePipeline) void {
        wgpuComputePipelineRelease(compute_pipeline);
    }
    extern fn wgpuComputePipelineRelease(compute_pipeline: ComputePipeline) void;
};

pub const Device = *opaque {
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

    pub fn createCommandEncoder(device: Device, descriptor: ?CommandEncoderDescriptor) CommandEncoder {
        return wgpuDeviceCreateCommandEncoder(device, if (descriptor) |d| &d else null);
    }
    extern fn wgpuDeviceCreateCommandEncoder(
        device: Device,
        descriptor: ?*const CommandEncoderDescriptor,
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

    pub fn createRenderBundleEncoder(
        device: Device,
        descriptor: RenderBundleEncoderDescriptor,
    ) RenderBundleEncoder {
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

    pub fn createShaderModule(device: Device, descriptor: ShaderModuleDescriptor) ShaderModule {
        return wgpuDeviceCreateShaderModule(device, &descriptor);
    }
    extern fn wgpuDeviceCreateShaderModule(device: Device, descriptor: *const ShaderModuleDescriptor) ShaderModule;

    pub fn createSwapChain(device: Device, surface: ?Surface, descriptor: SwapChainDescriptor) SwapChain {
        return wgpuDeviceCreateSwapChain(device, surface, &descriptor);
    }
    extern fn wgpuDeviceCreateSwapChain(
        device: Device,
        surface: ?Surface,
        descriptor: *const SwapChainDescriptor,
    ) SwapChain;

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

    pub fn injectError(device: Device, err_type: ErrorType, message: ?[*:0]const u8) void {
        wgpuDeviceInjectError(device, err_type, message);
    }
    extern fn wgpuDeviceInjectError(device: Device, err_type: ErrorType, message: ?[*:0]const u8) void;

    pub fn forceLoss(device: Device, reason: DeviceLostReason, message: ?[*:0]const u8) void {
        wgpuDeviceForceLoss(device, reason, message);
    }
    extern fn wgpuDeviceForceLoss(device: Device, reason: DeviceLostReason, message: ?[*:0]const u8) void;

    pub fn getAdapter(device: Device) Adapter {
        return wgpuDeviceGetAdapter(device);
    }
    extern fn wgpuDeviceGetAdapter(device: Device) Adapter;

    pub fn popErrorScope(device: Device, callback: ErrorCallback, userdata: ?*anyopaque) bool {
        return wgpuDevicePopErrorScope(device, callback, userdata);
    }
    extern fn wgpuDevicePopErrorScope(device: Device, callback: ErrorCallback, userdata: ?*anyopaque) bool;

    pub fn pushErrorScope(device: Device, filter: ErrorFilter) void {
        wgpuDevicePushErrorScope(device, filter);
    }
    extern fn wgpuDevicePushErrorScope(device: Device, filter: ErrorFilter) void;

    pub fn setDeviceLostCallback(
        device: Device,
        callback: DeviceLostCallback,
        userdata: ?*anyopaque,
    ) void {
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

pub const ExternalTexture = *opaque {
    pub fn destroy(external_texture: ExternalTexture) void {
        wgpuExternalTextureDestroy(external_texture);
    }
    extern fn wgpuExternalTextureDestroy(external_texture: ExternalTexture) void;

    pub fn setLabel(external_texture: ExternalTexture, label: ?[*:0]const u8) void {
        wgpuExternalTextureSetLabel(external_texture, label);
    }
    extern fn wgpuExternalTextureSetLabel(external_texture: ExternalTexture, label: ?[*:0]const u8) void;

    pub fn reference(external_texture: ExternalTexture) void {
        wgpuExternalTextureReference(external_texture);
    }
    extern fn wgpuExternalTextureReference(external_texture: ExternalTexture) void;

    pub fn release(external_texture: ExternalTexture) void {
        wgpuExternalTextureRelease(external_texture);
    }
    extern fn wgpuExternalTextureRelease(external_texture: ExternalTexture) void;
};

pub const Instance = *opaque {
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

pub const PipelineLayout = *opaque {
    pub fn setLabel(pipeline_layout: PipelineLayout, label: ?[*:0]const u8) void {
        wgpuPipelineLayoutSetLabel(pipeline_layout, label);
    }
    extern fn wgpuPipelineLayoutSetLabel(pipeline_layout: PipelineLayout, label: ?[*:0]const u8) void;

    pub fn reference(pipeline_layout: PipelineLayout) void {
        wgpuPipelineLayoutReference(pipeline_layout);
    }
    extern fn wgpuPipelineLayoutReference(pipeline_layout: PipelineLayout) void;

    pub fn release(pipeline_layout: PipelineLayout) void {
        wgpuPipelineLayoutRelease(pipeline_layout);
    }
    extern fn wgpuPipelineLayoutRelease(pipeline_layout: PipelineLayout) void;
};

pub const QuerySet = *opaque {
    pub fn destroy(query_set: QuerySet) void {
        wgpuQuerySetDestroy(query_set);
    }
    extern fn wgpuQuerySetDestroy(query_set: QuerySet) void;

    pub fn setLabel(query_set: QuerySet, label: ?[*:0]const u8) void {
        wgpuQuerySetSetLabel(query_set, label);
    }
    extern fn wgpuQuerySetSetLabel(query_set: QuerySet, label: ?[*:0]const u8) void;

    pub fn reference(query_set: QuerySet) void {
        wgpuQuerySetReference(query_set);
    }
    extern fn wgpuQuerySetReference(query_set: QuerySet) void;

    pub fn release(query_set: QuerySet) void {
        wgpuQuerySetRelease(query_set);
    }
    extern fn wgpuQuerySetRelease(query_set: QuerySet) void;
};

pub const Queue = *opaque {
    pub fn copyExternalTextureForBrowser(
        queue: Queue,
        source: ImageCopyExternalTexture,
        destination: ImageCopyTexture,
        copy_size: Extent3D,
        options: CopyTextureForBrowserOptions,
    ) void {
        wgpuQueueCopyExternalTextureForBrowser(queue, &source, &destination, &copy_size, &options);
    }
    extern fn wgpuQueueCopyExternalTextureForBrowser(
        queue: Queue,
        source: *const ImageCopyExternalTexture,
        destination: *const ImageCopyTexture,
        copy_size: *const Extent3D,
        options: *const CopyTextureForBrowserOptions,
    ) void;

    pub fn copyTextureForBrowser(
        queue: Queue,
        source: ImageCopyTexture,
        destination: ImageCopyTexture,
        copy_size: Extent3D,
        options: CopyTextureForBrowserOptions,
    ) void {
        wgpuQueueCopyTextureForBrowser(queue, &source, &destination, &copy_size, &options);
    }
    extern fn wgpuQueueCopyTextureForBrowser(
        queue: Queue,
        source: *const ImageCopyTexture,
        destination: *const ImageCopyTexture,
        copy_size: *const Extent3D,
        options: *const CopyTextureForBrowserOptions,
    ) void;

    pub fn onSubmittedWorkDone(
        queue: Queue,
        signal_value: u64,
        callback: QueueWorkDoneCallback,
        userdata: ?*anyopaque,
    ) void {
        wgpuQueueOnSubmittedWorkDone(queue, signal_value, callback, userdata);
    }
    extern fn wgpuQueueOnSubmittedWorkDone(
        queue: Queue,
        signal_value: u64,
        callback: QueueWorkDoneCallback,
        userdata: ?*anyopaque,
    ) void;

    pub fn setLabel(queue: Queue, label: ?[*:0]const u8) void {
        wgpuQueueSetLabel(queue, label);
    }
    extern fn wgpuQueueSetLabel(queue: Queue, label: ?[*:0]const u8) void;

    pub fn submit(queue: Queue, commands: []const CommandBuffer) void {
        wgpuQueueSubmit(queue, @as(u32, @intCast(commands.len)), commands.ptr);
    }
    extern fn wgpuQueueSubmit(queue: Queue, command_count: u32, commands: [*]const CommandBuffer) void;

    pub fn writeBuffer(
        queue: Queue,
        buffer: Buffer,
        buffer_offset: u64,
        comptime T: type,
        data: []const T,
    ) void {
        wgpuQueueWriteBuffer(
            queue,
            buffer,
            buffer_offset,
            @as(*const anyopaque, @ptrCast(data.ptr)),
            @as(u64, @intCast(data.len)) * @sizeOf(T),
        );
    }
    extern fn wgpuQueueWriteBuffer(
        queue: Queue,
        buffer: Buffer,
        buffer_offset: u64,
        data: *const anyopaque,
        size: u64,
    ) void;

    pub fn writeTexture(
        queue: Queue,
        destination: ImageCopyTexture,
        data_layout: TextureDataLayout,
        write_size: Extent3D,
        comptime T: type,
        data: []const T,
    ) void {
        wgpuQueueWriteTexture(
            queue,
            &destination,
            @as(*const anyopaque, @ptrCast(data.ptr)),
            @as(usize, @intCast(data.len)) * @sizeOf(T),
            &data_layout,
            &write_size,
        );
    }
    extern fn wgpuQueueWriteTexture(
        queue: Queue,
        destination: *const ImageCopyTexture,
        data: *const anyopaque,
        data_size: u64,
        data_layout: *const TextureDataLayout,
        write_size: *const Extent3D,
    ) void;

    pub fn reference(queue: Queue) void {
        wgpuQueueReference(queue);
    }
    extern fn wgpuQueueReference(queue: Queue) void;

    pub fn release(queue: Queue) void {
        wgpuQueueRelease(queue);
    }
    extern fn wgpuQueueRelease(queue: Queue) void;
};

pub const RenderBundle = *opaque {
    pub fn reference(render_bundle: RenderBundle) void {
        wgpuRenderBundleReference(render_bundle);
    }
    extern fn wgpuRenderBundleReference(render_bundle: RenderBundle) void;

    pub fn release(render_bundle: RenderBundle) void {
        wgpuRenderBundleRelease(render_bundle);
    }
    extern fn wgpuRenderBundleRelease(render_bundle: RenderBundle) void;
};

pub const RenderBundleEncoder = *opaque {
    pub fn draw(
        render_bundle_encoder: RenderBundleEncoder,
        vertex_count: u32,
        instance_count: u32,
        first_vertex: u32,
        first_instance: u32,
    ) void {
        wgpuRenderBundleEncoderDraw(
            render_bundle_encoder,
            vertex_count,
            instance_count,
            first_vertex,
            first_instance,
        );
    }
    extern fn wgpuRenderBundleEncoderDraw(
        render_bundle_encoder: RenderBundleEncoder,
        vertex_count: u32,
        instance_count: u32,
        first_vertex: u32,
        first_instance: u32,
    ) void;

    pub fn drawIndexed(
        render_bundle_encoder: RenderBundleEncoder,
        index_count: u32,
        instance_count: u32,
        first_index: u32,
        base_vertex: i32,
        first_instance: u32,
    ) void {
        wgpuRenderBundleEncoderDrawIndexed(
            render_bundle_encoder,
            index_count,
            instance_count,
            first_index,
            base_vertex,
            first_instance,
        );
    }
    extern fn wgpuRenderBundleEncoderDrawIndexed(
        render_bundle_encoder: RenderBundleEncoder,
        index_count: u32,
        instance_count: u32,
        first_index: u32,
        base_vertex: i32,
        first_instance: u32,
    ) void;

    pub fn drawIndexedIndirect(
        render_bundle_encoder: RenderBundleEncoder,
        indirect_buffer: Buffer,
        indirect_offset: u64,
    ) void {
        wgpuRenderBundleEncoderDrawIndexedIndirect(render_bundle_encoder, indirect_buffer, indirect_offset);
    }
    extern fn wgpuRenderBundleEncoderDrawIndexedIndirect(
        render_bundle_encoder: RenderBundleEncoder,
        indirect_buffer: Buffer,
        indirect_offset: u64,
    ) void;

    pub fn drawIndirect(
        render_bundle_encoder: RenderBundleEncoder,
        indirect_buffer: Buffer,
        indirect_offset: u64,
    ) void {
        wgpuRenderBundleEncoderDrawIndirect(render_bundle_encoder, indirect_buffer, indirect_offset);
    }
    extern fn wgpuRenderBundleEncoderDrawIndirect(
        render_bundle_encoder: RenderBundleEncoder,
        indirect_buffer: Buffer,
        indirect_offset: u64,
    ) void;

    pub fn finish(
        render_bundle_encoder: RenderBundleEncoder,
        descriptor: RenderBundleDescriptor,
    ) RenderBundle {
        return wgpuRenderBundleEncoderFinish(render_bundle_encoder, &descriptor);
    }
    extern fn wgpuRenderBundleEncoderFinish(
        render_bundle_encoder: RenderBundleEncoder,
        descriptor: *const RenderBundleDescriptor,
    ) RenderBundle;

    pub fn insertDebugMarker(
        render_bundle_encoder: RenderBundleEncoder,
        marker_label: [*:0]const u8,
    ) void {
        wgpuRenderBundleEncoderInsertDebugMarker(render_bundle_encoder, marker_label);
    }
    extern fn wgpuRenderBundleEncoderInsertDebugMarker(
        render_bundle_encoder: RenderBundleEncoder,
        marker_label: [*:0]const u8,
    ) void;

    pub fn popDebugGroup(render_bundle_encoder: RenderBundleEncoder) void {
        wgpuRenderBundleEncoderPopDebugGroup(render_bundle_encoder);
    }
    extern fn wgpuRenderBundleEncoderPopDebugGroup(render_bundle_encoder: RenderBundleEncoder) void;

    pub fn pushDebugGroup(render_bundle_encoder: RenderBundleEncoder, group_label: [*:0]const u8) void {
        wgpuRenderBundleEncoderPushDebugGroup(render_bundle_encoder, group_label);
    }
    extern fn wgpuRenderBundleEncoderPushDebugGroup(
        render_bundle_encoder: RenderBundleEncoder,
        group_label: [*:0]const u8,
    ) void;

    pub fn setBindGroup(
        render_bundle_encoder: RenderBundleEncoder,
        group_index: u32,
        group: BindGroup,
        dynamic_offsets: ?[]const u32,
    ) void {
        wgpuRenderBundleEncoderSetBindGroup(
            render_bundle_encoder,
            group_index,
            group,
            if (dynamic_offsets) |dynoff| @as(u32, @intCast(dynoff.len)) else 0,
            if (dynamic_offsets) |dynoff| dynoff.ptr else null,
        );
    }
    extern fn wgpuRenderBundleEncoderSetBindGroup(
        render_bundle_encoder: RenderBundleEncoder,
        group_index: u32,
        group: BindGroup,
        dynamic_offset_count: u32,
        dynamic_offsets: ?[*]const u32,
    ) void;

    pub fn setIndexBuffer(
        render_bundle_encoder: RenderBundleEncoder,
        buffer: Buffer,
        format: IndexFormat,
        offset: u64,
        size: u64,
    ) void {
        wgpuRenderBundleEncoderSetIndexBuffer(render_bundle_encoder, buffer, format, offset, size);
    }
    extern fn wgpuRenderBundleEncoderSetIndexBuffer(
        render_bundle_encoder: RenderBundleEncoder,
        buffer: Buffer,
        format: IndexFormat,
        offset: u64,
        size: u64,
    ) void;

    pub fn setLabel(render_bundle_encoder: RenderBundleEncoder, label: ?[*:0]const u8) void {
        wgpuRenderBundleEncoderSetLabel(render_bundle_encoder, label);
    }
    extern fn wgpuRenderBundleEncoderSetLabel(
        render_bundle_encoder: RenderBundleEncoder,
        label: ?[*:0]const u8,
    ) void;

    pub fn setPipeline(render_bundle_encoder: RenderBundleEncoder, pipeline: RenderPipeline) void {
        wgpuRenderBundleEncoderSetPipeline(render_bundle_encoder, pipeline);
    }
    extern fn wgpuRenderBundleEncoderSetPipeline(
        render_bundle_encoder: RenderBundleEncoder,
        pipeline: RenderPipeline,
    ) void;

    pub fn setVertexBuffer(
        render_bundle_encoder: RenderBundleEncoder,
        slot: u32,
        buffer: Buffer,
        offset: u64,
        size: u64,
    ) void {
        wgpuRenderBundleEncoderSetVertexBuffer(render_bundle_encoder, slot, buffer, offset, size);
    }
    extern fn wgpuRenderBundleEncoderSetVertexBuffer(
        render_bundle_encoder: RenderBundleEncoder,
        slot: u32,
        buffer: Buffer,
        offset: u64,
        size: u64,
    ) void;

    pub fn reference(render_bundle_encoder: RenderBundleEncoder) void {
        wgpuRenderBundleEncoderReference(render_bundle_encoder);
    }
    extern fn wgpuRenderBundleEncoderReference(render_bundle_encoder: RenderBundleEncoder) void;

    pub fn release(render_bundle_encoder: RenderBundleEncoder) void {
        wgpuRenderBundleEncoderRelease(render_bundle_encoder);
    }
    extern fn wgpuRenderBundleEncoderRelease(render_bundle_encoder: RenderBundleEncoder) void;
};

pub const RenderPassEncoder = *opaque {
    pub fn beginOcclusionQuery(render_pass_encoder: RenderPassEncoder, query_index: u32) void {
        wgpuRenderPassEncoderBeginOcclusionQuery(render_pass_encoder, query_index);
    }
    extern fn wgpuRenderPassEncoderBeginOcclusionQuery(
        render_pass_encoder: RenderPassEncoder,
        query_index: u32,
    ) void;

    pub fn draw(
        render_pass_encoder: RenderPassEncoder,
        vertex_count: u32,
        instance_count: u32,
        first_vertex: u32,
        first_instance: u32,
    ) void {
        wgpuRenderPassEncoderDraw(render_pass_encoder, vertex_count, instance_count, first_vertex, first_instance);
    }
    extern fn wgpuRenderPassEncoderDraw(
        render_pass_encoder: RenderPassEncoder,
        vertex_count: u32,
        instance_count: u32,
        first_vertex: u32,
        first_instance: u32,
    ) void;

    pub fn drawIndexed(
        render_pass_encoder: RenderPassEncoder,
        index_count: u32,
        instance_count: u32,
        first_index: u32,
        base_vertex: i32,
        first_instance: u32,
    ) void {
        wgpuRenderPassEncoderDrawIndexed(
            render_pass_encoder,
            index_count,
            instance_count,
            first_index,
            base_vertex,
            first_instance,
        );
    }
    extern fn wgpuRenderPassEncoderDrawIndexed(
        render_pass_encoder: RenderPassEncoder,
        index_count: u32,
        instance_count: u32,
        first_index: u32,
        base_vertex: i32,
        first_instance: u32,
    ) void;

    pub fn drawIndexedIndirect(
        render_pass_encoder: RenderPassEncoder,
        indirect_buffer: Buffer,
        indirect_offset: u64,
    ) void {
        wgpuRenderPassEncoderDrawIndexedIndirect(render_pass_encoder, indirect_buffer, indirect_offset);
    }
    extern fn wgpuRenderPassEncoderDrawIndexedIndirect(
        render_pass_encoder: RenderPassEncoder,
        indirect_buffer: Buffer,
        indirect_offset: u64,
    ) void;

    pub fn drawIndirect(
        render_pass_encoder: RenderPassEncoder,
        indirect_buffer: Buffer,
        indirect_offset: u64,
    ) void {
        wgpuRenderPassEncoderDrawIndirect(render_pass_encoder, indirect_buffer, indirect_offset);
    }
    extern fn wgpuRenderPassEncoderDrawIndirect(
        render_pass_encoder: RenderPassEncoder,
        indirect_buffer: Buffer,
        indirect_offset: u64,
    ) void;

    pub fn end(render_pass_encoder: RenderPassEncoder) void {
        wgpuRenderPassEncoderEnd(render_pass_encoder);
    }
    extern fn wgpuRenderPassEncoderEnd(render_pass_encoder: RenderPassEncoder) void;

    pub fn endOcclusionQuery(render_pass_encoder: RenderPassEncoder) void {
        wgpuRenderPassEncoderEndOcclusionQuery(render_pass_encoder);
    }
    extern fn wgpuRenderPassEncoderEndOcclusionQuery(render_pass_encoder: RenderPassEncoder) void;

    pub fn executeBundles(
        render_pass_encoder: RenderPassEncoder,
        bundle_count: u32,
        bundles: [*]const RenderBundle,
    ) void {
        wgpuRenderPassEncoderExecuteBundles(render_pass_encoder, bundle_count, bundles);
    }
    extern fn wgpuRenderPassEncoderExecuteBundles(
        render_pass_encoder: RenderPassEncoder,
        bundle_count: u32,
        bundles: [*]const RenderBundle,
    ) void;

    pub fn insertDebugMarker(render_pass_encoder: RenderPassEncoder, marker_label: [*:0]const u8) void {
        wgpuRenderPassEncoderInsertDebugMarker(render_pass_encoder, marker_label);
    }
    extern fn wgpuRenderPassEncoderInsertDebugMarker(
        render_pass_encoder: RenderPassEncoder,
        marker_label: [*:0]const u8,
    ) void;

    pub fn popDebugGroup(render_pass_encoder: RenderPassEncoder) void {
        wgpuRenderPassEncoderPopDebugGroup(render_pass_encoder);
    }
    extern fn wgpuRenderPassEncoderPopDebugGroup(render_pass_encoder: RenderPassEncoder) void;

    pub fn pushDebugGroup(render_pass_encoder: RenderPassEncoder, group_label: [*:0]const u8) void {
        wgpuRenderPassEncoderPushDebugGroup(render_pass_encoder, group_label);
    }
    extern fn wgpuRenderPassEncoderPushDebugGroup(
        render_pass_encoder: RenderPassEncoder,
        group_label: [*:0]const u8,
    ) void;

    pub fn setBindGroup(
        render_pass_encoder: RenderPassEncoder,
        group_index: u32,
        group: BindGroup,
        dynamic_offsets: ?[]const u32,
    ) void {
        wgpuRenderPassEncoderSetBindGroup(
            render_pass_encoder,
            group_index,
            group,
            if (dynamic_offsets) |dynoff| @as(u32, @intCast(dynoff.len)) else 0,
            if (dynamic_offsets) |dynoff| dynoff.ptr else null,
        );
    }
    extern fn wgpuRenderPassEncoderSetBindGroup(
        render_pass_encoder: RenderPassEncoder,
        group_index: u32,
        group: BindGroup,
        dynamic_offset_count: u32,
        dynamic_offsets: ?[*]const u32,
    ) void;

    pub fn setBlendConstant(render_pass_encoder: RenderPassEncoder, color: Color) void {
        wgpuRenderPassEncoderSetBlendConstant(render_pass_encoder, &color);
    }
    extern fn wgpuRenderPassEncoderSetBlendConstant(
        render_pass_encoder: RenderPassEncoder,
        color: *const Color,
    ) void;

    pub fn setIndexBuffer(
        render_pass_encoder: RenderPassEncoder,
        buffer: Buffer,
        format: IndexFormat,
        offset: u64,
        size: u64,
    ) void {
        wgpuRenderPassEncoderSetIndexBuffer(render_pass_encoder, buffer, format, offset, size);
    }
    extern fn wgpuRenderPassEncoderSetIndexBuffer(
        render_pass_encoder: RenderPassEncoder,
        buffer: Buffer,
        format: IndexFormat,
        offset: u64,
        size: u64,
    ) void;

    pub fn setLabel(render_pass_encoder: RenderPassEncoder, label: ?[*:0]const u8) void {
        wgpuRenderPassEncoderSetLabel(render_pass_encoder, label);
    }
    extern fn wgpuRenderPassEncoderSetLabel(render_pass_encoder: RenderPassEncoder, label: ?[*:0]const u8) void;

    pub fn setPipeline(render_pass_encoder: RenderPassEncoder, pipeline: RenderPipeline) void {
        wgpuRenderPassEncoderSetPipeline(render_pass_encoder, pipeline);
    }
    extern fn wgpuRenderPassEncoderSetPipeline(
        render_pass_encoder: RenderPassEncoder,
        pipeline: RenderPipeline,
    ) void;

    pub fn setScissorRect(
        render_pass_encoder: RenderPassEncoder,
        x: u32,
        y: u32,
        width: u32,
        height: u32,
    ) void {
        wgpuRenderPassEncoderSetScissorRect(render_pass_encoder, x, y, width, height);
    }
    extern fn wgpuRenderPassEncoderSetScissorRect(
        render_pass_encoder: RenderPassEncoder,
        x: u32,
        y: u32,
        width: u32,
        height: u32,
    ) void;

    pub fn setStencilReference(render_pass_encoder: RenderPassEncoder, ref: u32) void {
        wgpuRenderPassEncoderSetStencilReference(render_pass_encoder, ref);
    }
    extern fn wgpuRenderPassEncoderSetStencilReference(render_pass_encoder: RenderPassEncoder, ref: u32) void;

    pub fn setVertexBuffer(
        render_pass_encoder: RenderPassEncoder,
        slot: u32,
        buffer: Buffer,
        offset: u64,
        size: u64,
    ) void {
        wgpuRenderPassEncoderSetVertexBuffer(render_pass_encoder, slot, buffer, offset, size);
    }
    extern fn wgpuRenderPassEncoderSetVertexBuffer(
        render_pass_encoder: RenderPassEncoder,
        slot: u32,
        buffer: Buffer,
        offset: u64,
        size: u64,
    ) void;

    pub fn setViewport(
        render_pass_encoder: RenderPassEncoder,
        x: f32,
        y: f32,
        width: f32,
        height: f32,
        min_depth: f32,
        max_depth: f32,
    ) void {
        wgpuRenderPassEncoderSetViewport(render_pass_encoder, x, y, width, height, min_depth, max_depth);
    }
    extern fn wgpuRenderPassEncoderSetViewport(
        render_pass_encoder: RenderPassEncoder,
        x: f32,
        y: f32,
        width: f32,
        height: f32,
        min_depth: f32,
        max_depth: f32,
    ) void;

    pub fn writeTimestamp(
        render_pass_encoder: RenderPassEncoder,
        query_set: QuerySet,
        query_index: u32,
    ) void {
        wgpuRenderPassEncoderWriteTimestamp(render_pass_encoder, query_set, query_index);
    }
    extern fn wgpuRenderPassEncoderWriteTimestamp(
        render_pass_encoder: RenderPassEncoder,
        query_set: QuerySet,
        query_index: u32,
    ) void;

    pub fn reference(render_pass_encoder: RenderPassEncoder) void {
        wgpuRenderPassEncoderReference(render_pass_encoder);
    }
    extern fn wgpuRenderPassEncoderReference(render_pass_encoder: RenderPassEncoder) void;

    pub fn release(render_pass_encoder: RenderPassEncoder) void {
        wgpuRenderPassEncoderRelease(render_pass_encoder);
    }
    extern fn wgpuRenderPassEncoderRelease(render_pass_encoder: RenderPassEncoder) void;
};

pub const RenderPipeline = *opaque {
    pub fn getBindGroupLayout(render_pipeline: RenderPipeline, group_index: u32) BindGroupLayout {
        return wgpuRenderPipelineGetBindGroupLayout(render_pipeline, group_index);
    }
    extern fn wgpuRenderPipelineGetBindGroupLayout(
        render_pipeline: RenderPipeline,
        group_index: u32,
    ) BindGroupLayout;

    pub fn setLabel(render_pipeline: RenderPipeline, label: ?[*:0]const u8) void {
        wgpuRenderPipelineSetLabel(render_pipeline, label);
    }
    extern fn wgpuRenderPipelineSetLabel(render_pipeline: RenderPipeline, label: ?[*:0]const u8) void;

    pub fn reference(render_pipeline: RenderPipeline) void {
        wgpuRenderPipelineReference(render_pipeline);
    }
    extern fn wgpuRenderPipelineReference(render_pipeline: RenderPipeline) void;

    pub fn release(render_pipeline: RenderPipeline) void {
        wgpuRenderPipelineRelease(render_pipeline);
    }
    extern fn wgpuRenderPipelineRelease(render_pipeline: RenderPipeline) void;
};

pub const Sampler = *opaque {
    pub fn setLabel(sampler: Sampler, label: ?[*:0]const u8) void {
        wgpuSamplerSetLabel(sampler, label);
    }
    extern fn wgpuSamplerSetLabel(sampler: Sampler, label: ?[*:0]const u8) void;

    pub fn reference(sampler: Sampler) void {
        wgpuSamplerReference(sampler);
    }
    extern fn wgpuSamplerReference(sampler: Sampler) void;

    pub fn release(sampler: Sampler) void {
        wgpuSamplerRelease(sampler);
    }
    extern fn wgpuSamplerRelease(sampler: Sampler) void;
};

pub const ShaderModule = *opaque {
    pub fn getCompilationInfo(
        shader_module: ShaderModule,
        callback: CompilationInfoCallback,
        userdata: ?*anyopaque,
    ) void {
        wgpuShaderModuleGetCompilationInfo(shader_module, callback, userdata);
    }
    extern fn wgpuShaderModuleGetCompilationInfo(
        shader_module: ShaderModule,
        callback: CompilationInfoCallback,
        userdata: ?*anyopaque,
    ) void;

    pub fn setLabel(shader_module: ShaderModule, label: ?[*:0]const u8) void {
        wgpuShaderModuleSetLabel(shader_module, label);
    }
    extern fn wgpuShaderModuleSetLabel(shader_module: ShaderModule, label: ?[*:0]const u8) void;

    pub fn reference(shader_module: ShaderModule) void {
        wgpuShaderModuleReference(shader_module);
    }
    extern fn wgpuShaderModuleReference(shader_module: ShaderModule) void;

    pub fn release(shader_module: ShaderModule) void {
        wgpuShaderModuleRelease(shader_module);
    }
    extern fn wgpuShaderModuleRelease(shader_module: ShaderModule) void;
};

pub const Surface = *opaque {
    pub fn reference(surface: Surface) void {
        wgpuSurfaceReference(surface);
    }
    extern fn wgpuSurfaceReference(surface: Surface) void;

    pub fn release(surface: Surface) void {
        wgpuSurfaceRelease(surface);
    }
    extern fn wgpuSurfaceRelease(surface: Surface) void;
};

pub const SwapChain = *opaque {
    pub fn configure(
        swap_chain: SwapChain,
        format: TextureFormat,
        allowed_usage: TextureUsage,
        width: u32,
        height: u32,
    ) void {
        wgpuSwapChainConfigure(swap_chain, format, allowed_usage, width, height);
    }
    extern fn wgpuSwapChainConfigure(
        swap_chain: SwapChain,
        format: TextureFormat,
        allowed_usage: TextureUsage,
        width: u32,
        height: u32,
    ) void;

    pub fn getCurrentTextureView(swap_chain: SwapChain) TextureView {
        return wgpuSwapChainGetCurrentTextureView(swap_chain);
    }
    extern fn wgpuSwapChainGetCurrentTextureView(swap_chain: SwapChain) TextureView;

    pub fn present(swap_chain: SwapChain) void {
        wgpuSwapChainPresent(swap_chain);
    }
    extern fn wgpuSwapChainPresent(swap_chain: SwapChain) void;

    pub fn reference(swap_chain: SwapChain) void {
        wgpuSwapChainReference(swap_chain);
    }
    extern fn wgpuSwapChainReference(swap_chain: SwapChain) void;

    pub fn release(swap_chain: SwapChain) void {
        wgpuSwapChainRelease(swap_chain);
    }
    extern fn wgpuSwapChainRelease(swap_chain: SwapChain) void;
};

pub const Texture = *opaque {
    pub fn createView(texture: Texture, descriptor: TextureViewDescriptor) TextureView {
        return wgpuTextureCreateView(texture, &descriptor);
    }
    extern fn wgpuTextureCreateView(texture: Texture, descriptor: *const TextureViewDescriptor) TextureView;

    pub fn destroy(texture: Texture) void {
        wgpuTextureDestroy(texture);
    }
    extern fn wgpuTextureDestroy(texture: Texture) void;

    pub fn setLabel(texture: Texture, label: ?[*:0]const u8) void {
        wgpuTextureSetLabel(texture, label);
    }
    extern fn wgpuTextureSetLabel(texture: Texture, label: ?[*:0]const u8) void;

    pub fn reference(texture: Texture) void {
        wgpuTextureReference(texture);
    }
    extern fn wgpuTextureReference(texture: Texture) void;

    pub fn release(texture: Texture) void {
        wgpuTextureRelease(texture);
    }
    extern fn wgpuTextureRelease(texture: Texture) void;
};

pub const TextureView = *opaque {
    pub fn setLabel(texture_view: TextureView, label: ?[*:0]const u8) void {
        wgpuTextureViewSetLabel(texture_view, label);
    }
    extern fn wgpuTextureViewSetLabel(texture_view: TextureView, label: ?[*:0]const u8) void;

    pub fn reference(texture_view: TextureView) void {
        wgpuTextureViewReference(texture_view);
    }
    extern fn wgpuTextureViewReference(texture_view: TextureView) void;

    pub fn release(texture_view: TextureView) void {
        wgpuTextureViewRelease(texture_view);
    }
    extern fn wgpuTextureViewRelease(texture_view: TextureView) void;
};
