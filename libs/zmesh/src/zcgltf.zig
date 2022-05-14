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

pub const MemoryOptions = extern struct {
    alloc: ?fn (user: ?*anyopaque, size: usize) callconv(.C) ?*anyopaque = null,
    free: ?fn (user: ?*anyopaque, ptr: ?*anyopaque) callconv(.C) void = null,
    user_data: ?*anyopaque = null,
};

pub const FileOptions = extern struct {
    read: ?fn (*const MemoryOptions, *const FileOptions, CString, *usize, *(?*anyopaque)) callconv(.C) Result = null,
    release: ?fn (*const MemoryOptions, *const FileOptions, ?*anyopaque) callconv(.C) void = null,
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
    start_offset: usize,
    end_offset: usize,
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
    extras: Extras,
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
    pbr_metallic_roughness: PbrMetallicRoughness,
    pbr_specular_glossiness: PbrSpecularGlossiness,
    clearcoat: Clearcoat,
    ior: Ior,
    specular: Specular,
    sheen: Sheen,
    transmission: Transmission,
    volume: Volume,
    emissive_strength: EmissiveStrength,
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
