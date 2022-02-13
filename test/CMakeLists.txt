add_executable(test_main test_main.cpp)
target_link_libraries(test_main PRIVATE catch bimap0)

option(COVERAGE "Enable coverage reporting" OFF)
if(COVERAGE)
    message("Code coverage enabled")
    target_compile_options(test_main PUBLIC -O0 -g --coverage)
    target_link_options(test_main PUBLIC --coverage)
endif(COVERAGE)

