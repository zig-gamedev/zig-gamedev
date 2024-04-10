const gui = @import("gui.zig");

// This call will install GLFW callbacks to handle GUI interactions.
// Those callbacks will chain-call user's previously installed callbacks, if any.
// This means that custom user's callbacks need to be installed *before* calling zgpu.gui.init().
pub fn init(
    window: *const anyopaque, // zglfw.Window
) void {
    if (!ImGui_ImplGlfw_InitForOther(window, true)) {
        unreachable;
    }
}

pub fn initOpenGL(
    window: *const anyopaque, // zglfw.Window
) void {
    if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
        unreachable;
    }
}

pub fn deinit() void {
    ImGui_ImplGlfw_Shutdown();
}

pub fn newFrame() void {
    ImGui_ImplGlfw_NewFrame();
}

// Those functions are defined in `imgui_impl_glfw.cpp`
// (they include few custom changes).
extern fn ImGui_ImplGlfw_InitForOther(window: *const anyopaque, install_callbacks: bool) bool;
extern fn ImGui_ImplGlfw_InitForOpenGL(window: *const anyopaque, install_callbacks: bool) bool;
extern fn ImGui_ImplGlfw_NewFrame() void;
extern fn ImGui_ImplGlfw_Shutdown() void;
