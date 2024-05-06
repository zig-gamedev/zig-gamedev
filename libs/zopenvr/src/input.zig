const std = @import("std");

const common = @import("common.zig");

function_table: *FunctionTable,

const Self = @This();
const version = "IVRInput_010";
pub fn init() common.InitError!Self {
    return .{
        .function_table = try common.getFunctionTable(FunctionTable, version),
    };
}

pub fn setActionManifestPath(self: Self, action_manifest_path: [:0]const u8) common.InputError!void {
    const error_code = self.function_table.SetActionManifestPath(@constCast(action_manifest_path.ptr));
    try error_code.maybe();
}

pub fn getActionSetHandle(self: Self, action_set_name: [:0]const u8) common.InputError!common.ActionSetHandle {
    var result: common.ActionSetHandle = undefined;
    const error_code = self.function_table.GetActionSetHandle(@constCast(action_set_name.ptr), &result);
    try error_code.maybe();
    return result;
}
pub fn getActionHandle(self: Self, action_name: [:0]const u8) common.InputError!common.ActionHandle {
    var result: common.ActionHandle = undefined;
    const error_code = self.function_table.GetActionHandle(@constCast(action_name.ptr), &result);
    try error_code.maybe();
    return result;
}
pub fn getInputSourceHandle(self: Self, input_source_path: [:0]const u8) common.InputError!common.InputValueHandle {
    var result: common.InputValueHandle = undefined;
    const error_code = self.function_table.GetInputSourceHandle(@constCast(input_source_path.ptr), &result);
    try error_code.maybe();
    return result;
}

pub fn updateActionState(self: Self, sets: []common.ActiveActionSet) common.InputError!void {
    const error_code = self.function_table.UpdateActionState(@constCast(sets.ptr), @sizeOf(common.ActiveActionSet), @intCast(sets.len));
    try error_code.maybe();
}

pub fn getDigitalActionData(
    self: Self,
    action: common.ActionHandle,
    restrict_to_device: common.InputValueHandle,
) common.InputError!common.InputDigitalActionData {
    var result: common.InputDigitalActionData = undefined;

    const error_code = self.function_table.GetDigitalActionData(
        action,
        &result,
        @sizeOf(common.InputDigitalActionData),
        restrict_to_device,
    );
    try error_code.maybe();

    return result;
}
pub fn getAnalogActionData(
    self: Self,
    action: common.ActionHandle,
    restrict_to_device: common.InputValueHandle,
) common.InputError!common.InputAnalogActionData {
    var result: common.InputAnalogActionData = undefined;

    const error_code = self.function_table.GetAnalogActionData(
        action,
        &result,
        @sizeOf(common.InputAnalogActionData),
        restrict_to_device,
    );
    try error_code.maybe();

    return result;
}

pub fn getPoseActionDataRelativeToNow(
    self: Self,
    action: common.ActionHandle,
    origin: common.TrackingUniverseOrigin,
    predicted_seconds_from_now: f32,
    restrict_to_device: common.InputValueHandle,
) common.InputError!common.InputPoseActionData {
    var result: common.InputPoseActionData = undefined;

    const error_code = self.function_table.GetPoseActionDataRelativeToNow(
        action,
        origin,
        predicted_seconds_from_now,
        &result,
        @sizeOf(common.InputPoseActionData),
        restrict_to_device,
    );
    try error_code.maybe();

    return result;
}
pub fn getPoseActionDataForNextFrame(
    self: Self,
    action: common.ActionHandle,
    origin: common.TrackingUniverseOrigin,
    restrict_to_device: common.InputValueHandle,
) common.InputError!common.InputPoseActionData {
    var result: common.InputPoseActionData = undefined;

    const error_code = self.function_table.GetPoseActionDataForNextFrame(
        action,
        origin,
        &result,
        @sizeOf(common.InputPoseActionData),
        restrict_to_device,
    );
    try error_code.maybe();

    return result;
}
pub fn getSkeletalActionData(
    self: Self,
    action: common.ActionHandle,
) common.InputError!common.InputSkeletalActionData {
    var result: common.InputSkeletalActionData = undefined;

    const error_code = self.function_table.GetSkeletalActionData(
        action,
        &result,
        @sizeOf(common.InputSkeletalActionData),
    );
    try error_code.maybe();

    return result;
}
pub fn getDominantHand(
    self: Self,
) common.InputError!common.TrackedControllerRole {
    var result: common.TrackedControllerRole = undefined;

    const error_code = self.function_table.GetDominantHand(
        &result,
    );
    try error_code.maybe();

    return result;
}
pub fn setDominantHand(self: Self, dominant_hand: common.TrackedControllerRole) common.InputError!void {
    const error_code = self.function_table.SetDominantHand(
        dominant_hand,
    );
    try error_code.maybe();
}
pub fn getBoneCount(
    self: Self,
    action: common.ActionHandle,
) common.InputError!u32 {
    var result: u32 = undefined;

    const error_code = self.function_table.GetBoneCount(
        action,
        &result,
    );
    try error_code.maybe();

    return result;
}

pub fn allocBoneName(
    self: Self,
    allocator: std.mem.Allocator,
    action: common.ActionHandle,
    bone_index: common.BoneIndex,
) (common.InputError || error{OutOfMemory})![:0]u8 {
    var buffer: [common.max_bone_name_length:0]u8 = std.mem.zeroes([common.max_bone_name_length:0]u8);

    const error_code = self.function_table.GetBoneName(
        action,
        bone_index,
        &buffer,
        @intCast(common.max_bone_name_length),
    );
    try error_code.maybe();

    const buffer_slice = std.mem.sliceTo(&buffer, 0);
    const result = try allocator.allocSentinel(u8, buffer_slice.len, 0);
    std.mem.copyForwards(u8, result, buffer_slice);
    return result;
}

pub fn allocSkeletalReferenceTransforms(
    self: Self,
    allocator: std.mem.Allocator,
    action: common.ActionHandle,
    transform_space: common.SkeletalTransformSpace,
    reference_pose: common.SkeletalReferencePose,
    transform_count: usize,
) (common.InputError || error{OutOfMemory})![]common.BoneTransform {
    const result = try allocator.alloc(common.BoneTransform, transform_count);
    errdefer allocator.free(result);

    if (transform_count > 0) {
        const error_code = self.function_table.GetSkeletalReferenceTransforms(
            action,
            transform_space,
            reference_pose,
            result.ptr,
            @intCast(result.len),
        );
        try error_code.maybe();
    }
    return result;
}

pub fn getSkeletalTrackingLevel(self: Self, action: common.ActionHandle) common.InputError!common.SkeletalTrackingLevel {
    var result: common.SkeletalTrackingLevel = undefined;
    const error_code = self.function_table.GetSkeletalTrackingLevel(action, &result);
    try error_code.maybe();
    return result;
}
pub fn allocSkeletalBoneData(
    self: Self,
    allocator: std.mem.Allocator,
    action: common.ActionHandle,
    transform_space: common.SkeletalTransformSpace,
    motion_range: common.SkeletalMotionRange,
    transform_count: usize,
) (common.InputError || error{OutOfMemory})![]common.BoneTransform {
    const result = try allocator.alloc(common.BoneTransform, transform_count);
    errdefer allocator.free(result);

    if (transform_count > 0) {
        const error_code = self.function_table.GetSkeletalBoneData(
            action,
            transform_space,
            motion_range,
            result.ptr,
            @intCast(result.len),
        );
        try error_code.maybe();
    }
    return result;
}
pub fn getSkeletalSummaryData(self: Self, action: common.ActionHandle, summary_type: common.SummaryType) common.InputError!common.SkeletalSummaryData {
    var result: common.SkeletalSummaryData = undefined;
    const error_code = self.function_table.GetSkeletalSummaryData(action, summary_type, &result);
    try error_code.maybe();
    return result;
}

pub fn allocSkeletalBoneDataCompressed(
    self: Self,
    allocator: std.mem.Allocator,
    action: common.ActionHandle,
    motion_range: common.SkeletalMotionRange,
) (common.InputError || error{OutOfMemory})![]u8 {
    var buffer_length: u32 = 0;
    self.function_table.GetSkeletalBoneDataCompressed(action, motion_range, null, 0, &buffer_length).maybe() catch |err| switch (err) {
        else => return err,
    };
    const result = try allocator.alloc(u8, buffer_length);
    errdefer allocator.free(result);

    if (buffer_length > 0) {
        const error_code = self.function_table.GetSkeletalBoneDataCompressed(
            action,
            motion_range,
            result.ptr,
            @intCast(result.len),
            &buffer_length,
        );
        try error_code.maybe();
    }
    return result;
}

pub fn allocDecompressSkeletalBoneData(
    self: Self,
    allocator: std.mem.Allocator,
    compressed_buffer: []u8,
    transform_space: common.SkeletalTransformSpace,
    count: u32,
) (common.InputError || error{OutOfMemory})![]common.BoneTransform {
    const result = try allocator.alloc(common.BoneTransform, count);
    errdefer allocator.free(result);

    if (count > 0) {
        const error_code = self.function_table.DecompressSkeletalBoneData(
            compressed_buffer.ptr,
            @intCast(compressed_buffer.len),
            transform_space,
            result.ptr,
            count,
        );
        try error_code.maybe();
    }
    return result;
}

pub fn triggerHapticVibrationAction(
    self: Self,
    action: common.ActionHandle,
    start_seconds_from_now: f32,
    duration_seconds: f32,
    frequency: f32,
    amplitude: f32,
    restrict_to_device: common.InputValueHandle,
) common.InputError!void {
    const error_code = self.function_table.TriggerHapticVibrationAction(
        action,
        start_seconds_from_now,
        duration_seconds,
        frequency,
        amplitude,
        restrict_to_device,
    );
    try error_code.maybe();
}

pub fn allocActionOrigins(
    self: Self,
    allocator: std.mem.Allocator,
    action_set_handle: common.ActionSetHandle,
    digital_action_handle: common.ActionHandle,
    count: u32,
) (common.InputError || error{OutOfMemory})![]common.InputValueHandle {
    const result = try allocator.alloc(common.InputValueHandle, count);
    errdefer allocator.free(result);

    if (count > 0) {
        const error_code = self.function_table.GetActionOrigins(
            action_set_handle,
            digital_action_handle,
            result.ptr,
            count,
        );
        try error_code.maybe();
    }
    return result;
}
pub fn allocOriginLocalizedName(
    self: Self,
    allocator: std.mem.Allocator,
    origin: common.InputValueHandle,
    string_sections_to_include: i32,
    buffer_length: u32,
) (common.InputError || error{OutOfMemory})![:0]u8 {
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }
    const result = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    errdefer allocator.free(result);

    if (buffer_length > 0) {
        const error_code = self.function_table.GetOriginLocalizedName(
            origin,
            result.ptr,
            buffer_length,
            string_sections_to_include,
        );
        try error_code.maybe();
    }
    return result;
}
pub fn getOriginTrackedDeviceInfo(
    self: Self,
    origin: common.InputValueHandle,
) common.InputError!common.InputOriginInfo {
    var result: common.InputOriginInfo = undefined;
    const error_code = self.function_table.GetOriginTrackedDeviceInfo(
        origin,
        &result,
        @sizeOf(common.InputValueHandle),
    );
    try error_code.maybe();

    return result;
}

pub fn allocActionBindingInfo(
    self: Self,
    allocator: std.mem.Allocator,
    action: common.ActionHandle,
    count: u32,
) (common.InputError || error{OutOfMemory})![]common.InputBindingInfo {
    var result = try allocator.alloc(common.InputBindingInfo, count);
    errdefer allocator.free(result);

    var returned_binding_info_count: u32 = 0;
    if (count > 0) {
        const error_code = self.function_table.GetActionBindingInfo(
            action,
            result.ptr,
            @sizeOf(common.InputBindingInfo),
            count,
            &returned_binding_info_count,
        );
        try error_code.maybe();
        result = try allocator.realloc(result, returned_binding_info_count);
    }
    return result;
}
pub fn showActionOrigins(self: Self, action_set_handle: common.ActionSetHandle, action_handle: common.ActionHandle) common.InputError!void {
    const error_code = self.function_table.ShowActionOrigins(
        action_set_handle,
        action_handle,
    );
    try error_code.maybe();
}
pub fn showBindingsForActionSet(self: Self, sets: []const common.ActiveActionSet, origin_to_highlight: common.InputValueHandle) common.InputError!void {
    const error_code = self.function_table.ShowBindingsForActionSet(
        @constCast(sets.ptr),
        @sizeOf(common.ActiveActionSet),
        @intCast(sets.len),
        origin_to_highlight,
    );
    try error_code.maybe();
}

pub fn getComponentStateForBinding(
    self: Self,
    render_model_name: [:0]const u8,
    component_name: [:0]const u8,
    origin_info: []const common.InputBindingInfo,
) common.InputError!common.RenderModel.ComponentState {
    var result: common.RenderModel.ComponentState = undefined;
    const error_code = self.function_table.GetComponentStateForBinding(
        @constCast(render_model_name.ptr),
        @constCast(component_name.ptr),
        @constCast(origin_info.ptr),
        @sizeOf(common.InputBindingInfo),
        @intCast(origin_info.len),
        &result,
    );
    try error_code.maybe();
    return result;
}
pub fn isUsingLegacyInput(self: Self) bool {
    return self.function_table.IsUsingLegacyInput();
}

pub fn openBindingUI(
    self: Self,
    app_key: [:0]const u8,
    action_set_handle: common.ActionSetHandle,
    device_handle: common.InputValueHandle,
    show_on_desktop: bool,
) common.InputError!void {
    const error_code = self.function_table.OpenBindingUI(
        @constCast(app_key.ptr),
        action_set_handle,
        device_handle,
        show_on_desktop,
    );
    try error_code.maybe();
}

pub fn allocBindingVariant(
    self: Self,
    allocator: std.mem.Allocator,
    device_path: common.InputValueHandle,
    buffer_length: u32,
) (common.InputError || error{OutOfMemory})![:0]u8 {
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }
    const result = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    errdefer allocator.free(result);

    if (buffer_length > 0) {
        const error_code = self.function_table.GetBindingVariant(
            device_path,
            result.ptr,
            buffer_length,
        );
        try error_code.maybe();
    }
    return result;
}

const FunctionTable = extern struct {
    SetActionManifestPath: *const fn ([*c]u8) callconv(.C) common.InputErrorCode,
    GetActionSetHandle: *const fn ([*c]u8, *common.ActionSetHandle) callconv(.C) common.InputErrorCode,
    GetActionHandle: *const fn ([*c]u8, *common.ActionHandle) callconv(.C) common.InputErrorCode,
    GetInputSourceHandle: *const fn ([*c]u8, *common.InputValueHandle) callconv(.C) common.InputErrorCode,
    UpdateActionState: *const fn ([*c]common.ActiveActionSet, u32, u32) callconv(.C) common.InputErrorCode,
    GetDigitalActionData: *const fn (common.ActionHandle, *common.InputDigitalActionData, u32, common.InputValueHandle) callconv(.C) common.InputErrorCode,
    GetAnalogActionData: *const fn (common.ActionHandle, *common.InputAnalogActionData, u32, common.InputValueHandle) callconv(.C) common.InputErrorCode,
    GetPoseActionDataRelativeToNow: *const fn (common.ActionHandle, common.TrackingUniverseOrigin, f32, *common.InputPoseActionData, u32, common.InputValueHandle) callconv(.C) common.InputErrorCode,
    GetPoseActionDataForNextFrame: *const fn (common.ActionHandle, common.TrackingUniverseOrigin, *common.InputPoseActionData, u32, common.InputValueHandle) callconv(.C) common.InputErrorCode,
    GetSkeletalActionData: *const fn (common.ActionHandle, *common.InputSkeletalActionData, u32) callconv(.C) common.InputErrorCode,
    GetDominantHand: *const fn (*common.TrackedControllerRole) callconv(.C) common.InputErrorCode,
    SetDominantHand: *const fn (common.TrackedControllerRole) callconv(.C) common.InputErrorCode,
    GetBoneCount: *const fn (common.ActionHandle, *u32) callconv(.C) common.InputErrorCode,
    GetBoneHierarchy: *const fn (common.ActionHandle, [*c]common.BoneIndex, u32) callconv(.C) common.InputErrorCode,
    GetBoneName: *const fn (common.ActionHandle, common.BoneIndex, [*c]u8, u32) callconv(.C) common.InputErrorCode,
    GetSkeletalReferenceTransforms: *const fn (common.ActionHandle, common.SkeletalTransformSpace, common.SkeletalReferencePose, [*c]common.BoneTransform, u32) callconv(.C) common.InputErrorCode,
    GetSkeletalTrackingLevel: *const fn (common.ActionHandle, *common.SkeletalTrackingLevel) callconv(.C) common.InputErrorCode,
    GetSkeletalBoneData: *const fn (common.ActionHandle, common.SkeletalTransformSpace, common.SkeletalMotionRange, [*c]common.BoneTransform, u32) callconv(.C) common.InputErrorCode,
    GetSkeletalSummaryData: *const fn (common.ActionHandle, common.SummaryType, *common.SkeletalSummaryData) callconv(.C) common.InputErrorCode,
    GetSkeletalBoneDataCompressed: *const fn (common.ActionHandle, common.SkeletalMotionRange, ?*anyopaque, u32, [*c]u32) callconv(.C) common.InputErrorCode,
    DecompressSkeletalBoneData: *const fn (?*anyopaque, u32, common.SkeletalTransformSpace, [*c]common.BoneTransform, u32) callconv(.C) common.InputErrorCode,
    TriggerHapticVibrationAction: *const fn (common.ActionHandle, f32, f32, f32, f32, common.InputValueHandle) callconv(.C) common.InputErrorCode,
    GetActionOrigins: *const fn (common.ActionSetHandle, common.ActionHandle, [*c]common.InputValueHandle, u32) callconv(.C) common.InputErrorCode,
    GetOriginLocalizedName: *const fn (common.InputValueHandle, [*c]u8, u32, i32) callconv(.C) common.InputErrorCode,
    GetOriginTrackedDeviceInfo: *const fn (common.InputValueHandle, *common.InputOriginInfo, u32) callconv(.C) common.InputErrorCode,
    GetActionBindingInfo: *const fn (common.ActionHandle, [*c]common.InputBindingInfo, u32, u32, [*c]u32) callconv(.C) common.InputErrorCode,
    ShowActionOrigins: *const fn (common.ActionSetHandle, common.ActionHandle) callconv(.C) common.InputErrorCode,
    ShowBindingsForActionSet: *const fn ([*c]common.ActiveActionSet, u32, u32, common.InputValueHandle) callconv(.C) common.InputErrorCode,
    GetComponentStateForBinding: *const fn ([*c]u8, [*c]u8, [*c]common.InputBindingInfo, u32, u32, *common.RenderModel.ComponentState) callconv(.C) common.InputErrorCode,
    IsUsingLegacyInput: *const fn () callconv(.C) bool,
    OpenBindingUI: *const fn ([*c]u8, common.ActionSetHandle, common.InputValueHandle, bool) callconv(.C) common.InputErrorCode,
    GetBindingVariant: *const fn (common.InputValueHandle, [*c]u8, u32) callconv(.C) common.InputErrorCode,
};
