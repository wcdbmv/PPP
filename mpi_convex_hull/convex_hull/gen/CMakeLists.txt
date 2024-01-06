cmake_minimum_required(VERSION 3.25)
project(generate_point_cloud
        LANGUAGES CXX)

add_executable(${PROJECT_NAME} generate_point_cloud.cc)

target_link_libraries(${PROJECT_NAME}
                      PRIVATE convex_hull)
