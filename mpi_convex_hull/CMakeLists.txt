cmake_minimum_required(VERSION 3.25)
project(mpi_convex_hull
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(MPI REQUIRED)

add_subdirectory(benchmark)
add_subdirectory(convex_hull)

add_executable(${PROJECT_NAME} main.cc)
