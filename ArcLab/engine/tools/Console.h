#pragma once

#include <string>
#include <vector>

namespace arclab {

enum class ConsoleMessageType {
    Info,
    Warning,
    Error,
};

struct ConsoleMessage {
    ConsoleMessageType type = ConsoleMessageType::Info;
    std::string text;
};

class ConsoleBuffer {
public:
    void push(ConsoleMessageType type, std::string text)
    {
        messages_.push_back({ type, std::move(text) });
    }

    void info(std::string text)
    {
        push(ConsoleMessageType::Info, std::move(text));
    }

    void warning(std::string text)
    {
        push(ConsoleMessageType::Warning, std::move(text));
    }

    void error(std::string text)
    {
        push(ConsoleMessageType::Error, std::move(text));
    }

    const std::vector<ConsoleMessage>& messages() const
    {
        return messages_;
    }

private:
    std::vector<ConsoleMessage> messages_;
};

} // namespace arclab
