#pragma once

#include <array>
#include <cstdint>

namespace arclab {

enum class Key : std::uint16_t {
    Unknown = 0,
    Space,
    Escape,
    W,
    A,
    S,
    D,
    Q,
    E,
    Count,
};

class KeyboardState {
public:
    void set(Key key, bool down)
    {
        keys_[static_cast<std::size_t>(key)] = down;
    }

    bool down(Key key) const
    {
        return keys_[static_cast<std::size_t>(key)];
    }

private:
    std::array<bool, static_cast<std::size_t>(Key::Count)> keys_ {};
};

} // namespace arclab
