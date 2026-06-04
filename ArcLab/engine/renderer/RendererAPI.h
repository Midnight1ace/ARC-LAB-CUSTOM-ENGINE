#pragma once

namespace arclab {

enum class RendererBackend {
    None,
    OpenGL,
    Vulkan,
};

struct RendererCapabilities {
    RendererBackend backend = RendererBackend::None;
    bool supportsInstancing = false;
    bool supportsCompute = false;
    int maxTextureSize = 0;
};

} // namespace arclab
