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
    name: MutCString,
    data: MutCString,
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
