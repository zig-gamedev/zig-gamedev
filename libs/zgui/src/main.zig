//--------------------------------------------------------------------------------------------------
//
// Zig bindings for 'dear imgui' library. Easy to use, hand-crafted API with default arguments,
// named parameters and Zig style text formatting.
//
//--------------------------------------------------------------------------------------------------
pub const version = @import("std").SemanticVersion{ .major = 0, .minor = 9, .patch = 6 };

pub usingnamespace @import("gui.zig");
pub const plot = @import("plot.zig");
pub const backend = switch (@import("zgui_options").backend) {
    .glfw_wgpu => @import("backend_glfw_wgpu.zig"),
    .win32_dx12 => .{}, // TODO:
    .no_backend => .{},
};
