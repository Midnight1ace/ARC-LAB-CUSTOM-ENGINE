#pragma once

namespace arclab {

enum class MouseButton {
    Left = 0,
    Right,
    Middle,
    Count,
};

struct MouseState {
    double x = 0.0;
    double y = 0.0;
    double deltaX = 0.0;
    double deltaY = 0.0;
    double wheelDelta = 0.0;
    bool leftDown = false;
    bool rightDown = false;
    bool middleDown = false;
};

} // namespace arclab
