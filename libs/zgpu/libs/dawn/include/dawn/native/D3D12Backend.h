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

#ifndef INCLUDE_DAWN_NATIVE_D3D12BACKEND_H_
#define INCLUDE_DAWN_NATIVE_D3D12BACKEND_H_

#include <DXGI1_4.h>
#include <d3d12.h>
#include <windows.h>
#include <wrl/client.h>

#include "dawn/native/D3DBackend.h"

struct ID3D12Device;
struct ID3D12Resource;

namespace dawn::native::d3d12 {

class Device;

enum MemorySegment {
    Local,
    NonLocal,
};

DAWN_NATIVE_EXPORT uint64_t SetExternalMemoryReservation(WGPUDevice device,
                                                         uint64_t requestedReservationSize,
                                                         MemorySegment memorySegment);

struct DAWN_NATIVE_EXPORT PhysicalDeviceDiscoveryOptions
    : public d3d::PhysicalDeviceDiscoveryOptions {
    PhysicalDeviceDiscoveryOptions();
    explicit PhysicalDeviceDiscoveryOptions(Microsoft::WRL::ComPtr<IDXGIAdapter> adapter);
};

// TODO(dawn:1774): Deprecated.
using AdapterDiscoveryOptions = PhysicalDeviceDiscoveryOptions;

}  // namespace dawn::native::d3d12

#endif  // INCLUDE_DAWN_NATIVE_D3D12BACKEND_H_
