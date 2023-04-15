cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(zenoh-debug CXX)

add_subdirectory(external)

add_executable(
    zenoh-debug
    src/main.cpp
)

target_link_libraries(
    zenoh-debug PUBLIC
    external.zenoh-c
)
