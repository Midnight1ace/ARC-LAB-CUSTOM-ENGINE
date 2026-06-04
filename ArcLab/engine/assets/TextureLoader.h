#pragma once

#include "engine/math/Color.h"

#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

namespace arclab {

struct Image {
    int width = 0;
    int height = 0;
    std::vector<Color> pixels;
};

inline std::optional<Image> load_ppm_p6(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return std::nullopt;
    }

    std::string magic;
    int maxValue = 0;
    Image image;
    file >> magic >> image.width >> image.height >> maxValue;
    file.get();

    if (magic != "P6" || image.width <= 0 || image.height <= 0 || maxValue != 255) {
        return std::nullopt;
    }

    image.pixels.resize(static_cast<std::size_t>(image.width * image.height));
    for (Color& pixel : image.pixels) {
        unsigned char rgb[3] {};
        file.read(reinterpret_cast<char*>(rgb), 3);
        pixel = { rgb[0] / 255.0f, rgb[1] / 255.0f, rgb[2] / 255.0f, 1.0f };
    }

    return image;
}

} // namespace arclab
