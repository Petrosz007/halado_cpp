cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

project(
    BiMap
    VERSION 0.0.1
    DESCRIPTION "BiMap"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -pedantic -Werror)


add_subdirectory(src)
add_subdirectory(extern/catch)
add_subdirectory(test)
