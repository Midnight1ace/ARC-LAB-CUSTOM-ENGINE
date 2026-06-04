#pragma once

#include <algorithm>
#include <cmath>

namespace arclab {

constexpr double kPi = 3.141592653589793238462643383279502884;
constexpr double kEpsilon = 1.0e-9;

inline double radians(double degrees)
{
    return degrees * (kPi / 180.0);
}

inline double degrees(double radiansValue)
{
    return radiansValue * (180.0 / kPi);
}

inline double clamp(double value, double minValue, double maxValue)
{
    return std::max(minValue, std::min(value, maxValue));
}

inline bool nearly_zero(double value, double epsilon = kEpsilon)
{
    return std::abs(value) <= epsilon;
}

} // namespace arclab
