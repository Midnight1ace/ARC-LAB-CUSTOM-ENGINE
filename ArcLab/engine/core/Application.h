#pragma once

#include "engine/core/Time.h"
#include "engine/platform/Input.h"
#include "engine/platform/Window.h"

#include <functional>
#include <string>

namespace arclab {

struct ApplicationConfig {
    std::string name = "ArcLab";
    int windowWidth = 1280;
    int windowHeight = 720;
    double fixedDt = 1.0 / 120.0;
};

class Application {
public:
    using UpdateCallback = std::function<void(double frameDt, InputState&)>;

    explicit Application(ApplicationConfig config = {})
        : config_(std::move(config))
        , window_({ config_.name, config_.windowWidth, config_.windowHeight, true })
    {
    }

    bool start()
    {
        return window_.open();
    }

    void stop()
    {
        window_.close();
    }

    void tick_once(double frameDt, const UpdateCallback& update)
    {
        input_.begin_frame();
        if (update) {
            update(frameDt, input_);
        }
    }

    bool running() const
    {
        return window_.is_open();
    }

    Window& window()
    {
        return window_;
    }

    const ApplicationConfig& config() const
    {
        return config_;
    }

private:
    ApplicationConfig config_;
    Window window_;
    InputState input_;
};

} // namespace arclab
