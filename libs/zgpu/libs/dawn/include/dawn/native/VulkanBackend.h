// Copyright 2018 The Dawn Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef INCLUDE_DAWN_NATIVE_VULKANBACKEND_H_
#define INCLUDE_DAWN_NATIVE_VULKANBACKEND_H_

#include <vulkan/vulkan.h>

#include <array>
#include <vector>

#include "dawn/native/DawnNative.h"

namespace dawn::native::vulkan {

DAWN_NATIVE_EXPORT VkInstance GetInstance(WGPUDevice device);

DAWN_NATIVE_EXPORT PFN_vkVoidFunction GetInstanceProcAddr(WGPUDevice device, const char* pName);

struct DAWN_NATIVE_EXPORT PhysicalDeviceDiscoveryOptions
    : public PhysicalDeviceDiscoveryOptionsBase {
    PhysicalDeviceDiscoveryOptions();

    bool forceSwiftShader = false;
};

// TODO(dawn:1774): Deprecated.
using AdapterDiscoveryOptions = PhysicalDeviceDiscoveryOptions;

enum class NeedsDedicatedAllocation {
    Yes,
    No,
    // Use Vulkan reflection to detect whether a dedicated allocation is needed.
    Detect,
};

struct DAWN_NATIVE_EXPORT ExternalImageDescriptorVk : ExternalImageDescriptor {
  public:
    // The following members may be ignored if |ExternalImageDescriptor::isInitialized| is false
    // since the import does not need to preserve texture contents.

    // See https://www.khronos.org/registry/vulkan/specs/1.1/html/chap7.html. The acquire
    // operation old/new layouts must match exactly the layouts in the release operation. So
    // we may need to issue two barriers releasedOldLayout -> releasedNewLayout ->
    // cTextureDescriptor.usage if the new layout is not compatible with the desired usage.
    // The first barrier is the queue transfer, the second is the layout transition to our
    // desired usage.
    VkImageLayout releasedOldLayout = VK_IMAGE_LAYOUT_GENERAL;
    VkImageLayout releasedNewLayout = VK_IMAGE_LAYOUT_GENERAL;

    // Try to detect the need to use a dedicated allocation for imported images by default but let
    // the application override this as drivers have bugs and forget to require a dedicated
    // allocation.
    NeedsDedicatedAllocation dedicatedAllocation = NeedsDedicatedAllocation::Detect;

  protected:
    using ExternalImageDescriptor::ExternalImageDescriptor;
};

struct ExternalImageExportInfoVk : ExternalImageExportInfo {
  public:
    // See comments in |ExternalImageDescriptorVk|
    // Contains the old/new layouts used in the queue release operation.
    VkImageLayout releasedOldLayout;
    VkImageLayout releasedNewLayout;

  protected:
    using ExternalImageExportInfo::ExternalImageExportInfo;
};

// Can't use DAWN_PLATFORM_IS(LINUX) since header included in both Dawn and Chrome
#ifdef __linux__

// Common properties of external images represented by FDs. On successful import the file
// descriptor's ownership is transferred to the Dawn implementation and they shouldn't be
// used outside of Dawn again. TODO(enga): Also transfer ownership in the error case so the
// caller can assume the FD is always consumed.
struct DAWN_NATIVE_EXPORT ExternalImageDescriptorFD : ExternalImageDescriptorVk {
  public:
    int memoryFD;              // A file descriptor from an export of the memory of the image
    std::vector<int> waitFDs;  // File descriptors of semaphores which will be waited on

  protected:
    using ExternalImageDescriptorVk::ExternalImageDescriptorVk;
};

// Descriptor for opaque file descriptor image import
struct DAWN_NATIVE_EXPORT ExternalImageDescriptorOpaqueFD : ExternalImageDescriptorFD {
    ExternalImageDescriptorOpaqueFD();

    VkDeviceSize allocationSize;  // Must match VkMemoryAllocateInfo from image creation
    uint32_t memoryTypeIndex;     // Must match VkMemoryAllocateInfo from image creation
};

// The plane-wise offset and stride.
struct DAWN_NATIVE_EXPORT PlaneLayout {
    uint64_t offset;
    uint32_t stride;
};

// Descriptor for dma-buf file descriptor image import
struct DAWN_NATIVE_EXPORT ExternalImageDescriptorDmaBuf : ExternalImageDescriptorFD {
    ExternalImageDescriptorDmaBuf();

    static constexpr uint32_t kMaxPlanes = 3;
    std::array<PlaneLayout, kMaxPlanes> planeLayouts;
    uint64_t drmModifier;  // DRM modifier of the buffer
};

// Info struct that is written to in |ExportVulkanImage|.
struct DAWN_NATIVE_EXPORT ExternalImageExportInfoFD : ExternalImageExportInfoVk {
  public:
    // Contains the exported semaphore handles.
    std::vector<int> semaphoreHandles;

  protected:
    using ExternalImageExportInfoVk::ExternalImageExportInfoVk;
};

struct DAWN_NATIVE_EXPORT ExternalImageExportInfoOpaqueFD : ExternalImageExportInfoFD {
    ExternalImageExportInfoOpaqueFD();
};

struct DAWN_NATIVE_EXPORT ExternalImageExportInfoDmaBuf : ExternalImageExportInfoFD {
    ExternalImageExportInfoDmaBuf();
};

#ifdef __ANDROID__

// Descriptor for AHardwareBuffer image import
struct DAWN_NATIVE_EXPORT ExternalImageDescriptorAHardwareBuffer : ExternalImageDescriptorVk {
  public:
    ExternalImageDescriptorAHardwareBuffer();

    struct AHardwareBuffer* handle;  // The AHardwareBuffer which contains the memory of the image
    std::vector<int> waitFDs;        // File descriptors of semaphores which will be waited on

  protected:
    using ExternalImageDescriptorVk::ExternalImageDescriptorVk;
};

struct DAWN_NATIVE_EXPORT ExternalImageExportInfoAHardwareBuffer : ExternalImageExportInfoFD {
    ExternalImageExportInfoAHardwareBuffer();
};

#endif  // __ANDROID__

#endif  // __linux__

// Imports external memory into a Vulkan image. Internally, this uses external memory /
// semaphore extensions to import the image and wait on the provided synchronizaton
// primitives before the texture can be used.
// On failure, returns a nullptr.
DAWN_NATIVE_EXPORT WGPUTexture WrapVulkanImage(WGPUDevice device,
                                               const ExternalImageDescriptorVk* descriptor);

// Exports external memory from a Vulkan image. This must be called on wrapped textures
// before they are destroyed. It writes the semaphore to wait on and the old/new image
// layouts to |info|. Pass VK_IMAGE_LAYOUT_UNDEFINED as |desiredLayout| if you don't want to
// perform a layout transition.
DAWN_NATIVE_EXPORT bool ExportVulkanImage(WGPUTexture texture,
                                          VkImageLayout desiredLayout,
                                          ExternalImageExportInfoVk* info);
// |ExportVulkanImage| with default desiredLayout of VK_IMAGE_LAYOUT_UNDEFINED.
DAWN_NATIVE_EXPORT bool ExportVulkanImage(WGPUTexture texture, ExternalImageExportInfoVk* info);

}  // namespace dawn::native::vulkan

#endif  // INCLUDE_DAWN_NATIVE_VULKANBACKEND_H_
