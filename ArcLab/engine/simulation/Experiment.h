#pragma once

#include "engine/physics/Field.h"
#include "engine/physics/Physics.h"

#include <cstdint>
#include <string>
#include <vector>

namespace arclab {

enum class AccuracyMode {
    Preview,
    Accurate,
};

struct SimulationSettings {
    double fixedDt = 1.0 / 120.0;
    int solverIterations = 1;
    AccuracyMode accuracyMode = AccuracyMode::Preview;
    std::size_t maxParticles = 100000;
    double boundaryRadius = 8.0;
};

struct Experiment {
    std::string name;
    std::string description;
    std::string engineVersion = "0.1.0";
    std::string unitSystem = "demo-si";
    std::uint32_t randomSeed = 12345;
    SimulationSettings settings;
    ParticleEmitter emitter;
    std::vector<UniformField> fields;
};

inline Experiment make_magnetic_particle_confinement_experiment()
{
    Experiment experiment;
    experiment.name = "Magnetic Particle Confinement Experiment";
    experiment.description = "First ArcLab MVP seed: charged particles moving through simplified electric and magnetic fields.";
    experiment.settings.fixedDt = 1.0 / 120.0;
    experiment.settings.maxParticles = 100000;
    experiment.settings.boundaryRadius = 8.0;

    experiment.emitter.position = { -2.5, 0.0, 0.0 };
    experiment.emitter.direction = { 1.0, 0.12, 0.18 };
    experiment.emitter.ratePerSecond = 320.0;
    experiment.emitter.speed = 7.5;
    experiment.emitter.particleMass = 1.0;
    experiment.emitter.particleCharge = 1.0;
    experiment.emitter.particleLifetime = 5.0;

    UniformField magneticField;
    magneticField.type = FieldType::Magnetic;
    magneticField.direction = { 0.0, 1.0, 0.0 };
    magneticField.strength = 2.5;
    experiment.fields.push_back(magneticField);

    UniformField electricField;
    electricField.type = FieldType::Electric;
    electricField.direction = { 0.08, 0.0, 0.0 };
    electricField.strength = 0.35;
    experiment.fields.push_back(electricField);

    return experiment;
}

} // namespace arclab
