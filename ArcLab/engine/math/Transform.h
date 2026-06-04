#pragma once

#include "engine/math/Vector.h"

namespace arclab {

struct Transform {
    Vec3 position { 0.0, 0.0, 0.0 };
    Vec3 rotationEulerRadians { 0.0, 0.0, 0.0 };
    Vec3 scale { 1.0, 1.0, 1.0 };
};

} // namespace arclab
