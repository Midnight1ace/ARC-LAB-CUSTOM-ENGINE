#pragma once

#include "engine/math/Vector.h"

#include <algorithm>
#include <optional>

namespace arclab {

struct SphereCollider {
    Vec3 center;
    double radius = 0.5;
};

struct PlaneCollider {
    Vec3 normal { 0.0, 1.0, 0.0 };
    double distanceFromOrigin = 0.0;
};

struct AabbCollider {
    Vec3 min;
    Vec3 max;
};

struct Contact {
    Vec3 point;
    Vec3 normal;
    double penetration = 0.0;
};

inline std::optional<Contact> collide_sphere_plane(const SphereCollider& sphere, const PlaneCollider& plane)
{
    const Vec3 normal = normalized(plane.normal);
    const double signedDistance = dot(sphere.center, normal) - plane.distanceFromOrigin;
    if (signedDistance > sphere.radius) {
        return std::nullopt;
    }

    Contact contact;
    contact.normal = normal;
    contact.penetration = sphere.radius - signedDistance;
    contact.point = sphere.center - normal * signedDistance;
    return contact;
}

inline std::optional<Contact> collide_sphere_sphere(const SphereCollider& a, const SphereCollider& b)
{
    const Vec3 delta = b.center - a.center;
    const double distanceSquared = delta.length_squared();
    const double radiusSum = a.radius + b.radius;
    if (distanceSquared > radiusSum * radiusSum) {
        return std::nullopt;
    }

    const double dist = std::sqrt(distanceSquared);
    const Vec3 normal = nearly_zero(dist) ? Vec3 { 0.0, 1.0, 0.0 } : delta / dist;

    Contact contact;
    contact.normal = normal;
    contact.penetration = radiusSum - dist;
    contact.point = a.center + normal * (a.radius - contact.penetration * 0.5);
    return contact;
}

inline bool intersects(const AabbCollider& a, const AabbCollider& b)
{
    return a.min.x <= b.max.x && a.max.x >= b.min.x &&
        a.min.y <= b.max.y && a.max.y >= b.min.y &&
        a.min.z <= b.max.z && a.max.z >= b.min.z;
}

} // namespace arclab
