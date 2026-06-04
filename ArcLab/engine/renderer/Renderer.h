#pragma once

#include "engine/renderer/Camera.h"
#include "engine/renderer/DebugDraw.h"
#include "engine/renderer/RendererAPI.h"

#include <cstddef>

namespace arclab {

struct RenderStats {
    std::size_t debugLineCount = 0;
    std::size_t particleCount = 0;
};

class Renderer {
public:
    void begin_frame(const PerspectiveCamera& camera)
    {
        camera_ = camera;
        stats_ = {};
    }

    void submit_debug_lines(const DebugDrawList& drawList)
    {
        stats_.debugLineCount += drawList.lines().size();
    }

    void submit_particles(std::size_t particleCount)
    {
        stats_.particleCount += particleCount;
    }

    void end_frame()
    {
    }

    const RenderStats& stats() const
    {
        return stats_;
    }

    RendererCapabilities capabilities() const
    {
        return capabilities_;
    }

private:
    PerspectiveCamera camera_;
    RendererCapabilities capabilities_;
    RenderStats stats_;
};

} // namespace arclab
