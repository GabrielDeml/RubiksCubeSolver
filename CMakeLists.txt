cmake_minimum_required(VERSION 3.10)
# Ensure compatibility for dependencies requiring older policy versions (raylib)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project name
project(RandomCPP)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(main ${SOURCES})

# Optional: Set output directory
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# raylib (modern 3D package) via FetchContent
include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "Don't build raylib examples" FORCE)
set(BUILD_GAMES    OFF CACHE BOOL "Don't build raylib games" FORCE)

FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.0
)

FetchContent_MakeAvailable(raylib)

target_link_libraries(main PRIVATE raylib)

if(APPLE)
  # Silence deprecated OpenGL warnings on macOS builds
  target_compile_definitions(main PRIVATE GL_SILENCE_DEPRECATION)
endif()
