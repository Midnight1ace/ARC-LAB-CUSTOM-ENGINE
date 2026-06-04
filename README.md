# ArcLab Engine

ArcLab is a custom, physics-first simulation engine for scientific experiments. The first target is the **Magnetic Particle Confinement Experiment** from the project master plan: charged particles, electric and magnetic fields, deterministic fixed timesteps, live sensor data, and CSV export.

This repository currently starts with a self-contained C++20 console prototype. It does not depend on SDL, OpenGL, ImGui, or other third-party libraries yet, so the simulation core can be built and tested before the editor and renderer arrive.

## Current Features

- CMake project with an `arclab_engine` library and `arclab` executable.
- Core vector math.
- Fixed timestep simulation loop.
- Contiguous particle system.
- Uniform electric, magnetic, and gravity fields.
- Lorentz force integration: `F = q * (E + cross(v, B))`.
- Semi-implicit Euler integrator.
- Particle emitter for the magnetic confinement demo.
- Sensor recorder with CSV export.

## Build

```powershell
cmake -S . -B build
cmake --build build
```

Run the demo:

```powershell
.\build\Debug\arclab.exe --duration 5
```

Depending on your CMake generator, the executable may also be under `build\arclab.exe` or `build\Release\arclab.exe`.

## Output

The console demo writes:

```text
results/magnetic_confinement.csv
```

That file contains time, active particle count, lost particle count, average velocity, total kinetic energy, and average distance from the experiment center.

## Next Milestones

1. Load `experiments/DefaultExperiment.json` instead of using the built-in default.
2. Add unit tests for math, fields, and particle integration.
3. Add a renderer shell with grid, camera, particles, trails, and debug vectors.
4. Add editor panels with Dear ImGui and live graphs with ImPlot.
