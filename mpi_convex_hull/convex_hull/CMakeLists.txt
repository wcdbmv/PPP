cmake_minimum_required(VERSION 3.25)
project(convex_hull
        LANGUAGES CXX)

set(HEADERS
    include/graham_scan.h
    include/orientation.h
    include/point.h
    include/point_cloud.h
    include/tangent.h)

set(SOURCES
    src/graham_scan.cc
    src/orientation.cc
    src/point.cc
    src/point_cloud.cc
    src/tangent.cc)

add_library(${PROJECT_NAME} STATIC
            ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME}
  PUBLIC include)

add_subdirectory(gen)
add_subdirectory(test)
