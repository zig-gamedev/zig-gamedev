const std = @import("std");

const renderers = @import("renderers.zig");
const d3d12 = renderers.d3d12;
const vulkan = renderers.vulkan;

const root = @This();

pub const InitError = error{
    Unknown,
    InitInstallationNotFound,
    InitInstallationCorrupt,
    InitVRClientDLLNotFound,
    InitFileNotFound,
    InitFactoryNotFound,
    InitInterfaceNotFound,
    InitInvalidInterface,
    InitUserConfigDirectoryInvalid,
    InitHmdNotFound,
    InitNotInitialized,
    InitPathRegistryNotFound,
    InitNoConfigPath,
    InitNoLogPath,
    InitPathRegistryNotWritable,
    InitAppInfoInitFailed,
    InitRetry,
    InitInitCanceledByUser,
    InitAnotherAppLaunching,
    InitSettingsInitFailed,
    InitShuttingDown,
    InitTooManyObjects,
    InitNoServerForBackgroundApp,
    InitNotSupportedWithCompositor,
    InitNotAvailableToUtilityApps,
    InitInternal,
    InitHmdDriverIdIsNone,
    InitHmdNotFoundPresenceFailed,
    InitVRMonitorNotFound,
    InitVRMonitorStartupFailed,
    InitLowPowerWatchdogNotSupported,
    InitInvalidApplicationType,
    InitNotAvailableToWatchdogApps,
    InitWatchdogDisabledInSettings,
    InitVRDashboardNotFound,
    InitVRDashboardStartupFailed,
    InitVRHomeNotFound,
    InitVRHomeStartupFailed,
    InitRebootingBusy,
    InitFirmwareUpdateBusy,
    InitFirmwareRecoveryBusy,
    InitUSBServiceBusy,
    InitVRWebHelperStartupFailed,
    InitTrackerManagerInitFailed,
    InitAlreadyRunning,
    InitFailedForVrMonitor,
    InitPropertyManagerInitFailed,
    InitWebServerFailed,
    InitIllegalTypeTransition,
    InitMismatchedRuntimes,
    InitInvalidProcessId,
    InitVRServiceStartupFailed,
    InitPrismNeedsNewDrivers,
    InitPrismStartupTimedOut,
    InitCouldNotStartPrism,
    InitPrismClientInitFailed,
    InitPrismClientStartFailed,
    InitPrismExitedUnexpectedly,
    InitBadLuid,
    InitNoServerForAppContainer,
    InitDuplicateBootstrapper,
    InitVRDashboardServicePending,
    InitVRDashboardServiceTimeout,
    InitVRDashboardServiceStopped,
    InitVRDashboardAlreadyStarted,
    InitVRDashboardCopyFailed,
    InitVRDashboardTokenFailure,
    InitVRDashboardEnvironmentFailure,
    InitVRDashboardPathFailure,
    DriverFailed,
    DriverUnknown,
    DriverHmdUnknown,
    DriverNotLoaded,
    DriverRuntimeOutOfDate,
    DriverHmdInUse,
    DriverNotCalibrated,
    DriverCalibrationInvalid,
    DriverHmdDisplayNotFound,
    DriverTrackedDeviceInterfaceUnknown,
    DriverHmdDriverIdOutOfBounds,
    DriverHmdDisplayMirrored,
    DriverHmdDisplayNotFoundLaptop,
    DriverPeerDriverNotInstalled,
    DriverWirelessHmdNotConnected,
    IPCServerInitFailed,
    IPCConnectFailed,
    IPCSharedStateInitFailed,
    IPCCompositorInitFailed,
    IPCMutexInitFailed,
    IPCFailed,
    IPCCompositorConnectFailed,
    IPCCompositorInvalidConnectResponse,
    IPCConnectFailedAfterMultipleAttempts,
    IPCConnectFailedAfterTargetExited,
    IPCNamespaceUnavailable,
    CompositorFailed,
    CompositorD3D11HardwareRequired,
    CompositorFirmwareRequiresUpdate,
    CompositorOverlayInitFailed,
    CompositorScreenshotsInitFailed,
    CompositorUnableToCreateDevice,
    CompositorSharedStateIsNull,
    CompositorNotificationManagerIsNull,
    CompositorResourceManagerClientIsNull,
    CompositorMessageOverlaySharedStateInitFailure,
    CompositorPropertiesInterfaceIsNull,
    CompositorCreateFullscreenWindowFailed,
    CompositorSettingsInterfaceIsNull,
    CompositorFailedToShowWindow,
    CompositorDistortInterfaceIsNull,
    CompositorDisplayFrequencyFailure,
    CompositorRendererInitializationFailed,
    CompositorDXGIFactoryInterfaceIsNull,
    CompositorDXGIFactoryCreateFailed,
    CompositorDXGIFactoryQueryFailed,
    CompositorInvalidAdapterDesktop,
    CompositorInvalidHmdAttachment,
    CompositorInvalidOutputDesktop,
    CompositorInvalidDeviceProvided,
    CompositorD3D11RendererInitializationFailed,
    CompositorFailedToFindDisplayMode,
    CompositorFailedToCreateSwapChain,
    CompositorFailedToGetBackBuffer,
    CompositorFailedToCreateRenderTarget,
    CompositorFailedToCreateDXGI2SwapChain,
    CompositorFailedtoGetDXGI2BackBuffer,
    CompositorFailedToCreateDXGI2RenderTarget,
    CompositorFailedToGetDXGIDeviceInterface,
    CompositorSelectDisplayMode,
    CompositorFailedToCreateNvAPIRenderTargets,
    CompositorNvAPISetDisplayMode,
    CompositorFailedToCreateDirectModeDisplay,
    CompositorInvalidHmdPropertyContainer,
    CompositorUpdateDisplayFrequency,
    CompositorCreateRasterizerState,
    CompositorCreateWireframeRasterizerState,
    CompositorCreateSamplerState,
    CompositorCreateClampToBorderSamplerState,
    CompositorCreateAnisoSamplerState,
    CompositorCreateOverlaySamplerState,
    CompositorCreatePanoramaSamplerState,
    CompositorCreateFontSamplerState,
    CompositorCreateNoBlendState,
    CompositorCreateBlendState,
    CompositorCreateAlphaBlendState,
    CompositorCreateBlendStateMaskR,
    CompositorCreateBlendStateMaskG,
    CompositorCreateBlendStateMaskB,
    CompositorCreateDepthStencilState,
    CompositorCreateDepthStencilStateNoWrite,
    CompositorCreateDepthStencilStateNoDepth,
    CompositorCreateFlushTexture,
    CompositorCreateDistortionSurfaces,
    CompositorCreateConstantBuffer,
    CompositorCreateHmdPoseConstantBuffer,
    CompositorCreateHmdPoseStagingConstantBuffer,
    CompositorCreateSharedFrameInfoConstantBuffer,
    CompositorCreateOverlayConstantBuffer,
    CompositorCreateSceneTextureIndexConstantBuffer,
    CompositorCreateReadableSceneTextureIndexConstantBuffer,
    CompositorCreateLayerGraphicsTextureIndexConstantBuffer,
    CompositorCreateLayerComputeTextureIndexConstantBuffer,
    CompositorCreateLayerComputeSceneTextureIndexConstantBuffer,
    CompositorCreateComputeHmdPoseConstantBuffer,
    CompositorCreateGeomConstantBuffer,
    CompositorCreatePanelMaskConstantBuffer,
    CompositorCreatePixelSimUBO,
    CompositorCreateMSAARenderTextures,
    CompositorCreateResolveRenderTextures,
    CompositorCreateComputeResolveRenderTextures,
    CompositorCreateDriverDirectModeResolveTextures,
    CompositorOpenDriverDirectModeResolveTextures,
    CompositorCreateFallbackSyncTexture,
    CompositorShareFallbackSyncTexture,
    CompositorCreateOverlayIndexBuffer,
    CompositorCreateOverlayVertexBuffer,
    CompositorCreateTextVertexBuffer,
    CompositorCreateTextIndexBuffer,
    CompositorCreateMirrorTextures,
    CompositorCreateLastFrameRenderTexture,
    CompositorCreateMirrorOverlay,
    CompositorFailedToCreateVirtualDisplayBackbuffer,
    CompositorDisplayModeNotSupported,
    CompositorCreateOverlayInvalidCall,
    CompositorCreateOverlayAlreadyInitialized,
    CompositorFailedToCreateMailbox,
    CompositorWindowInterfaceIsNull,
    CompositorSystemLayerCreateInstance,
    CompositorSystemLayerCreateSession,
    CompositorCreateInverseDistortUVs,
    CompositorCreateBackbufferDepth,
    CompositorCannotDRMLeaseDisplay,
    CompositorCannotConnectToDisplayServer,
    CompositorGnomeNoDRMLeasing,
    CompositorFailedToInitializeEncoder,
    CompositorCreateBlurTexture,
    VendorSpecificUnableToConnectToOculusRuntime,
    VendorSpecificWindowsNotInDevMode,
    VendorSpecificOculusLinkNotEnabled,
    VendorSpecificHmdFoundCantOpenDevice,
    VendorSpecificHmdFoundUnableToRequestConfigStart,
    VendorSpecificHmdFoundNoStoredConfig,
    VendorSpecificHmdFoundConfigTooBig,
    VendorSpecificHmdFoundConfigTooSmall,
    VendorSpecificHmdFoundUnableToInitZLib,
    VendorSpecificHmdFoundCantReadFirmwareVersion,
    VendorSpecificHmdFoundUnableToSendUserDataStart,
    VendorSpecificHmdFoundUnableToGetUserDataStart,
    VendorSpecificHmdFoundUnableToGetUserDataNext,
    VendorSpecificHmdFoundUserDataAddressRange,
    VendorSpecificHmdFoundUserDataError,
    VendorSpecificHmdFoundConfigFailedSanityCheck,
    VendorSpecificOculusRuntimeBadInstall,
    VendorSpecificHmdFoundUnexpectedConfiguration1,
    SteamInstallationNotFound,
    LastError,
};

pub const InitErrorCode = enum(i32) {
    none = 0,
    unknown = 1,
    init_installation_not_found = 100,
    init_installation_corrupt = 101,
    init_vr_client_dll_not_found = 102,
    init_file_not_found = 103,
    init_factory_not_found = 104,
    init_interface_not_found = 105,
    init_invalid_interface = 106,
    init_user_config_directory_invalid = 107,
    init_hmd_not_found = 108,
    init_not_initialized = 109,
    init_path_registry_not_found = 110,
    init_no_config_path = 111,
    init_no_log_path = 112,
    init_path_registry_not_writable = 113,
    init_app_info_init_failed = 114,
    init_retry = 115,
    init_init_canceled_by_user = 116,
    init_another_app_launching = 117,
    init_settings_init_failed = 118,
    init_shutting_down = 119,
    init_too_many_objects = 120,
    init_no_server_for_background_app = 121,
    init_not_supported_with_compositor = 122,
    init_not_available_to_utility_apps = 123,
    init_internal = 124,
    init_hmd_driver_id_is_none = 125,
    init_hmd_not_found_presence_failed = 126,
    init_vr_monitor_not_found = 127,
    init_vr_monitor_startup_failed = 128,
    init_low_power_watchdog_not_supported = 129,
    init_invalid_application_type = 130,
    init_not_available_to_watchdog_apps = 131,
    init_watchdog_disabled_in_settings = 132,
    init_vr_dashboard_not_found = 133,
    init_vr_dashboard_startup_failed = 134,
    init_vr_home_not_found = 135,
    init_vr_home_startup_failed = 136,
    init_rebooting_busy = 137,
    init_firmware_update_busy = 138,
    init_firmware_recovery_busy = 139,
    init_usb_service_busy = 140,
    init_vr_web_helper_startup_failed = 141,
    init_tracker_manager_init_failed = 142,
    init_already_running = 143,
    init_failed_for_vr_monitor = 144,
    init_property_manager_init_failed = 145,
    init_web_server_failed = 146,
    init_illegal_type_transition = 147,
    init_mismatched_runtimes = 148,
    init_invalid_process_id = 149,
    init_vr_service_startup_failed = 150,
    init_prism_needs_new_drivers = 151,
    init_prism_startup_timed_out = 152,
    init_could_not_start_prism = 153,
    init_prism_client_init_failed = 154,
    init_prism_client_start_failed = 155,
    init_prism_exited_unexpectedly = 156,
    init_bad_luid = 157,
    init_no_server_for_app_container = 158,
    init_duplicate_bootstrapper = 159,
    init_vr_dashboard_service_pending = 160,
    init_vr_dashboard_service_timeout = 161,
    init_vr_dashboard_service_stopped = 162,
    init_vr_dashboard_already_started = 163,
    init_vr_dashboard_copy_failed = 164,
    init_vr_dashboard_token_failure = 165,
    init_vr_dashboard_environment_failure = 166,
    init_vr_dashboard_path_failure = 167,
    driver_failed = 200,
    driver_unknown = 201,
    driver_hmd_unknown = 202,
    driver_not_loaded = 203,
    driver_runtime_out_of_date = 204,
    driver_hmd_in_use = 205,
    driver_not_calibrated = 206,
    driver_calibration_invalid = 207,
    driver_hmd_display_not_found = 208,
    driver_tracked_device_interface_unknown = 209,
    driver_hmd_driver_id_out_of_bounds = 211,
    driver_hmd_display_mirrored = 212,
    driver_hmd_display_not_found_laptop = 213,
    driver_peer_driver_not_installed = 214,
    driver_wireless_hmd_not_connected = 215,
    ipc_server_init_failed = 300,
    ipc_connect_failed = 301,
    ipc_shared_state_init_failed = 302,
    ipc_compositor_init_failed = 303,
    ipc_mutex_init_failed = 304,
    ipc_failed = 305,
    ipc_compositor_connect_failed = 306,
    ipc_compositor_invalid_connect_response = 307,
    ipc_connect_failed_after_multiple_attempts = 308,
    ipc_connect_failed_after_target_exited = 309,
    ipc_namespace_unavailable = 310,
    compositor_failed = 400,
    compositor_d3d11_hardware_required = 401,
    compositor_firmware_requires_update = 402,
    compositor_overlay_init_failed = 403,
    compositor_screenshots_init_failed = 404,
    compositor_unable_to_create_device = 405,
    compositor_shared_state_is_null = 406,
    compositor_notification_manager_is_null = 407,
    compositor_resource_manager_client_is_null = 408,
    compositor_message_overlay_shared_state_init_failure = 409,
    compositor_properties_interface_is_null = 410,
    compositor_create_fullscreen_window_failed = 411,
    compositor_settings_interface_is_null = 412,
    compositor_failed_to_show_window = 413,
    compositor_distort_interface_is_null = 414,
    compositor_display_frequency_failure = 415,
    compositor_renderer_initialization_failed = 416,
    compositor_dxgi_factory_interface_is_null = 417,
    compositor_dxgi_factory_create_failed = 418,
    compositor_dxgi_factory_query_failed = 419,
    compositor_invalid_adapter_desktop = 420,
    compositor_invalid_hmd_attachment = 421,
    compositor_invalid_output_desktop = 422,
    compositor_invalid_device_provided = 423,
    compositor_d3d11_renderer_initialization_failed = 424,
    compositor_failed_to_find_display_mode = 425,
    compositor_failed_to_create_swap_chain = 426,
    compositor_failed_to_get_back_buffer = 427,
    compositor_failed_to_create_render_target = 428,
    compositor_failed_to_create_dxgi2_swap_chain = 429,
    compositor_failedto_get_dxgi2_back_buffer = 430,
    compositor_failed_to_create_dxgi2_render_target = 431,
    compositor_failed_to_get_dxgi_device_interface = 432,
    compositor_select_display_mode = 433,
    compositor_failed_to_create_nv_api_render_targets = 434,
    compositor_nv_api_set_display_mode = 435,
    compositor_failed_to_create_direct_mode_display = 436,
    compositor_invalid_hmd_property_container = 437,
    compositor_update_display_frequency = 438,
    compositor_create_rasterizer_state = 439,
    compositor_create_wireframe_rasterizer_state = 440,
    compositor_create_sampler_state = 441,
    compositor_create_clamp_to_border_sampler_state = 442,
    compositor_create_aniso_sampler_state = 443,
    compositor_create_overlay_sampler_state = 444,
    compositor_create_panorama_sampler_state = 445,
    compositor_create_font_sampler_state = 446,
    compositor_create_no_blend_state = 447,
    compositor_create_blend_state = 448,
    compositor_create_alpha_blend_state = 449,
    compositor_create_blend_state_mask_r = 450,
    compositor_create_blend_state_mask_g = 451,
    compositor_create_blend_state_mask_b = 452,
    compositor_create_depth_stencil_state = 453,
    compositor_create_depth_stencil_state_no_write = 454,
    compositor_create_depth_stencil_state_no_depth = 455,
    compositor_create_flush_texture = 456,
    compositor_create_distortion_surfaces = 457,
    compositor_create_constant_buffer = 458,
    compositor_create_hmd_pose_constant_buffer = 459,
    compositor_create_hmd_pose_staging_constant_buffer = 460,
    compositor_create_shared_frame_info_constant_buffer = 461,
    compositor_create_overlay_constant_buffer = 462,
    compositor_create_scene_texture_index_constant_buffer = 463,
    compositor_create_readable_scene_texture_index_constant_buffer = 464,
    compositor_create_layer_graphics_texture_index_constant_buffer = 465,
    compositor_create_layer_compute_texture_index_constant_buffer = 466,
    compositor_create_layer_compute_scene_texture_index_constant_buffer = 467,
    compositor_create_compute_hmd_pose_constant_buffer = 468,
    compositor_create_geom_constant_buffer = 469,
    compositor_create_panel_mask_constant_buffer = 470,
    compositor_create_pixel_sim_ubo = 471,
    compositor_create_msaa_render_textures = 472,
    compositor_create_resolve_render_textures = 473,
    compositor_create_compute_resolve_render_textures = 474,
    compositor_create_driver_direct_mode_resolve_textures = 475,
    compositor_open_driver_direct_mode_resolve_textures = 476,
    compositor_create_fallback_sync_texture = 477,
    compositor_share_fallback_sync_texture = 478,
    compositor_create_overlay_index_buffer = 479,
    compositor_create_overlay_vertex_buffer = 480,
    compositor_create_text_vertex_buffer = 481,
    compositor_create_text_index_buffer = 482,
    compositor_create_mirror_textures = 483,
    compositor_create_last_frame_render_texture = 484,
    compositor_create_mirror_overlay = 485,
    compositor_failed_to_create_virtual_display_backbuffer = 486,
    compositor_display_mode_not_supported = 487,
    compositor_create_overlay_invalid_call = 488,
    compositor_create_overlay_already_initialized = 489,
    compositor_failed_to_create_mailbox = 490,
    compositor_window_interface_is_null = 491,
    compositor_system_layer_create_instance = 492,
    compositor_system_layer_create_session = 493,
    compositor_create_inverse_distort_u_vs = 494,
    compositor_create_backbuffer_depth = 495,
    compositor_cannot_drm_lease_display = 496,
    compositor_cannot_connect_to_display_server = 497,
    compositor_gnome_no_drm_leasing = 498,
    compositor_failed_to_initialize_encoder = 499,
    compositor_create_blur_texture = 500,
    vendor_specific_unable_to_connect_to_oculus_runtime = 1000,
    vendor_specific_windows_not_in_dev_mode = 1001,
    vendor_specific_oculus_link_not_enabled = 1002,
    vendor_specific_hmd_found_cant_open_device = 1101,
    vendor_specific_hmd_found_unable_to_request_config_start = 1102,
    vendor_specific_hmd_found_no_stored_config = 1103,
    vendor_specific_hmd_found_config_too_big = 1104,
    vendor_specific_hmd_found_config_too_small = 1105,
    vendor_specific_hmd_found_unable_to_init_z_lib = 1106,
    vendor_specific_hmd_found_cant_read_firmware_version = 1107,
    vendor_specific_hmd_found_unable_to_send_user_data_start = 1108,
    vendor_specific_hmd_found_unable_to_get_user_data_start = 1109,
    vendor_specific_hmd_found_unable_to_get_user_data_next = 1110,
    vendor_specific_hmd_found_user_data_address_range = 1111,
    vendor_specific_hmd_found_user_data_error = 1112,
    vendor_specific_hmd_found_config_failed_sanity_check = 1113,
    vendor_specific_oculus_runtime_bad_install = 1114,
    vendor_specific_hmd_found_unexpected_configuration_1 = 1115,
    steam_installation_not_found = 2000,
    last_error = 2001,

    pub fn fromError(err: InitError) InitErrorCode {
        return switch (err) {
            inline else => |e| comptime for (std.meta.tags(InitErrorCode)) |tag| {
                @setEvalBranchQuota(50000);
                if (tag.maybe() == e) break tag;
            } else unreachable,
        };
    }

    pub fn maybe(init_error: InitErrorCode) InitError!void {
        return switch (init_error) {
            .none => {},
            .unknown => InitError.Unknown,
            .init_installation_not_found => InitError.InitInstallationNotFound,
            .init_installation_corrupt => InitError.InitInstallationCorrupt,
            .init_vr_client_dll_not_found => InitError.InitVRClientDLLNotFound,
            .init_file_not_found => InitError.InitFileNotFound,
            .init_factory_not_found => InitError.InitFactoryNotFound,
            .init_interface_not_found => InitError.InitInterfaceNotFound,
            .init_invalid_interface => InitError.InitInvalidInterface,
            .init_user_config_directory_invalid => InitError.InitUserConfigDirectoryInvalid,
            .init_hmd_not_found => InitError.InitHmdNotFound,
            .init_not_initialized => InitError.InitNotInitialized,
            .init_path_registry_not_found => InitError.InitPathRegistryNotFound,
            .init_no_config_path => InitError.InitNoConfigPath,
            .init_no_log_path => InitError.InitNoLogPath,
            .init_path_registry_not_writable => InitError.InitPathRegistryNotWritable,
            .init_app_info_init_failed => InitError.InitAppInfoInitFailed,
            .init_retry => InitError.InitRetry,
            .init_init_canceled_by_user => InitError.InitInitCanceledByUser,
            .init_another_app_launching => InitError.InitAnotherAppLaunching,
            .init_settings_init_failed => InitError.InitSettingsInitFailed,
            .init_shutting_down => InitError.InitShuttingDown,
            .init_too_many_objects => InitError.InitTooManyObjects,
            .init_no_server_for_background_app => InitError.InitNoServerForBackgroundApp,
            .init_not_supported_with_compositor => InitError.InitNotSupportedWithCompositor,
            .init_not_available_to_utility_apps => InitError.InitNotAvailableToUtilityApps,
            .init_internal => InitError.InitInternal,
            .init_hmd_driver_id_is_none => InitError.InitHmdDriverIdIsNone,
            .init_hmd_not_found_presence_failed => InitError.InitHmdNotFoundPresenceFailed,
            .init_vr_monitor_not_found => InitError.InitVRMonitorNotFound,
            .init_vr_monitor_startup_failed => InitError.InitVRMonitorStartupFailed,
            .init_low_power_watchdog_not_supported => InitError.InitLowPowerWatchdogNotSupported,
            .init_invalid_application_type => InitError.InitInvalidApplicationType,
            .init_not_available_to_watchdog_apps => InitError.InitNotAvailableToWatchdogApps,
            .init_watchdog_disabled_in_settings => InitError.InitWatchdogDisabledInSettings,
            .init_vr_dashboard_not_found => InitError.InitVRDashboardNotFound,
            .init_vr_dashboard_startup_failed => InitError.InitVRDashboardStartupFailed,
            .init_vr_home_not_found => InitError.InitVRHomeNotFound,
            .init_vr_home_startup_failed => InitError.InitVRHomeStartupFailed,
            .init_rebooting_busy => InitError.InitRebootingBusy,
            .init_firmware_update_busy => InitError.InitFirmwareUpdateBusy,
            .init_firmware_recovery_busy => InitError.InitFirmwareRecoveryBusy,
            .init_usb_service_busy => InitError.InitUSBServiceBusy,
            .init_vr_web_helper_startup_failed => InitError.InitVRWebHelperStartupFailed,
            .init_tracker_manager_init_failed => InitError.InitTrackerManagerInitFailed,
            .init_already_running => InitError.InitAlreadyRunning,
            .init_failed_for_vr_monitor => InitError.InitFailedForVrMonitor,
            .init_property_manager_init_failed => InitError.InitPropertyManagerInitFailed,
            .init_web_server_failed => InitError.InitWebServerFailed,
            .init_illegal_type_transition => InitError.InitIllegalTypeTransition,
            .init_mismatched_runtimes => InitError.InitMismatchedRuntimes,
            .init_invalid_process_id => InitError.InitInvalidProcessId,
            .init_vr_service_startup_failed => InitError.InitVRServiceStartupFailed,
            .init_prism_needs_new_drivers => InitError.InitPrismNeedsNewDrivers,
            .init_prism_startup_timed_out => InitError.InitPrismStartupTimedOut,
            .init_could_not_start_prism => InitError.InitCouldNotStartPrism,
            .init_prism_client_init_failed => InitError.InitPrismClientInitFailed,
            .init_prism_client_start_failed => InitError.InitPrismClientStartFailed,
            .init_prism_exited_unexpectedly => InitError.InitPrismExitedUnexpectedly,
            .init_bad_luid => InitError.InitBadLuid,
            .init_no_server_for_app_container => InitError.InitNoServerForAppContainer,
            .init_duplicate_bootstrapper => InitError.InitDuplicateBootstrapper,
            .init_vr_dashboard_service_pending => InitError.InitVRDashboardServicePending,
            .init_vr_dashboard_service_timeout => InitError.InitVRDashboardServiceTimeout,
            .init_vr_dashboard_service_stopped => InitError.InitVRDashboardServiceStopped,
            .init_vr_dashboard_already_started => InitError.InitVRDashboardAlreadyStarted,
            .init_vr_dashboard_copy_failed => InitError.InitVRDashboardCopyFailed,
            .init_vr_dashboard_token_failure => InitError.InitVRDashboardTokenFailure,
            .init_vr_dashboard_environment_failure => InitError.InitVRDashboardEnvironmentFailure,
            .init_vr_dashboard_path_failure => InitError.InitVRDashboardPathFailure,
            .driver_failed => InitError.DriverFailed,
            .driver_unknown => InitError.DriverUnknown,
            .driver_hmd_unknown => InitError.DriverHmdUnknown,
            .driver_not_loaded => InitError.DriverNotLoaded,
            .driver_runtime_out_of_date => InitError.DriverRuntimeOutOfDate,
            .driver_hmd_in_use => InitError.DriverHmdInUse,
            .driver_not_calibrated => InitError.DriverNotCalibrated,
            .driver_calibration_invalid => InitError.DriverCalibrationInvalid,
            .driver_hmd_display_not_found => InitError.DriverHmdDisplayNotFound,
            .driver_tracked_device_interface_unknown => InitError.DriverTrackedDeviceInterfaceUnknown,
            .driver_hmd_driver_id_out_of_bounds => InitError.DriverHmdDriverIdOutOfBounds,
            .driver_hmd_display_mirrored => InitError.DriverHmdDisplayMirrored,
            .driver_hmd_display_not_found_laptop => InitError.DriverHmdDisplayNotFoundLaptop,
            .driver_peer_driver_not_installed => InitError.DriverPeerDriverNotInstalled,
            .driver_wireless_hmd_not_connected => InitError.DriverWirelessHmdNotConnected,
            .ipc_server_init_failed => InitError.IPCServerInitFailed,
            .ipc_connect_failed => InitError.IPCConnectFailed,
            .ipc_shared_state_init_failed => InitError.IPCSharedStateInitFailed,
            .ipc_compositor_init_failed => InitError.IPCCompositorInitFailed,
            .ipc_mutex_init_failed => InitError.IPCMutexInitFailed,
            .ipc_failed => InitError.IPCFailed,
            .ipc_compositor_connect_failed => InitError.IPCCompositorConnectFailed,
            .ipc_compositor_invalid_connect_response => InitError.IPCCompositorInvalidConnectResponse,
            .ipc_connect_failed_after_multiple_attempts => InitError.IPCConnectFailedAfterMultipleAttempts,
            .ipc_connect_failed_after_target_exited => InitError.IPCConnectFailedAfterTargetExited,
            .ipc_namespace_unavailable => InitError.IPCNamespaceUnavailable,
            .compositor_failed => InitError.CompositorFailed,
            .compositor_d3d11_hardware_required => InitError.CompositorD3D11HardwareRequired,
            .compositor_firmware_requires_update => InitError.CompositorFirmwareRequiresUpdate,
            .compositor_overlay_init_failed => InitError.CompositorOverlayInitFailed,
            .compositor_screenshots_init_failed => InitError.CompositorScreenshotsInitFailed,
            .compositor_unable_to_create_device => InitError.CompositorUnableToCreateDevice,
            .compositor_shared_state_is_null => InitError.CompositorSharedStateIsNull,
            .compositor_notification_manager_is_null => InitError.CompositorNotificationManagerIsNull,
            .compositor_resource_manager_client_is_null => InitError.CompositorResourceManagerClientIsNull,
            .compositor_message_overlay_shared_state_init_failure => InitError.CompositorMessageOverlaySharedStateInitFailure,
            .compositor_properties_interface_is_null => InitError.CompositorPropertiesInterfaceIsNull,
            .compositor_create_fullscreen_window_failed => InitError.CompositorCreateFullscreenWindowFailed,
            .compositor_settings_interface_is_null => InitError.CompositorSettingsInterfaceIsNull,
            .compositor_failed_to_show_window => InitError.CompositorFailedToShowWindow,
            .compositor_distort_interface_is_null => InitError.CompositorDistortInterfaceIsNull,
            .compositor_display_frequency_failure => InitError.CompositorDisplayFrequencyFailure,
            .compositor_renderer_initialization_failed => InitError.CompositorRendererInitializationFailed,
            .compositor_dxgi_factory_interface_is_null => InitError.CompositorDXGIFactoryInterfaceIsNull,
            .compositor_dxgi_factory_create_failed => InitError.CompositorDXGIFactoryCreateFailed,
            .compositor_dxgi_factory_query_failed => InitError.CompositorDXGIFactoryQueryFailed,
            .compositor_invalid_adapter_desktop => InitError.CompositorInvalidAdapterDesktop,
            .compositor_invalid_hmd_attachment => InitError.CompositorInvalidHmdAttachment,
            .compositor_invalid_output_desktop => InitError.CompositorInvalidOutputDesktop,
            .compositor_invalid_device_provided => InitError.CompositorInvalidDeviceProvided,
            .compositor_d3d11_renderer_initialization_failed => InitError.CompositorD3D11RendererInitializationFailed,
            .compositor_failed_to_find_display_mode => InitError.CompositorFailedToFindDisplayMode,
            .compositor_failed_to_create_swap_chain => InitError.CompositorFailedToCreateSwapChain,
            .compositor_failed_to_get_back_buffer => InitError.CompositorFailedToGetBackBuffer,
            .compositor_failed_to_create_render_target => InitError.CompositorFailedToCreateRenderTarget,
            .compositor_failed_to_create_dxgi2_swap_chain => InitError.CompositorFailedToCreateDXGI2SwapChain,
            .compositor_failedto_get_dxgi2_back_buffer => InitError.CompositorFailedtoGetDXGI2BackBuffer,
            .compositor_failed_to_create_dxgi2_render_target => InitError.CompositorFailedToCreateDXGI2RenderTarget,
            .compositor_failed_to_get_dxgi_device_interface => InitError.CompositorFailedToGetDXGIDeviceInterface,
            .compositor_select_display_mode => InitError.CompositorSelectDisplayMode,
            .compositor_failed_to_create_nv_api_render_targets => InitError.CompositorFailedToCreateNvAPIRenderTargets,
            .compositor_nv_api_set_display_mode => InitError.CompositorNvAPISetDisplayMode,
            .compositor_failed_to_create_direct_mode_display => InitError.CompositorFailedToCreateDirectModeDisplay,
            .compositor_invalid_hmd_property_container => InitError.CompositorInvalidHmdPropertyContainer,
            .compositor_update_display_frequency => InitError.CompositorUpdateDisplayFrequency,
            .compositor_create_rasterizer_state => InitError.CompositorCreateRasterizerState,
            .compositor_create_wireframe_rasterizer_state => InitError.CompositorCreateWireframeRasterizerState,
            .compositor_create_sampler_state => InitError.CompositorCreateSamplerState,
            .compositor_create_clamp_to_border_sampler_state => InitError.CompositorCreateClampToBorderSamplerState,
            .compositor_create_aniso_sampler_state => InitError.CompositorCreateAnisoSamplerState,
            .compositor_create_overlay_sampler_state => InitError.CompositorCreateOverlaySamplerState,
            .compositor_create_panorama_sampler_state => InitError.CompositorCreatePanoramaSamplerState,
            .compositor_create_font_sampler_state => InitError.CompositorCreateFontSamplerState,
            .compositor_create_no_blend_state => InitError.CompositorCreateNoBlendState,
            .compositor_create_blend_state => InitError.CompositorCreateBlendState,
            .compositor_create_alpha_blend_state => InitError.CompositorCreateAlphaBlendState,
            .compositor_create_blend_state_mask_r => InitError.CompositorCreateBlendStateMaskR,
            .compositor_create_blend_state_mask_g => InitError.CompositorCreateBlendStateMaskG,
            .compositor_create_blend_state_mask_b => InitError.CompositorCreateBlendStateMaskB,
            .compositor_create_depth_stencil_state => InitError.CompositorCreateDepthStencilState,
            .compositor_create_depth_stencil_state_no_write => InitError.CompositorCreateDepthStencilStateNoWrite,
            .compositor_create_depth_stencil_state_no_depth => InitError.CompositorCreateDepthStencilStateNoDepth,
            .compositor_create_flush_texture => InitError.CompositorCreateFlushTexture,
            .compositor_create_distortion_surfaces => InitError.CompositorCreateDistortionSurfaces,
            .compositor_create_constant_buffer => InitError.CompositorCreateConstantBuffer,
            .compositor_create_hmd_pose_constant_buffer => InitError.CompositorCreateHmdPoseConstantBuffer,
            .compositor_create_hmd_pose_staging_constant_buffer => InitError.CompositorCreateHmdPoseStagingConstantBuffer,
            .compositor_create_shared_frame_info_constant_buffer => InitError.CompositorCreateSharedFrameInfoConstantBuffer,
            .compositor_create_overlay_constant_buffer => InitError.CompositorCreateOverlayConstantBuffer,
            .compositor_create_scene_texture_index_constant_buffer => InitError.CompositorCreateSceneTextureIndexConstantBuffer,
            .compositor_create_readable_scene_texture_index_constant_buffer => InitError.CompositorCreateReadableSceneTextureIndexConstantBuffer,
            .compositor_create_layer_graphics_texture_index_constant_buffer => InitError.CompositorCreateLayerGraphicsTextureIndexConstantBuffer,
            .compositor_create_layer_compute_texture_index_constant_buffer => InitError.CompositorCreateLayerComputeTextureIndexConstantBuffer,
            .compositor_create_layer_compute_scene_texture_index_constant_buffer => InitError.CompositorCreateLayerComputeSceneTextureIndexConstantBuffer,
            .compositor_create_compute_hmd_pose_constant_buffer => InitError.CompositorCreateComputeHmdPoseConstantBuffer,
            .compositor_create_geom_constant_buffer => InitError.CompositorCreateGeomConstantBuffer,
            .compositor_create_panel_mask_constant_buffer => InitError.CompositorCreatePanelMaskConstantBuffer,
            .compositor_create_pixel_sim_ubo => InitError.CompositorCreatePixelSimUBO,
            .compositor_create_msaa_render_textures => InitError.CompositorCreateMSAARenderTextures,
            .compositor_create_resolve_render_textures => InitError.CompositorCreateResolveRenderTextures,
            .compositor_create_compute_resolve_render_textures => InitError.CompositorCreateComputeResolveRenderTextures,
            .compositor_create_driver_direct_mode_resolve_textures => InitError.CompositorCreateDriverDirectModeResolveTextures,
            .compositor_open_driver_direct_mode_resolve_textures => InitError.CompositorOpenDriverDirectModeResolveTextures,
            .compositor_create_fallback_sync_texture => InitError.CompositorCreateFallbackSyncTexture,
            .compositor_share_fallback_sync_texture => InitError.CompositorShareFallbackSyncTexture,
            .compositor_create_overlay_index_buffer => InitError.CompositorCreateOverlayIndexBuffer,
            .compositor_create_overlay_vertex_buffer => InitError.CompositorCreateOverlayVertexBuffer,
            .compositor_create_text_vertex_buffer => InitError.CompositorCreateTextVertexBuffer,
            .compositor_create_text_index_buffer => InitError.CompositorCreateTextIndexBuffer,
            .compositor_create_mirror_textures => InitError.CompositorCreateMirrorTextures,
            .compositor_create_last_frame_render_texture => InitError.CompositorCreateLastFrameRenderTexture,
            .compositor_create_mirror_overlay => InitError.CompositorCreateMirrorOverlay,
            .compositor_failed_to_create_virtual_display_backbuffer => InitError.CompositorFailedToCreateVirtualDisplayBackbuffer,
            .compositor_display_mode_not_supported => InitError.CompositorDisplayModeNotSupported,
            .compositor_create_overlay_invalid_call => InitError.CompositorCreateOverlayInvalidCall,
            .compositor_create_overlay_already_initialized => InitError.CompositorCreateOverlayAlreadyInitialized,
            .compositor_failed_to_create_mailbox => InitError.CompositorFailedToCreateMailbox,
            .compositor_window_interface_is_null => InitError.CompositorWindowInterfaceIsNull,
            .compositor_system_layer_create_instance => InitError.CompositorSystemLayerCreateInstance,
            .compositor_system_layer_create_session => InitError.CompositorSystemLayerCreateSession,
            .compositor_create_inverse_distort_u_vs => InitError.CompositorCreateInverseDistortUVs,
            .compositor_create_backbuffer_depth => InitError.CompositorCreateBackbufferDepth,
            .compositor_cannot_drm_lease_display => InitError.CompositorCannotDRMLeaseDisplay,
            .compositor_cannot_connect_to_display_server => InitError.CompositorCannotConnectToDisplayServer,
            .compositor_gnome_no_drm_leasing => InitError.CompositorGnomeNoDRMLeasing,
            .compositor_failed_to_initialize_encoder => InitError.CompositorFailedToInitializeEncoder,
            .compositor_create_blur_texture => InitError.CompositorCreateBlurTexture,
            .vendor_specific_unable_to_connect_to_oculus_runtime => InitError.VendorSpecificUnableToConnectToOculusRuntime,
            .vendor_specific_windows_not_in_dev_mode => InitError.VendorSpecificWindowsNotInDevMode,
            .vendor_specific_oculus_link_not_enabled => InitError.VendorSpecificOculusLinkNotEnabled,
            .vendor_specific_hmd_found_cant_open_device => InitError.VendorSpecificHmdFoundCantOpenDevice,
            .vendor_specific_hmd_found_unable_to_request_config_start => InitError.VendorSpecificHmdFoundUnableToRequestConfigStart,
            .vendor_specific_hmd_found_no_stored_config => InitError.VendorSpecificHmdFoundNoStoredConfig,
            .vendor_specific_hmd_found_config_too_big => InitError.VendorSpecificHmdFoundConfigTooBig,
            .vendor_specific_hmd_found_config_too_small => InitError.VendorSpecificHmdFoundConfigTooSmall,
            .vendor_specific_hmd_found_unable_to_init_z_lib => InitError.VendorSpecificHmdFoundUnableToInitZLib,
            .vendor_specific_hmd_found_cant_read_firmware_version => InitError.VendorSpecificHmdFoundCantReadFirmwareVersion,
            .vendor_specific_hmd_found_unable_to_send_user_data_start => InitError.VendorSpecificHmdFoundUnableToSendUserDataStart,
            .vendor_specific_hmd_found_unable_to_get_user_data_start => InitError.VendorSpecificHmdFoundUnableToGetUserDataStart,
            .vendor_specific_hmd_found_unable_to_get_user_data_next => InitError.VendorSpecificHmdFoundUnableToGetUserDataNext,
            .vendor_specific_hmd_found_user_data_address_range => InitError.VendorSpecificHmdFoundUserDataAddressRange,
            .vendor_specific_hmd_found_user_data_error => InitError.VendorSpecificHmdFoundUserDataError,
            .vendor_specific_hmd_found_config_failed_sanity_check => InitError.VendorSpecificHmdFoundConfigFailedSanityCheck,
            .vendor_specific_oculus_runtime_bad_install => InitError.VendorSpecificOculusRuntimeBadInstall,
            .vendor_specific_hmd_found_unexpected_configuration_1 => InitError.VendorSpecificHmdFoundUnexpectedConfiguration1,
            .steam_installation_not_found => InitError.SteamInstallationNotFound,
            .last_error => InitError.LastError,
        };
    }

    pub fn asSymbol(init_error: InitErrorCode) [:0]const u8 {
        return std.mem.span(VR_GetVRInitErrorAsSymbol(init_error));
    }
    pub fn asEnglishDescription(init_error: InitErrorCode) [:0]const u8 {
        return std.mem.span(VR_GetVRInitErrorAsEnglishDescription(init_error));
    }
};

pub fn initErrorAsSymbol(init_error: InitError) [:0]const u8 {
    return InitErrorCode.fromError(init_error).asSymbol();
}
pub fn initErrorAsEnglishDescription(init_error: InitError) [:0]const u8 {
    return InitErrorCode.fromError(init_error).asEnglishDescription();
}

test "depth error" {
    const err = InitError.InitHmdNotFound;
    const sym = initErrorAsSymbol(err);
    try std.testing.expect(sym.len > 0);
    // std.debug.print("{s}\n", .{sym});
}

extern "openvr_api" fn VR_GetVRInitErrorAsSymbol(InitErrorCode) callconv(.C) [*c]const u8;
extern "openvr_api" fn VR_GetVRInitErrorAsEnglishDescription(InitErrorCode) callconv(.C) [*c]const u8;
extern "openvr_api" fn VR_GetGenericInterface([*c]const u8, *InitErrorCode) callconv(.C) ?*isize;

test "init error have english descriptions" {
    try std.testing.expectEqualStrings("No Error (0)", InitErrorCode.none.asEnglishDescription());
}

pub fn getFunctionTable(comptime T: type, comptime version: []const u8) InitError!*T {
    const interface_name: [*c]const u8 = "FnTable:" ++ version;

    var init_error: InitErrorCode = .none;
    const function_table_ptr = VR_GetGenericInterface(interface_name, &init_error);
    try init_error.maybe();
    return @ptrCast(function_table_ptr.?);
}

pub const Quad = extern struct {
    corners: [4]Vector3,
};

pub const Vector3 = extern struct {
    v: [3]f32,
};

pub const Vector4 = extern struct {
    v: [4]f32,
};

/// right-handed system
/// +y is up
/// +x is to the right
/// -z is forward
/// Distance unit is in meters
pub const Matrix34 = extern struct {
    m: [3][4]f32,
};

pub const Matrix44 = extern struct {
    m: [4][4]f32,
};

pub const Matrix33 = extern struct {
    m: [3][3]f32,
};

pub const Rect2 = extern struct {
    top_left: Vector2,
    bottom_right: Vector2,
};

pub const TrackingUniverseOrigin = enum(i32) {
    seated = 0,
    standing = 1,
    raw_and_uncalibrated = 2,
};

pub const max_tracked_device_count: usize = 64;

pub const TrackingResult = enum(i32) {
    uninitialized = 1,
    calibrating_in_progress = 100,
    calibrating_out_of_range = 101,
    running_ok = 200,
    running_out_of_range = 201,
    fallback_rotation_only = 300,
};

pub const TrackedDeviceIndex = u32;
pub const hmd: TrackedDeviceIndex = 0;

pub const tracked_device_index_hmd: TrackedDeviceIndex = hmd; // a copy that is more similar to the capi
pub const tracked_device_index_other: TrackedDeviceIndex = 0xFFFFFFFE;
pub const tracked_device_index_invalid: TrackedDeviceIndex = 0xFFFFFFFF;

pub const TrackedDevicePose = extern struct {
    device_to_absolute_tracking: Matrix34,
    velocity: Vector3,
    angular_velocity: Vector3,
    tracking_result: TrackingResult,
    pose_is_valid: bool,
    device_is_connected: bool,
};

pub const TextureType = enum(i32) {
    ///Handle has been invalidated
    invalid = -1,
    ///Handle is an ID3D11Texture
    directx = 0,
    ///Handle is an OpenGL texture name or an OpenGL render buffer name, depending on submit flags
    opengl = 1,
    ///Handle is a pointer to a VulkanTextureData structure
    vulkan = 2,
    ///Handle is a macOS cross-process-sharable IOSurfaceRef, deprecated in favor of TextureType.metal on supported platforms
    iosurface = 3,
    ///Handle is a pointer to a D3D12TextureData_t structure
    directx12 = 4,
    ///Handle is a HANDLE DXGI share handle, only supported for Overlay render targets.
    ///this texture is used directly by our renderer, so only perform atomic (copyresource or resolve) on it
    dxgi_shared_handle = 5,
    ///Handle is a MTLTexture conforming to the MTLSharedTexture protocol. Textures submitted to zopenvr.compositor.submit which
    ///are of type MTLTextureType2DArray assume layer 0 is the left eye texture (zopenvr.Eye.left), layer 1 is the right
    ///eye texture (zopenvr.Eye.right)
    metal = 6,
    reserved = 7,
};

pub const Eye = enum(i32) {
    left = 0,
    right = 1,
};

pub const Color = extern struct {
    r: f32,
    g: f32,
    b: f32,
    a: f32,
};

pub const TrackedControllerRole = enum(i32) {
    ///Invalid value for controller type
    invalid = 0,
    ///Tracked device associated with the left hand
    left_hand = 1,
    ///Tracked device associated with the right hand
    right_hand = 2,
    ///Tracked device is opting out of left/right hand selection
    opt_out = 3,
    ///Tracked device is a treadmill or other locomotion device
    treadmill = 4,
    ///Tracked device is a stylus
    stylus = 5,
    const max = TrackedControllerRole.stylus;
};

pub const Quaternion = extern struct {
    w: f64,
    x: f64,
    y: f64,
    z: f64,
};
pub const Quaternionf = extern struct {
    w: f32,
    x: f32,
    y: f32,
    z: f32,
};

pub const InputValueHandle = u64;
pub const ControllerAxis = extern struct {
    ///Ranges from -1.0 to 1.0 for joysticks and track pads. Ranges from 0.0 to 1.0 for triggers were 0 is fully released.
    x: f32,
    ///Ranges from -1.0 to 1.0 for joysticks and track pads. Is always 0.0 for triggers.
    y: f32,
};
pub const ControllerState = extern struct {
    packet_num: u32,
    button_pressed: u64,
    button_touched: u64,
    axis: [5]ControllerAxis,
};

pub const ApplicationError = error{
    AppKeyAlreadyExists,
    NoManifest,
    NoApplication,
    InvalidIndex,
    UnknownApplication,
    IPCFailed,
    ApplicationAlreadyRunning,
    InvalidManifest,
    InvalidApplication,
    LaunchFailed,
    ApplicationAlreadyStarting,
    LaunchInProgress,
    OldApplicationQuitting,
    TransitionAborted,
    IsTemplate,
    SteamVRIsExiting,
    BufferTooSmall,
    PropertyNotSet,
    UnknownProperty,
    InvalidParameter,
    NotImplemented,
};
pub const ApplicationErrorCode = enum(i32) {
    none = 0,

    /// Only one application can use any given key
    app_key_already_exists = 100,
    /// the running application does not have a manifest
    no_manifest = 101,
    /// No application is running
    no_application = 102,
    invalid_index = 103,
    /// the application could not be found
    unknown_application = 104,
    /// An IPC failure caused the request to fail
    ipc_failed = 105,
    application_already_running = 106,
    invalid_manifest = 107,
    invalid_application = 108,
    /// the process didn't start
    launch_failed = 109,
    /// the system was already starting the same application
    application_already_starting = 110,
    /// The system was already starting a different application
    launch_in_progress = 111,
    old_application_quitting = 112,
    transition_aborted = 113,
    /// error when you try to call LaunchApplication() on a template type app (use LaunchTemplateApplication)
    is_template = 114,
    steam_vr_is_exiting = 115,

    /// The provided buffer was too small to fit the requested data
    buffer_too_small = 200,
    /// The requested property was not set
    property_not_set = 201,
    unknown_property = 202,
    invalid_parameter = 203,

    /// Fcn is not implemented in current interface
    not_implemented = 300,

    pub fn fromError(err: ApplicationError) ApplicationErrorCode {
        return switch (err) {
            inline else => |e| comptime for (std.meta.tags(ApplicationErrorCode)) |tag| {
                if (tag.maybe() == e) break tag;
            } else unreachable,
        };
    }

    pub fn maybe(error_code: ApplicationErrorCode) ApplicationError!void {
        return switch (error_code) {
            .none => {},
            .app_key_already_exists => ApplicationError.AppKeyAlreadyExists,
            .no_manifest => ApplicationError.NoManifest,
            .no_application => ApplicationError.NoApplication,
            .invalid_index => ApplicationError.InvalidIndex,
            .unknown_application => ApplicationError.UnknownApplication,
            .ipc_failed => ApplicationError.IPCFailed,
            .application_already_running => ApplicationError.ApplicationAlreadyRunning,
            .invalid_manifest => ApplicationError.InvalidManifest,
            .invalid_application => ApplicationError.InvalidApplication,
            .launch_failed => ApplicationError.LaunchFailed,
            .application_already_starting => ApplicationError.ApplicationAlreadyStarting,
            .launch_in_progress => ApplicationError.LaunchInProgress,
            .old_application_quitting => ApplicationError.OldApplicationQuitting,
            .transition_aborted => ApplicationError.TransitionAborted,
            .is_template => ApplicationError.IsTemplate,
            .steam_vr_is_exiting => ApplicationError.SteamVRIsExiting,
            .buffer_too_small => ApplicationError.BufferTooSmall,
            .property_not_set => ApplicationError.PropertyNotSet,
            .unknown_property => ApplicationError.UnknownProperty,
            .invalid_parameter => ApplicationError.InvalidParameter,
            .not_implemented => ApplicationError.NotImplemented,
        };
    }
};
pub const max_application_key_length = 128;

pub const AppOverrideKeys = struct {
    key: [:0]u8,
    value: [:0]u8,
};

pub const AppKeys = struct {
    buffer: []u8,

    pub fn deinit(self: AppKeys, allocator: std.mem.Allocator) void {
        allocator.free(self.buffer);
    }

    pub fn allocKeys(self: AppKeys, allocator: std.mem.Allocator) ![][]const u8 {
        var keys = std.ArrayList([]const u8).init(allocator);
        var it = std.mem.splitScalar(u8, self.buffer, ',');
        while (it.next()) |key| {
            try keys.append(key);
        }
        return keys.toOwnedSlice();
    }
};
pub const SceneApplicationState = enum(i32) {
    ///Scene Application is not running
    none = 0,
    ///Scene Application is starting
    starting = 1,
    ///Scene Application is quitting
    quitting = 2,
    ///Scene Application is running, and submitting frames, a custom skybox, or a visible overlay
    running = 3,
    ///Scene Application is running, but not drawing anything
    waiting = 4,
};

pub const ApplicationProperty = enum(i32) {
    name = 0,

    launch_type = 11,
    working_directory = 12,
    binary_path = 13,
    arguments = 14,
    url = 15,

    description = 50,
    news_url = 51,
    image_path = 52,
    source = 53,
    action_manifest_url = 54,

    is_dashboard_overlay = 60,
    is_template = 61,
    is_instanced = 62,
    is_internal = 63,
    wants_compositor_pause_in_standby = 64,
    is_hidden = 65,

    last_launch_time = 70,

    pub fn fromType(comptime T: type) type {
        return switch (T) {
            bool => ApplicationProperty.Bool,
            u64 => ApplicationProperty.U64,
            else => @compileError("T must be one of bool, u64"),
        };
    }

    pub const String = enum(i32) {
        name = @intFromEnum(ApplicationProperty.name),
        launch_type = @intFromEnum(ApplicationProperty.launch_type),
        working_directory = @intFromEnum(ApplicationProperty.working_directory),
        binary_path = @intFromEnum(ApplicationProperty.binary_path),
        arguments = @intFromEnum(ApplicationProperty.arguments),
        url = @intFromEnum(ApplicationProperty.url),
        description = @intFromEnum(ApplicationProperty.description),
        news_url = @intFromEnum(ApplicationProperty.news_url),
        image_path = @intFromEnum(ApplicationProperty.image_path),
        source = @intFromEnum(ApplicationProperty.source),
        action_manifest_url = @intFromEnum(ApplicationProperty.action_manifest_url),
    };

    pub const Bool = enum(i32) {
        is_dashboard_overlay = @intFromEnum(ApplicationProperty.is_dashboard_overlay),
        is_template = @intFromEnum(ApplicationProperty.is_template),
        is_instanced = @intFromEnum(ApplicationProperty.is_instanced),
        is_internal = @intFromEnum(ApplicationProperty.is_internal),
        wants_compositor_pause_in_standby = @intFromEnum(ApplicationProperty.wants_compositor_pause_in_standby),
        is_hidden = @intFromEnum(ApplicationProperty.is_hidden),
    };

    pub const U64 = enum(i32) {
        last_launch_time = @intFromEnum(ApplicationProperty.last_launch_time),
    };
};

pub const MimeTypes = struct {
    buffer: []u8,

    pub fn deinit(self: MimeTypes, allocator: std.mem.Allocator) void {
        allocator.free(self.buffer);
    }

    pub fn allocTypes(self: MimeTypes, allocator: std.mem.Allocator) ![][]const u8 {
        var types = std.ArrayList([]const u8).init(allocator);
        var it = std.mem.splitScalar(u8, self.buffer, ',');
        while (it.next()) |t| {
            try types.append(t);
        }
        return types.toOwnedSlice();
    }
};

pub const CalibrationState = enum(i32) {
    ok = 1,
    warning = 100,
    warning_base_station_may_have_moved = 101,
    warning_base_station_removed = 102,
    warning_seated_bounds_invalid = 103,
    @"error" = 200,
    error_base_station_uninitialized = 201,
    error_base_station_conflict = 202,
    error_play_area_invalid = 203,
    error_collision_bounds_invalid = 204,
};

pub const PlayAreaSize = struct {
    x: f32,
    z: f32,
};

pub const BoundsColor = struct {
    bound_colors: []Color,
    camera_color: Color,

    pub fn deinit(self: BoundsColor, allocator: std.mem.Allocator) void {
        allocator.free(self.bound_colors);
    }
};

pub const CompositorError = error{
    RequestFailed,
    IncompatibleVersion,
    DoNotHaveFocus,
    InvalidTexture,
    IsNotSceneApplication,
    TextureIsOnWrongDevice,
    TextureUsesUnsupportedFormat,
    SharedTexturesNotSupported,
    IndexOutOfRange,
    AlreadySubmitted,
    InvalidBounds,
    AlreadySet,
};
pub const CompositorErrorCode = enum(i32) {
    none = 0,
    request_failed = 1,
    incompatible_version = 100,
    do_not_have_focus = 101,
    invalid_texture = 102,
    is_not_scene_application = 103,
    texture_is_on_wrong_device = 104,
    texture_uses_unsupported_format = 105,
    shared_textures_not_supported = 106,
    index_out_of_range = 107,
    already_submitted = 108,
    invalid_bounds = 109,
    already_set = 110,

    pub fn fromError(err: CompositorError) CompositorErrorCode {
        return switch (err) {
            inline else => |e| comptime for (std.meta.tags(CompositorErrorCode)) |tag| {
                if (tag.maybe() == e) break tag;
            } else unreachable,
        };
    }

    pub fn maybe(compositor_error: CompositorErrorCode) CompositorError!void {
        return switch (compositor_error) {
            .none => {},
            .request_failed => CompositorError.RequestFailed,
            .incompatible_version => CompositorError.IncompatibleVersion,
            .do_not_have_focus => CompositorError.DoNotHaveFocus,
            .invalid_texture => CompositorError.InvalidTexture,
            .is_not_scene_application => CompositorError.IsNotSceneApplication,
            .texture_is_on_wrong_device => CompositorError.TextureIsOnWrongDevice,
            .texture_uses_unsupported_format => CompositorError.TextureUsesUnsupportedFormat,
            .shared_textures_not_supported => CompositorError.SharedTexturesNotSupported,
            .index_out_of_range => CompositorError.IndexOutOfRange,
            .already_submitted => CompositorError.AlreadySubmitted,
            .invalid_bounds => CompositorError.InvalidBounds,
            .already_set => CompositorError.AlreadySet,
        };
    }
};

pub const CompositorPoses = struct {
    render_poses: []TrackedDevicePose,
    game_poses: []TrackedDevicePose,

    pub fn allocInit(allocator: std.mem.Allocator, render_poses_count: usize, game_poses_count: usize) !CompositorPoses {
        return .{
            .render_poses = try allocator.alloc(TrackedDevicePose, render_poses_count),
            .game_poses = try allocator.alloc(TrackedDevicePose, game_poses_count),
        };
    }

    pub fn deinit(self: CompositorPoses, allocator: std.mem.Allocator) void {
        allocator.free(self.render_poses);
        allocator.free(self.game_poses);
    }
};

pub const CompositorPose = struct {
    render_pose: TrackedDevicePose,
    game_pose: TrackedDevicePose,
};

pub const SubmitFlags = packed struct(i32) {
    lens_distortion_already_applied: bool = false,
    gl_render_buffer: bool = false,
    reserved: bool = false,
    texture_with_pose: bool = false,
    texture_with_depth: bool = false,
    frame_discontinuty: bool = false,
    vulkan_texture_with_array_data: bool = false,
    gl_array_texture: bool = false,
    is_egl: bool = false,

    _padding: u5 = 0,
    reserved2: bool = false,
    reserved3: bool = false,
    __padding: u16 = 0,
};

pub const ColorSpace = enum(i32) {
    ///Assumes 'gamma' for 8-bit per component formats, otherwise 'linear'.  This mirrors the DXGI formats which have _SRGB variants.
    auto = 0,
    ///Texture data can be displayed directly on the display without any conversion (a.k.a. display native format).
    gamma = 1,
    ///Same as gamma but has been converted to a linear representation using DXGI's sRGB conversion algorithm.
    linear = 2,
};
pub const D3D12TextureData = extern struct {
    resource: *d3d12.IResource,
    command_queue: *d3d12.ICommandQueue,
    node_mask: u32,
};
pub const Texture = extern struct {
    handle: *const anyopaque,
    texture_type: TextureType,
    color_space: ColorSpace,
};
pub const TextureBounds = extern struct {
    u_min: f32,
    v_min: f32,
    u_max: f32,
    v_max: f32,
};

pub const FrameTiming = extern struct {
    size: u32,
    frame_index: u32,
    num_frame_presents: u32,
    num_mis_presented: u32,
    num_dropped_frames: u32,
    reprojection_flags: u32,
    system_time_in_seconds: f64,
    pre_submit_gpu_ms: f32,
    post_submit_gpu_ms: f32,
    total_render_gpu_ms: f32,
    compositor_render_gpu_ms: f32,
    compositor_render_cpu_ms: f32,
    compositor_idle_cpu_ms: f32,
    client_frame_interval_ms: f32,
    present_call_cpu_ms: f32,
    wait_for_present_cpu_ms: f32,
    submit_frame_ms: f32,
    wait_get_poses_called_ms: f32,
    new_poses_ready_ms: f32,
    new_frame_ready_ms: f32,
    compositor_update_start_ms: f32,
    compositor_update_end_ms: f32,
    compositor_render_start_ms: f32,
    pose: TrackedDevicePose,
    num_v_syncs_ready_for_use: u32,
    num_v_syncs_to_first_view: u32,
};

pub const CumulativeStats = extern struct {
    pid: u32,
    num_frame_presents: u32,
    num_dropped_frames: u32,
    num_reprojected_frames: u32,
    num_frame_presents_on_startup: u32,
    num_dropped_frames_on_startup: u32,
    num_reprojected_frames_on_startup: u32,
    num_loading: u32,
    num_frame_presents_loading: u32,
    num_dropped_frames_loading: u32,
    num_reprojected_frames_loading: u32,
    num_timed_out: u32,
    num_frame_presents_timed_out: u32,
    num_dropped_frames_timed_out: u32,
    num_reprojected_frames_timed_out: u32,
    num_frame_submits: u32,
    sum_compositor_cpu_time_ms: f64,
    sum_compositor_gpu_time_ms: f64,
    sum_target_frame_times: f64,
    sum_application_cpu_time_ms: f64,
    sum_application_gpu_time_ms: f64,
    num_frames_with_depth: u32,
};

pub const Skybox = union(enum) {
    full: extern struct {
        front: Texture,
        back: Texture,
        left: Texture,
        right: Texture,
        top: Texture,
        bottom: Texture,
    },
    single_lat_long: Texture,
    stereo_lat_long: extern struct {
        left: Texture,
        right: Texture,
    },

    pub fn asSlice(self: Skybox) []Texture {
        return switch (self) {
            else => |skybox| std.mem.bytesAsSlice(Texture, std.mem.toBytes(skybox)),
        };
    }
};

pub const TimingMode = enum(i32) {
    implicit = 0,
    explicit_runtime_performs_post_present_handoff = 1,
    explicit_application_performs_post_present_handoff = 2,
};

pub const StageRenderSettings = extern struct {
    primary_color: Color,
    secondary_color: Color,
    vignette_inner_radius: f32,
    vignette_outer_radius: f32,
    fresnel_strength: f32,
    backface_culling: bool,
    greyscale: bool,
    wireframe: bool,
};

pub const BenchmarkResults = extern struct {
    mega_pixels_per_second: f32,
    hmd_recommended_mega_pixels_per_second: f32,
};

pub const CompositorPosePredictionIDs = struct {
    render_pose_prediction_id: u32,
    game_pose_prediction_id: u32,
};

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

    pub fn fromError(err: InputError) InputErrorCode {
        return switch (err) {
            inline else => |e| comptime for (std.meta.tags(InputErrorCode)) |tag| {
                if (tag.maybe() == e) break tag;
            } else unreachable,
        };
    }

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
    restricted_to_device: InputValueHandle,
    secondary_action_set: ActionSetHandle,
    padding: u32,
    priority: i32,
};

pub const InputDigitalActionData = extern struct {
    active: bool,
    active_origin: InputValueHandle,
    state: bool,
    changed: bool,
    update_time: f32,
};
pub const InputAnalogActionData = extern struct {
    active: bool,
    active_origin: InputValueHandle,
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
    active_origin: InputValueHandle,
    pose: TrackedDevicePose,
};
pub const InputSkeletalActionData = extern struct {
    active: bool,
    active_origin: InputValueHandle,
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
    position: Vector4,
    orientation: Quaternionf,
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
    device_path: InputValueHandle,
    tracked_device_index: TrackedDeviceIndex,
    render_model_component_name: [127:0]u8,
};

pub const InputBindingInfo = extern struct {
    device_path_name: [127:0]u8,
    input_path_name: [127:0]u8,
    mode_name: [127:0]u8,
    slot_name: [127:0]u8,
    input_source_type: [31:0]u8,
};
pub const max_bone_name_length: usize = 32;

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

    pub fn fromError(err: RenderModelError) RenderModelErrorCode {
        return switch (err) {
            inline else => |e| comptime for (std.meta.tags(RenderModelErrorCode)) |tag| {
                if (tag.maybe() == e) break tag;
            } else unreachable,
        };
    }

    pub fn maybe(error_code: RenderModelErrorCode) RenderModelError!void {
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
pub const ExternRenderModel = extern struct {
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
        position: Vector3,
        normal: Vector3,
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
        tracking_to_component_render_model: Matrix34,
        tracking_to_component_local: Matrix34,
        properties: ComponentProperties,
    };
};

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
    ///the ID was not valid.
    invalid = 0,
    ///Head-Mounted Displays
    hmd = 1,
    ///Tracked controllers
    controller = 2,
    ///Generic trackers, similar to controllers
    generic_tracker = 3,
    ///Camera and base stations that serve as tracking reference points
    tracking_reference = 4,
    ///Accessories that aren't necessarily tracked themselves, but may redirect video output from other tracked devices
    display_redirect = 5,
    max = 6,
};

///user_interaction_timeout means the device is in the process of timing out.
///InUse = ( user_interaction || user_interaction_timeout )
///EventType.tracked_device_user_interaction_started fires when the devices transitions from standby -> user_interaction or idle -> user_interaction.
///EventType.tracked_device_user_interaction_ended fires when the devices transitions from user_interaction_timeout -> idle
pub const DeviceActivityLevel = enum(i32) {
    unknown = -1,
    ///No activity for the last 10 seconds
    idle = 0,
    ///Activity (movement or prox sensor) is happening now
    user_interaction = 1,
    ///No activity for the last 0.5 seconds
    user_interaction_timeout = 2,
    ///Idle for at least 5 seconds (configurable in Settings -> Power Management)
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
            root.Matrix34 => TrackedDeviceProperty.Matrix34,
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
                root.Vector4 => TrackedDeviceProperty.Array.Vector4,
                root.Matrix34 => TrackedDeviceProperty.Array.Matrix34,
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

    pub fn fromError(err: TrackedPropertyError) TrackedPropertyErrorCode {
        return switch (err) {
            inline else => |e| comptime for (std.meta.tags(TrackedPropertyErrorCode)) |tag| {
                if (tag.maybe() == e) break tag;
            } else unreachable,
        };
    }

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
    tracked_device_index: TrackedDeviceIndex,
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
    button: ButtonId,
};
pub const EventMouse = extern struct {
    x: f32,
    y: f32,
    button: VRMouseButton,
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
    overlay_handle: OverlayHandle,
    device_path: u64,
    memory_block_id: u64,
    cursor_index: u32,
};
pub const EventStatus = extern struct {
    status_state: VRState,
};
pub const EventKeyboard = extern struct {
    /// 7 bytes of utf8 + null
    new_input: [8]u8,
    /// caller specified opaque token
    user_value: u64,
    overlay_handle: OverlayHandle,
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
    overlay_handle: OverlayHandle,
    visual_mode: u32,
};
pub const EventMessageOverlay = extern struct { // which EventType?
    response: MessageOverlayResponse,
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

    pub fn fromError(err: FirmwareError) FirmwareErrorCode {
        return switch (err) {
            FirmwareError.None => FirmwareErrorCode.none,
            FirmwareError.Success => FirmwareErrorCode.success,
            FirmwareError.Fail => FirmwareErrorCode.fail,
        };
    }

    pub fn maybe(firmware_error: FirmwareErrorCode) FirmwareError!void {
        return switch (firmware_error) {
            .none, .success => {},
            .fail => FirmwareError.Fail,
        };
    }
};

pub const VRState = enum(i32) {
    undefined = -1,
    off = 0,
    searching = 1,
    searching_alert = 2,
    ready = 3,
    ready_alert = 4,
    not_ready = 5,
    standby = 6,
    ready_alert_low = 7,
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

    ///data is controller
    button_press = 200,
    ///data is controller
    button_unpress = 201,
    ///data is controller
    button_touch = 202,
    ///data is controller
    button_untouch = 203,

    ///Sent to overlays with the (???)
    modal_cancel = 257,

    ///data is mouse
    mouse_move = 300,
    ///data is mouse
    mouse_button_down = 301,
    ///data is mouse
    mouse_button_up = 302,
    ///data is overlay
    focus_enter = 303,
    ///data is overlay
    focus_leave = 304,
    ///data is scroll
    scroll_discrete = 305,
    ///data is mouse
    touch_pad_move = 306,
    ///data is overlay, global event
    overlay_focus_changed = 307,
    reload_overlays = 308,
    ///data is scroll
    scroll_smooth = 309,
    ///data is mouse
    lock_mouse_position = 310,
    ///data is mouse
    unlock_mouse_position = 311,

    ///data is process DEPRECATED
    input_focus_captured = 400,
    ///data is process DEPRECATED
    input_focus_released = 401,
    ///data is process - The App actually drawing the scene changed (usually to or from the compositor)
    scene_application_changed = 404,
    ///data is process
    input_focus_changed = 406,
    ///data is process
    scene_application_using_wrong_graphics_adapter = 408,
    ///data is process - The App that action binds reloaded for
    action_binding_reloaded = 409,

    ///Sent to the scene application to request hiding render models temporarily
    hide_render_models = 410,
    ///Sent to the scene application to request restoring render model visibility
    show_render_models = 411,

    ///No data; but query openvr.Applications.getSceneApplicationState();
    scene_application_state_changed = 412,

    ///data is process - Called when the scene app's pipe has been closed.
    scene_app_pipe_disconnected = 413,

    console_opened = 420,
    console_closed = 421,

    ///Indicates that an overlay is now visible to someone and should be rendering normally. Reflects openvr.Overlay.isOverlayVisible() becoming true.
    overlay_shown = 500,
    ///Indicates that an overlay is no longer visible to someone and doesn't need to render frames. Reflects openvr.Overlay.isOverlayVisible() becoming false.
    overlay_hidden = 501,
    dashboard_activated = 502,
    dashboard_deactivated = 503,
    ///Sent to the overlay manager - data is overlay
    dashboard_requested = 505,
    ///Send to the overlay manager
    reset_dashboard = 506,
    ///Sent to overlays when a SetOverlayRaw or SetOverlayFromFile call finishes loading
    image_loaded = 508,
    ///Sent to keyboard renderer in the dashboard to invoke it
    show_keyboard = 509,
    ///Sent to keyboard renderer in the dashboard to hide it
    hide_keyboard = 510,
    ///Sent to an overlay when openvr.Overlay.setFocusOverlay is called on it
    overlay_gamepad_focus_gained = 511,
    ///Send to an overlay when it previously had focus and openvr.Overlay.setFocusOverlay is called on something else
    overlay_gamepad_focus_lost = 512,
    overlay_shared_texture_changed = 513,
    ///Screenshot button combo was pressed, Dashboard should request a screenshot
    screenshot_triggered = 516,
    ///Sent to overlays when a openvr.Overlay.setOverlayRaw or openvr.Overlay.setOverlayfromFile fails to load
    image_failed = 517,
    dashboard_overlay_created = 518,
    switch_gamepad_focus = 519,

    //screenshot api
    ///Sent by vrclient application to compositor to take a screenshot
    request_screenshot = 520,
    ///Sent by compositor to the application that the screenshot has been taken
    screenshot_taken = 521,
    ///Sent by compositor to the application that the screenshot failed to be taken
    screenshot_failed = 522,
    ///Sent by compositor to the dashboard that a completed screenshot was submitted
    submit_screenshot_to_dashboard = 523,
    ///Sent by compositor to the dashboard that a completed screenshot was submitted
    screenshot_progress_to_dashboard = 524,

    primary_dashboard_device_changed = 525,
    ///Sent by compositor whenever room-view is enabled
    room_view_shown = 526,
    ///Sent by compositor whenever room-view is disabled
    room_view_hidden = 527,
    ///data is showUi
    show_ui = 528,
    ///data is showDevTools
    show_dev_tools = 529,
    desktop_view_updating = 530,
    desktop_view_ready = 531,

    start_dashboard = 532,
    elevate_prism = 533,

    overlay_closed = 534,

    ///Sent when a dashboard thumbnail image changes
    dashboard_thumb_changed = 535,

    ///Sent when any known desktop related overlay is visible
    desktop_might_be_visible = 536,
    ///Sent when all known desktop related overlays are hidden
    desktop_might_be_hidden = 537,

    notification_shown = 600,
    notification_hidden = 601,
    notification_begin_interaction = 602,
    notification_destroyed = 603,

    ///data is process
    quit = 700,
    ///data is process
    process_quit = 701,
    ///data is process
    quit_acknowledged = 703,
    ///The driver has requested that SteamVR shut down
    driver_requested_quit = 704,
    ///A driver or other component wants the user to restart SteamVR
    restart_requested = 705,
    invalidate_swap_texture_sets = 706,

    ///this will never happen with the new chaperone system
    chaperone_data_has_changed = 800,
    chaperone_universe_has_changed = 801,
    ///this will never happen with the new chaperone system
    chaperone_temp_data_has_changed = 802,
    chaperone_settings_have_changed = 803,
    seated_zero_pose_reset = 804,
    ///Sent when the process needs to reload any cached data it retrieved from openvr.Chaperone
    chaperone_flush_cache = 805,
    ///Triggered by openvr.ChaperoneSetup.roomSetupStarting
    chaperone_room_setup_starting = 806,
    ///Triggered by openvr.ChaperoneSetup.commitWorkingCopy
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

    ///DEPRECATED: Sent only to the overlay it closed for, or globally if it was closed for a scene app
    keyboard_closed = 1200,
    keyboard_char_input = 1201,
    ///Sent when DONE button clicked on keyboard
    keyboard_done = 1202,
    ///Sent globally when the keyboard is opened. data.keyboard.overlay_handle is who it was opened for (scene app if openvr.overlay_handle_invalid)
    keyboard_opened_global = 1203,
    ///Sent globally when the keyboard is opened. data.keyboard.overlay_handle is who it was opened for (scene app if openvr.overlay_handle_invalid)
    keyboard_closed_global = 1204,

    application_list_updated = 1303,
    application_mime_type_load = 1304,
    process_connected = 1306,
    process_disconnected = 1307,

    compositor_chaperone_bounds_shown = 1410,
    compositor_chaperone_bounds_hidden = 1411,
    compositor_display_disconnected = 1412,
    compositor_display_reconnected = 1413,
    ///data is hdcpError
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

    ///data is hapticVibration
    input_haptic_vibration = 1700,
    ///data is inputBinding
    input_binding_load_failed = 1701,
    ///data is inputBinding
    input_binding_load_successful = 1702,
    ///no data
    input_action_manifest_reloaded = 1703,
    ///data is actionManifest
    input_action_manifest_load_failed = 1704,
    ///data is progressUpdate
    input_progress_update = 1705,
    input_tracker_activated = 1706,
    input_bindings_updated = 1707,
    input_binding_subscription_changed = 1708,

    ///data is spatialAnchor. broadcast
    spatial_anchors_pose_updated = 1800,
    ///data is spatialAnchor. broadcast
    spatial_anchors_descriptor_updated = 1801,
    ///data is spatialAnchor. sent to specific driver
    spatial_anchors_request_pose_update = 1802,
    ///data is spatialAnchor. sent to specific driver
    spatial_anchors_request_descriptor_update = 1803,

    ///user or system initiated generation of a system report. broadcast
    system_report_started = 1900,

    ///data is process
    monitor_show_headset_view = 2000,
    ///data is process
    monitor_hide_headset_view = 2001,

    audio_set_speakers_volume = 2100,
    audio_set_speakers_mute = 2101,
    audio_set_microphone_volume = 2102,
    audio_set_microphone_mute = 2103,

    // vendor spesific area
    vendor_specific_reserved_start = 10000,
    vendor_specific_reserved_end = 19999,

    _,
};

pub const RenderTargetSize = struct { width: u32, height: u32 };

pub const RawProjection = struct {
    left: f32,
    right: f32,
    top: f32,
    bottom: f32,
};

pub const VSyncTiming = struct {
    seconds_since_last_vsync: f32,
    frame_counter: u64,
};

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
            Matrix34 => .matrix34,
            Matrix44 => .matrix44,
            Vector2 => .vector2,
            Vector3 => .vector3,
            Vector4 => .vector4,
            Quad => .quad,
            else => @compileError("unsupported type " ++ @typeName(T)),
        };
    }
};
pub const EventWithPose = struct {
    event: Event,
    pose: TrackedDevicePose,
};

pub const ControllerStateWithPose = struct {
    controller_state: ControllerState,
    pose: TrackedDevicePose,
};

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

pub const OverlayHandle = u64;
pub const overlay_handle_invalid: OverlayHandle = 0;

pub const OverlayError = error{
    UnknownOverlay,
    InvalidHandle,
    PermissionDenied,
    OverlayLimitExceeded,
    WrongVisibilityType,
    KeyTooLong,
    NameTooLong,
    KeyInUse,
    WrongTransformType,
    InvalidTrackedDevice,
    InvalidParameter,
    ThumbnailCantBeDestroyed,
    ArrayTooSmall,
    RequestFailed,
    InvalidTexture,
    UnableToLoadFile,
    KeyboardAlreadyInUse,
    NoNeighbor,
    TooManyMaskPrimitives,
    BadMaskPrimitive,
    TextureAlreadyLocked,
    TextureLockCapacityReached,
    TextureNotLocked,
    TimedOut,
};

pub const OverlayErrorCode = enum(i32) {
    none = 0,
    unknown_overlay = 10,
    invalid_handle = 11,
    permission_denied = 12,
    ///No more overlays could be created because the maximum number already exist
    overlay_limit_exceeded = 13,
    wrong_visibility_type = 14,
    key_too_long = 15,
    name_too_long = 16,
    key_in_use = 17,
    wrong_transform_type = 18,
    invalid_tracked_device = 19,
    invalid_parameter = 20,
    thumbnail_cant_be_destroyed = 21,
    array_too_small = 22,
    request_failed = 23,
    invalid_texture = 24,
    unable_to_load_file = 25,
    keyboard_already_in_use = 26,
    no_neighbor = 27,
    too_many_mask_primitives = 29,
    bad_mask_primitive = 30,
    texture_already_locked = 31,
    texture_lock_capacity_reached = 32,
    texture_not_locked = 33,
    timed_out = 34,

    pub fn fromError(err: OverlayError) OverlayErrorCode {
        return switch (err) {
            inline else => |e| comptime for (std.meta.tags(OverlayErrorCode)) |tag| {
                if (tag.maybe() == e) break tag;
            } else unreachable,
        };
    }

    pub fn maybe(overlay_error: OverlayErrorCode) OverlayError!void {
        return switch (overlay_error) {
            .none => {},
            .unknown_overlay => OverlayError.UnknownOverlay,
            .invalid_handle => OverlayError.InvalidHandle,
            .permission_denied => OverlayError.PermissionDenied,
            .overlay_limit_exceeded => OverlayError.OverlayLimitExceeded,
            .wrong_visibility_type => OverlayError.WrongVisibilityType,
            .key_too_long => OverlayError.KeyTooLong,
            .name_too_long => OverlayError.NameTooLong,
            .key_in_use => OverlayError.KeyInUse,
            .wrong_transform_type => OverlayError.WrongTransformType,
            .invalid_tracked_device => OverlayError.InvalidTrackedDevice,
            .invalid_parameter => OverlayError.InvalidParameter,
            .thumbnail_cant_be_destroyed => OverlayError.ThumbnailCantBeDestroyed,
            .array_too_small => OverlayError.ArrayTooSmall,
            .request_failed => OverlayError.RequestFailed,
            .invalid_texture => OverlayError.InvalidTexture,
            .unable_to_load_file => OverlayError.UnableToLoadFile,
            .keyboard_already_in_use => OverlayError.KeyboardAlreadyInUse,
            .no_neighbor => OverlayError.NoNeighbor,
            .too_many_mask_primitives => OverlayError.TooManyMaskPrimitives,
            .bad_mask_primitive => OverlayError.BadMaskPrimitive,
            .texture_already_locked => OverlayError.TextureAlreadyLocked,
            .texture_lock_capacity_reached => OverlayError.TextureLockCapacityReached,
            .texture_not_locked => OverlayError.TextureNotLocked,
            .timed_out => OverlayError.TimedOut,
        };
    }
};

test "back to error code" {
    const expectEqual = std.testing.expectEqual;
    const code = OverlayErrorCode.array_too_small;
    _ = code.maybe() catch |err|
        try expectEqual(code, OverlayErrorCode.fromError(err));
}

///See comments in enum
pub const OverlayFlags = packed struct(u32) {
    _padding: u3 = 0,
    no_dashboard_tab: bool = false,
    __padding: u2 = 0,

    send_vr_discrete_scroll_events: bool = false,
    send_vr_touchpad_events: bool = false,

    show_touch_pad_scroll_wheel: bool = false,
    transfer_ownership_to_internal_process: bool = false,

    side_by_side_parallel: bool = false,
    side_by_side_crossed: bool = false,

    panorama: bool = false,
    stereo_panorama: bool = false,

    sort_with_non_scene_overlays: bool = false,
    visible_in_dashboard: bool = false,
    make_overlays_interactive_if_visible: bool = false,
    send_vr_smooth_scroll_events: bool = false,
    protected_content: bool = false,
    hide_laser_intersection: bool = false,
    wants_modal_behavior: bool = false,
    is_premultiplied: bool = false,
    ignore_texture_alpha: bool = false,
    enable_control_bar: bool = false,
    enable_control_bar_keyboard: bool = false,
    enable_control_bar_close: bool = false,
    reserved: bool = false,
    enable_click_stabilization: bool = false,
    multi_cursor: bool = false,
    ___padding: u3 = 0,

    pub const Enum = enum(u32) {
        ///Set this flag on a dashboard overlay to prevent a tab from showing up for that overlay
        NoDashboardTab = @bitCast(OverlayFlags{ .no_dashboard_tab = true }),
        ///When this is set the overlay will receive EventType.scroll_discrete events like a mouse wheel.
        ///Requires mouse input mode.
        SendVRDiscreteScrollEvents = @bitCast(OverlayFlags{ .send_vr_discrete_scroll_events = true }),
        ///Indicates that the overlay would like to receive touchpad events (EventData.touch_pad_move).
        SendVRTouchpadEvents = @bitCast(OverlayFlags{ .send_vr_touchpad_events = true }),
        ///If set this will render a vertical scroll wheel on the primary controller,
        ///only needed if not using OverlayFlags.send_vr_scroll_events (smooth or discrete) but you still want to represent a scroll wheel
        ShowTouchPadScrollWheel = @bitCast(OverlayFlags{ .show_touch_pad_scroll_wheel = true }),
        ///If this is set ownership and render access to the overlay are transferred
        ///to the new scene process on a call to openvr.Applications.launchInternalProcess
        TransferOwnershipToInternalProcess = @bitCast(OverlayFlags{ .transfer_ownership_to_internal_process = true }),
        ///If set, renders 50% of the texture in each eye, side by side
        ///Texture is left/right
        SideBySide_Parallel = @bitCast(OverlayFlags{ .side_by_side_parallel = true }),
        ///If set, renders 50% of the texture in each eye, side by side
        ///Texture is crossed and right/left
        SideBySide_Crossed = @bitCast(OverlayFlags{ .side_by_side_crossed = true }),
        ///Texture is a panorama
        Panorama = @bitCast(OverlayFlags{ .panorama = true }),
        ///Texture is a stereo panorama
        StereoPanorama = @bitCast(OverlayFlags{ .stereo_panorama = true }),
        ///If this is set on an overlay owned by the scene application that overlay
        ///will be sorted with the "Other" overlays on top of all other scene overlays
        SortWithNonSceneOverlays = @bitCast(OverlayFlags{ .sort_with_non_scene_overlays = true }),
        ///If set, the overlay will be shown in the dashboard, otherwise it will be hidden.
        VisibleInDashboard = @bitCast(OverlayFlags{ .visible_in_dashboard = true }),
        ///If this is set and the overlay's input method is not none, the system-wide laser mouse
        ///mode will be activated whenever this overlay is visible.
        MakeOverlaysInteractiveIfVisible = @bitCast(OverlayFlags{ .make_overlays_interactive_if_visible = true }),
        ///If this is set the overlay will receive smooth EventType.scroll_smooth that emulate trackpad scrolling.
        ///Requires mouse input mode.
        SendVRSmoothScrollEvents = @bitCast(OverlayFlags{ .send_vr_smooth_scroll_events = true }),
        ///If this is set, the overlay texture will be protected content, preventing unauthorized reads.
        ProtectedContent = @bitCast(OverlayFlags{ .protected_content = true }),
        ///If this is set, the laser mouse splat will not be drawn over this overlay. The overlay will
        ///be responsible for drawing its own "cursor".
        HideLaserIntersection = @bitCast(OverlayFlags{ .hide_laser_intersection = true }),
        ///If this is set, clicking away from the overlay will cause it to receive a EventType.modal_cancel event.
        ///This is ignored for dashboard overlays.
        WantsModalBehavior = @bitCast(OverlayFlags{ .wants_modal_behavior = true }),
        ///If this is set, alpha composition assumes the texture is pre-multiplied
        IsPremultiplied = @bitCast(OverlayFlags{ .is_premultiplied = true }),
        ///If this is set, the alpha values of the overlay texture will be ignored
        IgnoreTextureAlpha = @bitCast(OverlayFlags{ .ignore_texture_alpha = true }),
        ///If this is set, this overlay will have a control bar drawn underneath of it in the dashboard.
        EnableControlBar = @bitCast(OverlayFlags{ .enable_control_bar = true }),
        ///If this is set, the overlay control bar will provide a button to toggle the keyboard.
        EnableControlBarKeyboard = @bitCast(OverlayFlags{ .enable_control_bar_keyboard = true }),
        ///If this is set, the overlay control bar will provide a "close" button which will send a
        ///EventType.overlay_closed event to the overlay when pressed. Applications that use this flag are responsible
        ///for responding to the event with something that approximates "closing" behavior, such as destroying their
        ///overlay and/or shutting down their application.
        EnableControlBarClose = @bitCast(OverlayFlags{ .enable_control_bar_close = true }),
        ///Do not use
        Reserved = @bitCast(OverlayFlags{ .reserved = true }),
        ///If this is set, click stabilization will be applied to the laser interaction so that clicks more reliably
        ///trigger on the user's intended target
        EnableClickStabilization = @bitCast(OverlayFlags{ .enable_click_stabilization = true }),
        ///If this is set, laser mouse pointer events may be sent for the secondary laser. These events will have
        ///cursorIndex set to 0 for the primary laser and 1 for the secondary.
        MultiCursor = @bitCast(OverlayFlags{ .multi_cursor = true }),
    };
};

test "overlay flag bits are in the correct place" {
    const expect = std.testing.expect;
    try expect(@as(u32, @bitCast(OverlayFlags{ .no_dashboard_tab = true })) == 1 << 3);
    try expect(@as(u32, @bitCast(OverlayFlags{ .multi_cursor = true })) == 1 << 28);
    try expect(@as(u32, @bitCast(OverlayFlags{ .protected_content = true })) == 1 << 18);
    try expect(@as(u32, @bitCast(OverlayFlags{ .send_vr_discrete_scroll_events = true })) == 1 << 6);
}

pub const OverlayTransformType = enum(i32) {
    Invalid = -1,
    Absolute = 0,
    TrackedDeviceRelative = 1,
    //SystemOverlay = 2, // Deleted from the SDK.
    TrackedComponent = 3,
    Cursor = 4,
    DashboardTab = 5,
    DashboardThumb = 6,
    Mountable = 7,
    Projection = 8,
    Subview = 9,
};

pub const OverlayProjection = RawProjection; // order is left right top bottom

pub const OverlayInputMethod = enum(i32) {
    ///No input events will be generated automatically for this overlay
    None = 0,
    ///Tracked controllers will get mouse events automatically
    Mouse = 1,
    // DualAnalog = 2, // No longer supported
};

pub const OverlayIntersectionParams = extern struct {
    source: Vector3,
    direction: Vector3,
    origin: TrackingUniverseOrigin,
};

pub const OverlayIntersectionResults = extern struct {
    point: Vector3,
    normal: Vector3,
    uvs: Vector2,
    distance: f32,
};

pub const OverlayIntersectionMaskPrimitive_Data = extern union {
    rectangle: IntersectionMaskRectangle,
    circle: IntersectionMaskCircle,
};

pub const OverlayIntersectionMaskPrimitiveType = enum(i32) {
    rectangle,
    circle,
};

pub const IntersectionMaskRectangle = extern struct {
    top_left_x: f32,
    top_left_y: f32,
    width: f32,
    height: f32,
};

pub const IntersectionMaskCircle = extern struct {
    center_x: f32,
    center_y: f32,
    radius: f32,
};

pub const OverlayIntersectionMaskPrimitive = extern struct {
    primitive_type: OverlayIntersectionMaskPrimitiveType,
    primitive: OverlayIntersectionMaskPrimitive_Data,
};

pub const KeyboardFlags = packed struct(u32) {
    /// Makes the keyboard send key events immediately instead of accumulating a buffer
    minimal: bool = false,
    /// Makes the keyboard take all focus and dismiss when clicking off the panel
    modal: bool = false,
    /// Shows arrow keys on the keyboard when in minimal mode. Buffered (non-minimal) mode always has them. In minimal
    /// mode, when arrow keys are pressed, they send ANSI escape sequences (e.g. "\x1b[D" for left arrow).
    show_arrow_keys: bool = false,
    /// Shows the hide keyboard button instead of a Done button. The Done key sends a Event.keyboard_done when
    /// clicked. Hide only sends the Closed event.
    hide_done_key: bool = false,

    _padding: u28 = 0,
};

pub const GamepadTextInputMode = enum(i32) {
    normal = 0,
    password = 1,
    submit = 2,
};

pub const GamepadTextInputLineMode = enum(i32) {
    single_line = 0,
    multiple_lines = 1,
};

pub const MessageOverlayResponse = enum(u32) {
    button_press_0 = 0,
    button_press_1 = 1,
    button_press_2 = 2,
    button_press_3 = 3,
    couldnt_find_system_overlay = 4,
    couldnt_find_or_create_client_overlay = 5,
    application_quit = 6,
};

pub const VRMouseButton = packed struct(u32) {
    Left: bool = false,
    Right: bool = false,
    Middle: bool = false,

    _padding: u29 = 0,
};

pub fn RawImage(comptime T: type, comptime bytes_per_pixel: u32) type {
    comptime {
        switch (@typeInfo(T)) {
            .Int => |Int| if (Int.bits % 8 != 0) @compileError("type needs to be a whole number of bytes"),
            .Struct => |Struct| {
                if (Struct.layout != .@"packed") @compileError("struct needs to be packed");
                if (@typeInfo(Struct.backing_integer.?).Int.bits != 8 * bytes_per_pixel)
                    @compileError("struct has to be 1 pixel big (u" ++ 8 * bytes_per_pixel ++ ")");
            },
            else => @compileError("type '" ++ @typeName(T) ++ "' is not allowed here"),
        }
    }
    const is_struct = comptime @typeInfo(T) == .Struct;
    const type_bytes: u16 = comptime switch (@typeInfo(T)) {
        .Struct => |Struct| @typeInfo(Struct.backing_integer.?).Int.bits / 8,
        .Int => |Int| Int.bits / 8,
        else => unreachable,
    };

    return struct {
        const Sself = @This();
        allocator: std.mem.Allocator,

        width: u32,
        height: u32,
        bytes_per_pixel: u32,

        data: []T,

        pub fn init(allocator: std.mem.Allocator) !Sself {
            var image = Sself{
                .width = 0,
                .height = 0,
                .bytes_per_pixel = bytes_per_pixel,
                .data = undefined,

                .allocator = allocator,
            };
            try image.makeData();
            return image;
        }
        pub fn deinit(self: Sself) void {
            if (self.data == null) return;
            self.allocator.free(self.data);
        }
        pub fn makeData(self: Sself) !void {
            std.debug.assert(self.bytes_per_pixel > 0);
            std.debug.assert(self.bytes_per_pixel >= type_bytes);
            if (is_struct) std.debug.assert(self.bytes_per_pixel == bytes_per_pixel);
            std.debug.assert(self.bytes_per_pixel % type_bytes == 0);

            self.deinit();
            self.data = try self.allocator.alloc(T, self.width * self.height * self.bytes_per_pixel);
        }
    };
}

pub const GLSharedTextureHandle = *const anyopaque;

pub const VulkanTextureData = extern struct {
    image: u64,
    device: renderers.vulkan.VkDevice,
    physical_device: renderers.vulkan.VkPhysicalDevice,
    instance: renderers.vulkan.VkInstance,
    queue: renderers.vulkan.VkQueue,
    queue_family_index: u32,
    width: u32,
    height: u32,
    format: u32,
    sample_count: u32,
};
pub const VRVulkanTextureArrayData = extern struct {
    array_index: u32,
    array_size: u32,
};

// can't call it OverlayView since the name will conflict in openvr.zig
pub const VROverlayView = extern struct {
    overlay_handle: OverlayHandle,
    texture: Texture,
    texture_bounds: TextureBounds,
};

pub const DeviceType = enum(i32) {
    ///Invalid handle
    invalid = -1,
    ///Handle is an ID3D11Device
    directX11 = 0,
    ///Handle is a pointer to a openvr.VulkanDevice structure
    vulkan = 1,
};

pub const VulkanDevice = extern struct {
    instance: vulkan.VkInstance,
    device: vulkan.VkDevice,
    physical_device: vulkan.VkPhysicalDevice,
    queue: vulkan.VkQueue,
    queue_family_index: u32,
};

pub const NativeDevice = extern struct {
    ///See DeviceType definition
    handle: *const anyopaque,
    type: DeviceType,
};
