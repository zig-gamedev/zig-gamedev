pub const D3D12_CPU_DESCRIPTOR_HANDLE = extern struct {
    ptr: c_ulonglong,
};

pub const D3D12_GPU_DESCRIPTOR_HANDLE = extern struct {
    ptr: c_ulonglong,
};

pub fn init(
    device: *const anyopaque, // ID3D12Device
    num_frames_in_flight: u32,
    rtv_format: c_uint, // DXGI_FORMAT
    cbv_srv_heap: *const anyopaque, // ID3D12DescriptorHeap
    font_srv_cpu_desc_handle: D3D12_CPU_DESCRIPTOR_HANDLE,
    font_srv_gpu_desc_handle: D3D12_GPU_DESCRIPTOR_HANDLE,
) void {
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
    ImGui_ImplDX12_Shutdown();
}

pub fn newFrame() void {
    ImGui_ImplDX12_NewFrame();
}

pub fn render(
    draw_data: *const anyopaque, // *gui.DrawData
    gfx_command_list: *const anyopaque, // *ID3D12GraphicsCommandList
) void {
    ImGui_ImplDX12_RenderDrawData(draw_data, gfx_command_list);
}

// Those functions are defined in 'imgui_impl_dx12.cpp`
// (they include few custom changes).
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
