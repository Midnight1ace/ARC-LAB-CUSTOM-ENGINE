#pragma once

#include "engine/editor_ui/Panel.h"
#include "engine/simulation/Scene.h"

#include <string>
#include <vector>

namespace arclab {

struct PropertyRow {
    std::string name;
    std::string value;
};

class PropertyGrid : public Panel {
public:
    PropertyGrid()
        : Panel("Properties")
    {
    }

    void inspect_entity(const Scene& scene, EntityId entity)
    {
        rows_.clear();
        for (const Entity& item : scene.entities()) {
            if (item.id == entity) {
                rows_.push_back({ "id", std::to_string(item.id) });
                rows_.push_back({ "name", item.name });
                break;
            }
        }
    }

    const std::vector<PropertyRow>& rows() const
    {
        return rows_;
    }

private:
    std::vector<PropertyRow> rows_;
};

} // namespace arclab
