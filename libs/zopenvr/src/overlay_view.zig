const std = @import("std");

const common = @import("common.zig");

function_table: *FunctionTable,

const Self = @This();

const version = "IVROverlayView_003";
pub fn init() common.InitError!Self {
    return .{
        .function_table = try common.getFunctionTable(FunctionTable, version),
    };
}

pub fn acquireOverlayView(self: Self, overlay_handle: common.OverlayHandle) common.OverlayError!struct { native_device: common.NativeDevice, overlay_view: common.VROverlayView } {
    var overlay_view: common.VROverlayView = undefined;
    var native_device: common.NativeDevice = undefined; // pretty sure this is an output
    const err = self.function_table.AcquireOverlayView(overlay_handle, &native_device, &overlay_view, @sizeOf(common.VROverlayView));
    try err.maybe();
    return .{ .native_device = native_device, .overlay_view = overlay_view };
}

pub fn releaseOverlayView(self: Self, overlay_view: *common.VROverlayView) common.OverlayError!void {
    try self.function_table.ReleaseOverlayView(overlay_view).maybe();
}

pub fn postOverlayEvent(self: Self, overlay_handle: common.OverlayHandle, event: *const common.Event) void {
    self.function_table.PostOverlayEvent(overlay_handle, event);
}

pub fn isViewingPermitted(self: Self, overlay_handle: common.OverlayHandle) bool {
    return self.function_table.IsViewingPermitted(overlay_handle);
}

const FunctionTable = extern struct {
    AcquireOverlayView: *const fn (common.OverlayHandle, *common.NativeDevice, *common.VROverlayView, u32) callconv(.C) common.OverlayErrorCode,
    ReleaseOverlayView: *const fn (*common.VROverlayView) callconv(.C) common.OverlayErrorCode,
    PostOverlayEvent: *const fn (common.OverlayHandle, *const common.Event) callconv(.C) void,
    IsViewingPermitted: *const fn (common.OverlayHandle) callconv(.C) bool,
};
