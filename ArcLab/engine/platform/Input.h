#pragma once

#include "engine/platform/Keyboard.h"
#include "engine/platform/Mouse.h"

namespace arclab {

struct InputState {
    KeyboardState keyboard;
    MouseState mouse;

    void begin_frame()
    {
        mouse.deltaX = 0.0;
        mouse.deltaY = 0.0;
        mouse.wheelDelta = 0.0;
    }
};

} // namespace arclab
