const std = @import("std");

const gui = @import("gui.zig");

pub fn init(args: struct {
    hwnd: *const anyopaque, // HWND
    d3d12_device: *const anyopaque, // ID3D12Device*
    num_frames_in_flight: u16,
    rtv_format: c_int, // DXGI_FORMAT
    cbv_srv_heap: *const anyopaque, // ID3D12DescriptorHeap*
    font_srv_cpu_desc_handle: usize, // D3D12_CPU_DESCRIPTOR_HANDLE
    font_srv_gpu_desc_handle: usize, // D3D12_GPU_DESCRIPTOR_HANDLE
}) void {
    std.debug.assert(ImGui_ImplWin32_Init(args.hwnd));
    std.debug.assert(ImGui_ImplDX12_Init(
        args.d3d12_device,
        args.num_frames_in_flight,
        args.rtv_format,
        args.cbv_srv_heap,
        args.font_srv_cpu_desc_handle,
        args.font_srv_gpu_desc_handle,
    ));
}

pub fn deinit() void {
    ImGui_ImplWin32_Shutdown();
    ImGui_ImplDX12_Shutdown();
}

pub fn newFrame(fb_width: u32, fb_height: u32) void {
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX12_NewFrame();

    gui.io.setDisplaySize(@as(f32, @floatFromInt(fb_width)), @as(f32, @floatFromInt(fb_height)));
    gui.io.setDisplayFramebufferScale(1.0, 1.0);

    gui.newFrame();
}

pub fn draw(graphics_command_list: *const anyopaque) void {
    gui.render();
    ImGui_ImplDX12_RenderDrawData(gui.getDrawData(), graphics_command_list);
}

extern fn ImGui_ImplWin32_Init(hwnd: *const anyopaque) bool;
extern fn ImGui_ImplWin32_Shutdown() void;
extern fn ImGui_ImplWin32_NewFrame() void;
extern fn ImGui_ImplDX12_Init(
    device: *const anyopaque, // ID3D12Device*
    num_frames_in_flight: c_int,
    rtv_format: c_int, // DXGI_FORMAT
    cbv_srv_heap: *const anyopaque, // ID3D12DescriptorHeap*
    font_srv_cpu_desc_handle: usize, // D3D12_CPU_DESCRIPTOR_HANDLE
    font_srv_gpu_desc_handle: usize, // D3D12_GPU_DESCRIPTOR_HANDLE
) bool;
extern fn ImGui_ImplDX12_Shutdown() void;
extern fn ImGui_ImplDX12_NewFrame() void;
extern fn ImGui_ImplDX12_RenderDrawData(draw_data: *const anyopaque, graphics_command_list: *const anyopaque) void;
