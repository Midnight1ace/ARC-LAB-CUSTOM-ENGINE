#pragma once

#include "engine/math/Transform.h"
#include "engine/math/Vector.h"
#include "engine/physics/Material.h"

namespace arclab {

struct RigidBody {
    Transform transform;
    Vec3 velocity;
    Vec3 accumulatedForce;
    double mass = 1.0;
    bool isStatic = false;
    PhysicsMaterial material;

    void add_force(const Vec3& force)
    {
        accumulatedForce += force;
    }

    void integrate(double dt)
    {
        if (isStatic || mass <= 0.0) {
            accumulatedForce = {};
            return;
        }

        const Vec3 acceleration = accumulatedForce / mass;
        velocity += acceleration * dt;
        transform.position += velocity * dt;
        accumulatedForce = {};
    }
};

} // namespace arclab
