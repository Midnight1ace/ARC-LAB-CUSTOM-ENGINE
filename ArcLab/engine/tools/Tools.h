#pragma once

#include "engine/tools/Benchmark.h"
#include "engine/tools/Console.h"
#include "engine/tools/Diagnostics.h"
#include "engine/tools/Profiler.h"

namespace arclab {

struct ToolContext {
    ConsoleBuffer console;
    Profiler profiler;
    Diagnostics diagnostics;
};

} // namespace arclab
