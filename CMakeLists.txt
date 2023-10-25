# Setup
cmake_minimum_required(VERSION 3.12)
project(PopcatReloaded)

# Build Options
set(CMAKE_CXX_STANDARD 20)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default is Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if (WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3 -fsanitize=address")
endif()

# Libraries compilation
add_subdirectory("src/options")
add_subdirectory("src/board")
add_subdirectory("src/ai")

# Executable creation
add_executable(popcat "src/main.cc")
target_link_libraries(popcat
    options
    board
    ai
)

# Tests
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
    message("Running perft tests.")

    find_package(
        Python3
        REQUIRED
        COMPONENTS Interpreter
    )

    add_custom_target(perft
        COMMAND python
                "${CMAKE_SOURCE_DIR}/tests/tools/run_perft.py"
                "${CMAKE_SOURCE_DIR}/tests/perft/"
                "${CMAKE_CURRENT_BINARY_DIR}/Debug/popcat.exe"
                False
                False
        DEPENDS popcat
    )
endif()
