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
    src/minimax.cpp
    src/options.cpp
    src/search.cpp
    src/uai.cpp
)

target_link_libraries(main Threads::Threads "${CMAKE_CURRENT_SOURCE_DIR}/libs/libataxx/build/static/libataxx.a")
