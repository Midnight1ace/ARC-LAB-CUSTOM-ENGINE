#pragma once

#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

namespace arclab {

inline bool write_binary_file(const std::filesystem::path& path, const std::vector<unsigned char>& bytes)
{
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    file.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    return true;
}

inline std::vector<unsigned char> read_binary_file(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return {};
    }

    return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
}

} // namespace arclab
