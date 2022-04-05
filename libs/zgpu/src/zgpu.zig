const std = @import("std");
const glfw = @import("glfw");
const gpu = @import("gpu");
const c = @import("c.zig").c;

pub const GraphicsContext = struct {
    native_instance: gpu.NativeInstance,
    adapter_type: gpu.Adapter.Type,
    backend_type: gpu.Adapter.BackendType,
    //device: gpu.Device,
    window: glfw.Window,

    pub fn init(window: glfw.Window) GraphicsContext {
        c.dawnProcSetProcs(c.machDawnNativeGetProcs());
        const instance = c.machDawnNativeInstance_init();
        c.machDawnNativeInstance_discoverDefaultAdapters(instance);

        var native_instance = gpu.NativeInstance.wrap(c.machDawnNativeInstance_get(instance).?);

        const gpu_interface = native_instance.interface();
        const backend_adapter = switch (gpu_interface.waitForAdapter(&.{
            .power_preference = .high_performance,
        })) {
            .adapter => |v| v,
            .err => |err| {
                std.debug.print("failed to get adapter: error={} {s}\n", .{ err.code, err.message });
                std.process.exit(1);
            },
        };

        // Print which adapter we are going to use.
        const props = backend_adapter.properties;
        std.debug.print("zgpu: found {s} backend on {s} adapter: {s}, {s}\n", .{
            gpu.Adapter.backendTypeName(props.backend_type),
            gpu.Adapter.typeName(props.adapter_type),
            props.name,
            props.driver_description,
        });

        return GraphicsContext{
            .native_instance = native_instance,
            .adapter_type = props.adapter_type,
            .backend_type = props.backend_type,
            .window = window,
        };
    }
};
