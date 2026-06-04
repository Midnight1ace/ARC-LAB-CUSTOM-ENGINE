#pragma once

#include "engine/simulation/Experiment.h"
#include "engine/simulation/Recorder.h"
#include "engine/simulation/Snapshot.h"

namespace arclab {

class Simulation {
public:
    void load_experiment(const Experiment& experiment)
    {
        experiment_ = experiment;
        world_ = PhysicsWorld(experiment.settings.maxParticles);
        world_.set_boundary_radius(experiment.settings.boundaryRadius);
        world_.emitter() = experiment.emitter;

        for (const UniformField& field : experiment.fields) {
            world_.fields().add_uniform_field(field);
        }

        timeSeconds_ = 0.0;
        accumulatorSeconds_ = 0.0;
        recorder_.reset();
        recorder_.sample(timeSeconds_, world_.stats());
    }

    void reset()
    {
        load_experiment(experiment_);
    }

    void step_fixed()
    {
        world_.step(experiment_.settings.fixedDt);
        timeSeconds_ += experiment_.settings.fixedDt;
        recorder_.sample(timeSeconds_, world_.stats());
    }

    void step_frame(double frameDt)
    {
        accumulatorSeconds_ += frameDt;
        while (accumulatorSeconds_ >= experiment_.settings.fixedDt) {
            step_fixed();
            accumulatorSeconds_ -= experiment_.settings.fixedDt;
        }
    }

    double time_seconds() const
    {
        return timeSeconds_;
    }

    const PhysicsStats& stats() const
    {
        return world_.stats();
    }

    PhysicsWorld& world()
    {
        return world_;
    }

    const PhysicsWorld& world() const
    {
        return world_;
    }

    SensorRecorder& recorder()
    {
        return recorder_;
    }

    const SensorRecorder& recorder() const
    {
        return recorder_;
    }

    SimulationSnapshot snapshot() const
    {
        return make_snapshot(timeSeconds_, world_.particles());
    }

    void restore_snapshot(const SimulationSnapshot& snapshot)
    {
        world_.particles().clear();
        for (const ParticleSnapshot& saved : snapshot.particles) {
            Particle particle;
            particle.position = saved.position;
            particle.velocity = saved.velocity;
            particle.mass = saved.mass;
            particle.charge = saved.charge;
            particle.lifetime = saved.lifetime;
            particle.age = saved.age;
            world_.particles().spawn(particle);
        }
        timeSeconds_ = snapshot.timeSeconds;
    }

private:
    Experiment experiment_ = make_magnetic_particle_confinement_experiment();
    PhysicsWorld world_;
    SensorRecorder recorder_;
    double timeSeconds_ = 0.0;
    double accumulatorSeconds_ = 0.0;
};

} // namespace arclab
