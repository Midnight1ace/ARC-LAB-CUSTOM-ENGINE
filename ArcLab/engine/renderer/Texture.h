#pragma once

#include "engine/math/Color.h"

#include <vector>

namespace arclab {

struct Texture2D {
    int width = 1;
    int height = 1;
    std::vector<Color> pixels { Color::white() };
};

} // namespace arclab
