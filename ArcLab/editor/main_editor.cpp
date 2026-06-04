#include "editor/EditorApp.h"

#include <charconv>
#include <string_view>

int main(int argc, char** argv)
{
    arclab::EditorConfig config;
    for (int i = 1; i < argc; ++i) {
        const std::string_view arg(argv[i]);
        if (arg == "--frames" && i + 1 < argc) {
            const std::string_view raw(argv[++i]);
            int frames = 0;
            const auto result = std::from_chars(raw.data(), raw.data() + raw.size(), frames);
            if (result.ec == std::errc{} && frames > 0) {
                config.maxFrames = frames;
            }
        }
    }

    arclab::EditorApp app(config);
    return app.run();
}
