#pragma once

#include "engine/math/Color.h"

#include <algorithm>
#include <vector>

namespace arclab {

class Framebuffer {
public:
    Framebuffer(int width = 1280, int height = 720)
        : width_(width)
        , height_(height)
        , pixels_(static_cast<std::size_t>(width * height), Color::black())
    {
    }

    void clear(const Color& color)
    {
        std::fill(pixels_.begin(), pixels_.end(), color);
    }

    void set_pixel(int x, int y, const Color& color)
    {
        if (x < 0 || y < 0 || x >= width_ || y >= height_) {
            return;
        }
        pixels_[static_cast<std::size_t>(y * width_ + x)] = color;
    }

    int width() const { return width_; }
    int height() const { return height_; }
    const std::vector<Color>& pixels() const { return pixels_; }

private:
    int width_ = 0;
    int height_ = 0;
    std::vector<Color> pixels_;
};

} // namespace arclab
