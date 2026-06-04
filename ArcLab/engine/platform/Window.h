#pragma once

#include <string>

namespace arclab {

struct WindowDesc {
    std::string title = "ArcLab";
    int width = 1280;
    int height = 720;
    bool resizable = true;
};

class Window {
public:
    explicit Window(WindowDesc desc = {})
        : desc_(std::move(desc))
    {
    }

    bool open()
    {
        open_ = true;
        return open_;
    }

    void close()
    {
        open_ = false;
    }

    bool is_open() const
    {
        return open_;
    }

    const WindowDesc& desc() const
    {
        return desc_;
    }

private:
    WindowDesc desc_;
    bool open_ = false;
};

} // namespace arclab
