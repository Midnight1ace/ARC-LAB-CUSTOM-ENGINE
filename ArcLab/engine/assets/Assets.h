#pragma once

#include <cstdint>
#include <string>

namespace arclab {

using AssetId = std::uint64_t;
constexpr AssetId kInvalidAssetId = 0;

enum class AssetType {
    Unknown,
    Mesh,
    Texture,
    Shader,
    Experiment,
};

struct AssetHandle {
    AssetId id = kInvalidAssetId;
    AssetType type = AssetType::Unknown;

    bool valid() const
    {
        return id != kInvalidAssetId;
    }
};

struct AssetMetadata {
    AssetHandle handle;
    std::string name;
    std::string path;
};

} // namespace arclab
