#define CATCH_CONFIG_MAIN

#include <sut.hpp>

#if (BIMAP_VERSION >= 0)
#include <section0_tests.hpp>
#endif

#if (BIMAP_VERSION >= 1)
#include <section1_tests.hpp>
#endif
