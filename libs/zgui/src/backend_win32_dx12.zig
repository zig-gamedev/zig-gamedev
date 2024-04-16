const std = @import("std");

const gui = @import("gui.zig");
const backend_dx12 = @import("backend_dx12.zig");

pub fn init(
    hwnd: *const anyopaque, // HWND
    d3d12_device: *const anyopaque, // ID3D12Device*
    num_frames_in_flight: u16,
    rtv_format: u32, // DXGI_FORMAT
    cbv_srv_heap: *const anyopaque, // ID3D12DescriptorHeap*
    font_srv_cpu_desc_handle: backend_dx12.D3D12_CPU_DESCRIPTOR_HANDLE,
    font_srv_gpu_desc_handle: backend_dx12.D3D12_GPU_DESCRIPTOR_HANDLE,
) void {
    std.debug.assert(ImGui_ImplWin32_Init(hwnd));
    backend_dx12.init(
        d3d12_device,
        num_frames_in_flight,
        rtv_format,
        cbv_srv_heap,
        font_srv_cpu_desc_handle,
        font_srv_gpu_desc_handle,
    );
}

pub fn deinit() void {
    backend_dx12.deinit();
    ImGui_ImplWin32_Shutdown();
}

pub fn newFrame(fb_width: u32, fb_height: u32) void {
    ImGui_ImplWin32_NewFrame();
    backend_dx12.newFrame();

    gui.io.setDisplaySize(@as(f32, @floatFromInt(fb_width)), @as(f32, @floatFromInt(fb_height)));
    gui.io.setDisplayFramebufferScale(1.0, 1.0);

    gui.newFrame();
}

pub fn draw(graphics_command_list: *const anyopaque) void {
    gui.render();
    backend_dx12.render(gui.getDrawData(), graphics_command_list);
}

extern fn ImGui_ImplWin32_Init(hwnd: *const anyopaque) bool;
extern fn ImGui_ImplWin32_Shutdown() void;
extern fn ImGui_ImplWin32_NewFrame() void;
