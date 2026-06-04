#pragma once

#include "engine/physics/Particle.h"

namespace arclab {

struct DistanceConstraint {
    std::size_t a = 0;
    std::size_t b = 0;
    double restLength = 1.0;
    double stiffness = 1.0;

    void solve(ParticleSystem& particles) const
    {
        auto& data = particles.particles();
        if (a >= data.size() || b >= data.size()) {
            return;
        }

        const Vec3 delta = data[b].position - data[a].position;
        const double length = delta.length();
        if (nearly_zero(length)) {
            return;
        }

        const Vec3 correction = normalized(delta) * ((length - restLength) * 0.5 * stiffness);
        data[a].position += correction;
        data[b].position -= correction;
    }
};

} // namespace arclab
