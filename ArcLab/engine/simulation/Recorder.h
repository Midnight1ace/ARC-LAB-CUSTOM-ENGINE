#pragma once

#include "engine/physics/Physics.h"

#include <filesystem>
#include <fstream>
#include <vector>

namespace arclab {

struct SensorSample {
    double timeSeconds = 0.0;
    std::size_t activeParticles = 0;
    std::size_t lostParticles = 0;
    double averageVelocity = 0.0;
    double totalKineticEnergy = 0.0;
    double averageDistanceFromCenter = 0.0;
};

class SensorRecorder {
public:
    explicit SensorRecorder(double sampleIntervalSeconds = 1.0 / 30.0)
        : sampleIntervalSeconds_(sampleIntervalSeconds)
    {
    }

    void reset()
    {
        samples_.clear();
        nextSampleTime_ = 0.0;
    }

    void sample(double timeSeconds, const PhysicsStats& stats)
    {
        if (timeSeconds + 1.0e-9 < nextSampleTime_) {
            return;
        }

        samples_.push_back({
            timeSeconds,
            stats.activeParticles,
            stats.lostParticles,
            stats.averageVelocity,
            stats.totalKineticEnergy,
            stats.averageDistanceFromCenter,
        });

        nextSampleTime_ = timeSeconds + sampleIntervalSeconds_;
    }

    bool export_csv(const std::filesystem::path& path) const
    {
        std::ofstream file(path);
        if (!file.is_open()) {
            return false;
        }

        file << "time_seconds,active_particles,lost_particles,average_velocity,total_kinetic_energy,average_distance_from_center\n";
        for (const SensorSample& sample : samples_) {
            file << sample.timeSeconds << ','
                 << sample.activeParticles << ','
                 << sample.lostParticles << ','
                 << sample.averageVelocity << ','
                 << sample.totalKineticEnergy << ','
                 << sample.averageDistanceFromCenter << '\n';
        }

        return true;
    }

    const std::vector<SensorSample>& samples() const
    {
        return samples_;
    }

private:
    double sampleIntervalSeconds_ = 1.0 / 30.0;
    double nextSampleTime_ = 0.0;
    std::vector<SensorSample> samples_;
};

} // namespace arclab
