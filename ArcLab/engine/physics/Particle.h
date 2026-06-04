#pragma once

#include "engine/math/Vector.h"

#include <cstdint>
#include <vector>

namespace arclab {

enum ParticleFlags : std::uint32_t {
    ParticleAlive = 1u << 0u,
    ParticleLost = 1u << 1u,
};

struct Particle {
    Vec3 position;
    Vec3 velocity;
    Vec3 force;
    double mass = 1.0;
    double charge = 1.0;
    double temperature = 293.15;
    double lifetime = 5.0;
    double age = 0.0;
    std::uint32_t flags = ParticleAlive;

    bool alive() const
    {
        return (flags & ParticleAlive) != 0u && age < lifetime;
    }

    double kinetic_energy() const
    {
        return 0.5 * mass * velocity.length_squared();
    }
};

class ParticleSystem {
public:
    explicit ParticleSystem(std::size_t maxParticles = 100000)
        : maxParticles_(maxParticles)
    {
        particles_.reserve(maxParticles_);
    }

    bool spawn(const Particle& particle)
    {
        if (particles_.size() >= maxParticles_) {
            return false;
        }

        particles_.push_back(particle);
        return true;
    }

    void clear()
    {
        particles_.clear();
    }

    void remove_inactive()
    {
        auto write = particles_.begin();
        for (auto read = particles_.begin(); read != particles_.end(); ++read) {
            if (read->alive() && (read->flags & ParticleLost) == 0u) {
                *write = *read;
                ++write;
            }
        }
        particles_.erase(write, particles_.end());
    }

    std::vector<Particle>& particles()
    {
        return particles_;
    }

    const std::vector<Particle>& particles() const
    {
        return particles_;
    }

    std::size_t max_particles() const
    {
        return maxParticles_;
    }

private:
    std::size_t maxParticles_ = 100000;
    std::vector<Particle> particles_;
};

} // namespace arclab
