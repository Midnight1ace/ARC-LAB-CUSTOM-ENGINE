#pragma once

namespace arclab {

enum class PlaybackState {
    Stopped,
    Playing,
    Paused,
};

struct SimulationState {
    PlaybackState playback = PlaybackState::Stopped;
    double simulationSpeed = 1.0;
    bool deterministic = true;

    bool playing() const
    {
        return playback == PlaybackState::Playing;
    }
};

} // namespace arclab
