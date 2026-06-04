#pragma once

#include "engine/editor_ui/GraphPanel.h"
#include "engine/editor_ui/HierarchyPanel.h"
#include "engine/editor_ui/PropertyGrid.h"
#include "engine/editor_ui/Toolbar.h"
#include "engine/tools/Console.h"

namespace arclab {

class EditorUI {
public:
    void bind_recorder(const SensorRecorder* recorder)
    {
        graphPanel_.bind(recorder);
    }

    void inspect(Scene& scene, EntityId entity)
    {
        hierarchyPanel_.select(entity);
        propertyGrid_.inspect_entity(scene, entity);
    }

    HierarchyPanel& hierarchy()
    {
        return hierarchyPanel_;
    }

    PropertyGrid& properties()
    {
        return propertyGrid_;
    }

    GraphPanel& graphs()
    {
        return graphPanel_;
    }

    Toolbar& toolbar()
    {
        return toolbar_;
    }

    ConsoleBuffer& console()
    {
        return console_;
    }

private:
    HierarchyPanel hierarchyPanel_;
    PropertyGrid propertyGrid_;
    GraphPanel graphPanel_;
    Toolbar toolbar_;
    ConsoleBuffer console_;
};

} // namespace arclab
