#pragma once

#include "engine/math/Color.h"
#include "engine/math/Vector.h"
#include "engine/physics/Field.h"

#include <vector>

namespace arclab {

struct DebugLine {
    Vec3 start;
    Vec3 end;
    Color color;
};

class DebugDrawList {
public:
    void clear()
    {
        lines_.clear();
    }

    void line(const Vec3& start, const Vec3& end, const Color& color)
    {
        lines_.push_back({ start, end, color });
    }

    void vector(const Vec3& origin, const Vec3& direction, const Color& color, double scale = 1.0)
    {
        line(origin, origin + direction * scale, color);
    }

    void grid(double halfExtent = 10.0, double spacing = 1.0)
    {
        for (double p = -halfExtent; p <= halfExtent + kEpsilon; p += spacing) {
            const Color color = nearly_zero(p) ? Color::white() : Color::grid();
            line({ -halfExtent, 0.0, p }, { halfExtent, 0.0, p }, color);
            line({ p, 0.0, -halfExtent }, { p, 0.0, halfExtent }, color);
        }
    }

    void field_vectors(const FieldSystem& fields, double halfExtent = 4.0, double spacing = 1.0, double scale = 0.25)
    {
        for (double x = -halfExtent; x <= halfExtent + kEpsilon; x += spacing) {
            for (double z = -halfExtent; z <= halfExtent + kEpsilon; z += spacing) {
                const Vec3 origin { x, 0.05, z };
                const FieldSample sample = fields.sample_at(origin);
                if (!nearly_zero(sample.electric.length_squared())) {
                    vector(origin, sample.electric, Color::electric_blue(), scale);
                }
                if (!nearly_zero(sample.magnetic.length_squared())) {
                    vector(origin + Vec3 { 0.0, 0.08, 0.0 }, sample.magnetic, Color::magnetic_red(), scale);
                }
            }
        }
    }

    const std::vector<DebugLine>& lines() const
    {
        return lines_;
    }

private:
    std::vector<DebugLine> lines_;
};

} // namespace arclab
