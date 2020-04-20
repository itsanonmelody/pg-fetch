cmake_minimum_required(VERSION 3.8)

# Set output directory
set(OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/tests/.bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")

# Configure cli
add_executable(
    cli
    cli/main.cpp
    ../src/cli.cpp
    ../src/util.cpp
)

# Configure pq
add_executable(
    pq
    pq/main.cpp
)
target_link_libraries(pq pqxx)

# Configure dbf
add_executable(
    dbf
    dbf/main.cpp
)
target_link_libraries(dbf pqxx)