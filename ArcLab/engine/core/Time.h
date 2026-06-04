#pragma once

#include <chrono>

namespace arclab {

class Stopwatch {
public:
    using clock = std::chrono::steady_clock;

    Stopwatch()
        : startedAt_(clock::now())
    {
    }

    void reset()
    {
        startedAt_ = clock::now();
    }

    double elapsed_seconds() const
    {
        const auto elapsed = clock::now() - startedAt_;
        return std::chrono::duration<double>(elapsed).count();
    }

private:
    clock::time_point startedAt_;
};

} // namespace arclab
