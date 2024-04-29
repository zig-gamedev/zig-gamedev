const std = @import("std");

const common = @import("common.zig");

function_table: *FunctionTable,

const Self = @This();
const version = "IVRApplications_007";
pub fn init() common.InitError!Self {
    return .{
        .function_table = try common.getFunctionTable(FunctionTable, version),
    };
}

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

pub fn addApplicationManifest(self: Self, application_manifest_full_path: [:0]const u8, temporary: bool) ApplicationError!void {
    const error_code = self.function_table.AddApplicationManifest(@constCast(application_manifest_full_path.ptr), temporary);
    try error_code.maybe();
}
pub fn removeApplicationManifest(self: Self, application_manifest_full_path: [:0]const u8) ApplicationError!void {
    const error_code = self.function_table.RemoveApplicationManifest(@constCast(application_manifest_full_path.ptr));
    try error_code.maybe();
}
pub fn isApplicationInstalled(self: Self, app_key: [:0]const u8) bool {
    return self.function_table.IsApplicationInstalled(@constCast(app_key.ptr));
}
pub fn getApplicationCount(self: Self) u32 {
    return self.function_table.GetApplicationCount();
}
pub const max_application_key_length = 128;
pub fn allocApplicationKeyByIndex(self: Self, allocator: std.mem.Allocator, application_index: u32) (ApplicationError || error{OutOfMemory})![:0]u8 {
    var application_key_buffer: [max_application_key_length]u8 = undefined;

    const error_code = self.function_table.GetApplicationKeyByIndex(application_index, &application_key_buffer, @intCast(application_key_buffer.len));
    try error_code.maybe();

    const application_key_slice = std.mem.sliceTo(&application_key_buffer, 0);
    const application_key = try allocator.allocSentinel(u8, application_key_slice.len, 0);
    std.mem.copyForwards(u8, application_key, application_key_slice);
    return application_key;
}
pub fn allocApplicationKeyByProcessId(self: Self, allocator: std.mem.Allocator, process_id: u32) (ApplicationError || error{OutOfMemory})![:0]u8 {
    var application_key_buffer: [max_application_key_length]u8 = undefined;

    const error_code = self.function_table.GetApplicationKeyByProcessId(process_id, &application_key_buffer, @intCast(application_key_buffer.len));
    try error_code.maybe();

    const application_key_slice = std.mem.sliceTo(&application_key_buffer, 0);
    const application_key = try allocator.allocSentinel(u8, application_key_slice.len, 0);
    std.mem.copyForwards(u8, application_key, application_key_slice);
    return application_key;
}

pub fn launchApplication(self: Self, app_key: [:0]const u8) ApplicationError!void {
    const error_code = self.function_table.LaunchApplication(@constCast(app_key.ptr));
    try error_code.maybe();
}

pub const AppOverrideKeys = struct {
    key: [:0]u8,
    value: [:0]u8,
};
const ExternAppOverrideKeys = extern struct {
    key: [*c]u8,
    value: [*c]u8,
};

pub fn allocLaunchTemplateApplication(self: Self, allocator: std.mem.Allocator, template_app_key: [:0]const u8, new_app_key: [:0]const u8, keys: []AppOverrideKeys) (ApplicationError || error{OutOfMemory})!void {
    const extern_keys = try allocator.alloc(ExternAppOverrideKeys, keys.len);
    defer allocator.free(extern_keys);
    for (keys, 0..) |key, i| {
        extern_keys[i] = .{
            .key = key.key.ptr,
            .value = key.value.ptr,
        };
    }
    const error_code = self.function_table.LaunchTemplateApplication(@constCast(template_app_key.ptr), @constCast(new_app_key.ptr), extern_keys.ptr, @intCast(extern_keys.len));
    try error_code.maybe();
}

pub fn launchApplicationFromMimeType(self: Self, mime_type: [:0]const u8, args: [:0]const u8) ApplicationError!void {
    const error_code = self.function_table.LaunchApplicationFromMimeType(@constCast(mime_type.ptr), @constCast(args.ptr));
    try error_code.maybe();
}

pub fn launchDashboardOverlay(self: Self, app_key: [:0]const u8) ApplicationError!void {
    const error_code = self.function_table.LaunchDashboardOverlay(@constCast(app_key.ptr));
    try error_code.maybe();
}

pub fn cancelApplicationLaunch(self: Self, app_key: [:0]const u8) bool {
    return self.function_table.CancelApplicationLaunch(@constCast(app_key.ptr));
}

pub fn identifyApplication(self: Self, process_id: u32, app_key: [:0]const u8) ApplicationError!void {
    const error_code = self.function_table.IdentifyApplication(process_id, @constCast(app_key.ptr));
    try error_code.maybe();
}

pub fn getApplicationProcessId(self: Self, app_key: [:0]const u8) ApplicationError!u32 {
    return self.function_table.GetApplicationProcessId(@constCast(app_key.ptr));
}

pub fn getApplicationsErrorNameFromEnum(self: Self, error_code: ApplicationErrorCode) [:0]const u8 {
    return std.mem.span(self.function_table.GetApplicationsErrorNameFromEnum(error_code));
}

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

pub fn getApplicationProperty(self: Self, comptime T: type, app_key: [:0]const u8, property: ApplicationProperty.fromType(T)) ApplicationError!T {
    var error_code: ApplicationErrorCode = undefined;
    const result = switch (T) {
        bool => self.function_table.GetApplicationPropertyBool(@constCast(app_key.ptr), @enumFromInt(@intFromEnum(property)), &error_code),
        u64 => self.function_table.GetApplicationPropertyUint64(@constCast(app_key.ptr), @enumFromInt(@intFromEnum(property)), &error_code),
        else => @compileError("T must be bool, u64"),
    };
    try error_code.maybe();
    return result;
}

pub fn allocApplicationPropertyString(self: Self, allocator: std.mem.Allocator, app_key: [:0]const u8, property: ApplicationProperty.String) (ApplicationError || error{OutOfMemory})![:0]u8 {
    var error_code: ApplicationErrorCode = undefined;
    const buffer_length = self.function_table.GetApplicationPropertyString(@constCast(app_key.ptr), @enumFromInt(@intFromEnum(property)), null, 0, &error_code);
    try error_code.maybe();
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    errdefer allocator.free(buffer);

    if (buffer_length > 0) {
        error_code = undefined;
        _ = self.function_table.GetApplicationPropertyString(@constCast(app_key.ptr), @enumFromInt(@intFromEnum(property)), buffer.ptr, buffer_length, &error_code);
        try error_code.maybe();
    }

    return buffer;
}

pub fn getApplicationPropertyBool(self: Self, app_key: [:0]const u8, property: ApplicationProperty.Bool) ApplicationError!bool {
    return self.getApplicationProperty(bool, app_key, property);
}

pub fn getApplicationPropertyU64(self: Self, app_key: [:0]const u8, property: ApplicationProperty.U64) ApplicationError!u64 {
    return self.getApplicationProperty(u64, app_key, property);
}

pub fn setApplicationAutoLaunch(self: Self, app_key: [:0]const u8, auto_launch: bool) ApplicationError!void {
    const error_code = self.function_table.SetApplicationAutoLaunch(@constCast(app_key.ptr), auto_launch);
    try error_code.maybe();
}

pub fn getApplicationAutoLaunch(self: Self, app_key: [:0]const u8) ApplicationError!bool {
    return self.function_table.GetApplicationAutoLaunch(@constCast(app_key.ptr));
}

pub fn setDefaultApplicationForMimeType(self: Self, app_key: [:0]const u8, mime_type: [:0]const u8) ApplicationError!void {
    const error_code = self.function_table.SetDefaultApplicationForMimeType(@constCast(app_key.ptr), @constCast(mime_type.ptr));
    try error_code.maybe();
}

pub fn allocDefaultApplicationForMimeType(self: Self, allocator: std.mem.Allocator, mime_type: [:0]const u8) !?[:0]u8 {
    var application_key_buffer: [max_application_key_length:0]u8 = undefined;

    const result = self.function_table.GetDefaultApplicationForMimeType(@constCast(mime_type.ptr), application_key_buffer[0..], @intCast(application_key_buffer.len));
    if (!result) {
        return null;
    }

    const application_key_slice = std.mem.sliceTo(&application_key_buffer, 0);
    const application_key = try allocator.allocSentinel(u8, application_key_slice.len, 0);
    std.mem.copyForwards(u8, application_key, application_key_slice);
    return application_key;
}

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

pub fn allocApplicationSupportedMimeTypes(self: Self, allocator: std.mem.Allocator, app_key: [:0]const u8, buffer_length: u32) !?MimeTypes {
    if (buffer_length == 0) {
        return null;
    }
    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    const result = self.function_table.GetApplicationSupportedMimeTypes(@constCast(app_key.ptr), buffer.ptr, buffer_length);
    if (!result) {
        allocator.free(buffer);
        return null;
    }

    return MimeTypes{ .buffer = buffer };
}

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

pub fn allocApplicationsThatSupportMimeType(self: Self, allocator: std.mem.Allocator, mime_type: [:0]const u8) !AppKeys {
    const buffer_length = self.function_table.GetApplicationsThatSupportMimeType(@constCast(mime_type.ptr), null, 0);
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    if (buffer_length > 0) {
        _ = self.function_table.GetApplicationsThatSupportMimeType(@constCast(mime_type.ptr), buffer.ptr, buffer_length);
    }

    return AppKeys{ .buffer = buffer };
}

pub fn allocApplicationLaunchArguments(self: Self, allocator: std.mem.Allocator, handle: u32) ![:0]u8 {
    const buffer_length = self.function_table.GetApplicationLaunchArguments(handle, null, 0);
    if (buffer_length == 0) {
        return allocator.allocSentinel(u8, 0, 0);
    }

    const buffer = try allocator.allocSentinel(u8, buffer_length - 1, 0);
    if (buffer_length > 0) {
        _ = self.function_table.GetApplicationLaunchArguments(handle, buffer.ptr, buffer_length);
    }

    return buffer;
}

pub fn allocStartingApplication(self: Self, allocator: std.mem.Allocator) (ApplicationError || error{OutOfMemory})![:0]u8 {
    var application_key_buffer: [max_application_key_length]u8 = undefined;

    const error_code = self.function_table.GetStartingApplication(&application_key_buffer, @intCast(application_key_buffer.len));
    try error_code.maybe();

    const application_key_slice = std.mem.sliceTo(&application_key_buffer, 0);
    const application_key = try allocator.allocSentinel(u8, application_key_slice.len, 0);
    std.mem.copyForwards(u8, application_key, application_key_slice);
    return application_key;
}

pub const SceneApplicationState = enum(i32) {
    none = 0,
    starting = 1,
    quitting = 2,
    running = 3,
    waiting = 4,
};

pub fn getSceneApplicationState(self: Self) SceneApplicationState {
    return self.function_table.GetSceneApplicationState();
}

pub fn performApplicationPrelaunchCheck(self: Self, app_key: [:0]const u8) ApplicationError!void {
    const error_code = self.function_table.PerformApplicationPrelaunchCheck(@constCast(app_key.ptr));
    try error_code.maybe();
}

pub fn getSceneApplicationStateNameFromEnum(self: Self, state: SceneApplicationState) [:0]const u8 {
    return std.mem.span(self.function_table.GetSceneApplicationStateNameFromEnum(state));
}

pub fn launchInternalProcess(self: Self, binary_path: [:0]const u8, arguments: [:0]const u8, working_directory: [:0]const u8) ApplicationError!void {
    const error_code = self.function_table.LaunchInternalProcess(@constCast(binary_path.ptr), @constCast(arguments.ptr), @constCast(working_directory.ptr));
    try error_code.maybe();
}

pub fn getCurrentSceneProcessId(self: Self) u32 {
    return self.function_table.GetCurrentSceneProcessId();
}

pub const FunctionTable = extern struct {
    AddApplicationManifest: *const fn ([*c]u8, bool) callconv(.C) ApplicationErrorCode,
    RemoveApplicationManifest: *const fn ([*c]u8) callconv(.C) ApplicationErrorCode,
    IsApplicationInstalled: *const fn ([*c]u8) callconv(.C) bool,
    GetApplicationCount: *const fn () callconv(.C) u32,
    GetApplicationKeyByIndex: *const fn (u32, [*c]u8, u32) callconv(.C) ApplicationErrorCode,
    GetApplicationKeyByProcessId: *const fn (u32, [*c]u8, u32) callconv(.C) ApplicationErrorCode,
    LaunchApplication: *const fn ([*c]u8) callconv(.C) ApplicationErrorCode,
    LaunchTemplateApplication: *const fn ([*c]u8, [*c]u8, [*c]ExternAppOverrideKeys, u32) callconv(.C) ApplicationErrorCode,
    LaunchApplicationFromMimeType: *const fn ([*c]u8, [*c]u8) callconv(.C) ApplicationErrorCode,
    LaunchDashboardOverlay: *const fn ([*c]u8) callconv(.C) ApplicationErrorCode,
    CancelApplicationLaunch: *const fn ([*c]u8) callconv(.C) bool,
    IdentifyApplication: *const fn (u32, [*c]u8) callconv(.C) ApplicationErrorCode,
    GetApplicationProcessId: *const fn ([*c]u8) callconv(.C) u32,
    GetApplicationsErrorNameFromEnum: *const fn (ApplicationErrorCode) callconv(.C) [*c]u8,
    GetApplicationPropertyString: *const fn ([*c]u8, ApplicationProperty, [*c]u8, u32, *ApplicationErrorCode) callconv(.C) u32,
    GetApplicationPropertyBool: *const fn ([*c]u8, ApplicationProperty, *ApplicationErrorCode) callconv(.C) bool,
    GetApplicationPropertyUint64: *const fn ([*c]u8, ApplicationProperty, *ApplicationErrorCode) callconv(.C) u64,
    SetApplicationAutoLaunch: *const fn ([*c]u8, bool) callconv(.C) ApplicationErrorCode,
    GetApplicationAutoLaunch: *const fn ([*c]u8) callconv(.C) bool,
    SetDefaultApplicationForMimeType: *const fn ([*c]u8, [*c]u8) callconv(.C) ApplicationErrorCode,
    GetDefaultApplicationForMimeType: *const fn ([*c]u8, [*c]u8, u32) callconv(.C) bool,
    GetApplicationSupportedMimeTypes: *const fn ([*c]u8, [*c]u8, u32) callconv(.C) bool,
    GetApplicationsThatSupportMimeType: *const fn ([*c]u8, [*c]u8, u32) callconv(.C) u32,
    GetApplicationLaunchArguments: *const fn (u32, [*c]u8, u32) callconv(.C) u32,
    GetStartingApplication: *const fn ([*c]u8, u32) callconv(.C) ApplicationErrorCode,
    GetSceneApplicationState: *const fn () callconv(.C) SceneApplicationState,
    PerformApplicationPrelaunchCheck: *const fn ([*c]u8) callconv(.C) ApplicationErrorCode,
    GetSceneApplicationStateNameFromEnum: *const fn (SceneApplicationState) callconv(.C) [*c]u8,
    LaunchInternalProcess: *const fn ([*c]u8, [*c]u8, [*c]u8) callconv(.C) ApplicationErrorCode,
    GetCurrentSceneProcessId: *const fn () callconv(.C) u32,
};
