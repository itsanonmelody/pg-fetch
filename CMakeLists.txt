cmake_minimum_required(VERSION 3.8)
project(pg-fetch CXX)

# Configure options
option(PG_FETCH_BUILD_TESTS "Build test programs" ON)

# Set C standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/.bin")

# Find libpq
# find_package(PostgreSQL REQUIRED)

# Set source files
set(PG_FETCH_SOURCE
    src/main.cpp
    src/cli.cpp
    src/util.cpp
)

# Set libraries
set(PG_FETCH_LIBRARIES
    # ${PostgreSQL_LIBRARIES}
    pqxx
)

# Set include directories
include_directories(
    include
)

# Add third-party dependencies
set(SKIP_BUILD_TEST ON CACHE BOOL "Skip libpqxx build tests" FORCE)
add_subdirectory(deps/libpqxx)

# Configure executable
add_executable(pg-fetch ${PG_FETCH_SOURCE})
target_link_libraries(pg-fetch ${PG_FETCH_LIBRARIES})

# Build tests
IF(PG_FETCH_BUILD_TESTS)
    add_subdirectory(tests)
ENDIF()