const gui = @import("gui.zig");

pub const TextureFilterMode = enum(u32) {
    nearest,
    linear,
};

pub const Config = extern struct {
    pipeline_multisample_count: u32 = 1,
    texture_filter_mode: TextureFilterMode = .linear,
};

// Those callbacks will chain-call user's previously installed callbacks, if any.
// This means that custom user's callbacks need to be installed *before* calling zgpu.gui.init().
pub fn initWithConfig(
    wgpu_device: *const anyopaque, // wgpu.Device
    wgpu_swap_chain_format: u32, // wgpu.TextureFormat
    config: Config,
) void {
    if (!ImGui_ImplOffscreenWGPU_Init(wgpu_device, 1, wgpu_swap_chain_format, &config)) {
        unreachable;
    }
}

pub fn init(wgpu_device: *const anyopaque, wgpu_swap_chain_format: u32) void {
    initWithConfig(wgpu_device, wgpu_swap_chain_format, .{});
}

pub fn deinit() void {
    ImGui_ImplOffscreenWGPU_Shutdown();
}

pub fn newFrame(fb_width: u32, fb_height: u32) void {
    ImGui_ImplOffscreenWGPU_NewFrame();

    gui.io.setDisplaySize(@intToFloat(f32, fb_width), @intToFloat(f32, fb_height));
    gui.io.setDisplayFramebufferScale(1.0, 1.0);

    gui.newFrame();
}

pub fn draw(wgpu_render_pass: *const anyopaque) void {
    gui.render();
    ImGui_ImplOffscreenWGPU_RenderDrawData(gui.getDrawData(), wgpu_render_pass);
}

// Those functions are defined in 'imgui_impl_wgpu.cpp`
// (they include few custom changes).
extern fn ImGui_ImplOffscreenWGPU_Init(
    device: *const anyopaque,
    num_frames_in_flight: u32,
    rt_format: u32,
    config: *const Config,
) bool;
extern fn ImGui_ImplOffscreenWGPU_NewFrame() void;
extern fn ImGui_ImplOffscreenWGPU_RenderDrawData(draw_data: *const anyopaque, pass_encoder: *const anyopaque) void;
extern fn ImGui_ImplOffscreenWGPU_Shutdown() void;
