const std = @import("std");
const zwin32 = @import("zwin32");
const d3d12 = zwin32.d3d12;

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

extern fn VR_GetVRInitErrorAsSymbol(InitErrorCode) callconv(.C) [*c]u8;
extern fn VR_GetVRInitErrorAsEnglishDescription(InitErrorCode) callconv(.C) [*c]u8;

test "init error have english descriptions" {
    try std.testing.expectEqualStrings("No Error (0)", InitErrorCode.none.asEnglishDescription());
}

pub fn getFunctionTable(comptime T: type, comptime version: []const u8) InitError!*T {
    const interface_name: [*c]const u8 = "FnTable:" ++ version;

    var init_error: InitErrorCode = .none;
    const function_table: *T = @ptrCast(VR_GetGenericInterface(interface_name, &init_error));
    try init_error.maybe();
    return function_table;
}

extern fn VR_GetGenericInterface([*c]const u8, *InitErrorCode) callconv(.C) *isize;

pub const Quad = extern struct {
    corners: [4]Vector3,
};

pub const Vector3 = extern struct {
    v: [3]f32,
};

pub const Vector4 = extern struct {
    v: [4]f32,
};

pub const Matrix34 = extern struct {
    m: [3][4]f32,
};

pub const Matrix44 = extern struct {
    m: [4][4]f32,
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

pub const TrackedDevicePose = extern struct {
    device_to_absolute_tracking: Matrix34,
    velocity: Vector3,
    angular_velocity: Vector3,
    tracking_result: TrackingResult,
    pose_is_valid: bool,
    device_is_connected: bool,
};

pub const TextureType = enum(i32) {
    invalid = -1,
    directx = 0,
    opengl = 1,
    vulkan = 2,
    iosurface = 3,
    directx12 = 4,
    dxgi_shared_handle = 5,
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
    invalid = 0,
    left_hand = 1,
    right_hand = 2,
    opt_out = 3,
    treadmill = 4,
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
    x: f32,
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

    app_key_already_exists = 100, // Only one application can use any given key
    no_manifest = 101, // the running application does not have a manifest
    no_application = 102, // No application is running
    invalid_index = 103,
    unknown_application = 104, // the application could not be found
    ipc_failed = 105, // An IPC failure caused the request to fail
    application_already_running = 106,
    invalid_manifest = 107,
    invalid_application = 108,
    launch_failed = 109, // the process didn't start
    application_already_starting = 110, // the system was already starting the same application
    launch_in_progress = 111, // The system was already starting a different application
    old_application_quitting = 112,
    transition_aborted = 113,
    is_template = 114, // error when you try to call LaunchApplication() on a template type app (use LaunchTemplateApplication)
    steam_vr_is_exiting = 115,

    buffer_too_small = 200, // The provided buffer was too small to fit the requested data
    property_not_set = 201, // The requested property was not set
    unknown_property = 202,
    invalid_parameter = 203,

    not_implemented = 300, // Fcn is not implemented in current interface

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
    none = 0,
    starting = 1,
    quitting = 2,
    running = 3,
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
    auto = 0,
    gamma = 1,
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
