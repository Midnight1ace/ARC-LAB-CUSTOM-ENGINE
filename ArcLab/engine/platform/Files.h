#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

namespace arclab {

inline std::optional<std::string> read_text_file(const std::filesystem::path& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        return std::nullopt;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

inline bool write_text_file(const std::filesystem::path& path, const std::string& text)
{
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }

    file << text;
    return true;
}

} // namespace arclab
