add_library(bimap0 STATIC ${CMAKE_SOURCE_DIR}/include/bimap0.hpp bimap0.cpp)
target_include_directories(bimap0 PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(bimap1 STATIC ${CMAKE_SOURCE_DIR}/include/bimap1.hpp bimap1.cpp)
target_include_directories(bimap1 PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE bimap0 bimap1)
