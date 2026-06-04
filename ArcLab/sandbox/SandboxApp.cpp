#include "sandbox/SandboxApp.h"

#include "engine/core/Logging.h"
#include "engine/editor_ui/EditorUI.h"
#include "engine/renderer/DebugDraw.h"
#include "engine/renderer/Renderer.h"
#include "engine/renderer/SoftwarePreview.h"
#include "engine/simulation/Experiment.h"
#include "engine/simulation/ExperimentScene.h"
#include "engine/simulation/Simulation.h"

namespace arclab {

int run_sandbox_app()
{
    const Experiment experiment = make_magnetic_particle_confinement_experiment();
    Scene scene = make_scene_from_experiment(experiment);

    Simulation simulation;
    simulation.load_experiment(experiment);

    PerspectiveCamera camera;
    camera.orbit(0.45, 0.25, 0.0);

    DebugDrawList debugDraw;
    debugDraw.grid();
    debugDraw.field_vectors(simulation.world().fields());

    Renderer renderer;
    renderer.begin_frame(camera);
    renderer.submit_debug_lines(debugDraw);

    EditorUI editor;
    editor.bind_recorder(&simulation.recorder());
    if (!scene.entities().empty()) {
        editor.inspect(scene, scene.entities().front().id);
    }
    editor.console().info("Sandbox scene loaded with " + std::to_string(scene.entities().size()) + " entities.");

    for (int i = 0; i < 120; ++i) {
        simulation.step_fixed();
    }

    renderer.submit_particles(simulation.world().particles().particles().size());
    renderer.end_frame();

    SoftwarePreview preview(1280, 720);
    preview.clear();
    preview.draw_debug_lines(debugDraw);
    preview.draw_particles(simulation.world().particles());
    const bool previewSaved = preview.save_ppm("results/sandbox_preview.ppm");

    log_info("Sandbox frame prepared.");
    log_info("Scene entities: " + std::to_string(scene.entities().size()));
    log_info("Graph samples: " + std::to_string(editor.graphs().sample_count()));
    log_info("Debug lines: " + std::to_string(renderer.stats().debugLineCount));
    log_info("Particles submitted: " + std::to_string(renderer.stats().particleCount));
    log_info(std::string("Software preview: ") + (previewSaved ? "results/sandbox_preview.ppm" : "failed"));
    return 0;
}

} // namespace arclab
