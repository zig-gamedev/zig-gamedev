pub const Size = usize;
pub const Float = f32;
pub const Int = c_int;
pub const UInt = c_uint;
pub const Bool = c_int;

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
    alloc: fn (user: ?*anyopaque, size: Size) callconv(.C) ?*anyopaque,
    free: fn (user: ?*anyopaque, ptr: ?*anyopaque) callconv(.C) void,
    user_data: ?*anyopaque,
};

// TODO: Write proper function prototypes
pub const FileOptions = extern struct {
    read: ?*anyopaque,
    release: ?*anyopaque,
    user_data: ?*anyopaque,
};

pub const Options = extern struct {
    file_type: FileType,
    json_token_count: Size,
    memory: MemoryOptions,
    file: FileOptions,
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
