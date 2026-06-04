# Architecture

ArcLab is an engine library plus applications. The engine owns simulation, math, physics, rendering abstractions, serialization, assets, and editor-neutral state. Applications such as the console runner, sandbox, and editor link against the engine.

The first demo is Magnetic Particle Confinement: an emitter creates charged particles, fields apply Lorentz force, sensors record data, and render/debug systems expose the invisible physics.
