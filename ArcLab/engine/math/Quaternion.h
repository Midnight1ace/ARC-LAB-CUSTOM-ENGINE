#pragma once

#include "engine/math/Math.h"
#include "engine/math/Vector.h"

#include <cmath>

namespace arclab {

struct Quaternion {
    double w = 1.0;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    constexpr Quaternion() = default;

    constexpr Quaternion(double wValue, double xValue, double yValue, double zValue)
        : w(wValue)
        , x(xValue)
        , y(yValue)
        , z(zValue)
    {
    }

    static Quaternion identity()
    {
        return {};
    }

    static Quaternion from_axis_angle(const Vec3& axis, double angleRadians)
    {
        const Vec3 unitAxis = normalized(axis);
        const double halfAngle = angleRadians * 0.5;
        const double s = std::sin(halfAngle);
        return normalized_quaternion({ std::cos(halfAngle), unitAxis.x * s, unitAxis.y * s, unitAxis.z * s });
    }

    static Quaternion from_euler_xyz(const Vec3& radiansValue)
    {
        const Quaternion qx = from_axis_angle({ 1.0, 0.0, 0.0 }, radiansValue.x);
        const Quaternion qy = from_axis_angle({ 0.0, 1.0, 0.0 }, radiansValue.y);
        const Quaternion qz = from_axis_angle({ 0.0, 0.0, 1.0 }, radiansValue.z);
        return qz * qy * qx;
    }

    Quaternion operator*(const Quaternion& rhs) const
    {
        return {
            w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z,
            w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
            w * rhs.y - x * rhs.z + y * rhs.w + z * rhs.x,
            w * rhs.z + x * rhs.y - y * rhs.x + z * rhs.w,
        };
    }

    Quaternion conjugate() const
    {
        return { w, -x, -y, -z };
    }

    double length() const
    {
        return std::sqrt(w * w + x * x + y * y + z * z);
    }

    Vec3 rotate(const Vec3& vector) const
    {
        const Quaternion qv { 0.0, vector.x, vector.y, vector.z };
        const Quaternion result = (*this) * qv * conjugate();
        return { result.x, result.y, result.z };
    }

private:
    static Quaternion normalized_quaternion(const Quaternion& q)
    {
        const double len = q.length();
        if (nearly_zero(len)) {
            return {};
        }

        return { q.w / len, q.x / len, q.y / len, q.z / len };
    }
};

inline Quaternion normalized(const Quaternion& q)
{
    const double len = q.length();
    if (nearly_zero(len)) {
        return {};
    }

    return { q.w / len, q.x / len, q.y / len, q.z / len };
}

} // namespace arclab
