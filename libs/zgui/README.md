# zgui v0.5.0 - dear imgui bindings

Easy to use, hand-crafted API with default arguments, named parameters and Zig style text formatting. [Here](https://github.com/michal-z/zig-gamedev/tree/main/samples/minimal_zgpu_zgui) is a simple sample application, and [here](https://github.com/michal-z/zig-gamedev/tree/main/samples/gui_test_wgpu) is a full one.

## Features

* Most public dear imgui API exposed
* All memory allocations go through user provided Zig allocator
* [DrawList API](#drawlist-api) for vector graphics, text rendering and custom widgets
* [Test engine API](#test-engine-api) for automatic testing
* [Plot API](#plot-api) for advanced data visualizations
* [Gizmo API](#gizmo-api) for gizmo
* [Node editor API](#node-editor-api) for node based stuff

## Versions

* [ImGui](https://github.com/ocornut/imgui/tree/v1.91.0-docking) `1.91.0-docking`
* [ImGui test engine](https://github.com/ocornut/imgui_test_engine/tree/v1.91.0)  `1.91.0`
* [ImPlot](https://github.com/epezent/implot) `O.17`
* [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) `1.89 WIP`
* [ImGuiNodeEditor](https://github.com/thedmd/imgui-node-editor/tree/v0.9.3) `O.9.3`

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
        exe.root_module.addImport("zpool", zpool.module("root"));

        const zgpu = b.dependency("zgpu", .{});
        exe.root_module.addImport("zgpu", zgpu.module("root"));
        exe.linkLibrary(zgpu.artifact("zdawn"));
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

### Test Engine API
Zig wraper for [ImGUI test engine](https://github.com/ocornut/imgui_test_engine).

```zig
var check_b = false;
var _te: *zgui.te.TestEngine = zgui.te.getTestEngine().?;
fn registerTests() void {
    _ = _te.registerTest(
        "Awesome",
        "should_do_some_another_magic",
        @src(),
        struct {
            pub fn gui(ctx: *zgui.te.TestContext) !void {
                _ = ctx; // autofix
                _ = zgui.begin("Test Window", .{ .flags = .{ .no_saved_settings = true } });
                defer zgui.end();

                zgui.text("Hello, automation world", .{});
                _ = zgui.button("Click Me", .{});
                if (zgui.treeNode("Node")) {
                    defer zgui.treePop();

                    _ = zgui.checkbox("Checkbox", .{ .v = &check_b });
                }
            }

            pub fn run(ctx: *zgui.te.TestContext) !void {
                ctx.setRef("/Test Window");
                ctx.windowFocus("");

                ctx.itemAction(.click, "Click Me", .{}, null);
                ctx.itemAction(.open, "Node", .{}, null);
                ctx.itemAction(.check, "Node/Checkbox", .{}, null);
                ctx.itemAction(.uncheck, "Node/Checkbox", .{}, null);

                std.testing.expect(true) catch |err| {
                    zgui.te.checkTestError(@src(), err);
                    return;
                };
            }
        },
    );
}
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

### Gizmo API

Zig wraper for [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo).


### Node editor API

Zig wraper for [ImGuiNodeEditor](https://github.com/thedmd/imgui-node-editor).

```zig
var node_editor = zgui.node_editor.EditorContext.create(.{ .enable_smooth_zoom = true }),

zgui.node_editor.setCurrentEditor(node_editor);
defer zgui.node_editor.setCurrentEditor(null);
{
    zgui.node_editor.begin("NodeEditor", .{ 0, 0 });
    defer zgui.node_editor.end();

    zgui.node_editor.beginNode(1);
    {
        defer zgui.node_editor.endNode();

        zgui.textUnformatted("Node A");

        zgui.node_editor.beginPin(1, .input);
        {
            defer zgui.node_editor.endPin();
            zgui.textUnformatted("-> In");
        }

        zgui.sameLine(.{});

        zgui.node_editor.beginPin(2, .output);
        {
            defer zgui.node_editor.endPin();
            zgui.textUnformatted("Out ->");
        }
    }
}
```
