#pragma once

namespace arclab {

struct PhysicsMaterial {
    double restitution = 0.5;
    double friction = 0.4;
    double density = 1.0;
};

} // namespace arclab
