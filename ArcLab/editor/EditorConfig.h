#pragma once

#include <string>

namespace arclab {

struct EditorConfig {
    std::string title = "ArcLab Editor - Magnetic Particle Confinement";
    int width = 1280;
    int height = 720;
    double targetFrameSeconds = 1.0 / 60.0;
    int maxFrames = 0;
};

} // namespace arclab
