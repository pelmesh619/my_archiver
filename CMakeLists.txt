cmake_minimum_required(VERSION 3.12)

project(
        labwork6
        VERSION 1.0
        DESCRIPTION "C++ Year 2023 labwork5"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)
add_subdirectory(bin)

enable_testing()
add_subdirectory(tests)
add_executable(my_test my_test.cpp lib/hamming_coder.cpp)

add_executable(hamarc bin/main.cpp)
target_link_libraries(hamarc arg_parser hamming_coder archiver)
