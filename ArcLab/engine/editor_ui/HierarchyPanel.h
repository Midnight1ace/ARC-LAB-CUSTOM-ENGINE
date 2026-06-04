#pragma once

#include "engine/editor_ui/Panel.h"
#include "engine/simulation/Scene.h"

namespace arclab {

class HierarchyPanel : public Panel {
public:
    HierarchyPanel()
        : Panel("Scene Explorer")
    {
    }

    void select(EntityId entity)
    {
        selected_ = entity;
    }

    EntityId selected() const
    {
        return selected_;
    }

private:
    EntityId selected_ = kInvalidEntity;
};

} // namespace arclab
