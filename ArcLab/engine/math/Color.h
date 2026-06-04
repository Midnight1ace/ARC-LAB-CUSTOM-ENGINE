#pragma once

#include "engine/math/Math.h"

#include <cstdint>

namespace arclab {

struct Color {
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;

    constexpr Color() = default;

    constexpr Color(float red, float green, float blue, float alpha = 1.0f)
        : r(red)
        , g(green)
        , b(blue)
        , a(alpha)
    {
    }

    static constexpr Color white() { return { 1.0f, 1.0f, 1.0f, 1.0f }; }
    static constexpr Color black() { return { 0.0f, 0.0f, 0.0f, 1.0f }; }
    static constexpr Color grid() { return { 0.23f, 0.27f, 0.30f, 1.0f }; }
    static constexpr Color electric_blue() { return { 0.25f, 0.62f, 1.0f, 1.0f }; }
    static constexpr Color magnetic_red() { return { 1.0f, 0.25f, 0.22f, 1.0f }; }
    static constexpr Color velocity_green() { return { 0.25f, 0.95f, 0.50f, 1.0f }; }
    static constexpr Color warning_yellow() { return { 1.0f, 0.82f, 0.25f, 1.0f }; }

    std::uint32_t to_rgba8() const
    {
        const auto channel = [](float value) {
            return static_cast<std::uint32_t>(clamp(static_cast<double>(value), 0.0, 1.0) * 255.0 + 0.5);
        };

        return (channel(r) << 24u) | (channel(g) << 16u) | (channel(b) << 8u) | channel(a);
    }
};

inline Color lerp(const Color& a, const Color& b, double t)
{
    const float amount = static_cast<float>(clamp(t, 0.0, 1.0));
    return {
        a.r + (b.r - a.r) * amount,
        a.g + (b.g - a.g) * amount,
        a.b + (b.b - a.b) * amount,
        a.a + (b.a - a.a) * amount,
    };
}

} // namespace arclab
