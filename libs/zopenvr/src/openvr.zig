const std = @import("std");

const common = @import("common.zig");

const Self = @This();

pub fn init(application_type: ApplicationType) common.InitError!Self {
    var init_error: common.InitErrorCode = .none;
    _ = VR_InitInternal(&init_error, application_type) orelse
        try init_error.maybe();
    return .{};
}

pub const ApplicationType = enum(i32) {
    ///Some other kind of application that isn't covered by the other entries
    other = 0,
    ///Application will submit 3D frames
    scene = 1,
    ///Application only interacts with overlays
    overlay = 2,
    ///Application should not start SteamVR if it's not already running, and should not
    ///keep it running if everything else quits.
    background = 3,
    ///Init should not try to load any drivers. The application needs access to utility
    ///interfaces (like openvr.Settings and openvr.Applications) but not hardware.
    utility = 4,
    ///Reserved for vrmonitor
    vr_monitor = 5,
    ///Reserved for Steam
    steam_watchdog = 6,
    ///reserved for vrstartup
    bootstrapper = 7,
    ///reserved for vrwebhelper
    web_helper = 8,
    ///reserved for openxr (created instance, but not session yet)
    open_xr_instance = 9,
    ///reserved for openxr (started session)
    open_xr_scene = 10,
    ///reserved for openxr (started overlay session)
    open_xr_overlay = 11,
    ///reserved for the vrprismhost process
    prism = 12,
    ///reserved for the RoomView process
    room_view = 13,
    max = 14,
};

extern "openvr_api" fn VR_InitInternal(*common.InitErrorCode, ApplicationType) callconv(.C) ?*isize;
extern "openvr_api" fn VR_ShutdownInternal() callconv(.C) void;
extern "openvr_api" fn VR_IsHmdPresent() callconv(.C) bool;
extern "openvr_api" fn VR_IsRuntimeInstalled() callconv(.C) bool;

pub fn deinit(_: Self) void {
    VR_ShutdownInternal();
}

pub const isHmdPresent = VR_IsHmdPresent;
pub const isRuntimeInstalled = VR_IsRuntimeInstalled;

pub fn system(_: Self) common.InitError!System {
    return try System.init();
}

pub fn chaperone(_: Self) common.InitError!Chaperone {
    return try Chaperone.init();
}

pub fn compositor(_: Self) common.InitError!Compositor {
    return try Compositor.init();
}

pub fn applications(_: Self) common.InitError!Applications {
    return try Applications.init();
}

pub fn input(_: Self) common.InitError!Input {
    return try Input.init();
}

pub fn renderModels(_: Self) common.InitError!RenderModels {
    return try RenderModels.init();
}

pub fn overlay(_: Self) common.InitError!Overlay {
    return try Overlay.init();
}

pub fn overlayView(_: Self) common.InitError!Overlay {
    return try OverlayView.init();
}

pub usingnamespace @import("common.zig");
pub const System = @import("system.zig");
pub const Chaperone = @import("chaperone.zig");
pub const Compositor = @import("compositor.zig");
pub const Applications = @import("applications.zig");
pub const Input = @import("input.zig");
pub const RenderModels = @import("render_models.zig");
pub const Overlay = @import("overlay.zig");
pub const OverlayView = @import("overlay_view.zig");
