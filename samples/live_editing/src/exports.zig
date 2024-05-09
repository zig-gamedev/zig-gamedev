const std = @import("std");

const zwin32 = @import("zwin32");
const zd3d12 = @import("zd3d12");

const ErrorSetEnum = @import("error_set_enum.zig");

const ExportedEntry = struct {
    const Entry = @import("entry.zig");

    pub const init_error = ErrorSetEnum.initFn(@TypeOf(Entry.init));
    pub const renderFrameD3d12_error = ErrorSetEnum.initFn(@TypeOf(Entry.renderFrameD3d12));

    fn init(allocator: *const std.mem.Allocator, gctx: *zd3d12.GraphicsContext, error_enum: *init_error.Enum, result: *Entry) callconv(.C) bool {
        if (Entry.init(allocator.*, gctx)) |entry| {
            result.* = entry;
            return true;
        } else |err| {
            error_enum.* = init_error.errorToEnum(err);
            return false;
        }
    }

    fn deinit(entry: *Entry) callconv(.C) void {
        entry.deinit();
    }

    fn inputUpdated(entry: *Entry, input: Entry.Input) callconv(.C) void {
        entry.inputUpdated(input);
    }

    fn renderFrameD3d12(entry: *Entry, error_enum: *renderFrameD3d12_error.Enum) callconv(.C) bool {
        if (entry.renderFrameD3d12()) |_| {
            return true;
        } else |err| {
            error_enum.* = renderFrameD3d12_error.errorToEnum(err);
            return false;
        }
    }
    fn postRenderFrame(entry: *Entry) callconv(.C) void {
        entry.postRenderFrame();
    }
};

comptime {
    @export(ExportedEntry.init, .{ .name = "Entry.init" });
    @export(ExportedEntry.deinit, .{ .name = "Entry.deinit" });
    @export(ExportedEntry.inputUpdated, .{ .name = "Entry.inputUpdated" });
    @export(ExportedEntry.renderFrameD3d12, .{ .name = "Entry.renderFrameD3d12" });
    @export(ExportedEntry.postRenderFrame, .{ .name = "Entry.postRenderFrame" });
}
