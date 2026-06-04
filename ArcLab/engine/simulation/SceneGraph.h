#pragma once

#include "engine/simulation/Scene.h"

#include <unordered_map>
#include <vector>

namespace arclab {

class SceneGraph {
public:
    void set_parent(EntityId child, EntityId parent)
    {
        parent_[child] = parent;
        children_[parent].push_back(child);
    }

    EntityId parent_of(EntityId child) const
    {
        const auto found = parent_.find(child);
        return found == parent_.end() ? kInvalidEntity : found->second;
    }

    const std::vector<EntityId>& children_of(EntityId parent) const
    {
        static const std::vector<EntityId> empty;
        const auto found = children_.find(parent);
        return found == children_.end() ? empty : found->second;
    }

private:
    std::unordered_map<EntityId, EntityId> parent_;
    std::unordered_map<EntityId, std::vector<EntityId>> children_;
};

} // namespace arclab
