const std = @import("std");

const common = @import("common.zig");

function_table: *FunctionTable,

const Self = @This();

const version = "IVRSystem_022";
pub fn init() common.InitError!Self {
    return .{
        .function_table = try common.getFunctionTable(FunctionTable, version),
    };
}

pub fn getRecommendedRenderTargetSize(self: Self) common.RenderTargetSize {
    var render_target_size: common.RenderTargetSize = .{ .width = 0, .height = 0 };
    self.function_table.GetRecommendedRenderTargetSize(&render_target_size.width, &render_target_size.height);
    return render_target_size;
}

pub fn getProjectionMatrix(self: Self, eye: common.Eye, near: f32, far: f32) common.Matrix44 {
    return self.function_table.GetProjectionMatrix(eye, near, far);
}

pub fn getProjectionRaw(self: Self, eye: common.Eye) common.RawProjection {
    var raw_projection: common.RawProjection = undefined;
    self.function_table.GetProjectionRaw(eye, &raw_projection.left, &raw_projection.right, &raw_projection.top, &raw_projection.bottom);
    return raw_projection;
}

pub fn computeDistortion(self: Self, eye: common.Eye, u: f32, v: f32) ?common.DistortionCoordinates {
    var distortion_coordinates: common.DistortionCoordinates = undefined;
    if (self.function_table.ComputeDistortion(eye, u, v, &distortion_coordinates)) {
        return distortion_coordinates;
    } else {
        return null;
    }
}

pub fn getEyeToHeadTransform(self: Self, eye: common.Eye) common.Matrix34 {
    return self.function_table.GetEyeToHeadTransform(eye);
}

pub fn getTimeSinceLastVsync(self: Self) ?common.VSyncTiming {
    var timing: common.VSyncTiming = undefined;
    if (self.function_table.GetTimeSinceLastVsync(&timing.seconds_since_last_vsync, &timing.frame_counter)) {
        return timing;
    } else {
        return null;
    }
}

pub fn getDXGIOutputInfo(self: Self) ?i32 {
    var adapter_index: i32 = undefined;
    self.function_table.GetDXGIOutputInfo(&adapter_index);
    if (adapter_index == -1) {
        return null;
    }
    return adapter_index;
}
pub fn isDisplayOnDesktop(self: Self) bool {
    return self.function_table.IsDisplayOnDesktop();
}
pub fn setDisplayVisibility(self: Self, is_visible_on_desktop: bool) bool {
    return self.function_table.SetDisplayVisibility(is_visible_on_desktop);
}

pub fn allocDeviceToAbsoluteTrackingPose(self: Self, allocator: std.mem.Allocator, origin: common.TrackingUniverseOrigin, predicted_seconds_to_photons_from_now: f32, count: usize) ![]common.TrackedDevicePose {
    const tracked_device_poses = try allocator.alloc(common.TrackedDevicePose, count);
    if (count > 0) {
        self.function_table.GetDeviceToAbsoluteTrackingPose(origin, predicted_seconds_to_photons_from_now, tracked_device_poses.ptr, @intCast(tracked_device_poses.len));
    }

    return tracked_device_poses;
}

pub fn getSeatedZeroPoseToStandingAbsoluteTrackingPose(self: Self) common.Matrix34 {
    return self.function_table.GetSeatedZeroPoseToStandingAbsoluteTrackingPose();
}
pub fn getRawZeroPoseToStandingAbsoluteTrackingPose(self: Self) common.Matrix34 {
    return self.function_table.GetRawZeroPoseToStandingAbsoluteTrackingPose();
}
pub fn getSortedTrackedDeviceIndicesOfClass(self: Self, tracked_device_class: common.TrackedDeviceClass, tracked_device_indices: []common.TrackedDeviceIndex, relative_to_tracked_device_index: common.TrackedDeviceIndex) u32 {
    return self.function_table.GetSortedTrackedDeviceIndicesOfClass(tracked_device_class, tracked_device_indices.ptr, @intCast(tracked_device_indices.len), relative_to_tracked_device_index);
}

pub fn getTrackedDeviceActivityLevel(self: Self, device_index: common.TrackedDeviceIndex) common.DeviceActivityLevel {
    return self.function_table.GetTrackedDeviceActivityLevel(device_index);
}

pub fn applyTransform(self: Self, tracked_device_pose: common.TrackedDevicePose, transform: common.Matrix34) common.TrackedDevicePose {
    var result: common.TrackedDevicePose = undefined;
    self.function_table.ApplyTransform(&result, @constCast(&tracked_device_pose), @constCast(&transform));
    return result;
}

pub fn getTrackedDeviceIndexForControllerRole(self: Self, device_type: common.TrackedControllerRole) common.TrackedDeviceIndex {
    return self.function_table.GetTrackedDeviceIndexForControllerRole(device_type);
}
pub fn getControllerRoleForTrackedDeviceIndex(self: Self, tracked_device_index: common.TrackedDeviceIndex) common.TrackedControllerRole {
    return self.function_table.GetControllerRoleForTrackedDeviceIndex(tracked_device_index);
}

pub fn getTrackedDeviceClass(self: Self, device_index: common.TrackedDeviceIndex) common.TrackedDeviceClass {
    return self.function_table.GetTrackedDeviceClass(device_index);
}

pub fn isTrackedDeviceConnected(self: Self, device_index: common.TrackedDeviceIndex) bool {
    return self.function_table.IsTrackedDeviceConnected(device_index);
}

pub fn getTrackedDeviceProperty(self: Self, comptime T: type, device_index: common.TrackedDeviceIndex, property: common.TrackedDeviceProperty.fromType(T)) common.TrackedPropertyError!T {
    var property_error: common.TrackedPropertyErrorCode = undefined;
    const result = switch (T) {
        bool => self.function_table.GetBoolTrackedDeviceProperty(device_index, @enumFromInt(@intFromEnum(property)), &property_error),
        f32 => self.function_table.GetFloatTrackedDeviceProperty(device_index, @enumFromInt(@intFromEnum(property)), &property_error),
        i32 => self.function_table.GetInt32TrackedDeviceProperty(device_index, @enumFromInt(@intFromEnum(property)), &property_error),
        u64 => self.function_table.GetUint64TrackedDeviceProperty(device_index, @enumFromInt(@intFromEnum(property)), &property_error),
        common.Matrix34 => self.function_table.GetMatrix34TrackedDeviceProperty(device_index, @enumFromInt(@intFromEnum(property)), &property_error),
        else => @compileError("T must be bool, f32, i32, u64, Matrix34"),
    };
    try property_error.maybe();
    return result;
}

pub fn getTrackedDevicePropertyBool(self: Self, device_index: common.TrackedDeviceIndex, property: common.TrackedDeviceProperty.Bool) common.TrackedPropertyError!bool {
    return self.getTrackedDeviceProperty(bool, device_index, property);
}

pub fn getTrackedDevicePropertyF32(self: Self, device_index: common.TrackedDeviceIndex, property: common.TrackedDeviceProperty.F32) common.TrackedPropertyError!f32 {
    return self.getTrackedDeviceProperty(f32, device_index, property);
}

pub fn getTrackedDevicePropertyI32(self: Self, device_index: common.TrackedDeviceIndex, property: common.TrackedDeviceProperty.I32) common.TrackedPropertyError!i32 {
    return self.getTrackedDeviceProperty(i32, device_index, property);
}

pub fn getTrackedDevicePropertyU64(self: Self, device_index: common.TrackedDeviceIndex, property: common.TrackedDeviceProperty.U64) common.TrackedPropertyError!u64 {
    return self.getTrackedDeviceProperty(u64, device_index, property);
}

pub fn getTrackedDevicePropertyMatrix34(self: Self, device_index: common.TrackedDeviceIndex, property: common.TrackedDeviceProperty.Matrix34) common.TrackedPropertyError!common.Matrix34 {
    return self.getTrackedDeviceProperty(common.Matrix34, device_index, property);
}

pub fn allocTrackedDevicePropertyArray(self: Self, comptime T: type, allocator: std.mem.Allocator, device_index: common.TrackedDeviceIndex, property: common.TrackedDeviceProperty.Array.fromType(T)) common.TrackedPropertyError![]T {
    var property_error: common.TrackedPropertyErrorCode = undefined;
    const buffer_length = self.function_table.GetArrayTrackedDeviceProperty(device_index, @enumFromInt(@intFromEnum(property)), common.PropertyTypeTagCode.fromType(T), null, 0, &property_error);
    property_error.maybe() catch |err| switch (err) {
        common.TrackedPropertyError.BufferTooSmall => {},
        else => return err,
    };
    const buffer = try allocator.alloc(u8, buffer_length);

    if (buffer_length > 0) {
        property_error = undefined;
        _ = self.function_table.GetArrayTrackedDeviceProperty(device_index, @enumFromInt(@intFromEnum(property)), common.PropertyTypeTagCode.fromType(T), @ptrCast(buffer.ptr), buffer_length, &property_error);
        try property_error.maybe();
    }

    return @alignCast(std.mem.bytesAsSlice(T, buffer));
}

pub fn allocTrackedDevicePropertyArrayF32(self: Self, allocator: std.mem.Allocator, device_index: common.TrackedDeviceIndex, property: common.TrackedDeviceProperty.Array.F32) common.TrackedPropertyError![]f32 {
    return self.allocTrackedDevicePropertyArray(f32, allocator, device_index, property);
}

pub fn allocTrackedDevicePropertyArrayI32(self: Self, allocator: std.mem.Allocator, device_index: common.TrackedDeviceIndex, property: common.TrackedDeviceProperty.Array.I32) common.TrackedPropertyError![]i32 {
    return self.allocTrackedDevicePropertyArray(i32, allocator, device_index, property);
}

pub fn allocTrackedDevicePropertyArrayVector4(self: Self, allocator: std.mem.Allocator, device_index: common.TrackedDeviceIndex, property: common.TrackedDeviceProperty.Array.Vector4) common.TrackedPropertyError![]common.Vector4 {
    return self.allocTrackedDevicePropertyArray(common.Vector4, allocator, device_index, property);
}

pub fn allocTrackedDevicePropertyArrayMatrix34(self: Self, allocator: std.mem.Allocator, device_index: common.TrackedDeviceIndex, property: common.TrackedDeviceProperty.Array.Matrix34) common.TrackedPropertyError![]common.Matrix34 {
    return self.allocTrackedDevicePropertyArray(common.Matrix34, allocator, device_index, property);
}

pub fn allocTrackedDevicePropertyString(self: Self, allocator: std.mem.Allocator, device_index: common.TrackedDeviceIndex, property: common.TrackedDeviceProperty.String) common.TrackedPropertyError![:0]u8 {
    var property_error: common.TrackedPropertyErrorCode = undefined;
    const buffer_length = self.function_table.GetStringTrackedDeviceProperty(device_index, property, null, 0, &property_error);
    property_error.maybe() catch |err| switch (err) {
        common.TrackedPropertyError.BufferTooSmall => {},
        else => return err,
    };
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    if (buffer_length > 0) {
        property_error = undefined;
        _ = self.function_table.GetStringTrackedDeviceProperty(device_index, property, buffer.ptr, buffer_length, &property_error);
        try property_error.maybe();
    }
    return buffer;
}

pub fn getPropErrorNameFromEnum(self: Self, property_error: common.TrackedPropertyErrorCode) [:0]const u8 {
    return std.mem.span(self.function_table.GetPropErrorNameFromEnum(property_error));
}

pub fn pollNextEvent(self: Self) ?common.Event {
    var event: common.Event = undefined;
    if (self.function_table.PollNextEvent(&event, @sizeOf(common.Event))) {
        return event;
    } else {
        return null;
    }
}

pub fn pollNextEventWithPose(self: Self, origin: common.TrackingUniverseOrigin) ?common.EventWithPose {
    var event: common.Event = undefined;
    var pose: common.TrackedDevicePose = undefined;
    if (self.function_table.PollNextEventWithPose(origin, &event, @sizeOf(common.Event), &pose)) {
        return .{
            .event = event,
            .pose = pose,
        };
    } else {
        return null;
    }
}

pub fn getEventTypeNameFromEnum(self: Self, event_type: common.EventType) [:0]const u8 {
    return std.mem.span(self.function_table.GetEventTypeNameFromEnum(event_type));
}

pub fn getHiddenAreaMesh(self: Self, eye: common.Eye, mesh_type: common.HiddenAreaMeshType) []const common.Vector2 {
    const mesh = self.function_table.GetHiddenAreaMesh(eye, mesh_type);
    return if (mesh.triangle_count == 0)
        &.{}
    else
        mesh.vertex_data[0..mesh.triangle_count];
}

pub fn triggerHapticPulse(self: Self, device_index: common.TrackedDeviceIndex, axis_id: u32, duration_microseconds: u16) void {
    self.function_table.TriggerHapticPulse(device_index, axis_id, duration_microseconds);
}

pub fn getControllerState(self: Self, device_index: common.TrackedDeviceIndex) ?common.ControllerState {
    var controller_state: common.ControllerState = undefined;
    if (self.function_table.GetControllerState(device_index, &controller_state, @sizeOf(common.ControllerState))) {
        return controller_state;
    } else {
        return null;
    }
}

pub fn getControllerStateWithPose(self: Self, origin: common.TrackingUniverseOrigin, device_index: common.TrackedDeviceIndex) ?common.ControllerStateWithPose {
    var controller_state: common.ControllerState = undefined;
    var pose: common.TrackedDevicePose = undefined;
    if (self.function_table.GetControllerStateWithPose(origin, device_index, &controller_state, @sizeOf(common.ControllerState), &pose)) {
        return .{
            .controller_state = controller_state,
            .pose = pose,
        };
    } else {
        return null;
    }
}
pub fn getButtonIdNameFromEnum(self: Self, button_id: common.ButtonId) [:0]const u8 {
    return std.mem.span(self.function_table.GetButtonIdNameFromEnum(button_id));
}
pub fn getControllerAxisTypeNameFromEnum(self: Self, axis_type: common.ControllerAxisType) [:0]const u8 {
    return std.mem.span(self.function_table.GetControllerAxisTypeNameFromEnum(axis_type));
}
pub fn isInputAvailable(self: Self) bool {
    return self.function_table.IsInputAvailable();
}
pub fn isSteamVRDrawingControllers(self: Self) bool {
    return self.function_table.IsSteamVRDrawingControllers();
}
pub fn shouldApplicationPause(self: Self) bool {
    return self.function_table.ShouldApplicationPause();
}
pub fn shouldApplicationReduceRenderingWork(self: Self) bool {
    return self.function_table.ShouldApplicationReduceRenderingWork();
}
pub fn performFirmwareUpdate(self: Self, device_index: common.TrackedDeviceIndex) common.FirmwareError!void {
    const firmware_error = self.function_table.PerformFirmwareUpdate(device_index);
    try firmware_error.maybe();
}
pub fn acknowledgeQuitExiting(self: Self) void {
    self.function_table.AcknowledgeQuit_Exiting();
}

pub fn allocAppContainerFilePaths(self: Self, allocator: std.mem.Allocator) !common.FilePaths {
    const buffer_length = self.function_table.GetAppContainerFilePaths(null, 0);
    if (buffer_length == 0) {
        return .{ .buffer = try allocator.allocSentinel(u8, 0, 0) };
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    if (buffer_length > 0) {
        _ = self.function_table.GetAppContainerFilePaths(buffer.ptr, buffer_length);
    }
    return .{ .buffer = buffer };
}

pub fn getRuntimeVersion(self: Self) [:0]const u8 {
    return std.mem.span(self.function_table.GetRuntimeVersion());
}

const FunctionTable = extern struct {
    GetRecommendedRenderTargetSize: *const fn (*u32, *u32) callconv(.C) void,
    GetProjectionMatrix: *const fn (common.Eye, f32, f32) callconv(.C) common.Matrix44,
    GetProjectionRaw: *const fn (common.Eye, *f32, *f32, *f32, *f32) callconv(.C) void,
    ComputeDistortion: *const fn (common.Eye, f32, f32, *common.DistortionCoordinates) callconv(.C) bool,
    GetEyeToHeadTransform: *const fn (common.Eye) callconv(.C) common.Matrix34,
    GetTimeSinceLastVsync: *const fn (*f32, *u64) callconv(.C) bool,
    GetD3D9AdapterIndex: *const fn () callconv(.C) i32,
    GetDXGIOutputInfo: *const fn (*i32) callconv(.C) void,

    // skip vulkan
    GetOutputDevice: usize,

    IsDisplayOnDesktop: *const fn () callconv(.C) bool,
    SetDisplayVisibility: *const fn (bool) callconv(.C) bool,
    GetDeviceToAbsoluteTrackingPose: *const fn (common.TrackingUniverseOrigin, f32, [*c]common.TrackedDevicePose, u32) callconv(.C) void,
    GetSeatedZeroPoseToStandingAbsoluteTrackingPose: *const fn () callconv(.C) common.Matrix34,
    GetRawZeroPoseToStandingAbsoluteTrackingPose: *const fn () callconv(.C) common.Matrix34,
    GetSortedTrackedDeviceIndicesOfClass: *const fn (common.TrackedDeviceClass, [*c]common.TrackedDeviceIndex, u32, common.TrackedDeviceIndex) callconv(.C) u32,
    GetTrackedDeviceActivityLevel: *const fn (common.TrackedDeviceIndex) callconv(.C) common.DeviceActivityLevel,
    ApplyTransform: *const fn (*common.TrackedDevicePose, *common.TrackedDevicePose, *common.Matrix34) callconv(.C) void,
    GetTrackedDeviceIndexForControllerRole: *const fn (common.TrackedControllerRole) callconv(.C) common.TrackedDeviceIndex,
    GetControllerRoleForTrackedDeviceIndex: *const fn (common.TrackedDeviceIndex) callconv(.C) common.TrackedControllerRole,
    GetTrackedDeviceClass: *const fn (common.TrackedDeviceIndex) callconv(.C) common.TrackedDeviceClass,
    IsTrackedDeviceConnected: *const fn (common.TrackedDeviceIndex) callconv(.C) bool,
    GetBoolTrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, common.TrackedDeviceProperty, *common.TrackedPropertyErrorCode) callconv(.C) bool,
    GetFloatTrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, common.TrackedDeviceProperty, *common.TrackedPropertyErrorCode) callconv(.C) f32,
    GetInt32TrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, common.TrackedDeviceProperty, *common.TrackedPropertyErrorCode) callconv(.C) i32,
    GetUint64TrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, common.TrackedDeviceProperty, *common.TrackedPropertyErrorCode) callconv(.C) u64,
    GetMatrix34TrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, common.TrackedDeviceProperty, *common.TrackedPropertyErrorCode) callconv(.C) common.Matrix34,
    GetArrayTrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, common.TrackedDeviceProperty, common.PropertyTypeTagCode, ?*anyopaque, u32, *common.TrackedPropertyErrorCode) callconv(.C) u32,
    GetStringTrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, common.TrackedDeviceProperty.String, [*c]u8, u32, *common.TrackedPropertyErrorCode) callconv(.C) u32,
    GetPropErrorNameFromEnum: *const fn (common.TrackedPropertyErrorCode) callconv(.C) [*c]u8,
    PollNextEvent: *const fn (*common.Event, u32) callconv(.C) bool,
    PollNextEventWithPose: *const fn (common.TrackingUniverseOrigin, *common.Event, u32, *common.TrackedDevicePose) callconv(.C) bool,
    GetEventTypeNameFromEnum: *const fn (common.EventType) callconv(.C) [*c]u8,
    GetHiddenAreaMesh: *const fn (common.Eye, common.HiddenAreaMeshType) callconv(.C) common.HiddenAreaMesh,
    GetControllerState: *const fn (common.TrackedDeviceIndex, *common.ControllerState, u32) callconv(.C) bool,
    GetControllerStateWithPose: *const fn (common.TrackingUniverseOrigin, common.TrackedDeviceIndex, *common.ControllerState, u32, *common.TrackedDevicePose) callconv(.C) bool,
    TriggerHapticPulse: *const fn (common.TrackedDeviceIndex, u32, c_ushort) callconv(.C) void,
    GetButtonIdNameFromEnum: *const fn (common.ButtonId) callconv(.C) [*c]u8,
    GetControllerAxisTypeNameFromEnum: *const fn (common.ControllerAxisType) callconv(.C) [*c]u8,
    IsInputAvailable: *const fn () callconv(.C) bool,
    IsSteamVRDrawingControllers: *const fn () callconv(.C) bool,
    ShouldApplicationPause: *const fn () callconv(.C) bool,
    ShouldApplicationReduceRenderingWork: *const fn () callconv(.C) bool,
    PerformFirmwareUpdate: *const fn (common.TrackedDeviceIndex) callconv(.C) common.FirmwareErrorCode,
    AcknowledgeQuit_Exiting: *const fn () callconv(.C) void,
    GetAppContainerFilePaths: *const fn ([*c]u8, u32) callconv(.C) u32,
    GetRuntimeVersion: *const fn () callconv(.C) [*c]u8,
};
