
#include "dawn/dawn_proc.h"

static DawnProcTable procs;

static DawnProcTable nullProcs;

void dawnProcSetProcs(const DawnProcTable* procs_) {
    if (procs_) {
        procs = *procs_;
    } else {
        procs = nullProcs;
    }
}

WGPUInstance wgpuCreateInstance(WGPUInstanceDescriptor const * descriptor) {
return     procs.createInstance(descriptor);
}
WGPUProc wgpuGetProcAddress(WGPUDevice device, char const * procName) {
return     procs.getProcAddress(device, procName);
}

WGPUDevice wgpuAdapterCreateDevice(WGPUAdapter adapter, WGPUDeviceDescriptor const * descriptor) {
return     procs.adapterCreateDevice(adapter, descriptor);
}
size_t wgpuAdapterEnumerateFeatures(WGPUAdapter adapter, WGPUFeatureName * features) {
return     procs.adapterEnumerateFeatures(adapter, features);
}
WGPUInstance wgpuAdapterGetInstance(WGPUAdapter adapter) {
return     procs.adapterGetInstance(adapter);
}
bool wgpuAdapterGetLimits(WGPUAdapter adapter, WGPUSupportedLimits * limits) {
return     procs.adapterGetLimits(adapter, limits);
}
void wgpuAdapterGetProperties(WGPUAdapter adapter, WGPUAdapterProperties * properties) {
    procs.adapterGetProperties(adapter, properties);
}
bool wgpuAdapterHasFeature(WGPUAdapter adapter, WGPUFeatureName feature) {
return     procs.adapterHasFeature(adapter, feature);
}
void wgpuAdapterRequestDevice(WGPUAdapter adapter, WGPUDeviceDescriptor const * descriptor, WGPURequestDeviceCallback callback, void * userdata) {
    procs.adapterRequestDevice(adapter, descriptor, callback, userdata);
}
void wgpuAdapterReference(WGPUAdapter adapter) {
    procs.adapterReference(adapter);
}
void wgpuAdapterRelease(WGPUAdapter adapter) {
    procs.adapterRelease(adapter);
}

void wgpuBindGroupSetLabel(WGPUBindGroup bindGroup, char const * label) {
    procs.bindGroupSetLabel(bindGroup, label);
}
void wgpuBindGroupReference(WGPUBindGroup bindGroup) {
    procs.bindGroupReference(bindGroup);
}
void wgpuBindGroupRelease(WGPUBindGroup bindGroup) {
    procs.bindGroupRelease(bindGroup);
}

void wgpuBindGroupLayoutSetLabel(WGPUBindGroupLayout bindGroupLayout, char const * label) {
    procs.bindGroupLayoutSetLabel(bindGroupLayout, label);
}
void wgpuBindGroupLayoutReference(WGPUBindGroupLayout bindGroupLayout) {
    procs.bindGroupLayoutReference(bindGroupLayout);
}
void wgpuBindGroupLayoutRelease(WGPUBindGroupLayout bindGroupLayout) {
    procs.bindGroupLayoutRelease(bindGroupLayout);
}

void wgpuBufferDestroy(WGPUBuffer buffer) {
    procs.bufferDestroy(buffer);
}
void const * wgpuBufferGetConstMappedRange(WGPUBuffer buffer, size_t offset, size_t size) {
return     procs.bufferGetConstMappedRange(buffer, offset, size);
}
WGPUBufferMapState wgpuBufferGetMapState(WGPUBuffer buffer) {
return     procs.bufferGetMapState(buffer);
}
void * wgpuBufferGetMappedRange(WGPUBuffer buffer, size_t offset, size_t size) {
return     procs.bufferGetMappedRange(buffer, offset, size);
}
uint64_t wgpuBufferGetSize(WGPUBuffer buffer) {
return     procs.bufferGetSize(buffer);
}
WGPUBufferUsageFlags wgpuBufferGetUsage(WGPUBuffer buffer) {
return     procs.bufferGetUsage(buffer);
}
void wgpuBufferMapAsync(WGPUBuffer buffer, WGPUMapModeFlags mode, size_t offset, size_t size, WGPUBufferMapCallback callback, void * userdata) {
    procs.bufferMapAsync(buffer, mode, offset, size, callback, userdata);
}
void wgpuBufferSetLabel(WGPUBuffer buffer, char const * label) {
    procs.bufferSetLabel(buffer, label);
}
void wgpuBufferUnmap(WGPUBuffer buffer) {
    procs.bufferUnmap(buffer);
}
void wgpuBufferReference(WGPUBuffer buffer) {
    procs.bufferReference(buffer);
}
void wgpuBufferRelease(WGPUBuffer buffer) {
    procs.bufferRelease(buffer);
}

void wgpuCommandBufferSetLabel(WGPUCommandBuffer commandBuffer, char const * label) {
    procs.commandBufferSetLabel(commandBuffer, label);
}
void wgpuCommandBufferReference(WGPUCommandBuffer commandBuffer) {
    procs.commandBufferReference(commandBuffer);
}
void wgpuCommandBufferRelease(WGPUCommandBuffer commandBuffer) {
    procs.commandBufferRelease(commandBuffer);
}

WGPUComputePassEncoder wgpuCommandEncoderBeginComputePass(WGPUCommandEncoder commandEncoder, WGPUComputePassDescriptor const * descriptor) {
return     procs.commandEncoderBeginComputePass(commandEncoder, descriptor);
}
WGPURenderPassEncoder wgpuCommandEncoderBeginRenderPass(WGPUCommandEncoder commandEncoder, WGPURenderPassDescriptor const * descriptor) {
return     procs.commandEncoderBeginRenderPass(commandEncoder, descriptor);
}
void wgpuCommandEncoderClearBuffer(WGPUCommandEncoder commandEncoder, WGPUBuffer buffer, uint64_t offset, uint64_t size) {
    procs.commandEncoderClearBuffer(commandEncoder, buffer, offset, size);
}
void wgpuCommandEncoderCopyBufferToBuffer(WGPUCommandEncoder commandEncoder, WGPUBuffer source, uint64_t sourceOffset, WGPUBuffer destination, uint64_t destinationOffset, uint64_t size) {
    procs.commandEncoderCopyBufferToBuffer(commandEncoder, source, sourceOffset, destination, destinationOffset, size);
}
void wgpuCommandEncoderCopyBufferToTexture(WGPUCommandEncoder commandEncoder, WGPUImageCopyBuffer const * source, WGPUImageCopyTexture const * destination, WGPUExtent3D const * copySize) {
    procs.commandEncoderCopyBufferToTexture(commandEncoder, source, destination, copySize);
}
void wgpuCommandEncoderCopyTextureToBuffer(WGPUCommandEncoder commandEncoder, WGPUImageCopyTexture const * source, WGPUImageCopyBuffer const * destination, WGPUExtent3D const * copySize) {
    procs.commandEncoderCopyTextureToBuffer(commandEncoder, source, destination, copySize);
}
void wgpuCommandEncoderCopyTextureToTexture(WGPUCommandEncoder commandEncoder, WGPUImageCopyTexture const * source, WGPUImageCopyTexture const * destination, WGPUExtent3D const * copySize) {
    procs.commandEncoderCopyTextureToTexture(commandEncoder, source, destination, copySize);
}
void wgpuCommandEncoderCopyTextureToTextureInternal(WGPUCommandEncoder commandEncoder, WGPUImageCopyTexture const * source, WGPUImageCopyTexture const * destination, WGPUExtent3D const * copySize) {
    procs.commandEncoderCopyTextureToTextureInternal(commandEncoder, source, destination, copySize);
}
WGPUCommandBuffer wgpuCommandEncoderFinish(WGPUCommandEncoder commandEncoder, WGPUCommandBufferDescriptor const * descriptor) {
return     procs.commandEncoderFinish(commandEncoder, descriptor);
}
void wgpuCommandEncoderInjectValidationError(WGPUCommandEncoder commandEncoder, char const * message) {
    procs.commandEncoderInjectValidationError(commandEncoder, message);
}
void wgpuCommandEncoderInsertDebugMarker(WGPUCommandEncoder commandEncoder, char const * markerLabel) {
    procs.commandEncoderInsertDebugMarker(commandEncoder, markerLabel);
}
void wgpuCommandEncoderPopDebugGroup(WGPUCommandEncoder commandEncoder) {
    procs.commandEncoderPopDebugGroup(commandEncoder);
}
void wgpuCommandEncoderPushDebugGroup(WGPUCommandEncoder commandEncoder, char const * groupLabel) {
    procs.commandEncoderPushDebugGroup(commandEncoder, groupLabel);
}
void wgpuCommandEncoderResolveQuerySet(WGPUCommandEncoder commandEncoder, WGPUQuerySet querySet, uint32_t firstQuery, uint32_t queryCount, WGPUBuffer destination, uint64_t destinationOffset) {
    procs.commandEncoderResolveQuerySet(commandEncoder, querySet, firstQuery, queryCount, destination, destinationOffset);
}
void wgpuCommandEncoderSetLabel(WGPUCommandEncoder commandEncoder, char const * label) {
    procs.commandEncoderSetLabel(commandEncoder, label);
}
void wgpuCommandEncoderWriteBuffer(WGPUCommandEncoder commandEncoder, WGPUBuffer buffer, uint64_t bufferOffset, uint8_t const * data, uint64_t size) {
    procs.commandEncoderWriteBuffer(commandEncoder, buffer, bufferOffset, data, size);
}
void wgpuCommandEncoderWriteTimestamp(WGPUCommandEncoder commandEncoder, WGPUQuerySet querySet, uint32_t queryIndex) {
    procs.commandEncoderWriteTimestamp(commandEncoder, querySet, queryIndex);
}
void wgpuCommandEncoderReference(WGPUCommandEncoder commandEncoder) {
    procs.commandEncoderReference(commandEncoder);
}
void wgpuCommandEncoderRelease(WGPUCommandEncoder commandEncoder) {
    procs.commandEncoderRelease(commandEncoder);
}

void wgpuComputePassEncoderDispatchWorkgroups(WGPUComputePassEncoder computePassEncoder, uint32_t workgroupCountX, uint32_t workgroupCountY, uint32_t workgroupCountZ) {
    procs.computePassEncoderDispatchWorkgroups(computePassEncoder, workgroupCountX, workgroupCountY, workgroupCountZ);
}
void wgpuComputePassEncoderDispatchWorkgroupsIndirect(WGPUComputePassEncoder computePassEncoder, WGPUBuffer indirectBuffer, uint64_t indirectOffset) {
    procs.computePassEncoderDispatchWorkgroupsIndirect(computePassEncoder, indirectBuffer, indirectOffset);
}
void wgpuComputePassEncoderEnd(WGPUComputePassEncoder computePassEncoder) {
    procs.computePassEncoderEnd(computePassEncoder);
}
void wgpuComputePassEncoderInsertDebugMarker(WGPUComputePassEncoder computePassEncoder, char const * markerLabel) {
    procs.computePassEncoderInsertDebugMarker(computePassEncoder, markerLabel);
}
void wgpuComputePassEncoderPopDebugGroup(WGPUComputePassEncoder computePassEncoder) {
    procs.computePassEncoderPopDebugGroup(computePassEncoder);
}
void wgpuComputePassEncoderPushDebugGroup(WGPUComputePassEncoder computePassEncoder, char const * groupLabel) {
    procs.computePassEncoderPushDebugGroup(computePassEncoder, groupLabel);
}
void wgpuComputePassEncoderSetBindGroup(WGPUComputePassEncoder computePassEncoder, uint32_t groupIndex, WGPUBindGroup group, size_t dynamicOffsetCount, uint32_t const * dynamicOffsets) {
    procs.computePassEncoderSetBindGroup(computePassEncoder, groupIndex, group, dynamicOffsetCount, dynamicOffsets);
}
void wgpuComputePassEncoderSetLabel(WGPUComputePassEncoder computePassEncoder, char const * label) {
    procs.computePassEncoderSetLabel(computePassEncoder, label);
}
void wgpuComputePassEncoderSetPipeline(WGPUComputePassEncoder computePassEncoder, WGPUComputePipeline pipeline) {
    procs.computePassEncoderSetPipeline(computePassEncoder, pipeline);
}
void wgpuComputePassEncoderWriteTimestamp(WGPUComputePassEncoder computePassEncoder, WGPUQuerySet querySet, uint32_t queryIndex) {
    procs.computePassEncoderWriteTimestamp(computePassEncoder, querySet, queryIndex);
}
void wgpuComputePassEncoderReference(WGPUComputePassEncoder computePassEncoder) {
    procs.computePassEncoderReference(computePassEncoder);
}
void wgpuComputePassEncoderRelease(WGPUComputePassEncoder computePassEncoder) {
    procs.computePassEncoderRelease(computePassEncoder);
}

WGPUBindGroupLayout wgpuComputePipelineGetBindGroupLayout(WGPUComputePipeline computePipeline, uint32_t groupIndex) {
return     procs.computePipelineGetBindGroupLayout(computePipeline, groupIndex);
}
void wgpuComputePipelineSetLabel(WGPUComputePipeline computePipeline, char const * label) {
    procs.computePipelineSetLabel(computePipeline, label);
}
void wgpuComputePipelineReference(WGPUComputePipeline computePipeline) {
    procs.computePipelineReference(computePipeline);
}
void wgpuComputePipelineRelease(WGPUComputePipeline computePipeline) {
    procs.computePipelineRelease(computePipeline);
}

WGPUBindGroup wgpuDeviceCreateBindGroup(WGPUDevice device, WGPUBindGroupDescriptor const * descriptor) {
return     procs.deviceCreateBindGroup(device, descriptor);
}
WGPUBindGroupLayout wgpuDeviceCreateBindGroupLayout(WGPUDevice device, WGPUBindGroupLayoutDescriptor const * descriptor) {
return     procs.deviceCreateBindGroupLayout(device, descriptor);
}
WGPUBuffer wgpuDeviceCreateBuffer(WGPUDevice device, WGPUBufferDescriptor const * descriptor) {
return     procs.deviceCreateBuffer(device, descriptor);
}
WGPUCommandEncoder wgpuDeviceCreateCommandEncoder(WGPUDevice device, WGPUCommandEncoderDescriptor const * descriptor) {
return     procs.deviceCreateCommandEncoder(device, descriptor);
}
WGPUComputePipeline wgpuDeviceCreateComputePipeline(WGPUDevice device, WGPUComputePipelineDescriptor const * descriptor) {
return     procs.deviceCreateComputePipeline(device, descriptor);
}
void wgpuDeviceCreateComputePipelineAsync(WGPUDevice device, WGPUComputePipelineDescriptor const * descriptor, WGPUCreateComputePipelineAsyncCallback callback, void * userdata) {
    procs.deviceCreateComputePipelineAsync(device, descriptor, callback, userdata);
}
WGPUBuffer wgpuDeviceCreateErrorBuffer(WGPUDevice device, WGPUBufferDescriptor const * descriptor) {
return     procs.deviceCreateErrorBuffer(device, descriptor);
}
WGPUExternalTexture wgpuDeviceCreateErrorExternalTexture(WGPUDevice device) {
return     procs.deviceCreateErrorExternalTexture(device);
}
WGPUShaderModule wgpuDeviceCreateErrorShaderModule(WGPUDevice device, WGPUShaderModuleDescriptor const * descriptor, char const * errorMessage) {
return     procs.deviceCreateErrorShaderModule(device, descriptor, errorMessage);
}
WGPUTexture wgpuDeviceCreateErrorTexture(WGPUDevice device, WGPUTextureDescriptor const * descriptor) {
return     procs.deviceCreateErrorTexture(device, descriptor);
}
WGPUExternalTexture wgpuDeviceCreateExternalTexture(WGPUDevice device, WGPUExternalTextureDescriptor const * externalTextureDescriptor) {
return     procs.deviceCreateExternalTexture(device, externalTextureDescriptor);
}
WGPUPipelineLayout wgpuDeviceCreatePipelineLayout(WGPUDevice device, WGPUPipelineLayoutDescriptor const * descriptor) {
return     procs.deviceCreatePipelineLayout(device, descriptor);
}
WGPUQuerySet wgpuDeviceCreateQuerySet(WGPUDevice device, WGPUQuerySetDescriptor const * descriptor) {
return     procs.deviceCreateQuerySet(device, descriptor);
}
WGPURenderBundleEncoder wgpuDeviceCreateRenderBundleEncoder(WGPUDevice device, WGPURenderBundleEncoderDescriptor const * descriptor) {
return     procs.deviceCreateRenderBundleEncoder(device, descriptor);
}
WGPURenderPipeline wgpuDeviceCreateRenderPipeline(WGPUDevice device, WGPURenderPipelineDescriptor const * descriptor) {
return     procs.deviceCreateRenderPipeline(device, descriptor);
}
void wgpuDeviceCreateRenderPipelineAsync(WGPUDevice device, WGPURenderPipelineDescriptor const * descriptor, WGPUCreateRenderPipelineAsyncCallback callback, void * userdata) {
    procs.deviceCreateRenderPipelineAsync(device, descriptor, callback, userdata);
}
WGPUSampler wgpuDeviceCreateSampler(WGPUDevice device, WGPUSamplerDescriptor const * descriptor) {
return     procs.deviceCreateSampler(device, descriptor);
}
WGPUShaderModule wgpuDeviceCreateShaderModule(WGPUDevice device, WGPUShaderModuleDescriptor const * descriptor) {
return     procs.deviceCreateShaderModule(device, descriptor);
}
WGPUSwapChain wgpuDeviceCreateSwapChain(WGPUDevice device, WGPUSurface surface, WGPUSwapChainDescriptor const * descriptor) {
return     procs.deviceCreateSwapChain(device, surface, descriptor);
}
WGPUTexture wgpuDeviceCreateTexture(WGPUDevice device, WGPUTextureDescriptor const * descriptor) {
return     procs.deviceCreateTexture(device, descriptor);
}
void wgpuDeviceDestroy(WGPUDevice device) {
    procs.deviceDestroy(device);
}
size_t wgpuDeviceEnumerateFeatures(WGPUDevice device, WGPUFeatureName * features) {
return     procs.deviceEnumerateFeatures(device, features);
}
void wgpuDeviceForceLoss(WGPUDevice device, WGPUDeviceLostReason type, char const * message) {
    procs.deviceForceLoss(device, type, message);
}
WGPUAdapter wgpuDeviceGetAdapter(WGPUDevice device) {
return     procs.deviceGetAdapter(device);
}
bool wgpuDeviceGetLimits(WGPUDevice device, WGPUSupportedLimits * limits) {
return     procs.deviceGetLimits(device, limits);
}
WGPUQueue wgpuDeviceGetQueue(WGPUDevice device) {
return     procs.deviceGetQueue(device);
}
WGPUTextureUsageFlags wgpuDeviceGetSupportedSurfaceUsage(WGPUDevice device, WGPUSurface surface) {
return     procs.deviceGetSupportedSurfaceUsage(device, surface);
}
bool wgpuDeviceHasFeature(WGPUDevice device, WGPUFeatureName feature) {
return     procs.deviceHasFeature(device, feature);
}
void wgpuDeviceInjectError(WGPUDevice device, WGPUErrorType type, char const * message) {
    procs.deviceInjectError(device, type, message);
}
void wgpuDevicePopErrorScope(WGPUDevice device, WGPUErrorCallback callback, void * userdata) {
    procs.devicePopErrorScope(device, callback, userdata);
}
void wgpuDevicePushErrorScope(WGPUDevice device, WGPUErrorFilter filter) {
    procs.devicePushErrorScope(device, filter);
}
void wgpuDeviceSetDeviceLostCallback(WGPUDevice device, WGPUDeviceLostCallback callback, void * userdata) {
    procs.deviceSetDeviceLostCallback(device, callback, userdata);
}
void wgpuDeviceSetLabel(WGPUDevice device, char const * label) {
    procs.deviceSetLabel(device, label);
}
void wgpuDeviceSetLoggingCallback(WGPUDevice device, WGPULoggingCallback callback, void * userdata) {
    procs.deviceSetLoggingCallback(device, callback, userdata);
}
void wgpuDeviceSetUncapturedErrorCallback(WGPUDevice device, WGPUErrorCallback callback, void * userdata) {
    procs.deviceSetUncapturedErrorCallback(device, callback, userdata);
}
void wgpuDeviceTick(WGPUDevice device) {
    procs.deviceTick(device);
}
void wgpuDeviceValidateTextureDescriptor(WGPUDevice device, WGPUTextureDescriptor const * descriptor) {
    procs.deviceValidateTextureDescriptor(device, descriptor);
}
void wgpuDeviceReference(WGPUDevice device) {
    procs.deviceReference(device);
}
void wgpuDeviceRelease(WGPUDevice device) {
    procs.deviceRelease(device);
}

void wgpuExternalTextureDestroy(WGPUExternalTexture externalTexture) {
    procs.externalTextureDestroy(externalTexture);
}
void wgpuExternalTextureExpire(WGPUExternalTexture externalTexture) {
    procs.externalTextureExpire(externalTexture);
}
void wgpuExternalTextureRefresh(WGPUExternalTexture externalTexture) {
    procs.externalTextureRefresh(externalTexture);
}
void wgpuExternalTextureSetLabel(WGPUExternalTexture externalTexture, char const * label) {
    procs.externalTextureSetLabel(externalTexture, label);
}
void wgpuExternalTextureReference(WGPUExternalTexture externalTexture) {
    procs.externalTextureReference(externalTexture);
}
void wgpuExternalTextureRelease(WGPUExternalTexture externalTexture) {
    procs.externalTextureRelease(externalTexture);
}

WGPUSurface wgpuInstanceCreateSurface(WGPUInstance instance, WGPUSurfaceDescriptor const * descriptor) {
return     procs.instanceCreateSurface(instance, descriptor);
}
void wgpuInstanceProcessEvents(WGPUInstance instance) {
    procs.instanceProcessEvents(instance);
}
void wgpuInstanceRequestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const * options, WGPURequestAdapterCallback callback, void * userdata) {
    procs.instanceRequestAdapter(instance, options, callback, userdata);
}
void wgpuInstanceReference(WGPUInstance instance) {
    procs.instanceReference(instance);
}
void wgpuInstanceRelease(WGPUInstance instance) {
    procs.instanceRelease(instance);
}

void wgpuPipelineLayoutSetLabel(WGPUPipelineLayout pipelineLayout, char const * label) {
    procs.pipelineLayoutSetLabel(pipelineLayout, label);
}
void wgpuPipelineLayoutReference(WGPUPipelineLayout pipelineLayout) {
    procs.pipelineLayoutReference(pipelineLayout);
}
void wgpuPipelineLayoutRelease(WGPUPipelineLayout pipelineLayout) {
    procs.pipelineLayoutRelease(pipelineLayout);
}

void wgpuQuerySetDestroy(WGPUQuerySet querySet) {
    procs.querySetDestroy(querySet);
}
uint32_t wgpuQuerySetGetCount(WGPUQuerySet querySet) {
return     procs.querySetGetCount(querySet);
}
WGPUQueryType wgpuQuerySetGetType(WGPUQuerySet querySet) {
return     procs.querySetGetType(querySet);
}
void wgpuQuerySetSetLabel(WGPUQuerySet querySet, char const * label) {
    procs.querySetSetLabel(querySet, label);
}
void wgpuQuerySetReference(WGPUQuerySet querySet) {
    procs.querySetReference(querySet);
}
void wgpuQuerySetRelease(WGPUQuerySet querySet) {
    procs.querySetRelease(querySet);
}

void wgpuQueueCopyExternalTextureForBrowser(WGPUQueue queue, WGPUImageCopyExternalTexture const * source, WGPUImageCopyTexture const * destination, WGPUExtent3D const * copySize, WGPUCopyTextureForBrowserOptions const * options) {
    procs.queueCopyExternalTextureForBrowser(queue, source, destination, copySize, options);
}
void wgpuQueueCopyTextureForBrowser(WGPUQueue queue, WGPUImageCopyTexture const * source, WGPUImageCopyTexture const * destination, WGPUExtent3D const * copySize, WGPUCopyTextureForBrowserOptions const * options) {
    procs.queueCopyTextureForBrowser(queue, source, destination, copySize, options);
}
void wgpuQueueOnSubmittedWorkDone(WGPUQueue queue, uint64_t signalValue, WGPUQueueWorkDoneCallback callback, void * userdata) {
    procs.queueOnSubmittedWorkDone(queue, signalValue, callback, userdata);
}
void wgpuQueueSetLabel(WGPUQueue queue, char const * label) {
    procs.queueSetLabel(queue, label);
}
void wgpuQueueSubmit(WGPUQueue queue, size_t commandCount, WGPUCommandBuffer const * commands) {
    procs.queueSubmit(queue, commandCount, commands);
}
void wgpuQueueWriteBuffer(WGPUQueue queue, WGPUBuffer buffer, uint64_t bufferOffset, void const * data, size_t size) {
    procs.queueWriteBuffer(queue, buffer, bufferOffset, data, size);
}
void wgpuQueueWriteTexture(WGPUQueue queue, WGPUImageCopyTexture const * destination, void const * data, size_t dataSize, WGPUTextureDataLayout const * dataLayout, WGPUExtent3D const * writeSize) {
    procs.queueWriteTexture(queue, destination, data, dataSize, dataLayout, writeSize);
}
void wgpuQueueReference(WGPUQueue queue) {
    procs.queueReference(queue);
}
void wgpuQueueRelease(WGPUQueue queue) {
    procs.queueRelease(queue);
}

void wgpuRenderBundleSetLabel(WGPURenderBundle renderBundle, char const * label) {
    procs.renderBundleSetLabel(renderBundle, label);
}
void wgpuRenderBundleReference(WGPURenderBundle renderBundle) {
    procs.renderBundleReference(renderBundle);
}
void wgpuRenderBundleRelease(WGPURenderBundle renderBundle) {
    procs.renderBundleRelease(renderBundle);
}

void wgpuRenderBundleEncoderDraw(WGPURenderBundleEncoder renderBundleEncoder, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
    procs.renderBundleEncoderDraw(renderBundleEncoder, vertexCount, instanceCount, firstVertex, firstInstance);
}
void wgpuRenderBundleEncoderDrawIndexed(WGPURenderBundleEncoder renderBundleEncoder, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance) {
    procs.renderBundleEncoderDrawIndexed(renderBundleEncoder, indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}
void wgpuRenderBundleEncoderDrawIndexedIndirect(WGPURenderBundleEncoder renderBundleEncoder, WGPUBuffer indirectBuffer, uint64_t indirectOffset) {
    procs.renderBundleEncoderDrawIndexedIndirect(renderBundleEncoder, indirectBuffer, indirectOffset);
}
void wgpuRenderBundleEncoderDrawIndirect(WGPURenderBundleEncoder renderBundleEncoder, WGPUBuffer indirectBuffer, uint64_t indirectOffset) {
    procs.renderBundleEncoderDrawIndirect(renderBundleEncoder, indirectBuffer, indirectOffset);
}
WGPURenderBundle wgpuRenderBundleEncoderFinish(WGPURenderBundleEncoder renderBundleEncoder, WGPURenderBundleDescriptor const * descriptor) {
return     procs.renderBundleEncoderFinish(renderBundleEncoder, descriptor);
}
void wgpuRenderBundleEncoderInsertDebugMarker(WGPURenderBundleEncoder renderBundleEncoder, char const * markerLabel) {
    procs.renderBundleEncoderInsertDebugMarker(renderBundleEncoder, markerLabel);
}
void wgpuRenderBundleEncoderPopDebugGroup(WGPURenderBundleEncoder renderBundleEncoder) {
    procs.renderBundleEncoderPopDebugGroup(renderBundleEncoder);
}
void wgpuRenderBundleEncoderPushDebugGroup(WGPURenderBundleEncoder renderBundleEncoder, char const * groupLabel) {
    procs.renderBundleEncoderPushDebugGroup(renderBundleEncoder, groupLabel);
}
void wgpuRenderBundleEncoderSetBindGroup(WGPURenderBundleEncoder renderBundleEncoder, uint32_t groupIndex, WGPUBindGroup group, size_t dynamicOffsetCount, uint32_t const * dynamicOffsets) {
    procs.renderBundleEncoderSetBindGroup(renderBundleEncoder, groupIndex, group, dynamicOffsetCount, dynamicOffsets);
}
void wgpuRenderBundleEncoderSetIndexBuffer(WGPURenderBundleEncoder renderBundleEncoder, WGPUBuffer buffer, WGPUIndexFormat format, uint64_t offset, uint64_t size) {
    procs.renderBundleEncoderSetIndexBuffer(renderBundleEncoder, buffer, format, offset, size);
}
void wgpuRenderBundleEncoderSetLabel(WGPURenderBundleEncoder renderBundleEncoder, char const * label) {
    procs.renderBundleEncoderSetLabel(renderBundleEncoder, label);
}
void wgpuRenderBundleEncoderSetPipeline(WGPURenderBundleEncoder renderBundleEncoder, WGPURenderPipeline pipeline) {
    procs.renderBundleEncoderSetPipeline(renderBundleEncoder, pipeline);
}
void wgpuRenderBundleEncoderSetVertexBuffer(WGPURenderBundleEncoder renderBundleEncoder, uint32_t slot, WGPUBuffer buffer, uint64_t offset, uint64_t size) {
    procs.renderBundleEncoderSetVertexBuffer(renderBundleEncoder, slot, buffer, offset, size);
}
void wgpuRenderBundleEncoderReference(WGPURenderBundleEncoder renderBundleEncoder) {
    procs.renderBundleEncoderReference(renderBundleEncoder);
}
void wgpuRenderBundleEncoderRelease(WGPURenderBundleEncoder renderBundleEncoder) {
    procs.renderBundleEncoderRelease(renderBundleEncoder);
}

void wgpuRenderPassEncoderBeginOcclusionQuery(WGPURenderPassEncoder renderPassEncoder, uint32_t queryIndex) {
    procs.renderPassEncoderBeginOcclusionQuery(renderPassEncoder, queryIndex);
}
void wgpuRenderPassEncoderDraw(WGPURenderPassEncoder renderPassEncoder, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
    procs.renderPassEncoderDraw(renderPassEncoder, vertexCount, instanceCount, firstVertex, firstInstance);
}
void wgpuRenderPassEncoderDrawIndexed(WGPURenderPassEncoder renderPassEncoder, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance) {
    procs.renderPassEncoderDrawIndexed(renderPassEncoder, indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}
void wgpuRenderPassEncoderDrawIndexedIndirect(WGPURenderPassEncoder renderPassEncoder, WGPUBuffer indirectBuffer, uint64_t indirectOffset) {
    procs.renderPassEncoderDrawIndexedIndirect(renderPassEncoder, indirectBuffer, indirectOffset);
}
void wgpuRenderPassEncoderDrawIndirect(WGPURenderPassEncoder renderPassEncoder, WGPUBuffer indirectBuffer, uint64_t indirectOffset) {
    procs.renderPassEncoderDrawIndirect(renderPassEncoder, indirectBuffer, indirectOffset);
}
void wgpuRenderPassEncoderEnd(WGPURenderPassEncoder renderPassEncoder) {
    procs.renderPassEncoderEnd(renderPassEncoder);
}
void wgpuRenderPassEncoderEndOcclusionQuery(WGPURenderPassEncoder renderPassEncoder) {
    procs.renderPassEncoderEndOcclusionQuery(renderPassEncoder);
}
void wgpuRenderPassEncoderExecuteBundles(WGPURenderPassEncoder renderPassEncoder, size_t bundleCount, WGPURenderBundle const * bundles) {
    procs.renderPassEncoderExecuteBundles(renderPassEncoder, bundleCount, bundles);
}
void wgpuRenderPassEncoderInsertDebugMarker(WGPURenderPassEncoder renderPassEncoder, char const * markerLabel) {
    procs.renderPassEncoderInsertDebugMarker(renderPassEncoder, markerLabel);
}
void wgpuRenderPassEncoderPopDebugGroup(WGPURenderPassEncoder renderPassEncoder) {
    procs.renderPassEncoderPopDebugGroup(renderPassEncoder);
}
void wgpuRenderPassEncoderPushDebugGroup(WGPURenderPassEncoder renderPassEncoder, char const * groupLabel) {
    procs.renderPassEncoderPushDebugGroup(renderPassEncoder, groupLabel);
}
void wgpuRenderPassEncoderSetBindGroup(WGPURenderPassEncoder renderPassEncoder, uint32_t groupIndex, WGPUBindGroup group, size_t dynamicOffsetCount, uint32_t const * dynamicOffsets) {
    procs.renderPassEncoderSetBindGroup(renderPassEncoder, groupIndex, group, dynamicOffsetCount, dynamicOffsets);
}
void wgpuRenderPassEncoderSetBlendConstant(WGPURenderPassEncoder renderPassEncoder, WGPUColor const * color) {
    procs.renderPassEncoderSetBlendConstant(renderPassEncoder, color);
}
void wgpuRenderPassEncoderSetIndexBuffer(WGPURenderPassEncoder renderPassEncoder, WGPUBuffer buffer, WGPUIndexFormat format, uint64_t offset, uint64_t size) {
    procs.renderPassEncoderSetIndexBuffer(renderPassEncoder, buffer, format, offset, size);
}
void wgpuRenderPassEncoderSetLabel(WGPURenderPassEncoder renderPassEncoder, char const * label) {
    procs.renderPassEncoderSetLabel(renderPassEncoder, label);
}
void wgpuRenderPassEncoderSetPipeline(WGPURenderPassEncoder renderPassEncoder, WGPURenderPipeline pipeline) {
    procs.renderPassEncoderSetPipeline(renderPassEncoder, pipeline);
}
void wgpuRenderPassEncoderSetScissorRect(WGPURenderPassEncoder renderPassEncoder, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    procs.renderPassEncoderSetScissorRect(renderPassEncoder, x, y, width, height);
}
void wgpuRenderPassEncoderSetStencilReference(WGPURenderPassEncoder renderPassEncoder, uint32_t reference) {
    procs.renderPassEncoderSetStencilReference(renderPassEncoder, reference);
}
void wgpuRenderPassEncoderSetVertexBuffer(WGPURenderPassEncoder renderPassEncoder, uint32_t slot, WGPUBuffer buffer, uint64_t offset, uint64_t size) {
    procs.renderPassEncoderSetVertexBuffer(renderPassEncoder, slot, buffer, offset, size);
}
void wgpuRenderPassEncoderSetViewport(WGPURenderPassEncoder renderPassEncoder, float x, float y, float width, float height, float minDepth, float maxDepth) {
    procs.renderPassEncoderSetViewport(renderPassEncoder, x, y, width, height, minDepth, maxDepth);
}
void wgpuRenderPassEncoderWriteTimestamp(WGPURenderPassEncoder renderPassEncoder, WGPUQuerySet querySet, uint32_t queryIndex) {
    procs.renderPassEncoderWriteTimestamp(renderPassEncoder, querySet, queryIndex);
}
void wgpuRenderPassEncoderReference(WGPURenderPassEncoder renderPassEncoder) {
    procs.renderPassEncoderReference(renderPassEncoder);
}
void wgpuRenderPassEncoderRelease(WGPURenderPassEncoder renderPassEncoder) {
    procs.renderPassEncoderRelease(renderPassEncoder);
}

WGPUBindGroupLayout wgpuRenderPipelineGetBindGroupLayout(WGPURenderPipeline renderPipeline, uint32_t groupIndex) {
return     procs.renderPipelineGetBindGroupLayout(renderPipeline, groupIndex);
}
void wgpuRenderPipelineSetLabel(WGPURenderPipeline renderPipeline, char const * label) {
    procs.renderPipelineSetLabel(renderPipeline, label);
}
void wgpuRenderPipelineReference(WGPURenderPipeline renderPipeline) {
    procs.renderPipelineReference(renderPipeline);
}
void wgpuRenderPipelineRelease(WGPURenderPipeline renderPipeline) {
    procs.renderPipelineRelease(renderPipeline);
}

void wgpuSamplerSetLabel(WGPUSampler sampler, char const * label) {
    procs.samplerSetLabel(sampler, label);
}
void wgpuSamplerReference(WGPUSampler sampler) {
    procs.samplerReference(sampler);
}
void wgpuSamplerRelease(WGPUSampler sampler) {
    procs.samplerRelease(sampler);
}

void wgpuShaderModuleGetCompilationInfo(WGPUShaderModule shaderModule, WGPUCompilationInfoCallback callback, void * userdata) {
    procs.shaderModuleGetCompilationInfo(shaderModule, callback, userdata);
}
void wgpuShaderModuleSetLabel(WGPUShaderModule shaderModule, char const * label) {
    procs.shaderModuleSetLabel(shaderModule, label);
}
void wgpuShaderModuleReference(WGPUShaderModule shaderModule) {
    procs.shaderModuleReference(shaderModule);
}
void wgpuShaderModuleRelease(WGPUShaderModule shaderModule) {
    procs.shaderModuleRelease(shaderModule);
}

void wgpuSurfaceReference(WGPUSurface surface) {
    procs.surfaceReference(surface);
}
void wgpuSurfaceRelease(WGPUSurface surface) {
    procs.surfaceRelease(surface);
}

WGPUTexture wgpuSwapChainGetCurrentTexture(WGPUSwapChain swapChain) {
return     procs.swapChainGetCurrentTexture(swapChain);
}
WGPUTextureView wgpuSwapChainGetCurrentTextureView(WGPUSwapChain swapChain) {
return     procs.swapChainGetCurrentTextureView(swapChain);
}
void wgpuSwapChainPresent(WGPUSwapChain swapChain) {
    procs.swapChainPresent(swapChain);
}
void wgpuSwapChainReference(WGPUSwapChain swapChain) {
    procs.swapChainReference(swapChain);
}
void wgpuSwapChainRelease(WGPUSwapChain swapChain) {
    procs.swapChainRelease(swapChain);
}

WGPUTextureView wgpuTextureCreateView(WGPUTexture texture, WGPUTextureViewDescriptor const * descriptor) {
return     procs.textureCreateView(texture, descriptor);
}
void wgpuTextureDestroy(WGPUTexture texture) {
    procs.textureDestroy(texture);
}
uint32_t wgpuTextureGetDepthOrArrayLayers(WGPUTexture texture) {
return     procs.textureGetDepthOrArrayLayers(texture);
}
WGPUTextureDimension wgpuTextureGetDimension(WGPUTexture texture) {
return     procs.textureGetDimension(texture);
}
WGPUTextureFormat wgpuTextureGetFormat(WGPUTexture texture) {
return     procs.textureGetFormat(texture);
}
uint32_t wgpuTextureGetHeight(WGPUTexture texture) {
return     procs.textureGetHeight(texture);
}
uint32_t wgpuTextureGetMipLevelCount(WGPUTexture texture) {
return     procs.textureGetMipLevelCount(texture);
}
uint32_t wgpuTextureGetSampleCount(WGPUTexture texture) {
return     procs.textureGetSampleCount(texture);
}
WGPUTextureUsageFlags wgpuTextureGetUsage(WGPUTexture texture) {
return     procs.textureGetUsage(texture);
}
uint32_t wgpuTextureGetWidth(WGPUTexture texture) {
return     procs.textureGetWidth(texture);
}
void wgpuTextureSetLabel(WGPUTexture texture, char const * label) {
    procs.textureSetLabel(texture, label);
}
void wgpuTextureReference(WGPUTexture texture) {
    procs.textureReference(texture);
}
void wgpuTextureRelease(WGPUTexture texture) {
    procs.textureRelease(texture);
}

void wgpuTextureViewSetLabel(WGPUTextureView textureView, char const * label) {
    procs.textureViewSetLabel(textureView, label);
}
void wgpuTextureViewReference(WGPUTextureView textureView) {
    procs.textureViewReference(textureView);
}
void wgpuTextureViewRelease(WGPUTextureView textureView) {
    procs.textureViewRelease(textureView);
}

