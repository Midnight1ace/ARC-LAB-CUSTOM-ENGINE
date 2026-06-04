#pragma once

#include <functional>
#include <future>
#include <vector>

namespace arclab {

class JobSystem {
public:
    template <typename Fn>
    auto schedule(Fn&& fn)
    {
        return std::async(std::launch::async, std::forward<Fn>(fn));
    }

    void parallel_for(int count, const std::function<void(int)>& body)
    {
        std::vector<std::future<void>> jobs;
        jobs.reserve(static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i) {
            jobs.push_back(schedule([&, i] { body(i); }));
        }
        for (auto& job : jobs) {
            job.get();
        }
    }
};

} // namespace arclab
