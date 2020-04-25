cmake_minimum_required(VERSION 3.8)
project(pg-fetch C CXX)

# Configure options
option(PG_FETCH_BUILD_TESTS "Build test programs"      ON )
option(PG_FETCH_BUILD_32BIT "Build programs as 32 bit" OFF)

# Set C standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/.bin")

# Use 32 bit libpq
IF(PG_FETCH_BUILD_32BIT)
    set(PostgreSQL_FOUND ON CACHE BOOL "PostgreSQL found" FORCE)
    set(PostgreSQL_LIBRARIES "C:/Program Files (x86)/PostgreSQL/10/lib/libpq.lib" CACHE STRING "PostgreSQL library files" FORCE)
    set(PostgreSQL_INCLUDE_DIRS "C:/Program Files (x86)/PostgreSQL/10/include;C:/Program Files (x86)/PostgreSQL/10/include/server" CACHE STRING "PostgreSQL include directories" FORCE)
    set(PostgreSQL_LIBRARY_DIRS "C:/Program Files (x86)/PostgreSQL/10/lib" CACHE STRING "PostgreSQL library directories" FORCE)
    set(PostgreSQL_VERSION_STRING "10.12" CACHE STRING "PostgreSQL version" FORCE)
ELSE()
    find_package(PostgreSQL REQUIRED)
ENDIF()

# Set source files
set(PG_FETCH_SOURCE
    src/main.cpp
    src/cli.cpp
    src/util.cpp
)

# Set libraries
set(PG_FETCH_LIBRARIES
    pqxx
    shapelib
)

# Set include directories
include_directories(
    include
    deps/shapelib
    deps/libpqxx/include

    ${PostgreSQL_INCLUDE_DIRS}
)

message(STATUS "\n\n${PostgreSQL_INCLUDE_DIRS}\n\n")

# Add third-party dependencies
set(BUILD_TEST OFF CACHE BOOL "Build libpqxx test builds" FORCE)
add_subdirectory(deps/libpqxx)

# Build ShapeLib
set(SHAPE_LIB_SOURCES
    deps/shapelib/shpopen.c
    deps/shapelib/dbfopen.c
    deps/shapelib/safileio.c
    deps/shapelib/shptree.c
    deps/shapelib/sbnsearch.c
)
add_library(shapelib STATIC ${SHAPE_LIB_SOURCES})

# Configure executable
add_executable(pg-fetch ${PG_FETCH_SOURCE})
target_link_libraries(pg-fetch ${PG_FETCH_LIBRARIES})

# Build tests
IF(PG_FETCH_BUILD_TESTS)
    add_subdirectory(tests)
ENDIF()