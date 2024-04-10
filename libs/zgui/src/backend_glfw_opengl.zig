const gui = @import("gui.zig");
const backend_glfw = @import("backend_glfw.zig");

pub fn initWithGlSlVersion(
    window: *const anyopaque, // zglfw.Window
    glsl_version: ?[:0]const u8, // e.g. "#version 130"
) void {
    backend_glfw.initOpenGL(window);

    ImGui_ImplOpenGL3_Init(@ptrCast(glsl_version));
}

pub fn init(
    window: *const anyopaque, // zglfw.Window
) void {
    initWithGlSlVersion(window, null);
}

pub fn deinit() void {
    ImGui_ImplOpenGL3_Shutdown();
    backend_glfw.deinit();
}

pub fn newFrame(fb_width: u32, fb_height: u32) void {
    backend_glfw.newFrame();
    ImGui_ImplOpenGL3_NewFrame();

    gui.io.setDisplaySize(@as(f32, @floatFromInt(fb_width)), @as(f32, @floatFromInt(fb_height)));
    gui.io.setDisplayFramebufferScale(1.0, 1.0);

    gui.newFrame();
}

pub fn draw() void {
    gui.render();
    ImGui_ImplOpenGL3_RenderDrawData(gui.getDrawData());
}

// Those functions are defined in 'imgui_impl_opengl3.cpp`
// (they include few custom changes).
extern fn ImGui_ImplOpenGL3_Init(glsl_version: [*c]const u8) void;
extern fn ImGui_ImplOpenGL3_Shutdown() void;
extern fn ImGui_ImplOpenGL3_NewFrame() void;
extern fn ImGui_ImplOpenGL3_RenderDrawData(data: *const anyopaque) void;
