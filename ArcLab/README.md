# ArcLab Engine Project

This folder contains the C++20 ArcLab engine prototype.

## Build

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

## Run

```powershell
.\build\Debug\arclab.exe --duration 5 --experiment experiments\DefaultExperiment.json
```

Run a magnetic-field parameter sweep:

```powershell
.\build\Debug\arclab.exe --duration 2 --sweep
```

Run the sandbox renderer/editor-state dry run:

```powershell
.\build\Debug\arclab_sandbox.exe
```

Run the native software editor viewport:

```powershell
.\build\Debug\arclab_editor.exe
```

Run the editor briefly for verification:

```powershell
.\build\Debug\arclab_editor.exe --frames 3
```

Run tests:

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

## Current Engine Pieces

- Fixed-step simulation loop.
- Particle emitter and contiguous particle storage.
- Electric, magnetic, and gravity fields.
- Lorentz force for charged particles.
- Semi-implicit Euler integration.
- JSON experiment loading.
- CSV sensor recording.
- Magnetic-field batch sweeps.
- Scene/entity/component skeleton.
- Camera, matrix, quaternion, color, and debug draw primitives.
- Software preview renderer that exports `results/sandbox_preview.ppm`.
- Editor panel state models.
- Sandbox executable and automated tests.
