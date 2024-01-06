cmake_minimum_required(VERSION 3.25)
project(benchmark
        LANGUAGES CXX)

set(HEADERS
    include/benchmark.h)

set(SOURCES
    src/benchmark.cc)

add_library(${PROJECT_NAME} STATIC
            ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME}
  PUBLIC include)

target_link_libraries(${PROJECT_NAME}
                      PRIVATE MPI::MPI_CXX)
