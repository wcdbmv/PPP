cmake_minimum_required(VERSION 3.25)
project(utils
        LANGUAGES CXX)

set(HEADERS
    include/io.h)

set(SOURCES
    src/io.cc)

add_library(${PROJECT_NAME} STATIC
            ${HEADERS} ${SOURCES})

target_include_directories(${PROJECT_NAME}
  PUBLIC include)
