#pragma once

#include "engine/math/Vector.h"

#include <cstdint>

namespace arclab {

class Random {
public:
    explicit Random(std::uint64_t seed = 0xC0FFEEu)
        : state_(seed == 0 ? 0xC0FFEEu : seed)
    {
    }

    std::uint64_t next_u64()
    {
        std::uint64_t x = state_;
        x ^= x << 13u;
        x ^= x >> 7u;
        x ^= x << 17u;
        state_ = x;
        return x;
    }

    double next_double()
    {
        constexpr double scale = 1.0 / static_cast<double>(UINT64_MAX);
        return static_cast<double>(next_u64()) * scale;
    }

    double range(double minValue, double maxValue)
    {
        return minValue + (maxValue - minValue) * next_double();
    }

    Vec3 inside_unit_sphere()
    {
        for (;;) {
            Vec3 candidate {
                range(-1.0, 1.0),
                range(-1.0, 1.0),
                range(-1.0, 1.0),
            };
            if (candidate.length_squared() <= 1.0) {
                return candidate;
            }
        }
    }

private:
    std::uint64_t state_ = 0xC0FFEEu;
};

} // namespace arclab
