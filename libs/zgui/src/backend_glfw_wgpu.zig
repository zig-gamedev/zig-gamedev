const gui = @import("gui.zig");

pub const TextureFilterMode = enum(u32) {
    nearest,
    linear,
};

pub const Config = extern struct {
    pipeline_multisample_count: u32 = 1,
    texture_filter_mode: TextureFilterMode = .linear,
};

// This call will install GLFW callbacks to handle GUI interactions.
// Those callbacks will chain-call user's previously installed callbacks, if any.
// This means that custom user's callbacks need to be installed *before* calling zgpu.gui.init().
pub fn initWithConfig(
    window: *const anyopaque, // zglfw.Window
    wgpu_device: *const anyopaque, // wgpu.Device
    wgpu_swap_chain_format: u32, // wgpu.TextureFormat
    config: Config,
) void {
    if (!ImGui_ImplGlfw_InitForOther(window, true)) {
        unreachable;
    }

    if (!ImGui_ImplWGPU_Init(wgpu_device, 1, wgpu_swap_chain_format, &config)) {
        unreachable;
    }
}

pub fn init(window: *const anyopaque, wgpu_device: *const anyopaque, wgpu_swap_chain_format: u32) void {
    initWithConfig(window, wgpu_device, wgpu_swap_chain_format, .{});
}

pub fn deinit() void {
    ImGui_ImplWGPU_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}

pub fn newFrame(fb_width: u32, fb_height: u32) void {
    ImGui_ImplWGPU_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    gui.io.setDisplaySize(@as(f32, @floatFromInt(fb_width)), @as(f32, @floatFromInt(fb_height)));
    gui.io.setDisplayFramebufferScale(1.0, 1.0);

    gui.newFrame();
}

pub fn draw(wgpu_render_pass: *const anyopaque) void {
    gui.render();
    ImGui_ImplWGPU_RenderDrawData(gui.getDrawData(), wgpu_render_pass);
}

// Those functions are defined in `imgui_impl_glfw.cpp` and 'imgui_impl_wgpu.cpp`
// (they include few custom changes).
extern fn ImGui_ImplGlfw_InitForOther(window: *const anyopaque, install_callbacks: bool) bool;
extern fn ImGui_ImplGlfw_NewFrame() void;
extern fn ImGui_ImplGlfw_Shutdown() void;
extern fn ImGui_ImplWGPU_Init(
    device: *const anyopaque,
    num_frames_in_flight: u32,
    rt_format: u32,
    config: *const Config,
) bool;
extern fn ImGui_ImplWGPU_NewFrame() void;
extern fn ImGui_ImplWGPU_RenderDrawData(draw_data: *const anyopaque, pass_encoder: *const anyopaque) void;
extern fn ImGui_ImplWGPU_Shutdown() void;
