cmake_minimum_required(VERSION 3.16)
project(valdaryn CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Platform layer dependency: raylib (window / input / GPU blitting only) ---
# Fetched and built from source so the build is self-contained and portable.
include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

# Don't build raylib's own examples/games -- we only want the library.
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
    GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(raylib)

add_executable(valdaryn
    src/main.cpp       # platform: window + fixed-timestep loop     (mine)
    src/world.cpp      # the simulation                             (YOURS)
    src/render.cpp     # raylib drawing of a World                  (mine)
)

target_link_libraries(valdaryn PRIVATE raylib)

# Warnings on -- you want these loud while you own the sim internals.
if(NOT MSVC)
    target_compile_options(valdaryn PRIVATE -Wall -Wextra)
endif()
