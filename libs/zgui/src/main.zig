//--------------------------------------------------------------------------------------------------
//
// Zig bindings for 'dear imgui' library. Easy to use, hand-crafted API with default arguments,
// named parameters and Zig style text formatting.
//
//--------------------------------------------------------------------------------------------------
pub const version = @import("std").SemanticVersion{ .major = 0, .minor = 9, .patch = 2 };

pub usingnamespace @import("gui.zig");
pub const plot = @import("plot.zig");
pub const backend = @import("backend_glfw_wgpu.zig");
pub const offscreen = @import("offscreen_wgpu.zig");
