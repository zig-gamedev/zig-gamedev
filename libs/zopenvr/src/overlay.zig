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

pub fn findOverlay() void {
    @compileError("not implemented");
}

pub fn createOverlay() void {
    @compileError("not implemented");
}

pub fn destroyOverlay() void {
    @compileError("not implemented");
}

pub fn getOverlayKey() void {
    @compileError("not implemented");
}

pub fn getOverlayName() void {
    @compileError("not implemented");
}

pub fn setOverlayName() void {
    @compileError("not implemented");
}

pub fn getOverlayImageData() void {
    @compileError("not implemented");
}

pub fn getOverlayErrorNameFromEnum() void {
    @compileError("not implemented");
}

pub fn setOverlayRenderingPid() void {
    @compileError("not implemented");
}

pub fn getOverlayRenderingPid() void {
    @compileError("not implemented");
}

pub fn setOverlayFlag() void {
    @compileError("not implemented");
}

pub fn getOverlayFlag() void {
    @compileError("not implemented");
}

pub fn getOverlayFlags() void {
    @compileError("not implemented");
}

pub fn setOverlayColor() void {
    @compileError("not implemented");
}

pub fn getOverlayColor() void {
    @compileError("not implemented");
}

pub fn setOverlayAlpha() void {
    @compileError("not implemented");
}

pub fn getOverlayAlpha() void {
    @compileError("not implemented");
}

pub fn setOverlayTexelAspect() void {
    @compileError("not implemented");
}

pub fn getOverlayTexelAspect() void {
    @compileError("not implemented");
}

pub fn setOverlaySortOrder() void {
    @compileError("not implemented");
}

pub fn getOverlaySortOrder() void {
    @compileError("not implemented");
}

pub fn setOverlayWidthInMeters() void {
    @compileError("not implemented");
}

pub fn getOverlayWidthInMeters() void {
    @compileError("not implemented");
}

pub fn setOverlayCurvature() void {
    @compileError("not implemented");
}

pub fn getOverlayCurvature() void {
    @compileError("not implemented");
}

pub fn setOverlayPreCurvePitch() void {
    @compileError("not implemented");
}

pub fn getOverlayPreCurvePitch() void {
    @compileError("not implemented");
}

pub fn setOverlayTextureColorSpace() void {
    @compileError("not implemented");
}

pub fn getOverlayTextureColorSpace() void {
    @compileError("not implemented");
}

pub fn setOverlayTextureBounds() void {
    @compileError("not implemented");
}

pub fn getOverlayTextureBounds() void {
    @compileError("not implemented");
}

pub fn getOverlayTransformType() void {
    @compileError("not implemented");
}

pub fn setOverlayTransformAbsolute() void {
    @compileError("not implemented");
}

pub fn getOverlayTransformAbsolute() void {
    @compileError("not implemented");
}

pub fn setOverlayTransformTrackedDeviceRelative() void {
    @compileError("not implemented");
}

pub fn getOverlayTransformTrackedDeviceRelative() void {
    @compileError("not implemented");
}

pub fn setOverlayTransformTrackedDeviceComponent() void {
    @compileError("not implemented");
}

pub fn getOverlayTransformTrackedDeviceComponent() void {
    @compileError("not implemented");
}

pub fn setOverlayTransformCursor() void {
    @compileError("not implemented");
}

pub fn getOverlayTransformCursor() void {
    @compileError("not implemented");
}

pub fn setOverlayTransformProjection() void {
    @compileError("not implemented");
}

pub fn showOverlay() void {
    @compileError("not implemented");
}

pub fn hideOverlay() void {
    @compileError("not implemented");
}

pub fn isOverlayVisible() void {
    @compileError("not implemented");
}

pub fn getTransformForOverlayCoordinates() void {
    @compileError("not implemented");
}

pub fn waitFrameSync() void {
    @compileError("not implemented");
}

pub fn pollNextOverlayEvent() void {
    @compileError("not implemented");
}

pub fn getOverlayInputMethod() void {
    @compileError("not implemented");
}

pub fn setOverlayInputMethod() void {
    @compileError("not implemented");
}

pub fn getOverlayMouseScale() void {
    @compileError("not implemented");
}

pub fn setOverlayMouseScale() void {
    @compileError("not implemented");
}

pub fn computeOverlayIntersection() void {
    @compileError("not implemented");
}

pub fn isHoverTargetOverlay() void {
    @compileError("not implemented");
}

pub fn setOverlayIntersectionMask() void {
    @compileError("not implemented");
}

pub fn triggerLaserMouseHapticVibration() void {
    @compileError("not implemented");
}

pub fn setOverlayCursor() void {
    @compileError("not implemented");
}

pub fn setOverlayCursorPositionOverride() void {
    @compileError("not implemented");
}

pub fn clearOverlayCursorPositionOverride() void {
    @compileError("not implemented");
}

pub fn setOverlayTexture() void {
    @compileError("not implemented");
}

pub fn clearOverlayTexture() void {
    @compileError("not implemented");
}

pub fn setOverlayRaw() void {
    @compileError("not implemented");
}

pub fn setOverlayFromFile() void {
    @compileError("not implemented");
}

pub fn getOverlayTexture() void {
    @compileError("not implemented");
}

pub fn releaseNativeOverlayHandle() void {
    @compileError("not implemented");
}

pub fn getOverlayTextureSize() void {
    @compileError("not implemented");
}

pub fn createDashboardOverlay() void {
    @compileError("not implemented");
}

pub fn isDashboardVisible() void {
    @compileError("not implemented");
}

pub fn isActiveDashboardOverlay() void {
    @compileError("not implemented");
}

pub fn setDashboardOverlaySceneProcess() void {
    @compileError("not implemented");
}

pub fn getDashboardOverlaySceneProcess() void {
    @compileError("not implemented");
}

pub fn showDashboard() void {
    @compileError("not implemented");
}

pub fn getPrimaryDashboardDevice() void {
    @compileError("not implemented");
}

pub fn showKeyboard() void {
    @compileError("not implemented");
}

pub fn showKeyboardForOverlay() void {
    @compileError("not implemented");
}

pub fn getKeyboardText() void {
    @compileError("not implemented");
}

pub fn hideKeyboard() void {
    @compileError("not implemented");
}

pub fn setKeyboardTransformAbsolute() void {
    @compileError("not implemented");
}

pub fn setKeyboardPositionForOverlay() void {
    @compileError("not implemented");
}

pub fn showMessageOverlay() void {
    @compileError("not implemented");
}

pub fn closeMessageOverlay() void {
    @compileError("not implemented");
}

const FunctionTable = extern struct {
    FindOverlay: *const fn ([*c]const u8, *common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    CreateOverlay: *const fn ([*c]const u8, [*c]const u8, *common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    DestroyOverlay: *const fn (common.OverlayHandle) callconv(.C) common.OverlayErrorCode,
    GetOverlayKey: *const fn (common.OverlayHandle, [*c]const u8, u32, *common.OverlayErrorCode) callconv(.C) u32,
    GetOverlayName: *const fn (common.OverlayHandle, [*c]const u8, u32, *common.OverlayErrorCode) callconv(.C) u32,
    SetOverlayName: *const fn (common.OverlayHandle, [*c]const u8) callconv(.C) common.OverlayErrorCode,
    GetOverlayImageData: *const fn (common.OverlayHandle, anyopaque, u32, *u32, *u32) callconv(.C) common.OverlayErrorCode,
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
    SetOverlayRaw: *const fn (common.OverlayHandle, anyopaque, u32, u32, u32) callconv(.C) common.OverlayErrorCode,
    SetOverlayFromFile: *const fn (common.OverlayHandle, [*c]const u8) callconv(.C) common.OverlayErrorCode,
    GetOverlayTexture: *const fn (common.OverlayHandle, *anyopaque, anyopaque, *u32, *u32, *u32, *common.TextureType, *common.ColorSpace, *common.TextureBounds) callconv(.C) common.OverlayErrorCode,
    ReleaseNativeOverlayHandle: *const fn (common.OverlayHandle, anyopaque) callconv(.C) common.OverlayErrorCode,
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

    ShowMessageOverlay: *const fn ([*c]const u8, [*c]const u8, [*c]const u8, ?[*c]const u8, ?[*c]const u8, ?[*c]const u8) callconv(.C) void,
    CloseMessageOverlay: *const fn () callconv(.C) void,
};
