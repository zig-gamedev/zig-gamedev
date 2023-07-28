// Copyright 2023 The Dawn Authors
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

#ifndef INCLUDE_DAWN_NATIVE_D3DBACKEND_H_
#define INCLUDE_DAWN_NATIVE_D3DBACKEND_H_

#include <dxgi1_4.h>
#include <windows.h>
#include <wrl/client.h>

#include <memory>
#include <vector>

#include "dawn/native/DawnNative.h"
#include "dawn/webgpu_cpp_chained_struct.h"

namespace dawn::native::d3d {

class ExternalImageDXGIImpl;

DAWN_NATIVE_EXPORT Microsoft::WRL::ComPtr<IDXGIAdapter> GetDXGIAdapter(WGPUAdapter adapter);

// Can be chained in WGPURequestAdapterOptions
struct DAWN_NATIVE_EXPORT RequestAdapterOptionsLUID : wgpu::ChainedStruct {
    RequestAdapterOptionsLUID();

    ::LUID adapterLUID;
};

struct DAWN_NATIVE_EXPORT PhysicalDeviceDiscoveryOptions
    : public PhysicalDeviceDiscoveryOptionsBase {
    PhysicalDeviceDiscoveryOptions(WGPUBackendType type,
                                   Microsoft::WRL::ComPtr<IDXGIAdapter> adapter);
    Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
};

// TODO(dawn:1774): Deprecated.
using AdapterDiscoveryOptions = PhysicalDeviceDiscoveryOptions;

struct DAWN_NATIVE_EXPORT ExternalImageDescriptorDXGISharedHandle : ExternalImageDescriptor {
  public:
    ExternalImageDescriptorDXGISharedHandle();

    // Note: SharedHandle must be a handle to a texture object.
    HANDLE sharedHandle = nullptr;
};

struct DAWN_NATIVE_EXPORT ExternalImageDXGIFenceDescriptor {
    // Shared handle for the fence. This never passes ownership to the callee (when used as an input
    // parameter) or to the caller (when used as a return value or output parameter).
    HANDLE fenceHandle = nullptr;

    // The value that was previously signaled on this fence and should be waited on.
    uint64_t fenceValue = 0;
};

struct DAWN_NATIVE_EXPORT ExternalImageDXGIBeginAccessDescriptor {
    bool isInitialized = false;  // Whether the texture is initialized on import
    WGPUTextureUsageFlags usage = WGPUTextureUsage_None;

    // A list of fences to wait on before accessing the texture.
    std::vector<ExternalImageDXGIFenceDescriptor> waitFences;

    // Whether the texture is for a WebGPU swap chain.
    bool isSwapChainTexture = false;
};

class DAWN_NATIVE_EXPORT ExternalImageDXGI {
  public:
    ~ExternalImageDXGI();

    static std::unique_ptr<ExternalImageDXGI> Create(
        WGPUDevice device,
        const ExternalImageDescriptorDXGISharedHandle* descriptor);

    // Returns true if the external image resources are still valid, otherwise BeginAccess() is
    // guaranteed to fail e.g. after device destruction.
    bool IsValid() const;

    // Creates WGPUTexture wrapping the DXGI shared handle. The provided wait fences will be
    // synchronized before using the texture in any command lists. Empty fences (nullptr handle) are
    // ignored for convenience (EndAccess can return such fences).
    WGPUTexture BeginAccess(const ExternalImageDXGIBeginAccessDescriptor* descriptor);

    // Returns the signalFence that the client must wait on for correct synchronization. Can return
    // an empty fence (nullptr handle) if the texture wasn't accessed by Dawn.
    // Note that merely calling Destroy() on the WGPUTexture does not ensure synchronization.
    void EndAccess(WGPUTexture texture, ExternalImageDXGIFenceDescriptor* signalFence);

  private:
    explicit ExternalImageDXGI(std::unique_ptr<ExternalImageDXGIImpl> impl);

    std::unique_ptr<ExternalImageDXGIImpl> mImpl;
};

}  // namespace dawn::native::d3d

#endif  // INCLUDE_DAWN_NATIVE_D3DBACKEND_H_
