const std = @import("std");

const common = @import("common.zig");
const RenderModel = @import("render_models.zig").RenderModel;

function_table: *FunctionTable,

const Self = @This();
const version = "IVRInput_010";
pub fn init() common.InitError!Self {
    return .{
        .function_table = try common.getFunctionTable(FunctionTable, version),
    };
}

pub const InputError = error{
    NameNotFound,
    WrongType,
    InvalidHandle,
    InvalidParam,
    NoSteam,
    MaxCapacityReached,
    IPCError,
    NoActiveActionSet,
    InvalidDevice,
    InvalidSkeleton,
    InvalidBoneCount,
    InvalidCompressedData,
    NoData,
    BufferTooSmall,
    MismatchedActionManifest,
    MissingSkeletonData,
    InvalidBoneIndex,
    InvalidPriority,
    PermissionDenied,
    InvalidRenderModel,
};

pub const InputErrorCode = enum(i32) {
    none = 0,
    name_not_found = 1,
    wrong_type = 2,
    invalid_handle = 3,
    invalid_param = 4,
    no_steam = 5,
    max_capacity_reached = 6,
    ipc_error = 7,
    no_active_action_set = 8,
    invalid_device = 9,
    invalid_skeleton = 10,
    invalid_bone_count = 11,
    invalid_compressed_data = 12,
    no_data = 13,
    buffer_too_small = 14,
    mismatched_action_manifest = 15,
    missing_skeleton_data = 16,
    invalid_bone_index = 17,
    invalid_priority = 18,
    permission_denied = 19,
    invalid_render_model = 20,

    pub fn maybe(error_code: InputErrorCode) InputError!void {
        return switch (error_code) {
            .none => {},
            .name_not_found => InputError.NameNotFound,
            .wrong_type => InputError.WrongType,
            .invalid_handle => InputError.InvalidHandle,
            .invalid_param => InputError.InvalidParam,
            .no_steam => InputError.NoSteam,
            .max_capacity_reached => InputError.MaxCapacityReached,
            .ipc_error => InputError.IPCError,
            .no_active_action_set => InputError.NoActiveActionSet,
            .invalid_device => InputError.InvalidDevice,
            .invalid_skeleton => InputError.InvalidSkeleton,
            .invalid_bone_count => InputError.InvalidBoneCount,
            .invalid_compressed_data => InputError.InvalidCompressedData,
            .no_data => InputError.NoData,
            .buffer_too_small => InputError.BufferTooSmall,
            .mismatched_action_manifest => InputError.MismatchedActionManifest,
            .missing_skeleton_data => InputError.MissingSkeletonData,
            .invalid_bone_index => InputError.InvalidBoneIndex,
            .invalid_priority => InputError.InvalidPriority,
            .permission_denied => InputError.PermissionDenied,
            .invalid_render_model => InputError.InvalidRenderModel,
        };
    }
};

pub const ActionHandle = u64;
pub const ActionSetHandle = u64;

pub const ActiveActionSet = extern struct {
    action_set: ActionSetHandle,
    restricted_to_device: common.InputValueHandle,
    secondary_action_set: ActionSetHandle,
    padding: u32,
    priority: i32,
};

pub const InputDigitalActionData = extern struct {
    active: bool,
    active_origin: common.InputValueHandle,
    state: bool,
    changed: bool,
    update_time: f32,
};
pub const InputAnalogActionData = extern struct {
    active: bool,
    active_origin: common.InputValueHandle,
    x: f32,
    y: f32,
    z: f32,
    delta_x: f32,
    delta_y: f32,
    delta_z: f32,
    update_time: f32,
};

pub const InputPoseActionData = extern struct {
    active: bool,
    active_origin: common.InputValueHandle,
    pose: common.TrackedDevicePose,
};
pub const InputSkeletalActionData = extern struct {
    active: bool,
    active_origin: common.InputValueHandle,
};
pub const BoneIndex = i32;
pub const SkeletalTransformSpace = enum(i32) {
    model = 0,
    parent = 1,
};
pub const SkeletalReferencePose = enum(i32) {
    bind_pose = 0,
    open_hand = 1,
    fist = 2,
    grip_limit = 3,
};

pub const BoneTransform = extern struct {
    position: common.Vector4,
    orientation: common.Quaternionf,
};

pub const SkeletalTrackingLevel = enum(i32) {
    estimated = 0,
    partial = 1,
    full = 2,

    const count = @typeInfo(SkeletalTrackingLevel).Enum.fields.len;
    const max = SkeletalTrackingLevel.full;
};

pub const SkeletalMotionRange = enum(i32) {
    with_controller = 0,
    without_controller = 1,
};

pub const SummaryType = enum(i32) {
    from_animation = 0,
    from_device = 1,
};

pub const SkeletalSummaryData = extern struct {
    finger_curl: [5]f32,
    finger_splay: [4]f32,
};

pub const InputOriginInfo = extern struct {
    device_path: common.InputValueHandle,
    tracked_device_index: common.TrackedDeviceIndex,
    render_model_component_name: [127:0]u8,
};

pub const InputBindingInfo = extern struct {
    device_path_name: [127:0]u8,
    input_path_name: [127:0]u8,
    mode_name: [127:0]u8,
    slot_name: [127:0]u8,
    input_source_type: [31:0]u8,
};

pub fn setActionManifestPath(self: Self, action_manifest_path: [:0]const u8) InputError!void {
    const error_code = self.function_table.SetActionManifestPath(@constCast(action_manifest_path.ptr));
    try error_code.maybe();
}

pub fn getActionSetHandle(self: Self, action_set_name: [:0]const u8) InputError!ActionSetHandle {
    var result: ActionSetHandle = undefined;
    const error_code = self.function_table.GetActionSetHandle(@constCast(action_set_name.ptr), &result);
    try error_code.maybe();
    return result;
}
pub fn getActionHandle(self: Self, action_name: [:0]const u8) InputError!ActionHandle {
    var result: ActionHandle = undefined;
    const error_code = self.function_table.GetActionHandle(@constCast(action_name.ptr), &result);
    try error_code.maybe();
    return result;
}
pub fn getInputSourceHandle(self: Self, input_source_path: [:0]const u8) InputError!common.InputValueHandle {
    var result: common.InputValueHandle = undefined;
    const error_code = self.function_table.GetInputSourceHandle(@constCast(input_source_path.ptr), &result);
    try error_code.maybe();
    return result;
}

pub fn updateActionState(self: Self, sets: []ActiveActionSet) InputError!void {
    const error_code = self.function_table.UpdateActionState(@constCast(sets.ptr), @sizeOf(ActiveActionSet), @intCast(sets.len));
    try error_code.maybe();
}

pub fn getDigitalActionData(
    self: Self,
    action: ActionHandle,
    restrict_to_device: common.InputValueHandle,
) InputError!InputDigitalActionData {
    var result: InputDigitalActionData = undefined;

    const error_code = self.function_table.GetDigitalActionData(
        action,
        &result,
        @sizeOf(InputDigitalActionData),
        restrict_to_device,
    );
    try error_code.maybe();

    return result;
}
pub fn getAnalogActionData(
    self: Self,
    action: ActionHandle,
    restrict_to_device: common.InputValueHandle,
) InputError!InputAnalogActionData {
    var result: InputAnalogActionData = undefined;

    const error_code = self.function_table.GetAnalogActionData(
        action,
        &result,
        @sizeOf(InputAnalogActionData),
        restrict_to_device,
    );
    try error_code.maybe();

    return result;
}

pub fn getPoseActionDataRelativeToNow(
    self: Self,
    action: ActionHandle,
    origin: common.TrackingUniverseOrigin,
    predicted_seconds_from_now: f32,
    restrict_to_device: common.InputValueHandle,
) InputError!InputPoseActionData {
    var result: InputPoseActionData = undefined;

    const error_code = self.function_table.GetPoseActionDataRelativeToNow(
        action,
        origin,
        predicted_seconds_from_now,
        &result,
        @sizeOf(InputPoseActionData),
        restrict_to_device,
    );
    try error_code.maybe();

    return result;
}
pub fn getPoseActionDataForNextFrame(
    self: Self,
    action: ActionHandle,
    origin: common.TrackingUniverseOrigin,
    restrict_to_device: common.InputValueHandle,
) InputError!InputPoseActionData {
    var result: InputPoseActionData = undefined;

    const error_code = self.function_table.GetPoseActionDataForNextFrame(
        action,
        origin,
        &result,
        @sizeOf(InputPoseActionData),
        restrict_to_device,
    );
    try error_code.maybe();

    return result;
}
pub fn getSkeletalActionData(
    self: Self,
    action: ActionHandle,
) InputError!InputSkeletalActionData {
    var result: InputSkeletalActionData = undefined;

    const error_code = self.function_table.GetSkeletalActionData(
        action,
        &result,
        @sizeOf(InputSkeletalActionData),
    );
    try error_code.maybe();

    return result;
}
pub fn getDominantHand(
    self: Self,
) InputError!common.TrackedControllerRole {
    var result: common.TrackedControllerRole = undefined;

    const error_code = self.function_table.GetDominantHand(
        &result,
    );
    try error_code.maybe();

    return result;
}
pub fn setDominantHand(self: Self, dominant_hand: common.TrackedControllerRole) InputError!void {
    const error_code = self.function_table.SetDominantHand(
        dominant_hand,
    );
    try error_code.maybe();
}
pub fn getBoneCount(
    self: Self,
    action: ActionHandle,
) InputError!u32 {
    var result: u32 = undefined;

    const error_code = self.function_table.GetBoneCount(
        action,
        &result,
    );
    try error_code.maybe();

    return result;
}
pub const max_bone_name_length: usize = 32;
pub fn allocBoneName(
    self: Self,
    allocator: std.mem.Allocator,
    action: ActionHandle,
    bone_index: BoneIndex,
) (InputError || error{OutOfMemory})![:0]u8 {
    var buffer: [max_bone_name_length:0]u8 = std.mem.zeroes([max_bone_name_length:0]u8);

    const error_code = self.function_table.GetBoneName(
        action,
        bone_index,
        &buffer,
        @intCast(max_bone_name_length),
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
    action: ActionHandle,
    transform_space: SkeletalTransformSpace,
    reference_pose: SkeletalReferencePose,
    transform_count: usize,
) (InputError || error{OutOfMemory})![]BoneTransform {
    const result = try allocator.alloc(BoneTransform, transform_count);
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

pub fn getSkeletalTrackingLevel(self: Self, action: ActionHandle) InputError!SkeletalTrackingLevel {
    var result: SkeletalTrackingLevel = undefined;
    const error_code = self.function_table.GetSkeletalTrackingLevel(action, &result);
    try error_code.maybe();
    return result;
}
pub fn allocSkeletalBoneData(
    self: Self,
    allocator: std.mem.Allocator,
    action: ActionHandle,
    transform_space: SkeletalTransformSpace,
    motion_range: SkeletalMotionRange,
    transform_count: usize,
) (InputError || error{OutOfMemory})![]BoneTransform {
    const result = try allocator.alloc(BoneTransform, transform_count);
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
pub fn getSkeletalSummaryData(self: Self, action: ActionHandle, summary_type: SummaryType) InputError!SkeletalSummaryData {
    var result: SkeletalSummaryData = undefined;
    const error_code = self.function_table.GetSkeletalSummaryData(action, summary_type, &result);
    try error_code.maybe();
    return result;
}

pub fn allocSkeletalBoneDataCompressed(
    self: Self,
    allocator: std.mem.Allocator,
    action: ActionHandle,
    motion_range: SkeletalMotionRange,
) (InputError || error{OutOfMemory})![]u8 {
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
    transform_space: SkeletalTransformSpace,
    count: u32,
) (InputError || error{OutOfMemory})![]BoneTransform {
    const result = try allocator.alloc(BoneTransform, count);
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
    action: ActionHandle,
    start_seconds_from_now: f32,
    duration_seconds: f32,
    frequency: f32,
    amplitude: f32,
    restrict_to_device: common.InputValueHandle,
) InputError!void {
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
    action_set_handle: ActionSetHandle,
    digital_action_handle: ActionHandle,
    count: u32,
) (InputError || error{OutOfMemory})![]common.InputValueHandle {
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
) (InputError || error{OutOfMemory})![:0]u8 {
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
) InputError!InputOriginInfo {
    var result: InputOriginInfo = undefined;
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
    action: ActionHandle,
    count: u32,
) (InputError || error{OutOfMemory})![]InputBindingInfo {
    var result = try allocator.alloc(InputBindingInfo, count);
    errdefer allocator.free(result);

    var returned_binding_info_count: u32 = 0;
    if (count > 0) {
        const error_code = self.function_table.GetActionBindingInfo(
            action,
            result.ptr,
            @sizeOf(InputBindingInfo),
            count,
            &returned_binding_info_count,
        );
        try error_code.maybe();
        result = try allocator.realloc(result, returned_binding_info_count);
    }
    return result;
}
pub fn showActionOrigins(self: Self, action_set_handle: ActionSetHandle, action_handle: ActionHandle) InputError!void {
    const error_code = self.function_table.ShowActionOrigins(
        action_set_handle,
        action_handle,
    );
    try error_code.maybe();
}
pub fn showBindingsForActionSet(self: Self, sets: []const ActiveActionSet, origin_to_highlight: common.InputValueHandle) InputError!void {
    const error_code = self.function_table.ShowBindingsForActionSet(
        @constCast(sets.ptr),
        @sizeOf(ActiveActionSet),
        @intCast(sets.len),
        origin_to_highlight,
    );
    try error_code.maybe();
}

pub fn getComponentStateForBinding(
    self: Self,
    render_model_name: [:0]const u8,
    component_name: [:0]const u8,
    origin_info: []const InputBindingInfo,
) InputError!RenderModel.ComponentState {
    var result: RenderModel.ComponentState = undefined;
    const error_code = self.function_table.GetComponentStateForBinding(
        @constCast(render_model_name.ptr),
        @constCast(component_name.ptr),
        @constCast(origin_info.ptr),
        @sizeOf(InputBindingInfo),
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
    action_set_handle: ActionSetHandle,
    device_handle: common.InputValueHandle,
    show_on_desktop: bool,
) InputError!void {
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
) (InputError || error{OutOfMemory})![:0]u8 {
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

pub const FunctionTable = extern struct {
    SetActionManifestPath: *const fn ([*c]u8) callconv(.C) InputErrorCode,
    GetActionSetHandle: *const fn ([*c]u8, *ActionSetHandle) callconv(.C) InputErrorCode,
    GetActionHandle: *const fn ([*c]u8, *ActionHandle) callconv(.C) InputErrorCode,
    GetInputSourceHandle: *const fn ([*c]u8, *common.InputValueHandle) callconv(.C) InputErrorCode,
    UpdateActionState: *const fn ([*c]ActiveActionSet, u32, u32) callconv(.C) InputErrorCode,
    GetDigitalActionData: *const fn (ActionHandle, *InputDigitalActionData, u32, common.InputValueHandle) callconv(.C) InputErrorCode,
    GetAnalogActionData: *const fn (ActionHandle, *InputAnalogActionData, u32, common.InputValueHandle) callconv(.C) InputErrorCode,
    GetPoseActionDataRelativeToNow: *const fn (ActionHandle, common.TrackingUniverseOrigin, f32, *InputPoseActionData, u32, common.InputValueHandle) callconv(.C) InputErrorCode,
    GetPoseActionDataForNextFrame: *const fn (ActionHandle, common.TrackingUniverseOrigin, *InputPoseActionData, u32, common.InputValueHandle) callconv(.C) InputErrorCode,
    GetSkeletalActionData: *const fn (ActionHandle, *InputSkeletalActionData, u32) callconv(.C) InputErrorCode,
    GetDominantHand: *const fn (*common.TrackedControllerRole) callconv(.C) InputErrorCode,
    SetDominantHand: *const fn (common.TrackedControllerRole) callconv(.C) InputErrorCode,
    GetBoneCount: *const fn (ActionHandle, *u32) callconv(.C) InputErrorCode,
    GetBoneHierarchy: *const fn (ActionHandle, [*c]BoneIndex, u32) callconv(.C) InputErrorCode,
    GetBoneName: *const fn (ActionHandle, BoneIndex, [*c]u8, u32) callconv(.C) InputErrorCode,
    GetSkeletalReferenceTransforms: *const fn (ActionHandle, SkeletalTransformSpace, SkeletalReferencePose, [*c]BoneTransform, u32) callconv(.C) InputErrorCode,
    GetSkeletalTrackingLevel: *const fn (ActionHandle, *SkeletalTrackingLevel) callconv(.C) InputErrorCode,
    GetSkeletalBoneData: *const fn (ActionHandle, SkeletalTransformSpace, SkeletalMotionRange, [*c]BoneTransform, u32) callconv(.C) InputErrorCode,
    GetSkeletalSummaryData: *const fn (ActionHandle, SummaryType, *SkeletalSummaryData) callconv(.C) InputErrorCode,
    GetSkeletalBoneDataCompressed: *const fn (ActionHandle, SkeletalMotionRange, ?*anyopaque, u32, [*c]u32) callconv(.C) InputErrorCode,
    DecompressSkeletalBoneData: *const fn (?*anyopaque, u32, SkeletalTransformSpace, [*c]BoneTransform, u32) callconv(.C) InputErrorCode,
    TriggerHapticVibrationAction: *const fn (ActionHandle, f32, f32, f32, f32, common.InputValueHandle) callconv(.C) InputErrorCode,
    GetActionOrigins: *const fn (ActionSetHandle, ActionHandle, [*c]common.InputValueHandle, u32) callconv(.C) InputErrorCode,
    GetOriginLocalizedName: *const fn (common.InputValueHandle, [*c]u8, u32, i32) callconv(.C) InputErrorCode,
    GetOriginTrackedDeviceInfo: *const fn (common.InputValueHandle, *InputOriginInfo, u32) callconv(.C) InputErrorCode,
    GetActionBindingInfo: *const fn (ActionHandle, [*c]InputBindingInfo, u32, u32, [*c]u32) callconv(.C) InputErrorCode,
    ShowActionOrigins: *const fn (ActionSetHandle, ActionHandle) callconv(.C) InputErrorCode,
    ShowBindingsForActionSet: *const fn ([*c]ActiveActionSet, u32, u32, common.InputValueHandle) callconv(.C) InputErrorCode,
    GetComponentStateForBinding: *const fn ([*c]u8, [*c]u8, [*c]InputBindingInfo, u32, u32, *RenderModel.ComponentState) callconv(.C) InputErrorCode,
    IsUsingLegacyInput: *const fn () callconv(.C) bool,
    OpenBindingUI: *const fn ([*c]u8, ActionSetHandle, common.InputValueHandle, bool) callconv(.C) InputErrorCode,
    GetBindingVariant: *const fn (common.InputValueHandle, [*c]u8, u32) callconv(.C) InputErrorCode,
};
