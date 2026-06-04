#pragma once

#include <filesystem>
#include <string>

namespace arclab {

class Serializer {
public:
    virtual ~Serializer() = default;
    virtual bool save_text(const std::filesystem::path& path, const std::string& text) = 0;
    virtual bool load_text(const std::filesystem::path& path, std::string& outText) = 0;
};

} // namespace arclab
