cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(zenoh-debug C)

add_compile_options(
    -Wall
    -Wextra
)

add_subdirectory(external)

add_executable(
    zenoh-debug
    src/main.c
)

target_link_libraries(
    zenoh-debug PUBLIC
    external.zenoh-c
)
