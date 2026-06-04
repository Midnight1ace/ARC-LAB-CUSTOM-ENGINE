#pragma once

#include "editor/EditorConfig.h"

namespace arclab {

class EditorApp {
public:
    explicit EditorApp(EditorConfig config = {});

    int run();

private:
    EditorConfig config_;
};

int run_editor();
int run_editor_main();

} // namespace arclab
