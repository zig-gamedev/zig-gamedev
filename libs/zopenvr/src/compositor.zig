const std = @import("std");

const common = @import("common.zig");

function_table: *FunctionTable,

const Self = @This();

const version = "IVRCompositor_028";
pub fn init() common.InitError!Self {
    return .{
        .function_table = try common.getFunctionTable(FunctionTable, version),
    };
}

pub fn setTrackingSpace(self: Self, origin: common.TrackingUniverseOrigin) void {
    self.function_table.SetTrackingSpace(origin);
}

pub fn getTrackingSpace(self: Self) common.TrackingUniverseOrigin {
    return self.function_table.GetTrackingSpace();
}

pub fn allocWaitPoses(self: Self, allocator: std.mem.Allocator, render_poses_count: usize, game_poses_count: usize) (common.CompositorError || error{OutOfMemory})!common.CompositorPoses {
    const poses = try common.CompositorPoses.allocInit(allocator, render_poses_count, game_poses_count);
    errdefer poses.deinit(allocator);

    const compositor_error = self.function_table.WaitGetPoses(@ptrCast(poses.render_poses.ptr), @intCast(render_poses_count), @ptrCast(poses.game_poses.ptr), @intCast(game_poses_count));
    try compositor_error.maybe();

    return poses;
}

pub fn allocLastPoses(self: Self, allocator: std.mem.Allocator, render_poses_count: usize, game_poses_count: usize) (common.CompositorError || error{OutOfMemory})!common.CompositorPoses {
    const poses = try common.CompositorPoses.allocInit(allocator, render_poses_count, game_poses_count);
    errdefer poses.deinit(allocator);

    const compositor_error = self.function_table.GetLastPoses(@ptrCast(poses.render_poses.ptr), @intCast(render_poses_count), @ptrCast(poses.game_poses.ptr), @intCast(game_poses_count));
    try compositor_error.maybe();

    return poses;
}

pub fn getLastPoseForTrackedDeviceIndex(self: Self, device_index: common.TrackedDeviceIndex) common.CompositorError!common.CompositorPose {
    var pose: common.CompositorPose = undefined;
    const compositor_error = self.function_table.GetLastPoseForTrackedDeviceIndex(device_index, &pose.render_pose, &pose.game_pose);
    try compositor_error.maybe();

    return pose;
}

pub fn getLastRenderPoseForTrackedDeviceIndex(self: Self, device_index: common.TrackedDeviceIndex) common.CompositorError!common.TrackedDevicePose {
    var pose: common.TrackedDevicePose = undefined;
    const compositor_error = self.function_table.GetLastPoseForTrackedDeviceIndex(device_index, &pose, null);
    try compositor_error.maybe();

    return pose;
}

pub fn getLastGamePoseForTrackedDeviceIndex(self: Self, device_index: common.TrackedDeviceIndex) common.CompositorError!common.TrackedDevicePose {
    var pose: common.TrackedDevicePose = undefined;
    const compositor_error = self.function_table.GetLastPoseForTrackedDeviceIndex(device_index, null, &pose);
    try compositor_error.maybe();

    return pose;
}

pub fn submit(self: Self, eye: common.Eye, texture: *const common.Texture, texture_bounds: ?common.TextureBounds, flags: common.SubmitFlags) common.CompositorError!void {
    const compositor_error = self.function_table.Submit(eye, texture, if (texture_bounds) |tb| &tb else null, flags);
    try compositor_error.maybe();
}

pub fn submitWithArrayIndex(self: Self, eye: common.Eye, textures: [*]common.Texture, index: u32, texture_bounds: ?common.TextureBounds, flags: common.SubmitFlags) common.CompositorError!void {
    const compositor_error = self.function_table.SubmitWithArrayIndex(eye, textures, index, texture_bounds, flags);
    try compositor_error.maybe();
}

pub fn clearLastSubmittedFrame(self: Self) void {
    self.function_table.ClearLastSubmittedFrame();
}

pub fn postPresentHandoff(self: Self) void {
    self.function_table.PostPresentHandoff();
}

pub fn getFrameTiming(self: Self, frames_ago: u32) ?common.FrameTiming {
    var frame_timing: common.FrameTiming = undefined;
    frame_timing.size = @sizeOf(common.FrameTiming);
    if (self.function_table.GetFrameTiming(&frame_timing, frames_ago)) {
        return frame_timing;
    } else {
        return null;
    }
}

pub fn allocFrameTimings(self: Self, allocator: std.mem.Allocator, count: u32) ![]common.FrameTiming {
    var frame_timings = try allocator.alloc(common.FrameTiming, count);
    errdefer allocator.free(frame_timings);

    if (count > 0) {
        frame_timings[0].size = @sizeOf(common.FrameTiming);
        const actual_count = self.function_table.GetFrameTimings(frame_timings.ptr, count);
        frame_timings = try allocator.realloc(frame_timings, actual_count);
    }
    return frame_timings;
}

pub fn getFrameTimeRemaining(self: Self) f32 {
    return self.function_table.GetFrameTimeRemaining();
}

pub fn getCumulativeStats(self: Self) common.CumulativeStats {
    var cummulative_stats: common.CumulativeStats = undefined;
    self.function_table.GetCumulativeStats(&cummulative_stats, @sizeOf(common.CumulativeStats));
    return cummulative_stats;
}

pub fn fadeToColor(self: Self, seconds: f32, color: common.Color, background: bool) void {
    self.function_table.FadeToColor(seconds, color.r, color.g, color.b, color.a, background);
}

pub fn getCurrentFadeColor(self: Self, background: bool) common.Color {
    return self.function_table.GetCurrentFadeColor(background);
}

pub fn fadeGrid(self: Self, seconds: f32, background: bool) void {
    self.function_table.FadeGrid(seconds, background);
}

pub fn getCurrentGridAlpha(self: Self) f32 {
    return self.function_table.GetCurrentGridAlpha();
}

pub fn setSkyboxOverride(self: Self, skybox: common.Skybox) common.CompositorError!void {
    const textures = skybox.asSlice();
    const compositor_error = self.function_table.SetSkyboxOverride(textures.ptr, textures.len);
    try compositor_error.maybe();
}

pub fn clearSkyboxOverride(self: Self) void {
    self.function_table.ClearSkyboxOverride();
}

pub fn compositorBringToFront(self: Self) void {
    self.function_table.CompositorBringToFront();
}

pub fn compositorGoToBack(self: Self) void {
    self.function_table.CompositorGoToBack();
}

pub fn compositorQuit(self: Self) void {
    self.function_table.CompositorQuit();
}

pub fn isFullscreen(self: Self) bool {
    return self.function_table.IsFullscreen();
}

pub fn getCurrentSceneFocusProcess(self: Self) u32 {
    return self.function_table.GetCurrentSceneFocusProcess();
}

pub fn getLastFrameRenderer(self: Self) u32 {
    return self.function_table.GetLastFrameRenderer();
}

pub fn canRenderScene(self: Self) bool {
    return self.function_table.CanRenderScene();
}

pub fn showMirrorWindow(self: Self) void {
    self.function_table.ShowMirrorWindow();
}

pub fn hideMirrorWindow(self: Self) void {
    self.function_table.HideMirrorWindow();
}

pub fn isMirrorWindowVisible(self: Self) bool {
    return self.function_table.IsMirrorWindowVisible();
}

pub fn compositorDumpImages(self: Self) void {
    self.function_table.CompositorDumpImages();
}

pub fn shouldAppRenderWithLowResources(self: Self) bool {
    return self.function_table.ShouldAppRenderWithLowResources();
}

pub fn forceInterleavedReprojectionOn(self: Self, override: bool) void {
    return self.function_table.ForceInterleavedReprojectionOn(override);
}

pub fn forceReconnectProcess(self: Self) void {
    self.function_table.ForceReconnectProcess();
}

pub fn suspendRendering(self: Self, suspend_rendering: bool) void {
    self.function_table.SuspendRendering(suspend_rendering);
}

pub fn setExplicitTimingMode(self: Self, timing_mode: common.TimingMode) void {
    self.function_table.SetExplicitTimingMode(timing_mode);
}

pub fn submitExplicitTimingData(self: Self) common.CompositorError!void {
    const compositor_error = self.function_table.SubmitExplicitTimingData();
    try compositor_error.maybe();
}

pub fn isMotionSmoothingEnabled(self: Self) bool {
    return self.function_table.IsMotionSmoothingEnabled();
}

pub fn isMotionSmoothingSupported(self: Self) bool {
    return self.function_table.IsMotionSmoothingSupported();
}

pub fn isCurrentSceneFocusAppLoading(self: Self) bool {
    return self.function_table.IsCurrentSceneFocusAppLoading();
}

pub fn setStageOverrideAsync(self: Self, render_model_path: [:0]const u8, transform: common.Matrix34, stage_render_settings: common.StageRenderSettings) common.CompositorError!void {
    const compositor_error = self.function_table.SetStageOverride_Async(render_model_path.ptr, &transform, &stage_render_settings, @sizeOf(common.StageRenderSettings));
    try compositor_error.maybe();
}

pub fn clearStageOverride(self: Self) void {
    self.function_table.ClearStageOverride();
}

pub fn getCompositorBenchmarkResults(self: Self) ?common.BenchmarkResults {
    var benchmark_results: common.BenchmarkResults = undefined;
    if (self.function_table.GetCompositorBenchmarkResults(&benchmark_results, @sizeOf(common.BenchmarkResults))) {
        return benchmark_results;
    } else {
        return null;
    }
}

pub fn getLastPosePredictionIDs(self: Self) common.CompositorError!common.CompositorPosePredictionIDs {
    var prediction_ids: common.CompositorPosePredictionIDs = undefined;
    const compositor_error = self.function_table.GetLastPosePredictionIDs(&prediction_ids.render_pose_prediction_id, &prediction_ids.game_pose_prediction_id);
    try compositor_error.maybe();
    return prediction_ids;
}

pub fn allocPosesForFrame(self: Self, allocator: std.mem.Allocator, pose_prediction_id: u32, pose_count: u32) (common.CompositorError || error{OutOfMemory})![]common.TrackedDevicePose {
    const poses = try allocator.alloc(common.TrackedDevicePose, pose_count);
    const compositor_error = self.function_table.GetPosesForFrame(pose_prediction_id, poses.ptr, pose_count);
    try compositor_error.maybe();
    return poses;
}

const FunctionTable = extern struct {
    SetTrackingSpace: *const fn (common.TrackingUniverseOrigin) callconv(.C) void,
    GetTrackingSpace: *const fn () callconv(.C) common.TrackingUniverseOrigin,
    WaitGetPoses: *const fn (*common.TrackedDevicePose, u32, *common.TrackedDevicePose, u32) callconv(.C) common.CompositorErrorCode,
    GetLastPoses: *const fn (*common.TrackedDevicePose, u32, *common.TrackedDevicePose, u32) callconv(.C) common.CompositorErrorCode,
    GetLastPoseForTrackedDeviceIndex: *const fn (common.TrackedDeviceIndex, *common.TrackedDevicePose, *common.TrackedDevicePose) callconv(.C) common.CompositorErrorCode,
    Submit: *const fn (common.Eye, *const common.Texture, ?*const common.TextureBounds, common.SubmitFlags) callconv(.C) common.CompositorErrorCode,
    SubmitWithArrayIndex: *const fn (common.Eye, [*]common.Texture, u32, *common.TextureBounds, common.SubmitFlags) callconv(.C) common.CompositorErrorCode,
    ClearLastSubmittedFrame: *const fn () callconv(.C) void,
    PostPresentHandoff: *const fn () callconv(.C) void,
    GetFrameTiming: *const fn (*common.FrameTiming, u32) callconv(.C) bool,
    GetFrameTimings: *const fn ([*c]common.FrameTiming, u32) callconv(.C) u32,
    GetFrameTimeRemaining: *const fn () callconv(.C) f32,
    GetCumulativeStats: *const fn (*common.CumulativeStats, u32) callconv(.C) void,
    FadeToColor: *const fn (f32, f32, f32, f32, f32, bool) callconv(.C) void,
    GetCurrentFadeColor: *const fn (bool) callconv(.C) common.Color,
    FadeGrid: *const fn (f32, bool) callconv(.C) void,
    GetCurrentGridAlpha: *const fn () callconv(.C) f32,
    SetSkyboxOverride: *const fn (*common.Texture, u32) callconv(.C) common.CompositorErrorCode,
    ClearSkyboxOverride: *const fn () callconv(.C) void,
    CompositorBringToFront: *const fn () callconv(.C) void,
    CompositorGoToBack: *const fn () callconv(.C) void,
    CompositorQuit: *const fn () callconv(.C) void,
    IsFullscreen: *const fn () callconv(.C) bool,
    GetCurrentSceneFocusProcess: *const fn () callconv(.C) u32,
    GetLastFrameRenderer: *const fn () callconv(.C) u32,
    CanRenderScene: *const fn () callconv(.C) bool,
    ShowMirrorWindow: *const fn () callconv(.C) void,
    HideMirrorWindow: *const fn () callconv(.C) void,
    IsMirrorWindowVisible: *const fn () callconv(.C) bool,
    CompositorDumpImages: *const fn () callconv(.C) void,
    ShouldAppRenderWithLowResources: *const fn () callconv(.C) bool,
    ForceInterleavedReprojectionOn: *const fn (bool) callconv(.C) void,
    ForceReconnectProcess: *const fn () callconv(.C) void,
    SuspendRendering: *const fn (bool) callconv(.C) void,

    // skip over d3d11
    GetMirrorTextureD3D11: usize,
    ReleaseMirrorTextureD3D11: usize,

    // skip over opengl
    GetMirrorTextureGL: usize,
    ReleaseSharedGLTexture: usize,
    LockGLSharedTextureForAccess: usize,
    UnlockGLSharedTextureForAccess: usize,

    // skip over vulkan
    GetVulkanInstanceExtensionsRequired: usize,
    GetVulkanDeviceExtensionsRequired: usize,

    SetExplicitTimingMode: *const fn (common.TimingMode) callconv(.C) void,
    SubmitExplicitTimingData: *const fn () callconv(.C) common.CompositorErrorCode,
    IsMotionSmoothingEnabled: *const fn () callconv(.C) bool,
    IsMotionSmoothingSupported: *const fn () callconv(.C) bool,
    IsCurrentSceneFocusAppLoading: *const fn () callconv(.C) bool,
    SetStageOverride_Async: *const fn ([*c]const u8, *const common.Matrix34, *const common.StageRenderSettings, u32) callconv(.C) common.CompositorErrorCode,
    ClearStageOverride: *const fn () callconv(.C) void,
    GetCompositorBenchmarkResults: *const fn (*common.BenchmarkResults, u32) callconv(.C) bool,
    GetLastPosePredictionIDs: *const fn (*u32, *u32) callconv(.C) common.CompositorErrorCode,
    GetPosesForFrame: *const fn (u32, [*]common.TrackedDevicePose, u32) callconv(.C) common.CompositorErrorCode,
};
