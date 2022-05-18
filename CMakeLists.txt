cmake_minimum_required(VERSION 3.12)

# Project
project(engine VERSION 1.0 LANGUAGES CXX)

# Packages
find_package(Threads REQUIRED)

# Third party includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/libataxx/src/)

# Flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Add the executable
add_executable(
    main
    src/main.cpp
    src/options.cpp

    src/search/eval.cpp
    src/search/minimax.cpp
    src/search/search.cpp

    src/uai/display.cpp
    src/uai/go.cpp
    src/uai/isready.cpp
    src/uai/listen.cpp
    src/uai/moves.cpp
    src/uai/perft.cpp
    src/uai/position.cpp
    src/uai/setoption.cpp
    src/uai/split.cpp
    src/uai/uainewgame.cpp
)

# Add the executable
add_executable(
    tests
    tests/main.cpp

    tests/uai/moves.cpp
    tests/uai/position.cpp

    src/options.cpp
    src/uai/moves.cpp
    src/uai/position.cpp
)

target_link_libraries(main Threads::Threads "${CMAKE_CURRENT_SOURCE_DIR}/libs/libataxx/build/static/libataxx.a")
target_link_libraries(tests "${CMAKE_CURRENT_SOURCE_DIR}/libs/libataxx/build/static/libataxx.a")
