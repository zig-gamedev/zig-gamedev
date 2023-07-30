// Copyright 2022 The Dawn Authors
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

#ifndef INCLUDE_WEBGPU_WEBGPU_GLFW_H_
#define INCLUDE_WEBGPU_WEBGPU_GLFW_H_

#include <memory>

#include "webgpu/webgpu_cpp.h"

#if defined(WGPU_GLFW_SHARED_LIBRARY)
#if defined(_WIN32)
#if defined(WGPU_GLFW_IMPLEMENTATION)
#define WGPU_GLFW_EXPORT __declspec(dllexport)
#else
#define WGPU_GLFW_EXPORT __declspec(dllimport)
#endif
#else  // defined(_WIN32)
#if defined(WGPU_GLFW_IMPLEMENTATION)
#define WGPU_GLFW_EXPORT __attribute__((visibility("default")))
#else
#define WGPU_GLFW_EXPORT
#endif
#endif  // defined(_WIN32)
#else   // defined(WGPU_GLFW_SHARED_LIBRARY)
#define WGPU_GLFW_EXPORT
#endif  // defined(WGPU_GLFW_SHARED_LIBRARY)

struct GLFWwindow;

namespace wgpu::glfw {

// Does the necessary setup on the GLFWwindow to allow creating a wgpu::Surface with it and
// calls `instance.CreateSurface` with the correct descriptor for this window.
// Returns a null wgpu::Surface on failure.
WGPU_GLFW_EXPORT wgpu::Surface CreateSurfaceForWindow(const wgpu::Instance& instance,
                                                      GLFWwindow* window);

// Use for testing only. Does everything that CreateSurfaceForWindow does except the call to
// CreateSurface. Useful to be able to modify the descriptor for testing, or when trying to
// avoid using the global proc table.
WGPU_GLFW_EXPORT std::unique_ptr<wgpu::ChainedStruct> SetupWindowAndGetSurfaceDescriptor(
    GLFWwindow* window);

}  // namespace wgpu::glfw

#endif  // INCLUDE_WEBGPU_WEBGPU_GLFW_H_
