#include "engine/assets/AssetManager.h"
#include "engine/assets/Importer.h"
#include "engine/assets/ModelLoader.h"
#include "engine/assets/ShaderLoader.h"
#include "engine/assets/TextureLoader.h"
#include "engine/core/Allocator.h"
#include "engine/core/JobSystem.h"
#include "engine/core/Memory.h"
#include "engine/editor_ui/EditorUI.h"
#include "engine/math/Matrix.h"
#include "engine/platform/Platform.h"
#include "engine/physics/Collision.h"
#include "engine/physics/Constraint.h"
#include "engine/physics/Physics.h"
#include "engine/renderer/Framebuffer.h"
#include "engine/renderer/GraphicsContext.h"
#include "engine/renderer/Mesh.h"
#include "engine/renderer/RenderPass.h"
#include "engine/renderer/Shader.h"
#include "engine/renderer/Texture.h"
#include "engine/serialization/Serialization.h"
#include "engine/simulation/ExperimentScene.h"
#include "engine/simulation/Simulation.h"
#include "engine/tools/Benchmark.h"

#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>

namespace {

int failures = 0;

void expect(bool condition, const std::string& message)
{
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
        ++failures;
    }
}

void test_vector_math()
{
    const arclab::Vec3 x { 1.0, 0.0, 0.0 };
    const arclab::Vec3 y { 0.0, 1.0, 0.0 };
    const arclab::Vec3 z = arclab::cross(x, y);
    expect(std::abs(z.z - 1.0) < 1.0e-9, "cross product should produce +Z");
    expect(std::abs(arclab::dot(x, y)) < 1.0e-9, "orthogonal vectors should have zero dot product");
}

void test_quaternion_rotation()
{
    const arclab::Quaternion rotation = arclab::Quaternion::from_axis_angle({ 0.0, 1.0, 0.0 }, arclab::radians(90.0));
    const arclab::Vec3 rotated = rotation.rotate({ 1.0, 0.0, 0.0 });
    expect(std::abs(rotated.z + 1.0) < 1.0e-6, "Y-axis quaternion should rotate +X toward -Z");
}

void test_particle_simulation()
{
    arclab::Simulation simulation;
    simulation.load_experiment(arclab::make_magnetic_particle_confinement_experiment());
    for (int i = 0; i < 30; ++i) {
        simulation.step_fixed();
    }
    expect(simulation.stats().activeParticles > 0, "simulation should emit particles");
    expect(simulation.stats().averageVelocity > 0.0, "simulation should report average velocity");
}

void test_collision()
{
    const arclab::SphereCollider sphere { { 0.0, 0.25, 0.0 }, 0.5 };
    const arclab::PlaneCollider plane;
    const auto contact = arclab::collide_sphere_plane(sphere, plane);
    expect(contact.has_value(), "sphere should collide with plane");
    if (contact) {
        expect(contact->penetration > 0.0, "sphere-plane penetration should be positive");
    }

    const arclab::AabbCollider a { { -1.0, -1.0, -1.0 }, { 1.0, 1.0, 1.0 } };
    const arclab::AabbCollider b { { 0.5, 0.5, 0.5 }, { 2.0, 2.0, 2.0 } };
    expect(arclab::intersects(a, b), "AABBs should intersect");
}

void test_experiment_json()
{
    std::string error;
    const auto experiment = arclab::load_experiment_json("experiments/DefaultExperiment.json", &error);
    expect(experiment.has_value(), "DefaultExperiment.json should load: " + error);
    if (experiment) {
        expect(experiment->name == "Magnetic Particle Confinement Experiment", "experiment name should parse");
        expect(!experiment->fields.empty(), "experiment fields should parse");
    }
}

void test_scene_and_snapshot()
{
    arclab::Simulation simulation;
    simulation.load_experiment(arclab::make_magnetic_particle_confinement_experiment());
    for (int i = 0; i < 10; ++i) {
        simulation.step_fixed();
    }

    const arclab::SimulationSnapshot snapshot = simulation.snapshot();
    expect(!snapshot.particles.empty(), "snapshot should capture particle state");

    const arclab::Scene scene = arclab::make_scene_from_experiment(arclab::make_magnetic_particle_confinement_experiment());
    expect(scene.entities().size() >= 3, "experiment scene should contain emitter, fields, and sensors");
}

void test_batch_sweep()
{
    const auto results = arclab::sweep_magnetic_field(arclab::make_magnetic_particle_confinement_experiment(), { 1.0, 2.0 }, 0.2);
    expect(results.size() == 2, "batch sweep should return one result per field strength");
    expect(results.front().stats.activeParticles > 0, "batch sweep should run simulation");
}

void test_support_subsystems()
{
    arclab::AssetManager assets;
    arclab::Importer importer(assets);
    const arclab::AssetHandle handle = importer.import_file("experiments/DefaultExperiment.json");
    expect(handle.valid(), "importer should register an asset handle");

    arclab::LinearAllocator allocator(1024);
    expect(allocator.allocate(128) != nullptr, "linear allocator should allocate memory");

    arclab::MemoryStats memory;
    memory.record_allocation(64);
    memory.record_free(16);
    expect(memory.allocatedBytes == 48, "memory stats should track allocation/free");

    arclab::GraphicsContext graphics(arclab::RendererBackend::None);
    expect(graphics.initialize(), "graphics context should initialize");

    arclab::Framebuffer framebuffer(16, 16);
    framebuffer.clear(arclab::Color::black());
    framebuffer.set_pixel(1, 1, arclab::Color::white());
    expect(framebuffer.pixels().size() == 256, "framebuffer should own pixels");

    arclab::Mesh grid = arclab::Mesh::make_line_grid(1.0, 1.0);
    expect(!grid.vertices.empty(), "grid mesh should have vertices");

    arclab::RenderGraph graph;
    graph.add_pass({ "Debug Lines", true });
    expect(graph.passes().size() == 1, "render graph should store passes");

    arclab::Scene scene = arclab::make_scene_from_experiment(arclab::make_magnetic_particle_confinement_experiment());
    arclab::EditorUI editor;
    editor.inspect(scene, scene.entities().front().id);
    expect(!editor.properties().rows().empty(), "editor property grid should inspect an entity");

    expect(!arclab::current_platform().name.empty(), "platform info should have a name");
}

} // namespace

int main()
{
    test_vector_math();
    test_quaternion_rotation();
    test_collision();
    test_particle_simulation();
    test_experiment_json();
    test_scene_and_snapshot();
    test_batch_sweep();
    test_support_subsystems();

    if (failures == 0) {
        std::cout << "All ArcLab tests passed.\n";
    }

    return failures == 0 ? 0 : 1;
}
