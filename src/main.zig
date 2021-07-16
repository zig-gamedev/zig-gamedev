const std = @import("std");
const w = struct {
    usingnamespace std.os.windows;
    usingnamespace @import("windows/windows.zig");
    usingnamespace @import("windows/d3d12.zig");
};

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*c]const u8 = ".\\D3D12\\";

pub fn main() !void {
    try w.d3d12_load_dll();
    var p: ?*w.ID3D12PipelineState = null;
    const buffer_desc = w.D3D12_RESOURCE_DESC.buffer(100);
    std.debug.assert(w.D3D12CreateDevice(null, 0xb100, &w.IID_ID3D12Device, @ptrCast(**c_void, &p)) == 0);
    const pp = p.?;
    _ = pp.AddRef();
    std.debug.print("OK ({}, {})\n", .{ pp.Release(), buffer_desc.Width });
}
