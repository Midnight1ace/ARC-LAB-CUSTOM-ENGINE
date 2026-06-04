#pragma once

#include "engine/assets/AssetManager.h"

#include <filesystem>

namespace arclab {

class Importer {
public:
    explicit Importer(AssetManager& assets)
        : assets_(assets)
    {
    }

    AssetHandle import_file(const std::filesystem::path& path)
    {
        const std::string extension = path.extension().string();
        if (extension == ".obj") {
            return assets_.register_asset(AssetType::Mesh, path);
        }
        if (extension == ".ppm" || extension == ".png" || extension == ".jpg") {
            return assets_.register_asset(AssetType::Texture, path);
        }
        if (extension == ".glsl" || extension == ".vert" || extension == ".frag") {
            return assets_.register_asset(AssetType::Shader, path);
        }
        if (extension == ".json") {
            return assets_.register_asset(AssetType::Experiment, path);
        }
        return assets_.register_asset(AssetType::Unknown, path);
    }

private:
    AssetManager& assets_;
};

} // namespace arclab
