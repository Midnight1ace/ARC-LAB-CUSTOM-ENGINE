#pragma once

#include "engine/physics/Particle.h"

#include <vector>

namespace arclab {

struct ParticleSnapshot {
    Vec3 position;
    Vec3 velocity;
    double mass = 1.0;
    double charge = 1.0;
    double lifetime = 0.0;
    double age = 0.0;
};

struct SimulationSnapshot {
    double timeSeconds = 0.0;
    std::vector<ParticleSnapshot> particles;
};

inline SimulationSnapshot make_snapshot(double timeSeconds, const ParticleSystem& particles)
{
    SimulationSnapshot snapshot;
    snapshot.timeSeconds = timeSeconds;
    snapshot.particles.reserve(particles.particles().size());

    for (const Particle& particle : particles.particles()) {
        snapshot.particles.push_back({
            particle.position,
            particle.velocity,
            particle.mass,
            particle.charge,
            particle.lifetime,
            particle.age,
        });
    }

    return snapshot;
}

} // namespace arclab
