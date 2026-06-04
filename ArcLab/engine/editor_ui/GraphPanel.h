#pragma once

#include "engine/editor_ui/Panel.h"
#include "engine/simulation/Recorder.h"

namespace arclab {

class GraphPanel : public Panel {
public:
    GraphPanel()
        : Panel("Graphs")
    {
    }

    void bind(const SensorRecorder* recorder)
    {
        recorder_ = recorder;
    }

    std::size_t sample_count() const
    {
        return recorder_ == nullptr ? 0u : recorder_->samples().size();
    }

private:
    const SensorRecorder* recorder_ = nullptr;
};

} // namespace arclab
