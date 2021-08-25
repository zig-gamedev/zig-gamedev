const builtin = @import("builtin");
const std = @import("std");
const w = @import("win32");
const gr = @import("graphics");
const lib = @import("library");
const c = @import("c");
usingnamespace @import("vectormath");
const assert = std.debug.assert;

pub export var D3D12SDKVersion: u32 = 4;
pub export var D3D12SDKPath: [*:0]const u8 = ".\\d3d12\\";

const window_name = "zig-gamedev: imgui test";
const window_width = 1920;
const window_height = 1080;

const DemoState = struct {
    grfx: gr.GraphicsContext,
    gui: gr.GuiContext,
    frame_stats: lib.FrameStats,

    //ui: struct {},
};

fn init(allocator: *std.mem.Allocator) DemoState {
    const window = lib.initWindow(allocator, window_name, window_width, window_height) catch unreachable;
    var grfx = gr.GraphicsContext.init(window);

    grfx.beginFrame();

    var gui = gr.GuiContext.init(allocator, &grfx);

    grfx.finishGpuCommands();

    return .{
        .grfx = grfx,
        .gui = gui,
        .frame_stats = lib.FrameStats.init(),
    };
}

fn deinit(demo: *DemoState, allocator: *std.mem.Allocator) void {
    demo.grfx.finishGpuCommands();
    demo.gui.deinit(&demo.grfx);
    demo.grfx.deinit(allocator);
    lib.deinitWindow(allocator);
    demo.* = undefined;
}

fn update(demo: *DemoState) void {
    demo.frame_stats.update();

    lib.newImGuiFrame(demo.frame_stats.delta_time);

    const main_viewport = c.igGetMainViewport();
    c.igSetNextWindowPos(
        c.ImVec2{ .x = main_viewport.*.WorkPos.x + 650.0, .y = main_viewport.*.WorkPos.y + 20.0 },
        c.ImGuiCond_FirstUseEver,
        c.ImVec2{ .x = 0.0, .y = 0.0 },
    );
    c.igSetNextWindowSize(c.ImVec2{ .x = 550.0, .y = 680.0 }, c.ImGuiCond_FirstUseEver);

    if (!c.igBegin("Gui Demo", null, c.ImGuiWindowFlags_None)) {
        c.igEnd();
        return;
    }

    const static = struct {
        var number: u32 = 0;
    };

    c.igPushItemWidth(c.igGetFontSize() * -12.0);

    c.igText("dear imgui says hello. (%s)", c.igGetVersion());
    c.igSpacing();

    if (c.igCollapsingHeader_TreeNodeFlags("Help", c.ImGuiTreeNodeFlags_None)) {
        c.igText("ABOUT THIS DEMO:");
        c.igBulletText("Sections below are demonstrating many aspects of the library.");
        c.igBulletText("The \"Examples\" menu above leads to more demo contents.");
        c.igBulletText("The \"Tools\" menu above gives access to: About Box, Style Editor,\n" ++
            "and Metrics/Debugger (general purpose Dear ImGui debugging tool).");
        c.igSeparator();

        c.igText("PROGRAMMER GUIDE:");
        c.igBulletText("See the ShowDemoWindow() code in imgui_demo.cpp. <- you are here!");
        c.igBulletText("See comments in imgui.cpp.");
        c.igBulletText("See example applications in the examples/ folder.");
        c.igBulletText("Read the FAQ at http://www.dearimgui.org/faq/");
        c.igBulletText("Set 'io.ConfigFlags |= NavEnableKeyboard' for keyboard controls.");
        c.igBulletText("Set 'io.ConfigFlags |= NavEnableGamepad' for gamepad controls.");
        c.igSeparator();

        c.igText("USER GUIDE:");
        c.igShowUserGuide();
    }

    c.igText("Number is %d", .{static.number});
    static.number += 1;

    c.igPopItemWidth();
    c.igEnd();
}

fn draw(demo: *DemoState) void {
    var grfx = &demo.grfx;
    grfx.beginFrame();

    const back_buffer = grfx.getBackBuffer();

    grfx.addTransitionBarrier(back_buffer.resource_handle, w.D3D12_RESOURCE_STATE_RENDER_TARGET);
    grfx.flushResourceBarriers();

    grfx.cmdlist.OMSetRenderTargets(
        1,
        &[_]w.D3D12_CPU_DESCRIPTOR_HANDLE{back_buffer.descriptor_handle},
        w.TRUE,
        null,
    );
    grfx.cmdlist.ClearRenderTargetView(
        back_buffer.descriptor_handle,
        &[4]f32{ 0.0, 0.0, 0.0, 1.0 },
        0,
        null,
    );

    demo.gui.draw(grfx);

    grfx.addTransitionBarrier(back_buffer.resource_handle, w.D3D12_RESOURCE_STATE_PRESENT);
    grfx.flushResourceBarriers();

    grfx.endFrame();
}

pub fn main() !void {
    // WIC requires below call (when we pass COINIT_MULTITHREADED '_ = wic_factory.Release()' crashes on exit).
    _ = w.ole32.CoInitializeEx(null, @enumToInt(w.COINIT_APARTMENTTHREADED));
    defer w.ole32.CoUninitialize();

    _ = w.SetProcessDPIAware();

    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer {
        const leaked = gpa.deinit();
        std.debug.assert(leaked == false);
    }
    const allocator = &gpa.allocator;

    var demo = init(allocator);
    defer deinit(&demo, allocator);

    while (true) {
        var message = std.mem.zeroes(w.user32.MSG);
        const has_message = w.user32.peekMessageA(&message, null, 0, 0, w.user32.PM_REMOVE) catch unreachable;
        if (has_message) {
            _ = w.user32.translateMessage(&message);
            _ = w.user32.dispatchMessageA(&message);
            if (message.message == w.user32.WM_QUIT) {
                break;
            }
        } else {
            update(&demo);
            draw(&demo);
        }
    }
}
