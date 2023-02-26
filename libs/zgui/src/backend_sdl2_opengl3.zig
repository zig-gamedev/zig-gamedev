const gui = @import("gui.zig");

pub fn init(window: *const anyopaque, sdl_gl_context: *const anyopaque) void {
    if (!ImGui_ImplSDL2_InitForOpenGL(window, sdl_gl_context)) unreachable;
    if (!ImGui_ImplOpenGL3_Init(null)) unreachable;
}

pub fn deinit() void {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
}

pub fn newFrame(fb_width: u32, fb_height: u32) void {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    gui.io.setDisplaySize(@intToFloat(f32, fb_width), @intToFloat(f32, fb_height));
    gui.io.setDisplayFramebufferScale(1.0, 1.0);

    gui.newFrame();
}

pub fn draw(_: *const anyopaque) void {
    gui.render();
    ImGui_ImplOpenGL3_RenderDrawData(gui.getDrawData());
}

pub fn processEvent(event: *const anyopaque) bool {
    return ImGui_ImplSDL2_ProcessEvent(event);
}

extern fn ImGui_ImplSDL2_InitForOpenGL(window: *const anyopaque, sdl_context: *const anyopaque) bool;
extern fn ImGui_ImplSDL2_Shutdown() void;
extern fn ImGui_ImplSDL2_NewFrame() void;
extern fn ImGui_ImplSDL2_ProcessEvent(event: *const anyopaque) bool;
extern fn ImGui_ImplOpenGL3_Init(glsl_version: ?[*:0]const u8) bool;
extern fn ImGui_ImplOpenGL3_NewFrame() void;
extern fn ImGui_ImplOpenGL3_RenderDrawData(draw_data: *const anyopaque) void;
extern fn ImGui_ImplOpenGL3_Shutdown() void;
