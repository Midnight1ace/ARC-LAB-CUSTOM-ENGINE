#pragma once

#include <string>
#include <utility>
#include <vector>

namespace arclab {

struct RenderPass {
    std::string name;
    bool enabled = true;
};

class RenderGraph {
public:
    void add_pass(RenderPass pass)
    {
        passes_.push_back(std::move(pass));
    }

    const std::vector<RenderPass>& passes() const
    {
        return passes_;
    }

private:
    std::vector<RenderPass> passes_;
};

} // namespace arclab
