#pragma once

#include "engine/physics/Physics.h"

#include <string>

namespace arclab {

enum class SensorQuantity {
    ActiveParticles,
    LostParticles,
    AverageVelocity,
    TotalKineticEnergy,
    AverageDistanceFromCenter,
};

struct SensorDefinition {
    std::string name = "Average Velocity";
    SensorQuantity quantity = SensorQuantity::AverageVelocity;
    double sampleRateHz = 30.0;
};

inline double read_sensor_value(const SensorDefinition& sensor, const PhysicsStats& stats)
{
    switch (sensor.quantity) {
    case SensorQuantity::ActiveParticles:
        return static_cast<double>(stats.activeParticles);
    case SensorQuantity::LostParticles:
        return static_cast<double>(stats.lostParticles);
    case SensorQuantity::AverageVelocity:
        return stats.averageVelocity;
    case SensorQuantity::TotalKineticEnergy:
        return stats.totalKineticEnergy;
    case SensorQuantity::AverageDistanceFromCenter:
        return stats.averageDistanceFromCenter;
    }

    return 0.0;
}

} // namespace arclab
