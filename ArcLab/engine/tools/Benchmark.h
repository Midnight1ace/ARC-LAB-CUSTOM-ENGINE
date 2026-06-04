#pragma once

#include "engine/core/Time.h"
#include "engine/simulation/Experiment.h"
#include "engine/simulation/Simulation.h"

#include <string>
#include <vector>

namespace arclab {

struct BatchRunResult {
    std::string label;
    double fieldStrength = 0.0;
    double durationSeconds = 0.0;
    PhysicsStats stats;
    double wallSeconds = 0.0;
};

inline BatchRunResult run_field_strength_batch(const Experiment& baseExperiment, double fieldStrength, double durationSeconds)
{
    Experiment experiment = baseExperiment;
    for (UniformField& field : experiment.fields) {
        if (field.type == FieldType::Magnetic) {
            field.strength = fieldStrength;
        }
    }

    Simulation simulation;
    simulation.load_experiment(experiment);

    Stopwatch stopwatch;
    const int steps = static_cast<int>(durationSeconds / experiment.settings.fixedDt);
    for (int i = 0; i < steps; ++i) {
        simulation.step_fixed();
    }

    return {
        "B=" + std::to_string(fieldStrength),
        fieldStrength,
        durationSeconds,
        simulation.stats(),
        stopwatch.elapsed_seconds(),
    };
}

inline std::vector<BatchRunResult> sweep_magnetic_field(
    const Experiment& experiment,
    const std::vector<double>& fieldStrengths,
    double durationSeconds)
{
    std::vector<BatchRunResult> results;
    results.reserve(fieldStrengths.size());
    for (double strength : fieldStrengths) {
        results.push_back(run_field_strength_batch(experiment, strength, durationSeconds));
    }
    return results;
}

} // namespace arclab
