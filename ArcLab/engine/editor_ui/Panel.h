#pragma once

#include <string>

namespace arclab {

class Panel {
public:
    explicit Panel(std::string title)
        : title_(std::move(title))
    {
    }

    virtual ~Panel() = default;

    const std::string& title() const
    {
        return title_;
    }

    bool visible() const
    {
        return visible_;
    }

    void set_visible(bool visible)
    {
        visible_ = visible;
    }

private:
    std::string title_;
    bool visible_ = true;
};

} // namespace arclab
