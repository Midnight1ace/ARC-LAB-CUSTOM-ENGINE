#pragma once

#include "engine/simulation/Experiment.h"
#include "engine/simulation/Scene.h"

namespace arclab {

inline Scene make_scene_from_experiment(const Experiment& experiment)
{
    Scene scene;

    const EntityId emitter = scene.create_entity("Particle Emitter");
    scene.transform(emitter).transform.position = experiment.emitter.position;
    scene.add_particle_emitter(emitter, experiment.emitter);

    int fieldIndex = 1;
    for (const UniformField& field : experiment.fields) {
        const EntityId fieldEntity = scene.create_entity("Field " + std::to_string(fieldIndex++));
        scene.add_field(fieldEntity, field);
    }

    const EntityId velocitySensor = scene.create_entity("Average Velocity Sensor");
    scene.add_sensor(velocitySensor, { "average_velocity", 30.0 });

    const EntityId energySensor = scene.create_entity("Kinetic Energy Sensor");
    scene.add_sensor(energySensor, { "total_kinetic_energy", 30.0 });

    return scene;
}

} // namespace arclab
