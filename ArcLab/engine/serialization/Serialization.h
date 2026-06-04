#pragma once

#include "engine/serialization/JSONSerializer.h"
#include "engine/simulation/Experiment.h"

#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>

namespace arclab {

inline double json_number_or(const JsonValue* value, double fallback)
{
    if (value == nullptr) {
        return fallback;
    }

    const double* number = value->number();
    return number == nullptr ? fallback : *number;
}

inline bool json_bool_or(const JsonValue* value, bool fallback)
{
    if (value == nullptr) {
        return fallback;
    }

    const bool* boolean = value->boolean();
    return boolean == nullptr ? fallback : *boolean;
}

inline std::string json_string_or(const JsonValue* value, const std::string& fallback)
{
    if (value == nullptr) {
        return fallback;
    }

    const std::string* string = value->string();
    return string == nullptr ? fallback : *string;
}

inline Vec3 json_vec3_or(const JsonValue* value, const Vec3& fallback)
{
    if (value == nullptr) {
        return fallback;
    }

    const JsonValue::Array* array = value->array();
    if (array == nullptr || array->size() != 3) {
        return fallback;
    }

    return {
        json_number_or(&(*array)[0], fallback.x),
        json_number_or(&(*array)[1], fallback.y),
        json_number_or(&(*array)[2], fallback.z),
    };
}

inline FieldType field_type_from_string(const std::string& value, FieldType fallback)
{
    if (value == "electric") {
        return FieldType::Electric;
    }
    if (value == "magnetic") {
        return FieldType::Magnetic;
    }
    if (value == "gravity") {
        return FieldType::Gravity;
    }

    return fallback;
}

inline std::optional<Experiment> load_experiment_json(const std::filesystem::path& path, std::string* error = nullptr)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        if (error != nullptr) {
            *error = "Could not open experiment file.";
        }
        return std::nullopt;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    JsonParseResult parsed = parse_json(buffer.str());
    if (!parsed.value) {
        if (error != nullptr) {
            *error = parsed.error;
        }
        return std::nullopt;
    }

    Experiment experiment = make_magnetic_particle_confinement_experiment();
    const JsonValue& root = *parsed.value;

    experiment.name = json_string_or(find_json_member(root, "name"), experiment.name);
    experiment.description = json_string_or(find_json_member(root, "description"), experiment.description);
    experiment.engineVersion = json_string_or(find_json_member(root, "engine_version"), experiment.engineVersion);
    experiment.unitSystem = json_string_or(find_json_member(root, "unit_system"), experiment.unitSystem);
    experiment.randomSeed = static_cast<std::uint32_t>(json_number_or(find_json_member(root, "random_seed"), experiment.randomSeed));

    if (const JsonValue* simulation = find_json_member(root, "simulation")) {
        experiment.settings.fixedDt = json_number_or(find_json_member(*simulation, "fixed_dt"), experiment.settings.fixedDt);
        experiment.settings.solverIterations = static_cast<int>(
            json_number_or(find_json_member(*simulation, "solver_iterations"), experiment.settings.solverIterations));
        experiment.settings.maxParticles = static_cast<std::size_t>(
            json_number_or(find_json_member(*simulation, "max_particles"), static_cast<double>(experiment.settings.maxParticles)));
        experiment.settings.boundaryRadius = json_number_or(
            find_json_member(*simulation, "boundary_radius"), experiment.settings.boundaryRadius);
    }

    if (const JsonValue* emitter = find_json_member(root, "emitter")) {
        experiment.emitter.position = json_vec3_or(find_json_member(*emitter, "position"), experiment.emitter.position);
        experiment.emitter.direction = json_vec3_or(find_json_member(*emitter, "direction"), experiment.emitter.direction);
        experiment.emitter.ratePerSecond = json_number_or(
            find_json_member(*emitter, "rate_per_second"), experiment.emitter.ratePerSecond);
        experiment.emitter.speed = json_number_or(find_json_member(*emitter, "speed"), experiment.emitter.speed);
        experiment.emitter.particleMass = json_number_or(
            find_json_member(*emitter, "particle_mass"), experiment.emitter.particleMass);
        experiment.emitter.particleCharge = json_number_or(
            find_json_member(*emitter, "particle_charge"), experiment.emitter.particleCharge);
        experiment.emitter.particleLifetime = json_number_or(
            find_json_member(*emitter, "particle_lifetime"), experiment.emitter.particleLifetime);
    }

    if (const JsonValue* fields = find_json_member(root, "fields")) {
        const JsonValue::Array* fieldArray = fields->array();
        if (fieldArray != nullptr) {
            experiment.fields.clear();
            for (const JsonValue& fieldValue : *fieldArray) {
                UniformField field;
                field.type = field_type_from_string(json_string_or(find_json_member(fieldValue, "type"), "magnetic"), field.type);
                field.direction = json_vec3_or(find_json_member(fieldValue, "direction"), field.direction);
                field.strength = json_number_or(find_json_member(fieldValue, "strength"), field.strength);
                field.enabled = json_bool_or(find_json_member(fieldValue, "enabled"), field.enabled);
                experiment.fields.push_back(field);
            }
        }
    }

    return experiment;
}

} // namespace arclab
