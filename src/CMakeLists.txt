add_library(bimap0 STATIC ${CMAKE_SOURCE_DIR}/include/bimap0.hpp)
set_target_properties(bimap0 PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(bimap0 PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(bimap1 STATIC ${CMAKE_SOURCE_DIR}/include/bimap1.hpp)
set_target_properties(bimap1 PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(bimap1 PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_library(bimap2 STATIC ${CMAKE_SOURCE_DIR}/include/bimap2.hpp)
set_target_properties(bimap2 PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(bimap1 PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE bimap0 bimap1 bimap2)
