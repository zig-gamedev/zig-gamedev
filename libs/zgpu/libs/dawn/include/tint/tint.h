// Copyright 2020 The Tint Authors.
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

#ifndef INCLUDE_TINT_TINT_H_
#define INCLUDE_TINT_TINT_H_

// Guard for accidental includes to private headers
#define CURRENTLY_IN_TINT_PUBLIC_HEADER

// TODO(tint:88): When implementing support for an install target, all of these
//                headers will need to be moved to include/tint/.

#include "src/tint/demangler.h"
#include "src/tint/diagnostic/printer.h"
#include "src/tint/inspector/inspector.h"
#include "src/tint/reader/reader.h"
#include "src/tint/sem/type_manager.h"
#include "src/tint/transform/binding_remapper.h"
#include "src/tint/transform/first_index_offset.h"
#include "src/tint/transform/manager.h"
#include "src/tint/transform/multiplanar_external_texture.h"
#include "src/tint/transform/renamer.h"
#include "src/tint/transform/robustness.h"
#include "src/tint/transform/single_entry_point.h"
#include "src/tint/transform/substitute_override.h"
#include "src/tint/transform/vertex_pulling.h"
#include "src/tint/writer/flatten_bindings.h"
#include "src/tint/writer/writer.h"

#if TINT_BUILD_SPV_READER
#include "src/tint/reader/spirv/parser.h"
#endif  // TINT_BUILD_SPV_READER

#if TINT_BUILD_WGSL_READER
#include "src/tint/reader/wgsl/parser.h"
#endif  // TINT_BUILD_WGSL_READER

#if TINT_BUILD_SPV_WRITER
#include "spirv-tools/libspirv.hpp"
#include "src/tint/writer/spirv/generator.h"
#endif  // TINT_BUILD_SPV_WRITER

#if TINT_BUILD_WGSL_WRITER
#include "src/tint/writer/wgsl/generator.h"
#endif  // TINT_BUILD_WGSL_WRITER

#if TINT_BUILD_MSL_WRITER
#include "src/tint/writer/msl/generator.h"
#endif  // TINT_BUILD_MSL_WRITER

#if TINT_BUILD_HLSL_WRITER
#include "src/tint/writer/hlsl/generator.h"
#endif  // TINT_BUILD_HLSL_WRITER

#if TINT_BUILD_GLSL_WRITER
#include "src/tint/writer/glsl/generator.h"
#endif  // TINT_BUILD_GLSL_WRITER

namespace tint {

/// Initialize initializes the Tint library. Call before using the Tint API.
void Initialize();

/// Shutdown uninitializes the Tint library. Call after using the Tint API.
void Shutdown();

}  // namespace tint

#undef CURRENTLY_IN_TINT_PUBLIC_HEADER

#endif  // INCLUDE_TINT_TINT_H_
