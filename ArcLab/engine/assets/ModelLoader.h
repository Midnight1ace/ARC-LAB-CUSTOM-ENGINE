#pragma once

#include "engine/math/Vector.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace arclab {

struct LoadedModel {
    std::vector<Vec3> positions;
};

inline LoadedModel load_obj_positions(const std::filesystem::path& path)
{
    LoadedModel model;
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::string prefix;
        stream >> prefix;
        if (prefix == "v") {
            Vec3 position;
            stream >> position.x >> position.y >> position.z;
            model.positions.push_back(position);
        }
    }

    return model;
}

} // namespace arclab
