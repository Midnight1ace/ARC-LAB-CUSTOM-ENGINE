#pragma once

#include "engine/platform/OS.h"

#include <string>

namespace arclab {

struct PlatformInfo {
    std::string name = platform_name();
    bool supportsNativeWindow = native_window_supported();
};

inline PlatformInfo current_platform()
{
    return {};
}

} // namespace arclab
