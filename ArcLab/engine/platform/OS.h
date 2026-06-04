#pragma once

#include <string>

namespace arclab {

inline std::string platform_name()
{
#if defined(_WIN32)
    return "Windows";
#elif defined(__APPLE__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#else
    return "Unknown";
#endif
}

inline bool native_window_supported()
{
#if defined(_WIN32)
    return true;
#else
    return false;
#endif
}

} // namespace arclab
