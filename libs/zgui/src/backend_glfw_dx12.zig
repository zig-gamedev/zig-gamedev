const gui = @import("gui.zig");

pub fn init(
    window: *const anyopaque, // zglfw.Window
    device: *const anyopaque, // ID3D12Device
    num_frames_in_flight: u32,
    rtv_format: c_uint, // DXGI_FORMAT
    cbv_srv_heap: *const anyopaque, // ID3D12DescriptorHeap
    font_srv_cpu_desc_handle: D3D12_CPU_DESCRIPTOR_HANDLE,
    font_srv_gpu_desc_handle: D3D12_GPU_DESCRIPTOR_HANDLE,
) void {
    if (!ImGui_ImplGlfw_InitForOther(window, true)) {
        @panic("failed to init glfw for imgui");
    }

    if (!ImGui_ImplDX12_Init(
        device,
        num_frames_in_flight,
        rtv_format,
        cbv_srv_heap,
        font_srv_cpu_desc_handle,
        font_srv_gpu_desc_handle,
    )) {
        @panic("failed to init d3d12 for imgui");
    }
}

pub fn deinit() void {
    ImGui_ImplGlfw_Shutdown();
    ImGui_ImplDX12_Shutdown();
}

pub fn newFrame(fb_width: u32, fb_height: u32) void {
    ImGui_ImplGlfw_NewFrame();
    ImGui_ImplDX12_NewFrame();

    gui.io.setDisplaySize(@as(f32, @floatFromInt(fb_width)), @as(f32, @floatFromInt(fb_height)));
    gui.io.setDisplayFramebufferScale(1.0, 1.0);

    gui.newFrame();
}

pub fn draw(
    graphics_command_list: *const anyopaque, // *ID3D12GraphicsCommandList
) void {
    gui.render();
    ImGui_ImplDX12_RenderDrawData(gui.getDrawData(), graphics_command_list);
}

pub const D3D12_CPU_DESCRIPTOR_HANDLE = extern struct {
    ptr: c_ulonglong,
};

pub const D3D12_GPU_DESCRIPTOR_HANDLE = extern struct {
    ptr: c_ulonglong,
};

extern fn ImGui_ImplGlfw_InitForOther(window: *const anyopaque, install_callbacks: bool) bool;
extern fn ImGui_ImplGlfw_NewFrame() void;
extern fn ImGui_ImplGlfw_Shutdown() void;
extern fn ImGui_ImplDX12_Init(
    device: *const anyopaque, // ID3D12Device
    num_frames_in_flight: u32,
    rtv_format: u32, // DXGI_FORMAT
    cbv_srv_heap: *const anyopaque, // ID3D12DescriptorHeap
    font_srv_cpu_desc_handle: D3D12_CPU_DESCRIPTOR_HANDLE,
    font_srv_gpu_desc_handle: D3D12_GPU_DESCRIPTOR_HANDLE,
) bool;
extern fn ImGui_ImplDX12_Shutdown() void;
extern fn ImGui_ImplDX12_NewFrame() void;
extern fn ImGui_ImplDX12_RenderDrawData(
    draw_data: *const anyopaque, // *ImDrawData
    graphics_command_list: *const anyopaque, // *ID3D12GraphicsCommandList
) void;
