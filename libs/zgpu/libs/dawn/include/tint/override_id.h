// Copyright 2022 The Tint Authors.
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

#ifndef SRC_TINT_OVERRIDE_ID_H_
#define SRC_TINT_OVERRIDE_ID_H_

#include <stdint.h>
#include <functional>

#include "src/tint/reflection.h"

namespace tint {

/// OverrideId is a numerical identifier for an override variable, unique per program.
struct OverrideId {
    uint16_t value = 0;

    /// Reflect the fields of this struct so that it can be used by tint::ForeachField()
    TINT_REFLECT(value);
};

/// Equality operator for OverrideId
/// @param lhs the OverrideId on the left of the '=' operator
/// @param rhs the OverrideId on the right of the '=' operator
/// @returns true if `lhs` is equal to `rhs`
inline bool operator==(OverrideId lhs, OverrideId rhs) {
    return lhs.value == rhs.value;
}

/// Less-than operator for OverrideId
/// @param lhs the OverrideId on the left of the '<' operator
/// @param rhs the OverrideId on the right of the '<' operator
/// @returns true if `lhs` comes before `rhs`
inline bool operator<(OverrideId lhs, OverrideId rhs) {
    return lhs.value < rhs.value;
}

}  // namespace tint

namespace std {

/// Custom std::hash specialization for tint::OverrideId.
template <>
class hash<tint::OverrideId> {
  public:
    /// @param id the override identifier
    /// @return the hash of the override identifier
    inline std::size_t operator()(tint::OverrideId id) const {
        return std::hash<decltype(tint::OverrideId::value)>()(id.value);
    }
};

}  // namespace std

#endif  // SRC_TINT_OVERRIDE_ID_H_
