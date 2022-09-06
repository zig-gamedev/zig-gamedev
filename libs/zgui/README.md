# zgui - dear imgui bindings

Easy to use, hand-crafted API with default arguments, named parameters and Zig style text formatting. For a test application please see [here](https://github.com/michal-z/zig-gamedev/tree/main/samples/gui_test_wgpu).

## Getting started

Copy `zgui` and `zglfw` folders to a `libs` subdirectory of the root of your project.

Then in your `build.zig` add:
```zig
const zgui = @import("libs/zgui/build.zig");
const zglfw = @import("libs/zglfw/build.zig");

pub fn build(b: *std.build.Builder) void {
    ...
    const zgui_pkg = zgui.getPkg(&.{zglfw.pkg});

    exe.addPackage(zgui_pkg);
    exe.addPackage(zglfw.pkg);

    zgui.link(exe);
    zglfw.link(exe);
}
```
Now in your code you may import and use `zgui`:
```zig
const zgui = @import("zgui");

zgui.init();
defer zgui.deinit();

_ = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", 16.0);

zgui.backend.init(
    window,
    demo.gctx.device,
    @enumToInt(swapchain_format),
);
defer zgui.backend.deinit();

var value0: f32 = 0.0;
var value1: f32 = 0.0;

// Main loop
while (...) {
    zgui.backend.newFrame(framebuffer_width, framebuffer_height);

    zgui.bulletText(
        "Average :  {d:.3} ms/frame ({d:.1} fps)",
        .{ demo.gctx.stats.average_cpu_time, demo.gctx.stats.fps },
    );
    zgui.bulletText("W, A, S, D :  move camera", .{});
    zgui.spacing();

    if (zgui.button("Setup Scene", .{})) {
        // Button pressed.
    }

    if (zgui.dragFloat("Drag 1", .{ .v = &value0 })) {
        // value0 has changed
    }

    if (zgui.dragFloat("Drag 2", .{ .v = &value0, .min = -1.0, .max = 1.0 })) {
        // value1 has changed
    }

    // Setup wgpu render pass here

    zgui.backend.draw(pass);
}
```
