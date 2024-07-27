const config = @import("rendermodesConfig");

pub const d3d12 = if (config.d3d12) @import("zwin32").d3d12 else struct {
    pub const ICommandQueue = anyopaque;
    pub const IResource = anyopaque;
};

pub const d3d11 = if (config.d3d11) @import("zwin32").d3d11 else struct {
    pub const IShaderResourceView = anyopaque;
    pub const IResource = anyopaque;
    pub const ITexture2D = anyopaque;
    pub const IDevice = anyopaque;
};

pub const opengl = if (config.opengl) @import("zopengl").bindings else struct {
    pub const Uint = c_uint;
};

pub const vulkan = struct { //if (config.vulkan) @import("vulkan") else struct {
    pub const VkPhysicalDevice = anyopaque;
    pub const VkDevice = anyopaque;
    pub const VkInstance = anyopaque;
    pub const VkQueue = anyopaque;
};
