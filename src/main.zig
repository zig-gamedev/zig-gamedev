const std = @import("std");
const w = struct {
    usingnamespace std.os.windows;
    usingnamespace @import("windows/windows.zig");
    usingnamespace @import("windows/d3d12.zig");
    usingnamespace @import("windows/d3d12sdklayers.zig");
};

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*c]const u8 = ".\\D3D12\\";

pub fn main() !void {
    try w.d3d12_load_dll();

    var maybe_debug: ?*w.ID3D12Debug5 = null;
    _ = w.D3D12GetDebugInterface(&w.IID_ID3D12Debug5, @ptrCast(*?*c_void, &maybe_debug));
    _ = w.D3D12GetDebugInterface(&w.IID_ID3D12Debug5, null);
    const debug = maybe_debug.?;
    debug.EnableDebugLayer();
    debug.SetEnableGPUBasedValidation(w.TRUE);

    var maybe_device: ?*w.ID3D12Device = null;
    std.debug.assert(w.D3D12CreateDevice(null, 0xb100, &w.IID_ID3D12Device, @ptrCast(*?*c_void, &maybe_device)) == 0);
    const device = maybe_device.?;

    const buffer_desc = w.D3D12_RESOURCE_DESC.buffer(100);

    _ = device.AddRef();

    std.debug.print("OK ({}, {})\n", .{ device.Release(), buffer_desc.Width });
}
