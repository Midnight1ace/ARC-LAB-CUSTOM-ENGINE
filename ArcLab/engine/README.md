# ArcLab Engine Library

This folder contains the reusable engine code. The current implementation is mostly header-only to speed up early iteration, with `engine.cpp` acting as the console demo bridge.

Subsystems:

- `core`: lifecycle, logging, memory, jobs, timing.
- `math`: vectors, matrices, quaternions, colors, transforms.
- `physics`: particles, fields, rigid bodies, collisions, constraints.
- `simulation`: experiments, scenes, sensors, recorders, snapshots.
- `renderer`: cameras, debug draw, software preview, render abstractions.
- `editor_ui`: editor state models.
- `assets` and `serialization`: loading and persistence helpers.
