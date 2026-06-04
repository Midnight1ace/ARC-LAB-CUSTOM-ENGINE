cmake_minimum_required(VERSION 3.24)

project(
    ArcLab
    VERSION 0.1.0
    DESCRIPTION "Physics-first scientific simulation engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(engine)

add_executable(arclab main.cpp)
target_link_libraries(arclab PRIVATE arclab_engine)

add_executable(arclab_sandbox
    sandbox/Sandbox.cpp
    sandbox/SandboxApp.cpp
)
target_link_libraries(arclab_sandbox PRIVATE arclab_engine)

add_executable(arclab_editor
    editor/main_editor.cpp
    editor/EditorApp.cpp
    editor/EditorMain.cpp
)
target_link_libraries(arclab_editor PRIVATE arclab_engine)

enable_testing()
add_executable(arclab_tests tests/TestRunner.cpp)
target_link_libraries(arclab_tests PRIVATE arclab_engine)
add_test(NAME arclab_tests COMMAND arclab_tests)
set_tests_properties(arclab_tests PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    target_compile_options(arclab PRIVATE /W4 /permissive-)
    target_compile_options(arclab_sandbox PRIVATE /W4 /permissive-)
    target_compile_options(arclab_editor PRIVATE /W4 /permissive-)
    target_compile_options(arclab_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(arclab PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(arclab_sandbox PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(arclab_editor PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(arclab_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
    target_link_libraries(arclab_editor PRIVATE user32 gdi32)
endif()
