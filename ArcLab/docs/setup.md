# Setup

Build from the `ArcLab` folder:

```powershell
cmake -S . -B build
cmake --build build --config Debug
ctest --test-dir build -C Debug --output-on-failure
```

Run:

```powershell
.\build\Debug\arclab.exe --duration 2
.\build\Debug\arclab.exe --duration 2 --sweep
.\build\Debug\arclab_sandbox.exe
.\build\Debug\arclab_editor.exe
```
