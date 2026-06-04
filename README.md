# ArcLab Engine

ArcLab is a custom, physics-first simulation engine for scientific experiments. The first target is the **Magnetic Particle Confinement Experiment** from the project master plan: charged particles, electric and magnetic fields, deterministic fixed timesteps, live sensor data, and CSV export.

This repository currently contains a self-contained C++20 console prototype in `ArcLab/`. It does not depend on SDL, OpenGL, ImGui, or other third-party libraries yet, so the simulation core can be built and tested before the editor and renderer arrive.

## Current Features

- CMake project with an `arclab_engine` library, `arclab` executable, sandbox executable, and tests.
- Core vector math.
- Quaternion, matrix, color, camera, and debug draw primitives.
- Fixed timestep simulation loop.
- Contiguous particle system.
- Uniform electric, magnetic, and gravity fields.
- Lorentz force integration: `F = q * (E + cross(v, B))`.
- Semi-implicit Euler integrator.
- Particle emitter for the magnetic confinement demo.
- Sensor recorder with CSV export.
- JSON experiment loading.
- Scene/entity/component skeleton and editor panel state.
- Magnetic-field parameter sweep export.
- Software preview image export from the sandbox.

## Build

```powershell
cd ArcLab
cmake -S . -B build
cmake --build build
```

Run the demo:

```powershell
.\build\Debug\arclab.exe --duration 5
```

Run the sweep:

```powershell
.\build\Debug\arclab.exe --duration 2 --sweep
```

Run tests:

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

Run the native editor viewport:

```powershell
.\build\Debug\arclab_editor.exe
```

Depending on your CMake generator, the executable may also be under `build\arclab.exe` or `build\Release\arclab.exe`.

## Output

The console demo writes:

```text
ArcLab/results/magnetic_confinement.csv
ArcLab/results/magnetic_field_sweep.csv
ArcLab/results/sandbox_preview.ppm
```

That file contains time, active particle count, lost particle count, average velocity, total kinetic energy, and average distance from the experiment center.

## Next Milestones

1. Replace the dry-run renderer with an actual window and graphics backend.
2. Render grid, debug vectors, particles, and trails.
3. Connect Dear ImGui panels to the existing editor-state models.
4. Add live graphs with ImPlot.
