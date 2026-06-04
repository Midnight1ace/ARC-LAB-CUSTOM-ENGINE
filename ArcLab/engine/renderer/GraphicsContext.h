#pragma once

#include "engine/renderer/RendererAPI.h"

namespace arclab {

class GraphicsContext {
public:
    explicit GraphicsContext(RendererBackend backend = RendererBackend::None)
        : backend_(backend)
    {
    }

    bool initialize()
    {
        initialized_ = true;
        return true;
    }

    bool initialized() const
    {
        return initialized_;
    }

    RendererBackend backend() const
    {
        return backend_;
    }

private:
    RendererBackend backend_ = RendererBackend::None;
    bool initialized_ = false;
};

} // namespace arclab
