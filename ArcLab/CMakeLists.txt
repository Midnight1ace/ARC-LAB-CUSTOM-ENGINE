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

if(MSVC)
    target_compile_options(arclab PRIVATE /W4 /permissive-)
else()
    target_compile_options(arclab PRIVATE -Wall -Wextra -Wpedantic)
endif()
