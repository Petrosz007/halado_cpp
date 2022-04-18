add_library(original_tests STATIC sut.hpp original_tests/f0.hpp original_tests/f1.hpp original_tests/f2.hpp)
target_include_directories(original_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(original_tests PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(original_tests PUBLIC catch bimap0 bimap1 bimap2)

add_library(section_tests STATIC sut.hpp section0_tests.hpp section1_tests.hpp section2_tests.hpp)
target_include_directories(section_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(section_tests PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(section_tests PUBLIC catch bimap0 bimap1 bimap2 original_tests)

add_executable(test_main test_main.cpp)
set_target_properties(test_main PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(test_main PRIVATE catch section_tests)

option(COVERAGE "Enable coverage reporting" OFF)
if(COVERAGE)
  message("Code coverage enabled")
  target_compile_options(test_main PUBLIC -O0 -g --coverage)
  target_link_options(test_main PUBLIC --coverage)
endif(COVERAGE)

