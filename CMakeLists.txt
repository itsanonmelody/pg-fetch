cmake_minimum_required(VERSION 3.8)
project(pg-fetch CXX)

# Set C standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/.bin")

# Set source files
set(PG_FETCH_SOURCE
    src/main.cpp
    src/cli.cpp
    src/util.cpp
)

# Set include directories
include_directories(
    include
)

# Configure executable
add_executable(pg-fetch ${PG_FETCH_SOURCE})