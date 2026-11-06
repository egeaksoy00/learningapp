cmake_minimum_required(VERSION 3.20)

project(CodePathLearning VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CODEPATH_FETCH_RAYLIB "Download raylib when it is not installed" ON)

find_package(raylib 5.0 QUIET)

if(NOT raylib_FOUND AND CODEPATH_FETCH_RAYLIB)
    include(FetchContent)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(raylib)
endif()

if(NOT TARGET raylib)
    message(FATAL_ERROR "raylib was not found. Install it or configure with CODEPATH_FETCH_RAYLIB=ON.")
endif()

add_executable(codepath-learning
    src/main.cpp
)
target_link_libraries(codepath-learning PRIVATE raylib)

if(APPLE)
    target_link_libraries(codepath-learning PRIVATE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
endif()

