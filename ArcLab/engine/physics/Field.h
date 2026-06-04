#pragma once

#include "engine/math/Vector.h"

#include <vector>

namespace arclab {

enum class FieldType {
    Electric,
    Magnetic,
    Gravity,
};

struct FieldSample {
    Vec3 electric;
    Vec3 magnetic;
    Vec3 gravityAcceleration;
};

struct UniformField {
    FieldType type = FieldType::Magnetic;
    Vec3 direction { 0.0, 1.0, 0.0 };
    double strength = 1.0;
    bool enabled = true;

    Vec3 vector() const
    {
        return normalized(direction) * strength;
    }
};

class FieldSystem {
public:
    void add_uniform_field(const UniformField& field)
    {
        fields_.push_back(field);
    }

    void clear()
    {
        fields_.clear();
    }

    FieldSample sample_at(const Vec3&) const
    {
        FieldSample sample;
        for (const UniformField& field : fields_) {
            if (!field.enabled) {
                continue;
            }

            switch (field.type) {
            case FieldType::Electric:
                sample.electric += field.vector();
                break;
            case FieldType::Magnetic:
                sample.magnetic += field.vector();
                break;
            case FieldType::Gravity:
                sample.gravityAcceleration += field.vector();
                break;
            }
        }

        return sample;
    }

    const std::vector<UniformField>& fields() const
    {
        return fields_;
    }

private:
    std::vector<UniformField> fields_;
};

} // namespace arclab
