# zgui v0.1.0 - dear imgui bindings

Easy to use, hand-crafted API with default arguments, named parameters and Zig style text formatting. [Here](https://github.com/michal-z/zig-gamedev/tree/main/samples/minimal_zgpu_zgui) is a simple sample application, and [here](https://github.com/michal-z/zig-gamedev/tree/main/samples/gui_test_wgpu) is a full one.

## Features

* Most public dear imgui API exposed
* All memory allocations go through user provided Zig allocator
* [DrawList API](#drawlist-api) for vector graphics, text rendering and custom widgets
* [Plot API](#plot-api) for advanced data visualizations

## Getting started

Copy `zgui` to a subdirectory in your project and add the following to your `build.zig.zon` .dependencies:
```zig
    .zgui = .{ .path = "libs/zgui" },
```

To get glfw/wgpu rendering backend working also copy `zglfw`, `system-sdk`, `zgpu` and `zpool` folders and add the depenency paths (see [zgpu](https://github.com/zig-gamedev/zig-gamedev/tree/main/libs/zgpu) for the details).

Then in your `build.zig` add:
```zig

pub fn build(b: *std.Build) void {
    const exe = b.addExecutable(.{ ... });

    const zgui = b.dependency("zgui", .{
        .shared = false,
        .with_implot = true,
    });
    exe.root_module.addImport("zgui", zgui.module("root"));
    exe.linkLibrary(zgui.artifact("imgui"));
    
    { // Needed for glfw/wgpu rendering backend
        const zglfw = b.dependency("zglfw", .{});
        exe.root_module.addImport("zglfw", zglfw.module("root"));
        exe.linkLibrary(zglfw.artifact("glfw"));

        const zpool = b.dependency("zpool", .{});
        exe.root_module.addImport("zpool", zglfw.module("root"));

        const zgpu = b.dependency("zgpu", .{});
        exe.root_module.addImport("zgpu", zglfw.module("root"));
        exe.linkLibrary(zglfw.artifact("wgpu"));
    }
}
```

Now in your code you may import and use `zgui`:

```zig
const zgui = @import("zgui");

zgui.init(allocator);

_ = zgui.io.addFontFromFile(content_dir ++ "Roboto-Medium.ttf", 16.0);

zgui.backend.init(
    window,
    demo.gctx.device,
    @enumToInt(swapchain_format),
    @enumToInt(depth_format),
);
```

```zig
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

### Building a shared library

If your project spans multiple zig modules that both use ImGui, such as an exe paired with a dll, you may want to build the `zgui` dependencies (`zgui_pkg.zgui_c_cpp`) as a shared library. This can be enabled with the `shared` build option. Then, in `build.zig`, use `zgui_pkg.link` to link `zgui` to all the modules that use ImGui.

When built this way, the ImGui context will be located in the shared library. However, the `zgui` zig code (which is compiled separately into each module) requires its own memory buffer which has to be initialized separately with `initNoContext`.

In your executable:
```zig
const zgui = @import("zgui");
zgui.init(allocator);
defer zgui.deinit();
```

In your shared library:
```zig
const zgui = @import("zgui");
zgui.initNoContext(allocator);
defer zgui.deinitNoContxt();
```

### DrawList API

```zig
draw_list.addQuad(.{
    .p1 = .{ 170, 420 },
    .p2 = .{ 270, 420 },
    .p3 = .{ 220, 520 },
    .p4 = .{ 120, 520 },
    .col = 0xff_00_00_ff,
    .thickness = 3.0,
});
draw_list.addText(.{ 130, 130 }, 0xff_00_00_ff, "The number is: {}", .{7});
draw_list.addCircleFilled(.{ .p = .{ 200, 600 }, .r = 50, .col = 0xff_ff_ff_ff });
draw_list.addCircle(.{ .p = .{ 200, 600 }, .r = 30, .col = 0xff_00_00_ff, .thickness = 11 });
draw_list.addPolyline(
    &.{ .{ 100, 700 }, .{ 200, 600 }, .{ 300, 700 }, .{ 400, 600 } },
    .{ .col = 0xff_00_aa_11, .thickness = 7 },
);
```
### Plot API
```zig
if (zgui.plot.beginPlot("Line Plot", .{ .h = -1.0 })) {
    zgui.plot.setupAxis(.x1, .{ .label = "xaxis" });
    zgui.plot.setupAxisLimits(.x1, .{ .min = 0, .max = 5 });
    zgui.plot.setupLegend(.{ .south = true, .west = true }, .{});
    zgui.plot.setupFinish();
    zgui.plot.plotLineValues("y data", i32, .{ .v = &.{ 0, 1, 0, 1, 0, 1 } });
    zgui.plot.plotLine("xy data", f32, .{
        .xv = &.{ 0.1, 0.2, 0.5, 2.5 },
        .yv = &.{ 0.1, 0.3, 0.5, 0.9 },
    });
    zgui.plot.endPlot();
}
```
