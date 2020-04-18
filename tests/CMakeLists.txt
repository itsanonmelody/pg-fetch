cmake_minimum_required(VERSION 3.8)

# Set output directory
set(OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/tests/.bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")

# Set source files
set(SOURCE
    "${PROJECT_SOURCE_DIR}/src/cli.cpp"
    "${PROJECT_SOURCE_DIR}/src/util.cpp"
)

# Set include directories
set(
    "${PROJECT_SOURCE_DIR}/include"
)

add_executable(
    cli
    cli/main.cpp
    ${SOURCE}
)