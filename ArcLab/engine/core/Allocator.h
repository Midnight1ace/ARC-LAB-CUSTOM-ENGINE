#pragma once

#include <cstddef>
#include <vector>

namespace arclab {

class LinearAllocator {
public:
    explicit LinearAllocator(std::size_t capacityBytes)
        : buffer_(capacityBytes)
    {
    }

    void* allocate(std::size_t bytes, std::size_t alignment = alignof(std::max_align_t))
    {
        std::size_t aligned = offset_;
        const std::size_t misalignment = aligned % alignment;
        if (misalignment != 0) {
            aligned += alignment - misalignment;
        }
        if (aligned + bytes > buffer_.size()) {
            return nullptr;
        }

        offset_ = aligned + bytes;
        return buffer_.data() + aligned;
    }

    void reset()
    {
        offset_ = 0;
    }

    std::size_t used() const
    {
        return offset_;
    }

private:
    std::vector<std::byte> buffer_;
    std::size_t offset_ = 0;
};

} // namespace arclab
