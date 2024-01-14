cmake_minimum_required(VERSION 3.25)
project(lab02
        LANGUAGES C)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME}
                      PRIVATE MPI::MPI_C)
