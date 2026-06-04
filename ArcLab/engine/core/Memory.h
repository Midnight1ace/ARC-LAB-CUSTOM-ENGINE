#pragma once

#include <cstddef>

namespace arclab {

struct MemoryStats {
    std::size_t allocatedBytes = 0;
    std::size_t peakBytes = 0;

    void record_allocation(std::size_t bytes)
    {
        allocatedBytes += bytes;
        if (allocatedBytes > peakBytes) {
            peakBytes = allocatedBytes;
        }
    }

    void record_free(std::size_t bytes)
    {
        allocatedBytes = bytes > allocatedBytes ? 0 : allocatedBytes - bytes;
    }
};

} // namespace arclab
