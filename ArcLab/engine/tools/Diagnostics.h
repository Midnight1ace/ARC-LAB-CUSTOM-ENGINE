#pragma once

#include <cstddef>

namespace arclab {

struct Diagnostics {
    double frameMilliseconds = 0.0;
    double simulationMilliseconds = 0.0;
    double renderMilliseconds = 0.0;
    std::size_t activeParticles = 0;
    std::size_t debugLines = 0;
    std::size_t estimatedRamBytes = 0;
};

} // namespace arclab
