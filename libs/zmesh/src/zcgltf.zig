const builtin = @import("builtin");
const std = @import("std");
const assert = std.debug.assert;

pub const Bool32 = i32;
pub const CString = [*:0]const u8;
pub const MutCString = [*:0]u8;

pub const FileType = enum(c_int) {
    invalid,
    gltf,
    glb,
};

pub const Result = enum(c_int) {
    success,
    data_too_short,
    unknown_format,
    invalid_json,
    invalid_gltf,
    invalid_options,
    file_not_found,
    io_error,
    out_of_memory,
    legacy_gltf,
};

const MallocFn = *const fn (user: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque;
const FreeFn = *const fn (user: ?*anyopaque, ptr: ?*anyopaque) callconv(.C) void;

pub const MemoryOptions = extern struct {
    alloc_func: ?MallocFn = null,
    free_func: ?FreeFn = null,
    user_data: ?*anyopaque = null,
};

pub const FileOptions = extern struct {
    const ReadFn = *const fn (
        *const MemoryOptions,
        *const FileOptions,
        CString,
        *usize,
        *?*anyopaque,
    ) callconv(.C) Result;

    const ReleaseFn = *const fn (*const MemoryOptions, *const FileOptions, ?*anyopaque) callconv(.C) void;

    read: ?ReadFn = null,
    release: ?ReleaseFn = null,
    user_data: ?*anyopaque = null,
};

pub const Options = extern struct {
    file_type: FileType = .invalid,
    json_token_count: usize = 0,
    memory: MemoryOptions = .{},
    file: FileOptions = .{},
};

pub const BufferViewType = enum(c_int) {
    invalid,
    indices,
    vertices,
};

pub const AttributeType = enum(c_int) {
    invalid,
    position,
    normal,
    tangent,
    texcoord,
    color,
    joints,
    weights,
    custom,
};

pub const ComponentType = enum(c_int) {
    invalid,
    r_8,
    r_8u,
    r_16,
    r_16u,
    r_32u,
    r_32f,
};

pub const Type = enum(c_int) {
    invalid,
    scalar,
    vec2,
    vec3,
    vec4,
    mat2,
    mat3,
    mat4,

    pub fn numComponents(dtype: Type) usize {
        return switch (dtype) {
            .vec2 => 2,
            .vec3 => 3,
            .vec4 => 4,
            .mat2 => 4,
            .mat3 => 9,
            .mat4 => 16,
            else => 1,
        };
    }
};

pub const PrimitiveType = enum(c_int) {
    points,
    lines,
    line_loop,
    line_strip,
    triangles,
    triangle_strip,
    triangle_fan,
};

pub const AlphaMode = enum(c_int) {
    @"opaque",
    mask,
    blend,
};

pub const AnimationPathType = enum(c_int) {
    invalid,
    translation,
    rotation,
    scale,
    weights,
};

pub const InterpolationType = enum(c_int) {
    linear,
    step,
    cubic_spline,
};

pub const CameraType = enum(c_int) {
    invalid,
    perspective,
    orthographic,
};

pub const LightType = enum(c_int) {
    invalid,
    directional,
    point,
    spot,
};

pub const DataFreeMethod = enum(c_int) {
    none,
    file_release,
    memory_free,
};

pub const MeshoptCompressionMode = enum(c_int) {
    invalid,
    attributes,
    triangles,
    indices,
};

pub const MeshoptCompressionFilter = enum(c_int) {
    none,
    octahedral,
    quaternion,
    exponential,
};

pub const Extras = extern struct {
    start_offset: usize, // DEPRECATED: Please use `data` instead.
    end_offset: usize, // DEPRECATED: Please use `data` instead.

    data: ?[*]u8,
};

pub const Extension = extern struct {
    name: ?MutCString,
    data: ?MutCString,
};

pub const Buffer = extern struct {
    name: ?MutCString,
    size: usize,
    uri: ?MutCString,
    data: ?*anyopaque, // loaded by loadBuffers()
    data_free_method: DataFreeMethod,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const MeshoptCompression = extern struct {
    buffer: *Buffer,
    offset: usize,
    size: usize,
    stride: usize,
    count: usize,
    mode: MeshoptCompressionMode,
    filter: MeshoptCompressionFilter,
};

pub const BufferView = extern struct {
    name: ?MutCString,
    buffer: *Buffer,
    offset: usize,
    size: usize,
    stride: usize, // 0 == automatically determined by accessor
    view_type: BufferViewType,
    data: ?*anyopaque, // overrides buffer.data if present, filled by extensions
    has_meshopt_compression: Bool32,
    meshopt_compression: MeshoptCompression,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,

    pub fn data(bv: BufferView) ?[*]u8 {
        return cgltf_buffer_view_data(&bv);
    }
};

pub const AccessorSparse = extern struct {
    count: usize,
    indices_buffer_view: *BufferView,
    indices_byte_offset: usize,
    indices_component_type: ComponentType,
    values_buffer_view: *BufferView,
    values_byte_offset: usize,
    extras: Extras,
    indices_extras: Extras,
    values_extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
    indices_extensions_count: usize,
    indices_extensions: ?[*]Extension,
    values_extensions_count: usize,
    values_extensions: ?[*]Extension,
};

pub const Accessor = extern struct {
    name: ?MutCString,
    component_type: ComponentType,
    normalized: Bool32,
    type: Type,
    offset: usize,
    count: usize,
    stride: usize,
    buffer_view: ?*BufferView,
    has_min: Bool32,
    min: [16]f32,
    has_max: Bool32,
    max: [16]f32,
    is_sparse: Bool32,
    sparse: AccessorSparse,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,

    pub fn unpackFloatsCount(accessor: Accessor) usize {
        return cgltf_accessor_unpack_floats(&accessor, null, 0);
    }

    pub fn unpackFloats(accessor: Accessor, out: []f32) []f32 {
        const count = cgltf_accessor_unpack_floats(&accessor, out.ptr, out.len);
        return out[0..count];
    }

    pub fn unpackIndicesCount(accessor: Accessor) usize {
        return cgltf_accessor_unpack_indices(&accessor, null, 0);
    }

    pub fn unpackIndices(accessor: Accessor, out: []u32) []u32 {
        const count = cgltf_accessor_unpack_indices(&accessor, out.ptr, out.len);
        return out[0..count];
    }

    pub fn readFloat(accessor: Accessor, index: usize, out: []f32) bool {
        assert(out.len == accessor.type.numComponents());
        const result = cgltf_accessor_read_float(&accessor, index, out.ptr, out.len);
        return result != 0;
    }

    pub fn readUint(accessor: Accessor, index: usize, out: []u32) bool {
        assert(out.len == accessor.type.numComponents());
        const result = cgltf_accessor_read_uint(&accessor, index, out.ptr, out.len);
        return result != 0;
    }

    pub fn readIndex(accessor: Accessor, index: usize) usize {
        return cgltf_accessor_read_index(&accessor, index);
    }
};

pub const Attribute = extern struct {
    name: ?MutCString,
    type: AttributeType,
    index: i32,
    data: *Accessor,
};

pub const Image = extern struct {
    name: ?MutCString,
    uri: ?MutCString,
    buffer_view: ?*BufferView,
    mime_type: ?MutCString,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const Sampler = extern struct {
    uri: ?MutCString,
    mag_filter: i32,
    min_filter: i32,
    wrap_s: i32,
    wrap_t: i32,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const Texture = extern struct {
    name: ?MutCString,
    image: ?*Image,
    sampler: ?*Sampler,
    has_basisu: Bool32,
    basisu_image: ?*Image,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const TextureTransform = extern struct {
    offset: [2]f32,
    rotation: f32,
    scale: [2]f32,
    has_texcoord: Bool32,
    texcoord: i32,
};

pub const TextureView = extern struct {
    texture: ?*Texture,
    texcoord: i32,
    scale: f32,
    has_transform: Bool32,
    transform: TextureTransform,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const PbrMetallicRoughness = extern struct {
    base_color_texture: TextureView,
    metallic_roughness_texture: TextureView,
    base_color_factor: [4]f32,
    metallic_factor: f32,
    roughness_factor: f32,
};

pub const PbrSpecularGlossiness = extern struct {
    diffuse_texture: TextureView,
    specular_glossiness_texture: TextureView,
    diffuse_factor: [4]f32,
    specular_factor: [3]f32,
    glossiness_factor: f32,
};

pub const Clearcoat = extern struct {
    clearcoat_texture: TextureView,
    clearcoat_roughness_texture: TextureView,
    clearcoat_normal_texture: TextureView,
    clearcoat_factor: f32,
    clearcoat_roughness_factor: f32,
};

pub const Transmission = extern struct {
    transmission_texture: TextureView,
    transmission_factor: f32,
};

pub const Ior = extern struct {
    ior: f32,
};

pub const Specular = extern struct {
    specular_texture: TextureView,
    specular_color_texture: TextureView,
    specular_color_factor: [3]f32,
    specular_factor: f32,
};

pub const Volume = extern struct {
    thickness_texture: TextureView,
    thickness_factor: f32,
    attentuation_color: [3]f32,
    attentuation_distance: f32,
};

pub const Sheen = extern struct {
    sheen_color_texture: TextureView,
    sheen_color_factor: [3]f32,
    sheen_roughness_texture: TextureView,
    sheen_roughness_factor: f32,
};

pub const EmissiveStrength = extern struct {
    emissive_strength: f32,
};

pub const Iridescence = extern struct {
    iridescence_factor: f32,
    iridescence_texture: TextureView,
    iridescence_ior: f32,
    iridescence_thickness_min: f32,
    iridescence_thickness_max: f32,
    iridescence_thickness_texture: TextureView,
};

pub const Anisotropy = extern struct {
    anisotropy_strength: f32,
    anisotropy_rotation: f32,
    anisotropy_texture: TextureView,
};

pub const Material = extern struct {
    name: ?MutCString,
    has_pbr_metallic_roughness: Bool32,
    has_pbr_specular_glossiness: Bool32,
    has_clearcoat: Bool32,
    has_transmission: Bool32,
    has_volume: Bool32,
    has_ior: Bool32,
    has_specular: Bool32,
    has_sheen: Bool32,
    has_emissive_strength: Bool32,
    has_iridescence: Bool32,
    has_anisotropy: Bool32,
    pbr_metallic_roughness: PbrMetallicRoughness,
    pbr_specular_glossiness: PbrSpecularGlossiness,
    clearcoat: Clearcoat,
    ior: Ior,
    specular: Specular,
    sheen: Sheen,
    transmission: Transmission,
    volume: Volume,
    emissive_strength: EmissiveStrength,
    iridescence: Iridescence,
    anisotropy: Anisotropy,
    normal_texture: TextureView,
    occlusion_texture: TextureView,
    emissive_texture: TextureView,
    emissive_factor: [3]f32,
    alpha_mode: AlphaMode,
    alpha_cutoff: f32,
    double_sided: Bool32,
    unlit: Bool32,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const MaterialMapping = extern struct {
    variant: usize,
    material: ?*Material,
    extras: Extras,
};

pub const MorphTarget = extern struct {
    attributes: ?[*]Attribute,
    attributes_count: usize,
};

pub const DracoMeshCompression = extern struct {
    buffer_view: ?*BufferView,
    attributes: ?[*]Attribute,
    attributes_count: usize,
};

pub const MeshGpuInstancing = extern struct {
    attributes: ?[*]Attribute,
    attributes_count: usize,
};

pub const Primitive = extern struct {
    type: PrimitiveType,
    indices: ?*Accessor,
    material: ?*Material,
    attributes: [*]Attribute, // required
    attributes_count: usize,
    targets: ?[*]MorphTarget,
    targets_count: usize,
    extras: Extras,
    has_draco_mesh_compression: Bool32,
    draco_mesh_compression: DracoMeshCompression,
    mappings: ?[*]MaterialMapping,
    mappings_count: usize,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const Mesh = extern struct {
    name: ?MutCString,
    primitives: [*]Primitive, // required
    primitives_count: usize,
    weights: ?[*]f32,
    weights_count: usize,
    target_names: ?[*]MutCString,
    target_names_count: usize,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const Skin = extern struct {
    name: ?MutCString,
    joints: [*]*Node, // required
    joints_count: usize,
    skeleton: ?*Node,
    inverse_bind_matrices: ?*Accessor,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const CameraPerspective = extern struct {
    has_aspect_ratio: Bool32,
    aspect_ratio: f32,
    yfov: f32,
    has_zfar: Bool32,
    zfar: f32,
    znear: f32,
    extras: Extras,
};

pub const CameraOrthographic = extern struct {
    xmag: f32,
    ymag: f32,
    zfar: f32,
    znear: f32,
    extras: Extras,
};

pub const Camera = extern struct {
    name: ?MutCString,
    type: CameraType,
    data: extern union {
        perspective: CameraPerspective,
        orthographic: CameraOrthographic,
    },
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const Light = extern struct {
    name: ?MutCString,
    color: [3]f32,
    intensity: f32,
    type: LightType,
    range: f32,
    spot_inner_cone_angle: f32,
    spot_outer_cone_angle: f32,
    extras: Extras,
};

pub const Node = extern struct {
    name: ?MutCString,
    parent: ?*Node,
    children: ?[*]*Node,
    children_count: usize,
    skin: ?*Skin,
    mesh: ?*Mesh,
    camera: ?*Camera,
    light: ?*Light,
    weights: [*]f32,
    weights_count: usize,
    has_translation: Bool32,
    has_rotation: Bool32,
    has_scale: Bool32,
    has_matrix: Bool32,
    translation: [3]f32,
    rotation: [4]f32,
    scale: [3]f32,
    matrix: [16]f32,
    extras: Extras,
    has_mesh_gpu_instancing: Bool32,
    mesh_gpu_instancing: MeshGpuInstancing,
    extensions_count: usize,
    extensions: ?[*]Extension,

    pub fn transformLocal(node: Node) [16]f32 {
        var transform: [16]f32 = undefined;
        cgltf_node_transform_local(&node, &transform);
        return transform;
    }

    pub fn transformWorld(node: Node) [16]f32 {
        var transform: [16]f32 = undefined;
        cgltf_node_transform_world(&node, &transform);
        return transform;
    }
};

pub const Scene = extern struct {
    name: ?MutCString,
    nodes: ?[*]*Node,
    nodes_count: usize,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const AnimationSampler = extern struct {
    input: *Accessor, // required
    output: *Accessor, // required
    interpolation: InterpolationType,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const AnimationChannel = extern struct {
    sampler: *AnimationSampler, // required
    target_node: ?*Node,
    target_path: AnimationPathType,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const Animation = extern struct {
    name: ?MutCString,
    samplers: [*]AnimationSampler, // required
    samplers_count: usize,
    channels: [*]AnimationChannel, // required
    channels_count: usize,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const MaterialVariant = extern struct {
    name: ?MutCString,
    extras: Extras,
};

pub const Asset = extern struct {
    copyright: ?MutCString,
    generator: ?MutCString,
    version: ?MutCString,
    min_version: ?MutCString,
    extras: Extras,
    extensions_count: usize,
    extensions: ?[*]Extension,
};

pub const Data = extern struct {
    file_type: FileType,
    file_data: ?*anyopaque,

    asset: Asset,

    meshes: ?[*]Mesh,
    meshes_count: usize,

    materials: ?[*]Material,
    materials_count: usize,

    accessors: ?[*]Accessor,
    accessors_count: usize,

    buffer_views: ?[*]BufferView,
    buffer_views_count: usize,

    buffers: ?[*]Buffer,
    buffers_count: usize,

    images: ?[*]Image,
    images_count: usize,

    textures: ?[*]Texture,
    textures_count: usize,

    samplers: ?[*]Sampler,
    samplers_count: usize,

    skins: ?[*]Skin,
    skins_count: usize,

    cameras: ?[*]Camera,
    cameras_count: usize,

    lights: ?[*]Light,
    lights_count: usize,

    nodes: ?[*]Node,
    nodes_count: usize,

    scenes: ?[*]Scene,
    scenes_count: usize,

    scene: ?*Scene,

    animations: ?[*]Animation,
    animations_count: usize,

    variants: ?[*]MaterialVariant,
    variants_count: usize,

    extras: Extras,

    data_extensions_count: usize,
    data_extensions: ?[*]Extension,

    extensions_used: ?[*]MutCString,
    extensions_used_count: usize,

    extensions_required: ?[*]MutCString,
    extensions_required_count: usize,

    json: ?CString,
    json_size: usize,

    bin: ?*const anyopaque,
    bin_size: usize,

    memory: MemoryOptions,
    file: FileOptions,

    pub fn writeFile(data: Data, path: [*:0]const u8, options: Options) !void {
        const result = cgltf_write_file(&options, path, &data);
        try resultToError(result);
    }

    pub fn writeBuffer(data: Data, buffer: []u8, options: Options) usize {
        return cgltf_write(&options, buffer.ptr, buffer.len, &data);
    }
};

pub const Error = error{
    DataTooShort,
    UnknownFormat,
    InvalidJson,
    InvalidGltf,
    InvalidOptions,
    FileNotFound,
    IoError,
    OutOfMemory,
    LegacyGltf,
};

pub fn parse(options: Options, data: []const u8) Error!*Data {
    var out_data: ?*Data = null;
    const result = cgltf_parse(&options, data.ptr, data.len, &out_data);
    try resultToError(result);
    return out_data.?;
}

pub fn parseFile(options: Options, path: [*:0]const u8) Error!*Data {
    var out_data: ?*Data = null;
    const result = cgltf_parse_file(&options, path, &out_data);
    try resultToError(result);
    return out_data.?;
}

pub fn loadBuffers(options: Options, data: *Data, gltf_path: [*:0]const u8) Error!void {
    const result = cgltf_load_buffers(&options, data, gltf_path);
    try resultToError(result);
}

pub fn free(data: *Data) void {
    cgltf_free(data);
}

pub fn validate(data: *Data) Result {
    return cgltf_validate(data);
}

extern fn cgltf_parse(
    options: ?*const Options,
    data: ?*const anyopaque,
    size: usize,
    out_data: ?*?*Data,
) Result;

extern fn cgltf_parse_file(
    options: ?*const Options,
    path: ?[*:0]const u8,
    out_data: ?*?*Data,
) Result;

extern fn cgltf_load_buffers(
    options: ?*const Options,
    data: ?*Data,
    gltf_path: ?[*:0]const u8,
) Result;

extern fn cgltf_load_buffer_base64(
    options: ?*const Options,
    size: usize,
    base64: ?[*:0]const u8,
    out_data: ?*?*Data,
) Result;

extern fn cgltf_decode_string(string: ?MutCString) usize;
extern fn cgltf_decode_uri(string: ?MutCString) usize;

extern fn cgltf_free(data: ?*Data) void;
extern fn cgltf_validate(data: ?*Data) Result;

extern fn cgltf_node_transform_local(node: ?*const Node, out_matrix: ?*[16]f32) void;
extern fn cgltf_node_transform_world(node: ?*const Node, out_matrix: ?*[16]f32) void;

extern fn cgltf_buffer_view_data(view: ?*const BufferView) ?[*]u8;

extern fn cgltf_accessor_read_float(
    accessor: ?*const Accessor,
    index: usize,
    out: ?[*]f32,
    element_size: usize,
) Bool32;

extern fn cgltf_accessor_read_uint(
    accessor: ?*const Accessor,
    index: usize,
    out: ?[*]u32,
    element_size: usize,
) Bool32;

extern fn cgltf_accessor_read_index(
    accessor: ?*const Accessor,
    index: usize,
) usize;

extern fn cgltf_accessor_unpack_floats(
    accessor: ?*const Accessor,
    out: ?[*]f32,
    float_count: usize,
) usize;

extern fn cgltf_accessor_unpack_indices(
    accessor: ?*const Accessor,
    out: ?[*]u32,
    index_count: usize,
) usize;

extern fn cgltf_copy_extras_json(
    data: ?*const Data,
    extras: ?*const Extras,
    dest: ?[*]u8,
    dest_size: ?*usize,
) Result;

extern fn cgltf_write_file(
    options: ?*const Options,
    path: ?[*:0]const u8,
    data: ?*const Data,
) Result;

extern fn cgltf_write(
    options: ?*const Options,
    buffer: ?[*]u8,
    size: usize,
    data: ?*const Data,
) usize;

fn resultToError(result: Result) Error!void {
    switch (result) {
        .success => return,
        .data_too_short => return error.DataTooShort,
        .unknown_format => return error.UnknownFormat,
        .invalid_json => return error.InvalidJson,
        .invalid_gltf => return error.InvalidGltf,
        .invalid_options => return error.InvalidOptions,
        .file_not_found => return error.FileNotFound,
        .io_error => return error.IoError,
        .out_of_memory => return error.OutOfMemory,
        .legacy_gltf => return error.LegacyGltf,
    }
}

// TESTS ///////////////////////////////////////////////////////////////////////////////////////////

test {
    std.testing.refAllDeclsRecursive(@This());
}

test "extern struct layout" {
    @setEvalBranchQuota(10_000);
    const c = @cImport(@cInclude("cgltf.h"));
    inline for (comptime std.meta.declarations(@This())) |decl| {
        const ZigType = @field(@This(), decl.name);
        if (@TypeOf(ZigType) != type) {
            continue;
        }
        if (comptime std.meta.activeTag(@typeInfo(ZigType)) == .Struct and
            @typeInfo(ZigType).Struct.layout == .@"extern")
        {
            comptime var c_name_buf: [256]u8 = undefined;
            const c_name = comptime try cTypeNameFromZigTypeName(&c_name_buf, decl.name);
            const CType = @field(c, c_name);
            std.testing.expectEqual(@sizeOf(CType), @sizeOf(ZigType)) catch |err| {
                std.log.err("@sizeOf({s}) != @sizeOf({s})", .{ decl.name, c_name });
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
                        .{ decl.name, std.meta.fieldNames(ZigType)[i], c_name, c_field_name },
                    );
                    return err;
                };
                i += 1;
            }
        }
    }
}

test "enum" {
    @setEvalBranchQuota(10_000);
    const c = @cImport(@cInclude("cgltf.h"));
    inline for (comptime std.meta.declarations(@This())) |decl| {
        const ZigType = @field(@This(), decl.name);
        if (@TypeOf(ZigType) != type) {
            continue;
        }
        if (comptime std.meta.activeTag(@typeInfo(ZigType)) == .Enum) {
            comptime var c_name_buf: [256]u8 = undefined;
            const c_name = comptime try cTypeNameFromZigTypeName(&c_name_buf, decl.name);
            const CType = @field(c, c_name);
            std.testing.expectEqual(@sizeOf(CType), @sizeOf(ZigType)) catch |err| {
                std.log.err("@sizeOf({s}) != @sizeOf({s})", .{ decl.name, c_name });
                return err;
            };
            inline for (comptime std.meta.fieldNames(ZigType)) |field_name| {
                const c_field_name = comptime buildName: {
                    var buf: [256]u8 = undefined;
                    var fbs = std.io.fixedBufferStream(&buf);
                    try fbs.writer().writeAll(c_name);
                    try fbs.writer().writeByte('_');
                    try fbs.writer().writeAll(field_name);
                    break :buildName fbs.getWritten();
                };
                std.testing.expectEqual(
                    @field(c, c_field_name),
                    @intFromEnum(@field(ZigType, field_name)),
                ) catch |err| {
                    std.log.err("{s}.{s} != {s}", .{ decl.name, field_name, c_field_name });
                    return err;
                };
            }
        }
    }
}

fn cTypeNameFromZigTypeName(
    comptime buf: []u8,
    comptime zig_name: []const u8,
) ![]const u8 {
    comptime var fbs = std.io.fixedBufferStream(buf);
    try fbs.writer().writeAll("cgltf");
    for (zig_name) |char| {
        if (std.ascii.isUpper(char)) {
            try fbs.writer().writeByte('_');
            try fbs.writer().writeByte(std.ascii.toLower(char));
        } else {
            try fbs.writer().writeByte(char);
        }
    }
    return fbs.getWritten();
}
