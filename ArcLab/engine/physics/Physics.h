#pragma once

#include "engine/math/Math.h"
#include "engine/physics/Field.h"
#include "engine/physics/Integrator.h"
#include "engine/physics/Particle.h"

#include <cstddef>

namespace arclab {

struct ParticleEmitter {
    Vec3 position { -2.0, 0.0, 0.0 };
    Vec3 direction { 1.0, 0.15, 0.0 };
    double ratePerSecond = 240.0;
    double speed = 8.0;
    double particleMass = 1.0;
    double particleCharge = 1.0;
    double particleLifetime = 6.0;
    double accumulator = 0.0;
    bool enabled = true;

    void emit(ParticleSystem& particles, double dt)
    {
        if (!enabled || ratePerSecond <= 0.0) {
            return;
        }

        accumulator += ratePerSecond * dt;
        const Vec3 initialVelocity = normalized(direction) * speed;

        while (accumulator >= 1.0) {
            Particle particle;
            particle.position = position;
            particle.velocity = initialVelocity;
            particle.mass = particleMass;
            particle.charge = particleCharge;
            particle.lifetime = particleLifetime;

            if (!particles.spawn(particle)) {
                break;
            }

            accumulator -= 1.0;
        }
    }

    void reset()
    {
        accumulator = 0.0;
    }
};

struct PhysicsStats {
    std::size_t activeParticles = 0;
    std::size_t lostParticles = 0;
    double averageVelocity = 0.0;
    double totalKineticEnergy = 0.0;
    double averageDistanceFromCenter = 0.0;
};

class PhysicsWorld {
public:
    explicit PhysicsWorld(std::size_t maxParticles = 100000)
        : particles_(maxParticles)
    {
    }

    void reset()
    {
        particles_.clear();
        fields_.clear();
        emitter_.reset();
        stats_ = {};
    }

    void step(double dt)
    {
        emitter_.emit(particles_, dt);

        std::size_t lostThisStep = 0;
        for (Particle& particle : particles_.particles()) {
            const FieldSample sample = fields_.sample_at(particle.position);

            const Vec3 lorentzForce = particle.charge * (sample.electric + cross(particle.velocity, sample.magnetic));
            const Vec3 gravityForce = particle.mass * sample.gravityAcceleration;
            particle.force += lorentzForce + gravityForce;

            SemiImplicitEuler::integrate(particle, dt);

            if (particle.position.length() > boundaryRadius_) {
                particle.flags |= ParticleLost;
                ++lostThisStep;
            }
        }

        particles_.remove_inactive();
        stats_.lostParticles += lostThisStep;
        refresh_stats();
    }

    ParticleSystem& particles()
    {
        return particles_;
    }

    const ParticleSystem& particles() const
    {
        return particles_;
    }

    FieldSystem& fields()
    {
        return fields_;
    }

    const FieldSystem& fields() const
    {
        return fields_;
    }

    ParticleEmitter& emitter()
    {
        return emitter_;
    }

    const ParticleEmitter& emitter() const
    {
        return emitter_;
    }

    void set_boundary_radius(double radius)
    {
        boundaryRadius_ = radius;
    }

    double boundary_radius() const
    {
        return boundaryRadius_;
    }

    const PhysicsStats& stats() const
    {
        return stats_;
    }

private:
    void refresh_stats()
    {
        const std::size_t lostParticles = stats_.lostParticles;
        stats_ = {};
        stats_.lostParticles = lostParticles;
        stats_.activeParticles = particles_.particles().size();

        if (stats_.activeParticles == 0) {
            return;
        }

        double velocitySum = 0.0;
        double distanceSum = 0.0;
        for (const Particle& particle : particles_.particles()) {
            velocitySum += particle.velocity.length();
            distanceSum += particle.position.length();
            stats_.totalKineticEnergy += particle.kinetic_energy();
        }

        stats_.averageVelocity = velocitySum / static_cast<double>(stats_.activeParticles);
        stats_.averageDistanceFromCenter = distanceSum / static_cast<double>(stats_.activeParticles);
    }

    ParticleSystem particles_;
    FieldSystem fields_;
    ParticleEmitter emitter_;
    PhysicsStats stats_;
    double boundaryRadius_ = 8.0;
};

} // namespace arclab
