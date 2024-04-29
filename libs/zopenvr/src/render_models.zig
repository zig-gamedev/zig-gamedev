const std = @import("std");

const common = @import("common.zig");

function_table: *FunctionTable,

const Self = @This();
const version = "IVRRenderModels_006";
pub fn init() common.InitError!Self {
    return .{
        .function_table = try common.getFunctionTable(FunctionTable, version),
    };
}

pub const RenderModelError = error{
    Loading,
    NotSupported,
    InvalidArg,
    InvalidModel,
    NoShapes,
    MultipleShapes,
    TooManyVertices,
    MultipleTextures,
    BufferTooSmall,
    NotEnoughNormals,
    NotEnoughTexCoords,
    InvalidTexture,
};
pub const RenderModelErrorCode = enum(i32) {
    none = 0,
    loading = 100,
    not_supported = 200,
    invalid_arg = 300,
    invalid_model = 301,
    no_shapes = 302,
    multiple_shapes = 303,
    too_many_vertices = 304,
    multiple_textures = 305,
    buffer_too_small = 306,
    not_enough_normals = 307,
    not_enough_tex_coords = 308,
    invalid_texture = 400,

    fn maybe(error_code: RenderModelErrorCode) RenderModelError!void {
        return switch (error_code) {
            .none => {},
            .loading => RenderModelError.Loading,
            .not_supported => RenderModelError.NotSupported,
            .invalid_arg => RenderModelError.InvalidArg,
            .invalid_model => RenderModelError.InvalidModel,
            .no_shapes => RenderModelError.NoShapes,
            .multiple_shapes => RenderModelError.MultipleShapes,
            .too_many_vertices => RenderModelError.TooManyVertices,
            .multiple_textures => RenderModelError.MultipleTextures,
            .buffer_too_small => RenderModelError.BufferTooSmall,
            .not_enough_normals => RenderModelError.NotEnoughNormals,
            .not_enough_tex_coords => RenderModelError.NotEnoughTexCoords,
            .invalid_texture => RenderModelError.InvalidTexture,
        };
    }
};

pub const TextureID = i32;
pub const ComponentProperties = u32;
const ExternRenderModel = extern struct {
    vertex_data: [*c]RenderModel.Vertex,
    vertex_count: u32,
    index_data: [*c]u16,
    triangle_count: u32,
    diffuse_texture_id: TextureID,
};
pub const RenderModel = struct {
    extern_ptr: *ExternRenderModel,

    vertex_data: []Vertex,
    index_data: []u16,
    triangle_count: u32,
    diffuse_texture_id: TextureID,

    pub fn init(extern_ptr: *ExternRenderModel) RenderModel {
        return .{
            .extern_ptr = extern_ptr,
            .vertex_data = extern_ptr.vertex_data[0..extern_ptr.vertex_count],
            .index_data = extern_ptr.index_data[0 .. extern_ptr.triangle_count * 3],
            .triangle_count = extern_ptr.triangle_count,
            .diffuse_texture_id = extern_ptr.diffuse_texture_id,
        };
    }

    pub const Vertex = extern struct {
        position: common.Vector3,
        normal: common.Vector3,
        texture_coord: [2]f32,
    };

    pub const TextureMap = extern struct {
        width: u16,
        height: u16,
        texture_map_data: [*c]u8,
        format: TextureFormat,
        mip_levels: u16,
    };

    pub const TextureFormat = enum(i32) {
        rgba8_srgb = 0,
        bc2 = 1,
        bc4 = 2,
        bc7 = 3,
        bc7_srgb = 4,
        rgba16_float = 5,
    };

    pub const ControllerModeState = extern struct {
        scroll_wheel_visible: bool,
    };

    pub const ComponentState = extern struct {
        tracking_to_component_render_model: common.Matrix34,
        tracking_to_component_local: common.Matrix34,
        properties: ComponentProperties,
    };
};

pub fn loadRenderModel(self: Self, render_model_name: [:0]const u8) RenderModelError!RenderModel {
    while (true) : (std.time.sleep(10_000_000)) {
        return self.loadRenderModelAsync(render_model_name) catch |err| switch (err) {
            error.Loading => continue,
            else => return err,
        };
    }
}

pub fn loadRenderModelAsync(self: Self, render_model_name: [:0]const u8) RenderModelError!RenderModel {
    var result: *ExternRenderModel = undefined;

    const error_code = self.function_table.LoadRenderModel_Async(@constCast(render_model_name.ptr), &result);
    try error_code.maybe();

    return RenderModel.init(result);
}

pub fn freeRenderModel(self: Self, render_model: RenderModel) void {
    self.function_table.FreeRenderModel(render_model.extern_ptr);
}

pub fn loadTexture(self: Self, texture_id: TextureID) RenderModelError!*RenderModel.TextureMap {
    while (true) : (std.time.sleep(10_000_000)) {
        return self.loadTextureAsync(texture_id) catch |err| switch (err) {
            error.Loading => continue,
            else => return err,
        };
    }
}

pub fn loadTextureAsync(self: Self, texture_id: TextureID) RenderModelError!*RenderModel.TextureMap {
    var result: *RenderModel.TextureMap = undefined;

    const error_code = self.function_table.LoadTexture_Async(texture_id, &result);
    try error_code.maybe();

    return result;
}

pub fn freeTexture(self: Self, texture: *RenderModel.TextureMap) void {
    self.function_table.FreeTexture(texture);
}

pub fn allocRenderModelName(self: Self, allocator: std.mem.Allocator, render_model_index: u32) ![:0]u8 {
    const buffer_length = self.function_table.GetRenderModelName(render_model_index, null, 0);
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    if (buffer_length > 0) {
        _ = self.function_table.GetRenderModelName(render_model_index, buffer.ptr, buffer_length);
    }

    return buffer;
}

pub fn getRenderModelCount(self: Self) u32 {
    return self.function_table.GetRenderModelCount();
}
pub fn getComponentCount(self: Self, render_model_name: [:0]const u8) u32 {
    return self.function_table.GetComponentCount(@constCast(render_model_name.ptr));
}
pub fn allocComponentName(self: Self, allocator: std.mem.Allocator, render_model_name: [:0]const u8, component_index: u32) ![:0]u8 {
    const buffer_length = self.function_table.GetComponentName(@constCast(render_model_name.ptr), component_index, null, 0);
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    if (buffer_length > 0) {
        _ = self.function_table.GetComponentName(@constCast(render_model_name.ptr), component_index, buffer.ptr, buffer_length);
    }

    return buffer;
}
pub fn getComponentButtonMask(self: Self, render_model_name: [:0]const u8, component_name: [:0]const u8) u64 {
    return self.function_table.GetComponentButtonMask(@constCast(render_model_name.ptr), @constCast(component_name.ptr));
}
pub fn allocComponentRenderModelName(self: Self, allocator: std.mem.Allocator, render_model_name: [:0]const u8, component_name: [:0]const u8) ![:0]u8 {
    const buffer_length = self.function_table.GetComponentRenderModelName(@constCast(render_model_name.ptr), @constCast(component_name.ptr), null, 0);
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    if (buffer_length > 0) {
        _ = self.function_table.GetComponentRenderModelName(@constCast(render_model_name.ptr), @constCast(component_name.ptr), buffer.ptr, buffer_length);
    }

    return buffer;
}

pub fn getComponentStateForDevicePath(self: Self, render_model_name: [:0]const u8, component_name: [:0]const u8, device_path: common.InputValueHandle, state: RenderModel.ControllerModeState) ?RenderModel.ComponentState {
    var result: RenderModel.ComponentState = undefined;
    if (self.function_table.GetComponentStateForDevicePath(@constCast(render_model_name.ptr), @constCast(component_name.ptr), device_path, @constCast(&state), &result)) {
        return result;
    }
    return null;
}

pub fn renderModelHasComponent(self: Self, render_model_name: [:0]const u8, component_name: [:0]const u8) bool {
    return self.function_table.RenderModelHasComponent(@constCast(render_model_name.ptr), @constCast(component_name.ptr));
}

pub fn allocRenderModelThumbnailURL(self: Self, allocator: std.mem.Allocator, render_model_name: [:0]const u8) (RenderModelError || error{OutOfMemory})![:0]u8 {
    var error_code: RenderModelErrorCode = undefined;
    const buffer_length = self.function_table.GetRenderModelThumbnailURL(@constCast(render_model_name.ptr), null, 0, &error_code);
    error_code.maybe() catch |err| switch (err) {
        RenderModelError.BufferTooSmall => {},
        else => return err,
    };
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    errdefer allocator.free(buffer);

    if (buffer_length > 0) {
        _ = self.function_table.GetRenderModelThumbnailURL(@constCast(render_model_name.ptr), buffer.ptr, buffer_length, &error_code);
        try error_code.maybe();
    }

    return buffer;
}
pub fn allocRenderModelOriginalPath(self: Self, allocator: std.mem.Allocator, render_model_name: [:0]const u8) (RenderModelError || error{OutOfMemory})![:0]u8 {
    var error_code: RenderModelErrorCode = undefined;
    const buffer_length = self.function_table.GetRenderModelOriginalPath(@constCast(render_model_name.ptr), null, 0, &error_code);
    error_code.maybe() catch |err| switch (err) {
        RenderModelError.BufferTooSmall => {},
        else => return err,
    };
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    errdefer allocator.free(buffer);

    if (buffer_length > 0) {
        _ = self.function_table.GetRenderModelOriginalPath(@constCast(render_model_name.ptr), buffer.ptr, buffer_length, &error_code);
        try error_code.maybe();
    }

    return buffer;
}

pub fn getRenderModelErrorNameFromEnum(self: Self, error_code: RenderModelErrorCode) [:0]const u8 {
    return std.mem.span(self.function_table.GetRenderModelErrorNameFromEnum(error_code));
}

pub const FunctionTable = extern struct {
    LoadRenderModel_Async: *const fn ([*c]u8, **ExternRenderModel) callconv(.C) RenderModelErrorCode,
    FreeRenderModel: *const fn (*ExternRenderModel) callconv(.C) void,
    LoadTexture_Async: *const fn (TextureID, **RenderModel.TextureMap) callconv(.C) RenderModelErrorCode,
    FreeTexture: *const fn (*RenderModel.TextureMap) callconv(.C) void,

    // skip d3d11
    LoadTextureD3D11_Async: *const fn (TextureID, ?*anyopaque, [*c]?*anyopaque) callconv(.C) RenderModelErrorCode,
    LoadIntoTextureD3D11_Async: *const fn (TextureID, ?*anyopaque) callconv(.C) RenderModelErrorCode,
    FreeTextureD3D11: *const fn (?*anyopaque) callconv(.C) void,

    GetRenderModelName: *const fn (u32, [*c]u8, u32) callconv(.C) u32,
    GetRenderModelCount: *const fn () callconv(.C) u32,
    GetComponentCount: *const fn ([*c]u8) callconv(.C) u32,
    GetComponentName: *const fn ([*c]u8, u32, [*c]u8, u32) callconv(.C) u32,
    GetComponentButtonMask: *const fn ([*c]u8, [*c]u8) callconv(.C) u64,
    GetComponentRenderModelName: *const fn ([*c]u8, [*c]u8, [*c]u8, u32) callconv(.C) u32,
    GetComponentStateForDevicePath: *const fn ([*c]u8, [*c]u8, common.InputValueHandle, *RenderModel.ControllerModeState, *RenderModel.ComponentState) callconv(.C) bool,

    // deprecated
    GetComponentState: *const fn ([*c]u8, [*c]u8, [*c]common.ControllerState, [*c]RenderModel.ControllerModeState, [*c]RenderModel.ComponentState) callconv(.C) bool,

    RenderModelHasComponent: *const fn ([*c]u8, [*c]u8) callconv(.C) bool,
    GetRenderModelThumbnailURL: *const fn ([*c]u8, [*c]u8, u32, [*c]RenderModelErrorCode) callconv(.C) u32,
    GetRenderModelOriginalPath: *const fn ([*c]u8, [*c]u8, u32, [*c]RenderModelErrorCode) callconv(.C) u32,
    GetRenderModelErrorNameFromEnum: *const fn (RenderModelErrorCode) callconv(.C) [*c]u8,
};
