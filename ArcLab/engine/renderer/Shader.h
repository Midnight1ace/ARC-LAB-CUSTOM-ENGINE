#pragma once

#include <string>

namespace arclab {

enum class ShaderStage {
    Vertex,
    Fragment,
    Compute,
};

struct Shader {
    ShaderStage stage = ShaderStage::Vertex;
    std::string name;
    std::string source;
};

} // namespace arclab
