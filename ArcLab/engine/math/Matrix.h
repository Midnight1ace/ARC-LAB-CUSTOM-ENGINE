#pragma once

#include "engine/math/Quaternion.h"
#include "engine/math/Vector.h"

#include <array>

namespace arclab {

struct Mat4 {
    std::array<double, 16> m {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0,
    };

    static Mat4 identity()
    {
        return {};
    }

    double& at(int row, int column)
    {
        return m[static_cast<std::size_t>(column * 4 + row)];
    }

    double at(int row, int column) const
    {
        return m[static_cast<std::size_t>(column * 4 + row)];
    }

    static Mat4 translation(const Vec3& value)
    {
        Mat4 result;
        result.at(0, 3) = value.x;
        result.at(1, 3) = value.y;
        result.at(2, 3) = value.z;
        return result;
    }

    static Mat4 scale(const Vec3& value)
    {
        Mat4 result;
        result.at(0, 0) = value.x;
        result.at(1, 1) = value.y;
        result.at(2, 2) = value.z;
        return result;
    }

    static Mat4 rotation(const Quaternion& q)
    {
        const Quaternion n = normalized(q);
        const double xx = n.x * n.x;
        const double yy = n.y * n.y;
        const double zz = n.z * n.z;
        const double xy = n.x * n.y;
        const double xz = n.x * n.z;
        const double yz = n.y * n.z;
        const double wx = n.w * n.x;
        const double wy = n.w * n.y;
        const double wz = n.w * n.z;

        Mat4 result;
        result.at(0, 0) = 1.0 - 2.0 * (yy + zz);
        result.at(0, 1) = 2.0 * (xy - wz);
        result.at(0, 2) = 2.0 * (xz + wy);
        result.at(1, 0) = 2.0 * (xy + wz);
        result.at(1, 1) = 1.0 - 2.0 * (xx + zz);
        result.at(1, 2) = 2.0 * (yz - wx);
        result.at(2, 0) = 2.0 * (xz - wy);
        result.at(2, 1) = 2.0 * (yz + wx);
        result.at(2, 2) = 1.0 - 2.0 * (xx + yy);
        return result;
    }
};

inline Mat4 operator*(const Mat4& lhs, const Mat4& rhs)
{
    Mat4 result;
    result.m.fill(0.0);

    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            for (int i = 0; i < 4; ++i) {
                result.at(row, column) += lhs.at(row, i) * rhs.at(i, column);
            }
        }
    }

    return result;
}

inline Vec3 transform_point(const Mat4& matrix, const Vec3& point)
{
    const double x = matrix.at(0, 0) * point.x + matrix.at(0, 1) * point.y + matrix.at(0, 2) * point.z + matrix.at(0, 3);
    const double y = matrix.at(1, 0) * point.x + matrix.at(1, 1) * point.y + matrix.at(1, 2) * point.z + matrix.at(1, 3);
    const double z = matrix.at(2, 0) * point.x + matrix.at(2, 1) * point.y + matrix.at(2, 2) * point.z + matrix.at(2, 3);
    const double w = matrix.at(3, 0) * point.x + matrix.at(3, 1) * point.y + matrix.at(3, 2) * point.z + matrix.at(3, 3);

    if (nearly_zero(w)) {
        return { x, y, z };
    }

    return { x / w, y / w, z / w };
}

inline Mat4 perspective(double verticalFovRadians, double aspectRatio, double nearPlane, double farPlane)
{
    Mat4 result;
    result.m.fill(0.0);

    const double f = 1.0 / std::tan(verticalFovRadians * 0.5);
    result.at(0, 0) = f / aspectRatio;
    result.at(1, 1) = f;
    result.at(2, 2) = (farPlane + nearPlane) / (nearPlane - farPlane);
    result.at(2, 3) = (2.0 * farPlane * nearPlane) / (nearPlane - farPlane);
    result.at(3, 2) = -1.0;
    return result;
}

inline Mat4 look_at(const Vec3& eye, const Vec3& target, const Vec3& up)
{
    const Vec3 forward = normalized(target - eye);
    const Vec3 right = normalized(cross(forward, up));
    const Vec3 cameraUp = cross(right, forward);

    Mat4 result;
    result.at(0, 0) = right.x;
    result.at(0, 1) = right.y;
    result.at(0, 2) = right.z;
    result.at(0, 3) = -dot(right, eye);
    result.at(1, 0) = cameraUp.x;
    result.at(1, 1) = cameraUp.y;
    result.at(1, 2) = cameraUp.z;
    result.at(1, 3) = -dot(cameraUp, eye);
    result.at(2, 0) = -forward.x;
    result.at(2, 1) = -forward.y;
    result.at(2, 2) = -forward.z;
    result.at(2, 3) = dot(forward, eye);
    return result;
}

} // namespace arclab
