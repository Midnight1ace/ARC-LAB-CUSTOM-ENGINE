#pragma once

#include "engine/math/Math.h"

#include <cmath>
#include <ostream>

namespace arclab {

struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;

    constexpr Vec3() = default;

    constexpr Vec3(double xValue, double yValue, double zValue)
        : x(xValue)
        , y(yValue)
        , z(zValue)
    {
    }

    constexpr Vec3 operator+(const Vec3& rhs) const
    {
        return { x + rhs.x, y + rhs.y, z + rhs.z };
    }

    constexpr Vec3 operator-(const Vec3& rhs) const
    {
        return { x - rhs.x, y - rhs.y, z - rhs.z };
    }

    constexpr Vec3 operator-() const
    {
        return { -x, -y, -z };
    }

    constexpr Vec3 operator*(double scalar) const
    {
        return { x * scalar, y * scalar, z * scalar };
    }

    constexpr Vec3 operator/(double scalar) const
    {
        return { x / scalar, y / scalar, z / scalar };
    }

    Vec3& operator+=(const Vec3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vec3& operator*=(double scalar)
    {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    Vec3& operator/=(double scalar)
    {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    double length_squared() const
    {
        return x * x + y * y + z * z;
    }

    double length() const
    {
        return std::sqrt(length_squared());
    }
};

inline constexpr Vec3 operator*(double scalar, const Vec3& vector)
{
    return vector * scalar;
}

inline double dot(const Vec3& lhs, const Vec3& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline Vec3 cross(const Vec3& lhs, const Vec3& rhs)
{
    return {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x,
    };
}

inline Vec3 normalized(const Vec3& vector)
{
    const double length = vector.length();
    if (nearly_zero(length)) {
        return {};
    }

    return vector / length;
}

inline double distance(const Vec3& lhs, const Vec3& rhs)
{
    return (lhs - rhs).length();
}

inline std::ostream& operator<<(std::ostream& out, const Vec3& vector)
{
    out << '(' << vector.x << ", " << vector.y << ", " << vector.z << ')';
    return out;
}

} // namespace arclab
