#pragma once

#include "engine/physics/Particle.h"

namespace arclab {

struct SemiImplicitEuler {
    static void integrate(Particle& particle, double dt)
    {
        if (particle.mass <= 0.0) {
            return;
        }

        const Vec3 acceleration = particle.force / particle.mass;
        particle.velocity += acceleration * dt;
        particle.position += particle.velocity * dt;
        particle.age += dt;
        particle.force = {};
    }
};

} // namespace arclab
