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

pub fn loadRenderModel(self: Self, render_model_name: [:0]const u8) common.RenderModelError!common.RenderModel {
    while (true) : (std.time.sleep(10_000_000)) {
        return self.loadRenderModelAsync(render_model_name) catch |err| switch (err) {
            error.Loading => continue,
            else => return err,
        };
    }
}

pub fn loadRenderModelAsync(self: Self, render_model_name: [:0]const u8) common.RenderModelError!common.RenderModel {
    var result: *common.ExternRenderModel = undefined;

    const error_code = self.function_table.LoadRenderModel_Async(@constCast(render_model_name.ptr), &result);
    try error_code.maybe();

    return common.RenderModel.init(result);
}

pub fn freeRenderModel(self: Self, render_model: common.RenderModel) void {
    self.function_table.FreeRenderModel(render_model.extern_ptr);
}

pub fn loadTexture(self: Self, texture_id: common.TextureID) common.RenderModelError!*common.RenderModel.TextureMap {
    while (true) : (std.time.sleep(10_000_000)) {
        return self.loadTextureAsync(texture_id) catch |err| switch (err) {
            error.Loading => continue,
            else => return err,
        };
    }
}

pub fn loadTextureAsync(self: Self, texture_id: common.TextureID) common.RenderModelError!*common.RenderModel.TextureMap {
    var result: *common.RenderModel.TextureMap = undefined;

    const error_code = self.function_table.LoadTexture_Async(texture_id, &result);
    try error_code.maybe();

    return result;
}

pub fn freeTexture(self: Self, texture: *common.RenderModel.TextureMap) void {
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

pub fn getComponentStateForDevicePath(self: Self, render_model_name: [:0]const u8, component_name: [:0]const u8, device_path: common.InputValueHandle, state: common.RenderModel.ControllerModeState) ?common.RenderModel.ComponentState {
    var result: common.RenderModel.ComponentState = undefined;
    if (self.function_table.GetComponentStateForDevicePath(@constCast(render_model_name.ptr), @constCast(component_name.ptr), device_path, @constCast(&state), &result)) {
        return result;
    }
    return null;
}

pub fn renderModelHasComponent(self: Self, render_model_name: [:0]const u8, component_name: [:0]const u8) bool {
    return self.function_table.RenderModelHasComponent(@constCast(render_model_name.ptr), @constCast(component_name.ptr));
}

pub fn allocRenderModelThumbnailURL(self: Self, allocator: std.mem.Allocator, render_model_name: [:0]const u8) (common.RenderModelError || error{OutOfMemory})![:0]u8 {
    var error_code: common.RenderModelErrorCode = undefined;
    const buffer_length = self.function_table.GetRenderModelThumbnailURL(@constCast(render_model_name.ptr), null, 0, &error_code);
    error_code.maybe() catch |err| switch (err) {
        common.RenderModelError.BufferTooSmall => {},
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
pub fn allocRenderModelOriginalPath(self: Self, allocator: std.mem.Allocator, render_model_name: [:0]const u8) (common.RenderModelError || error{OutOfMemory})![:0]u8 {
    var error_code: common.RenderModelErrorCode = undefined;
    const buffer_length = self.function_table.GetRenderModelOriginalPath(@constCast(render_model_name.ptr), null, 0, &error_code);
    error_code.maybe() catch |err| switch (err) {
        common.RenderModelError.BufferTooSmall => {},
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

pub fn getRenderModelErrorNameFromEnum(self: Self, error_code: common.RenderModelErrorCode) [:0]const u8 {
    return std.mem.span(self.function_table.GetRenderModelErrorNameFromEnum(error_code));
}

const FunctionTable = extern struct {
    LoadRenderModel_Async: *const fn ([*c]u8, **common.ExternRenderModel) callconv(.C) common.RenderModelErrorCode,
    FreeRenderModel: *const fn (*common.ExternRenderModel) callconv(.C) void,
    LoadTexture_Async: *const fn (common.TextureID, **common.RenderModel.TextureMap) callconv(.C) common.RenderModelErrorCode,
    FreeTexture: *const fn (*common.RenderModel.TextureMap) callconv(.C) void,

    // skip d3d11
    LoadTextureD3D11_Async: *const fn (common.TextureID, ?*anyopaque, [*c]?*anyopaque) callconv(.C) common.RenderModelErrorCode,
    LoadIntoTextureD3D11_Async: *const fn (common.TextureID, ?*anyopaque) callconv(.C) common.RenderModelErrorCode,
    FreeTextureD3D11: *const fn (?*anyopaque) callconv(.C) void,

    GetRenderModelName: *const fn (u32, [*c]u8, u32) callconv(.C) u32,
    GetRenderModelCount: *const fn () callconv(.C) u32,
    GetComponentCount: *const fn ([*c]u8) callconv(.C) u32,
    GetComponentName: *const fn ([*c]u8, u32, [*c]u8, u32) callconv(.C) u32,
    GetComponentButtonMask: *const fn ([*c]u8, [*c]u8) callconv(.C) u64,
    GetComponentRenderModelName: *const fn ([*c]u8, [*c]u8, [*c]u8, u32) callconv(.C) u32,
    GetComponentStateForDevicePath: *const fn ([*c]u8, [*c]u8, common.InputValueHandle, *common.RenderModel.ControllerModeState, *common.RenderModel.ComponentState) callconv(.C) bool,

    // deprecated
    GetComponentState: *const fn ([*c]u8, [*c]u8, [*c]common.ControllerState, [*c]common.RenderModel.ControllerModeState, [*c]common.RenderModel.ComponentState) callconv(.C) bool,

    RenderModelHasComponent: *const fn ([*c]u8, [*c]u8) callconv(.C) bool,
    GetRenderModelThumbnailURL: *const fn ([*c]u8, [*c]u8, u32, [*c]common.RenderModelErrorCode) callconv(.C) u32,
    GetRenderModelOriginalPath: *const fn ([*c]u8, [*c]u8, u32, [*c]common.RenderModelErrorCode) callconv(.C) u32,
    GetRenderModelErrorNameFromEnum: *const fn (common.RenderModelErrorCode) callconv(.C) [*c]u8,
};
