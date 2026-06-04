#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace arclab {

enum class LogLevel {
    Info,
    Warning,
    Error,
};

inline const char* log_level_name(LogLevel level)
{
    switch (level) {
    case LogLevel::Info:
        return "info";
    case LogLevel::Warning:
        return "warn";
    case LogLevel::Error:
        return "error";
    }

    return "log";
}

inline std::string timestamp_seconds()
{
    using clock = std::chrono::system_clock;
    const auto now = clock::now().time_since_epoch();
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now).count();
    return std::to_string(seconds);
}

inline void log(LogLevel level, const std::string& message)
{
    std::ostream& out = level == LogLevel::Error ? std::cerr : std::cout;
    out << "[arclab][" << log_level_name(level) << "][" << timestamp_seconds() << "] " << message << '\n';
}

inline void log_info(const std::string& message)
{
    log(LogLevel::Info, message);
}

inline void log_warning(const std::string& message)
{
    log(LogLevel::Warning, message);
}

inline void log_error(const std::string& message)
{
    log(LogLevel::Error, message);
}

inline std::string to_string_trimmed(double value)
{
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(4) << value;
    std::string text = stream.str();

    while (text.size() > 1 && text.back() == '0') {
        text.pop_back();
    }
    if (!text.empty() && text.back() == '.') {
        text.pop_back();
    }

    return text;
}

} // namespace arclab
