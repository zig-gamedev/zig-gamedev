const std = @import("std");

const common = @import("common.zig");

function_table: *FunctionTable,

const Self = @This();

const VkInstance = anyopaque;

pub const Vector2 = extern struct {
    v: [2]f32,
};
pub const HiddenAreaMesh = extern struct {
    vertex_data: [*c]Vector2,
    triangle_count: u32,
};

pub const DistortionCoordinates = extern struct {
    red: [2]f32,
    green: [2]f32,
    blue: [2]f32,
};

pub const TrackedDeviceClass = enum(i32) {
    invalid = 0,
    hmd = 1,
    controller = 2,
    generic_tracker = 3,
    tracking_reference = 4,
    display_redirect = 5,
    max = 6,
};

pub const DeviceActivityLevel = enum(i32) {
    unknown = -1,
    idle = 0,
    user_interaction = 1,
    user_interaction_timeout = 2,
    standby = 3,
    idle_timeout = 4,
};

pub const TrackedDeviceProperty = enum(i32) {
    invalid = 0,
    tracking_system_name_string = 1000,
    model_number_string = 1001,
    serial_number_string = 1002,
    render_model_name_string = 1003,
    will_drift_in_yaw_bool = 1004,
    manufacturer_name_string = 1005,
    tracking_firmware_version_string = 1006,
    hardware_revision_string = 1007,
    all_wireless_dongle_descriptions_string = 1008,
    connected_wireless_dongle_string = 1009,
    device_is_wireless_bool = 1010,
    device_is_charging_bool = 1011,
    device_battery_percentage_float = 1012,
    status_display_transform_matrix34 = 1013,
    firmware_update_available_bool = 1014,
    firmware_manual_update_bool = 1015,
    firmware_manual_update_url_string = 1016,
    hardware_revision_uint64 = 1017,
    firmware_version_uint64 = 1018,
    fpga_version_uint64 = 1019,
    vrc_version_uint64 = 1020,
    radio_version_uint64 = 1021,
    dongle_version_uint64 = 1022,
    block_server_shutdown_bool = 1023,
    can_unify_coordinate_system_with_hmd_bool = 1024,
    contains_proximity_sensor_bool = 1025,
    device_provides_battery_status_bool = 1026,
    device_can_power_off_bool = 1027,
    firmware_programming_target_string = 1028,
    device_class_int32 = 1029,
    has_camera_bool = 1030,
    driver_version_string = 1031,
    firmware_force_update_required_bool = 1032,
    vive_system_button_fix_required_bool = 1033,
    parent_driver_uint64 = 1034,
    resource_root_string = 1035,
    registered_device_type_string = 1036,
    input_profile_path_string = 1037,
    never_tracked_bool = 1038,
    num_cameras_int32 = 1039,
    camera_frame_layout_int32 = 1040,
    camera_stream_format_int32 = 1041,
    additional_device_settings_path_string = 1042,
    identifiable_bool = 1043,
    bootloader_version_uint64 = 1044,
    additional_system_report_data_string = 1045,
    composite_firmware_version_string = 1046,
    firmware_remind_update_bool = 1047,
    peripheral_application_version_uint64 = 1048,
    manufacturer_serial_number_string = 1049,
    computed_serial_number_string = 1050,
    estimated_device_first_use_time_int32 = 1051,
    device_power_usage_float = 1052,
    ignore_motion_for_standby_bool = 1053,
    actual_tracking_system_name_string = 1054,
    reports_time_since_v_sync_bool = 2000,
    seconds_from_vsync_to_photons_float = 2001,
    display_frequency_float = 2002,
    user_ipd_meters_float = 2003,
    current_universe_id_uint64 = 2004,
    previous_universe_id_uint64 = 2005,
    display_firmware_version_uint64 = 2006,
    is_on_desktop_bool = 2007,
    display_mc_type_int32 = 2008,
    display_mc_offset_float = 2009,
    display_mc_scale_float = 2010,
    edid_vendor_id_int32 = 2011,
    display_mc_image_left_string = 2012,
    display_mc_image_right_string = 2013,
    display_gc_black_clamp_float = 2014,
    edid_product_id_int32 = 2015,
    camera_to_head_transform_matrix34 = 2016,
    display_gc_type_int32 = 2017,
    display_gc_offset_float = 2018,
    display_gc_scale_float = 2019,
    display_gc_prescale_float = 2020,
    display_gc_image_string = 2021,
    lens_center_left_u_float = 2022,
    lens_center_left_v_float = 2023,
    lens_center_right_u_float = 2024,
    lens_center_right_v_float = 2025,
    user_head_to_eye_depth_meters_float = 2026,
    camera_firmware_version_uint64 = 2027,
    camera_firmware_description_string = 2028,
    display_fpga_version_uint64 = 2029,
    display_bootloader_version_uint64 = 2030,
    display_hardware_version_uint64 = 2031,
    audio_firmware_version_uint64 = 2032,
    camera_compatibility_mode_int32 = 2033,
    screenshot_horizontal_field_of_view_degrees_float = 2034,
    screenshot_vertical_field_of_view_degrees_float = 2035,
    display_suppressed_bool = 2036,
    display_allow_night_mode_bool = 2037,
    display_mc_image_width_int32 = 2038,
    display_mc_image_height_int32 = 2039,
    display_mc_image_num_channels_int32 = 2040,
    display_mc_image_data_binary = 2041,
    seconds_from_photons_to_vblank_float = 2042,
    driver_direct_mode_sends_vsync_events_bool = 2043,
    display_debug_mode_bool = 2044,
    graphics_adapter_luid_uint64 = 2045,
    driver_provided_chaperone_path_string = 2048,
    expected_tracking_reference_count_int32 = 2049,
    expected_controller_count_int32 = 2050,
    named_icon_path_controller_left_device_off_string = 2051,
    named_icon_path_controller_right_device_off_string = 2052,
    named_icon_path_tracking_reference_device_off_string = 2053,
    do_not_apply_prediction_bool = 2054,
    camera_to_head_transforms_matrix34_array = 2055,
    distortion_mesh_resolution_int32 = 2056,
    driver_is_drawing_controllers_bool = 2057,
    driver_requests_application_pause_bool = 2058,
    driver_requests_reduced_rendering_bool = 2059,
    minimum_ipd_step_meters_float = 2060,
    audio_bridge_firmware_version_uint64 = 2061,
    image_bridge_firmware_version_uint64 = 2062,
    imu_to_head_transform_matrix34 = 2063,
    imu_factory_gyro_bias_vector3 = 2064,
    imu_factory_gyro_scale_vector3 = 2065,
    imu_factory_accelerometer_bias_vector3 = 2066,
    imu_factory_accelerometer_scale_vector3 = 2067,
    configuration_includes_lighthouse20_features_bool = 2069,
    additional_radio_features_uint64 = 2070,
    camera_white_balance_vector4_array = 2071,
    camera_distortion_function_int32_array = 2072,
    camera_distortion_coefficients_float_array = 2073,
    expected_controller_type_string = 2074,
    hmd_tracking_style_int32 = 2075,
    driver_provided_chaperone_visibility_bool = 2076,
    hmd_column_correction_setting_prefix_string = 2077,
    camera_supports_compatibility_modes_bool = 2078,
    supports_room_view_depth_projection_bool = 2079,
    display_available_frame_rates_float_array = 2080,
    display_supports_multiple_framerates_bool = 2081,
    display_color_mult_left_vector3 = 2082,
    display_color_mult_right_vector3 = 2083,
    display_supports_runtime_framerate_change_bool = 2084,
    display_supports_analog_gain_bool = 2085,
    display_min_analog_gain_float = 2086,
    display_max_analog_gain_float = 2087,
    camera_exposure_time_float = 2088,
    camera_global_gain_float = 2089,
    dashboard_scale_float = 2091,
    peer_button_info_string = 2092,
    hmd_supports_hdr10_bool = 2093,
    hmd_enable_parallel_render_cameras_bool = 2094,
    driver_provided_chaperone_json_string = 2095,
    force_system_layer_use_app_poses_bool = 2096,
    ipd_ui_range_min_meters_float = 2100,
    ipd_ui_range_max_meters_float = 2101,
    hmd_supports_hdcp14_legacy_compat_bool = 2102,
    hmd_supports_mic_monitoring_bool = 2103,
    hmd_supports_display_port_training_mode_bool = 2104,
    hmd_supports_room_view_direct_bool = 2105,
    hmd_supports_app_throttling_bool = 2106,
    hmd_supports_gpu_bus_monitoring_bool = 2107,
    driver_displays_ipd_changes_bool = 2108,
    driver_reserved_01 = 2109,
    dsc_version_int32 = 2110,
    dsc_slice_count_int32 = 2111,
    dscbp_px16_int32 = 2112,
    hmd_max_distorted_texture_width_int32 = 2113,
    hmd_max_distorted_texture_height_int32 = 2114,
    hmd_allow_supersample_filtering_bool = 2115,
    driver_requested_mura_correction_mode_int32 = 2200,
    driver_requested_mura_feather_inner_left_int32 = 2201,
    driver_requested_mura_feather_inner_right_int32 = 2202,
    driver_requested_mura_feather_inner_top_int32 = 2203,
    driver_requested_mura_feather_inner_bottom_int32 = 2204,
    driver_requested_mura_feather_outer_left_int32 = 2205,
    driver_requested_mura_feather_outer_right_int32 = 2206,
    driver_requested_mura_feather_outer_top_int32 = 2207,
    driver_requested_mura_feather_outer_bottom_int32 = 2208,
    audio_default_playback_device_id_string = 2300,
    audio_default_recording_device_id_string = 2301,
    audio_default_playback_device_volume_float = 2302,
    audio_supports_dual_speaker_and_jack_output_bool = 2303,
    audio_driver_manages_playback_volume_control_bool = 2304,
    audio_driver_playback_volume_float = 2305,
    audio_driver_playback_mute_bool = 2306,
    audio_driver_manages_recording_volume_control_bool = 2307,
    audio_driver_recording_volume_float = 2308,
    audio_driver_recording_mute_bool = 2309,
    attached_device_id_string = 3000,
    supported_buttons_uint64 = 3001,
    axis0_type_int32 = 3002,
    axis1_type_int32 = 3003,
    axis2_type_int32 = 3004,
    axis3_type_int32 = 3005,
    axis4_type_int32 = 3006,
    controller_role_hint_int32 = 3007,
    field_of_view_left_degrees_float = 4000,
    field_of_view_right_degrees_float = 4001,
    field_of_view_top_degrees_float = 4002,
    field_of_view_bottom_degrees_float = 4003,
    tracking_range_minimum_meters_float = 4004,
    tracking_range_maximum_meters_float = 4005,
    mode_label_string = 4006,
    can_wireless_identify_bool = 4007,
    nonce_int32 = 4008,
    icon_path_name_string = 5000,
    named_icon_path_device_off_string = 5001,
    named_icon_path_device_searching_string = 5002,
    named_icon_path_device_searching_alert_string = 5003,
    named_icon_path_device_ready_string = 5004,
    named_icon_path_device_ready_alert_string = 5005,
    named_icon_path_device_not_ready_string = 5006,
    named_icon_path_device_standby_string = 5007,
    named_icon_path_device_alert_low_string = 5008,
    named_icon_path_device_standby_alert_string = 5009,
    display_hidden_area_binary_start = 5100,
    display_hidden_area_binary_end = 5150,
    parent_container = 5151,
    override_container_uint64 = 5152,
    user_config_path_string = 6000,
    install_path_string = 6001,
    has_display_component_bool = 6002,
    has_controller_component_bool = 6003,
    has_camera_component_bool = 6004,
    has_driver_direct_mode_component_bool = 6005,
    has_virtual_display_component_bool = 6006,
    has_spatial_anchors_support_bool = 6007,
    supports_xr_texture_sets_bool = 6008,
    controller_type_string = 7000,
    controller_hand_selection_priority_int32 = 7002,
    vendor_specific_reserved_start = 10000,
    vendor_specific_reserved_end = 10999,
    tracked_device_property_max = 1000000,

    pub fn fromType(comptime T: type) type {
        return switch (T) {
            bool => TrackedDeviceProperty.Bool,
            f32 => TrackedDeviceProperty.F32,
            i32 => TrackedDeviceProperty.I32,
            u64 => TrackedDeviceProperty.U64,
            common.Matrix34 => TrackedDeviceProperty.Matrix34,
            else => @compileError("T must be one of bool, f32, i32, u64, Matrix34"),
        };
    }

    pub const Bool = enum(i32) {
        will_drift_in_yaw = @intFromEnum(TrackedDeviceProperty.will_drift_in_yaw_bool),
        device_is_wireless = @intFromEnum(TrackedDeviceProperty.device_is_wireless_bool),
        device_is_charging = @intFromEnum(TrackedDeviceProperty.device_is_charging_bool),
        firmware_update_available = @intFromEnum(TrackedDeviceProperty.firmware_update_available_bool),
        firmware_manual_update = @intFromEnum(TrackedDeviceProperty.firmware_manual_update_bool),
        block_server_shutdown = @intFromEnum(TrackedDeviceProperty.block_server_shutdown_bool),
        can_unify_coordinate_system_with_hmd = @intFromEnum(TrackedDeviceProperty.can_unify_coordinate_system_with_hmd_bool),
        contains_proximity_sensor = @intFromEnum(TrackedDeviceProperty.contains_proximity_sensor_bool),
        device_provides_battery_status = @intFromEnum(TrackedDeviceProperty.device_provides_battery_status_bool),
        device_can_power_off = @intFromEnum(TrackedDeviceProperty.device_can_power_off_bool),
        has_camera = @intFromEnum(TrackedDeviceProperty.has_camera_bool),
        firmware_force_update_required = @intFromEnum(TrackedDeviceProperty.firmware_force_update_required_bool),
        vive_system_button_fix_required = @intFromEnum(TrackedDeviceProperty.vive_system_button_fix_required_bool),
        never_tracked = @intFromEnum(TrackedDeviceProperty.never_tracked_bool),
        identifiable = @intFromEnum(TrackedDeviceProperty.identifiable_bool),
        firmware_remind_update = @intFromEnum(TrackedDeviceProperty.firmware_remind_update_bool),
        ignore_motion_for_standby = @intFromEnum(TrackedDeviceProperty.ignore_motion_for_standby_bool),
        reports_time_since_v_sync = @intFromEnum(TrackedDeviceProperty.reports_time_since_v_sync_bool),
        is_on_desktop = @intFromEnum(TrackedDeviceProperty.is_on_desktop_bool),
        display_suppressed = @intFromEnum(TrackedDeviceProperty.display_suppressed_bool),
        display_allow_night_mode = @intFromEnum(TrackedDeviceProperty.display_allow_night_mode_bool),
        driver_direct_mode_sends_vsync_events = @intFromEnum(TrackedDeviceProperty.driver_direct_mode_sends_vsync_events_bool),
        display_debug_mode = @intFromEnum(TrackedDeviceProperty.display_debug_mode_bool),
        do_not_apply_prediction = @intFromEnum(TrackedDeviceProperty.do_not_apply_prediction_bool),
        driver_is_drawing_controllers = @intFromEnum(TrackedDeviceProperty.driver_is_drawing_controllers_bool),
        driver_requests_application_pause = @intFromEnum(TrackedDeviceProperty.driver_requests_application_pause_bool),
        driver_requests_reduced_rendering = @intFromEnum(TrackedDeviceProperty.driver_requests_reduced_rendering_bool),
        configuration_includes_lighthouse20_features = @intFromEnum(TrackedDeviceProperty.configuration_includes_lighthouse20_features_bool),
        driver_provided_chaperone_visibility = @intFromEnum(TrackedDeviceProperty.driver_provided_chaperone_visibility_bool),
        camera_supports_compatibility_modes = @intFromEnum(TrackedDeviceProperty.camera_supports_compatibility_modes_bool),
        supports_room_view_depth_projection = @intFromEnum(TrackedDeviceProperty.supports_room_view_depth_projection_bool),
        display_supports_multiple_framerates = @intFromEnum(TrackedDeviceProperty.display_supports_multiple_framerates_bool),
        display_supports_runtime_framerate_change = @intFromEnum(TrackedDeviceProperty.display_supports_runtime_framerate_change_bool),
        display_supports_analog_gain = @intFromEnum(TrackedDeviceProperty.display_supports_analog_gain_bool),
        hmd_supports_hdr10 = @intFromEnum(TrackedDeviceProperty.hmd_supports_hdr10_bool),
        hmd_enable_parallel_render_cameras = @intFromEnum(TrackedDeviceProperty.hmd_enable_parallel_render_cameras_bool),
        force_system_layer_use_app_poses = @intFromEnum(TrackedDeviceProperty.force_system_layer_use_app_poses_bool),
        hmd_supports_hdcp14_legacy_compat = @intFromEnum(TrackedDeviceProperty.hmd_supports_hdcp14_legacy_compat_bool),
        hmd_supports_mic_monitoring = @intFromEnum(TrackedDeviceProperty.hmd_supports_mic_monitoring_bool),
        hmd_supports_display_port_training_mode = @intFromEnum(TrackedDeviceProperty.hmd_supports_display_port_training_mode_bool),
        hmd_supports_room_view_direct = @intFromEnum(TrackedDeviceProperty.hmd_supports_room_view_direct_bool),
        hmd_supports_app_throttling = @intFromEnum(TrackedDeviceProperty.hmd_supports_app_throttling_bool),
        hmd_supports_gpu_bus_monitoring = @intFromEnum(TrackedDeviceProperty.hmd_supports_gpu_bus_monitoring_bool),
        driver_displays_ipd_changes = @intFromEnum(TrackedDeviceProperty.driver_displays_ipd_changes_bool),
        hmd_allow_supersample_filtering = @intFromEnum(TrackedDeviceProperty.hmd_allow_supersample_filtering_bool),
        audio_supports_dual_speaker_and_jack_output = @intFromEnum(TrackedDeviceProperty.audio_supports_dual_speaker_and_jack_output_bool),
        audio_driver_manages_playback_volume_control = @intFromEnum(TrackedDeviceProperty.audio_driver_manages_playback_volume_control_bool),
        audio_driver_playback_mute = @intFromEnum(TrackedDeviceProperty.audio_driver_playback_mute_bool),
        audio_driver_manages_recording_volume_control = @intFromEnum(TrackedDeviceProperty.audio_driver_manages_recording_volume_control_bool),
        audio_driver_recording_mute = @intFromEnum(TrackedDeviceProperty.audio_driver_recording_mute_bool),
        can_wireless_identify = @intFromEnum(TrackedDeviceProperty.can_wireless_identify_bool),
        has_display_component = @intFromEnum(TrackedDeviceProperty.has_display_component_bool),
        has_controller_component = @intFromEnum(TrackedDeviceProperty.has_controller_component_bool),
        has_camera_component = @intFromEnum(TrackedDeviceProperty.has_camera_component_bool),
        has_driver_direct_mode_component = @intFromEnum(TrackedDeviceProperty.has_driver_direct_mode_component_bool),
        has_virtual_display_component = @intFromEnum(TrackedDeviceProperty.has_virtual_display_component_bool),
        has_spatial_anchors_support = @intFromEnum(TrackedDeviceProperty.has_spatial_anchors_support_bool),
        supports_xr_texture_sets = @intFromEnum(TrackedDeviceProperty.supports_xr_texture_sets_bool),
    };

    pub const F32 = enum(i32) {
        device_battery_percentage = @intFromEnum(TrackedDeviceProperty.device_battery_percentage_float),
        device_power_usage = @intFromEnum(TrackedDeviceProperty.device_power_usage_float),
        seconds_from_vsync_to_photons = @intFromEnum(TrackedDeviceProperty.seconds_from_vsync_to_photons_float),
        display_frequency = @intFromEnum(TrackedDeviceProperty.display_frequency_float),
        user_ipd_meters = @intFromEnum(TrackedDeviceProperty.user_ipd_meters_float),
        display_mc_offset = @intFromEnum(TrackedDeviceProperty.display_mc_offset_float),
        display_mc_scale = @intFromEnum(TrackedDeviceProperty.display_mc_scale_float),
        display_gc_black_clamp = @intFromEnum(TrackedDeviceProperty.display_gc_black_clamp_float),
        display_gc_offset = @intFromEnum(TrackedDeviceProperty.display_gc_offset_float),
        display_gc_scale = @intFromEnum(TrackedDeviceProperty.display_gc_scale_float),
        display_gc_prescale = @intFromEnum(TrackedDeviceProperty.display_gc_prescale_float),
        lens_center_left_u = @intFromEnum(TrackedDeviceProperty.lens_center_left_u_float),
        lens_center_left_v = @intFromEnum(TrackedDeviceProperty.lens_center_left_v_float),
        lens_center_right_u = @intFromEnum(TrackedDeviceProperty.lens_center_right_u_float),
        lens_center_right_v = @intFromEnum(TrackedDeviceProperty.lens_center_right_v_float),
        user_head_to_eye_depth_meters = @intFromEnum(TrackedDeviceProperty.user_head_to_eye_depth_meters_float),
        screenshot_horizontal_field_of_view_degrees = @intFromEnum(TrackedDeviceProperty.screenshot_horizontal_field_of_view_degrees_float),
        screenshot_vertical_field_of_view_degrees = @intFromEnum(TrackedDeviceProperty.screenshot_vertical_field_of_view_degrees_float),
        seconds_from_photons_to_vblank = @intFromEnum(TrackedDeviceProperty.seconds_from_photons_to_vblank_float),
        minimum_ipd_step_meters = @intFromEnum(TrackedDeviceProperty.minimum_ipd_step_meters_float),
        display_min_analog_gain = @intFromEnum(TrackedDeviceProperty.display_min_analog_gain_float),
        display_max_analog_gain = @intFromEnum(TrackedDeviceProperty.display_max_analog_gain_float),
        camera_exposure_time = @intFromEnum(TrackedDeviceProperty.camera_exposure_time_float),
        camera_global_gain = @intFromEnum(TrackedDeviceProperty.camera_global_gain_float),
        dashboard_scale = @intFromEnum(TrackedDeviceProperty.dashboard_scale_float),
        ipd_ui_range_min_meters = @intFromEnum(TrackedDeviceProperty.ipd_ui_range_min_meters_float),
        ipd_ui_range_max_meters = @intFromEnum(TrackedDeviceProperty.ipd_ui_range_max_meters_float),
        audio_default_playback_device_volume = @intFromEnum(TrackedDeviceProperty.audio_default_playback_device_volume_float),
        audio_driver_playback_volume = @intFromEnum(TrackedDeviceProperty.audio_driver_playback_volume_float),
        audio_driver_recording_volume = @intFromEnum(TrackedDeviceProperty.audio_driver_recording_volume_float),
        field_of_view_left_degrees = @intFromEnum(TrackedDeviceProperty.field_of_view_left_degrees_float),
        field_of_view_right_degrees = @intFromEnum(TrackedDeviceProperty.field_of_view_right_degrees_float),
        field_of_view_top_degrees = @intFromEnum(TrackedDeviceProperty.field_of_view_top_degrees_float),
        field_of_view_bottom_degrees = @intFromEnum(TrackedDeviceProperty.field_of_view_bottom_degrees_float),
        tracking_range_minimum_meters = @intFromEnum(TrackedDeviceProperty.tracking_range_minimum_meters_float),
        tracking_range_maximum_meters = @intFromEnum(TrackedDeviceProperty.tracking_range_maximum_meters_float),
    };

    pub const I32 = enum(i32) {
        device_class = @intFromEnum(TrackedDeviceProperty.device_class_int32),
        num_cameras = @intFromEnum(TrackedDeviceProperty.num_cameras_int32),
        camera_frame_layout = @intFromEnum(TrackedDeviceProperty.camera_frame_layout_int32),
        camera_stream_format = @intFromEnum(TrackedDeviceProperty.camera_stream_format_int32),
        estimated_device_first_use_time = @intFromEnum(TrackedDeviceProperty.estimated_device_first_use_time_int32),
        display_mc_type = @intFromEnum(TrackedDeviceProperty.display_mc_type_int32),
        edid_vendor_id = @intFromEnum(TrackedDeviceProperty.edid_vendor_id_int32),
        edid_product_id = @intFromEnum(TrackedDeviceProperty.edid_product_id_int32),
        display_gc_type = @intFromEnum(TrackedDeviceProperty.display_gc_type_int32),
        camera_compatibility_mode = @intFromEnum(TrackedDeviceProperty.camera_compatibility_mode_int32),
        display_mc_image_width = @intFromEnum(TrackedDeviceProperty.display_mc_image_width_int32),
        display_mc_image_height = @intFromEnum(TrackedDeviceProperty.display_mc_image_height_int32),
        display_mc_image_num_channels = @intFromEnum(TrackedDeviceProperty.display_mc_image_num_channels_int32),
        expected_tracking_reference_count = @intFromEnum(TrackedDeviceProperty.expected_tracking_reference_count_int32),
        expected_controller_count = @intFromEnum(TrackedDeviceProperty.expected_controller_count_int32),
        distortion_mesh_resolution = @intFromEnum(TrackedDeviceProperty.distortion_mesh_resolution_int32),
        hmd_tracking_style = @intFromEnum(TrackedDeviceProperty.hmd_tracking_style_int32),
        dsc_version = @intFromEnum(TrackedDeviceProperty.dsc_version_int32),
        dsc_slice_count = @intFromEnum(TrackedDeviceProperty.dsc_slice_count_int32),
        dscbp_px16 = @intFromEnum(TrackedDeviceProperty.dscbp_px16_int32),
        hmd_max_distorted_texture_width = @intFromEnum(TrackedDeviceProperty.hmd_max_distorted_texture_width_int32),
        hmd_max_distorted_texture_height = @intFromEnum(TrackedDeviceProperty.hmd_max_distorted_texture_height_int32),
        driver_requested_mura_correction_mode = @intFromEnum(TrackedDeviceProperty.driver_requested_mura_correction_mode_int32),
        driver_requested_mura_feather_inner_left = @intFromEnum(TrackedDeviceProperty.driver_requested_mura_feather_inner_left_int32),
        driver_requested_mura_feather_inner_right = @intFromEnum(TrackedDeviceProperty.driver_requested_mura_feather_inner_right_int32),
        driver_requested_mura_feather_inner_top = @intFromEnum(TrackedDeviceProperty.driver_requested_mura_feather_inner_top_int32),
        driver_requested_mura_feather_inner_bottom = @intFromEnum(TrackedDeviceProperty.driver_requested_mura_feather_inner_bottom_int32),
        driver_requested_mura_feather_outer_left = @intFromEnum(TrackedDeviceProperty.driver_requested_mura_feather_outer_left_int32),
        driver_requested_mura_feather_outer_right = @intFromEnum(TrackedDeviceProperty.driver_requested_mura_feather_outer_right_int32),
        driver_requested_mura_feather_outer_top = @intFromEnum(TrackedDeviceProperty.driver_requested_mura_feather_outer_top_int32),
        driver_requested_mura_feather_outer_bottom = @intFromEnum(TrackedDeviceProperty.driver_requested_mura_feather_outer_bottom_int32),
        axis0_type = @intFromEnum(TrackedDeviceProperty.axis0_type_int32),
        axis1_type = @intFromEnum(TrackedDeviceProperty.axis1_type_int32),
        axis2_type = @intFromEnum(TrackedDeviceProperty.axis2_type_int32),
        axis3_type = @intFromEnum(TrackedDeviceProperty.axis3_type_int32),
        axis4_type = @intFromEnum(TrackedDeviceProperty.axis4_type_int32),
        controller_role_hint = @intFromEnum(TrackedDeviceProperty.controller_role_hint_int32),
        nonce = @intFromEnum(TrackedDeviceProperty.nonce_int32),
        controller_hand_selection_priority = @intFromEnum(TrackedDeviceProperty.controller_hand_selection_priority_int32),
    };

    pub const U64 = enum(i32) {
        hardware_revision = @intFromEnum(TrackedDeviceProperty.hardware_revision_uint64),
        firmware_version = @intFromEnum(TrackedDeviceProperty.firmware_version_uint64),
        fpga_version = @intFromEnum(TrackedDeviceProperty.fpga_version_uint64),
        vrc_version = @intFromEnum(TrackedDeviceProperty.vrc_version_uint64),
        radio_version = @intFromEnum(TrackedDeviceProperty.radio_version_uint64),
        dongle_version = @intFromEnum(TrackedDeviceProperty.dongle_version_uint64),
        parent_driver = @intFromEnum(TrackedDeviceProperty.parent_driver_uint64),
        bootloader_version = @intFromEnum(TrackedDeviceProperty.bootloader_version_uint64),
        peripheral_application_version = @intFromEnum(TrackedDeviceProperty.peripheral_application_version_uint64),
        current_universe_id = @intFromEnum(TrackedDeviceProperty.current_universe_id_uint64),
        previous_universe_id = @intFromEnum(TrackedDeviceProperty.previous_universe_id_uint64),
        display_firmware_version = @intFromEnum(TrackedDeviceProperty.display_firmware_version_uint64),
        camera_firmware_version = @intFromEnum(TrackedDeviceProperty.camera_firmware_version_uint64),
        display_fpga_version = @intFromEnum(TrackedDeviceProperty.display_fpga_version_uint64),
        display_bootloader_version = @intFromEnum(TrackedDeviceProperty.display_bootloader_version_uint64),
        display_hardware_version = @intFromEnum(TrackedDeviceProperty.display_hardware_version_uint64),
        audio_firmware_version = @intFromEnum(TrackedDeviceProperty.audio_firmware_version_uint64),
        graphics_adapter_luid = @intFromEnum(TrackedDeviceProperty.graphics_adapter_luid_uint64),
        audio_bridge_firmware_version = @intFromEnum(TrackedDeviceProperty.audio_bridge_firmware_version_uint64),
        image_bridge_firmware_version = @intFromEnum(TrackedDeviceProperty.image_bridge_firmware_version_uint64),
        additional_radio_features = @intFromEnum(TrackedDeviceProperty.additional_radio_features_uint64),
        supported_buttons = @intFromEnum(TrackedDeviceProperty.supported_buttons_uint64),
        override_container = @intFromEnum(TrackedDeviceProperty.override_container_uint64),
    };

    pub const Matrix34 = enum(i32) {
        status_display_transform = @intFromEnum(TrackedDeviceProperty.status_display_transform_matrix34),
        camera_to_head_transform = @intFromEnum(TrackedDeviceProperty.camera_to_head_transform_matrix34),
        imu_to_head_transform = @intFromEnum(TrackedDeviceProperty.imu_to_head_transform_matrix34),
    };

    pub const String = enum(i32) {
        tracking_system_name = @intFromEnum(TrackedDeviceProperty.tracking_system_name_string),
        model_number = @intFromEnum(TrackedDeviceProperty.model_number_string),
        serial_number = @intFromEnum(TrackedDeviceProperty.serial_number_string),
        render_model_name = @intFromEnum(TrackedDeviceProperty.render_model_name_string),
        manufacturer_name = @intFromEnum(TrackedDeviceProperty.manufacturer_name_string),
        tracking_firmware_version = @intFromEnum(TrackedDeviceProperty.tracking_firmware_version_string),
        hardware_revision = @intFromEnum(TrackedDeviceProperty.hardware_revision_string),
        all_wireless_dongle_descriptions = @intFromEnum(TrackedDeviceProperty.all_wireless_dongle_descriptions_string),
        connected_wireless_dongle = @intFromEnum(TrackedDeviceProperty.connected_wireless_dongle_string),
        firmware_manual_update_url = @intFromEnum(TrackedDeviceProperty.firmware_manual_update_url_string),
        firmware_programming_target = @intFromEnum(TrackedDeviceProperty.firmware_programming_target_string),
        driver_version = @intFromEnum(TrackedDeviceProperty.driver_version_string),
        resource_root = @intFromEnum(TrackedDeviceProperty.resource_root_string),
        registered_device_type = @intFromEnum(TrackedDeviceProperty.registered_device_type_string),
        input_profile_path = @intFromEnum(TrackedDeviceProperty.input_profile_path_string),
        additional_device_settings_path = @intFromEnum(TrackedDeviceProperty.additional_device_settings_path_string),
        additional_system_report_data = @intFromEnum(TrackedDeviceProperty.additional_system_report_data_string),
        composite_firmware_version = @intFromEnum(TrackedDeviceProperty.composite_firmware_version_string),
        manufacturer_serial_number = @intFromEnum(TrackedDeviceProperty.manufacturer_serial_number_string),
        computed_serial_number = @intFromEnum(TrackedDeviceProperty.computed_serial_number_string),
        actual_tracking_system_name = @intFromEnum(TrackedDeviceProperty.actual_tracking_system_name_string),
        display_mc_image_left = @intFromEnum(TrackedDeviceProperty.display_mc_image_left_string),
        display_mc_image_right = @intFromEnum(TrackedDeviceProperty.display_mc_image_right_string),
        display_gc_image = @intFromEnum(TrackedDeviceProperty.display_gc_image_string),
        camera_firmware_description = @intFromEnum(TrackedDeviceProperty.camera_firmware_description_string),
        driver_provided_chaperone_path = @intFromEnum(TrackedDeviceProperty.driver_provided_chaperone_path_string),
        named_icon_path_controller_left_device_off = @intFromEnum(TrackedDeviceProperty.named_icon_path_controller_left_device_off_string),
        named_icon_path_controller_right_device_off = @intFromEnum(TrackedDeviceProperty.named_icon_path_controller_right_device_off_string),
        named_icon_path_tracking_reference_device_off = @intFromEnum(TrackedDeviceProperty.named_icon_path_tracking_reference_device_off_string),
        expected_controller_type = @intFromEnum(TrackedDeviceProperty.expected_controller_type_string),
        hmd_column_correction_setting_prefix = @intFromEnum(TrackedDeviceProperty.hmd_column_correction_setting_prefix_string),
        peer_button_info = @intFromEnum(TrackedDeviceProperty.peer_button_info_string),
        driver_provided_chaperone_json = @intFromEnum(TrackedDeviceProperty.driver_provided_chaperone_json_string),
        audio_default_playback_device_id = @intFromEnum(TrackedDeviceProperty.audio_default_playback_device_id_string),
        audio_default_recording_device_id = @intFromEnum(TrackedDeviceProperty.audio_default_recording_device_id_string),
        attached_device_id = @intFromEnum(TrackedDeviceProperty.attached_device_id_string),
        mode_label = @intFromEnum(TrackedDeviceProperty.mode_label_string),
        icon_path_name = @intFromEnum(TrackedDeviceProperty.icon_path_name_string),
        named_icon_path_device_off = @intFromEnum(TrackedDeviceProperty.named_icon_path_device_off_string),
        named_icon_path_device_searching = @intFromEnum(TrackedDeviceProperty.named_icon_path_device_searching_string),
        named_icon_path_device_searching_alert = @intFromEnum(TrackedDeviceProperty.named_icon_path_device_searching_alert_string),
        named_icon_path_device_ready = @intFromEnum(TrackedDeviceProperty.named_icon_path_device_ready_string),
        named_icon_path_device_ready_alert = @intFromEnum(TrackedDeviceProperty.named_icon_path_device_ready_alert_string),
        named_icon_path_device_not_ready = @intFromEnum(TrackedDeviceProperty.named_icon_path_device_not_ready_string),
        named_icon_path_device_standby = @intFromEnum(TrackedDeviceProperty.named_icon_path_device_standby_string),
        named_icon_path_device_alert_low = @intFromEnum(TrackedDeviceProperty.named_icon_path_device_alert_low_string),
        named_icon_path_device_standby_alert = @intFromEnum(TrackedDeviceProperty.named_icon_path_device_standby_alert_string),
        user_config_path = @intFromEnum(TrackedDeviceProperty.user_config_path_string),
        install_path = @intFromEnum(TrackedDeviceProperty.install_path_string),
        controller_type = @intFromEnum(TrackedDeviceProperty.controller_type_string),
    };

    pub const Array = enum(i32) {
        camera_to_head_transforms_matrix34 = @intFromEnum(TrackedDeviceProperty.camera_to_head_transforms_matrix34_array),
        camera_white_balance_vector4 = @intFromEnum(TrackedDeviceProperty.camera_white_balance_vector4_array),
        camera_distortion_function_int32 = @intFromEnum(TrackedDeviceProperty.camera_distortion_function_int32_array),
        camera_distortion_coefficients_float = @intFromEnum(TrackedDeviceProperty.camera_distortion_coefficients_float_array),
        display_available_frame_rates_float = @intFromEnum(TrackedDeviceProperty.display_available_frame_rates_float_array),

        pub fn fromType(comptime T: type) type {
            return switch (T) {
                f32 => TrackedDeviceProperty.Array.F32,
                i32 => TrackedDeviceProperty.Array.I32,
                common.Vector4 => TrackedDeviceProperty.Array.Vector4,
                common.Matrix34 => TrackedDeviceProperty.Array.Matrix34,
                else => @compileError("T must be one of f32, i32, Vector4, Matrix34"),
            };
        }

        pub const F32 = enum(i32) {
            camera_distortion_coefficients = @intFromEnum(TrackedDeviceProperty.camera_distortion_coefficients_float_array),
            display_available_frame_rates = @intFromEnum(TrackedDeviceProperty.display_available_frame_rates_float_array),
        };

        pub const I32 = enum(i32) {
            camera_distortion_function = @intFromEnum(TrackedDeviceProperty.camera_distortion_function_int32_array),
        };

        pub const Vector4 = enum(i32) {
            camera_white_balance = @intFromEnum(TrackedDeviceProperty.camera_white_balance_vector4_array),
        };

        pub const Matrix34 = enum(i32) {
            camera_to_head_transforms = @intFromEnum(TrackedDeviceProperty.camera_to_head_transforms_matrix34_array),
        };
    };
};

pub const TrackedPropertyError = error{
    WrongDataType,
    WrongDeviceClass,
    BufferTooSmall,
    UnknownProperty,
    InvalidDevice,
    CouldNotContactServer,
    ValueNotProvidedByDevice,
    StringExceedsMaximumLength,
    NotYetAvailable,
    PermissionDenied,
    InvalidOperation,
    CannotWriteToWildcards,
    IpcReadFailure,
    OutOfMemory,
    InvalidContainer,
};

pub const TrackedPropertyErrorCode = enum(i32) {
    success = 0,
    wrong_data_type = 1,
    wrong_device_class = 2,
    buffer_too_small = 3,
    unknown_property = 4,
    invalid_device = 5,
    could_not_contact_server = 6,
    value_not_provided_by_device = 7,
    string_exceeds_maximum_length = 8,
    not_yet_available = 9,
    permission_denied = 10,
    invalid_operation = 11,
    cannot_write_to_wildcards = 12,
    ipc_read_failure = 13,
    out_of_memory = 14,
    invalid_container = 15,

    pub fn maybe(property_error: TrackedPropertyErrorCode) TrackedPropertyError!void {
        return switch (property_error) {
            .success => {},
            .wrong_data_type => TrackedPropertyError.WrongDataType,
            .wrong_device_class => TrackedPropertyError.WrongDeviceClass,
            .buffer_too_small => TrackedPropertyError.BufferTooSmall,
            .unknown_property => TrackedPropertyError.UnknownProperty,
            .invalid_device => TrackedPropertyError.InvalidDevice,
            .could_not_contact_server => TrackedPropertyError.CouldNotContactServer,
            .value_not_provided_by_device => TrackedPropertyError.ValueNotProvidedByDevice,
            .string_exceeds_maximum_length => TrackedPropertyError.StringExceedsMaximumLength,
            .not_yet_available => TrackedPropertyError.NotYetAvailable,
            .permission_denied => TrackedPropertyError.PermissionDenied,
            .invalid_operation => TrackedPropertyError.InvalidOperation,
            .cannot_write_to_wildcards => TrackedPropertyError.CannotWriteToWildcards,
            .ipc_read_failure => TrackedPropertyError.IpcReadFailure,
            .out_of_memory => TrackedPropertyError.OutOfMemory,
            .invalid_container => TrackedPropertyError.InvalidContainer,
        };
    }
};

pub const PropertyTypeTag = u32;

pub const Event = extern struct {
    event_type: EventType,
    tracked_device_index: common.TrackedDeviceIndex,
    event_age_seconds: f32,
    data: EventData,
};

pub const EventReserved = extern struct {
    reserved0: u64,
    reserved1: u64,
    reserved2: u64,
    reserved3: u64,
    reserved4: u64,
    reserved5: u64,
};
pub const EventController = extern struct {
    button: u32,
};
pub const EventMouse = extern struct {
    x: f32,
    y: f32,
    button: u32,
    cursor_index: u32,
};
pub const EventScroll = extern struct {
    x_delta: f32,
    y_delta: f32,
    unused: u32,
    viewport_scale: f32,
    cursor_index: u32,
};
pub const EventProcess = extern struct {
    pid: u32,
    old_pid: u32,
    forced: bool,
    connection_lost: bool,
};
pub const EventNotification = extern struct { // which EventType?
    user_value: u64,
    notification_id: u32,
};
pub const EventOverlay = extern struct {
    overlay_handle: u64,
    device_path: u64,
    memory_block_id: u64,
    cursor_index: u32,
};
pub const EventStatus = extern struct { // which EventType?
    status_state: u32,
};
pub const EventKeyboard = extern struct {
    new_input: [8]u8,
    user_value: u64,
    overlay_handle: u64,
};
pub const EventIpd = extern struct {
    ipd_meters: f32,
};
pub const EventChaperone = extern struct {
    previous_universe: u64,
    current_universe: u64,
};
pub const EventPerformanceTest = extern struct {
    fidelity_level: u32,
};

pub const EventTouchPadMove = extern struct {
    finger_down: bool,
    seconds_finger_down: f32,
    x_first: f32,
    y_first: f32,
    x_raw: f32,
    y_raw: f32,
};

pub const EventSeatedZeroPoseReset = extern struct {
    reset_by_system_menu: bool,
};
pub const EventScreenshot = extern struct { // which EventType?
    handle: u32,
    type: u32,
};
pub const EventScreenshotProgress = extern struct {
    progress: f32,
};
pub const EventApplicationLaunch = extern struct { // which EventType?
    pid: u32,
    args_handle: u32,
};

pub const EventEditingCameraSurface = extern struct {
    overlay_handle: u64,
    visual_mode: u32,
};
pub const EventMessageOverlay = extern struct { // which EventType?
    response: u32,
};
pub const PropertyContainerHandle = u64;
pub const EventProperty = extern struct {
    container: PropertyContainerHandle,
    prop: TrackedDeviceProperty,
};
pub const EventHapticVibration = extern struct {
    container_handle: u64,
    component_handle: u64,
    duration_seconds: f32,
    frequency: f32,
    amplitude: f32,
};
pub const WebConsoleHandle = u64;
pub const EventWebConsole = extern struct { //  which EventType?
    web_console_handle: WebConsoleHandle,
};
pub const EventInputBindingLoad = extern struct {
    app_container: PropertyContainerHandle,
    path_message: u64,
    path_url: u64,
    path_controller_type: u64,
};
pub const EventInputActionManifestLoad = extern struct {
    path_app_key: u64,
    path_message: u64,
    path_message_param: u64,
    path_manifest_path: u64,
};
pub const SpatialAnchorHandle = u32;
pub const EventSpatialAnchor = extern struct {
    handle: SpatialAnchorHandle,
};
pub const EventProgressUpdate = extern struct {
    application_property_container: u64,
    path_device: u64,
    path_input_source: u64,
    path_progress_action: u64,
    path_icon: u64,
    progress: f32,
};
pub const ShowUIType = enum(i32) {
    controller_binding = 0,
    manage_trackers = 1,
    pairing = 3,
    settings = 4,
    debug_commands = 5,
    full_controller_binding = 6,
    manage_drivers = 7,
};
pub const EventShowUI = extern struct {
    type: ShowUIType,
};

pub const EventShowDevTools = extern struct {
    browser_identifier: i32,
};
pub const HDCPError = enum(i32) {
    none = 0,
    link_lost = 1,
    tampered = 2,
    device_revoked = 3,
    unknown = 4,
};
pub const EventHDCPError = extern struct {
    code: HDCPError,
};
pub const EventAudioVolumeControl = extern struct {
    volume_level: f32,
};
pub const EventAudioMuteControl = extern struct {
    mute: bool,
};

pub const EventData = extern union {
    reserved: EventReserved,
    controller: EventController,
    mouse: EventMouse,
    scroll: EventScroll,
    process: EventProcess,
    notification: EventNotification,
    overlay: EventOverlay,
    status: EventStatus,
    keyboard: EventKeyboard,
    ipd: EventIpd,
    chaperone: EventChaperone,
    performance_test: EventPerformanceTest,
    touch_pad_move: EventTouchPadMove,
    seated_zero_pose_reset: EventSeatedZeroPoseReset,
    screenshot: EventScreenshot,
    screenshot_progress: EventScreenshotProgress,
    application_launch: EventApplicationLaunch,
    camera_surface: EventEditingCameraSurface,
    message_overlay: EventMessageOverlay,
    property: EventProperty,
    haptic_vibration: EventHapticVibration,
    web_console: EventWebConsole,
    input_binding: EventInputBindingLoad,
    action_manifest: EventInputActionManifestLoad,
    spatial_anchor: EventSpatialAnchor,
    progress_update: EventProgressUpdate,
    show_ui: EventShowUI,
    show_dev_tools: EventShowDevTools,
    hdcp_error: EventHDCPError,
    audio_volume_control: EventAudioVolumeControl,
    audio_mute_control: EventAudioMuteControl,
};
pub const HiddenAreaMeshType = enum(i32) {
    standard = 0,
    inverse = 1,
    line_loop = 2,
    max = 3,
};

pub const ButtonId = enum(i32) {
    system = 0,
    application_menu = 1,
    grip = 2,
    d_pad_left = 3,
    d_pad_up = 4,
    d_pad_right = 5,
    d_pad_down = 6,
    a = 7,
    proximity_sensor = 31,
    axis0 = 32,
    axis1 = 33,
    axis2 = 34,
    axis3 = 35,
    axis4 = 36,
    reserved0 = 50,
    reserved1 = 51,
    max = 64,

    const steam_vr_touchpad = ButtonId.axis0;
    const steam_vr_trigger = ButtonId.axis1;
    const dashboard_back = ButtonId.grip;
    const index_controller_a = ButtonId.grip;
    const index_controller_b = ButtonId.application_menu;
    const index_controller_joy_stick = ButtonId.axis3;
};
pub const ControllerAxisType = enum(i32) {
    none = 0,
    track_pad = 1,
    joystick = 2,
    trigger = 3,
};
pub const FirmwareError = error{
    None,
    Success,
    Fail,
};
pub const FirmwareErrorCode = enum(i32) {
    none = 0,
    success = 1,
    fail = 2,

    pub fn maybe(firmware_error: FirmwareErrorCode) FirmwareError!void {
        return switch (firmware_error) {
            .none, .success => {},
            .fail => FirmwareError.Fail,
        };
    }
};

pub const EventType = enum(i32) {
    none = 0,
    tracked_device_activated = 100,
    tracked_device_deactivated = 101,
    tracked_device_updated = 102,
    tracked_device_user_interaction_started = 103,
    tracked_device_user_interaction_ended = 104,
    ipd_changed = 105,
    enter_standby_mode = 106,
    leave_standby_mode = 107,
    tracked_device_role_changed = 108,
    watchdog_wake_up_requested = 109,
    lens_distortion_changed = 110,
    property_changed = 111,
    wireless_disconnect = 112,
    wireless_reconnect = 113,
    reserved_01 = 114,
    reserved_02 = 115,
    button_press = 200,
    button_unpress = 201,
    button_touch = 202,
    button_untouch = 203,
    modal_cancel = 257,
    mouse_move = 300,
    mouse_button_down = 301,
    mouse_button_up = 302,
    focus_enter = 303,
    focus_leave = 304,
    scroll_discrete = 305,
    touch_pad_move = 306,
    overlay_focus_changed = 307,
    reload_overlays = 308,
    scroll_smooth = 309,
    lock_mouse_position = 310,
    unlock_mouse_position = 311,
    input_focus_captured = 400,
    input_focus_released = 401,
    scene_application_changed = 404,
    input_focus_changed = 406,
    scene_application_using_wrong_graphics_adapter = 408,
    action_binding_reloaded = 409,
    hide_render_models = 410,
    show_render_models = 411,
    scene_application_state_changed = 412,
    scene_app_pipe_disconnected = 413,
    console_opened = 420,
    console_closed = 421,
    overlay_shown = 500,
    overlay_hidden = 501,
    dashboard_activated = 502,
    dashboard_deactivated = 503,
    dashboard_requested = 505,
    reset_dashboard = 506,
    image_loaded = 508,
    show_keyboard = 509,
    hide_keyboard = 510,
    overlay_gamepad_focus_gained = 511,
    overlay_gamepad_focus_lost = 512,
    overlay_shared_texture_changed = 513,
    screenshot_triggered = 516,
    image_failed = 517,
    dashboard_overlay_created = 518,
    switch_gamepad_focus = 519,
    request_screenshot = 520,
    screenshot_taken = 521,
    screenshot_failed = 522,
    submit_screenshot_to_dashboard = 523,
    screenshot_progress_to_dashboard = 524,
    primary_dashboard_device_changed = 525,
    room_view_shown = 526,
    room_view_hidden = 527,
    show_ui = 528,
    show_dev_tools = 529,
    desktop_view_updating = 530,
    desktop_view_ready = 531,
    start_dashboard = 532,
    elevate_prism = 533,
    overlay_closed = 534,
    dashboard_thumb_changed = 535,
    desktop_might_be_visible = 536,
    desktop_might_be_hidden = 537,
    notification_shown = 600,
    notification_hidden = 601,
    notification_begin_interaction = 602,
    notification_destroyed = 603,
    quit = 700,
    process_quit = 701,
    quit_acknowledged = 703,
    driver_requested_quit = 704,
    restart_requested = 705,
    invalidate_swap_texture_sets = 706,
    chaperone_data_has_changed = 800,
    chaperone_universe_has_changed = 801,
    chaperone_temp_data_has_changed = 802,
    chaperone_settings_have_changed = 803,
    seated_zero_pose_reset = 804,
    chaperone_flush_cache = 805,
    chaperone_room_setup_starting = 806,
    chaperone_room_setup_finished = 807,
    standing_zero_pose_reset = 808,
    audio_settings_have_changed = 820,
    background_setting_has_changed = 850,
    camera_settings_have_changed = 851,
    reprojection_setting_has_changed = 852,
    model_skin_settings_have_changed = 853,
    environment_settings_have_changed = 854,
    power_settings_have_changed = 855,
    enable_home_app_settings_have_changed = 856,
    steam_vr_section_setting_changed = 857,
    lighthouse_section_setting_changed = 858,
    null_section_setting_changed = 859,
    user_interface_section_setting_changed = 860,
    notifications_section_setting_changed = 861,
    keyboard_section_setting_changed = 862,
    perf_section_setting_changed = 863,
    dashboard_section_setting_changed = 864,
    web_interface_section_setting_changed = 865,
    trackers_section_setting_changed = 866,
    last_known_section_setting_changed = 867,
    dismissed_warnings_section_setting_changed = 868,
    gpu_speed_section_setting_changed = 869,
    windows_mr_section_setting_changed = 870,
    other_section_setting_changed = 871,
    any_driver_settings_changed = 872,
    status_update = 900,
    web_interface_install_driver_completed = 950,
    mc_image_updated = 1000,
    firmware_update_started = 1100,
    firmware_update_finished = 1101,
    keyboard_closed = 1200,
    keyboard_char_input = 1201,
    keyboard_done = 1202,
    keyboard_opened_global = 1203,
    keyboard_closed_global = 1204,
    application_list_updated = 1303,
    application_mime_type_load = 1304,
    process_connected = 1306,
    process_disconnected = 1307,
    compositor_chaperone_bounds_shown = 1410,
    compositor_chaperone_bounds_hidden = 1411,
    compositor_display_disconnected = 1412,
    compositor_display_reconnected = 1413,
    compositor_hdcp_error = 1414,
    compositor_application_not_responding = 1415,
    compositor_application_resumed = 1416,
    compositor_out_of_video_memory = 1417,
    compositor_display_mode_not_supported = 1418,
    compositor_stage_override_ready = 1419,
    compositor_request_disconnect_reconnect = 1420,
    tracked_camera_start_video_stream = 1500,
    tracked_camera_stop_video_stream = 1501,
    tracked_camera_pause_video_stream = 1502,
    tracked_camera_resume_video_stream = 1503,
    tracked_camera_editing_surface = 1550,
    performance_test_enable_capture = 1600,
    performance_test_disable_capture = 1601,
    performance_test_fidelity_level = 1602,
    message_overlay_closed = 1650,
    message_overlay_close_requested = 1651,
    input_haptic_vibration = 1700,
    input_binding_load_failed = 1701,
    input_binding_load_successful = 1702,
    input_action_manifest_reloaded = 1703,
    input_action_manifest_load_failed = 1704,
    input_progress_update = 1705,
    input_tracker_activated = 1706,
    input_bindings_updated = 1707,
    input_binding_subscription_changed = 1708,
    spatial_anchors_pose_updated = 1800,
    spatial_anchors_descriptor_updated = 1801,
    spatial_anchors_request_pose_update = 1802,
    spatial_anchors_request_descriptor_update = 1803,
    system_report_started = 1900,
    monitor_show_headset_view = 2000,
    monitor_hide_headset_view = 2001,
    audio_set_speakers_volume = 2100,
    audio_set_speakers_mute = 2101,
    audio_set_microphone_volume = 2102,
    audio_set_microphone_mute = 2103,
    vendor_specific_reserved_start = 10000,
    vendor_specific_reserved_end = 19999,

    _,
};

const version = "IVRSystem_022";
pub fn init() common.InitError!Self {
    return .{
        .function_table = try common.getFunctionTable(FunctionTable, version),
    };
}

pub const RenderTargetSize = struct { width: u32, height: u32 };

pub fn getRecommendedRenderTargetSize(self: Self) RenderTargetSize {
    var render_target_size: RenderTargetSize = .{ .width = 0, .height = 0 };
    self.function_table.GetRecommendedRenderTargetSize(&render_target_size.width, &render_target_size.height);
    return render_target_size;
}

pub fn getProjectionMatrix(self: Self, eye: common.Eye, near: f32, far: f32) common.Matrix44 {
    return self.function_table.GetProjectionMatrix(eye, near, far);
}

pub const RawProjection = struct {
    left: f32,
    right: f32,
    top: f32,
    bottom: f32,
};
pub fn getProjectionRaw(self: Self, eye: common.Eye) RawProjection {
    var raw_projection: RawProjection = undefined;
    self.function_table.GetProjectionRaw(eye, &raw_projection.left, &raw_projection.right, &raw_projection.top, &raw_projection.bottom);
    return raw_projection;
}

pub fn computeDistortion(self: Self, eye: common.Eye, u: f32, v: f32) ?DistortionCoordinates {
    var distortion_coordinates: DistortionCoordinates = undefined;
    if (self.function_table.ComputeDistortion(eye, u, v, &distortion_coordinates)) {
        return distortion_coordinates;
    } else {
        return null;
    }
}

pub fn getEyeToHeadTransform(self: Self, eye: common.Eye) common.Matrix34 {
    return self.function_table.GetEyeToHeadTransform(eye);
}

pub const VSyncTiming = struct {
    seconds_since_last_vsync: f32,
    frame_counter: u64,
};

pub fn getTimeSinceLastVsync(self: Self) ?VSyncTiming {
    var timing: VSyncTiming = undefined;
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
pub fn getSortedTrackedDeviceIndicesOfClass(self: Self, tracked_device_class: TrackedDeviceClass, tracked_device_indices: []common.TrackedDeviceIndex, relative_to_tracked_device_index: common.TrackedDeviceIndex) u32 {
    return self.function_table.GetSortedTrackedDeviceIndicesOfClass(tracked_device_class, tracked_device_indices.ptr, @intCast(tracked_device_indices.len), relative_to_tracked_device_index);
}

pub fn getTrackedDeviceActivityLevel(self: Self, device_index: common.TrackedDeviceIndex) DeviceActivityLevel {
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

pub fn getTrackedDeviceClass(self: Self, device_index: common.TrackedDeviceIndex) TrackedDeviceClass {
    return self.function_table.GetTrackedDeviceClass(device_index);
}

pub fn isTrackedDeviceConnected(self: Self, device_index: common.TrackedDeviceIndex) bool {
    return self.function_table.IsTrackedDeviceConnected(device_index);
}

pub fn getTrackedDeviceProperty(self: Self, comptime T: type, device_index: common.TrackedDeviceIndex, property: TrackedDeviceProperty.fromType(T)) TrackedPropertyError!T {
    var property_error: TrackedPropertyErrorCode = undefined;
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

pub fn getTrackedDevicePropertyBool(self: Self, device_index: common.TrackedDeviceIndex, property: TrackedDeviceProperty.Bool) TrackedPropertyError!bool {
    return self.getTrackedDeviceProperty(bool, device_index, property);
}

pub fn getTrackedDevicePropertyF32(self: Self, device_index: common.TrackedDeviceIndex, property: TrackedDeviceProperty.F32) TrackedPropertyError!f32 {
    return self.getTrackedDeviceProperty(f32, device_index, property);
}

pub fn getTrackedDevicePropertyI32(self: Self, device_index: common.TrackedDeviceIndex, property: TrackedDeviceProperty.I32) TrackedPropertyError!i32 {
    return self.getTrackedDeviceProperty(i32, device_index, property);
}

pub fn getTrackedDevicePropertyU64(self: Self, device_index: common.TrackedDeviceIndex, property: TrackedDeviceProperty.U64) TrackedPropertyError!u64 {
    return self.getTrackedDeviceProperty(u64, device_index, property);
}

pub fn getTrackedDevicePropertyMatrix34(self: Self, device_index: common.TrackedDeviceIndex, property: TrackedDeviceProperty.Matrix34) TrackedPropertyError!common.Matrix34 {
    return self.getTrackedDeviceProperty(common.Matrix34, device_index, property);
}

pub const PropertyTypeTagCode = enum(u32) {
    invalid = 0,
    float = 1,
    int32 = 2,
    uint64 = 3,
    bool = 4,
    string = 5,
    @"error" = 6,
    double = 7,
    matrix34 = 20,
    matrix44 = 21,
    vector3 = 22,
    vector4 = 23,
    vector2 = 24,
    quad = 25,
    hidden_area = 30,
    action = 32,
    input_value = 33,
    wildcard = 34,
    haptic_vibration = 35,
    skeleton = 36,
    spatial_anchor_pose = 40,
    json = 41,
    active_action_set = 42,

    pub fn fromType(comptime T: type) PropertyTypeTagCode {
        return switch (T) {
            f32 => .float,
            i32 => .int32,
            u64 => .uint64,
            bool => .bool,
            f64 => .double,
            common.Matrix34 => .matrix34,
            common.Matrix44 => .matrix44,
            Vector2 => .vector2,
            common.Vector3 => .vector3,
            common.Vector4 => .vector4,
            common.Quad => .quad,
            else => @compileError("unsupported type " ++ @typeName(T)),
        };
    }
};

pub fn allocTrackedDevicePropertyArray(self: Self, comptime T: type, allocator: std.mem.Allocator, device_index: common.TrackedDeviceIndex, property: TrackedDeviceProperty.Array.fromType(T)) TrackedPropertyError![]T {
    var property_error: TrackedPropertyErrorCode = undefined;
    const buffer_length = self.function_table.GetArrayTrackedDeviceProperty(device_index, @enumFromInt(@intFromEnum(property)), PropertyTypeTagCode.fromType(T), null, 0, &property_error);
    property_error.maybe() catch |err| switch (err) {
        TrackedPropertyError.BufferTooSmall => {},
        else => return err,
    };
    const buffer = try allocator.alloc(u8, buffer_length);

    if (buffer_length > 0) {
        property_error = undefined;
        _ = self.function_table.GetArrayTrackedDeviceProperty(device_index, @enumFromInt(@intFromEnum(property)), PropertyTypeTagCode.fromType(T), @ptrCast(buffer.ptr), buffer_length, &property_error);
        try property_error.maybe();
    }

    return @alignCast(std.mem.bytesAsSlice(T, buffer));
}

pub fn allocTrackedDevicePropertyArrayF32(self: Self, allocator: std.mem.Allocator, device_index: common.TrackedDeviceIndex, property: TrackedDeviceProperty.Array.F32) TrackedPropertyError![]f32 {
    return self.allocTrackedDevicePropertyArray(f32, allocator, device_index, property);
}

pub fn allocTrackedDevicePropertyArrayI32(self: Self, allocator: std.mem.Allocator, device_index: common.TrackedDeviceIndex, property: TrackedDeviceProperty.Array.I32) TrackedPropertyError![]i32 {
    return self.allocTrackedDevicePropertyArray(i32, allocator, device_index, property);
}

pub fn allocTrackedDevicePropertyArrayVector4(self: Self, allocator: std.mem.Allocator, device_index: common.TrackedDeviceIndex, property: TrackedDeviceProperty.Array.Vector4) TrackedPropertyError![]common.Vector4 {
    return self.allocTrackedDevicePropertyArray(common.Vector4, allocator, device_index, property);
}

pub fn allocTrackedDevicePropertyArrayMatrix34(self: Self, allocator: std.mem.Allocator, device_index: common.TrackedDeviceIndex, property: TrackedDeviceProperty.Array.Matrix34) TrackedPropertyError![]common.Matrix34 {
    return self.allocTrackedDevicePropertyArray(common.Matrix34, allocator, device_index, property);
}

pub fn allocTrackedDevicePropertyString(self: Self, allocator: std.mem.Allocator, device_index: common.TrackedDeviceIndex, property: TrackedDeviceProperty.String) TrackedPropertyError![:0]u8 {
    var property_error: TrackedPropertyErrorCode = undefined;
    const buffer_length = self.function_table.GetStringTrackedDeviceProperty(device_index, property, null, 0, &property_error);
    property_error.maybe() catch |err| switch (err) {
        TrackedPropertyError.BufferTooSmall => {},
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

pub fn getPropErrorNameFromEnum(self: Self, property_error: TrackedPropertyErrorCode) [:0]const u8 {
    return std.mem.span(self.function_table.GetPropErrorNameFromEnum(property_error));
}

pub fn pollNextEvent(self: Self) ?Event {
    var event: Event = undefined;
    if (self.function_table.PollNextEvent(&event, @sizeOf(Event))) {
        return event;
    } else {
        return null;
    }
}
pub const EventWithPose = struct {
    event: Event,
    pose: common.TrackedDevicePose,
};

pub fn pollNextEventWithPose(self: Self, origin: common.TrackingUniverseOrigin) ?EventWithPose {
    var event: Event = undefined;
    var pose: common.TrackedDevicePose = undefined;
    if (self.function_table.PollNextEventWithPose(origin, &event, @sizeOf(Event), &pose)) {
        return .{
            .event = event,
            .pose = pose,
        };
    } else {
        return null;
    }
}

pub fn getEventTypeNameFromEnum(self: Self, event_type: EventType) [:0]const u8 {
    return std.mem.span(self.function_table.GetEventTypeNameFromEnum(event_type));
}

pub fn getHiddenAreaMesh(self: Self, eye: common.Eye, mesh_type: HiddenAreaMeshType) []const Vector2 {
    const mesh = self.function_table.GetHiddenAreaMesh(eye, mesh_type);
    return if (mesh.triangle_count == 0)
        &[_]Vector2{}
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

pub const ControllerStateWithPose = struct {
    controller_state: common.ControllerState,
    pose: common.TrackedDevicePose,
};
pub fn getControllerStateWithPose(self: Self, origin: common.TrackingUniverseOrigin, device_index: common.TrackedDeviceIndex) ?ControllerStateWithPose {
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
pub fn getButtonIdNameFromEnum(self: Self, button_id: ButtonId) [:0]const u8 {
    return std.mem.span(self.function_table.GetButtonIdNameFromEnum(button_id));
}
pub fn getControllerAxisTypeNameFromEnum(self: Self, axis_type: ControllerAxisType) [:0]const u8 {
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
pub fn performFirmwareUpdate(self: Self, device_index: common.TrackedDeviceIndex) FirmwareError!void {
    const firmware_error = self.function_table.PerformFirmwareUpdate(device_index);
    try firmware_error.maybe();
}
pub fn acknowledgeQuitExiting(self: Self) void {
    self.function_table.AcknowledgeQuit_Exiting();
}

pub const FilePaths = struct {
    buffer: [:0]u8,

    pub fn deinit(self: FilePaths, allocator: std.mem.Allocator) void {
        allocator.free(self.buffer);
    }

    pub fn allocPaths(self: FilePaths, allocator: std.mem.Allocator) ![][]const u8 {
        var paths = std.ArrayList([]const u8).init(allocator);
        var it = std.mem.splitScalar(u8, self.buffer, ';');
        while (it.next()) |path| {
            try paths.append(path);
        }
        return paths.toOwnedSlice();
    }
};

pub fn allocAppContainerFilePaths(self: Self, allocator: std.mem.Allocator) !FilePaths {
    const buffer_length = self.function_table.GetAppContainerFilePaths(null, 0);
    if (buffer_length == 0) {
        return FilePaths{ .buffer = try allocator.allocSentinel(u8, 0, 0) };
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    if (buffer_length > 0) {
        _ = self.function_table.GetAppContainerFilePaths(buffer.ptr, buffer_length);
    }
    return FilePaths{ .buffer = buffer };
}

pub fn getRuntimeVersion(self: Self) [:0]const u8 {
    return std.mem.span(self.function_table.GetRuntimeVersion());
}

pub const FunctionTable = extern struct {
    GetRecommendedRenderTargetSize: *const fn (*u32, *u32) callconv(.C) void,
    GetProjectionMatrix: *const fn (common.Eye, f32, f32) callconv(.C) common.Matrix44,
    GetProjectionRaw: *const fn (common.Eye, *f32, *f32, *f32, *f32) callconv(.C) void,
    ComputeDistortion: *const fn (common.Eye, f32, f32, *DistortionCoordinates) callconv(.C) bool,
    GetEyeToHeadTransform: *const fn (common.Eye) callconv(.C) common.Matrix34,
    GetTimeSinceLastVsync: *const fn (*f32, *u64) callconv(.C) bool,
    GetD3D9AdapterIndex: *const fn () callconv(.C) i32,
    GetDXGIOutputInfo: *const fn (*i32) callconv(.C) void,
    GetOutputDevice: *const fn (*u64, common.TextureType, ?*VkInstance) callconv(.C) void,
    IsDisplayOnDesktop: *const fn () callconv(.C) bool,
    SetDisplayVisibility: *const fn (bool) callconv(.C) bool,
    GetDeviceToAbsoluteTrackingPose: *const fn (common.TrackingUniverseOrigin, f32, [*c]common.TrackedDevicePose, u32) callconv(.C) void,
    GetSeatedZeroPoseToStandingAbsoluteTrackingPose: *const fn () callconv(.C) common.Matrix34,
    GetRawZeroPoseToStandingAbsoluteTrackingPose: *const fn () callconv(.C) common.Matrix34,
    GetSortedTrackedDeviceIndicesOfClass: *const fn (TrackedDeviceClass, [*c]common.TrackedDeviceIndex, u32, common.TrackedDeviceIndex) callconv(.C) u32,
    GetTrackedDeviceActivityLevel: *const fn (common.TrackedDeviceIndex) callconv(.C) DeviceActivityLevel,
    ApplyTransform: *const fn (*common.TrackedDevicePose, *common.TrackedDevicePose, *common.Matrix34) callconv(.C) void,
    GetTrackedDeviceIndexForControllerRole: *const fn (common.TrackedControllerRole) callconv(.C) common.TrackedDeviceIndex,
    GetControllerRoleForTrackedDeviceIndex: *const fn (common.TrackedDeviceIndex) callconv(.C) common.TrackedControllerRole,
    GetTrackedDeviceClass: *const fn (common.TrackedDeviceIndex) callconv(.C) TrackedDeviceClass,
    IsTrackedDeviceConnected: *const fn (common.TrackedDeviceIndex) callconv(.C) bool,
    GetBoolTrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, TrackedDeviceProperty, *TrackedPropertyErrorCode) callconv(.C) bool,
    GetFloatTrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, TrackedDeviceProperty, *TrackedPropertyErrorCode) callconv(.C) f32,
    GetInt32TrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, TrackedDeviceProperty, *TrackedPropertyErrorCode) callconv(.C) i32,
    GetUint64TrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, TrackedDeviceProperty, *TrackedPropertyErrorCode) callconv(.C) u64,
    GetMatrix34TrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, TrackedDeviceProperty, *TrackedPropertyErrorCode) callconv(.C) common.Matrix34,
    GetArrayTrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, TrackedDeviceProperty, PropertyTypeTagCode, ?*anyopaque, u32, *TrackedPropertyErrorCode) callconv(.C) u32,
    GetStringTrackedDeviceProperty: *const fn (common.TrackedDeviceIndex, TrackedDeviceProperty.String, [*c]u8, u32, *TrackedPropertyErrorCode) callconv(.C) u32,
    GetPropErrorNameFromEnum: *const fn (TrackedPropertyErrorCode) callconv(.C) [*c]u8,
    PollNextEvent: *const fn (*Event, u32) callconv(.C) bool,
    PollNextEventWithPose: *const fn (common.TrackingUniverseOrigin, *Event, u32, *common.TrackedDevicePose) callconv(.C) bool,
    GetEventTypeNameFromEnum: *const fn (EventType) callconv(.C) [*c]u8,
    GetHiddenAreaMesh: *const fn (common.Eye, HiddenAreaMeshType) callconv(.C) HiddenAreaMesh,
    GetControllerState: *const fn (common.TrackedDeviceIndex, *common.ControllerState, u32) callconv(.C) bool,
    GetControllerStateWithPose: *const fn (common.TrackingUniverseOrigin, common.TrackedDeviceIndex, *common.ControllerState, u32, *common.TrackedDevicePose) callconv(.C) bool,
    TriggerHapticPulse: *const fn (common.TrackedDeviceIndex, u32, c_ushort) callconv(.C) void,
    GetButtonIdNameFromEnum: *const fn (ButtonId) callconv(.C) [*c]u8,
    GetControllerAxisTypeNameFromEnum: *const fn (ControllerAxisType) callconv(.C) [*c]u8,
    IsInputAvailable: *const fn () callconv(.C) bool,
    IsSteamVRDrawingControllers: *const fn () callconv(.C) bool,
    ShouldApplicationPause: *const fn () callconv(.C) bool,
    ShouldApplicationReduceRenderingWork: *const fn () callconv(.C) bool,
    PerformFirmwareUpdate: *const fn (common.TrackedDeviceIndex) callconv(.C) FirmwareErrorCode,
    AcknowledgeQuit_Exiting: *const fn () callconv(.C) void,
    GetAppContainerFilePaths: *const fn ([*c]u8, u32) callconv(.C) u32,
    GetRuntimeVersion: *const fn () callconv(.C) [*c]u8,
};
