const std = @import("std");

const common = @import("common.zig");
const d3d11 = @import("renderers.zig").d3d11;

function_table: *FunctionTable,

const Self = @This();

const version = "IVROverlay_027";
pub fn init() common.InitError!Self {
    return .{
        .function_table = try common.getFunctionTable(FunctionTable, version),
    };
}

pub fn findOverlay(self: Self, overlay_key: [:0]const u8) common.OverlayError!common.OverlayHandle {
    var handle: common.OverlayHandle = undefined;
    const overlay_error = self.function_table.FindOverlay(overlay_key.ptr, &handle);
    try overlay_error.maybe();
    return handle;
}

pub fn createOverlay(self: Self, overlay_key: [:0]const u8, overlay_name: [:0]const u8) common.OverlayError!common.OverlayHandle {
    var handle: common.OverlayHandle = undefined;
    const overlay_error = self.function_table.CreateOverlay(overlay_key.ptr, overlay_name.ptr, &handle);
    try overlay_error.maybe();
    return handle;
}

pub fn destroyOverlay(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!void {
    return self.function_table.DestroyOverlay(overlay_handle).maybe();
}

pub fn getOverlayKey(self: Self, allocator: std.mem.Allocator, overlay_handle: common.OverlayHandle) (common.OverlayError || error{OutOfMemory})![:0]u8 {
    var error_code: common.OverlayErrorCode = undefined;
    const buffer_length = self.function_table.GetOverlayKey(overlay_handle, null, 0, &error_code);
    try error_code.maybe();
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    errdefer allocator.free(buffer);

    if (buffer.len > 0) {
        error_code = undefined;
        _ = self.function_table.GetOverlayKey(overlay_handle, buffer.ptr, buffer_length, &error_code);
        try error_code.maybe();
    }

    return buffer;
}

pub fn getOverlayName(self: Self, allocator: std.mem.Allocator, overlay_handle: common.OverlayHandle) (common.OverlayError || error{OutOfMemory})![:0]u8 {
    var error_code: common.OverlayErrorCode = undefined;
    const buffer_length = self.function_table.GetOverlayName(overlay_handle, null, 0, &error_code);
    try error_code.maybe();
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    errdefer allocator.free(buffer);

    if (buffer.len > 0) {
        error_code = undefined;
        _ = self.function_table.GetOverlayName(overlay_handle, buffer.ptr, buffer_length, &error_code);
        try error_code.maybe();
    }

    return buffer;
}

pub fn setOverlayName(self: Self, overlay_handle: common.OverlayHandle, name: [:0]const u8) common.OverlayError!void {
    return self.function_table.SetOverlayName(overlay_handle, name.ptr).maybe();
}

pub fn getOverlayImageData(
    self: Self,
    allocator: std.mem.Allocator,
    comptime ArrayT: type,
    overlay_handle: common.OverlayHandle,
) common.OverlayError!common.RawImage(ArrayT, 4) {
    var image = try common.RawImage(ArrayT, 4).init(allocator);
    var err = self.function_table.GetOverlayImageData(overlay_handle, null, 0, &image.width, &image.height).maybe();
    if (err != common.OverlayError.ArrayTooSmall) return err;

    try image.makeData();
    errdefer image.deinit();

    if (image.data.len > 0) {
        err = self.function_table.GetOverlayImageData(
            overlay_handle,
            image.data.ptr,
            @sizeOf(ArrayT) * image.data.len,
            &image.width,
            &image.height,
        );
        try err.maybe();
    }

    return image;
}

pub fn getOverlayErrorNameFromEnum(self: Self, overlay_error: common.OverlayErrorCode) [:0]const u8 {
    return std.mem.span(self.function_table.GetOverlayErrorNameFromEnum(overlay_error));
}
pub fn getOverlayErrorNameFromError(self: Self, overlay_error: common.OverlayError) [:0]const u8 {
    return self.getOverlayErrorNameFromEnum(common.OverlayErrorCode.fromError(overlay_error));
}

pub fn setOverlayRenderingPid(self: Self, overlay_handle: common.OverlayHandle, pid: u32) common.OverlayError!void {
    return self.function_table.SetOverlayRenderingPid(overlay_handle, pid).maybe();
}

pub fn getOverlayRenderingPid(self: Self, overlay_handle: common.OverlayHandle) u32 {
    return self.function_table.GetOverlayRenderingPid(overlay_handle);
}

pub fn setOverlayFlag(self: Self, overlay_handle: common.OverlayHandle, flag: common.OverlayFlags.Enum, enabled: bool) common.OverlayError!void {
    return self.function_table.SetOverlayFlag(overlay_handle, flag, enabled).maybe();
}

pub fn getOverlayFlag(self: Self, overlay_handle: common.OverlayHandle, flag: common.OverlayFlags.Enum) common.OverlayError!bool {
    var enabled: bool = undefined;
    try self.function_table.GetOverlayFlag(overlay_handle, flag, &enabled).maybe();
    return enabled;
}

pub fn getOverlayFlags(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!common.OverlayFlags {
    var flags: common.OverlayFlags = undefined;
    try self.function_table.GetOverlayFlags(overlay_handle, &flags).maybe();
    return flags;
}

pub fn setOverlayColor(self: Self, overlay_handle: common.OverlayHandle, red: f32, green: f32, blue: f32) common.OverlayError!void {
    return self.function_table.SetOverlayColor(overlay_handle, red, green, blue).maybe();
}

pub fn getOverlayColor(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!struct { red: f32, green: f32, blue: f32 } {
    var red: f32 = undefined;
    var green: f32 = undefined;
    var blue: f32 = undefined;
    try self.function_table.GetOverlayColor(overlay_handle, &red, &green, &blue).maybe();

    return .{
        .red = red,
        .green = green,
        .blue = blue,
    };
}

pub fn setOverlayAlpha(self: Self, overlay_handle: common.OverlayHandle, alpha: f32) common.OverlayError!void {
    return self.function_table.SetOverlayAlpha(overlay_handle, alpha).maybe();
}

pub fn getOverlayAlpha(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!f32 {
    var alpha: f32 = undefined;
    try self.function_table.GetOverlayAlpha(overlay_handle, &alpha);
    return alpha;
}

pub fn setOverlayTexelAspect(self: Self, overlay_handle: common.OverlayHandle, texel_aspect: f32) common.OverlayError!void {
    return self.function_table.SetOverlayTexelAspect(overlay_handle, texel_aspect).maybe();
}

pub fn getOverlayTexelAspect(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!f32 {
    var texel_aspect: f32 = undefined;
    try self.function_table.GetOverlayTexelAspect(overlay_handle, &texel_aspect).maybe();
    return texel_aspect;
}

pub fn setOverlaySortOrder(self: Self, overlay_handle: common.OverlayHandle, sort_order: u32) common.OverlayError!void {
    return self.function_table.SetOverlaySortOrder(overlay_handle, sort_order).maybe();
}

pub fn getOverlaySortOrder(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!u32 {
    var sort_order: u32 = undefined;
    try self.function_table.GetOverlaySortOrder(overlay_handle, &sort_order);
    return sort_order;
}

pub fn setOverlayWidthInMeters(self: Self, overlay_handle: common.OverlayHandle, width_in_meters: f32) common.OverlayError!void {
    return self.function_table.SetOverlayWidthInMeters(overlay_handle, width_in_meters).maybe();
}

pub fn getOverlayWidthInMeters(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!f32 {
    var width_in_meters: f32 = undefined;
    try self.function_table.GetOverlayWidthInMeters(overlay_handle, &width_in_meters).maybe();
    return width_in_meters;
}

/// Use to draw overlay as a curved surface. Curvature is a percentage from [0..1] where 1 is a fully closed cylinder and 0 is a flat plane.
pub fn setOverlayCurvature(self: Self, overlay_handle: common.OverlayHandle, curvature: f32) common.OverlayError!void {
    std.debug.assert(0 <= curvature and curvature <= 1);
    return self.function_table.SetOverlayCurvature(overlay_handle, curvature).maybe();
}

/// Use to draw overlay as a curved surface. Radius is in meters and is based on the screen width.
pub fn setOverlayCurvatureRadius(self: Self, overlay_handle: common.OverlayHandle, radius: f32) common.OverlayError!void {
    std.debug.assert(0 < radius and radius <= 2 * std.math.pi);
    const width = try self.getOverlayWidthInMeters(overlay_handle);
    const curvature = width / (2 * std.math.pi * radius);
    return self.setOverlayCurvature(overlay_handle, curvature);
}

/// returns overlay curvature. Curvature is a percentage from (0..1] where 1 is a fully closed cylinder.
pub fn getOverlayCurvature(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!f32 {
    var curvature: f32 = undefined;
    try self.function_table.GetOverlayCurvature(overlay_handle, &curvature).maybe();
    return curvature;
}

/// returns overlay curve radius in meters
pub fn getOverlayCurvatureRadius(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!f32 {
    const width = try self.getOverlayWidthInMeters(overlay_handle);
    const curvature = try self.getOverlayCurvature(overlay_handle);
    const radius = width / (2 * std.math.pi * curvature);
    return radius;
}

/// Sets the pitch angle (in radians) of the overlay before curvature is applied -- to form a fan or disk.
pub fn setOverlayPreCurvePitch(self: Self, overlay_handle: common.OverlayHandle, radians: f32) common.OverlayError!void {
    std.debug.assert(0 <= radians and radians <= 2 * std.math.pi);
    return self.function_table.SetOverlayPreCurvePitch(overlay_handle, radians).maybe();
}

/// returns overlay pre-curve angle in radians
pub fn getOverlayPreCurvePitch(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!f32 {
    var radians: f32 = undefined;
    try self.function_table.GetOverlayPreCurvePitch(overlay_handle, &radians).maybe();
    return radians;
}

pub fn setOverlayTextureColorSpace(self: Self, overlay_handle: common.OverlayHandle, texture_color_space: common.ColorSpace) common.OverlayError!void {
    return self.function_table.SetOverlayTextureColorSpace(overlay_handle, texture_color_space).maybe();
}

pub fn getOverlayTextureColorSpace(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!common.ColorSpace {
    var texture_color_space: common.ColorSpace = undefined;
    try self.function_table.GetOverlayTextureColorSpace(overlay_handle, &texture_color_space).maybe();
    return texture_color_space;
}

pub fn setOverlayTextureBounds(self: Self, overlay_handle: common.OverlayHandle, overlay_texture_bounds: common.TextureBounds) common.OverlayError!void {
    return self.function_table.SetOverlayTextureBounds(overlay_handle, &overlay_texture_bounds).maybe();
}

pub fn getOverlayTextureBounds(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!common.TextureBounds {
    var overlay_texture_bounds: common.TextureBounds = undefined;
    try self.function_table.GetOverlayTextureBounds(overlay_handle, &overlay_texture_bounds).maybe();
    return overlay_texture_bounds;
}

pub fn getOverlayTransformType(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!common.OverlayTransformType {
    var transform_type: common.OverlayTransformType = undefined;
    try self.function_table.GetOverlayTransformType(overlay_handle, &transform_type).maybe();
    return transform_type;
}

pub fn setOverlayTransformAbsolute(
    self: Self,
    overlay_handle: common.OverlayHandle,
    tracking_origin: common.TrackingUniverseOrigin,
    tracking_origin_to_overlay_transform: common.Matrix34,
) common.OverlayError!void {
    return self.function_table.SetOverlayTransformAbsolute(overlay_handle, tracking_origin, tracking_origin_to_overlay_transform).maybe();
}

pub fn getOverlayTransformAbsolute(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!struct {
    tracking_origin: common.TrackingUniverseOrigin,
    tracking_origin_to_overlay_transform: common.Matrix34,
} {
    var tracking_origin: common.TrackingUniverseOrigin = undefined;
    var tracking_origin_to_overlay_transform: common.Matrix34 = undefined;
    try self.function_table.GetOverlayTransformAbsolute(overlay_handle, &tracking_origin, &tracking_origin_to_overlay_transform).maybe();

    return .{
        .tracking_origin = tracking_origin,
        .tracking_origin_to_overlay_transform = tracking_origin_to_overlay_transform,
    };
}

pub fn setOverlayTransformTrackedDeviceRelative(
    self: Self,
    overlay_handle: common.OverlayHandle,
    tracked_device: common.TrackedDeviceIndex,
    tracked_device_to_overlay_transform: common.Matrix34,
) common.OverlayError!void {
    const err = self.function_table.SetOverlayTransformTrackedDeviceRelative(
        overlay_handle,
        tracked_device,
        &tracked_device_to_overlay_transform,
    );
    return err.maybe();
}

pub fn getOverlayTransformTrackedDeviceRelative(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!struct {
    tracked_device: common.TrackedDeviceIndex,
    tracked_device_to_overlay_transform: common.Matrix34,
} {
    var tracked_device: common.TrackedDeviceIndex = undefined;
    var tracked_device_to_overlay_transform: common.Matrix34 = undefined;
    const err = self.function_table.GetOverlayTransformTrackedDeviceRelative(overlay_handle, &tracked_device, &tracked_device_to_overlay_transform);
    try err.maybe();

    return .{
        .tracked_device = tracked_device,
        .tracked_device_to_overlay_transform = tracked_device_to_overlay_transform,
    };
}

pub fn setOverlayTransformTrackedDeviceComponent(
    self: Self,
    overlay_handle: common.OverlayHandle,
    device_index: common.TrackedDeviceIndex,
    component_name: [:0]const u8,
) common.OverlayError!void {
    return self.function_table.SetOverlayTransformTrackedDeviceComponent(overlay_handle, device_index, component_name.ptr);
}

pub fn getOverlayTransformTrackedDeviceComponent(self: Self, overlay_handle: common.OverlayHandle, out_component_name_buffer: [:0]u8) common.OverlayError!common.TrackedDeviceIndex {
    var tracked_device: common.TrackedDeviceIndex = undefined;
    try self.function_table.GetOverlayTransformTrackedDeviceComponent(overlay_handle, &tracked_device, out_component_name_buffer.ptr, out_component_name_buffer.len);
    return tracked_device;
}

pub fn setOverlayTransformCursor(self: Self, overlay_handle: common.OverlayHandle, hotspot: common.Vector2) common.OverlayError!void {
    return self.function_table.SetOverlayTransformCursor(overlay_handle, &hotspot).maybe();
}

pub fn getOverlayTransformCursor(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!common.Vector2 {
    var hotspot: common.Vector2 = undefined;
    try self.function_table.GetOverlayTransformCursor(overlay_handle, &hotspot).maybe();
    return hotspot;
}

pub fn setOverlayTransformProjection(
    self: Self,
    overlay_handle: common.OverlayHandle,
    tracking_origin: common.TrackingUniverseOrigin,
    tracking_origin_to_overlay_transform: common.Matrix34,
    projection: common.OverlayProjection,
    eye: common.Eye,
) common.OverlayError!void {
    return self.function_table.SetOverlayTransformProjection(
        overlay_handle,
        tracking_origin,
        &tracking_origin_to_overlay_transform,
        &projection,
        eye,
    ).maybe();
}

pub fn showOverlay(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!void {
    return self.function_table.ShowOverlay(overlay_handle).maybe();
}

pub fn hideOverlay(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!void {
    return self.function_table.HideOverlay(overlay_handle).maybe();
}

pub fn isOverlayVisible(self: Self, overlay_handle: common.OverlayHandle) bool {
    return self.function_table.IsOverlayVisible(overlay_handle);
}

pub fn getTransformForOverlayCoordinates(
    self: Self,
    overlay_handle: common.OverlayHandle,
    tracking_origin: common.TrackingUniverseOrigin,
    coordinates_in_overlay: common.Vector2,
) common.OverlayError!common.Matrix34 {
    var transform: common.Matrix34 = undefined;
    try self.function_table.GetTransformForOverlayCoordinates(overlay_handle, tracking_origin, coordinates_in_overlay, &transform).maybe();
    return transform;
}

/// This function will block until the top of each frame, and can therefore be used to synchronize with the runtime's update rate.
///
/// Note: In non-async mode, some signals may be dropped due to scene app performance, so passing a timeout of 1000/refresh rate
/// may be useful depending on the overlay app's desired behavior.
pub fn waitFrameSync(self: Self, timeout_ms: u32) common.OverlayError!void {
    return self.function_table.WaitFrameSync(timeout_ms).maybe();
}

pub fn pollNextOverlayEvent(self: Self, overlay_handle: common.OverlayHandle) ?common.Event {
    var event: common.Event = undefined;
    if (self.function_table.PollNextOverlayEvent(overlay_handle, &event, @sizeOf(common.Event))) {
        return event;
    }
    return null;
}

pub fn getOverlayInputMethod(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!common.OverlayInputMethod {
    var input_method: common.OverlayInputMethod = undefined;
    try self.function_table.GetOverlayInputMethod(overlay_handle, &input_method).maybe();
    return input_method;
}

pub fn setOverlayInputMethod(
    self: Self,
    overlay_handle: common.OverlayHandle,
    input_method: common.OverlayInputMethod,
) common.OverlayError!void {
    return self.function_table.SetOverlayInputMethod(overlay_handle, input_method).maybe();
}

pub fn getOverlayMouseScale(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!common.Vector2 {
    var mouse_scale: common.Vector2 = undefined;
    try self.function_table.GetOverlayMouseScale(overlay_handle, &mouse_scale).maybe();
    return mouse_scale;
}

pub fn setOverlayMouseScale(self: Self, overlay_handle: common.OverlayHandle, mouse_scale: common.Vector2) common.OverlayError!void {
    return self.function_table.SetOverlayMouseScale(overlay_handle, &mouse_scale).maybe();
}

pub fn computeOverlayIntersection(
    self: Self,
    overlay_handle: common.OverlayHandle,
    params: common.OverlayIntersectionParams,
) ?common.OverlayIntersectionResults {
    var results: common.OverlayIntersectionResults = undefined;
    if (self.function_table.ComputeOverlayIntersection(overlay_handle, &params, &results)) {
        return results;
    }
    return null;
}

pub fn isHoverTargetOverlay(self: Self, overlay_handle: common.OverlayHandle) bool {
    return self.function_table.IsHoverTargetOverlay(overlay_handle);
}

pub fn setOverlayIntersectionMask(
    self: Self,
    overlay_handle: common.OverlayHandle,
    mask_primitives: []common.OverlayIntersectionMaskPrimitive,
) common.OverlayError!void {
    const err = self.function_table.SetOverlayIntersectionMask(
        overlay_handle,
        mask_primitives.ptr,
        mask_primitives.len,
        @sizeOf(common.OverlayIntersectionMaskPrimitive),
    );
    return err.maybe();
}

pub fn triggerLaserMouseHapticVibration(
    self: Self,
    overlay_handle: common.OverlayError,
    duration_seconds: f32,
    frequency: f32,
    amplitude: f32,
) common.OverlayError!void {
    return self.function_table.TriggerLaserMouseHapticVibration(overlay_handle, duration_seconds, frequency, amplitude).maybe();
}

pub fn setOverlayCursor(self: Self, overlay_handle: common.OverlayHandle, cursor_handle: common.OverlayHandle) common.OverlayError!void {
    return self.function_table.SetOverlayCursor(overlay_handle, cursor_handle).maybe();
}

pub fn setOverlayCursorPositionOverride(self: Self, overlay_handle: common.OverlayHandle, cursor: common.Vector2) common.OverlayError!void {
    return self.function_table.SetOverlayCursorPositionOverride(overlay_handle, cursor.ptr).maybe();
}

pub fn clearOverlayCursorPositionOverride(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!void {
    return self.function_table.ClearOverlayCursorPositionOverride(overlay_handle).maybe();
}

pub fn setOverlayTexture(self: Self, overlay_handle: common.OverlayHandle, texture: common.Texture) common.OverlayError!void {
    return self.function_table.SetOverlayTexture(overlay_handle, &texture).maybe();
}

pub fn clearOverlayTexture(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!void {
    return self.function_table.ClearOverlayTexture(overlay_handle).maybe();
}

pub fn setOverlayRaw(
    self: Self,
    comptime T: type,
    overlay_handle: common.OverlayHandle,
    buffer: [*]T,
    width: u32,
    height: u32,
    bytes_per_pixel: u32,
) common.OverlayError!void {
    comptime {
        const type_info = @typeInfo(T);
        if (type_info == .Int) {
            if (type_info.Int.bits % 8 != 0) @compileError("int needs to be a whole number of bytes");
        } else if (type_info == .Struct) {
            if (type_info.Struct.layout != .@"packed") @compileError("struct needs to be packed");
            if (@typeInfo(type_info.Struct.backing_integer.?).Int.bits % 8 != 0) @compileError("struct needs to be a whole number of bytes big");
        } else @compileError("type '" ++ @typeName(T) ++ "' is not allowed here");
    }
    return self.function_table.SetOverlayRaw(overlay_handle, buffer, width, height, bytes_per_pixel).maybe();
}

pub fn setOverlayRawFromRawImage(
    self: Self,
    comptime T: type,
    comptime bytes_per_pixel: u32,
    overlay_handle: common.OverlayHandle,
    image: common.RawImage(T, bytes_per_pixel),
) common.OverlayError!void {
    self.setOverlayRaw(T, overlay_handle, image.data, image.width, image.height, image.bytes_per_pixel);
}
pub fn setOverlayRawFromRawImageU8(self: Self, overlay_handle: common.OverlayHandle, image: common.RawImage(u8, 4)) common.OverlayError!void {
    return self.setOverlayRawFromRawImage(u8, 4, overlay_handle, image);
}

pub fn setOverlayFromFile(self: Self, overlay_handle: common.OverlayHandle, file_path: [:0]const u8) common.OverlayError!void {
    return self.function_table.SetOverlayFromFile(overlay_handle, file_path.ptr).maybe();
}

// TODO: make typeing accomedate more engines rather then just what was in the docs
pub fn getOverlayTextureD3D11(self: Self, overlay_handle: common.OverlayHandle, native_texture_ref: *d3d11.IResource) common.OverlayError!struct {
    native_texture_handle: *d3d11.IShaderResourceView,
    width: u32,
    height: u32,
    native_format: u32,
    api_type: common.TextureType,
    color_space: common.ColorSpace,
    texture_bounds: common.TextureBounds,
} {
    comptime if (@import("builtin").os.tag != .windows) @compileError("can't guarantee it works, so disabling");

    var native_texture_handle: *d3d11.IShaderResourceView = undefined;
    var width: u32 = undefined;
    var height: u32 = undefined;
    var native_format: u32 = undefined;
    var api_type: common.TextureType = undefined;
    var color_space: common.ColorSpace = undefined;
    var texture_bounds: common.TextureBounds = undefined;

    const err = self.function_table.GetOverlayTexture(
        overlay_handle,
        &native_texture_handle,
        native_texture_ref,
        &width,
        &height,
        &native_format,
        &api_type,
        &color_space,
        &texture_bounds,
    );

    try err.maybe();
    return .{
        .native_texture_handle = native_texture_handle.?,
        .width = width,
        .height = height,
        .native_format = native_format,
        .api_type = api_type,
        .color_space = color_space,
        .texture_bounds = texture_bounds,
    };
}

pub fn releaseNativeOverlayHandleD3D11(self: Self, overlay_handle: common.OverlayHandle, native_texture_handle: *d3d11.IShaderResourceView) common.OverlayError!void {
    return self.function_table.ReleaseNativeOverlayHandle(overlay_handle, native_texture_handle).maybe();
}

pub fn getOverlayTextureSize(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!struct { width: u32, height: u32 } {
    var width: u32 = undefined;
    var height: u32 = undefined;
    try self.function_table.GetOverlayTextureSize(overlay_handle, &width, &height).maybe();

    return .{
        .width = width,
        .height = height,
    };
}

pub fn createDashboardOverlay(self: Self, overlay_key: [:0]const u8, overlay_friendly_name: [:0]const u8) common.OverlayError!struct {
    overlay_handle: common.OverlayHandle,
    thumbail_handle: common.OverlayHandle,
} {
    var overlay_handle: common.OverlayHandle = undefined;
    var thumbail_handle: common.OverlayHandle = undefined;
    const err = self.function_table.CreateDashboardOverlay(overlay_key.ptr, overlay_friendly_name.ptr, &overlay_handle, &thumbail_handle);
    try err.maybe();

    return .{
        .overlay_handle = overlay_handle,
        .thumbnail_handle = overlay_handle,
    };
}

pub fn isDashboardVisible(self: Self) bool {
    return self.function_table.IsDashboardVisible();
}

pub fn isActiveDashboardOverlay(self: Self, overlay_handle: common.OverlayHandle) bool {
    return self.function_table.IsActiveDashboardOverlay(overlay_handle);
}

pub fn setDashboardOverlaySceneProcess(self: Self, overlay_handle: common.OverlayHandle, process_id: u32) common.OverlayError!void {
    return self.function_table.SetDashboardOverlaySceneProcess(overlay_handle, process_id).maybe();
}

pub fn getDashboardOverlaySceneProcess(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!u32 {
    var process_id: u32 = undefined;
    try self.function_table.GetDashboardOverlaySceneProcess(overlay_handle, &process_id).maybe();
    return process_id;
}

pub fn showDashboard(self: Self, overlay_to_show: [:0]const u8) void {
    self.function_table.ShowDashboard(overlay_to_show.ptr);
}

pub fn getPrimaryDashboardDevice(self: Self) common.TrackedDeviceIndex {
    return self.function_table.GetPrimaryDashboardDevice();
}

pub fn showKeyboard(
    self: Self,
    input_mode: common.GamepadTextInputMode,
    line_input_more: common.GamepadTextInputLineMode,
    flags: common.KeyboardFlags,
    description: [:0]const u8,
    max_characters: u32,
    existing_text: [:0]const u8,
    user_value: u64,
) common.OverlayError!void {
    const err = self.function_table.ShowKeyboard(
        input_mode,
        line_input_more,
        flags,
        description.ptr,
        max_characters,
        existing_text.ptr,
        user_value,
    );
    return err.maybe();
}

pub fn showKeyboardForOverlay(
    self: Self,
    overlay_handle: common.OverlayHandle,
    input_mode: common.GamepadTextInputMode,
    line_input_more: common.GamepadTextInputLineMode,
    flags: common.KeyboardFlags,
    description: [:0]const u8,
    max_characters: u32,
    existing_text: [:0]const u8,
    user_value: u64,
) common.OverlayError!void {
    const err = self.function_table.ShowKeyboardForOverlay(
        overlay_handle,
        input_mode,
        line_input_more,
        flags,
        description.ptr,
        max_characters,
        existing_text.ptr,
        user_value,
    );
    return err.maybe();
}

// TODO: check if max length is the same as max chars from above or one off
pub fn getKeyboardText(self: Self, allocator: std.mem.Allocator, max_length: u32) error{OutOfMemory}![:0]u8 {
    const buffer = try allocator.allocSentinel(u8, max_length - 1, 0);
    const size = self.function_table.GetKeyboardText(buffer.ptr, max_length);
    _ = size;

    return buffer;
}

pub fn hideKeyboard(self: Self) void {
    self.function_table.HideKeyboard();
}

pub fn setKeyboardTransformAbsolute(self: Self, tracking_origin: common.TrackingUniverseOrigin, tracking_origin_to_keyboard_transform: common.Matrix34) void {
    self.function_table.SetKeyboardTransformAbsolute(tracking_origin, &tracking_origin_to_keyboard_transform);
}

pub fn setKeyboardPositionForOverlay(self: Self, overlay_handle: common.OverlayHandle, avoid_rect: common.Rect2) void {
    self.function_table.SetKeyboardPositionForOverlay(overlay_handle, avoid_rect);
}

pub fn showMessageOverlay(
    self: Self,
    text: [:0]const u8,
    caption: [:0]const u8,
    button0_text: [:0]const u8,
    button1_text: ?[:0]const u8,
    button2_text: ?[:0]const u8,
    button3_text: ?[:0]const u8,
) common.MessageOverlayResponse {
    return self.function_table.ShowMessageOverlay(
        text.ptr,
        caption.ptr,
        button0_text.ptr,
        if (button1_text) |str| str.ptr else null,
        if (button2_text) |str| str.ptr else null,
        if (button3_text) |str| str.ptr else null,
    );
}

pub fn closeMessageOverlay(self: Self) void {
    self.function_table.CloseMessageOverlay();
}

const FunctionTable = extern struct {
    FindOverlay: *const fn ([*c]const u8, *common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    CreateOverlay: *const fn ([*c]const u8, [*c]const u8, *common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    DestroyOverlay: *const fn (common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    GetOverlayKey: *const fn (common.OverlayHandle, [*c]u8, u32, *common.OverlayErrorCode) callconv(.C) u32,
    GetOverlayName: *const fn (common.OverlayHandle, [*c]u8, u32, *common.OverlayErrorCode) callconv(.C) u32,
    SetOverlayName: *const fn (common.OverlayHandle, [*c]const u8) callconv(.C) common.OverlayErrorCode,
    GetOverlayImageData: *const fn (common.OverlayHandle, ?*anyopaque, u32, *u32, *u32) callconv(.C) common.OverlayErrorCode,
    GetOverlayErrorNameFromEnum: *const fn (common.OverlayErrorCode) callconv(.C) [*c]const u8,

    SetOverlayRenderingPid: *const fn (common.OverlayHandle, u32) callconv(.C) common.OverlayErrorCode,
    GetOverlayRenderingPid: *const fn (common.OverlayHandle) callconv(.C) u32,
    SetOverlayFlag: *const fn (common.OverlayHandle, common.OverlayFlags.Enum, bool) callconv(.C) common.OverlayErrorCode,
    GetOverlayFlag: *const fn (common.OverlayHandle, common.OverlayFlags.Enum, *bool) callconv(.C) common.OverlayErrorCode,
    GetOverlayFlags: *const fn (common.OverlayHandle, *common.OverlayFlags) callconv(.C) common.OverlayErrorCode,
    SetOverlayColor: *const fn (common.OverlayHandle, f32, f32, f32) callconv(.C) common.OverlayErrorCode,
    GetOverlayColor: *const fn (common.OverlayHandle, *f32, *f32, *f32) callconv(.C) common.OverlayErrorCode,
    SetOverlayAlpha: *const fn (common.OverlayHandle, f32) callconv(.C) common.OverlayErrorCode,
    GetOverlayAlpha: *const fn (common.OverlayHandle, *f32) callconv(.C) common.OverlayErrorCode,
    SetOverlayTexelAspect: *const fn (common.OverlayHandle, f32) callconv(.C) common.OverlayErrorCode,
    GetOverlayTexelAspect: *const fn (common.OverlayHandle, *f32) callconv(.C) common.OverlayErrorCode,
    SetOverlaySortOrder: *const fn (common.OverlayHandle, u32) callconv(.C) common.OverlayErrorCode,
    GetOverlaySortOrder: *const fn (common.OverlayHandle, *u32) callconv(.C) common.OverlayErrorCode,
    SetOverlayWidthInMeters: *const fn (common.OverlayHandle, f32) callconv(.C) common.OverlayErrorCode,
    GetOverlayWidthInMeters: *const fn (common.OverlayHandle, *f32) callconv(.C) common.OverlayErrorCode,
    SetOverlayCurvature: *const fn (common.OverlayHandle, f32) callconv(.C) common.OverlayErrorCode,
    GetOverlayCurvature: *const fn (common.OverlayHandle, *f32) callconv(.C) common.OverlayErrorCode,
    SetOverlayPreCurvePitch: *const fn (common.OverlayHandle, f32) callconv(.C) common.OverlayErrorCode,
    GetOverlayPreCurvePitch: *const fn (common.OverlayHandle, *f32) callconv(.C) common.OverlayErrorCode,
    SetOverlayTextureColorSpace: *const fn (common.OverlayHandle, common.ColorSpace) callconv(.C) common.OverlayErrorCode,
    GetOverlayTextureColorSpace: *const fn (common.OverlayHandle, *common.ColorSpace) callconv(.C) common.OverlayErrorCode,
    SetOverlayTextureBounds: *const fn (common.OverlayHandle, *const common.TextureBounds) callconv(.C) common.OverlayErrorCode,
    GetOverlayTextureBounds: *const fn (common.OverlayHandle, *common.TextureBounds) callconv(.C) common.OverlayErrorCode,
    GetOverlayTransformType: *const fn (common.OverlayHandle, *common.OverlayTransformType) callconv(.C) common.OverlayErrorCode,
    SetOverlayTransformAbsolute: *const fn (common.OverlayHandle, common.TrackingUniverseOrigin, *const common.Matrix34) callconv(.C) common.OverlayErrorCode,
    GetOverlayTransformAbsolute: *const fn (common.OverlayHandle, *common.TrackingUniverseOrigin, *common.Matrix34) callconv(.C) common.OverlayErrorCode,
    SetOverlayTransformTrackedDeviceRelative: *const fn (common.OverlayHandle, common.TrackedDeviceIndex, *const common.Matrix34) callconv(.C) common.OverlayErrorCode,
    GetOverlayTransformTrackedDeviceRelative: *const fn (common.OverlayHandle, *common.TrackedDeviceIndex, *common.Matrix34) callconv(.C) common.OverlayErrorCode,
    SetOverlayTransformTrackedDeviceComponent: *const fn (common.OverlayHandle, common.TrackedDeviceIndex, [*c]const u8) callconv(.C) common.OverlayErrorCode,
    GetOverlayTransformTrackedDeviceComponent: *const fn (common.OverlayHandle, *common.TrackedDeviceIndex, [*c]u8, u32) callconv(.C) common.OverlayErrorCode,
    SetOverlayTransformCursor: *const fn (common.OverlayHandle, *const common.Vector2) callconv(.C) common.OverlayErrorCode,
    GetOverlayTransformCursor: *const fn (common.OverlayHandle, *common.Vector2) callconv(.C) common.OverlayErrorCode,
    SetOverlayTransformProjection: *const fn (common.OverlayHandle, common.TrackingUniverseOrigin, *const common.Matrix34, *const common.OverlayProjection, common.Eye) callconv(.C) common.OverlayErrorCode,
    ShowOverlay: *const fn (common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    HideOverlay: *const fn (common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    IsOverlayVisible: *const fn (common.OverlayHandle) callconv(.C) bool,
    GetTransformForOverlayCoordinates: *const fn (common.OverlayHandle, common.TrackingUniverseOrigin, common.Vector2, *common.Matrix34) callconv(.C) common.OverlayErrorCode,
    WaitFrameSync: *const fn (u32) callconv(.C) common.OverlayErrorCode,

    PollNextOverlayEvent: *const fn (common.OverlayHandle, *common.Event, u32) callconv(.C) bool,
    GetOverlayInputMethod: *const fn (common.OverlayHandle, *common.OverlayInputMethod) callconv(.C) common.OverlayErrorCode,
    SetOverlayInputMethod: *const fn (common.OverlayHandle, common.OverlayInputMethod) callconv(.C) common.OverlayErrorCode,
    GetOverlayMouseScale: *const fn (common.OverlayHandle, *common.Vector2) callconv(.C) common.OverlayErrorCode,
    SetOverlayMouseScale: *const fn (common.OverlayHandle, *const common.Vector2) callconv(.C) common.OverlayErrorCode,
    ComputeOverlayIntersection: *const fn (common.OverlayHandle, *const common.OverlayIntersectionParams, *common.OverlayIntersectionResults) callconv(.C) bool,
    IsHoverTargetOverlay: *const fn (common.OverlayHandle) callconv(.C) bool,
    SetOverlayIntersectionMask: *const fn (common.OverlayHandle, [*c]common.OverlayIntersectionMaskPrimitive, u32, u32) callconv(.C) common.OverlayErrorCode,
    TriggerLaserMouseHapticVibration: *const fn (common.OverlayHandle, f32, f32, f32) callconv(.C) common.OverlayErrorCode,
    SetOverlayCursor: *const fn (common.OverlayHandle, common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    SetOverlayCursorPositionOverride: *const fn (common.OverlayHandle, *const common.Vector2) callconv(.C) common.OverlayErrorCode,
    ClearOverlayCursorPositionOverride: *const fn (common.OverlayHandle) callconv(.C) common.OverlayErrorCode,

    SetOverlayTexture: *const fn (common.OverlayHandle, *const common.Texture) callconv(.C) common.OverlayErrorCode,
    ClearOverlayTexture: *const fn (common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    SetOverlayRaw: *const fn (common.OverlayHandle, ?*anyopaque, u32, u32, u32) callconv(.C) common.OverlayErrorCode,
    SetOverlayFromFile: *const fn (common.OverlayHandle, [*c]const u8) callconv(.C) common.OverlayErrorCode,
    GetOverlayTexture: *const fn (common.OverlayHandle, *?*anyopaque, ?*anyopaque, *u32, *u32, *u32, *common.TextureType, *common.ColorSpace, *common.TextureBounds) callconv(.C) common.OverlayErrorCode,
    ReleaseNativeOverlayHandle: *const fn (common.OverlayHandle, ?*anyopaque) callconv(.C) common.OverlayErrorCode,
    GetOverlayTextureSize: *const fn (common.OverlayHandle, *u32, *u32) callconv(.C) common.OverlayErrorCode,

    CreateDashboardOverlay: *const fn ([*c]const u8, [*c]const u8, *common.OverlayHandle, *common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    IsDashboardVisible: *const fn () callconv(.C) bool,
    IsActiveDashboardOverlay: *const fn (common.OverlayHandle) callconv(.C) bool,
    SetDashboardOverlaySceneProcess: *const fn (common.OverlayHandle, u32) callconv(.C) common.OverlayErrorCode,
    GetDashboardOverlaySceneProcess: *const fn (common.OverlayHandle, *u32) callconv(.C) common.OverlayErrorCode,
    ShowDashboard: *const fn ([*c]const u8) callconv(.C) void,
    GetPrimaryDashboardDevice: *const fn () callconv(.C) common.TrackedDeviceIndex,

    ShowKeyboard: *const fn (common.GamepadTextInputMode, common.GamepadTextInputLineMode, common.KeyboardFlags, [*c]const u8, u32, [*c]const u8, u64) callconv(.C) common.OverlayErrorCode,
    ShowKeyboardForOverlay: *const fn (common.OverlayHandle, common.GamepadTextInputMode, common.GamepadTextInputLineMode, common.KeyboardFlags, [*c]const u8, u32, [*c]const u8, u64) callconv(.C) common.OverlayErrorCode,
    GetKeyboardText: *const fn ([*c]u8, u32) callconv(.C) u32,
    HideKeyboard: *const fn () callconv(.C) void,
    SetKeyboardTransformAbsolute: *const fn (common.TrackingUniverseOrigin, *const common.Matrix34) callconv(.C) void,
    SetKeyboardPositionForOverlay: *const fn (common.OverlayHandle, common.Rect2) callconv(.C) void,

    ShowMessageOverlay: *const fn ([*c]const u8, [*c]const u8, [*c]const u8, [*c]const u8, [*c]const u8, [*c]const u8) callconv(.C) common.MessageOverlayResponse,
    CloseMessageOverlay: *const fn () callconv(.C) void,
};
