add_library(section_tests STATIC sut.hpp section0_tests.hpp section1_tests.hpp)
target_include_directories(section_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(section_tests PUBLIC catch bimap0 bimap1)

add_executable(test_main test_main.cpp)
target_link_libraries(test_main PRIVATE catch section_tests)

option(COVERAGE "Enable coverage reporting" OFF)
if(COVERAGE)
  message("Code coverage enabled")
  target_compile_options(test_main PUBLIC -O0 -g --coverage)
  target_link_options(test_main PUBLIC --coverage)
endif(COVERAGE)

