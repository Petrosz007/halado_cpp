project(
    Catch
    VERSION 2.13.8
)

add_library(catch INTERFACE)
target_include_directories(catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
