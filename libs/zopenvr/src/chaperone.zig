const std = @import("std");

const common = @import("common.zig");

function_table: *FunctionTable,

const Self = @This();

const version = "IVRChaperone_004";
pub fn init() common.InitError!Self {
    return .{
        .function_table = try common.getFunctionTable(FunctionTable, version),
    };
}

pub fn getCalibrationState(self: Self) common.CalibrationState {
    return self.function_table.GetCalibrationState();
}

pub fn getPlayAreaSize(self: Self) ?common.PlayAreaSize {
    var play_area: common.PlayAreaSize = undefined;
    if (self.function_table.GetPlayAreaSize(&play_area.x, &play_area.z)) {
        return play_area;
    } else {
        return null;
    }
}

pub fn getPlayAreaRect(self: Self) ?common.Quad {
    var play_area: common.Quad = undefined;
    if (self.function_table.GetPlayAreaRect(&play_area)) {
        return play_area;
    } else {
        return null;
    }
}

pub fn reloadInfo(self: Self) void {
    self.function_table.ReloadInfo();
}

pub fn setSceneColor(self: Self, scene_color: common.Color) void {
    self.function_table.SetSceneColor(scene_color);
}

pub fn allocBoundsColor(self: Self, allocator: std.mem.Allocator, collision_bounds_fade_distance: f32, bound_colors_count: usize) !common.BoundsColor {
    var bounds_color: common.BoundsColor = undefined;
    bounds_color.bound_colors = try allocator.alloc(common.Color, bound_colors_count);
    self.function_table.GetBoundsColor(bounds_color.bound_colors.ptr, @intCast(bounds_color.bound_colors.len), collision_bounds_fade_distance, &bounds_color.camera_color);
    return bounds_color;
}

pub fn areBoundsVisible(self: Self) bool {
    return self.function_table.AreBoundsVisible();
}

pub fn forceBoundsVisible(self: Self, force: bool) void {
    self.function_table.ForceBoundsVisible(force);
}

pub fn resetZeroPose(self: Self, origin: common.TrackingUniverseOrigin) void {
    self.function_table.ResetZeroPose(origin);
}

const FunctionTable = extern struct {
    GetCalibrationState: *const fn () callconv(.C) common.CalibrationState,
    GetPlayAreaSize: *const fn (*f32, *f32) callconv(.C) bool,
    GetPlayAreaRect: *const fn (*common.Quad) callconv(.C) bool,
    ReloadInfo: *const fn () callconv(.C) void,
    SetSceneColor: *const fn (common.Color) callconv(.C) void,
    GetBoundsColor: *const fn ([*c]common.Color, c_int, f32, *common.Color) callconv(.C) void,
    AreBoundsVisible: *const fn () callconv(.C) bool,
    ForceBoundsVisible: *const fn (bool) callconv(.C) void,
    ResetZeroPose: *const fn (common.TrackingUniverseOrigin) callconv(.C) void,
};
