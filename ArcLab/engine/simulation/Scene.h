#pragma once

#include "engine/math/Transform.h"
#include "engine/physics/Field.h"
#include "engine/physics/Physics.h"

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace arclab {

using EntityId = std::uint32_t;
constexpr EntityId kInvalidEntity = 0;

struct Entity {
    EntityId id = kInvalidEntity;
    std::string name;
};

struct TransformComponent {
    Transform transform;
};

struct ParticleEmitterComponent {
    ParticleEmitter emitter;
};

struct FieldComponent {
    UniformField field;
};

struct SensorComponent {
    std::string quantity = "average_velocity";
    double sampleRateHz = 30.0;
};

class Scene {
public:
    EntityId create_entity(const std::string& name)
    {
        const EntityId id = nextEntityId_++;
        entities_.push_back({ id, name });
        transforms_.emplace(id, TransformComponent {});
        return id;
    }

    const std::vector<Entity>& entities() const
    {
        return entities_;
    }

    TransformComponent& transform(EntityId id)
    {
        return transforms_[id];
    }

    void add_particle_emitter(EntityId id, const ParticleEmitter& emitter)
    {
        emitters_[id] = ParticleEmitterComponent { emitter };
    }

    void add_field(EntityId id, const UniformField& field)
    {
        fields_[id] = FieldComponent { field };
    }

    void add_sensor(EntityId id, const SensorComponent& sensor)
    {
        sensors_[id] = sensor;
    }

    const std::unordered_map<EntityId, ParticleEmitterComponent>& emitters() const
    {
        return emitters_;
    }

    const std::unordered_map<EntityId, FieldComponent>& fields() const
    {
        return fields_;
    }

    const std::unordered_map<EntityId, SensorComponent>& sensors() const
    {
        return sensors_;
    }

    std::optional<Entity> find_by_name(const std::string& name) const
    {
        for (const Entity& entity : entities_) {
            if (entity.name == name) {
                return entity;
            }
        }

        return std::nullopt;
    }

private:
    EntityId nextEntityId_ = 1;
    std::vector<Entity> entities_;
    std::unordered_map<EntityId, TransformComponent> transforms_;
    std::unordered_map<EntityId, ParticleEmitterComponent> emitters_;
    std::unordered_map<EntityId, FieldComponent> fields_;
    std::unordered_map<EntityId, SensorComponent> sensors_;
};

} // namespace arclab
