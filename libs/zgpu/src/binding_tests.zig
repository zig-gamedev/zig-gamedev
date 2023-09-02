const std = @import("std");
const testing = std.testing;
const c = @cImport({
    @cInclude("webgpu/webgpu.h");
});
const gpu = @import("wgpu.zig");

// test struct sizes
fn assertStructBindings(zig_type: anytype, c_type: anytype) void {
    if (@sizeOf(zig_type) != @sizeOf(c_type)) {
        @compileLog("emscripten zgpu type has different size from webgpu.h type:", zig_type, c_type, @sizeOf(zig_type), @sizeOf(c_type));
        unreachable;
    }
    // slow to build tests that try to verify each struct field size and alignment to match between webgpu.h (either shipped with dawn or emscripten) and wgpu.zig
    //     @setEvalBranchQuota(2000);
    //     const zig_fields = @typeInfo(zig_type).Struct.fields;
    //     const c_fields = @typeInfo(c_type).Struct.fields;
    //     std.debug.assert(zig_fields.len == c_fields.len);
    //     var i = 0; 
    //     while (i<zig_fields.len) : (i+=1) {
    //         std.debug.assert(zig_fields[i].alignment == c_fields[i].alignment);
    //         std.debug.assert(@sizeOf(zig_fields[i].@"type") == @sizeOf(c_fields[i].@"type"));
    //     }
}

comptime {
    assertStructBindings(gpu.ChainedStruct, c.WGPUChainedStruct);
    assertStructBindings(gpu.ChainedStructOut, c.WGPUChainedStructOut);
    assertStructBindings(gpu.AdapterProperties, c.WGPUAdapterProperties);
    assertStructBindings(gpu.BindGroupEntry, c.WGPUBindGroupEntry);
    assertStructBindings(gpu.BlendComponent, c.WGPUBlendComponent);
    assertStructBindings(gpu.BufferBindingLayout, c.WGPUBufferBindingLayout);
    assertStructBindings(gpu.BufferDescriptor, c.WGPUBufferDescriptor);
    assertStructBindings(gpu.Color, c.WGPUColor);
    assertStructBindings(gpu.CommandBufferDescriptor, c.WGPUCommandBufferDescriptor);
    assertStructBindings(gpu.CommandEncoderDescriptor, c.WGPUCommandEncoderDescriptor);
    assertStructBindings(gpu.CompilationMessage, c.WGPUCompilationMessage);
    assertStructBindings(gpu.ComputePassTimestampWrite, c.WGPUComputePassTimestampWrite);
    assertStructBindings(gpu.ConstantEntry, c.WGPUConstantEntry);
    assertStructBindings(gpu.Extent3D, c.WGPUExtent3D);
    //assertStructBindings(gpu.InstanceDescriptor, c.WGPUInstanceDescriptor);
    assertStructBindings(gpu.Limits, c.WGPULimits);
    assertStructBindings(gpu.MultisampleState, c.WGPUMultisampleState);
    assertStructBindings(gpu.Origin3D, c.WGPUOrigin3D);
    assertStructBindings(gpu.PipelineLayoutDescriptor, c.WGPUPipelineLayoutDescriptor);
    //assertStructBindings(gpu.PrimitiveDepthClipControl, c.WGPUPrimitiveDepthClipControl);
    assertStructBindings(gpu.PrimitiveState, c.WGPUPrimitiveState);
    assertStructBindings(gpu.QuerySetDescriptor, c.WGPUQuerySetDescriptor);
    //assertStructBindings(gpu.QueueDescriptor, c.WGPUQueueDescriptor);
    assertStructBindings(gpu.RenderBundleDescriptor, c.WGPURenderBundleDescriptor);
    assertStructBindings(gpu.RenderBundleEncoderDescriptor, c.WGPURenderBundleEncoderDescriptor);
    assertStructBindings(gpu.RenderPassDepthStencilAttachment, c.WGPURenderPassDepthStencilAttachment);
    //assertStructBindings(gpu.RenderPassDescriptorMaxDrawCount, c.WGPURenderPassDescriptorMaxDrawCount);
    assertStructBindings(gpu.RenderPassTimestampWrite, c.WGPURenderPassTimestampWrite);
    assertStructBindings(gpu.RequestAdapterOptions, c.WGPURequestAdapterOptions);
    assertStructBindings(gpu.SamplerBindingLayout, c.WGPUSamplerBindingLayout);
    assertStructBindings(gpu.SamplerDescriptor, c.WGPUSamplerDescriptor);
    assertStructBindings(gpu.ShaderModuleDescriptor, c.WGPUShaderModuleDescriptor);
    //assertStructBindings(gpu.ShaderModuleSPIRVDescriptor, c.WGPUShaderModuleSPIRVDescriptor);
    //assertStructBindings(gpu.ShaderModuleWGSLDescriptor, c.WGPUShaderModuleWGSLDescriptor);
    assertStructBindings(gpu.StencilFaceState, c.WGPUStencilFaceState);
    assertStructBindings(gpu.StorageTextureBindingLayout, c.WGPUStorageTextureBindingLayout);
    assertStructBindings(gpu.SurfaceDescriptor, c.WGPUSurfaceDescriptor);
    assertStructBindings(gpu.SurfaceDescriptorFromCanvasHTMLSelector, c.WGPUSurfaceDescriptorFromCanvasHTMLSelector);
    //assertStructBindings(gpu.SwapChainDescriptor, c.WGPUSwapChainDescriptor);
    assertStructBindings(gpu.TextureBindingLayout, c.WGPUTextureBindingLayout);
    assertStructBindings(gpu.TextureDataLayout, c.WGPUTextureDataLayout);
    assertStructBindings(gpu.TextureViewDescriptor, c.WGPUTextureViewDescriptor);
    assertStructBindings(gpu.VertexAttribute, c.WGPUVertexAttribute);
    assertStructBindings(gpu.BindGroupDescriptor, c.WGPUBindGroupDescriptor);
    assertStructBindings(gpu.BindGroupLayoutEntry, c.WGPUBindGroupLayoutEntry);
    assertStructBindings(gpu.BlendState, c.WGPUBlendState);
    assertStructBindings(gpu.CompilationInfo, c.WGPUCompilationInfo);
    assertStructBindings(gpu.ComputePassDescriptor, c.WGPUComputePassDescriptor);
    assertStructBindings(gpu.DepthStencilState, c.WGPUDepthStencilState);
    assertStructBindings(gpu.ImageCopyBuffer, c.WGPUImageCopyBuffer);
    assertStructBindings(gpu.ImageCopyTexture, c.WGPUImageCopyTexture);
    assertStructBindings(gpu.ProgrammableStageDescriptor, c.WGPUProgrammableStageDescriptor);
    assertStructBindings(gpu.RenderPassColorAttachment, c.WGPURenderPassColorAttachment);
    assertStructBindings(gpu.RequiredLimits, c.WGPURequiredLimits);
    assertStructBindings(gpu.SupportedLimits, c.WGPUSupportedLimits);
    assertStructBindings(gpu.TextureDescriptor, c.WGPUTextureDescriptor);
    assertStructBindings(gpu.VertexBufferLayout, c.WGPUVertexBufferLayout);
    assertStructBindings(gpu.BindGroupLayoutDescriptor, c.WGPUBindGroupLayoutDescriptor);
    assertStructBindings(gpu.ColorTargetState, c.WGPUColorTargetState);
    assertStructBindings(gpu.ComputePipelineDescriptor, c.WGPUComputePipelineDescriptor);
    assertStructBindings(gpu.DeviceDescriptor, c.WGPUDeviceDescriptor);
    assertStructBindings(gpu.RenderPassDescriptor, c.WGPURenderPassDescriptor);
    assertStructBindings(gpu.VertexState, c.WGPUVertexState);
    assertStructBindings(gpu.FragmentState, c.WGPUFragmentState);
    assertStructBindings(gpu.RenderPipelineDescriptor, c.WGPURenderPipelineDescriptor);
}
