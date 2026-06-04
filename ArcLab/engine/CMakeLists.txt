add_library(arclab_engine
    engine.cpp
    renderer/Renderer.cpp
)

target_include_directories(arclab_engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

if(MSVC)
    target_compile_options(arclab_engine PRIVATE /W4 /permissive-)
else()
    target_compile_options(arclab_engine PRIVATE -Wall -Wextra -Wpedantic)
endif()
