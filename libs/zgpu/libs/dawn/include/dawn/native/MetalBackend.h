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

#ifndef INCLUDE_DAWN_NATIVE_METALBACKEND_H_
#define INCLUDE_DAWN_NATIVE_METALBACKEND_H_

#include <vector>

#include "dawn/native/DawnNative.h"

// The specifics of the Metal backend expose types in function signatures that might not be
// available in dependent's minimum supported SDK version. Suppress all availability errors using
// clang's pragmas. Dependents using the types without guarded availability will still get errors
// when using the types.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunguarded-availability"

struct __IOSurface;
typedef __IOSurface* IOSurfaceRef;

#ifdef __OBJC__
#import <Metal/Metal.h>
#endif  // __OBJC__

namespace dawn::native::metal {

struct DAWN_NATIVE_EXPORT PhysicalDeviceDiscoveryOptions
    : public PhysicalDeviceDiscoveryOptionsBase {
    PhysicalDeviceDiscoveryOptions();
};

// TODO(dawn:1774): Deprecated.
using AdapterDiscoveryOptions = PhysicalDeviceDiscoveryOptions;

struct DAWN_NATIVE_EXPORT ExternalImageMTLSharedEventDescriptor {
    // Shared event handle `id<MTLSharedEvent>`.
    // This never passes ownership to the callee (when used as an input
    // parameter) or to the caller (when used as a return value or output parameter).
#ifdef __OBJC__
    id<MTLSharedEvent> sharedEvent = nil;
    static_assert(sizeof(id<MTLSharedEvent>) == sizeof(void*));
    static_assert(alignof(id<MTLSharedEvent>) == alignof(void*));
#else
    void* sharedEvent = nullptr;
#endif

    // The value that was previously signaled on this event and should be waited on.
    uint64_t signaledValue = 0;
};

struct DAWN_NATIVE_EXPORT ExternalImageDescriptorIOSurface : ExternalImageDescriptor {
  public:
    ExternalImageDescriptorIOSurface();
    ~ExternalImageDescriptorIOSurface();

    IOSurfaceRef ioSurface;

    // A list of events to wait on before accessing the texture.
    std::vector<ExternalImageMTLSharedEventDescriptor> waitEvents;
};

struct DAWN_NATIVE_EXPORT ExternalImageIOSurfaceEndAccessDescriptor
    : ExternalImageMTLSharedEventDescriptor {
    bool isInitialized;
};

DAWN_NATIVE_EXPORT WGPUTexture WrapIOSurface(WGPUDevice device,
                                             const ExternalImageDescriptorIOSurface* descriptor);

DAWN_NATIVE_EXPORT void IOSurfaceEndAccess(WGPUTexture texture,
                                           ExternalImageIOSurfaceEndAccessDescriptor* descriptor);

// When making Metal interop with other APIs, we need to be careful that QueueSubmit doesn't
// mean that the operations will be visible to other APIs/Metal devices right away. macOS
// does have a global queue of graphics operations, but the command buffers are inserted there
// when they are "scheduled". Submitting other operations before the command buffer is
// scheduled could lead to races in who gets scheduled first and incorrect rendering.
DAWN_NATIVE_EXPORT void WaitForCommandsToBeScheduled(WGPUDevice device);

}  // namespace dawn::native::metal

#pragma clang diagnostic pop

#endif  // INCLUDE_DAWN_NATIVE_METALBACKEND_H_
