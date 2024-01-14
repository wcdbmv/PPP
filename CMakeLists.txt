cmake_minimum_required(VERSION 3.25)
project(mpi
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(MPI REQUIRED)

add_compile_definitions("$<$<CONFIG:DEBUG>:DEBUG>")
add_compile_options("$<$<CONFIG:RELEASE>:-O2>")

add_subdirectory(mpi_convex_hull)
