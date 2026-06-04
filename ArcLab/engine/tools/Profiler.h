#pragma once

#include "engine/core/Time.h"

#include <map>
#include <string>

namespace arclab {

struct ProfileMetric {
    double totalMilliseconds = 0.0;
    double lastMilliseconds = 0.0;
    int samples = 0;
};

class Profiler {
public:
    void record(const std::string& name, double milliseconds)
    {
        ProfileMetric& metric = metrics_[name];
        metric.lastMilliseconds = milliseconds;
        metric.totalMilliseconds += milliseconds;
        ++metric.samples;
    }

    const std::map<std::string, ProfileMetric>& metrics() const
    {
        return metrics_;
    }

private:
    std::map<std::string, ProfileMetric> metrics_;
};

class ScopedProfile {
public:
    ScopedProfile(Profiler& profiler, std::string name)
        : profiler_(profiler)
        , name_(std::move(name))
    {
    }

    ~ScopedProfile()
    {
        profiler_.record(name_, stopwatch_.elapsed_seconds() * 1000.0);
    }

private:
    Profiler& profiler_;
    std::string name_;
    Stopwatch stopwatch_;
};

} // namespace arclab
