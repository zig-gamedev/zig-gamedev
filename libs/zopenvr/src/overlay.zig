const std = @import("std");

const common = @import("common.zig");

function_table: *FunctionTable,

const Self = @This();

const version = "IVROverlay_027";
pub fn init() common.InitError!Self {
    return .{
        .function_table = try common.getFunctionTable(FunctionTable, version),
    };
}

pub fn findOverlay(_: Self) void {
    @compileError("not implemented");
}

pub fn createOverlay(self: Self, overlay_key: [:0]const u8, overlay_name: [:0]const u8) common.OverlayError!common.OverlayHandle {
    var handle: common.OverlayHandle = undefined;
    const overlay_error = self.function_table.CreateOverlay(overlay_key.ptr, overlay_name.ptr, &handle);
    try overlay_error.maybe();
    return handle;
}

pub fn destroyOverlay(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayKey(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayName(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayName(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayImageData(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayErrorNameFromEnum(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayRenderingPid(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayRenderingPid(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayFlag(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayFlag(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayFlags(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayColor(self: Self, overlay_handle: common.OverlayHandle, red: f32, green: f32, blue: f32) common.OverlayError!void {
    try self.function_table.SetOverlayColor(overlay_handle, red, green, blue).maybe();
}

pub fn getOverlayColor(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayAlpha(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayAlpha(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayTexelAspect(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayTexelAspect(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlaySortOrder(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlaySortOrder(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayWidthInMeters(self: Self, overlay_handle: common.OverlayHandle, width_in_meters: f32) common.OverlayError!void {
    try self.function_table.SetOverlayWidthInMeters(overlay_handle, width_in_meters).maybe();
}

pub fn getOverlayWidthInMeters(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayCurvature(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayCurvature(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayPreCurvePitch(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayPreCurvePitch(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayTextureColorSpace(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayTextureColorSpace(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayTextureBounds(self: Self, overlay_handle: common.OverlayHandle, overlay_texture_bounds: common.TextureBounds) common.OverlayError!void {
    try self.function_table.SetOverlayTextureBounds(overlay_handle, &overlay_texture_bounds).maybe();
}

pub fn getOverlayTextureBounds(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayTransformType(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayTransformAbsolute(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayTransformAbsolute(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayTransformTrackedDeviceRelative(self: Self, overlay_handle: common.OverlayHandle, tracked_device: common.TrackedDeviceIndex, tracked_device_to_overlay_transform: common.Matrix34) common.OverlayError!void {
    try self.function_table.SetOverlayTransformTrackedDeviceRelative(overlay_handle, tracked_device, &tracked_device_to_overlay_transform).maybe();
}

pub fn getOverlayTransformTrackedDeviceRelative(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayTransformTrackedDeviceComponent(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayTransformTrackedDeviceComponent(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayTransformCursor(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayTransformCursor(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayTransformProjection(_: Self) void {
    @compileError("not implemented");
}

pub fn showOverlay(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!void {
    try self.function_table.ShowOverlay(overlay_handle).maybe();
}

pub fn hideOverlay(_: Self) void {
    @compileError("not implemented");
}

pub fn isOverlayVisible(_: Self) void {
    @compileError("not implemented");
}

pub fn getTransformForOverlayCoordinates(_: Self) void {
    @compileError("not implemented");
}

pub fn waitFrameSync(_: Self) void {
    @compileError("not implemented");
}

pub fn pollNextOverlayEvent(self: Self, overlay_handle: common.OverlayHandle) ?common.Event {
    var event: common.Event = undefined;
    if (self.function_table.PollNextOverlayEvent(overlay_handle, &event, @sizeOf(common.Event))) {
        return event;
    }
    return null;
}

pub fn getOverlayInputMethod(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayInputMethod(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayMouseScale(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayMouseScale(_: Self) void {
    @compileError("not implemented");
}

pub fn computeOverlayIntersection(_: Self) void {
    @compileError("not implemented");
}

pub fn isHoverTargetOverlay(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayIntersectionMask(_: Self) void {
    @compileError("not implemented");
}

pub fn triggerLaserMouseHapticVibration(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayCursor(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayCursorPositionOverride(_: Self) void {
    @compileError("not implemented");
}

pub fn clearOverlayCursorPositionOverride(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayTexture(_: Self) void {
    @compileError("not implemented");
}

pub fn clearOverlayTexture(_: Self) void {
    @compileError("not implemented");
}

pub fn setOverlayRaw(self: Self, comptime T: type, overlay_handle: common.OverlayHandle, buffer: [*]T, width: u32, height: u32, bytes_per_pixel: u32) common.OverlayError!void {
    std.debug.assert(@typeInfo(T) == .Int);
    try self.function_table.SetOverlayRaw(overlay_handle, buffer, width, height, bytes_per_pixel).maybe();
}

pub fn setOverlayFromFile(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayTexture(_: Self) void {
    @compileError("not implemented");
}

pub fn releaseNativeOverlayHandle(_: Self) void {
    @compileError("not implemented");
}

pub fn getOverlayTextureSize(_: Self) void {
    @compileError("not implemented");
}

pub fn createDashboardOverlay(_: Self) void {
    @compileError("not implemented");
}

pub fn isDashboardVisible(_: Self) void {
    @compileError("not implemented");
}

pub fn isActiveDashboardOverlay(_: Self) void {
    @compileError("not implemented");
}

pub fn setDashboardOverlaySceneProcess(_: Self) void {
    @compileError("not implemented");
}

pub fn getDashboardOverlaySceneProcess(_: Self) void {
    @compileError("not implemented");
}

pub fn showDashboard(_: Self) void {
    @compileError("not implemented");
}

pub fn getPrimaryDashboardDevice(_: Self) void {
    @compileError("not implemented");
}

pub fn showKeyboard(_: Self) void {
    @compileError("not implemented");
}

pub fn showKeyboardForOverlay(_: Self) void {
    @compileError("not implemented");
}

pub fn getKeyboardText(_: Self) void {
    @compileError("not implemented");
}

pub fn hideKeyboard(_: Self) void {
    @compileError("not implemented");
}

pub fn setKeyboardTransformAbsolute(_: Self) void {
    @compileError("not implemented");
}

pub fn setKeyboardPositionForOverlay(_: Self) void {
    @compileError("not implemented");
}

pub fn showMessageOverlay(_: Self) void {
    @compileError("not implemented");
}

pub fn closeMessageOverlay(_: Self) void {
    @compileError("not implemented");
}

const FunctionTable = extern struct {
    FindOverlay: *const fn ([*c]const u8, *common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    CreateOverlay: *const fn ([*c]const u8, [*c]const u8, *common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    DestroyOverlay: *const fn (common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    GetOverlayKey: *const fn (common.OverlayHandle, [*c]const u8, u32, *common.OverlayErrorCode) callconv(.C) u32,
    GetOverlayName: *const fn (common.OverlayHandle, [*c]const u8, u32, *common.OverlayErrorCode) callconv(.C) u32,
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
    GetOverlayTransformTrackedDeviceComponent: *const fn (common.OverlayHandle, *common.TrackedDeviceIndex, [*c]const u8, u32) callconv(.C) common.OverlayErrorCode,
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
    SetOverlayIntersectionMask: *const fn (common.OverlayHandle, *common.OverlayIntersectionMaskPrimitive, u32, u32) callconv(.C) common.OverlayErrorCode,
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
    ShowDashboard: *const fn ([*c]const u8) callconv(.C) common.OverlayErrorCode,
    GetPrimaryDashboardDevice: *const fn () callconv(.C) common.TrackedDeviceIndex,

    ShowKeyboard: *const fn (common.GamepadTextInputMode, common.GamepadTextInputLineMode, common.KeyboardFlags, [*c]const u8, u32, [*c]const u8, u64) callconv(.C) common.OverlayErrorCode,
    ShowKeyboardForOverlay: *const fn (common.OverlayHandle, common.GamepadTextInputMode, common.GamepadTextInputLineMode, common.KeyboardFlags, [*c]const u8, u32, [*c]const u8, u64) callconv(.C) common.OverlayErrorCode,
    GetKeyboardText: *const fn ([*c]const u8, u32) callconv(.C) u32,
    HideKeyboard: *const fn () callconv(.C) void,
    SetKeyboardTransformAbsolute: *const fn (common.TrackingUniverseOrigin, *const common.Matrix34) callconv(.C) void,
    SetKeyboardPositionForOverlay: *const fn (common.OverlayHandle, common.Rect2) callconv(.C) void,

    ShowMessageOverlay: *const fn ([*c]const u8, [*c]const u8, [*c]const u8, [*c]const u8, [*c]const u8, [*c]const u8) callconv(.C) void,
    CloseMessageOverlay: *const fn () callconv(.C) void,
};
