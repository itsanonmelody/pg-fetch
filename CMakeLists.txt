cmake_minimum_required(VERSION 3.8)
project(pg-fetch C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/.bin")

# Set source files
set(PG_FETCH_SOURCE
    src/main.c
)

# Configure executable
add_executable(pg-fetch ${PG_FETCH_SOURCE})