const std = @import("std");
const OpenVR = @import("zopenvr");

const zglfw = @import("zglfw");

const zwindows = @import("zwindows");
const windows = zwindows.windows;
const d3d12 = zwindows.d3d12;
const dxgi = zwindows.dxgi;

const zd3d12 = @import("zd3d12");

const zgui = @import("zgui");

// We need to export below symbols for DirectX 12 Agility SDK.
pub export const D3D12SDKVersion: u32 = 610;
pub export const D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const content_dir = @import("build_options").content_dir;
const window_title = "zig-gamedev: openvr test";

const ui = @import("./ui.zig");

const Surface = struct {
    window: *zglfw.Window,
    gctx: zd3d12.GraphicsContext,
    scale_factor: f32,
    framebuffer_size: [2]i32,

    const Self = @This();

    fn init(allocator: std.mem.Allocator, width: i32, height: i32) !Self {
        zglfw.windowHintTyped(.client_api, .no_api);
        zglfw.windowHintTyped(.maximized, true);
        const window = try zglfw.Window.create(width, height, window_title, null);

        const win32_window = zglfw.getWin32Window(window) orelse return error.FailedToGetWin32Window;
        const gctx = zd3d12.GraphicsContext.init(.{
            .allocator = allocator,
            .window = win32_window,
        });

        zgui.init(allocator);
        zgui.plot.init();

        {
            const cbv_srv = gctx.cbv_srv_uav_gpu_heaps[0];
            zgui.backend.init(
                window,
                gctx.device,
                zd3d12.GraphicsContext.max_num_buffered_frames,
                @intFromEnum(dxgi.FORMAT.R8G8B8A8_UNORM),
                cbv_srv.heap.?,
                @bitCast(cbv_srv.base.cpu_handle),
                @bitCast(cbv_srv.base.gpu_handle),
            );
        }

        const scale_factor = scale_factor: {
            const scale = window.getContentScale();
            break :scale_factor @max(scale[0], scale[1]);
        };
        {
            _ = zgui.io.addFontFromFile(
                content_dir ++ "Roboto-Medium.ttf",
                std.math.floor(16.0 * scale_factor),
            );

            zgui.getStyle().scaleAllSizes(scale_factor);
        }
        return .{
            .window = window,
            .gctx = gctx,
            .scale_factor = scale_factor,
            .framebuffer_size = [_]i32{ width, height },
        };
    }

    pub fn setFrameBufferSize(self: *Self, next_framebuffer_size: [2]i32) void {
        self.gctx.resize(@intCast(next_framebuffer_size[0]), @intCast(next_framebuffer_size[1]));
        self.framebuffer_size = next_framebuffer_size;
    }

    fn deinit(self: *Self, allocator: std.mem.Allocator) void {
        zgui.backend.deinit();
        zgui.plot.deinit();
        zgui.deinit();
        self.gctx.deinit(allocator);
        self.window.destroy();
    }
};

const SystemWindow = struct {
    projection_matrix_eye: OpenVR.Eye = .left,
    projection_matrix_near: f32 = 0,
    projection_matrix_far: f32 = 0,

    projection_raw_eye: OpenVR.Eye = .left,

    compute_distortion_eye: OpenVR.Eye = .left,
    compute_distortion_u: f32 = 0,
    compute_distortion_v: f32 = 0,
    eye_to_head_transform_eye: OpenVR.Eye = .left,

    set_display_visibility: bool = false,
    set_display_visibility_result: ?bool = null,

    device_to_absolute_tracking_pose_origin: OpenVR.TrackingUniverseOrigin = .seated,
    device_to_absolute_tracking_pose_predicted_seconds_to_photons_from_now: f32 = 0,
    device_to_absolute_tracking_pose_count: usize = 0,

    sorted_tracked_device_indices_of_class_tracked_device_class: OpenVR.TrackedDeviceClass = .invalid,
    sorted_tracked_device_indices_of_class_tracked_device_indices: std.ArrayList(OpenVR.TrackedDeviceIndex),
    sorted_tracked_device_indices_of_class_relative_to_tracked_device_index: OpenVR.TrackedDeviceIndex = 0,

    tracked_device_activity_level_device_index: OpenVR.TrackedDeviceIndex = 0,

    apply_transform_tracked_device_pose: OpenVR.TrackedDevicePose = .{
        .device_to_absolute_tracking = std.mem.zeroes(OpenVR.Matrix34),
        .velocity = std.mem.zeroes(OpenVR.Vector3),
        .angular_velocity = std.mem.zeroes(OpenVR.Vector3),
        .tracking_result = .uninitialized,
        .pose_is_valid = false,
        .device_is_connected = false,
    },
    apply_transform: OpenVR.Matrix34 = std.mem.zeroes(OpenVR.Matrix34),

    tracked_device_index_for_controller_role_device_type: OpenVR.TrackedControllerRole = .invalid,
    controller_role_for_device_index: OpenVR.TrackedDeviceIndex = 0,

    tracked_device_class_device_index: OpenVR.TrackedDeviceIndex = 0,
    is_tracked_device_connected_device_index: OpenVR.TrackedDeviceIndex = 0,

    tracked_device_property_bool_device_index: OpenVR.TrackedDeviceIndex = 0,
    tracked_device_property_bool: OpenVR.TrackedDeviceProperty.Bool = .will_drift_in_yaw,

    tracked_device_property_f32_device_index: OpenVR.TrackedDeviceIndex = 0,
    tracked_device_property_f32: OpenVR.TrackedDeviceProperty.F32 = .device_battery_percentage,

    tracked_device_property_i32_device_index: OpenVR.TrackedDeviceIndex = 0,
    tracked_device_property_i32: OpenVR.TrackedDeviceProperty.I32 = .device_class,

    tracked_device_property_u64_device_index: OpenVR.TrackedDeviceIndex = 0,
    tracked_device_property_u64: OpenVR.TrackedDeviceProperty.U64 = .hardware_revision,

    tracked_device_property_matrix34_device_index: OpenVR.TrackedDeviceIndex = 0,
    tracked_device_property_matrix34: OpenVR.TrackedDeviceProperty.Matrix34 = .status_display_transform,

    tracked_device_property_array_f32_device_index: OpenVR.TrackedDeviceIndex = 0,
    tracked_device_property_array_f32: OpenVR.TrackedDeviceProperty.Array.F32 = .camera_distortion_coefficients,

    tracked_device_property_array_i32_device_index: OpenVR.TrackedDeviceIndex = 0,
    tracked_device_property_array_i32: OpenVR.TrackedDeviceProperty.Array.I32 = .camera_distortion_function,

    tracked_device_property_array_vector4_device_index: OpenVR.TrackedDeviceIndex = 0,
    tracked_device_property_array_vector4: OpenVR.TrackedDeviceProperty.Array.Vector4 = .camera_white_balance,

    tracked_device_property_array_matrix34_device_index: OpenVR.TrackedDeviceIndex = 0,
    tracked_device_property_array_matrix34: OpenVR.TrackedDeviceProperty.Array.Matrix34 = .camera_to_head_transforms,

    tracked_device_property_string_device_index: OpenVR.TrackedDeviceIndex = 0,
    tracked_device_property_string: OpenVR.TrackedDeviceProperty.String = .tracking_system_name,

    prop_error_name_enum: OpenVR.TrackedPropertyErrorCode = .success,

    next_event: ??OpenVR.Event = null,

    poll_next_event_with_pose_origin: OpenVR.TrackingUniverseOrigin = .seated,
    next_event_with_pose: ??OpenVR.EventWithPose = null,
    event_type_name_enum: OpenVR.EventType = .none,

    hidden_area_mesh_eye: OpenVR.Eye = .left,
    hidden_area_mesh_type: OpenVR.HiddenAreaMeshType = .standard,

    trigger_haptic_pulse_device_index: OpenVR.TrackedDeviceIndex = 0,
    trigger_haptic_pulse_axis_id: u32 = 0,
    trigger_haptic_pulse_duration_microseconds: u16 = 0,

    controller_state_device_index: OpenVR.TrackedDeviceIndex = 0,

    controller_state_with_pose_origin: OpenVR.TrackingUniverseOrigin = .seated,
    corigin: OpenVR.TrackedDeviceIndex = 0,
    controller_state_with_pose_device_index: OpenVR.TrackedDeviceIndex = 0,

    button_name_enum: OpenVR.ButtonId = .system,

    controller_axis_type_name_enum: OpenVR.ControllerAxisType = .none,

    perform_fireware_update_device_index: OpenVR.TrackedDeviceIndex = 0,
    perform_firmware_update_result: ?OpenVR.FirmwareError!void = null,

    pub fn init(allocator: std.mem.Allocator) SystemWindow {
        return .{
            .sorted_tracked_device_indices_of_class_tracked_device_indices = std.ArrayList(OpenVR.TrackedDeviceIndex).init(allocator),
        };
    }

    fn deinit(self: SystemWindow) void {
        self.sorted_tracked_device_indices_of_class_tracked_device_indices.deinit();
    }

    fn show(self: *SystemWindow, system: OpenVR.System, allocator: std.mem.Allocator, focus: bool) !void {
        if (focus) {
            zgui.setNextWindowFocus();
        }
        zgui.setNextWindowPos(.{ .x = 100, .y = 0, .cond = .first_use_ever });
        defer zgui.end();
        if (zgui.begin("System", .{ .flags = .{ .always_auto_resize = true } })) {
            try ui.getter(OpenVR.System, "getRecommendedRenderTargetSize", system, .{}, "[width, height]");
            try ui.getter(OpenVR.System, "getProjectionMatrix", system, .{
                .eye = &self.projection_matrix_eye,
                .near = &self.projection_matrix_near,
                .far = &self.projection_matrix_far,
            }, null);
            try ui.getter(OpenVR.System, "getProjectionRaw", system, .{
                .eye = &self.projection_raw_eye,
            }, null);
            try ui.getter(OpenVR.System, "computeDistortion", system, .{
                .eye = &self.compute_distortion_eye,
                .u = &self.compute_distortion_u,
                .v = &self.compute_distortion_v,
            }, null);
            try ui.getter(OpenVR.System, "getEyeToHeadTransform", system, .{
                .eye = &self.eye_to_head_transform_eye,
            }, null);
            try ui.getter(OpenVR.System, "getTimeSinceLastVsync", system, .{}, null);
            try ui.getter(OpenVR.System, "getDXGIOutputInfo", system, .{}, null);
            try ui.getter(OpenVR.System, "isDisplayOnDesktop", system, .{}, null);
            try ui.persistedSetter(OpenVR.System, "setDisplayVisibility", system, .{
                .is_visible_on_desktop = &self.set_display_visibility,
            }, &self.set_display_visibility_result, null);
            try ui.allocGetter(allocator, OpenVR.System, "allocDeviceToAbsoluteTrackingPose", system, .{
                .origin = &self.device_to_absolute_tracking_pose_origin,
                .predicted_seconds_to_photons_from_now = &self.device_to_absolute_tracking_pose_predicted_seconds_to_photons_from_now,
                .count = &self.device_to_absolute_tracking_pose_count,
            }, null);
            try ui.getter(OpenVR.System, "getSeatedZeroPoseToStandingAbsoluteTrackingPose", system, .{}, null);
            try ui.getter(OpenVR.System, "getRawZeroPoseToStandingAbsoluteTrackingPose", system, .{}, null);
            try ui.getter(OpenVR.System, "getSortedTrackedDeviceIndicesOfClass", system, .{
                .tracked_device_class = &self.sorted_tracked_device_indices_of_class_tracked_device_class,
                .tracked_device_indices = &self.sorted_tracked_device_indices_of_class_tracked_device_indices,
                .relative_to_tracked_device_index = &self.sorted_tracked_device_indices_of_class_relative_to_tracked_device_index,
            }, null);
            try ui.getter(OpenVR.System, "getTrackedDeviceActivityLevel", system, .{
                .device_index = &self.tracked_device_activity_level_device_index,
            }, null);
            try ui.getter(OpenVR.System, "applyTransform", system, .{ .tracked_device_pose = &self.apply_transform_tracked_device_pose, .transform = &self.apply_transform }, null);
            try ui.getter(OpenVR.System, "getTrackedDeviceIndexForControllerRole", system, .{
                .device_type = &self.tracked_device_index_for_controller_role_device_type,
            }, null);
            try ui.getter(OpenVR.System, "getControllerRoleForTrackedDeviceIndex", system, .{
                .device_index = &self.controller_role_for_device_index,
            }, null);

            try ui.getter(OpenVR.System, "getTrackedDeviceClass", system, .{
                .device_index = &self.tracked_device_class_device_index,
            }, null);
            try ui.getter(OpenVR.System, "isTrackedDeviceConnected", system, .{
                .device_index = &self.is_tracked_device_connected_device_index,
            }, null);

            try ui.getter(OpenVR.System, "getTrackedDevicePropertyBool", system, .{
                .device_index = &self.tracked_device_property_bool_device_index,
                .property = &self.tracked_device_property_bool,
            }, null);
            try ui.getter(OpenVR.System, "getTrackedDevicePropertyF32", system, .{
                .device_index = &self.tracked_device_property_f32_device_index,
                .property = &self.tracked_device_property_f32,
            }, null);
            try ui.getter(OpenVR.System, "getTrackedDevicePropertyI32", system, .{
                .device_index = &self.tracked_device_property_i32_device_index,
                .property = &self.tracked_device_property_i32,
            }, null);
            try ui.getter(OpenVR.System, "getTrackedDevicePropertyU64", system, .{
                .device_index = &self.tracked_device_property_u64_device_index,
                .property = &self.tracked_device_property_u64,
            }, null);
            try ui.getter(OpenVR.System, "getTrackedDevicePropertyMatrix34", system, .{
                .device_index = &self.tracked_device_property_matrix34_device_index,
                .property = &self.tracked_device_property_matrix34,
            }, null);

            try ui.allocGetter(allocator, OpenVR.System, "allocTrackedDevicePropertyArrayF32", system, .{
                .device_index = &self.tracked_device_property_array_f32_device_index,
                .property = &self.tracked_device_property_array_f32,
            }, null);
            try ui.allocGetter(allocator, OpenVR.System, "allocTrackedDevicePropertyArrayI32", system, .{
                .device_index = &self.tracked_device_property_array_i32_device_index,
                .property = &self.tracked_device_property_array_i32,
            }, null);
            try ui.allocGetter(allocator, OpenVR.System, "allocTrackedDevicePropertyArrayVector4", system, .{
                .device_index = &self.tracked_device_property_array_vector4_device_index,
                .property = &self.tracked_device_property_array_vector4,
            }, null);
            try ui.allocGetter(allocator, OpenVR.System, "allocTrackedDevicePropertyArrayMatrix34", system, .{
                .device_index = &self.tracked_device_property_array_matrix34_device_index,
                .property = &self.tracked_device_property_array_matrix34,
            }, null);

            try ui.allocGetter(allocator, OpenVR.System, "allocTrackedDevicePropertyString", system, .{
                .device_index = &self.tracked_device_property_string_device_index,
                .property = &self.tracked_device_property_string,
            }, null);

            try ui.getter(OpenVR.System, "getPropErrorNameFromEnum", system, .{
                .property_error = &self.prop_error_name_enum,
            }, null);

            try ui.persistedSetter(OpenVR.System, "pollNextEvent", system, .{}, &self.next_event, null);
            try ui.persistedSetter(OpenVR.System, "pollNextEventWithPose", system, .{
                .origin = &self.poll_next_event_with_pose_origin,
            }, &self.next_event_with_pose, null);
            try ui.getter(OpenVR.System, "getEventTypeNameFromEnum", system, .{
                .event_type = &self.event_type_name_enum,
            }, null);

            try ui.getter(OpenVR.System, "getHiddenAreaMesh", system, .{
                .eye = &self.hidden_area_mesh_eye,
                .mesh_type = &self.hidden_area_mesh_type,
            }, null);

            try ui.setter(OpenVR.System, "triggerHapticPulse", system, .{
                .device_index = &self.trigger_haptic_pulse_device_index,
                .axis_id = &self.trigger_haptic_pulse_axis_id,
                .duration_microseconds = &self.trigger_haptic_pulse_duration_microseconds,
            }, null);

            try ui.getter(OpenVR.System, "getControllerState", system, .{
                .device_index = &self.controller_state_device_index,
            }, null);
            try ui.getter(OpenVR.System, "getControllerStateWithPose", system, .{
                .origin = &self.controller_state_with_pose_origin,
                .device_index = &self.controller_state_with_pose_device_index,
            }, null);

            try ui.getter(OpenVR.System, "getButtonIdNameFromEnum", system, .{
                .button_id = &self.button_name_enum,
            }, null);

            try ui.getter(OpenVR.System, "getControllerAxisTypeNameFromEnum", system, .{
                .axis_type = &self.controller_axis_type_name_enum,
            }, null);

            try ui.getter(OpenVR.System, "isInputAvailable", system, .{}, null);
            try ui.getter(OpenVR.System, "isSteamVRDrawingControllers", system, .{}, null);
            try ui.getter(OpenVR.System, "shouldApplicationPause", system, .{}, null);
            try ui.getter(OpenVR.System, "shouldApplicationReduceRenderingWork", system, .{}, null);

            try ui.persistedSetter(OpenVR.System, "performFirmwareUpdate", system, .{
                .device_index = &self.perform_fireware_update_device_index,
            }, &self.perform_firmware_update_result, null);

            try ui.setter(OpenVR.System, "acknowledgeQuitExiting", system, .{}, null);

            try ui.allocGetter(allocator, OpenVR.System, "allocAppContainerFilePaths", system, .{}, null);

            try ui.getter(OpenVR.System, "getRuntimeVersion", system, .{}, null);
        }
    }
};

const ApplicationsWindow = struct {
    add_application_manifest_full_path: [256:0]u8 = std.mem.zeroes([256:0]u8),
    add_application_manifest_temporary: bool = false,
    add_application_manifest_result: ?OpenVR.ApplicationError!void = null,

    remove_application_manifest_full_path: [256:0]u8 = std.mem.zeroes([256:0]u8),
    remove_application_manifest_result: ?OpenVR.ApplicationError!void = null,

    is_application_installed_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),

    application_key_by_index: u32 = 0,
    application_key_by_process_id: u32 = 0,

    launch_application_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    launch_application_result: ?OpenVR.ApplicationError!void = null,

    launch_template_application_template_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    launch_template_application_new_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    launch_template_application_keys: std.ArrayList(OpenVR.AppOverrideKeys),
    launch_template_application_result: ?OpenVR.ApplicationError!void = null,

    launch_application_from_mime_type: [256:0]u8 = std.mem.zeroes([256:0]u8),
    launch_application_from_mime_type_args: [256:0]u8 = std.mem.zeroes([256:0]u8),
    launch_application_from_mime_type_result: ?OpenVR.ApplicationError!void = null,

    launch_dashboard_overlay_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    launch_dashboard_overlay_result: ?OpenVR.ApplicationError!void = null,

    cancel_application_launch_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    cancel_application_launch_result: ?bool = null,

    identify_application_process_id: u32 = 0,
    identify_application_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    identify_application_result: ?OpenVR.ApplicationError!void = null,

    application_process_id_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),

    application_error_name_enum: OpenVR.ApplicationErrorCode = .none,

    application_property_string_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    application_property_string: OpenVR.ApplicationProperty.String = .name,

    application_property_bool_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    application_property_bool: OpenVR.ApplicationProperty.Bool = .is_dashboard_overlay,

    application_property_u64_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    application_property_u64: OpenVR.ApplicationProperty.U64 = .last_launch_time,

    set_application_auto_launch_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    set_application_auto_launch: bool = false,
    set_application_auto_launch_result: ?OpenVR.ApplicationError!void = null,

    application_auto_launch_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),

    set_default_application_for_mime_type_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    set_default_application_for_mime_type: [256:0]u8 = std.mem.zeroes([256:0]u8),
    set_default_application_for_mime_type_result: ?OpenVR.ApplicationError!void = null,

    default_application_for_mime_type: [256:0]u8 = std.mem.zeroes([256:0]u8),

    application_supported_mime_types_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    application_supported_mime_types_buffer_length: u32 = 0,

    applications_that_support_mime_type: [256:0]u8 = std.mem.zeroes([256:0]u8),

    application_launch_arguments_handle: u32 = 0,
    application_launch_arguments_result: ?error{OutOfMemory}![:0]u8 = null,

    perform_application_prelaunch_check_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    perform_application_prelaunch_check_result: ?OpenVR.ApplicationError!void = null,

    scene_application_state_name_enum: OpenVR.SceneApplicationState = .none,

    launch_internal_process_binary_path: [256:0]u8 = std.mem.zeroes([256:0]u8),
    launch_internal_process_arguments: [256:0]u8 = std.mem.zeroes([256:0]u8),
    launch_internal_process_working_directory: [256:0]u8 = std.mem.zeroes([256:0]u8),
    launch_internal_process_result: ?OpenVR.ApplicationError!void = null,

    pub fn init(allocator: std.mem.Allocator) ApplicationsWindow {
        return .{
            .launch_template_application_keys = std.ArrayList(OpenVR.AppOverrideKeys).init(allocator),
        };
    }

    pub fn deinit(self: ApplicationsWindow, allocator: std.mem.Allocator) void {
        for (self.launch_template_application_keys.items) |*key| {
            allocator.free(key.key);
            allocator.free(key.value);
        }
        self.launch_template_application_keys.deinit();
        if (self.application_launch_arguments_result) |result| {
            if (result) |args| {
                allocator.free(args);
            } else |_| {}
        }
    }

    fn show(self: *ApplicationsWindow, applications: OpenVR.Applications, allocator: std.mem.Allocator, focus: bool) !void {
        if (focus) {
            zgui.setNextWindowFocus();
        }
        zgui.setNextWindowPos(.{ .x = 100, .y = 0, .cond = .first_use_ever });
        defer zgui.end();
        if (zgui.begin("Applications", .{ .flags = .{ .always_auto_resize = true } })) {
            try ui.persistedSetter(OpenVR.Applications, "addApplicationManifest", applications, .{
                .application_manifest_full_path = &self.add_application_manifest_full_path,
                .temporary = &self.add_application_manifest_temporary,
            }, &self.add_application_manifest_result, null);
            try ui.persistedSetter(OpenVR.Applications, "removeApplicationManifest", applications, .{
                .application_manifest_full_path = &self.remove_application_manifest_full_path,
            }, &self.remove_application_manifest_result, null);
            try ui.getter(OpenVR.Applications, "isApplicationInstalled", applications, .{
                .app_key = &self.is_application_installed_app_key,
            }, null);
            try ui.getter(OpenVR.Applications, "getApplicationCount", applications, .{}, null);
            try ui.allocGetter(allocator, OpenVR.Applications, "allocApplicationKeyByIndex", applications, .{
                .application_index = &self.application_key_by_index,
            }, null);
            try ui.allocGetter(allocator, OpenVR.Applications, "allocApplicationKeyByProcessId", applications, .{
                .process_id = &self.application_key_by_process_id,
            }, null);
            try ui.persistedSetter(OpenVR.Applications, "launchApplication", applications, .{
                .app_key = &self.launch_application_app_key,
            }, &self.launch_application_result, null);
            try ui.allocPersistedSetter(allocator, OpenVR.Applications, "allocLaunchTemplateApplication", applications, .{
                .template_app_key = &self.launch_template_application_template_app_key,
                .new_app_key = &self.launch_template_application_new_app_key,
                .keys = &self.launch_template_application_keys,
            }, &self.launch_template_application_result, null);
            try ui.persistedSetter(OpenVR.Applications, "launchApplicationFromMimeType", applications, .{
                .mime_type = &self.launch_application_from_mime_type,
                .args = &self.launch_application_from_mime_type_args,
            }, &self.launch_application_from_mime_type_result, null);
            try ui.persistedSetter(OpenVR.Applications, "launchDashboardOverlay", applications, .{
                .app_key = &self.launch_dashboard_overlay_app_key,
            }, &self.launch_dashboard_overlay_result, null);
            try ui.persistedSetter(OpenVR.Applications, "cancelApplicationLaunch", applications, .{
                .app_key = &self.cancel_application_launch_app_key,
            }, &self.cancel_application_launch_result, null);
            try ui.persistedSetter(OpenVR.Applications, "identifyApplication", applications, .{
                .process_id = &self.identify_application_process_id,
                .app_key = &self.identify_application_app_key,
            }, &self.identify_application_result, null);
            try ui.getter(OpenVR.Applications, "getApplicationProcessId", applications, .{
                .app_key = &self.application_process_id_app_key,
            }, null);
            try ui.getter(OpenVR.Applications, "getApplicationsErrorNameFromEnum", applications, .{
                .error_code = &self.application_error_name_enum,
            }, null);
            try ui.allocGetter(allocator, OpenVR.Applications, "allocApplicationPropertyString", applications, .{
                .app_key = &self.application_property_string_app_key,
                .property = &self.application_property_string,
            }, null);
            try ui.getter(OpenVR.Applications, "getApplicationPropertyBool", applications, .{
                .app_key = &self.application_property_bool_app_key,
                .property = &self.application_property_bool,
            }, null);
            try ui.getter(OpenVR.Applications, "getApplicationPropertyU64", applications, .{
                .app_key = &self.application_property_u64_app_key,
                .property = &self.application_property_u64,
            }, null);
            try ui.persistedSetter(OpenVR.Applications, "setApplicationAutoLaunch", applications, .{
                .app_key = &self.set_application_auto_launch_app_key,
                .auto_launch = &self.set_application_auto_launch,
            }, &self.set_application_auto_launch_result, null);
            try ui.getter(OpenVR.Applications, "getApplicationAutoLaunch", applications, .{
                .app_key = &self.application_auto_launch_app_key,
            }, null);
            try ui.persistedSetter(OpenVR.Applications, "setDefaultApplicationForMimeType", applications, .{
                .app_key = &self.set_default_application_for_mime_type_app_key,
                .mime_type = &self.set_default_application_for_mime_type,
            }, &self.set_default_application_for_mime_type_result, null);
            try ui.allocGetter(allocator, OpenVR.Applications, "allocDefaultApplicationForMimeType", applications, .{
                .mime_type = &self.default_application_for_mime_type,
            }, null);
            try ui.allocGetter(allocator, OpenVR.Applications, "allocApplicationSupportedMimeTypes", applications, .{
                .app_key = &self.application_supported_mime_types_app_key,
                .buffer_length = &self.application_supported_mime_types_buffer_length,
            }, null);

            // don't know why this segfaults
            // try ui.allocGetter(allocator, OpenVR.Applications, "allocApplicationsThatSupportMimeType", applications, .{
            //     .mime_type = &self.applications_that_support_mime_type,
            // }, null);

            try ui.allocGetter(allocator, OpenVR.Applications, "allocApplicationLaunchArguments", applications, .{
                .handle = &self.application_launch_arguments_handle,
            }, null);
            try ui.allocGetter(allocator, OpenVR.Applications, "allocStartingApplication", applications, .{}, null);
            try ui.getter(OpenVR.Applications, "getSceneApplicationState", applications, .{}, null);
            try ui.persistedSetter(OpenVR.Applications, "performApplicationPrelaunchCheck", applications, .{
                .app_key = &self.perform_application_prelaunch_check_app_key,
            }, &self.perform_application_prelaunch_check_result, null);
            try ui.getter(OpenVR.Applications, "getSceneApplicationStateNameFromEnum", applications, .{
                .state = &self.scene_application_state_name_enum,
            }, null);
            try ui.persistedSetter(OpenVR.Applications, "launchInternalProcess", applications, .{
                .binary_path = &self.launch_internal_process_binary_path,
                .arguments = &self.launch_internal_process_arguments,
                .working_directory = &self.launch_internal_process_working_directory,
            }, &self.launch_internal_process_result, null);
            try ui.getter(OpenVR.Applications, "getCurrentSceneProcessId", applications, .{}, null);
        }
    }
};

const ChaperoneWindow = struct {
    scene_color: OpenVR.Color = .{ .r = 0, .b = 0, .g = 0, .a = 1 },
    bound_colors_count: usize = 1,
    collision_bounds_fade_distance: f32 = 0,
    force_bounds_visible: bool = false,
    reset_zero_pose_origin: OpenVR.TrackingUniverseOrigin = .seated,

    fn show(self: *ChaperoneWindow, chaperone: OpenVR.Chaperone, allocator: std.mem.Allocator, focus: bool) !void {
        if (focus) {
            zgui.setNextWindowFocus();
        }
        zgui.setNextWindowPos(.{ .x = 100, .y = 0, .cond = .first_use_ever });
        defer zgui.end();
        if (zgui.begin("Chaperone", .{ .flags = .{ .always_auto_resize = true } })) {
            try ui.getter(OpenVR.Chaperone, "getCalibrationState", chaperone, .{}, null);
            try ui.getter(OpenVR.Chaperone, "getPlayAreaSize", chaperone, .{}, "{x: meters, z: meters}");
            try ui.getter(OpenVR.Chaperone, "getPlayAreaRect", chaperone, .{}, "{corners: [4][x meters, y meters, z meters]}");

            try ui.setter(OpenVR.Chaperone, "reloadInfo", chaperone, .{}, null);
            try ui.setter(OpenVR.Chaperone, "setSceneColor", chaperone, .{ .scene_color = &self.scene_color }, null);
            try ui.allocGetter(allocator, OpenVR.Chaperone, "allocBoundsColor", chaperone, .{
                .collision_bounds_fade_distance = &self.collision_bounds_fade_distance,
                .bound_colors_count = &self.bound_colors_count,
            }, null);
            try ui.getter(OpenVR.Chaperone, "areBoundsVisible", chaperone, .{}, null);
            try ui.setter(OpenVR.Chaperone, "forceBoundsVisible", chaperone, .{ .force = &self.force_bounds_visible }, null);
            try ui.setter(OpenVR.Chaperone, "resetZeroPose", chaperone, .{ .origin = &self.reset_zero_pose_origin }, null);
        }
    }
};

const CompositorWindow = struct {
    wait_render_poses_count: usize = 1,
    wait_game_poses_count: usize = 1,
    waited_poses: ?(OpenVR.CompositorError || error{OutOfMemory})!OpenVR.CompositorPoses = null,

    last_render_poses_count: usize = 1,
    last_game_poses_count: usize = 1,
    last_pose_device_index: u32 = 0,
    frame_timing_frames_ago: u32 = 0,
    frame_timings_count: u32 = 0,
    fade_color_seconds: f32 = 0,
    fade_color_background: bool = false,
    fade_color: OpenVR.Color = .{ .r = 0, .g = 0, .b = 0, .a = 1 },
    current_fade_color_background: bool = false,
    fade_grid_seconds: f32 = 0,
    fade_grid_background: bool = false,
    tracking_space_origin: OpenVR.TrackingUniverseOrigin = .seated,
    force_interleaved_reprojection_override_on: bool = false,
    suspend_rendering: bool = false,
    explicit_timing_mode: OpenVR.TimingMode = .implicit,
    submit_explicit_timing_data_result: ?OpenVR.CompositorError!void = null,

    stage_override_async_render_model_path: [256:0]u8 = std.mem.zeroes([256:0]u8),
    stage_override_async_transform: OpenVR.Matrix34 = std.mem.zeroes(OpenVR.Matrix34),
    stage_override_async_stage_render_settings: OpenVR.StageRenderSettings = .{
        .primary_color = std.mem.zeroes(OpenVR.Color),
        .secondary_color = std.mem.zeroes(OpenVR.Color),
        .vignette_inner_radius = 0,
        .vignette_outer_radius = 0,
        .fresnel_strength = 0,
        .backface_culling = false,
        .greyscale = false,
        .wireframe = false,
    },
    stage_override_async_result: ?OpenVR.CompositorError!void = null,

    poses_for_frame_pose_prediction_id: u32 = 0,
    poses_for_frame_pose_count: u32 = 0,

    pub fn deinit(self: CompositorWindow, allocator: std.mem.Allocator) void {
        if (self.waited_poses) |poses| {
            if (poses) |p| {
                p.deinit(allocator);
            } else |_| {}
        }
    }

    fn show(self: *CompositorWindow, compositor: OpenVR.Compositor, allocator: std.mem.Allocator, focus: bool) !void {
        if (focus) {
            zgui.setNextWindowFocus();
        }
        zgui.setNextWindowPos(.{ .x = 100, .y = 0, .cond = .first_use_ever });
        defer zgui.end();
        if (zgui.begin("Compositor", .{ .flags = .{ .always_auto_resize = true } })) {
            try ui.getter(OpenVR.Compositor, "getTrackingSpace", compositor, .{}, null);
            try ui.setter(OpenVR.Compositor, "setTrackingSpace", compositor, .{ .origin = &self.tracking_space_origin }, null);
            ui.allocPersistedSetter(
                allocator,
                OpenVR.Compositor,
                "allocWaitPoses",
                compositor,
                .{
                    .render_poses_count = &self.wait_render_poses_count,
                    .game_poses_count = &self.wait_game_poses_count,
                },
                &self.waited_poses,
                null,
            ) catch |err| switch (err) {
                error.DoNotHaveFocus => {
                    zgui.indent(.{ .indent_w = 30 });
                    defer zgui.unindent(.{ .indent_w = 30 });
                    zgui.text("{!}", .{err});
                    zgui.newLine();
                },
                else => return err,
            };
            try ui.allocGetter(allocator, OpenVR.Compositor, "allocLastPoses", compositor, .{
                .render_poses_count = &self.last_render_poses_count,
                .game_poses_count = &self.last_game_poses_count,
            }, null);
            try ui.getter(OpenVR.Compositor, "getLastPoseForTrackedDeviceIndex", compositor, .{
                .device_index = &self.last_pose_device_index,
            }, null);

            // try ui.setter(OpenVR.Compositor, "submit", compositor, ...
            // try ui.setter(OpenVR.Compositor, "submitWithArrayIndex", compositor, ...
            try ui.setter(OpenVR.Compositor, "clearLastSubmittedFrame", compositor, .{}, null);
            try ui.setter(OpenVR.Compositor, "postPresentHandoff", compositor, .{}, null);

            try ui.getter(OpenVR.Compositor, "getFrameTiming", compositor, .{
                .frames_ago = &self.frame_timing_frames_ago,
            }, null);
            try ui.allocGetter(allocator, OpenVR.Compositor, "allocFrameTimings", compositor, .{
                .count = &self.frame_timings_count,
            }, null);
            try ui.getter(OpenVR.Compositor, "getFrameTimeRemaining", compositor, .{}, null);
            try ui.getter(OpenVR.Compositor, "getCumulativeStats", compositor, .{}, null);
            try ui.setter(OpenVR.Compositor, "fadeToColor", compositor, .{
                .seconds = &self.fade_color_seconds,
                .color = &self.fade_color,
                .background = &self.fade_color_background,
            }, null);
            try ui.getter(OpenVR.Compositor, "getCurrentFadeColor", compositor, .{ .background = &self.current_fade_color_background }, null);
            try ui.setter(OpenVR.Compositor, "fadeGrid", compositor, .{
                .seconds = &self.fade_grid_seconds,
                .background = &self.fade_grid_background,
            }, null);
            try ui.getter(OpenVR.Compositor, "getCurrentGridAlpha", compositor, .{}, null);

            // try ui.setter(OpenVR.Compositor, "setSkyboxOverride", compositor, ...
            try ui.setter(OpenVR.Compositor, "clearSkyboxOverride", compositor, .{}, null);

            try ui.setter(OpenVR.Compositor, "compositorBringToFront", compositor, .{}, null);
            try ui.setter(OpenVR.Compositor, "compositorGoToBack", compositor, .{}, null);
            try ui.setter(OpenVR.Compositor, "compositorQuit", compositor, .{}, null);

            try ui.getter(OpenVR.Compositor, "isFullscreen", compositor, .{}, null);
            try ui.getter(OpenVR.Compositor, "getCurrentSceneFocusProcess", compositor, .{}, null);
            try ui.getter(OpenVR.Compositor, "getLastFrameRenderer", compositor, .{}, null);
            try ui.getter(OpenVR.Compositor, "canRenderScene", compositor, .{}, null);

            try ui.setter(OpenVR.Compositor, "showMirrorWindow", compositor, .{}, null);
            try ui.setter(OpenVR.Compositor, "hideMirrorWindow", compositor, .{}, null);
            try ui.getter(OpenVR.Compositor, "isMirrorWindowVisible", compositor, .{}, null);

            try ui.setter(OpenVR.Compositor, "compositorDumpImages", compositor, .{}, null);

            try ui.getter(OpenVR.Compositor, "shouldAppRenderWithLowResources", compositor, .{}, null);

            try ui.setter(OpenVR.Compositor, "forceInterleavedReprojectionOn", compositor, .{
                .override = &self.force_interleaved_reprojection_override_on,
            }, null);
            try ui.setter(OpenVR.Compositor, "forceReconnectProcess", compositor, .{}, null);
            try ui.setter(OpenVR.Compositor, "suspendRendering", compositor, .{
                .suspend_rendering = &self.suspend_rendering,
            }, null);

            try ui.setter(OpenVR.Compositor, "setExplicitTimingMode", compositor, .{
                .explicit_timing_mode = &self.explicit_timing_mode,
            }, null);
            try ui.persistedSetter(OpenVR.Compositor, "submitExplicitTimingData", compositor, .{}, &self.submit_explicit_timing_data_result, null);

            try ui.getter(OpenVR.Compositor, "isMotionSmoothingEnabled", compositor, .{}, null);
            try ui.getter(OpenVR.Compositor, "isMotionSmoothingSupported", compositor, .{}, null);
            try ui.getter(OpenVR.Compositor, "isCurrentSceneFocusAppLoading", compositor, .{}, null);

            try ui.persistedSetter(OpenVR.Compositor, "setStageOverrideAsync", compositor, .{
                .render_model_path = &self.stage_override_async_render_model_path,
                .transform = &self.stage_override_async_transform,
                .stage_render_settings = &self.stage_override_async_stage_render_settings,
            }, &self.stage_override_async_result, null);
            try ui.setter(OpenVR.Compositor, "clearStageOverride", compositor, .{}, null);
            try ui.getter(OpenVR.Compositor, "getCompositorBenchmarkResults", compositor, .{}, null);
            try ui.getter(OpenVR.Compositor, "getLastPosePredictionIDs", compositor, .{}, null);
            try ui.allocGetter(allocator, OpenVR.Compositor, "allocPosesForFrame", compositor, .{
                .pose_prediction_id = &self.poses_for_frame_pose_prediction_id,
                .pose_count = &self.poses_for_frame_pose_count,
            }, null);
        }
    }
};

const InputWindow = struct {
    set_action_manifest_path: [256:0]u8 = std.mem.zeroes([256:0]u8),
    set_action_manifest_path_result: ?OpenVR.InputError!void = null,

    action_set_handle_action_set_name: [256:0]u8 = std.mem.zeroes([256:0]u8),
    action_handle_action_name: [256:0]u8 = std.mem.zeroes([256:0]u8),
    input_source_handle_input_source_path: [256:0]u8 = std.mem.zeroes([256:0]u8),

    update_action_state_sets: std.ArrayList(OpenVR.ActiveActionSet),
    update_action_state_result: ?OpenVR.InputError!void = null,

    digital_action_data_action: OpenVR.ActionHandle = 0,
    digital_action_data_restrict_to_device: OpenVR.InputValueHandle = 0,
    analog_action_data_action: OpenVR.ActionHandle = 0,
    analog_action_data_restrict_to_device: OpenVR.InputValueHandle = 0,
    pose_action_data_relative_to_now_action: OpenVR.ActionHandle = 0,
    pose_action_data_relative_to_now_origin: OpenVR.TrackingUniverseOrigin = .seated,
    pose_action_data_relative_to_now_predicted_seconds_from_now: f32 = 0,
    pose_action_data_relative_to_now_restrict_to_device: OpenVR.InputValueHandle = 0,
    pose_action_data_for_next_frame_action: OpenVR.ActionHandle = 0,
    pose_action_data_for_next_frame_origin: OpenVR.TrackingUniverseOrigin = .seated,
    pose_action_data_for_next_frame_restrict_to_device: OpenVR.InputValueHandle = 0,
    skeletal_action_data_action: OpenVR.ActionHandle = 0,
    set_dominant_hand: OpenVR.TrackedControllerRole = .invalid,
    set_dominant_hand_result: ?OpenVR.InputError!void = null,
    bone_count_action: OpenVR.ActionHandle = 0,

    bone_name_action: OpenVR.ActionHandle = 0,
    bone_name_bone_index: OpenVR.BoneIndex = 0,

    skeletal_reference_transforms_action: OpenVR.ActionHandle = 0,
    skeletal_reference_transforms_transform_space: OpenVR.SkeletalTransformSpace = .model,
    skeletal_reference_transforms_reference_pose: OpenVR.SkeletalReferencePose = .bind_pose,
    skeletal_reference_transforms_transform_count: usize = 0,
    skeletal_tracking_level_action: OpenVR.ActionHandle = 0,
    skeletal_bone_data_action: OpenVR.ActionHandle = 0,
    skeletal_bone_data_transform_space: OpenVR.SkeletalTransformSpace = .model,
    skeletal_bone_data_motion_range: OpenVR.SkeletalMotionRange = .with_controller,
    skeletal_bone_data_transform_count: usize = 0,
    skeletal_summary_data_action: OpenVR.ActionHandle = 0,
    skeletal_summary_data_summary_type: OpenVR.SummaryType = .from_animation,
    skeletal_bone_data_compressed_action: OpenVR.ActionHandle = 0,
    skeletal_bone_data_compressed_motion_range: OpenVR.SkeletalMotionRange = .with_controller,

    trigger_haptic_vibration_action: OpenVR.ActionHandle = 0,
    trigger_haptic_vibration_action_start_seconds_from_now: f32 = 0,
    trigger_haptic_vibration_action_duration_seconds: f32 = 0,
    trigger_haptic_vibration_action_frequency: f32 = 0,
    trigger_haptic_vibration_action_amplitude: f32 = 0,
    trigger_haptic_vibration_action_restrict_to_device: OpenVR.InputValueHandle = 0,
    trigger_haptic_vibration_action_result: ?OpenVR.InputError!void = null,

    action_origins_action_set_handle: OpenVR.ActionSetHandle = 0,
    action_origins_digital_action_handle: OpenVR.ActionHandle = 0,
    action_origins_count: u32 = 0,
    origin_localized_name_origin: OpenVR.InputValueHandle = 0,
    origin_localized_name_string_section_to_include: i32 = 0,
    origin_localized_name_buffer_length: u32 = 0,
    origin_tracked_device_info_origin: OpenVR.InputValueHandle = 0,
    action_binding_info_action: OpenVR.ActionHandle = 0,
    action_binding_info_count: u32 = 0,
    show_action_origins_action_set_handle: OpenVR.ActionSetHandle = 0,
    show_action_origins_result: ?OpenVR.InputError!void = null,

    show_action_origins_action_handle: OpenVR.ActionHandle = 0,
    show_bindings_for_action_set_sets: std.ArrayList(OpenVR.ActiveActionSet),
    show_bindings_for_action_set_origin_to_highlight: OpenVR.InputValueHandle = 0,
    show_bindings_for_action_set_result: ?OpenVR.InputError!void = null,

    component_state_for_binding_render_model_name: [256:0]u8 = std.mem.zeroes([256:0]u8),
    component_state_for_binding_component_name: [256:0]u8 = std.mem.zeroes([256:0]u8),
    component_state_for_binding_origin_info: std.ArrayList(OpenVR.InputBindingInfo),
    open_binding_ui_app_key: [OpenVR.max_application_key_length:0]u8 = std.mem.zeroes([OpenVR.max_application_key_length:0]u8),
    open_binding_ui_action_set_handle: OpenVR.ActionSetHandle = 0,
    open_binding_ui_device_handle: OpenVR.InputValueHandle = 0,
    open_binding_ui_show_on_desktop: bool = false,
    open_binding_ui_result: ?OpenVR.InputError!void = null,

    binding_variant_device_path: OpenVR.InputValueHandle = 0,
    binding_variant_buffer_length: u32 = 0,

    pub fn init(allocator: std.mem.Allocator) InputWindow {
        return .{
            .update_action_state_sets = std.ArrayList(OpenVR.ActiveActionSet).init(allocator),
            .show_bindings_for_action_set_sets = std.ArrayList(OpenVR.ActiveActionSet).init(allocator),
            .component_state_for_binding_origin_info = std.ArrayList(OpenVR.InputBindingInfo).init(allocator),
        };
    }

    fn deinit(self: InputWindow) void {
        self.component_state_for_binding_origin_info.deinit();
        self.show_bindings_for_action_set_sets.deinit();
        self.update_action_state_sets.deinit();
    }

    fn show(self: *InputWindow, input: OpenVR.Input, allocator: std.mem.Allocator, focus: bool) !void {
        if (focus) {
            zgui.setNextWindowFocus();
        }
        zgui.setNextWindowPos(.{ .x = 100, .y = 0, .cond = .first_use_ever });
        defer zgui.end();
        if (zgui.begin("Input", .{ .flags = .{ .always_auto_resize = true } })) {
            try ui.persistedSetter(OpenVR.Input, "setActionManifestPath", input, .{
                .action_manifest_path = &self.set_action_manifest_path,
            }, &self.set_action_manifest_path_result, null);
            try ui.getter(OpenVR.Input, "getActionSetHandle", input, .{
                .action_set_name = &self.action_set_handle_action_set_name,
            }, null);
            try ui.getter(OpenVR.Input, "getActionHandle", input, .{
                .action_name = &self.action_handle_action_name,
            }, null);
            try ui.getter(OpenVR.Input, "getInputSourceHandle", input, .{
                .input_source_path = &self.input_source_handle_input_source_path,
            }, null);
            try ui.persistedSetter(OpenVR.Input, "updateActionState", input, .{
                .sets = &self.update_action_state_sets,
            }, &self.update_action_state_result, null);
            try ui.getter(OpenVR.Input, "getDigitalActionData", input, .{
                .action = &self.digital_action_data_action,
                .restrict_to_device = &self.digital_action_data_restrict_to_device,
            }, null);
            try ui.getter(OpenVR.Input, "getAnalogActionData", input, .{
                .action = &self.analog_action_data_action,
                .restrict_to_device = &self.analog_action_data_restrict_to_device,
            }, null);
            try ui.getter(OpenVR.Input, "getPoseActionDataRelativeToNow", input, .{
                .action = &self.pose_action_data_relative_to_now_action,
                .origin = &self.pose_action_data_relative_to_now_origin,
                .predicted_seconds_from_now = &self.pose_action_data_relative_to_now_predicted_seconds_from_now,
                .restrict_to_device = &self.pose_action_data_relative_to_now_restrict_to_device,
            }, null);
            try ui.getter(OpenVR.Input, "getPoseActionDataForNextFrame", input, .{
                .action = &self.pose_action_data_for_next_frame_action,
                .origin = &self.pose_action_data_for_next_frame_origin,
                .restrict_to_device = &self.pose_action_data_for_next_frame_restrict_to_device,
            }, null);
            try ui.getter(OpenVR.Input, "getSkeletalActionData", input, .{
                .action = &self.skeletal_action_data_action,
            }, null);
            try ui.getter(OpenVR.Input, "getDominantHand", input, .{}, null);
            try ui.persistedSetter(OpenVR.Input, "setDominantHand", input, .{
                .dominant_hand = &self.set_dominant_hand,
            }, &self.set_dominant_hand_result, null);
            try ui.getter(OpenVR.Input, "getBoneCount", input, .{
                .action = &self.bone_count_action,
            }, null);
            try ui.allocGetter(allocator, OpenVR.Input, "allocBoneName", input, .{
                .action = &self.bone_name_action,
                .bond_index = &self.bone_name_bone_index,
            }, null);
            try ui.allocGetter(allocator, OpenVR.Input, "allocSkeletalReferenceTransforms", input, .{
                .action = &self.skeletal_reference_transforms_action,
                .transform_space = &self.skeletal_reference_transforms_transform_space,
                .reference_pose = &self.skeletal_reference_transforms_reference_pose,
                .transform_count = &self.skeletal_reference_transforms_transform_count,
            }, null);
            try ui.getter(OpenVR.Input, "getSkeletalTrackingLevel", input, .{
                .action = &self.skeletal_tracking_level_action,
            }, null);
            try ui.allocGetter(allocator, OpenVR.Input, "allocSkeletalBoneData", input, .{
                .action = &self.skeletal_bone_data_action,
                .transform_space = &self.skeletal_bone_data_transform_space,
                .motion_range = &self.skeletal_bone_data_motion_range,
                .transform_count = &self.skeletal_bone_data_transform_count,
            }, null);
            try ui.getter(OpenVR.Input, "getSkeletalSummaryData", input, .{
                .action = &self.skeletal_summary_data_action,
                .summary_type = &self.skeletal_summary_data_summary_type,
            }, null);
            try ui.persistedSetter(OpenVR.Input, "triggerHapticVibrationAction", input, .{
                .action = &self.trigger_haptic_vibration_action,
                .start_seconds_from_now = &self.trigger_haptic_vibration_action_start_seconds_from_now,
                .duration_seconds = &self.trigger_haptic_vibration_action_duration_seconds,
                .frequency = &self.trigger_haptic_vibration_action_frequency,
                .amplitude = &self.trigger_haptic_vibration_action_amplitude,
                .restrict_to_device = &self.trigger_haptic_vibration_action_restrict_to_device,
            }, &self.trigger_haptic_vibration_action_result, null);
            try ui.allocGetter(allocator, OpenVR.Input, "allocActionOrigins", input, .{
                .action_set_handle = &self.action_origins_action_set_handle,
                .digital_action_handle = &self.action_origins_digital_action_handle,
                .count = &self.action_origins_count,
            }, null);
            try ui.allocGetter(allocator, OpenVR.Input, "allocOriginLocalizedName", input, .{
                .origin = &self.origin_localized_name_origin,
                .string_section_to_include = &self.origin_localized_name_string_section_to_include,
                .buffer_length = &self.origin_localized_name_buffer_length,
            }, null);
            try ui.getter(OpenVR.Input, "getOriginTrackedDeviceInfo", input, .{
                .origin = &self.origin_tracked_device_info_origin,
            }, null);
            try ui.allocGetter(allocator, OpenVR.Input, "allocActionBindingInfo", input, .{
                .action = &self.action_binding_info_action,
                .count = &self.action_binding_info_count,
            }, null);
            try ui.persistedSetter(OpenVR.Input, "showActionOrigins", input, .{
                .action_set_handle = &self.show_action_origins_action_set_handle,
                .action_handle = &self.show_action_origins_action_handle,
            }, &self.show_action_origins_result, null);
            try ui.persistedSetter(OpenVR.Input, "showBindingsForActionSet", input, .{
                .sets = &self.show_bindings_for_action_set_sets,
                .origin_to_highlight = &self.show_bindings_for_action_set_origin_to_highlight,
            }, &self.show_bindings_for_action_set_result, null);
            try ui.getter(OpenVR.Input, "getComponentStateForBinding", input, .{
                .render_model_name = &self.component_state_for_binding_render_model_name,
                .component_name = &self.component_state_for_binding_component_name,
                .origin_info = &self.component_state_for_binding_origin_info,
            }, null);
            try ui.getter(OpenVR.Input, "isUsingLegacyInput", input, .{}, null);
            try ui.persistedSetter(OpenVR.Input, "openBindingUI", input, .{
                .app_key = &self.open_binding_ui_app_key,
                .action_set_handle = &self.open_binding_ui_action_set_handle,
                .device_handle = &self.open_binding_ui_device_handle,
                .show_on_desktop = &self.open_binding_ui_show_on_desktop,
            }, &self.open_binding_ui_result, null);
            try ui.allocGetter(allocator, OpenVR.Input, "allocBindingVariant", input, .{
                .device_path = &self.binding_variant_device_path,
                .buffer_length = &self.binding_variant_buffer_length,
            }, null);
        }
    }
};

const RenderModelsWindow = struct {
    load_render_model_async_render_model_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    load_render_model_async_result: ?OpenVR.RenderModelError!OpenVR.RenderModel = null,
    load_texture_async_texture_id: OpenVR.TextureID = 0,
    load_texture_async_result: ?OpenVR.RenderModelError!*OpenVR.RenderModel.TextureMap = null,
    render_model_name_render_model_index: u32 = 0,
    component_count_render_model_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    component_name_render_model_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    component_name_component_index: u32 = 0,
    component_button_mask_render_model_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    component_button_mask_component_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    component_render_model_name_render_model_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    component_render_model_name_component_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    component_state_for_device_path_render_model_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    component_state_for_device_path_component_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    component_state_for_device_path: OpenVR.InputValueHandle = 0,
    component_state_for_device_path_state: OpenVR.RenderModel.ControllerModeState = .{
        .scroll_wheel_visible = false,
    },
    render_model_has_component_render_model_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    render_model_has_component_component_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    render_model_thumbnail_url_render_model_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    render_model_original_path_render_model_name: [255:0]u8 = std.mem.zeroes([255:0]u8),
    render_model_error_name_from_enum: OpenVR.RenderModelErrorCode = .none,

    pub fn init() RenderModelsWindow {
        return .{};
    }

    fn deinit(self: RenderModelsWindow, render_models: OpenVR.RenderModels) void {
        if (self.load_render_model_async_result) |result| {
            if (result) |model| {
                render_models.freeRenderModel(model);
            } else |_| {}
        }
        if (self.load_texture_async_result) |result| {
            if (result) |texture| {
                render_models.freeTexture(texture);
            } else |_| {}
        }
    }

    fn show(self: *RenderModelsWindow, render_models: OpenVR.RenderModels, allocator: std.mem.Allocator, focus: bool) !void {
        if (focus) {
            zgui.setNextWindowFocus();
        }
        zgui.setNextWindowPos(.{ .x = 100, .y = 0, .cond = .first_use_ever });
        defer zgui.end();
        if (zgui.begin("Render models", .{ .flags = .{ .always_auto_resize = true } })) {
            try ui.persistedSetter(OpenVR.RenderModels, "loadRenderModelAsync", render_models, .{
                .render_model_name = &self.load_render_model_async_render_model_name,
            }, &self.load_render_model_async_result, null);
            try ui.deinitSetter(OpenVR.RenderModels, "freeRenderModel", render_models, .{
                .render_model = &self.load_render_model_async_result,
            }, &self.load_render_model_async_result, null);
            try ui.persistedSetter(OpenVR.RenderModels, "loadTextureAsync", render_models, .{
                .texture_id = &self.load_texture_async_texture_id,
            }, &self.load_texture_async_result, null);
            try ui.deinitSetter(OpenVR.RenderModels, "freeTexture", render_models, .{
                .texture = &self.load_texture_async_result,
            }, &self.load_texture_async_result, null);
            try ui.allocGetter(allocator, OpenVR.RenderModels, "allocRenderModelName", render_models, .{
                .render_model_index = &self.render_model_name_render_model_index,
            }, null);
            try ui.getter(OpenVR.RenderModels, "getRenderModelCount", render_models, .{}, null);
            try ui.getter(OpenVR.RenderModels, "getComponentCount", render_models, .{
                .render_model_name = &self.component_count_render_model_name,
            }, null);
            try ui.allocGetter(allocator, OpenVR.RenderModels, "allocComponentName", render_models, .{
                .render_model_name = &self.component_name_render_model_name,
                .component_index = &self.component_name_component_index,
            }, null);
            try ui.getter(OpenVR.RenderModels, "getComponentButtonMask", render_models, .{
                .render_model_name = &self.component_button_mask_render_model_name,
                .component_name = &self.component_button_mask_component_name,
            }, null);
            try ui.allocGetter(allocator, OpenVR.RenderModels, "allocComponentRenderModelName", render_models, .{
                .render_model_name = &self.component_render_model_name_render_model_name,
                .component_name = &self.component_render_model_name_component_name,
            }, null);
            try ui.getter(OpenVR.RenderModels, "getComponentStateForDevicePath", render_models, .{
                .render_model_name = &self.component_state_for_device_path_render_model_name,
                .component_name = &self.component_state_for_device_path_component_name,
                .device_path = &self.component_state_for_device_path,
                .state = &self.component_state_for_device_path_state,
            }, null);
            try ui.getter(OpenVR.RenderModels, "renderModelHasComponent", render_models, .{
                .render_model_name = &self.render_model_has_component_render_model_name,
                .component_name = &self.render_model_has_component_component_name,
            }, null);
            try ui.allocGetter(allocator, OpenVR.RenderModels, "allocRenderModelThumbnailURL", render_models, .{
                .render_model_name = &self.render_model_thumbnail_url_render_model_name,
            }, null);
            try ui.allocGetter(allocator, OpenVR.RenderModels, "allocRenderModelOriginalPath", render_models, .{
                .render_model_name = &self.render_model_original_path_render_model_name,
            }, null);
            try ui.getter(OpenVR.RenderModels, "getRenderModelErrorNameFromEnum", render_models, .{
                .error_code = &self.render_model_error_name_from_enum,
            }, null);
        }
    }
};

const OpenVRWindow = struct {
    openvr: ?OpenVR.InitError!OpenVR = null,

    system: ?OpenVR.InitError!OpenVR.System = null,
    system_window: SystemWindow,

    chaperone: ?OpenVR.InitError!OpenVR.Chaperone = null,
    chaperone_window: ChaperoneWindow = .{},

    compositor: ?OpenVR.InitError!OpenVR.Compositor = null,
    compositor_window: CompositorWindow = .{},

    applications: ?OpenVR.InitError!OpenVR.Applications = null,
    applications_window: ApplicationsWindow,

    input: ?OpenVR.InitError!OpenVR.Input = null,
    input_window: InputWindow,

    render_models: ?OpenVR.InitError!OpenVR.RenderModels = null,
    render_models_window: RenderModelsWindow,

    symbol_static_error: OpenVR.InitErrorCode = .none,
    english_static_error: OpenVR.InitErrorCode = .none,

    pub fn init(allocator: std.mem.Allocator) OpenVRWindow {
        return .{
            .system_window = SystemWindow.init(allocator),
            .applications_window = ApplicationsWindow.init(allocator),
            .input_window = InputWindow.init(allocator),
            .render_models_window = RenderModelsWindow.init(),
        };
    }

    pub fn deinit(self: *OpenVRWindow, allocator: std.mem.Allocator) void {
        self.system_window.deinit();
        self.compositor_window.deinit(allocator);
        self.applications_window.deinit(allocator);
        self.input_window.deinit();
        if (self.render_models) |rm| {
            if (rm) |render_models| {
                self.render_models_window.deinit(render_models);
            } else |_| {}
        }
        if (self.openvr) |openvr| {
            if (openvr) |ovr| {
                ovr.deinit();
            } else |_| {}
        }
        self.openvr = null;
        self.system = null;
        self.chaperone = null;
        self.applications = null;
        self.input = null;
        self.render_models = null;
    }

    fn show(self: *OpenVRWindow, allocator: std.mem.Allocator) !void {
        zgui.setNextWindowPos(.{ .x = 0, .y = 0, .cond = .first_use_ever });

        defer zgui.end();
        if (zgui.begin("OpenVR", .{ .flags = .{ .always_auto_resize = true } })) {
            try ui.staticGetter(OpenVR, "isHmdPresent", .{}, null);
            try ui.staticGetter(OpenVR, "isRuntimeInstalled", .{}, null);
            if (self.openvr) |openvr| {
                if (openvr) |ovr| {
                    if (zgui.button("deinit()", .{})) {
                        self.deinit(allocator);
                        return;
                    }
                    zgui.newLine();

                    if (self.system) |system| {
                        if (system) |sys| {
                            const focus = zgui.button("focus system window", .{});
                            try self.system_window.show(sys, allocator, focus);
                        } else |err| {
                            zgui.text("system() error: {!}", .{err});
                        }
                    } else {
                        self.system = ovr.system();
                    }
                    if (self.chaperone) |chaperone| {
                        if (chaperone) |chap| {
                            const focus = zgui.button("focus chaperone window", .{});
                            try self.chaperone_window.show(chap, allocator, focus);
                        } else |err| {
                            zgui.text("chaperone() error: {!}", .{err});
                        }
                    } else {
                        self.chaperone = ovr.chaperone();
                    }
                    if (self.compositor) |compositor| {
                        if (compositor) |comp| {
                            const focus = zgui.button("focus compositor window", .{});
                            try self.compositor_window.show(comp, allocator, focus);
                        } else |err| {
                            zgui.text("compositor() error: {!}", .{err});
                        }
                    } else {
                        self.compositor = ovr.compositor();
                    }
                    if (self.applications) |applications| {
                        if (applications) |apps| {
                            const focus = zgui.button("focus applications window", .{});
                            try self.applications_window.show(apps, allocator, focus);
                        } else |err| {
                            zgui.text("applications() error: {!}", .{err});
                        }
                    } else {
                        self.applications = ovr.applications();
                    }
                    if (self.input) |input| {
                        if (input) |inp| {
                            const focus = zgui.button("focus input window", .{});
                            try self.input_window.show(inp, allocator, focus);
                        } else |err| {
                            zgui.text("input() error: {!}", .{err});
                        }
                    } else {
                        self.input = ovr.input();
                    }
                    if (self.render_models) |render_models| {
                        if (render_models) |rm| {
                            const focus = zgui.button("focus render models window", .{});
                            try self.render_models_window.show(rm, allocator, focus);
                        } else |err| {
                            zgui.text("renderModels() error: {!}", .{err});
                        }
                    } else {
                        self.render_models = ovr.renderModels();
                    }
                } else |err| {
                    zgui.text("OpenVR.init() error: {!}", .{err});
                }
            } else {
                if (zgui.button("OpenVR.init()", .{})) {
                    self.openvr = OpenVR.init(.scene);
                }
            }
            {
                zgui.separatorText("InitErrorCode");
                try ui.staticGetter(OpenVR.InitErrorCode, "asSymbol", .{ .init_error = &self.symbol_static_error }, null);
                try ui.staticGetter(OpenVR.InitErrorCode, "asEnglishDescription", .{ .init_error = &self.english_static_error }, null);
            }
        }
    }
};

pub fn main() !void {
    // Change current working directory to where the executable is located.
    {
        var buffer: [1024]u8 = undefined;
        const path = std.fs.selfExeDirPath(buffer[0..]) catch ".";
        try std.posix.chdir(path);
    }

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    const allocator = gpa.allocator();

    try zglfw.init();
    defer zglfw.terminate();

    var surface = try Surface.init(allocator, 1280, 720);
    defer surface.deinit(allocator);

    var open_vr_window = OpenVRWindow.init(allocator);
    defer open_vr_window.deinit(allocator);

    var frame_timer = try std.time.Timer.start();

    while (!surface.window.shouldClose() and surface.window.getKey(.escape) != .press) {
        {
            const next_framebuffer_size = surface.window.getFramebufferSize();
            if (!std.meta.eql(surface.framebuffer_size, next_framebuffer_size)) {
                surface.setFrameBufferSize(next_framebuffer_size);
            }
        }

        {
            // spin loop for frame limiter
            const frame_rate_target: u64 = 60;
            const target_ns = @divTrunc(std.time.ns_per_s, frame_rate_target);
            while (frame_timer.read() < target_ns) {
                std.atomic.spinLoopHint();
            }
            frame_timer.reset();
        }

        // poll for input immediately after vsync or frame limiter to reduce input latency
        zglfw.pollEvents();

        {
            surface.gctx.beginFrame();
            defer surface.gctx.endFrame();

            const back_buffer = surface.gctx.getBackBuffer();
            surface.gctx.addTransitionBarrier(back_buffer.resource_handle, .{ .RENDER_TARGET = true });
            surface.gctx.flushResourceBarriers();

            surface.gctx.cmdlist.OMSetRenderTargets(
                1,
                &.{back_buffer.descriptor_handle},
                windows.TRUE,
                null,
            );
            surface.gctx.cmdlist.ClearRenderTargetView(
                back_buffer.descriptor_handle,
                &.{ 0.0, 0.0, 0.0, 1.0 },
                0,
                null,
            );

            zgui.backend.newFrame(
                @intCast(surface.framebuffer_size[0]),
                @intCast(surface.framebuffer_size[1]),
            );

            // try display_window.show(allocator, surface);
            try open_vr_window.show(allocator);

            zgui.backend.draw(surface.gctx.cmdlist);

            surface.gctx.addTransitionBarrier(back_buffer.resource_handle, d3d12.RESOURCE_STATES.PRESENT);
            surface.gctx.flushResourceBarriers();
        }
    }

    surface.gctx.finishGpuCommands();
}
