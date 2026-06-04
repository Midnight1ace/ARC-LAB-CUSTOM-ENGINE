#pragma once

#include "engine/assets/Assets.h"

#include <filesystem>
#include <unordered_map>

namespace arclab {

class AssetManager {
public:
    AssetHandle register_asset(AssetType type, const std::filesystem::path& path)
    {
        AssetHandle handle { nextId_++, type };
        assets_[handle.id] = { handle, path.stem().string(), path.string() };
        return handle;
    }

    const AssetMetadata* find(AssetHandle handle) const
    {
        const auto found = assets_.find(handle.id);
        return found == assets_.end() ? nullptr : &found->second;
    }

    std::size_t size() const
    {
        return assets_.size();
    }

private:
    AssetId nextId_ = 1;
    std::unordered_map<AssetId, AssetMetadata> assets_;
};

} // namespace arclab
