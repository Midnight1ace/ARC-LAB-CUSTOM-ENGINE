#include "engine/engine.h"

#include "engine/core/Logging.h"
#include "engine/serialization/Serialization.h"
#include "engine/simulation/Experiment.h"
#include "engine/simulation/Simulation.h"

#include <charconv>
#include <filesystem>
#include <optional>
#include <string_view>
#include <system_error>

namespace arclab {
namespace {

struct DemoOptions {
    double durationSeconds = 5.0;
    std::filesystem::path experimentPath = "experiments/DefaultExperiment.json";
};

DemoOptions parse_options(int argc, char** argv)
{
    DemoOptions options;

    for (int i = 1; i < argc; ++i) {
        const std::string_view arg(argv[i]);
        if (arg == "--duration" && i + 1 < argc) {
            double value = options.durationSeconds;
            const std::string_view raw(argv[++i]);
            const auto* begin = raw.data();
            const auto* end = raw.data() + raw.size();
            const auto result = std::from_chars(begin, end, value);
            if (result.ec == std::errc{} && value > 0.0) {
                options.durationSeconds = value;
            } else {
                log_warning("Ignoring invalid --duration value.");
            }
        } else if (arg == "--experiment" && i + 1 < argc) {
            options.experimentPath = argv[++i];
        }
    }

    return options;
}

} // namespace

int run_console_demo(int argc, char** argv)
{
    const DemoOptions options = parse_options(argc, argv);

    Experiment experiment = make_magnetic_particle_confinement_experiment();
    std::string loadError;
    if (std::filesystem::exists(options.experimentPath)) {
        std::optional<Experiment> loaded = load_experiment_json(options.experimentPath, &loadError);
        if (loaded) {
            experiment = *loaded;
            log_info("Loaded experiment file: " + options.experimentPath.string());
        } else {
            log_warning("Using built-in experiment because JSON load failed: " + loadError);
        }
    } else {
        log_warning("Experiment file not found; using built-in default: " + options.experimentPath.string());
    }

    Simulation simulation;
    simulation.load_experiment(experiment);

    log_info("ArcLab Engine booted.");
    log_info("Running experiment: " + experiment.name);
    log_info("Fixed timestep: " + to_string_trimmed(experiment.settings.fixedDt) + " seconds");

    const int stepCount = static_cast<int>(options.durationSeconds / experiment.settings.fixedDt);
    const int reportEvery = static_cast<int>(0.5 / experiment.settings.fixedDt);

    for (int step = 0; step < stepCount; ++step) {
        simulation.step_fixed();

        if (reportEvery > 0 && (step % reportEvery) == 0) {
            const PhysicsStats& stats = simulation.stats();
            log_info("t=" + to_string_trimmed(simulation.time_seconds()) +
                     "s particles=" + std::to_string(stats.activeParticles) +
                     " avg_v=" + to_string_trimmed(stats.averageVelocity) +
                     " ke=" + to_string_trimmed(stats.totalKineticEnergy) +
                     " lost=" + std::to_string(stats.lostParticles));
        }
    }

    std::filesystem::create_directories("results");
    const std::filesystem::path csvPath = "results/magnetic_confinement.csv";
    if (simulation.recorder().export_csv(csvPath)) {
        log_info("Exported sensor CSV: " + csvPath.string());
    } else {
        log_error("Failed to export sensor CSV: " + csvPath.string());
        return 1;
    }

    const PhysicsStats& finalStats = simulation.stats();
    log_info("Finished. Active particles: " + std::to_string(finalStats.activeParticles) +
             ", average distance from center: " + to_string_trimmed(finalStats.averageDistanceFromCenter));
    return 0;
}

} // namespace arclab
