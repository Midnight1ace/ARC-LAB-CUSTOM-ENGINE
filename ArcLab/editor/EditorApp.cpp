#include "editor/EditorApp.h"

#include "engine/core/Logging.h"
#include "engine/renderer/DebugDraw.h"
#include "engine/renderer/SoftwarePreview.h"
#include "engine/simulation/Experiment.h"
#include "engine/simulation/Simulation.h"

#include <chrono>
#include <string>
#include <thread>
#include <utility>

#if defined(_WIN32)
#define NOMINMAX
#include <windows.h>
#endif

namespace arclab {
namespace {

struct EditorFrameState {
    Simulation simulation;
    DebugDrawList debugDraw;
    SoftwarePreview preview;
    bool running = true;

    EditorFrameState(int width, int height)
        : preview(width, height)
    {
        simulation.load_experiment(make_magnetic_particle_confinement_experiment());
    }
};

void render_editor_frame(EditorFrameState& state)
{
    for (int i = 0; i < 2; ++i) {
        state.simulation.step_fixed();
    }

    state.debugDraw.clear();
    state.debugDraw.grid(10.0, 1.0);
    state.debugDraw.field_vectors(state.simulation.world().fields(), 4.0, 1.0, 0.18);

    state.preview.clear();
    state.preview.draw_debug_lines(state.debugDraw);
    state.preview.draw_particles(state.simulation.world().particles());
}

#if defined(_WIN32)

EditorFrameState* state_from_window(HWND window)
{
    return reinterpret_cast<EditorFrameState*>(GetWindowLongPtrW(window, GWLP_USERDATA));
}

std::wstring widen(const std::string& text)
{
    return std::wstring(text.begin(), text.end());
}

LRESULT CALLBACK editor_window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case WM_CREATE: {
        const auto* create = reinterpret_cast<CREATESTRUCTW*>(lparam);
        SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
        return 0;
    }
    case WM_KEYDOWN:
        if (wparam == VK_ESCAPE) {
            DestroyWindow(window);
            return 0;
        }
        break;
    case WM_CLOSE:
        DestroyWindow(window);
        return 0;
    case WM_DESTROY:
        if (EditorFrameState* state = state_from_window(window)) {
            state->running = false;
        }
        PostQuitMessage(0);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT paint;
        HDC dc = BeginPaint(window, &paint);
        if (EditorFrameState* state = state_from_window(window)) {
            BITMAPINFO bitmap {};
            bitmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bitmap.bmiHeader.biWidth = state->preview.width();
            bitmap.bmiHeader.biHeight = -state->preview.height();
            bitmap.bmiHeader.biPlanes = 1;
            bitmap.bmiHeader.biBitCount = 32;
            bitmap.bmiHeader.biCompression = BI_RGB;
            SetDIBitsToDevice(
                dc,
                0,
                0,
                static_cast<DWORD>(state->preview.width()),
                static_cast<DWORD>(state->preview.height()),
                0,
                0,
                0,
                static_cast<UINT>(state->preview.height()),
                state->preview.rgba8_pixels().data(),
                &bitmap,
                DIB_RGB_COLORS);
        }
        EndPaint(window, &paint);
        return 0;
    }
    default:
        break;
    }

    return DefWindowProcW(window, message, wparam, lparam);
}

int run_win32_editor(const EditorConfig& config)
{
    HINSTANCE instance = GetModuleHandleW(nullptr);
    const wchar_t* className = L"ArcLabEditorWindow";

    WNDCLASSW windowClass {};
    windowClass.lpfnWndProc = editor_window_proc;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = className;
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    RegisterClassW(&windowClass);

    EditorFrameState state(config.width, config.height);
    render_editor_frame(state);

    HWND window = CreateWindowExW(
        0,
        className,
        widen(config.title).c_str(),
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        config.width,
        config.height,
        nullptr,
        nullptr,
        instance,
        &state);

    if (window == nullptr) {
        log_error("Failed to create ArcLab editor window.");
        return 1;
    }

    log_info("ArcLab editor window opened. Press Escape to close.");

    MSG message {};
    int frameCount = 0;
    while (state.running) {
        while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }

        render_editor_frame(state);
        InvalidateRect(window, nullptr, FALSE);
        ++frameCount;
        if (config.maxFrames > 0 && frameCount >= config.maxFrames) {
            DestroyWindow(window);
        }
        std::this_thread::sleep_for(std::chrono::duration<double>(config.targetFrameSeconds));
    }

    return 0;
}

#endif

} // namespace

EditorApp::EditorApp(EditorConfig config)
    : config_(std::move(config))
{
}

int EditorApp::run()
{
#if defined(_WIN32)
    return run_win32_editor(config_);
#else
    log_warning("Native editor window is only implemented for Windows right now.");
    return 0;
#endif
}

int run_editor()
{
    EditorApp app;
    return app.run();
}

} // namespace arclab
