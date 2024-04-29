const std = @import("std");
const zwin32 = @import("zwin32");
const d3d12 = zwin32.d3d12;

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
    fn maybe(compositor_error: CompositorErrorCode) CompositorError!void {
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

pub fn allocWaitPoses(self: Self, allocator: std.mem.Allocator, render_poses_count: usize, game_poses_count: usize) (CompositorError || error{OutOfMemory})!Poses {
    const poses = try Poses.allocInit(allocator, render_poses_count, game_poses_count);
    errdefer poses.deinit(allocator);

    const compositor_error = self.function_table.WaitGetPoses(@ptrCast(poses.render_poses.ptr), @intCast(render_poses_count), @ptrCast(poses.game_poses.ptr), @intCast(game_poses_count));
    try compositor_error.maybe();

    return poses;
}

pub const Poses = struct {
    render_poses: []common.TrackedDevicePose,
    game_poses: []common.TrackedDevicePose,

    pub fn allocInit(allocator: std.mem.Allocator, render_poses_count: usize, game_poses_count: usize) !Poses {
        return .{
            .render_poses = try allocator.alloc(common.TrackedDevicePose, render_poses_count),
            .game_poses = try allocator.alloc(common.TrackedDevicePose, game_poses_count),
        };
    }

    pub fn deinit(self: Poses, allocator: std.mem.Allocator) void {
        allocator.free(self.render_poses);
        allocator.free(self.game_poses);
    }
};

pub fn allocLastPoses(self: Self, allocator: std.mem.Allocator, render_poses_count: usize, game_poses_count: usize) (CompositorError || error{OutOfMemory})!Poses {
    const poses = try Poses.allocInit(allocator, render_poses_count, game_poses_count);
    errdefer poses.deinit(allocator);

    const compositor_error = self.function_table.GetLastPoses(@ptrCast(poses.render_poses.ptr), @intCast(render_poses_count), @ptrCast(poses.game_poses.ptr), @intCast(game_poses_count));
    try compositor_error.maybe();

    return poses;
}

pub const Pose = struct {
    render_pose: common.TrackedDevicePose,
    game_pose: common.TrackedDevicePose,
};

pub fn getLastPoseForTrackedDeviceIndex(self: Self, device_index: common.TrackedDeviceIndex) CompositorError!Pose {
    var pose: Pose = undefined;
    const compositor_error = self.function_table.GetLastPoseForTrackedDeviceIndex(device_index, &pose.render_pose, &pose.game_pose);
    try compositor_error.maybe();

    return pose;
}

pub fn getLastRenderPoseForTrackedDeviceIndex(self: Self, device_index: common.TrackedDeviceIndex) CompositorError!common.TrackedDevicePose {
    var pose: common.TrackedDevicePose = undefined;
    const compositor_error = self.function_table.GetLastPoseForTrackedDeviceIndex(device_index, &pose, null);
    try compositor_error.maybe();

    return pose;
}

pub fn getLastGamePoseForTrackedDeviceIndex(self: Self, device_index: common.TrackedDeviceIndex) CompositorError!common.TrackedDevicePose {
    var pose: common.TrackedDevicePose = undefined;
    const compositor_error = self.function_table.GetLastPoseForTrackedDeviceIndex(device_index, null, &pose);
    try compositor_error.maybe();

    return pose;
}

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
    texture_type: common.TextureType,
    color_space: ColorSpace,
};
pub const TextureBounds = extern struct {
    u_min: f32,
    v_min: f32,
    u_max: f32,
    v_max: f32,
};

pub fn submit(self: Self, eye: common.Eye, texture: *const Texture, texture_bounds: ?TextureBounds, flags: SubmitFlags) CompositorError!void {
    const compositor_error = self.function_table.Submit(eye, texture, if (texture_bounds) |tb| &tb else null, flags);
    try compositor_error.maybe();
}

pub fn submitWithArrayIndex(self: Self, eye: common.Eye, texture: *Texture, index: u32, texture_bounds: ?TextureBounds, flags: SubmitFlags) CompositorError!void {
    const compositor_error = self.function_table.SubmitWithArrayIndex(eye, texture, index, texture_bounds, flags);
    try compositor_error.maybe();
}

pub fn clearLastSubmittedFrame(self: Self) void {
    self.function_table.ClearLastSubmittedFrame();
}

pub fn postPresentHandoff(self: Self) void {
    self.function_table.PostPresentHandoff();
}

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
    pose: common.TrackedDevicePose,
    num_v_syncs_ready_for_use: u32,
    num_v_syncs_to_first_view: u32,
};

pub fn getFrameTiming(self: Self, frames_ago: u32) ?FrameTiming {
    var frame_timing: FrameTiming = undefined;
    frame_timing.size = @sizeOf(FrameTiming);
    if (self.function_table.GetFrameTiming(&frame_timing, frames_ago)) {
        return frame_timing;
    } else {
        return null;
    }
}

pub fn allocFrameTimings(self: Self, allocator: std.mem.Allocator, count: u32) ![]FrameTiming {
    var frame_timings = try allocator.alloc(FrameTiming, count);
    errdefer allocator.free(frame_timings);

    if (count > 0) {
        frame_timings[0].size = @sizeOf(FrameTiming);
        const actual_count = self.function_table.GetFrameTimings(frame_timings.ptr, count);
        frame_timings = try allocator.realloc(frame_timings, actual_count);
    }
    return frame_timings;
}

pub fn getFrameTimeRemaining(self: Self) f32 {
    return self.function_table.GetFrameTimeRemaining();
}

pub const shared_double = f64;
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
    sum_compositor_cpu_time_ms: shared_double,
    sum_compositor_gpu_time_ms: shared_double,
    sum_target_frame_times: shared_double,
    sum_application_cpu_time_ms: shared_double,
    sum_application_gpu_time_ms: shared_double,
    num_frames_with_depth: u32,
};

pub fn getCumulativeStats(self: Self) CumulativeStats {
    var cummulative_stats: CumulativeStats = undefined;
    self.function_table.GetCumulativeStats(&cummulative_stats, @sizeOf(CumulativeStats));
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

pub const Skybox = union(enum) {
    full: extern struct {
        front: common.Texture,
        back: common.Texture,
        left: common.Texture,
        right: common.Texture,
        top: common.Texture,
        bottom: common.Texture,
    },
    single_lat_long: common.Texture,
    stereo_lat_long: extern struct {
        left: common.Texture,
        right: common.Texture,
    },

    pub fn asSlice(self: Skybox) []common.Texture {
        return switch (self) {
            else => |skybox| std.mem.bytesAsSlice(common.Texture, std.mem.toBytes(skybox)),
        };
    }
};

pub fn setSkyboxOverride(self: Self, skybox: Skybox) CompositorError!void {
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

//pub fn showMirrorWindow(self: Self) void {
//    self.function_table.ShowMirrorWindow();
//}

//pub fn hideMirrorWindow(self: Self) void {
//    self.function_table.HideMirrorWindow();
//}

//pub fn isMirrorWindowVisible(self: Self) bool {
//    return self.function_table.IsMirrorWindowVisible();
//}

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

//pub  fn getMirrorTextureD3D11(self: Self, eye: common.Eye, d3d11_device_or_resource: *anyopaque) CompositorError!?*anyopaque {
//    self.function_table.GetMirrorTextureD3D11();
//}

//pub  fn releaseMirrorTextureD3D11(self: Self, d3d11_shader_resource_view: *anyopaque) void {
//    self.function_table.ReleaseMirrorTextureD3D11();
//}

//pub  fn getMirrorTextureGL(self: Self, eye: common.Eye, texture_id: *glUInt) CompositorError!GlSharedTextureHandle {
//    self.function_table.GetMirrorTextureGL();
//}

//pub  fn releaseSharedGLTexture(self: Self, texture_id: glUInt, shared_texture_handle: GlSharedTextureHandle) bool {
//    self.function_table.ReleaseSharedGLTexture();
//}

//pub  fn lockGLSharedTextureForAccess(self: Self shared_texture_handle: GlSharedTextureHandle) void {
//    self.function_table.LockGLSharedTextureForAccess();
//}

//pub  fn unlockGLSharedTextureForAccess(self: Self shared_texture_handle: GlSharedTextureHandle) void {
//    self.function_table.UnlockGLSharedTextureForAccess();
//}

//pub fn allocVulkanInstanceExtensionsRequired(self: Self, allocator: std.mem.Allocator) ![][]u8 {
//    self.function_table.GetVulkanInstanceExtensionsRequired(;
//}

//pub  fn getVulkanDeviceExtensionsRequired(self: Self, *VkPhysicalDevice, allocator: std.mem.Allocator) ![][]u8 {
//    self.function_table.GetVulkanDeviceExtensionsRequired();
//}

pub const TimingMode = enum(i32) {
    implicit = 0,
    explicit_runtime_performs_post_present_handoff = 1,
    explicit_application_performs_post_present_handoff = 2,
};
pub fn setExplicitTimingMode(self: Self, timing_mode: TimingMode) void {
    self.function_table.SetExplicitTimingMode(timing_mode);
}

pub fn submitExplicitTimingData(self: Self) CompositorError!void {
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

pub const StageRenderSettings = extern struct {
    primary_color: common.Color,
    secondary_color: common.Color,
    vignette_inner_radius: f32,
    vignette_outer_radius: f32,
    fresnel_strength: f32,
    backface_culling: bool,
    greyscale: bool,
    wireframe: bool,
};
pub fn setStageOverrideAsync(self: Self, render_model_path: []const u8, transform: common.Matrix34, stage_render_settings: StageRenderSettings) CompositorError!void {
    const compositor_error = self.function_table.SetStageOverride_Async(render_model_path, &transform, &stage_render_settings, @sizeOf(StageRenderSettings));
    try compositor_error.maybe();
}

pub fn clearStageOverride(self: Self) void {
    self.function_table.ClearStageOverride();
}

pub const BenchmarkResults = extern struct {
    mega_pixels_per_second: f32,
    hmd_recommended_mega_pixels_per_second: f32,
};
pub fn getCompositorBenchmarkResults(self: Self) ?BenchmarkResults {
    var benchmark_results: BenchmarkResults = undefined;
    if (self.function_table.GetCompositorBenchmarkResults(&benchmark_results)) {
        return benchmark_results;
    } else {
        return null;
    }
}

pub const PosePredictionIDs = struct { render_pose_prediction_id: u32, game_pose_prediction_id: u32 };
pub fn getLastPosePredictionIDs(self: Self) CompositorError!PosePredictionIDs {
    var prediction_ids: PosePredictionIDs = undefined;
    const compositor_error = self.function_table.GetLastPosePredictionIDs(&prediction_ids.render_pose_prediction_id, &prediction_ids.gamer_pose_prediction_id);
    try compositor_error.maybe();
    return prediction_ids;
}

pub fn PosesForFrame(self: Self, allocator: std.mem.Allocator, pose_prediction_id: u32, pose_count: usize) CompositorError![]common.TrackedDevicePose {
    const device_poses = try allocator.alloc(u8, pose_count);
    const compositor_error = self.function_table.GetPosesForFrame(pose_prediction_id, device_poses.ptr, device_poses.len);
    try compositor_error.maybe();
    return device_poses;
}

const glUInt = anyopaque;
const glSharedTextureHandle = anyopaque;
const VkPhysicalDevice = anyopaque;
pub const FunctionTable = extern struct {
    SetTrackingSpace: *const fn (common.TrackingUniverseOrigin) callconv(.C) void,
    GetTrackingSpace: *const fn () callconv(.C) common.TrackingUniverseOrigin,
    WaitGetPoses: *const fn (*common.TrackedDevicePose, u32, *common.TrackedDevicePose, u32) callconv(.C) CompositorErrorCode,
    GetLastPoses: *const fn (*common.TrackedDevicePose, u32, *common.TrackedDevicePose, u32) callconv(.C) CompositorErrorCode,
    GetLastPoseForTrackedDeviceIndex: *const fn (common.TrackedDeviceIndex, *common.TrackedDevicePose, *common.TrackedDevicePose) callconv(.C) CompositorErrorCode,
    Submit: *const fn (common.Eye, *const Texture, ?*const TextureBounds, SubmitFlags) callconv(.C) CompositorErrorCode,
    SubmitWithArrayIndex: *const fn (common.Eye, *Texture, u32, *TextureBounds, SubmitFlags) callconv(.C) CompositorErrorCode,
    ClearLastSubmittedFrame: *const fn () callconv(.C) void,
    PostPresentHandoff: *const fn () callconv(.C) void,
    GetFrameTiming: *const fn (*FrameTiming, u32) callconv(.C) bool,
    GetFrameTimings: *const fn ([*c]FrameTiming, u32) callconv(.C) u32,
    GetFrameTimeRemaining: *const fn () callconv(.C) f32,
    GetCumulativeStats: *const fn (*CumulativeStats, u32) callconv(.C) void,
    FadeToColor: *const fn (f32, f32, f32, f32, f32, bool) callconv(.C) void,
    GetCurrentFadeColor: *const fn (bool) callconv(.C) common.Color,
    FadeGrid: *const fn (f32, bool) callconv(.C) void,
    GetCurrentGridAlpha: *const fn () callconv(.C) f32,
    SetSkyboxOverride: *const fn (*Texture, u32) callconv(.C) CompositorErrorCode,
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
    GetMirrorTextureD3D11: *const fn (common.Eye, ?*anyopaque, *?*anyopaque) callconv(.C) CompositorErrorCode,
    ReleaseMirrorTextureD3D11: *const fn (?*anyopaque) callconv(.C) void,
    GetMirrorTextureGL: *const fn (common.Eye, *glUInt, *glSharedTextureHandle) callconv(.C) CompositorErrorCode,
    ReleaseSharedGLTexture: usize,
    LockGLSharedTextureForAccess: usize,
    UnlockGLSharedTextureForAccess: usize,
    //ReleaseSharedGLTexture: *const fn (glUInt, glSharedTextureHandle) callconv(.C) bool,
    //LockGLSharedTextureForAccess: *const fn (glSharedTextureHandle) callconv(.C) void,
    //UnlockGLSharedTextureForAccess: *const fn (glSharedTextureHandle) callconv(.C) void,
    GetVulkanInstanceExtensionsRequired: *const fn ([*c]u8, u32) callconv(.C) u32,
    GetVulkanDeviceExtensionsRequired: *const fn (?*VkPhysicalDevice, [*c]u8, u32) callconv(.C) u32,
    SetExplicitTimingMode: *const fn (TimingMode) callconv(.C) void,
    SubmitExplicitTimingData: *const fn () callconv(.C) CompositorErrorCode,
    IsMotionSmoothingEnabled: *const fn () callconv(.C) bool,
    IsMotionSmoothingSupported: *const fn () callconv(.C) bool,
    IsCurrentSceneFocusAppLoading: *const fn () callconv(.C) bool,
    SetStageOverride_Async: *const fn ([*c]u8, *common.Matrix34, *StageRenderSettings, u32) callconv(.C) CompositorErrorCode,
    ClearStageOverride: *const fn () callconv(.C) void,
    GetCompositorBenchmarkResults: *const fn (BenchmarkResults, u32) callconv(.C) bool,
    GetLastPosePredictionIDs: *const fn (u32, u32) callconv(.C) CompositorErrorCode,
    GetPosesForFrame: *const fn (u32, common.TrackedDevicePose, u32) callconv(.C) CompositorErrorCode,
};
