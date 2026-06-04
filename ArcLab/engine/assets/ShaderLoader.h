#pragma once

#include "engine/platform/Files.h"

#include <filesystem>
#include <optional>
#include <string>

namespace arclab {

struct ShaderSource {
    std::string path;
    std::string text;
};

inline std::optional<ShaderSource> load_shader_source(const std::filesystem::path& path)
{
    std::optional<std::string> text = read_text_file(path);
    if (!text) {
        return std::nullopt;
    }
    return ShaderSource { path.string(), *text };
}

} // namespace arclab
